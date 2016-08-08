//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <math.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int clamp(
	int value,
	int min,
	int max
	)
{
	if(value < min)
	{
		return min;
	}
	else if(value > max)
	{
		return max;
	}
	else
	{
		return value;
	}
}

int autoscale(
	int width,
	int height,
	unsigned int image[width][height],
	int min,
	int max
	)
{
	int i = 0, j = 0;
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			image[i][j] -= min;
			//image[i][j] *= 0xff;
			image[i][j] *= (double)0xff/(max-min);
			//image[i][j] = clamp(image[i][j], 0, 0xffffff);
		}
	}
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
	int i = 0, j = 0, yofs = 0, ofs = 0, min = 255, max = 0;

	// Draw to
	yofs = 0;
	for(i = 0; i < SCREEN_HEIGHT; i++)
	{
		for(j = 0, ofs = yofs; j < SCREEN_WIDTH; j++, ofs++)
		{
			/*((unsigned int*)screenSurface->pixels)[ofs] = i * i + j * j + tick;*/
			((unsigned int*)screenSurface->pixels)[ofs] =
				(sin((double)i/32+(double)tick/64)+1)*0xf;
				//(sin((double)i/32) + cos((double)j/32));
				/*sin((double)i/32)*64 + 0xff00* sin((double)j/32)*64 + tick/4;*/
			if(((unsigned int*)screenSurface->pixels)[ofs] > max)
			{
				max = ((unsigned int*)screenSurface->pixels)[ofs];
			}
			else if(((unsigned int*)screenSurface->pixels)[ofs] < min)
			{
				min = ((unsigned int*)screenSurface->pixels)[ofs];
			}
		}
		yofs += screenSurface->pitch / 4;
	}

	autoscale(SCREEN_WIDTH, SCREEN_HEIGHT, screenSurface->pixels, min, max);

	// Unlock if
	if(SDL_MUSTLOCK(screenSurface))
	{
		SDL_UnlockSurface(screenSurface);
	}

	return 0;
}

int WinMain( int argc, char* args[] )
{
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
		window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface( window );

			//Fill the surface white
			//SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );

			//Wait two seconds
			//SDL_Delay( 2000 );
		}
	}
	int quit = 0;
	while (!quit)
	{
		SDL_Delay(16);
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
					// ifescape is pressed, return (and thus, quit)
					if(event.key.keysym.sym == SDLK_ESCAPE)
					{
						quit = 1;
					}
					/*else if(event.key.keysym.sym == SLDK_s)*/
					/*{*/

					/*}*/
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
