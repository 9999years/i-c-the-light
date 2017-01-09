//functions for plotting points on an sdl surface

#include <stdio.h>

#include "sdl.h"

#ifndef PLOT_H
#define PLOT_H

unsigned int getpixel(
	SDL_Surface *screen,
	int x, int y
);

int plot(
	SDL_Surface *screen,
	int x, int y,
	unsigned int color
);

#endif //PLOT_H
