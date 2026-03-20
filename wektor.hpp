#ifndef WEKTOR_HPP
#define WEKTOR_HPP

struct wektor{
	float x; 
	float y;
	wektor operator+(wektor a);
	wektor operator*(float b);
	wektor operator/(float b);
	wektor operator-(wektor a);
	wektor operator-();
	wektor operator+=(wektor a);
	wektor operator-=(wektor a);
	wektor operator*=(float a);
	wektor operator/=(float a);
	float len();
	float get_angle();
	wektor rotate(float angle);
};

wektor operator*(float a, wektor b);

wektor create(float angle, float length);

wektor rotate(wektor a, float angle);

struct line{
	wektor orgin;
	wektor end;
};

/*
float len(wektor a);
*/

#endif
