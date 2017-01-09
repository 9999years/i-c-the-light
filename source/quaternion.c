//functions for dealing with quaternions
#include "quaternion.h"

//debugging function
void dumpquaternion(quaternion in)
{
	fprintf(logfile,
		"    r: %.2f\n"
		"    a: %.2f\n"
		"    b: %.2f\n"
		"    c: %.2f\n",
		in.r, in.a, in.b, in.c
	);
	return;
}

//construction method
quaternion constq(float r, float a, float b, float c)
{
	return (quaternion){
		.r = r,
		.a = a,
		.b = b,
		.c = c
	};
}

//addition
quaternion addq(quaternion one, quaternion two)
{
	quaternion ret;
	ret.r = one.r + two.r;
	ret.a = one.a + two.a;
	ret.b = one.b + two.b;
	ret.c = one.c + two.c;
	return ret;
}

//subtraction
quaternion subq(quaternion one, quaternion two)
{
	quaternion ret;
	ret.r = one.r - two.r;
	ret.a = one.a - two.a;
	ret.b = one.b - two.b;
	ret.c = one.c - two.c;
	return ret;
}

//i don't really know what a conjugate is either semantically,
//practically, or at all in relationship to quaternions, so
//don't ask what this does. the algo itself is correct though.
quaternion conjugateq(quaternion in)
{
	quaternion ret;
	ret.r = in.r;
	ret.a = -1.0F * in.a;
	ret.b = -1.0F * in.b;
	ret.c = -1.0F * in.c;
	return ret;
}

//multiplication
//this works
quaternion multq(quaternion one, quaternion two)
{
	quaternion ret;
	ret.r =   one.r * two.r
		- one.a * two.a
		- one.b * two.b
		- one.c * two.c;

	ret.a =   one.r * two.a
		+ one.a * two.r
		+ one.b * two.c
		- one.c * two.b;

	ret.b =   one.r * two.b
		- one.a * two.c
		+ one.b * two.r
		+ one.c * two.a;

	ret.c =   one.r * two.c
		+ one.a * two.b
		- one.b * two.a
		+ one.c * two.r;
	return ret;
}

//scalar multiplication, aka multiplication by a number
//with only a real component
quaternion multqs(quaternion one, float two)
{
	quaternion ret;
	ret.r = one.r * two;
	ret.a = one.a * two;
	ret.b = one.b * two;
	ret.c = one.c * two;
	return ret;
}

//various terms cancel so quaternion squaring is much simpler
quaternion sqrq(quaternion in)
{
	quaternion ret;
	ret.r =   in.r * in.r
		- in.a * in.a
		- in.b * in.b
		- in.c * in.c;
	ret.a = 2 * in.r * in.a;
	ret.b = 2 * in.r * in.b;
	ret.c = 2 * in.r * in.c;
	return ret;
}

//this signifies nothing but im just ripping it out of
//crane's implementation
float dotq(quaternion in)
{
	return
		  in.r * in.r
		+ in.a * in.a
		+ in.b * in.b
		+ in.c * in.c;
}

//if `power` is negative, the behavior is undefined
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

//length / magnitude / absolute value
float magnq(quaternion in)
{
	return sqrt(
		  in.r * in.r
		+ in.a * in.a
		+ in.b * in.b
		+ in.c * in.c
	);
}

//this is entirely empty. do not trust it
quaternion invq(quaternion in)
{
	//DON'T USE THIS UNTIL I PUT CODE IN IT
	printf("WHAT THE FUCK ARE YOU DOING I SAID NOT TO CALL THIS FUNCTION\n");
	return in;
}
