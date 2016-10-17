//functions for dealing with quaternions
#include <math.h>

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

quaternion constq(float r, float a, float b, float c)
{
	return (quaternion){
		.r = r,
		.a = a,
		.b = b,
		.c = c
	};
}

quaternion addq(quaternion one, quaternion two)
{
	quaternion ret;
	ret.r = one.r + two.r;
	ret.a = one.a + two.a;
	ret.b = one.b + two.b;
	ret.c = one.c + two.c;
	return ret;
}

quaternion subq(quaternion one, quaternion two)
{
	quaternion ret;
	ret.r = one.r - two.r;
	ret.a = one.a - two.a;
	ret.b = one.b - two.b;
	ret.c = one.c - two.c;
	return ret;
}

quaternion conjugateq(quaternion in)
{
	quaternion ret;
	ret.r = in.r;
	ret.a = -1.0F * in.a;
	ret.b = -1.0F * in.b;
	ret.c = -1.0F * in.c;
	return ret;
}

quaternion multq(quaternion one, quaternion two)
{
	quaternion ret;
	ret.r =   one.r * two.r
		- one.a * two.a
		- one.b * two.b
		- one.c * two.c;

	ret.a =   one.r * two.r
		+ one.a * two.a
		+ one.b * two.b
		- one.c * two.c;

	ret.b =   one.r * two.r
		+ one.a * two.a
		+ one.b * two.b
		- one.c * two.c;

	ret.c =   one.r * two.r
		+ one.a * two.a
		+ one.b * two.b
		- one.c * two.c;

	//ret.r =   one.r * two.c
		//+ one.a * two.b
		//- one.b * two.a
		//+ one.c * two.r;

	//ret.a =  -one.r * two.b
		//+ one.a * two.c
		//+ one.b * two.r
		//+ one.c * two.a;

	//ret.b =   one.r * two.a
		//- one.a * two.r
		//+ one.b * two.c
		//+ one.c * two.b;

	//ret.c =  -one.r * two.r
		//- one.a * two.a
		//- one.b * two.b
		//+ one.c * two.c;
	return ret;
}

quaternion sqrq(quaternion in)
{
	return multq(in, in);
}

//don't pass anything negative into power or some bad shit will happen
quaternion powq(quaternion in, int power)
{
	quaternion ret = in;
	quaternion tmp;
	int i;
	for(i = 0; i < power; i++) {
		tmp = multq(ret, in);
		ret = tmp;
	}
	return ret;
}

float magnq(quaternion in)
{
	return sqrt(
		  in.r * in.r
		+ in.a * in.a
		+ in.b * in.b
		+ in.c * in.c
		);
}

quaternion invq(quaternion in)
{
	//DON'T USE THIS UNTIL I PUT CODE IN IT
	return in;
}

#endif //QUATERNION_H
