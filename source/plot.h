#include <SDL/SDL.h>

unsigned int getpixel(
	SDL_Surface *screenSurface,
	int x, int y
	)
{
	if(
		   x >= screenSurface->w
		|| x < 0
		|| y >= screenSurface->h
		|| y < 0
	) {
		//out of bounds
		//returning negative or 0 wouldnt work
		//so let's just wrap around the edges
		//printf("PIXEL OUT OF BOUNDS IN getpixel IN plot.h\n");
		x = abs(x%screenSurface->w);
		y = abs(y%screenSurface->h);
	}
	return ((unsigned int*)screenSurface->pixels)[x + y*screenSurface->w];
}

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
	) {
		//out of bounds
		//printf("OOB\n");
		//printf("x: %d\ny: %d\nw: %d\nh: %d\n", x, y, screenSurface->w, screenSurface->h);
		return 1;
	}

	((unsigned int*)screenSurface->pixels)[x + y*screenSurface->w] =
		color;

	return 0;
}
