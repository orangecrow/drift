# this is my makefile 

CC = g++

all: sdl_stuff.o wektor.o car_physics.o
	$(CC) sdl_stuff.o wektor.o car_physics.o -lm -lSDL3 -o drift

sdl_stuff.o: sdl_stuff.cpp
	$(CC) sdl_stuff.cpp -c -o sdl_stuff.o

wektor.o: wektor.cpp
	$(CC) wektor.cpp -c -o wektor.o

car_physics.o: car_physics.cpp
	$(CC) car_physics.cpp -c -o car_physics.o 

clean:
	rm *o drift
