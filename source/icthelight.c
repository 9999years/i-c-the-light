//display
#include <SDL/SDL.h>
//logging, file out
#include <stdio.h>
//sin functions
#include <math.h>
//timestamp for file names
#include <time.h>

//my headers
#include "color.h"
#include "ppm.h"
#include "line.h"

//Screen dimension constants
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define HALF_SCREEN_WIDTH 320
#define HALF_SCREEN_HEIGHT 240

#define PI 3.1415926535
#define TAU 6.2831853071
#define HALF_PI 1.57079632675

#define BOX_WIDTH 200
#define BOX_HEIGHT 100
#define WIGGLE_AMP 20

int render(SDL_Surface *screenSurface, unsigned int width, unsigned int height)
{

	int tick = SDL_GetTicks();

	SDL_FillRect(screenSurface, NULL, 0x000000);

	rgbcolor red = {0xff, 0x11, 0x11};
	rgbcolor rot;
	/*shifthue(rot, red, (double)tick/128);*/

	int i = 0;
	int sides = 5;
	for(i = 0; i < sides; i++)
	{
		shifthue(rot, red, (double)i*(double)tick/32);
		unsigned int color = colortoint(rot);
		int ax, ay, bx, by, ox, oy;

		ox = WIGGLE_AMP * sin((double)tick/256 + 2);
		oy = WIGGLE_AMP * cos((double)tick/256 + 2);

		ax = HALF_SCREEN_WIDTH
			+ BOX_WIDTH *sin(
				i*(TAU/sides)
				+ (double)tick/512
			);

		ay = HALF_SCREEN_HEIGHT
			+ BOX_HEIGHT*cos(
				i*(TAU/sides)
				+ (double)tick/512
			) * sin((double)tick/512) + BOX_HEIGHT;

		bx = HALF_SCREEN_WIDTH
			+ BOX_WIDTH *sin(
				(i-1)*(TAU/sides)
				+ (double)tick/512
			);

		by = HALF_SCREEN_HEIGHT
			+ BOX_HEIGHT*cos(
				(i-1)*(TAU/sides)
				+ (double)tick/512
			) * sin((double)tick/512) + BOX_HEIGHT;

		drawline(screenSurface,
			width, height,
			ax + ox, ay + oy,
			bx + ox, by + oy,
			color
			);
		drawline(screenSurface,
			width, height,
			ax + ox, ay + oy - BOX_WIDTH,
			bx + ox, by + oy - BOX_WIDTH,
			color
			);
		drawline(screenSurface,
			width, height,
			ax + ox, ay + oy,
			ax + ox, ay + oy - BOX_WIDTH,
			color
			);
	}

	return 0;
}

int WinMain( int argc, char* args[] )
{
	printf("hello!\n");
	char filename[256] = "output/UNINITIALIZED.ppm";
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	else
	{
		//Create window
		window = SDL_CreateWindow( "I C the Light", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);

			//save
			render(screenSurface, SCREEN_WIDTH, SCREEN_HEIGHT);

			sprintf(filename, "../output/image%lu.ppm", (unsigned long int)time(NULL));
			if(writeppm(filename, PORTABLE_PIXMAP, SCREEN_WIDTH, SCREEN_HEIGHT, screenSurface->pixels)
				!= 0)
				printf("image write error!\n");
		}
	}
	int quit = 0;
	while (!quit)
	{
		SDL_Delay(16);
		// render
		render(screenSurface, SCREEN_WIDTH, SCREEN_HEIGHT);

		//Update the surface
		SDL_UpdateWindowSurface( window );

		// poll forevents, and handle the ones we care about.
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYUP:
					// if escape is pressed, quit
					if(event.key.keysym.sym == SDLK_ESCAPE)
					{
						quit = 1;
					}
					else if(event.key.keysym.sym == SDLK_s)
					{
						sprintf(filename, "../output/image%lu.ppm", (unsigned long int)time(NULL));
						if(writeppm(filename, PORTABLE_PIXMAP, SCREEN_WIDTH, SCREEN_HEIGHT, screenSurface->pixels)
							!= 0)
							printf("image write error!\n");
					}
					break;

				case SDL_QUIT:
					quit = 1;
					break;
			}
		}
	}

	//Destroy window
	SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
