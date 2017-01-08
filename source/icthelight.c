//repo here: https://github.com/9999years/i-c-the-light
//MIT/expat license
//rebecca turner
//consult ../readme.md

//display
#include <SDL/SDL.h>
#undef main
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
#define SCREEN_WIDTH  2160
#define SCREEN_HEIGHT 2160

//globals
int frame = 0;
FILE *logfile;
FILE *plotfile;

//global distance estimator
float de(vec3 pos)
{
	//quaternion c = constq(-0.2F, 0.6F, 0.2F, 0.2F);
	quaternion c = constq(-0.137F, -0.630F, -0.475F, -0.046);
	//quaternion c = constq(-0.213F, -0.0410F, -0.563F, -0.560);
	return distancejulia(pos, c, 64);
	//return distserpenski(pos);
	//return
		//opu(
		//disttorus(pos, const3(0.0F, 0.0F, 0.0F), 2.0F, 5.5F);
		//distsphere(pos, const3(50.0F, 0.0F, 50.0F), 10.0F);
		//opwobble3(
			//pos,
			//distsphere(pos, const3(0.0F, 0.0F, 0.0F), 15.0F),
			//3.0F,
			//10.0F
			//);
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
float blinnphong(vec3 cam, vec3 pos, vec3 rot, vec3 light)
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
	vec3 normal = getnormal(pos, 0.01F);
	//vec3 halfway = div3s(add3(cam, light), magn3(add3(cam, light)));
	//vec3 halfway = avg3(cam, light);
	vec3 halfway = unit3(add3(cam, light));
	float ret =
		dot3(light, normal) * diffuse_intensity
		+ pow(dot3(normal, halfway), alpha) * specular_intensity;
	//ret *= randf(0.0F, 1.F);
	//printf("ret: %f\n", ret);
	return ret;
}

void render(SDL_Surface *screen, const int frame)
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

#define MAX_DISTANCE 0.1F
#define MIN_DISTANCE 0.001F
#define BOUNDING_RADIUS 100.0F

	printf("initializing render scene, allocating\n");

