# this is my makefile 

CC = gcc

all: sdl_stuff.o wektor.o car_physics.o
	$(CC) sdl_stuff.o wektor.o car_physics.o -lm -lSDL3 -o a.out

sdl_stuff.o: sdl_stuff.c
	$(CC) sdl_stuff.c -c -o sdl_stuff.o

wektor.o: wektor.c
	$(CC) wektor.c -c -o wektor.o

car_physics.o: car_physics.c 
	$(CC) car_physics.c -c -o car_physics.o 

clean:
	cp a.out game.out
	rm *o a.out
