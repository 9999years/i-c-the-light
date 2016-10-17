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
	return distancejulia(pos, constq(-0.213F, -0.0410F, -0.563F, -0.560F));
	//return disttorus(pos, const3(50.0F, 0.0F, 50.0F), 5.0F, 15.0F);
		//distsphere(pos, const3(50.0F, 0.0F, 50.0F), 10.0F);
		//opwobble3(
			//pos,
			//5.0F,
			//5.0F
			//);
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
	//vec3 camera_ofs = fromdirection3(time, 0.0F, 1000.0F);
	vec3 camera_ofs = {
		.x = 250.0F,
		.y = -1000.0F + 250.0F * sin(time),
		.z = 250.0F
	};
	//camera rotation
	//pointing along y
	//this is the direction the rays will fire
	//if it isn't a unit vector things will explode probably
	vec3 camera_rot = unit3(inv3(camera_ofs));
	//vec3 camera_rot = fromdirection3(time, time + 1.0F, 1.0F);
	//vec3 camera_rot = {
		//.x = sin(time),
		//.y = cos(time),
		//.z = 0.0F
	//};
	//screen aspect ratio
	const float aspect = (float)screen->w / (float)screen->h;
	//size of area rays will be casted from in coord space
	//NOT screen pixels!!! that's `samples`
	vec2 camera_size;
	camera_size.x = 100.0F;
	//infer height from screen ratio
	camera_size.y = aspect * camera_size.x;
	//horiz samples
	//these are the values the for() loops go to
	const int xsamples = screen->w;
	const int zsamples = aspect * xsamples;
	//loop from 0 to samples
	//map each sample onto 0..xres, offset by camera_ofs
	//shoot in dir of camera_rot
	//origin of each ray, where it fires from
	vec3 ray_orig;
	//unit vector of the direction to go into
	vec3 ray_rot = camera_rot;
	//actual position of the point being measured currently,
	//RELATIVE TO RAY_ORIG
	vec3 ray_ofs;
	//real actual position of the point being measured
	vec3 ray_pos;
	vec3 tmp;
	vec3 light;//= {
		//.x = 0.0F,//707107,
		//.y = 0.0F,
		//.z = 1.0F
	//};
	light = fromdirection3(time + 1.0F, time, 1.0F);
	//steps to march
	const int steps = 64;
	float distance;
	int i, j, k;
	for(i = 0; i < zsamples; i++) {
	for(j = 0; j < xsamples; j++) {
		//reset the ray offset
		ray_ofs.x = 0;
		ray_ofs.y = 0;
		ray_ofs.z = 0;
		//ray_rot = unit3(
		//our position in the screen + camera offset
		ray_orig.x =
			camera_ofs.x
			+ scale(j, 0, xsamples, 0, camera_size.x);
		ray_orig.y =
			camera_ofs.y;
		ray_orig.z =
			camera_ofs.z
			+ scale(i, 0, zsamples, 0, camera_size.y);
		for(k = 0; k < steps; k++) {
			ray_pos = add3(
				ray_ofs,
				ray_orig
				);
			distance = de(ray_pos);
			if(distance <= 0.05F) {
				fprintf(logfile, "PLOT!\n");
				plot(
					screen,
					j, i,
					colortoint(graytocolor(bclamp(
					//k * 20
					//255.0F * (float)k / (float)steps
					//500.0F / distance
					blinnphong(ray_orig, ray_pos, ray_rot, light)
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
			tmp = add3(
				distalong3(ray_rot, distance),
				ray_ofs
				);
			ray_ofs = tmp;
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
			fprintf(logfile, "hash file write error!\nfilename: %s\n", hashfilename);
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
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
