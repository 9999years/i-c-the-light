//repo: https://github.com/9999years/i-c-the-light
//rebecca turner
#include "icthelight.h"
#include <math.h>

//global distance estimator
float de(vec2 pos, const int lframe, float r)
{
	float timef = (float)((float)lframe * TAU) / FRAMES_IN_ROTATION;
	vec2 a = const2(
		50.0F + 100.0F * cos(timef),
		50.0F + 100.0F * sin(timef)
	);
	timef += 0.5F;
	vec2 b = const2(
		300.0F + 150.0F * cos(timef),
		300.0F + 150.0F * sin(timef)
	);
	return distline2(pos, a, b) - r;
}

//returns a normal
//why does this work????
vec2 getnormal(vec2 pos, float samplesize)
{
	printf("GETNORMAL IS NOT A THING\n");
	return const2(0.0F, 0.0F);
	//vec2 ret;
	//vec2 xunit = {
		//.x = samplesize,
		//.y = 0.0F
	//};
	//vec2 yunit = {
		//.x = 0.0F,
		//.y = samplesize
	//};
	//ret.x = de(add2(pos, xunit), frame) - de(sub2(pos, xunit), frame);
	//ret.y = de(add2(pos, yunit), frame) - de(sub2(pos, yunit), frame);
	//return unit2(ret);
}

void render(SDL_Surface *screen, const int lframe)
{
	/* here's how im setting up the axes (right-handed)
	 *      z
	 *      |
	 *      |
	 *      |
	 *      |
	 *      |
	 *      +----------- y
	 *     /
	 *    /
	 *   /
	 *  /
	 * x
	 */

#define MAX_DISTANCE 10000.0F
#define MIN_DISTANCE 1.0F
#define BOUNDING_RADIUS 10000.0F

	printf("initializing render scene, allocating space\n");

	//const float timef = (float)(lframe * TAU) / FRAMES_IN_ROTATION;
	//unsigned long int timeint = time(NULL);
	//printf("time: %f for %lu\n", timef, timeint);
	//SDL_FillRect(screen, NULL, 0xffffff);
	SDL_FillRect(screen, NULL, 0x000000);

	//stores values to calculate colors
	//i can set exposure by figuring out the minimum/maximum of this array
	float *values = (float *)malloc(sizeof(float) * screen->w * screen->h);
	//the coordinates of matching keys in `values`, for plotting
	int *coords = (int *)malloc(sizeof(int) * screen->w * screen->h);
	//the length of both of those arrays
	int coordslen = 0;

	if(values == NULL) {
		printf("no memory for values!\n");
		return;
	}

	if(coords == NULL) {
		printf("no memory for keys!\n");
		return;
	}

	//ensure there's a minimum point for the exposure
	values[coordslen] = 0;
	coords[coordslen] = 0;
	coordslen++;

	//distance from measure_pos
	float distance;
	//total distance travelled, i hope i hope i hope
	//float totaldistance;

	//preview value
	float predist;

	//steps to march
	const int steps = lframe;

	printf("beginning render loop\n");

	int x, y, k;
	for(y = 0; y < screen->h; y++) {
		//fprintf(logfile, "NEW RAY!\n");
		//new sample, new distance
		distance = 0.0F;
		for(x = 0, k = 0; x < screen->w && k < steps; k++) {
			predist = de(
				const2(
					(float)x,
					(float)y
				), 0,
				20.0F
			);

			distance =
				fmin(predist, MAX_DISTANCE);

			//if((i % 10 == 0) && (j % 10 == 0))
				//fprintf(plotfile, "%d\t%d\t%f\n",
					//i, j, totaldistance);

			//fprintf(logfile, "step %d, distance: %f\n",
				//k, distance);

			if(distance <= MIN_DISTANCE) {
				//fprintf(logfile, "PLOT!\n");
				//printf("d: %f\n", totaldistance);
				//printf("k: %d\n", k);
				//vec3 norm;
				//norm = mult3s(getnormal(measure_pos, 0.2F), 255.0F);
				coords[coordslen] =
					x + y * screen->w;
				values[coordslen] =
					1.0F;
				coordslen++;
				x++;
			} else {
				coords[coordslen] =
					x + y * screen->w;
				values[coordslen] =
					//(float)k;
					0.5F;
				coordslen++;
				x += distance;
			}
		}
	}

	printf("done calculating array\n");

	struct limits limit = getlimits(values, coordslen);

	printf("done calculating limits (min: %.3f, max: %.3f)\n",
		limit.min, limit.max
	);

	int i;
	for(i = 0; i < coordslen; i++) {
		plot(
			screen,
			coords[i] % screen->w,
			coords[i] / screen->h,
			colortoint(graytocolor(bclamp(
				scale(values[i], limit.min, limit.max, 0, 255)
				//+ random(-5, 5)
			)))
		);
	}

			//colortoint(graytocolor(bclamp(k * 6)))
			//dot3(norm, light)
			//255.0F * (float)k / (float)steps
			//0.01F / (totaldistance + 0.00003F)
			//blinnphong(camera, measure_pos, ray_rot, light)
			//distance <= 2.0F ? 0xffffff : 0x000000

	free(values);
	free(coords);
	return;
}
