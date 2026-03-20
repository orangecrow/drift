#include "car_physics.h"
#include "wektor.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

object car ={{100,100},{0,0},1,1,2,2,0,0,0};

bool keys[4] = {0,0,0,0};
wektor line = {0,0};
wektor pointA = {0,-30};
wektor pointB;
wektor pointC = {0,30};
wektor pointD;

bool keys2[4] = {0,0,0,0};

int camera_mode = 2;

int apply_phiscs(){
	float friciotn_multiplier = 300;
	float max_static_force = 1 * friciotn_multiplier;
	float max_dynamic_force = 0.8 * friciotn_multiplier;
	float scale=1;
	float acceleration = 1;
	float force_multiplier = 3000; // I think this should not be touched
	wektor vroom = {0,0};
	static bool back_drift = false;
	static bool front_drift = false; 
	wektor force_front_wheels = {0,0};
	wektor force_back_wheels = {0,0};
	wektor previous_force_front_wheels = {0,0};
	wektor previous_force_back_wheels = {0,0};
	static int i = 0;
	int time_slow_down=1;
	i++;

	// time test
	static int test = 0;
	static float secsElapsed = 1 ;
	static clock_t startTime = 0; 
	if (test == 1){
		secsElapsed = (float)(clock() - startTime)/CLOCKS_PER_SEC;
		test++;
		printf("\n%f\n", secsElapsed);
	}
	if (test == 0){
		startTime = clock();
		test++;
	}
	if (test == 2){
		secsElapsed = 0.001;
	}

	if ( i % time_slow_down== 0){

		//while( car.direction_angle > 360 ) car.direction_angle -= 360;
		//while( car.direction_angle < -360 ) car.direction_angle -= 360;

		//apply movement
		car.pos = add(car.pos,mul(car.speed,secsElapsed));
		car.direction_angle += car.angular_speed*secsElapsed;

		car.steering_wheel = 0;
		if ( keys[0] ) {
			//car.speed = add(car.speed , create(car.direction_angle, acceleration));
			vroom = create(car.direction_angle, acceleration);
		}
		if ( keys[1] ) {
			//car.speed = sub(car.speed , create(car.direction_angle, acceleration));
			vroom = create(car.direction_angle, -acceleration);
		}
		if ( keys[2] ){ 
			car.steering_wheel -= 10.0/360 * 2 * M_PI;
			//car.direction_angle= - M_PI/6;
		}
		if ( keys[3] ){ 
			car.steering_wheel += 10.0/360 * 2 * M_PI;
			//car.direction_angle= M_PI/6;
		}
		if(!keys[3] && !keys[2]){
			//car.steering_wheel = 0;
		}

		//calculate force wektors
		wektor back_wheel_speed = add (car.speed, create(car.direction_angle - M_PI/2, 1*car.angular_speed*car.back_length));
		force_back_wheels.x = rotate(back_wheel_speed, -car.direction_angle).x*secsElapsed;
		force_back_wheels = add(force_back_wheels,previous_force_back_wheels);
		force_back_wheels.y = 0;
		force_back_wheels = rotate(force_back_wheels, car.direction_angle);
		force_back_wheels = mul(force_back_wheels, -force_multiplier);
		//force_back_wheels = mul(force_back_wheels, len(force_back_wheels));
		force_back_wheels = add(force_back_wheels, vroom);
		//force normalization:
		if (len(force_back_wheels) > max_static_force) back_drift = true;
		if (len(force_back_wheels) < max_dynamic_force) back_drift = false;
		if (back_drift){
			scale = max_static_force /len(force_back_wheels)*(max_dynamic_force)/friciotn_multiplier;
			force_back_wheels=mul(force_back_wheels, scale);
		}
		previous_force_back_wheels = mul(force_back_wheels , 0.9) ;

		//now front
		wektor front_wheel_speed = add (car.speed, create(car.direction_angle + M_PI/2, 1*car.angular_speed*car.front_length));
		force_front_wheels.x = rotate(front_wheel_speed, -(car.steering_wheel+car.direction_angle)).x*secsElapsed;
		force_front_wheels = add(force_front_wheels,previous_force_front_wheels);
		force_front_wheels.y = 0;
		force_front_wheels = rotate(force_front_wheels, (car.steering_wheel+car.direction_angle));
		force_front_wheels = mul(force_front_wheels, -force_multiplier);
		//force_front_wheels = mul(force_front_wheels, len(force_front_wheels));
		//force normalization:
		if (len(force_front_wheels) > max_static_force) front_drift = true;
		if (len(force_front_wheels) < max_dynamic_force) front_drift = false;
		if (front_drift){
			scale = max_static_force /len(force_front_wheels)*(max_dynamic_force)/friciotn_multiplier;
			force_front_wheels=mul(force_front_wheels, scale);
		}
		previous_force_front_wheels = mul(force_front_wheels , 0.9);

		//apply_force
		car.speed = add(car.speed, mul ( add(force_back_wheels, force_front_wheels), secsElapsed / car.mass ));

		//apply_moment_back
		car.angular_speed += rotate(force_back_wheels, -car.direction_angle).x * -car.back_length / 2.0 /car.angular_inertia * secsElapsed;

		//apply_moment_front
		car.angular_speed += rotate(force_front_wheels, -car.direction_angle).x * car.front_length / 2.0 /car.angular_inertia * secsElapsed;
		//line=force_front_wheels;


		if (camera_mode == 1){
			pointA=add(car.pos,create(car.direction_angle,car.front_length));
			pointB=add(add(car.pos,create(car.direction_angle,car.front_length)),mul(force_front_wheels,1));
			pointC=add(car.pos,create(car.direction_angle, -car.back_length));
			pointD=add(add(car.pos,create(car.direction_angle, -car.back_length)),mul(force_back_wheels,1));
		}
		if (camera_mode == 2){
			pointA=create(car.direction_angle,car.front_length);
			pointB=add(create(car.direction_angle,car.front_length),mul(force_front_wheels,1));

			pointC=create(car.direction_angle, -car.back_length);
			pointD=add(create(car.direction_angle, -car.back_length),mul(force_back_wheels,1));
		}

/*
		printf("-----------------------\n");
		printf("calculation: %f\n", rotate(force_front_wheels, -car.direction_angle).x );
		printf("pos.x: %f\n", car.pos.x);
		printf("pos.y: %f\n", car.pos.y);
		printf("speed.x: %f\n", car.speed.x);
		printf("speed.y: %f\n", car.speed.y);
		printf("back_length: %f\n", car.back_length);
		printf("front_length: %f\n", car.front_length);
		printf("mass: %f\n", car.mass);
		printf("angular_inertia: %f\n", car.angular_inertia);
		printf("angular_speed: %f\n", car.angular_speed);
		printf("direction_angle: %f\n", car.direction_angle);
		printf("steering_wheel: %f\n", car.steering_wheel);
		printf("speed angle: %f\n", get_angle(car.speed));
		printf("-----------------------\n");
		*/

		//experimental auto drift
		static int step = 0;
		if (i == 1*time_slow_down) keys2[0]=1;
		if (i == 150*time_slow_down) keys2[0]=0;
		if (i == 150*time_slow_down) keys2[3]=1;
		if (step == 0 && car.direction_angle > M_PI/2){
			keys2[3]=0;
			step = 1;
		}
		if ( step == 1 ) {
			keys2[0]= 1; // engine start
			keys2[3]= 1; // turn left
		}
		if ( step == 1 && i > 700*time_slow_down ) step = 2;
		//Now we should get to the middle


		float max_angular_speed = M_PI/36/4;
		float min_angular_speed = max_angular_speed - M_PI/360;
		//printf("angular speed: %f\n", car.angular_speed/max_angular_speed);
		if(step == 2 && car.angular_speed > max_angular_speed){
			keys2[0]=0; // engine stop
			keys2[3]=0; // don't turn left
			keys2[2]=1; // turn right
		}
		if(step == 2 && car.angular_speed < max_angular_speed){
			keys2[0]=1; // engine start
		}
		if(step == 2 && car.angular_speed < min_angular_speed){
			keys2[2]=0; // don't turn right 
		}else if( step == 2 ){
			keys2[2]=1; // turn right
		}

		//direction angle normalization
		if (car.direction_angle > 2 * M_PI) car.direction_angle-=2*M_PI;
		if (car.direction_angle < - 2 * M_PI) car.direction_angle+=2*M_PI;


		// make car stay on screen 
		/*
		if (car.pos.x > WINDOW_WIDTH) car.pos.x-=WINDOW_WIDTH;
		if (car.pos.y > WINDOW_HEIGHT) car.pos.y-=WINDOW_HEIGHT;
		if (car.pos.x < 0) car.pos.x+=WINDOW_WIDTH;
		if (car.pos.y < 0) car.pos.y+=WINDOW_HEIGHT;
		*/
	}

	return 0;
}
