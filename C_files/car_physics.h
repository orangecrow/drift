#include "wektor.h"
#include <math.h>
#ifndef CAR_PHYSICS_H
#define CAR_PHYSICS_H
extern bool keys[];

typedef struct object{
	wektor pos;
	wektor speed;
	float front_length;
	float back_length;
	float mass;
	float angular_inertia;
	float angular_speed;
	float direction_angle; 
	float steering_wheel;
} object;

extern object car;

int apply_phiscs();

extern int camera_mode;

extern wektor pointA;
extern wektor pointB;
extern wektor pointC;
extern wektor pointD;

#endif
