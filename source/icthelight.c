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
#include "quaternion.h"
#include "logging.h"

//Screen dimension constants
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

//globals
int frame = 0;
FILE *logfile;

//global distance estimator
float de(vec3 pos)
{
	//return distancejulia(pos, constq(-0.213F, -0.0410F, -0.563F, -0.560F));
	return
		//disttorus(pos, const3(50.0F, 0.0F, 50.0F), 5.0F, 15.0F);
		//distsphere(pos, const3(50.0F, 0.0F, 50.0F), 10.0F);
		opwobble3(
			pos,
			distsphere(pos, const3(50.0F, 0.0F, 50.0F), 15.0F),
			5.0F,
			5.0F
			);
		//distsphere(pos, sphere, 10.0F);
}

//returns a normal
//why does this work????
vec3 getnormal(vec3 pos, float samplesize)
{
	vec3 ret;
	vec3 xunit = {
		.x = samplesize,
		.y = 0.0F,
		.z = 0.0F
	};
	vec3 yunit = {
		.x = 0.0F,
		.y = samplesize,
		.z = 0.0F
	};
	vec3 zunit = {
		.x = 0.0F,
		.y = 0.0F,
		.z = samplesize
	};
	ret.x = de(add3(pos, xunit)) - de(sub3(pos, xunit));
	ret.y = de(add3(pos, yunit)) - de(sub3(pos, yunit));
	ret.z = de(add3(pos, zunit)) - de(sub3(pos, zunit));
	return unit3(ret);
}

//takes a light vector and a position vector
//the position vector is the distance between the camera and the point on the
//surface (vec V here:
//https://en.m.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model#Description
//can usually be found with )
unsigned int blinnphong(vec3 cam, vec3 pos, vec3 rot, vec3 light)
{
	//k_s: specular constant
#define k_s 1.0F
	//k_d: diffuse constant
#define k_d 0.5F
	//k_a: ambient constant
#define k_a 0.0F
	//α: shininess constant (larger = smaller highlight)
#define alpha 4.0F
	//i_s: specular intensity
#define specular_intensity 1.0F
	//i_d: diffuse intensity
#define diffuse_intensity 1.0F
	//i_a: ambient intensity
#define ambient_intensity 0.0F
	//L: light vector, from surface to light
	//N: normal
	//R: reflection vector
	//V: viewer vector
	//phong:
	//k_a * i_a + k_d * (L · N) * i_d + k_s * (R · V)^α * i_s
	//blinn-phong:
	//k_a * i_a + k_d * (L · N) * i_d + k_s * (N · H)^α * i_s
	cam = unit3(sub3(cam, pos));
	//pos = unit3(pos);
	//vec3 normal = getnormal(sub3(pos, mult3s(unit3(rot), 2.0F)), 10.0F);
	vec3 normal = getnormal(add3(pos, rot), 2.0F);
	//vec3 halfway = avg3(cam, light);
	vec3 halfway = unit3(add3(cam, light));
	float ret =
		dot3(light, normal) * diffuse_intensity
		+ pow(dot3(normal, halfway), alpha) * specular_intensity;
	return
		(unsigned int)(scale(ret, -2.0F, 2.0F, 0.0F, 255.0F));
}

