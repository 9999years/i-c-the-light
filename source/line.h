#include <SDL/SDL.h>

void drawline(
	SDL_Surface *screenSurface,
	unsigned int startx,
	unsigned int starty,
	unsigned int endx,
	unsigned int endy,
	unsigned int color
	)
{
	int i = startx, j = starty;

	rgbcolor pixel;

	// Draw to
	for(i = 0; i < endx; i++)
	{
		for(j = 0; j < endy; j++)
		{
			((unsigned int*)screenSurface->pixels)[j + i*SCREEN_WIDTH] =
				color;
		}
	}
	return;
}
