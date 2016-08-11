#include <SDL/SDL.h>

int plot(
	SDL_Surface *screenSurface,
	int x, int y,
	unsigned int color
	)
{
	if(
		   x >= screenSurface->w
		|| x < 0
		|| y >= screenSurface->h
		|| y < 0
		)
	{
		//out of bounds
		return 1;
	}

	((unsigned int*)screenSurface->pixels)[x + y*screenSurface->w] =
		color;

	return 0;
}
