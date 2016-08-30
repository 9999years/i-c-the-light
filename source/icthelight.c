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
#include "distance.h"

//Screen dimension constants
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

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

void render(SDL_Surface *screenSurface)
{

	int tick = SDL_GetTicks();

	SDL_FillRect(screenSurface, NULL, 0x000000);

	vec2 ca, cb, p;
	ca.x = screenSurface->w/2 + 150 * cos((float)tick/512);
	ca.y = screenSurface->h/2 + 150 * sin((float)tick/512);
	cb.x = screenSurface->w/2 - 150 * cos((float)tick/450 + 0.5F);
	cb.y = screenSurface->h/2 - 150 * sin((float)tick/450 + 1.5F);

	int i, j;
	float A, B, D;
	for(i = 0; i < screenSurface->h; i++) {
		for(j = 0; j < screenSurface->w; j++) {
			p.x = j;
			p.y = i;
			A = distcircle(p, ca, 250.0F);
			B = distcircle(p, cb, 150.0F);
			D = ops(B, A);
			//D = sindisplace2(p, D, 25, 40);
			if(abs(D) < 10)
			plot(screenSurface,
				j, i,
				//0xffffff
				colortoint(graytocolor(clamp(abs(100/D))))
				);
		}
	}
	//plot surface x y color

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
			screenSurface = SDL_GetWindowSurface(window);

			//save
			render(screenSurface);

			saveframe(screenSurface);
		}
	}
	int quit = 0;
	int frame = 0;
	while (!quit) {
		frame++;

		SDL_Delay(16);
		// render
		//if(frame%30 == 0)
		render(screenSurface);

		//Update the surface
		SDL_UpdateWindowSurface(window);

		// poll for events, and handle the ones we care about.
		quit = handleevents(screenSurface);
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
