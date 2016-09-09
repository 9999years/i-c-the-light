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
#include "common.h"
#include "color.h"
#include "ppm.h"
#include "line.h"
#include "vector.h"
#include "distance.h"

//Screen dimension constants
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

void render(SDL_Surface *screen)
{
	//int tick = SDL_GetTicks();
	SDL_FillRect(screen, NULL, 0x000000);

	/*
	* here's how im setting up the axis (right-handed)
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
	*
	*/

	//camera offset from origin
	//backwards 1000 units on the y axis
	vec3 camera_ofs = fromdirection3(-PI, 0.0F, 1000.0F);
	//camera rotation
	//pointing along y
	//this is the direction the rays will fire
	//if it isn't a unit vector things will explode probably
	vec3 camera_rot = fromdirection3(PI, 0.0F, 1.0F);
	//screen aspect ratio
	const float aspect = (float)screen->w / (float)screen->h;
	//size of area rays will be casted from in coord space
	//NOT screen pixels!!! that's `samples`
	vec2 camera_size;
	camera_size.x = 500.0F;
	//infer height from screen ratio
	camera_size.y = aspect * camera_size.x;
	//horiz samples
	//these are the values the for() loops go to
	const int xsamples = screen->w;
	const int zsamples = aspect * xsamples;
	//loop from 0 to samples
	//map each sample onto 0..xres, offset by camera_ofs
	//shoot in dir of camera_rot
	vec3 ray_ofs;
	vec3 ray_rot = camera_rot;
	//steps to march
	const int steps = 5;
	int i, j, k;
	for(i = 0; i < zsamples; i++) {
		for(j = 0; j < xsamples; j++) {
			//our position in the screen + camera offset
			ray_ofs.x =
				scale(i, 0, xsamples, 0, camera_size.x)
				+ camera_ofs.x;
			//plot(
				//screen,
				//j, i,
				//0xffffff
				//);
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
		}
	}
	int quit = 0;
	int frame = 0;
	clock_t start, end;
	double total;
	while(!quit) {
		start = clock();

		//SDL_Delay(16);
		// render
		render(screen);


		//Update the surface
		SDL_UpdateWindowSurface(window);

		// poll for events, and handle the ones we care about.
		quit = handleevents(screen);

		end = clock();
		if(frame%30 == 0) {
			if(frame == 0) {
				saveframe(screen);
			}
			total = (double)(end - start) / CLOCKS_PER_SEC;
			printf("%.4f FPS\n", 1 / total);
		}
		frame++;
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
