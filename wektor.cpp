#include <cmath>
#include "wektor.hpp"

wektor operator*(float a, wektor b){
	return b*a;
}

wektor wektor::operator+(wektor a){
	wektor b;
	b.x = a.x+this->x;
	b.y = a.y+this->y;
	return b;
}

wektor wektor::operator+=(wektor a){
	*this = *this + a;
	return *this;
}

wektor wektor::operator-=(wektor a){
	*this = *this - a;
	return *this;
}

wektor wektor::operator*=(float a){
	*this = *this * a;
	return *this;
}

wektor wektor::operator/=(float a){
	*this = *this / a;
	return *this;
}

wektor wektor::operator* (float b){
	wektor a;
	a.x=this->x*b;
	a.y=this->y*b;
	return a;
}

wektor wektor::operator/ (float b){
	if (b != 0){
		wektor a;
		a.x=this->x/b;
		a.y=this->y/b;
		return a;
	}else {
		return *this;
	}
}

wektor wektor::operator-(){
	wektor b;
	b.x = 0-this->x;
	b.y = 0-this->y;
	return b;
	
}
wektor wektor::operator-(wektor a){
/*	wektor b;
	b.x = this->x-a.x;
	b.y = this->y-a.y;
	return b;
	*/
	return (*this+a*-1);
}

float wektor::len(){
	return sqrt(pow(this->y,2)+pow(this->x,2));
}

float wektor::get_angle(){
	if (this->len() > 0){
		if (x > y){
			if (this->y >= 0 ) {
				return asin(this->x/this->len());
			}else{
				return M_PI - asin(this->x/this->len());
			}
		}else{
			if (this->x >= 0 ) {
				return acos(this->y/this->len());
			}else{
				return - acos(this->y/this->len());
			}
		}
	}else {
		return 0;
	}
}

wektor wektor::rotate(float angle){
	float ang = this->get_angle();
	float length = this->len();
	ang += angle;
	//if (ang > 360) ang-=360;
	//if (ang < -360) ang+=360;
	this->x=sin(ang)*length;
	this->y=cos(ang)*length;
	return *this;
}

wektor rotate(wektor a, float angle){
	wektor c;
	float ang = a.get_angle();
	float length = a.len();
	ang += angle;
	//if (ang > 360) ang-=360;
	//if (ang < -360) ang+=360;
	c.x=sin(ang)*length;
	c.y=cos(ang)*length;
	return c;
}

wektor create(float angle, float length){
	wektor c;
	c.x = 0;
	c.y = length;
	return rotate(c, angle);
}

/*
float len(wektor a){
	return sqrt(pow(a.y,2)+pow(a.x,2));
}
*/
