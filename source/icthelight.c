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
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

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

void render(SDL_Surface *screen)
{

	int tick = SDL_GetTicks();
	float time = (float)tick/200;

	SDL_FillRect(screen, NULL, 0x000000);

	vec2 c, a, b;
	//c.x = screen->w/2;
	//c.y = screen->h/2;// + 150 * sin((float)tick/512);
	c.x = 80  + 15 * cos(time);
	c.y = 120 + 15 * sin(time);
	time += 1;
	a.x = 375 + 35 * cos(time);
	a.y = 400 + 35 * sin(time);

	int i, j;
	float dist;
	for(i = 0; i < screen->h; i++) {
		for(j = 0; j < screen->w; j++) {
			dist = distline2(a, c, (vec2){.x = j, .y = i});
			plot(
				screen,
				j, i,
				colortoint(graytocolor(clamp(
				100/dist
				)))
				);
		}
	}
	return;
}

void saveframe(SDL_Surface *screen)
{
	char filename[256] = "output/UNINITIALIZED.ppm";
	unsigned long int timeint = time(NULL);
	sprintf(filename, "../output/image%lu.ppm", timeint);
	if(
		writeppm(
			filename,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			screen->pixels
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

int handleevents(SDL_Surface *screen)
{
	SDL_Event event;
	while((SDL_PollEvent(&event))) {
		switch(event.type) {
		case SDL_KEYUP:
			// if escape is pressed, quit
			if((event.key.keysym.sym == SDLK_ESCAPE) || (event.key.keysym.sym == SDLK_q)) {
				return 1;
			} else if(event.key.keysym.sym == SDLK_s) {
				saveframe(screen);
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
	SDL_Surface* screen = NULL;

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
	clock_t start, end;
	double total;
	while (!quit) {
		start = clock();
		frame++;

		//SDL_Delay(16);
		// render
		render(screen);


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
