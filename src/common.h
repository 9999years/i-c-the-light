//common functions useful around and about that don't have a place in any of
//the other headers
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMMON_H
#define COMMON_H

#define PI_SQ      9.869604401089358F
#define TAU        6.283185307179586F
#define PI         3.141592653589793F
#define HALF_PI    1.570796326794897F
#define QUARTER_PI 0.785398163397448F

struct limits {
	float min;
	float max;
} limits;

//i might replace this with sfmt one day
//but not today
//shamelessly pilfered from
//https://stackoverflow.com/questions/2999075/generate-a-random-number-within-range/2999130#2999130
int random(int min, int max);

float randf(float min, float max);

//I Can't Believe It's Not Sin(x)
float fakesin(float x);

float min(float a, float b);

float max(float a, float b);

float fclamp(float val, float fmin, float fmax);

//interpolates between a and b
//b_interp is the percent of b to use
//eg b_interp = 1 --> 0% a, 100% b
//if b_interp is > 1 or < 0 you're gonna get some wacky nonsense
float lerp(float a, float b, float b_interp);

//ok, this one is kinda complicated to describe
//scales a number val (assumed to be in the range of valmin to valmax)
//to the equivalent fractional distance from min to max
//eg if you have a percent value and you want it to be a byte val
//you could call scale(val, 0.0, 1.0, 0.0, 255.0)
float scale(float val, float valmin, float valmax, float min, float max);

//is `find` in `argv[]`?
//returns 1 if yes, 0 if no
int searchargs(int argc, char *argv[], char *find);

//useful for a long and short arg pair
int searchargspair(int argc, char *argv[], char *findone, char *findtwo);

struct limits getlimits(float *values, int length);

float averageArray(int length, float *arr);

#endif //COMMON_H
