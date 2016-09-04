//I C the Light: a distance-estimating ray marcher
//repo here: https://github.com/9999years/i-c-the-light
//MIT/expat license
//rebecca turner
//consult ../readme.md

//display
#include <SDL/SDL.h>
//logging, file out
#include <stdio.h>
#include <io.h>
//rand
#include <stdlib.h>
//sin functions
#include <math.h>
//timestamp for file names
#include <time.h>

//project files
#include "color.h"
#include "ppm.h"
#include "line.h"
#include "vector.h"
#include "complex.h"
#include "distance.h"

//Screen dimension constants

#define PI 3.1415926535
#define TAU 6.2831853071
#define HALF_PI 1.57079632675

//mandlebrot should be like 3:2
#define SCREEN_WIDTH  600
#define SCREEN_HEIGHT 400

#define BOX_WIDTH 50
#define BOX_HEIGHT 50
#define WIGGLE_AMP 20

//i might replace this with sfmt one day
//but not today
//shamelessly pilfered from
//https://stackoverflow.com/questions/2999075/generate-a-random-number-within-range/2999130#2999130
int random(int min, int max)
{
	int range = max - min;
	int divisor = RAND_MAX/(range+1);
	int retval;
	do {
		retval = rand() / divisor;
	} while (retval > range);
	return retval + min;
}

void render(SDL_Surface *screen, int frame)
{
	//int tick = SDL_GetTicks();
	//float time = (float)tick/200;

	SDL_FillRect(screen, NULL, 0x000000);

	vec2 c, a;
	c.x = -1.5F;
	c.y =  0.5F;
	a.x =  0.5F;
	a.y = -0.5F;

	const float zoom = (float)frame / 4;

	const complex center = {
		.a = -0.74364085F,
		.b =  0.13182733F
	};

	const float realmin = center.a - 2.0F / zoom;
	const float realmax = center.a + 1.0F / zoom;
	const float realrange = realmax - realmin;
	const float imin    = center.b - 1.0F / zoom;
	const float imax    = center.b + 1.0F / zoom;
	const float irange = imax - imin;

	int i, j;
	int k, l, hits;
#define SAMPLES 4
	const int sampsqr = SAMPLES * SAMPLES;
	const int scale = 0xff / sampsqr;
	const int iterations = 256;
	//const float thresh = 0.01F;
	const float thresh = 0.5F;
	float dist;
	vec2 point;
	//O(scary) but actually fine
	//no but seriously the deepest loop gets hit like 1.5 mil times
	for(i = 0; i < screen->h; i++) {
		for(j = 0; j < screen->w; j++) {
			point.x =
				((float)j / (float)screen->w)
				* realrange + realmin;
			point.y =
				((float)i / (float)screen->h)
				* irange + imin;
			dist = distline2(
				a, c, point
				);
			//dist = distmandlebrot(
					//point,
					//iterations
				//);
			//if(i % 25 == 0 && j % 25 == 0) {
				//printf("dist: %f\n", dist);
			//}
			//if it's far out, skip ahead a bit
			//if(dist > thresh) {
				//plot(
					//screen,
					//j, i,
					//colortoint(shifthue(
					//(struct rgbcolor){0xff, 0x88, 0x88},
					//i * 2 + j
					//))
					//);
				//j += floor(
					//((dist * 0.5F) / realrange) * (float)screen->w
					//);
				//continue;
			//}
			//for each pixel, multisample
			//hits = 0;
			//printf("PRE:\n    %f + %fi\n", point.a, point.b);
			//for(k = 0; k < SAMPLES; k++) {
				//for(l = 0; l < SAMPLES; l++) {
					//point.a += ((float)k /
						//(SAMPLES * screen->w))
						//* realrange;
					//point.b += ((float)l /
						//(SAMPLES * screen->h))
						//* irange;
					////printf("POST:\n    %f + %fi\n\n", point.a, point.b);
					//dist = distmandlebrot(point, iterations);
					//if(dist < thresh)
						//hits++;
				//}
			//}
			//dist = fabsf(dist);
			dist = fclamp(2048.0F * dist * zoom, 0.0F, 255.0F);
			dist = pow(dist, 0.25F);
			//if(dist > thresh)
				//continue;
			plot(
				screen,
				j, i,
				//0xFFFFFF
				colortoint(/*invertcolor*/(graytocolor(bclamp(
					128.0F * dist
					//hits * scale
				))))
				);
		}
	}
	//if(tick > 0)
	//printf("tick: %f\nV: %f\n", tick, V);
	return;
}

void saveframe(SDL_Surface *screenSurface)
{
	char filename[256] = "output/UNINITIALIZED.ppm";
	unsigned long int timeint = time(NULL);
	sprintf(filename, "../output/image%lu.ppm", timeint);
	if(
		writeppm(
			filename,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			screenSurface->pixels
			) != 0
	) {
		printf("image write error!\n");
		return;
	}
	char shacmd[256];
	sprintf(shacmd, "sha256sum %s", filename);
	FILE *file = popen(shacmd, "r");
	if(file == NULL) {
		printf("failed to get image hash!\n");
		return;
	}
	//the hash is 64 characters but we need a 0 at the end too
	char sha[68];
	int i;
	char c;
	for(i = 0; (i < 64) && (c != EOF) && (c != 0x20); i++) {
		sha[i] = c = fgetc(file);
	}
	sha[i++] = 0;
	printf("sha: %s\n", sha);
	pclose(file);

	char hashfilename[256];
	sprintf(hashfilename, "../output/hash/%s", sha);

	if(_access(hashfilename, 0) != -1) {
		//file exists, delete img
		if(unlink(filename) != 0) {
			printf("image delete error!\n");
		}
	} else {
		FILE *hashfile = fopen(hashfilename, "w");
		if(hashfile == NULL)
			printf("hash file write error!\nfilename: %s\n", hashfilename);
		fclose(hashfile);
	}
	return;
}

int handleevents(SDL_Surface *screenSurface)
{
	SDL_Event event;
	while((SDL_PollEvent(&event))) {
		switch(event.type) {
		case SDL_KEYUP:
			// if escape is pressed, quit
			if((event.key.keysym.sym == SDLK_ESCAPE) || (event.key.keysym.sym == SDLK_q)) {
				return 1;
			} else if(event.key.keysym.sym == SDLK_s) {
				saveframe(screenSurface);
			}
			break;

		case SDL_QUIT:
			return 1;
		}
	}
	return 0;
}

int WinMain(/*int argc, char* args[]*/)
{
	printf("hello!\n");
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	} else {
		//Create window
		window = SDL_CreateWindow("I C the Light", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		} else {
			//Get window surface
			screen = SDL_GetWindowSurface(window);

			//save
			render(screen);

			saveframe(screen);
		}
	}
	int quit = 0;
	int frame = 0;
	while (!quit) {
		start = clock();

		render(screen, frame);
		//autosave the first frame
		if(frame == 0)
			saveframe(screen);

		//Update the surface
		SDL_UpdateWindowSurface(window);

		// poll for events, and handle the ones we care about.
		quit = handleevents(screen);

		end = clock();
		if(frame%30 == 0) {
			total = (double)(end - start) / CLOCKS_PER_SEC;
			printf("%.4f FPS\n", 1 / total);
		}
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
