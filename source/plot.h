//functions for plotting points on an sdl surface

#include <SDL/SDL.h>
#include <stdio.h>

#ifndef PLOT_H
#define PLOT_H

unsigned int getpixel(
	SDL_Surface *screen,
	int x, int y
	)
{
	if(
		   x >= screen->w
		|| x < 0
		|| y >= screen->h
		|| y < 0
	) {
		//out of bounds
		//returning negative or 0 wouldnt work
		//so let's just wrap around the edges
		//printf("PIXEL OUT OF BOUNDS IN getpixel IN plot.h\n");
		x = abs(x%screen->w);
		y = abs(y%screen->h);
	}
	return ((unsigned int*)screen->pixels)[x + y*screen->w];
}

int plot(
	SDL_Surface *screen,
	int x, int y,
	unsigned int color
	)
{
	if(
		   x >= screen->w
		|| x < 0
		|| y >= screen->h
		|| y < 0
	) {
		//out of bounds
		//printf("OOB\n");
		//printf("x: %d\ny: %d\nw: %d\nh: %d\n", x, y, screen->w, screen->h);
		return 1;
	}

	((unsigned int*)screen->pixels)[x + y * screen->w] =
		color;

	return 0;
}
#endif //PLOT_H