void render(SDL_Surface *screen, int frame)
{
	float time = (float)frame / 3.0F;
	SDL_FillRect(screen, NULL, 0x000000);

	/*
	 * here's how im setting up the axes (right-handed)
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

	//screen aspect ratio
	const float aspect = (float)screen->w / (float)screen->h;
	//horiz samples
	//these are the values the for() loops go to
	const int wsamples = screen->w;
	const int hsamples = aspect * wsamples;

	//portion of viewport to render
	//this is multiplied by the viewport vectors to get a point on the viewport
	//the ray is shot through
	float wfrac, hfrac;

	//focal length of the camera
	//longer = more zoomed in
	float focallength = 100.0F;

	//width of the camera (horiz. line at the center of the viewport)
	vec3 viewport_width = const3(100.0F, 0.0F, 0.0F);
	//height of the camera (vert. line at the center of the viewport)
	vec3 viewport_height = const3(0.0F, 0.0F, 100.0F);

	//offset of the center of the viewport from the origin
	//essentially the camera position
	vec3 viewport_ofs = const3(0.0F, -1000.0F, 0.0F);

	//point the ray will travel through, other than the camera
	vec3 ray_through;

	//camera
	//the rays are shot from here through the viewport
	//it's backwards and perpendicular from the plane containing the viewport
	vec3 camera;
	camera = add3(
		viewport_ofs,
		mult3s(
			perp3(viewport_width, viewport_height),
			focallength
			)
		);

	//direction for the ray to travel in
	vec3 ray_rot;

	//the current position to be measured by the distance estimator
	//generally camera + distance in the direction of ray_rot
	vec3 measure_pos;

	//distance from measure_pos
	float distance;

	vec3 light = fromdirection3(time + 1.0F, time, 1.0F);
	//steps to march
	const int steps = 64;
	int i, j, k;
	for(i = 0; i < hsamples; i++) {
	for(j = 0; j < wsamples; j++) {
		//new sample, new distance
		distance = 0.0F;

		//what portion of the viewport are we rendering to?
		//-0.5 to 0.5 because viewport_ofs represents the center
		//of the viewport and viewport_width and _height represent
		//whole, not half widths of the viewport size
		wfrac = scale(j, 0, wsamples, -0.5F, 0.5F);
		hfrac = scale(i, 0, hsamples, -0.5F, 0.5F);
		ray_through = add3(
			mult3s(viewport_width, wfrac),
			mult3s(viewport_height, hfrac)
			);
		ray_rot = unit3(through3(camera, ray_through));

		for(k = 0; k < steps; k++) {
			measure_pos = add3(
				camera,
				distalong3(
					ray_rot,
					distance
					)
				);

			distance = de(measure_pos);
			fprintf(logfile, "step %d, distance: %f",
				k, distance);

			if(distance <= 0.05F) {
				fprintf(logfile, "PLOT!\n");
				plot(
					screen,
					j, i,
					colortoint(graytocolor(bclamp(
					k * 20
					//255.0F * (float)k / (float)steps
					//500.0F / distance
					//blinnphong(vec3 cam, vec3 pos, vec3 rot, vec3 light)
					//blinnphong(camera, measure_pos, ray_rot, light)
					//distance <= 2.0F ? 0xffffff : 0x000000
					)))
					);
				break;
			} else if(distance >= 100000.0F
				|| !isfinite(distance)) {
				//we're done here
				fprintf(logfile, "LIMIT EXCEEDED, BREAK!\n");
				break;
			}
		}
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
		fprintf(logfile, "image write error!\n");
		return;
	}
	char shacmd[256];
	sprintf(shacmd, "sha256sum %s", filename);
	FILE *file = popen(shacmd, "r");
	if(file == NULL) {
		fprintf(logfile, "failed to get image hash!\n");
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
	printf("SHA: %s\n", sha);
	fprintf(logfile, "SHA: %s\n", sha);
	pclose(file);

	char hashfilename[256];
	sprintf(hashfilename, "../output/hash/%s", sha);

	if(_access(hashfilename, 0) != -1) {
		//file exists, delete img
		if(unlink(filename) != 0) {
			fprintf(logfile, "image delete error!\n");
		}
	} else {
		FILE *hashfile = fopen(hashfilename, "w");
		if(hashfile == NULL)
			fprintf(logfile,
				"hash file write error!\nfilename: %s\n",
				hashfilename
				);
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
	printf("Hello!\n");
	const time_t unixtime = time(NULL);
	printf( "I C the Light by Rebecca Turner (MIT/Expat)\n"
		"%s\n", ctime(&unixtime)
		);

	initializelogfile();

	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen = NULL;

	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	} else {
		//Create window
		window = SDL_CreateWindow(
			"I C the Light",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN
			);
		if(window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		} else {
			//Get window surface
			screen = SDL_GetWindowSurface(window);
		}
	}
	int quit = 0;
	clock_t start, end;
	double total;
	while(!quit) {
		start = clock();

		//poll for events, and handle the ones we care about.
		//this returns 1 if we need to quit
		quit = handleevents(screen);

		//SDL_Delay(16);
		// render
		if(frame == 0)
			render(screen, frame);

		//Update the surface
		SDL_UpdateWindowSurface(window);

		end = clock();
		//if(frame%30 == 0) {
			if(frame == 0) {
				saveframe(screen);
			}
			total = (double)(end - start) / CLOCKS_PER_SEC;
			printf("%.4f FPS\n", 1 / total);
			fprintf(logfile, "%.4f FPS\n", 1 / total);
		//}
		frame++;
		//quit after first frame
		//just for debugging
		//quit = 1;
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
