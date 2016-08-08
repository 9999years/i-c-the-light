//display
#include "SDL/SDL.h"
//logging, file out
#include <stdio.h>
//sin functions
#include <math.h>
//timestamp for file names
#include <time.h>
//int / array color conversion
#include "color.h"

//Screen dimension constants
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define HALF_SCREEN_WIDTH 320
#define HALF_SCREEN_HEIGHT 240

enum {
	PORTABLE_BITMAP = 1,
	PORTABLE_GRAYMAP = 2,
	PORTABLE_PIXMAP = 3
};

int writeppm(
	char *filename,
	char filetype,
	int width,
	int height,
	unsigned int image[]
	)
{
	FILE *file = fopen(filename, "w");
	if(file == NULL)
	{
		printf("file open failure!\n");
		return 1;
	}
	fprintf(file, "P%d\n%d %d\n%d\n", filetype, width, height, 0xff);
	int i = 0, j = 0, k = 0;
	rgbcolor pixel;
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			inttocolor(pixel, image[j + i*width]);
			for(k = 0; k < CHANNELS; k++)
			{
				fprintf(file, "%d ", pixel[k]);
			}
		}
		fprintf(file, "\n");
	}
	printf("write done!\n");
	return 0;
}

int render(SDL_Surface *screenSurface)
{
	// Lock surface if
	if(SDL_MUSTLOCK(screenSurface))
	{
		if(SDL_LockSurface(screenSurface) < 0)
		{
			return 1;
		}
	}

	//get the time
	int tick = SDL_GetTicks();

	// Declare a couple of
	int i = 0, j = 0, yofs = 0, ofs = 0;

	rgbcolor pixel;

	// Draw to
	yofs = 0;
	for(i = 0; i < SCREEN_HEIGHT; i++)
	{
		for(j = 0, ofs = yofs; j < SCREEN_WIDTH; j++, ofs++)
		{
			pixel[BLUE] =
				(double)(i-HALF_SCREEN_HEIGHT)/8*(double)(j-HALF_SCREEN_WIDTH)/8+(double)tick/32;

			pixel[GREEN] =
				pixel[BLUE]*cos((double)i/32+(double)j/32);

			pixel[RED] =
				pixel[BLUE]*sin((double)i/32+(double)j/32);

			((unsigned int*)screenSurface->pixels)[ofs] =
				colortoint(pixel);
		}
		yofs += screenSurface->pitch / 4;
	}

	// Unlock if
	if(SDL_MUSTLOCK(screenSurface))
	{
		SDL_UnlockSurface(screenSurface);
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
			screenSurface = SDL_GetWindowSurface( window );

			//save
			render(screenSurface);
			sprintf(filename, "output/image%lu.ppm", (unsigned long int)time(NULL));
			if(writeppm(filename, PORTABLE_PIXMAP, SCREEN_WIDTH, SCREEN_HEIGHT, screenSurface->pixels)
				!= 0)
				printf("image write error!\n");
		}
	}
	int quit = 0;
	while (!quit)
	{
		//SDL_Delay(16);
		// render
		render(screenSurface);

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
						sprintf(filename, "output/image%lu.ppm", (unsigned long int)time(NULL));
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
