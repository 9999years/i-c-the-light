#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
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
	char *filename = "./sintest.dat";
	FILE *file = fopen(filename, "w");
	if(file == NULL) {
		printf("file open failure!\n");
		return 1;
	}
	clock_t start, end;
	double total;
	int i;
	float theta, sinval;
//#define DATA_POINTS 921600
#define DATA_POINTS 64
	//printf("testing 460,800 (1280*720/2) values of sin()\n");
	start = clock();
	for(i = -DATA_POINTS; i < DATA_POINTS; i++) {
		//-4 to 4
		//theta = ((float)random(-1024, 1024) / 256) * HALF_PI;
		theta = ((long double)i / DATA_POINTS) * TAU;
		//sinval += fsin(theta);
		//printf("sin(%.8f) = %.8f\n", theta, sinval);
		fprintf(
			file,
			"%.20f "
			"%.20f"
			//"%.20f "
			"\n",
			theta,
			fsin(theta)
			);
	}
	end = clock();
	total = (double)(end - start) / CLOCKS_PER_SEC;
	printf("time taken: %f\n(%.4f FPS)", total, 1 / total);
	//fclose(file);
	return 0;
}