#define FRAMES_IN_ROTATION 60.0F
	const float timef = (float)(frame * TAU) / FRAMES_IN_ROTATION;
	unsigned long int timeint = time(NULL);
	printf("time: %f for %lu\n", timef, timeint);
	const float sintime = sin(timef);
	const float costime = cos(timef);
	//const float tworoottwo = sqrt(2.0F) / 2.0F;
	//how big the viewport is
	const float viewport_size = 2.75F;
	SDL_FillRect(screen, NULL, 0xffffff);
	//SDL_FillRect(screen, NULL, 0x000000);

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

	//screen aspect ratio
	//const float aspect = (float)screen->w / (float)screen->h;

	//portion of viewport to render
	//this is multiplied by the viewport vectors to get a point on the
	//viewport the ray is shot through
	float wfrac, hfrac;

	//focal length of the camera
	//longer = more zoomed in
	float focallength = 20.0F;
	//float focallength = frame * frame;
	//printf("f: %f\n", focallength);

	//width of the camera (horiz. line at the center of the viewport)
	vec3 viewport_width = const3(
		viewport_size * costime,
		viewport_size * sintime,
		//viewport_size * tworoottwo,
		//viewport_size * tworoottwo,
		0.0F
	);

	//height of the camera (vert. line at the center of the viewport)
	vec3 viewport_height = const3(0.0F, 0.0F, viewport_size);

	//offset of the center of the viewport from the origin
	//essentially the camera position
	vec3 viewport_ofs = const3(
		2.0F * cos(timef - HALF_PI),
		2.0F * sin(timef - HALF_PI),
		0.0F
	);


	//point the ray will travel through, other than the camera
	vec3 ray_through;

	//camera
	//the rays are shot from here through the viewport
	//it's backwards and perpendicular from the plane
	//containing the viewport
	//viewport_ofs + (perp to the w & height of the vp) * focal len
	vec3 camera = add3(
		viewport_ofs,
		mult3s(
			unit3(perp3(viewport_width, viewport_height)),
			focallength
		)
	);
	//printf("cam:\n");
	//dump3(camera);

	//direction for the ray to travel in
	vec3 ray_rot;

	//the current position to be measured by the distance estimator
	//generally camera + distance in the direction of ray_rot
	vec3 measure_pos;

	//ray origin (location on the viewport plane)
	vec3 ray_orig;

	//distance from measure_pos
	float distance;
	//total distance travelled, i hope i hope i hope
	float totaldistance;

	//preview value
	float predist;

	vec3 light = fromdirection3(timef + 1.0F, timef, 1.0F);

	//steps to march
	const int steps = 512;

	printf("beginning render loop\n");

	int i, j, k;
	for(i = 0; i < screen->h; i++) {
	for(j = 0; j < screen->w; j++) {
		//fprintf(logfile, "NEW RAY!\n");
		//new sample, new distance
		distance = 0.0F;
		totaldistance = 0.0F;

		//what portion of the viewport are we rendering to?
		//-0.5 to 0.5 because viewport_ofs represents the center
		//of the viewport and viewport_width and _height represent
		//whole, not half widths of the viewport size
		wfrac = scale(j, 0, screen->w, -0.5F, 0.5F);
		hfrac = scale(i, 0, screen->h, -0.5F, 0.5F);

		//multiply that by width / height to get a real location
		ray_through = add3(
			mult3s(viewport_width, wfrac),
			mult3s(viewport_height, hfrac)
		);

		//where the ray starts (on the viewport)
		ray_orig = add3(ray_through, viewport_ofs);

		//printf("ray start pos:\n");
		//dump3(ray_orig);

		//a unit vec from the camera through the viewport
		ray_rot = unit3(through3(
			camera,
			add3(ray_through, viewport_ofs)
		));

		//fprintf(plotfile, "%f\t%f\t%f\n",
			//ray_rot.x,
			//ray_rot.y,
			//ray_rot.z
			//);

		//continue;

		for(k = 0; k < steps; k++) {
			//if((distance != 0.5F) && (distance != 0.0F)) {
				//[>f<]printf(//logfile,
					//"-----------------\n"
					//"k: %d\n"
					//"total: %.4f\n"
					//"dist: %.4f\n"
					//"predist: %.4f\n",
					//k, totaldistance, distance, predist
				//);
			//}
			measure_pos = add3(
				ray_orig,
				mult3s(
					ray_rot,
					totaldistance
				)
			);

			predist = de(measure_pos);

			totaldistance += distance =
				//fclamp(de(measure_pos), MIN_DISTANCE, MAX_DISTANCE);
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
				coords[coordslen] = j + i * screen->w;
				values[coordslen] =
					//(float)k;
					(1.5F - (float)k / (float)steps) *
					blinnphong(camera, measure_pos, ray_rot, light);
				coordslen++;
				break;
			} else if(totaldistance >= BOUNDING_RADIUS
				|| !isfinite(distance)) {
				//we're done here
				//fprintf(logfile, "LIMIT EXCEEDED, BREAK!\n");
				break;
			}
			//SDL_Delay(200);
		}
	}
	}

	printf("done calculating array\n");

	struct limits limit = getlimits(values, coordslen);

	printf("done calculating limits\n");

	for(i = 0; i < coordslen; i++) {
		plot(
			screen,
			coords[i] % screen->w,
			coords[i] / screen->h,
			colortoint(graytocolor(bclamp(
				scale(values[i], limit.min, limit.max, 0, 255)
				+ random(-5, 5)
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

void saveframe(SDL_Surface *screen)
{
	char filename[256] = "output/UNINITIALIZED.ppm";
	unsigned long int timeint = time(NULL);
	sprintf(filename, "../output/image%lu_f%d.ppm", timeint, frame);
	printf("printing %s\n", filename);
	int writestatus;
	if(
		(writestatus = writeppm(
			filename,
			screen->w,
			screen->h,
			screen->pixels
		)) != 0
	) {
		switch(writestatus) {
		case PPM_FILE_OPEN_FAILURE:
			fprintf(logfile, "image write error!\n");
			break;
		case PPM_WRITE_ALL_BLACK:
			fprintf(logfile, "image is all black!\n");
			break;
		default:
			fprintf(logfile, "unknown write error!\n");
			break;
		}
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
		printf("image already rendered, deleting\n");
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
		case SDL_KEYDOWN:
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

int main(int argc, char *argv[])
{
	printf("Hello!\n");
	const time_t unixtime = time(NULL);
	printf( "I C the Light by Rebecca Turner (MIT/Expat)\n"
		"%s\n", ctime(&unixtime)
	);

	printf(
		"╭─────────────────────────────────────╮\n"
		"│ ·           ╷         ╷ ·     ╷     │\n"
		"│ ╷   ╭─╮   ┼ ├─╮ ╭─╮   │ ╷ ╭─╮ ├─╮ ┼ │\n"
		"│ │   │     │ │ │ ├─┘   │ │ ╰─╯ │ │ │ │\n"
		"│ ╵   ╰─╯   ╰ ╵ ╵ ╰─╯   ╵ ╵ ╰─╮ ╵ ╵ ╰ │\n"
		"╰───────────────────────────╰─╯───────╯\n"
	);

	initializelogfile();

	//The window we'll be rendering to
	SDL_Window *window = NULL;

	//The surface contained by the window
	SDL_Surface *screen = NULL;

	char *window_arg = "window";

	if(searchargs(argc, argv, window_arg) == 1) {
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
	} else {
		unsigned int rmask, gmask, bmask, amask;
#define SCREEN_BIT_DEPTH 32
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#endif
		screen = SDL_CreateRGBSurface(
			0,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SCREEN_BIT_DEPTH, rmask, gmask, bmask, amask
		);
	}

	if(window != NULL) {
		SDL_FillRect(screen, NULL, 0x000000);
		SDL_UpdateWindowSurface(window);
	}

	int quit = 0;
	clock_t start, end;
	double total;
	do {
		start = clock();
		//printf("frame: %d\n", frame);

		//poll for events, and handle the ones we care about.
		//this returns 1 if we need to quit
		quit = handleevents(screen);

		// render
		//if(frame == 0) {
			render(screen, frame);
			saveframe(screen);
		//}

		//Update the surface
		if(window != NULL) {
			SDL_UpdateWindowSurface(window);
		}

		end = clock();
		//if(frame%30 == 0) {
			////if(frame == 0)
				////saveframe(screen);
		//}
		total = (double)(end - start) / CLOCKS_PER_SEC;
		printf("%.4f FPS = %.4f SPF\n", 1 / total, total);
		frame++;
		if(frame > FRAMES_IN_ROTATION + 1) {
			quit = 1;
		}
	} while(!quit);

	//Destroy window
	if(window != NULL) {
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
