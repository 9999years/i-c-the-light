//functions for plotting points on a surface

#include <stdio.h>
#include "surface.h"

#ifndef PLOT_H
#define PLOT_H

unsigned int getpixel(
	surface *screen,
	int x, int y
);

int plot(
	surface *screen,
	int x, int y,
	unsigned int color
);

#endif //PLOT_H
