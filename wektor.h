#ifndef WEKTOR_H
#define WEKTOR_H

typedef struct wektor{
	float x; 
	float y;
} wektor;

wektor add(wektor a, wektor b);

wektor mul(wektor a, float b);

wektor sub(wektor a, wektor b);

float len(wektor a);

float get_angle(wektor a);

wektor rotate(wektor a, float angle);

wektor create(float angle, float length);

#endif
