objects = main.o draw.o game_utility.o game_math.o game_data.o Matrix3x3.o Matrix4.o triangle.o \
		vector4.o vector3.o vector2.o game_string.o gamevf_vector.o gamevi_vector.o Camera.o \
		object.o 
h_files = headers/draw.h headers/game_utility.h headers/vector2.h headers/Matrix3x3.h  headers/Camera.h\
			headers/vector3.h
includes = `pkg-config --cflags --libs sdl2`
libs = -L usr/local/Cellar/sdl2/2.32.10/libs
CC = gcc -c -O2 -std=c11
LDFLAGS= -L/usr/local/opt/libomp/lib -lomp
CFLAGS= -I/usr/local/opt/libomp/include -Xpreprocessor -fopenmp 
#LDFLAGS += -L/usr/local/include -Xpreprocessor -fopenmp
#CFLAGS = -I/usr/local/lib -lomp

#For compilers to find libomp you may need to set:
#export LDFLAGS="-L/usr/local/opt/libomp/lib"
#export CPPFLAGS="-I/usr/local/opt/libomp/include"
FLAGS = -fopenmp


main : $(objects)
		gcc  -ggdb -o main $(objects) -I $(includes) $(LDFLAGS) $(CFLAGS)

main.o : main.c $(h_files)
		$(CC) main.c

vector4.o : source/vector4.c headers/vector4.h
			$(CC) source/vector4.c

vector3.o : source/vector3.c headers/vector3.h 
			$(CC) source/vector3.c 

vector2.o : source/vector2.c headers/vector2.h
			$(CC) source/vector2.c

draw.o : source/draw.c headers/draw.h headers/game_math.h headers/triangle.h headers/object.h \
		headers/Matrix3x3.h headers/Matrix4.h headers/game_utility.h headers/Camera.h
		$(CC) source/draw.c

game_utility.o : source/game_utility.c headers/game_utility.h
			$(CC) source/game_utility.c

game_math.o : source/game_math.c headers/game_math.h 
			$(CC) source/game_math.c

game_data.o : source/game_data.c headers/game_data.h
				$(CC) source/game_data.c

Matrix3x3.o : source/Matrix3x3.c headers/Matrix3x3.h headers/game_utility.h
			$(CC) source/Matrix3x3.c

Matrix4.o : source/Matrix4.c headers/Matrix4.h headers/Matrix3x3.h
			$(CC) source/Matrix4.c

triangle.o : source/triangle.c headers/triangle.h headers/vector3.h headers/Matrix3x3.h \
			headers/vector2.h
			$(CC) source/triangle.c

game_string.o : source/game_string.c headers/game_string.h
				$(CC) source/game_string.c

gamevf_vector.o : source/gamevf_vector.c headers/gamevf_vector.h headers/game_string.h
				$(CC) source/gamevf_vector.c

gamevi_vector.o : source/gamevi_vector.c headers/gamevi_vector.h headers/game_string.h
				$(CC) source/gamevi_vector.c

object.o : source/object.c headers/object.h
			$(CC) source/object.c

Camera.o : source/Camera.c headers/Camera.h headers/vector3.h
			$(CC) source/Camera.c

clean: 
	rm main $(objects)