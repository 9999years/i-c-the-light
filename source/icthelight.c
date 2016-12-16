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
#include "common.h"
#include "ppm.h"
#include "line.h"
#include "vector.h"
#include "complex.h"
#include "distance.h"

int frame = 0;

//mandlebrot should be like 3:2
#define SCREEN_WIDTH  1200
#define SCREEN_HEIGHT 800

void render(SDL_Surface *screen)
{
	//float time = (float)frame / 5.0F;
	SDL_FillRect(screen, NULL, 0x000000);
	float width = 2.0F / ((float)frame * (float)frame);
	float aspect = (float)screen->w / (float)screen->h;
	float height = width / aspect;
	complex center = {
		.a = -0.743643135F,
		.b = 0.131825963F
	};

	const int iterations = 256 + 2 * frame;

	//const float threshold = 0.1F;
	complex point;
	float dist;
	int i, j;
	for(i = 0; i < screen->h; i++) {
	for(j = 0; j < screen->w; j++) {
		point.a = scale((float)j, 0, screen->w,
			center.a - width, center.a + width);
		point.b = scale((float)i, 0, screen->h,
			center.b - height, center.b + height);
		//if it's far out, skip ahead a bit
		dist = distmandlebrot(point, iterations);
		//if(dist > threshold) {
			//j += floor(dist - threshold);
			//continue;
		//} else
		if(dist > 0.0) {
			plot(
				screen,
				j, i,
				colortoint(graytocolor(0xff - bclamp(
					90000.0F * dist / width
					)))
				//colortoint(graytocolor(10.0F / fabsf(dist)))
				);
		} else {
			plot(
				screen,
				j, i,
				0xFFFFFF
				);
		}
	}
	}
	return;
}

void saveframe(SDL_Surface *screenSurface)
{
	char filename[256] = "output/UNINITIALIZED.ppm";
	//unsigned long int timeint = time(NULL);
	//sprintf(filename, "../seq2/image%lu.ppm", timeint);
	sprintf(filename, "../seq2/image%d.ppm", frame);
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
	//char shacmd[256];
	//sprintf(shacmd, "sha256sum %s", filename);
	//FILE *file = popen(shacmd, "r");
	//if(file == NULL) {
		//printf("failed to get image hash!\n");
		//return;
	//}
	////the hash is 64 characters but we need a 0 at the end too
	//char sha[68];
	//int i;
	//char c;
	//for(i = 0; (i < 64) && (c != EOF) && (c != 0x20); i++) {
		//sha[i] = c = fgetc(file);
	//}
	//sha[i++] = 0;
	//printf("sha: %s\n", sha);
	//pclose(file);

	//char hashfilename[256];
	//sprintf(hashfilename, "../output/hash/%s", sha);

	//if(_access(hashfilename, 0) != -1) {
		////file exists, delete img
		//if(unlink(filename) != 0) {
			//printf("image delete error!\n");
		//}
	//} else {
		//FILE *hashfile = fopen(hashfilename, "w");
		//if(hashfile == NULL)
			//printf("hash file write error!\nfilename: %s\n", hashfilename);
		//fclose(hashfile);
	//}
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
		}
	}
	//clock_t start, end;
	//double total;
	int quit = 0;
	while(quit != 1) {
		// render
		//SDL_Delay(50);
		render(screen);
		//if(frame == 0) {
			saveframe(screen);
		//}

		//Update the surface
		SDL_UpdateWindowSurface(window);

		// poll for events, and handle the ones we care about.
		quit = handleevents(screen);

		frame++;
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
