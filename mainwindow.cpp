#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "cv.h" // incluye las definiciones de OpenCV
#include <opencv/cv.h>
//#include "cvaux.h" // Stuff.
#include <opencv/cvaux.h>
//#include "highgui.h" // incluye highGUI. Necesario para crear ventanas de visualizacion
#include <opencv/highgui.h>
#include "iostream"
#include "qextserialport.h"

#include <libplayerc++/playerc++.h>
#include <unistd.h>
#include<qprocess.h>

using namespace std;
using namespace PlayerCc;

PlayerClient    robot("localhost", 6665);
Position2dProxy p2dProxy(&robot,0);


bool prendido=false;
bool finTramo1=false;

QextSerialPort *port;

void MainWindow::onDataAvailable(){
    cout << "Evento!!" << endl;
    QByteArray data = port->readAll();
    if(data.length() > 0){
        cout << "Recibido: " << endl;
        for(int i = 0; i < data.length(); i++){
            cout << data.at(i) << endl;
            if(data.at(i)=='I'){
                finTramo1=true;
            }
        }
        cout << "Fin recibido" << endl;
    }
}

void etapa2(){
    CvCapture *capture = NULL;
    capture = cvCreateCameraCapture(0);

    if(!capture){
        cout<<"error"<<endl;
    }

    IplImage* frame;

    frame = NULL;
    cvNamedWindow( "Ventana del frame", CV_WINDOW_FREERATIO );

    int x,y,i,j;
    int detecta=0;
    int punto =0;
    CvScalar s;

    while(1) {
        frame = cvQueryFrame( capture );//Grabs the frame from a file
        if( !frame ) break;

        cvFlip(frame, frame, 1);

        while (true){
            x= rand() % (frame->width-25);
            y= rand() % (frame->height-25);

            detecta++;
            if (x>=1 && y>=1){
                for (i=-1; i<=1; i++){
                    for (j=-1; j<=1; j++){
                        s= cvGet2D(frame, y-j, x-i);
                        //detecta el color rojo
                        if ((int)s.val[0]<=80 && (int)s.val[1]>=160 && (int)s.val[2]>=160){ //colores en orden BGR
                            cvCircle(frame, cvPoint(x-i,y-j),10, CV_RGB(0,255,255), CV_FILLED, CV_AA,0);
                            punto++;
                        }
                    }
                }
            }
            if (punto==10) {break;}
            if (detecta==10000) {break;}
        }


        //*************************** Si detecta la señal de STOP, manda un ascci para que se abra la puerta *******************************************

        if (punto==10){
            port->putChar('a');
            cout<<"abrir la puerta"<<endl;
            sleep(2);
            p2dProxy.SetSpeed(0.5,0);
        } else {
            p2dProxy.SetSpeed(0.5,0);
        }

        //*************************************************************************************************************

        punto=0;
        detecta=0;

        cvShowImage("Ventana del frame", frame);

        char c=cvWaitKey(33);
        if( c == 27 ) break;
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /*QProcess process;
    process.start("player", QStringList() << "/home/juanmartin/Dropbox/Proyectos GUDA/Romaa 2015/Stage/empty.cfg");
    sleep(3);*/


    port = new QextSerialPort("/dev/ttyACM0");
    port->setBaudRate(BAUD9600);
    port->setQueryMode(QextSerialPort::EventDriven);
    port->open(QIODevice::ReadWrite);
    port->flush();
    port->putChar('a');
    connect(port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));

    //**********************************************************************************
    //          Codigo para hacer mover al romaa para adelante
    //**********************************************************************************

    //PlayerClient    robot("localhost", 6665);
    //Position2dProxy p2dProxy(&robot,0);
    p2dProxy.SetMotorEnable(1);
    //p2dProxy.SetSpeed(0.5, 0);
    //sleep(5); //va a andar por 5 segundos;


    //*********************************************************************************
    //          Codigo  del filtro de particulas detectando el color rojo en el sig IF:
    //          if ((int)s.val[0]<=80 && (int)s.val[1]<=80 && (int)s.val[2]>=160){
    //***********************************************************************************


    CvCapture *capture = NULL;
    capture = cvCreateCameraCapture(1);

    if(!capture){
        cout<<"error"<<endl;
    }

    IplImage* frame;

    frame = NULL;
    //IplImage* gray_frame = NULL ;

    //cvNamedWindow( "Centro de masa", CV_WINDOW_AUTOSIZE );
    cvNamedWindow( "Ventana del frame", CV_WINDOW_FREERATIO );

    int x,y,i,j;
    int detecta=0;
    int punto =0;
    int puntoAzul =0;
    CvScalar s;

    int sumPosX, sumPosY, sumPosObstX, sumPosObstY;
    double posRepresX, posRepresY, posRepresObstX, posRepresObstY;
    bool obstaculo;

    while(1) {
        frame = cvQueryFrame( capture );//Grabs the frame from a file
        //cvCircle(frame, cvPoint(200,200),100, CV_RGB(0,0,255), CV_FILLED, CV_AA,0);
        if( !frame ) break;

        cvFlip(frame, frame, 1);

        sumPosX=0;
        sumPosY=0;
        sumPosObstX=0;
        sumPosObstY=0;
        while (true){
            x= rand() % (frame->width-25);
            y= rand() % (frame->height-25);

            detecta++;
            if (x>=1 && y>=1){
                for (i=-1; i<=1; i++){
                    for (j=-1; j<=1; j++){
                        s= cvGet2D(frame, y-j, x-i);
                        //detecta el color rojo
                        if ((int)s.val[0]<=80 && (int)s.val[1]<=80 && (int)s.val[2]>=160){ //colores en orden BGR
                            cvCircle(frame, cvPoint(x-i,y-j),10, CV_RGB(0,255,255), CV_FILLED, CV_AA,0);
                            sumPosX+=x-i;
                            sumPosY+=y-j;
                            punto++;
                        }
                        //Detecta el color azul
                        if ((int)s.val[0]>=160 && (int)s.val[1]<=80 && (int)s.val[2]<=80){ //colores en orden BGR
                            cvCircle(frame, cvPoint(x-i,y-j),10, CV_RGB(255,0,255), CV_FILLED, CV_AA,0);
                            sumPosObstX+=x-i;
                            sumPosObstY+=y-j;
                            puntoAzul++;
                        }
                    }
                }
            }
            if (punto==10) {break;}
            if (detecta==10000) {break;}
        }

        cvCircle(frame, cvPoint(frame->width/2-30,240),10, CV_RGB(255,255,255), CV_FILLED, CV_AA,0); //puntos de referencia centro vertical
        cvCircle(frame, cvPoint(frame->width/2+30,240),10, CV_RGB(255,255,255), CV_FILLED, CV_AA,0); //puntos de referencia centro vertical

        //*************************** Se mueve cuando es detectado por la camara         *******************************************
        //*************************** Hace que vaya siempre por el medio vertical de la pantalla. *********************************************

        if (punto!=0 && obstaculo==false){
            posRepresX= sumPosX/punto;
            posRepresY= sumPosY/punto;
            cvCircle(frame, cvPoint(posRepresX,posRepresY),10, CV_RGB(255,255,0), CV_FILLED, CV_AA,0);
            if (posRepresX <= frame->width/2-30){
                p2dProxy.SetSpeed(0.5, DTOR(-20)); //20 grados convertidos en radianes
            } else if (posRepresX >= frame->width/2+30){
                p2dProxy.SetSpeed(0.5, DTOR(20)); //20 grados convertidos en radianes
            } else {
                p2dProxy.SetSpeed(0.5, 0);
            }
            prendido=true;
        }

        //****************************Si encuentra un obstaculo a una distancia menor a 200 px se detiene ******************************
        if (puntoAzul!=0){
            posRepresObstY= sumPosObstY/puntoAzul;
            if (puntoAzul>5 && punto!=0 && posRepresY- posRepresObstY<=200){
                obstaculo=true;
            } else {
                obstaculo= false;
            }
        }else{
            obstaculo=false;
        }

        //*************************************** Si no se detecta el robot, hace que pare, y no se mueva ********************************************************

        if ( ( detecta==10000 && prendido==true ) || obstaculo==true ){
            p2dProxy.SetSpeed(0,0);
            //port->putChar('c');
            prendido=false;
        }
        //*************************************************************************************************************

        punto=0;
        puntoAzul=0;
        detecta=0;

        cvShowImage("Ventana del frame", frame);

        if (finTramo1==true){
            p2dProxy.SetSpeed(0, 1);
            sleep(1);
            p2dProxy.SetSpeed(0,0);
            break;
        }

        char c=cvWaitKey(33);
        if( c == 27 ) break;
    }

    etapa2();

    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}
