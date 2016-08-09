//display
#include <SDL/SDL.h>
//logging, file out
#include <stdio.h>
//sin functions
#include <math.h>
//timestamp for file names
#include <time.h>
//int / array color conversion
#include "color.h"
//ppm handling
#include "ppm.h"
//line drawing
#include "line.h"

//Screen dimension constants
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define HALF_SCREEN_WIDTH 320
#define HALF_SCREEN_HEIGHT 240

int lastx = HALF_SCREEN_WIDTH, lasty = HALF_SCREEN_HEIGHT;

int render(SDL_Surface *screenSurface, unsigned int width, unsigned int height)
{

	int tick = SDL_GetTicks();

	rgbcolor color = {0xff, 0x00, 0x00}, newcolor;
	shifthue(newcolor, color, (float)tick/4);

	int newx = HALF_SCREEN_WIDTH+ ((double)tick/128)*cos((double)tick/256);
	int newy = HALF_SCREEN_HEIGHT+((double)tick/128)*sin((double)tick/256);

	drawline(screenSurface,
		width, height,
		//50, 100,
		//HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT,
		lastx, lasty,
		//400, 200,
		newx, newy,
		colortoint(newcolor)
		);
	lastx = newx;
	lasty = newy;
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
			//color it black just to be safe
			SDL_FillRect(screenSurface, NULL, 0x000000);

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
