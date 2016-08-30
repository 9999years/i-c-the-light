#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "trig.h"

	//float tp = 1.0/(2.0*PI);
	//x *= tp;
	//x -= (0.25) + floor(x + 0.25);
	//x *= (16.0) * (fabsf(x) - 0.5);
////#if EXTRA_PRECISION
	//x += 0.225 * x * (fabsf(x) - 1.0);
////#endif
	//return x;


int random(int min, int max) {
	int range = max - min;
	int divisor = RAND_MAX/(range+1);
	int retval;
	do {
		retval = rand() / divisor;
	} while (retval > range);
	return retval + min;
}

int main(int argc, char *argv[])
{
	char *filename = "./sin.dat";
	FILE *file = fopen(filename, "w");
	if(file == NULL) {
		printf("file open failure!\n");
		return 1;
	}
	int i;
	float theta;
#define DATA_POINTS 512
	for(i = 0; i < DATA_POINTS; i++) {
		//theta = ((float)random(0,1000) / 1000) * HALF_PI;
		theta = ((long double)i / DATA_POINTS) * HALF_PI;
		fprintf(
			file,
			"%.20f "
			"%.20f "
			//"%.20f "
			"\n",
			theta,
			//fcos(theta)
			sin(theta)
			);
	}
	fclose(file);
	return 0;
}
