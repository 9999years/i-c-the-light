//function for drawing lines

#include "plot.h"

#ifndef LINE_H
#define LINE_H

int drawline(
	SDL_Surface *screen,
	int x,
	int y,
	int endx,
	int endy,
	unsigned int color
);
#endif //LINE_H
