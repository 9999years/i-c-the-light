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

int frame = 0;
int realframe = 0;

void renderwhole(SDL_Surface *screen, int frame)
{
	float time = (float)frame/30;

	vec2 c, a;
	c.x = 80  + 15 * cos(time);
	c.y = 120 + 15 * sin(time);
	time += 1;
	a.x = 375 + 35 * cos(time);
	a.y = 400 + 35 * sin(time);

	const float threshold = 2.0F;
	vec2 point;
	float dist;

	int i, j;
	for(i = 0; i < screen->h; i++) {
	for(j = 0; j < screen->w; j++) {
		point.x = j;
		point.y = i;
		//if it's far out, skip ahead a bit
		dist = distline2(point, c, a);
		if(dist > threshold) {
			j += floor(dist - threshold);
			continue;
		}
		plot(
			screen,
			j, i,
			0xFFFFFF
			);
	}
	}
	return;
}

void render(SDL_Surface *screen, int frame, int *i, int *j)
{
	float time = (float)frame/30;
	SDL_FillRect(screen, NULL, 0x000000);
	
	renderwhole(screen, frame);

	vec2 c, a;
	//c.x = screen->w/2;
	//c.y = screen->h/2;// + 150 * sin((float)tick/512);
	c.x = 80  + 15 * cos(time);
	c.y = 120 + 15 * sin(time);
	time += 1;
	a.x = 375 + 35 * cos(time);
	a.y = 400 + 35 * sin(time);

	const float threshold = 2.0F;
	vec2 point;
	float dist;

	int k, l;

	for(k = 0; k <= *i; k++) {
	for(l = 0; l < screen->w; l++) {
		point.x = l;
		point.y = k;
		plot(
			screen,
			l, k,
			0x00AAAA
			);
		//if it's far out, skip ahead a bit
		dist = distline2(point, c, a);
		if(dist > threshold) {
			//plot(screen, j, i, 0xBB00BB);
			l += floor(dist - threshold);
			continue;
		}
		plot(
			screen,
			l, k,
			0xFFFFFF
			);
		if(k == *i && l > *j)
			break;
	}
	}
	point.x = *j;
	point.y = *i;
	plot(
		screen,
		*j, *i,
		0x00AAAA
		);
	dist = distline2(point, c, a);
	drawcircle(
		screen,
		*j, //center coords
		*i,
		floor(dist),
		0xAA0000
		);
	//if it's far out, skip ahead a bit
	if(dist > threshold) {
		drawline(
			screen,
			*j,
			*i,
			*j + floor(dist - threshold),
			*i,
			0xAA0000
			);
		*j += min(floor(dist - threshold), screen->w - *j);
		drawline(
			screen,
			*j,
			*i,
			*j - 5,
			*i - 5,
			0xAA0000
			);
		drawline(
			screen,
			*j,
			*i,
			*j - 5,
			*i + 5,
			0xAA0000
			);
		return;
	}
	drawline(
		screen,
		*j - 10,
		*i - 10,
		*j + 10,
		*i + 10,
		0xAA00AA
		);
	drawline(
		screen,
		*j + 10,
		*i - 10,
		*j - 10,
		*i + 10,
		0xAA00AA
		);
	drawcircle(screen, *j,     *i,     13, 0xAA00AA);
	drawcircle(screen, *j + 1, *i,     13, 0xAA00AA);
	drawcircle(screen, *j - 1, *i,     13, 0xAA00AA);
	drawcircle(screen, *j,     *i + 1, 13, 0xAA00AA);
	drawcircle(screen, *j,     *i - 1, 13, 0xAA00AA);
	plot(
		screen,
		*j, *i,
		0xFFFFFF
		);

	//nothing left to do if we’ve already hit the line
	//if(hits == sampsqr)
		//break;
	return;
}

void saveframe(SDL_Surface *screen)
{
	char filename[256] = "../output/seq1/UNINIT.ppm";
	sprintf(filename, "../output/seq1/image%d.ppm", realframe);
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
	sprintf(hashfilename, "../output/seq1/hash/%s", sha);

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
			} else if(event.key.keysym.sym == SDLK_k) {
				return 2;
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
	int i = 0;
	int j = 0;
	while(quit != 1) {
		// render
		SDL_Delay(50);
		render(screen, frame, &i, &j);

		//Update the surface
		SDL_UpdateWindowSurface(window);

		// poll for events, and handle the ones we care about.
		quit = handleevents(screen);
		if(quit == 2)
			j += 2500;

		//if(realframe == 0) {
			saveframe(screen);
		//}

		j++;
		if(j > screen->w) {
			i += max((j - screen->w) / screen->w, 1);
			j = 0;
			if(i > screen->h) {
				i = 0;
				frame++;
				quit = 1;
			}
		}
		realframe++;
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
