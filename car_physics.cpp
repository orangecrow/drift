#include "car_physics.hpp"
#include "wektor.hpp"
#include <cmath>
#include <iostream>

Object car ={{100,100},{0,0},30,30,20,2000,M_PI*3000/180,0,0};

int camera_mode = 1;
line force1 = {{0,0},{0,0}};
line force2 = {{0,0},{0,0}};
line force3 = {{200,200},{0,0}};

int apply_phiscs(Input in){
	const float tick_time = 0.0001;
	const float spring_constant = 100000;
	float max_static_friction_coefficient = 1;
	float max_dynamic_friction_coefficient = 0.8;
	wektor back_wheels_displacement;
	wektor front_wheels_displacement;
	static wektor previous_back_wheels_displacement = {0,0}; 
	static wektor previous_front_wheels_displacement = {0,0};
	wektor back_wheels_force;
	wektor front_wheels_force;
	wektor spring_force;
	wektor dampener_force;
	const float dampener_strentgh = 1000;
	static bool front_wheels_drift = 0;
	static bool back_wheels_drift;
	float vroom = in.gas * 100000;

	car.pos += car.speed * tick_time;
	car.direction_angle += car.angular_speed * tick_time;
	car.steering_wheel = - in.steering_wheel/360*10*2*M_PI;

	// back wheels
	back_wheels_displacement = car.speed ;
	back_wheels_displacement.rotate( -car.direction_angle );
	back_wheels_displacement.x -= car.angular_speed * car.back_length;
	back_wheels_displacement.y = 0;
	back_wheels_displacement.rotate( car.direction_angle );
	dampener_force = -back_wheels_displacement * dampener_strentgh;

	back_wheels_displacement += previous_back_wheels_displacement;
	previous_back_wheels_displacement = back_wheels_displacement ;

	back_wheels_displacement.rotate( -car.direction_angle );
	back_wheels_displacement.x -= car.angular_speed * car.back_length;
	back_wheels_displacement.y = 0;
	back_wheels_displacement.rotate( car.direction_angle );

	back_wheels_displacement *= tick_time;
	spring_force = -back_wheels_displacement * spring_constant;
	back_wheels_force = dampener_force + spring_force; 

	// front wheels
	front_wheels_displacement = car.speed ;
	front_wheels_displacement.rotate( -car.direction_angle );
	front_wheels_displacement.x += car.angular_speed * car.front_length;
	front_wheels_displacement.rotate( -car.steering_wheel );
	front_wheels_displacement.y = 0;
	front_wheels_displacement.rotate( car.direction_angle );
	front_wheels_displacement.rotate( car.steering_wheel );
	dampener_force = -front_wheels_displacement * dampener_strentgh;

	front_wheels_displacement += previous_front_wheels_displacement;
	previous_front_wheels_displacement = front_wheels_displacement ;
	front_wheels_displacement.rotate( -car.direction_angle );
	front_wheels_displacement.x += car.angular_speed * car.front_length;
	front_wheels_displacement.rotate( -car.steering_wheel );
	front_wheels_displacement.y = 0;
	front_wheels_displacement.rotate( car.direction_angle );
	front_wheels_displacement.rotate( car.steering_wheel );

	front_wheels_displacement *= tick_time;
	spring_force =  -front_wheels_displacement * spring_constant;
	front_wheels_force =  dampener_force + spring_force ;

	//temprorary way to add acceleration
	back_wheels_force.rotate(-car.direction_angle);
	back_wheels_force.y += vroom;
	back_wheels_force.rotate(car.direction_angle);

	// apply forces
	car.speed += back_wheels_force * tick_time / car.mass;
	car.speed += front_wheels_force * tick_time / car.mass;
	car.angular_speed += rotate(front_wheels_force, -car.direction_angle).x * car.front_length * tick_time / car.angular_inertia; 
	car.angular_speed -= rotate(back_wheels_force, -car.direction_angle).x * car.back_length * tick_time / car.angular_inertia; 

	force1.orgin = car.pos + create(car.direction_angle,30);
	force1.orgin += {15,30};
	force2.orgin = car.pos - create(car.direction_angle,30);
	force2.orgin += {15,30};
	force1.end = force1.orgin + front_wheels_force/1000;
	force2.end = force2.orgin + back_wheels_force/1000;

	static wektor strzalka = {0,0};
	static float angle = 0;

	if (front_wheels_drift == 0) {
		force3.end = force3.orgin + create( M_PI/2*1.1, 100);
		front_wheels_drift = 1;
		strzalka = - (force3.orgin - force3.end);
	}
	force3.end = force3.orgin + strzalka.rotate(M_PI/180*30*tick_time);
	/*
	force3.end = force3.orgin + create( M_PI/2*1.1, 100);
	strzalka = - (force3.orgin - force3.end);
	angle += tick_time;
	force3.end = force3.orgin + strzalka.rotate(angle);
	*/
	static int counter = 0;
	counter++;
	if (counter%(int)(1/tick_time/100) == 0 & counter < 1000) {
		std::cout << strzalka.x << "\n" << strzalka.y << "\n angle :" << strzalka.get_angle()<< "\n";
	}



	return 0;
}
