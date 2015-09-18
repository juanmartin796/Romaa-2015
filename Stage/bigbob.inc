define bigbob position
(
	size [1.25 1 1]

	#centro de rotacion del robot
        origin [0.0 0.0 0.5 0]

	drive "diff"

	# Definimos el cuerpo del robot
	block
	(
		points 6 		#tiene 6 vertices el cuerpo del robot
		point[5] [0 0]		#define los puntos de cada vertice en coordenadas x e y
		point[4] [0 1]
		point[3] [0.75 1]
		point[2] [1 0.75]
		point[1] [1 0.25]
		point[0] [0.75 0]
		z [0 1]			#define la altura del robot

		)
	block
	(
		points 4
		point[3] [1 0.75]
		point[2] [1.25 0.75]
		point[1] [1.25 0.625]
		point[0] [1 0.625]
		z [0 0.5]
	)
	block
	(
		points 4
		point[3] [1 0.375]
		point[2] [1.25 0.375]
		point[1] [1.25 0.25]
		point[0] [1 0.25]
		z [0 0.5]
	)

	bigbobs_sonars()
)

define bigbobs_sonars ranger
(
	# Numero de sonares
	scount 4

	# define la posicion de cada sonar [xpos ypos angulo]
	spose[0] [ 0.75 0.1875 0 ]
	spose[1] [ 0.75 -0.1875 0 ]
	spose[2] [ 0.25 0.5 30]
	spose[3] [ 0.25 -0.5 -30]

)
