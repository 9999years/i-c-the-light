//common functions useful around and about that don't have a place in any of
//the other headers
#include <math.h>
#include <stdlib.h>

#ifndef COMMON_H
#define COMMON_H

#define PI_SQ   9.86960440108F
#define TAU     6.28318530717F
#define PI      3.14159265359F
#define HALF_PI 1.57079632679F

//i might replace this with sfmt one day
//but not today
//shamelessly pilfered from
//https://stackoverflow.com/questions/2999075/generate-a-random-number-within-range/2999130#2999130
int random(int min, int max) {
	int range = max - min;
	int divisor = RAND_MAX/(range+1);
	int retval;
	do {
		retval = rand() / divisor;
	} while (retval > range);
	return retval + min;
}

//I Can't Believe It's Not Sin(x)
float fakesin(float x)
{
	x -= 1;
	return (-x * x) + 1;
}

float min(float a, float b)
{
	return a < b ? a : b;
}

float max(float a, float b)
{
	return a > b ? a : b;
}

float fclamp(float val, float fmin, float fmax)
{
	return min(max(val, fmin), fmax);
}

//interpolates between a and b
//b_interp is the percent of b to use
//eg b_interp = 1 --> 0% a, 100% b
//if b_interp is > 1 or < 0 you're gonna get some wacky nonsense
float lerp(float a, float b, float b_interp)
{
	return a * (1.0F - b_interp) + b * b_interp;
}

//ok, this one is kinda complicated to describe
//scales a number val (assumed to be in the range of valmin to valmax)
//to the equivalent fractional distance from min to max
//it's useful i swear
float scale(float val, float valmin, float valmax, float min, float max)
{
	float valscale = (val - valmin) / (valmax - valmin);
	return valscale * (max - min) + min;
}

#endif //COMMON_H
