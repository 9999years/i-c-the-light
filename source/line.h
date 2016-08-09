#include <SDL/SDL.h>
#include "plot.h"

int drawline(
	SDL_Surface *screenSurface,
	unsigned int width,
	unsigned int height,
	int x,
	int y,
	int endx,
	int endy,
	unsigned int color
	)
{
	//code from https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C

	int deltax = abs(endx-x), signx = x<endx ? 1 : -1;
	int deltay = abs(endy-y), signy = y<endy ? 1 : -1;
	int error = (deltax>deltay ? deltax : -deltay)/2, errorcompare;

	while(1){
		//if this silently fails, that's OK
		plot(screenSurface, x, y, width, height, color);
		if(x == endx && y == endy)
			break;
		errorcompare = error;
		if(errorcompare >-deltax) { error -= deltay; x += signx; }
		if(errorcompare < deltay) { error += deltax; y += signy; }
	}

	return 0;
}
