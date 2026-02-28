#include <math.h>
#include "wektor.h"

wektor add(wektor a, wektor b){
	wektor c;
	c.x=a.x+b.x;
	c.y=a.y+b.y;
	return c;
}

wektor mul(wektor a, float b){
	a.x=a.x*b;
	a.y=a.y*b;
	return a;
}

wektor sub(wektor a, wektor b){
	return add(a, mul(b,-1));
}

float len(wektor a){
	return sqrt(pow(a.y,2)+pow(a.x,2));
}

float get_angle(wektor a){
	if (len(a) > 0){
		if (a.y >= 0 ) {
			return asin(a.x/len(a));
		}else
			return M_PI - asin(a.x/len(a));
	}else {
		return 0;
	}
}

wektor rotate(wektor a, float angle){
	wektor c;
	float ang = get_angle(a);
	float length = len(a);
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
