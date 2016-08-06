#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"

SDL_Surface *screen;

void render()
{
	// Lock surface if
	if (SDL_MUSTLOCK(screen))
		if (SDL_LockSurface(screen) < 0)
			return;

	// Ask SDL for the time in
	int tick = SDL_GetTicks();

	// Declare a couple of
	int i, j, yofs, ofs;

	// Draw to
	yofs = 0;
	for (i = 0; i < 480; i++)
	{
		for (j = 0, ofs = yofs; j < 640; j++, ofs++)
		{
			((unsigned int*)screen->pixels)[ofs] = i * i + j * j + tick;
		}
		yofs += screen->pitch / 4;
	}

	// Unlock if
	if (SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);

	// Tell SDL to update the whole
	SDL_UpdateRect(screen, 0, 0, 640, 480);
}


// Entry
int main(int argc, char *argv[])
{
	// Initialize SDL's subsystems - in this case, only video.
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// Register SDL_Quit to be called at exit; makes sure things
	// cleaned up when we quit.
	atexit(SDL_Quit);

	// Attempt to create a 640x480 window with 32bit pixels.
	screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

	// If we fail, return error.
	if ( screen == NULL )
	{
		fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
		exit(1);
	}

	// Main loop: loop forever.
	while (1)
	{
		// Render
		render();

		// Poll for events, and handle the ones we care about.
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					break;
				case SDL_KEYUP:
					// If escape is pressed, return (and thus, quit)
					if (event.key.keysym.sym == SDLK_ESCAPE)
						return 0;
					break;
				case SDL_QUIT:
					return(0);
			}
		}
	}
	return 0;
}
