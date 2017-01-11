//common functions useful around and about that don't have a place in any of
//the other headers
#include "common.h"

//i might replace this with sfmt one day
//but not today
//shamelessly pilfered from
//https://stackoverflow.com/questions/2999075/generate-a-random-number-within-range/2999130#2999130
int random(int min, int max) {
	int range = max - min;
	int divisor = RAND_MAX / (range + 1);
	int retval;
	do {
		retval = rand() / divisor;
	} while (retval > range);
	return retval + min;
}

float randf(float min, float max) {
	float range = max - min;
	return ((float)rand() / (float)RAND_MAX) * range + min;
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
//eg if you have a percent value and you want it to be a byte val
//you could call scale(val, 0.0, 1.0, 0.0, 255.0)
float scale(float val, float valmin, float valmax, float min, float max)
{
	float valscale = (val - valmin) / (valmax - valmin);
	return valscale * (max - min) + min;
}

//is `find` in `argv[]`?
//returns index if yes, -1 if no
int searchargs(int argc, char *argv[], char *find)
{
	int i;
	for(i = 0; i < argc; i++) {
		if(strcmp(argv[i], find) == 0) {
			return i;
		}
	}
	return -1;
}

struct limits getlimits(float *values, int length)
{
	struct limits ret;
	ret.min = ret.max = values[0];
	int i;
	for(i = 1; i < length; i++) {
		if(values[i] > ret.max) {
			ret.max = values[i];
			//printf("max: %f\ni: %d\n", values[i], i);
		} else if(values[i] < ret.min) {
			ret.min = values[i];
			//printf("min: %f\ni: %d\n", values[i], i);
		}
	}
	return ret;
}
