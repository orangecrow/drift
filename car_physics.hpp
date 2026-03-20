#include "wektor.hpp"
#ifndef CAR_PHYSICS_HPP
#define CAR_PHYSICS_HPP

struct Object{
	wektor pos;
	wektor speed;
	float front_length;
	float back_length;
	float mass;
	float angular_inertia;
	float angular_speed;
	float direction_angle; 
	float steering_wheel;
};

struct Input{
	float steering_wheel;
	float gas;
	float hand_brake;
	float brake;
};

extern Object car;
extern int camera_mode;
extern line force1;
extern line force2;
extern line force3;

int apply_phiscs(Input in);



#endif
