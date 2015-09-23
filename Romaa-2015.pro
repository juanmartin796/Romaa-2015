#-------------------------------------------------
#
# Project created by QtCreator 2015-06-22T22:19:23
#
#-------------------------------------------------

QT       += core gui

TARGET = Romaa_1.3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

#-------------------Librerías---------------------
INCLUDEPATH += "/usr/include/opencv"
INCLUDEPATH += "/usr/local/include/player-3.0"
LIBS += `pkg-config opencv --cflags --libs`
LIBS += -L/usr/local/lib64 -lplayerc++

CONFIG += extserialport
#-------------------------------------------------

 QT += widgets
