objects = main.o draw.o game_utility.o game_math.o game_data.o
h_files = headers/draw.h headers/game_utility.h headers/vector2.h
includes = `pkg-config --cflags --libs sdl2`
libs = -L usr/local/Cellar/sdl2/2.32.10/libs
CC = gcc -c


main : $(objects)
		gcc  -v -o main $(objects) -I $(includes) 

main.o : main.c $(h_files)
		$(CC) main.c

vector2.o : source/vector2.c headers/vector2.h 
			$(CC) source/vector2.c 

draw.o : source/draw.c headers/draw.h headers/game_math.h
		$(CC) source/draw.c

game_utility.o : source/game_utility.c headers/game_utility.h
			$(CC) source/game_utility.c

game_math.o : source/game_math.c headers/game_math.h 
			$(CC) source/game_math.c

game_data.o : source/game_data.c headers/game_data.h
				$(CC) source/game_data.c
clean: 
	rm main $(objects)