//functions for dealing with quaternions
#include <math.h>
#include "logging.h"

#ifndef QUATERNION_H
#define QUATERNION_H

typedef struct quaternion quaternion;
struct quaternion
{
	float r;
	float a;
	float b;
	float c;
};

//debugging function
void dumpquaternion(quaternion in);

//construction method
quaternion constq(float r, float a, float b, float c);

//addition
quaternion addq(quaternion one, quaternion two);

//subtraction
quaternion subq(quaternion one, quaternion two);

//i don't really know what a conjugate is either semantically,
//practically, or at all in relationship to quaternions, so
//don't ask what this does. the algo itself is correct though.
quaternion conjugateq(quaternion in);

//multiplication
//this works
quaternion multq(quaternion one, quaternion two);

//scalar multiplication, aka multiplication by a number
//with only a real component
quaternion multqs(quaternion one, float two);

//various terms cancel so quaternion squaring is much simpler
quaternion sqrq(quaternion in);

//this signifies nothing but im just ripping it out of
//crane's implementation
float dotq(quaternion in);

//if `power` is negative, the behavior is undefined
quaternion powq(quaternion in, int power);

//length / magnitude / absolute value
float magnq(quaternion in);

//this is entirely empty. do not trust it
quaternion invq(quaternion in);

#endif //QUATERNION_H
