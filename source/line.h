#include <SDL/SDL.h>
#include "plot.h"

int drawline(
	SDL_Surface *screenSurface,
	int x,
	int y,
	int endx,
	int endy,
	unsigned int color
	)
{
	//code from https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
	//changed to suck a lot less but the implementation is from there
	//who writes `for(;;)`????

	int deltax = abs(endx-x), signx = x < endx ? 1 : -1;
	int deltay = abs(endy-y), signy = y < endy ? 1 : -1;
	int error = (deltax > deltay ? deltax : -deltay)/2, errorcompare;

	while(x != endx || y != endy) {
		plot(screenSurface, x, y, color);
		errorcompare = error;
		if(errorcompare > -deltax) { error -= deltay; x += signx; }
		if(errorcompare <  deltay) { error += deltax; y += signy; }
	}

	return 0;
}

//this function is cute but won't stay in the build for long, i dont think
//i dont like that it's 99% the same as drawline but has to be different
//maybe if i come up with a more elegant way of doing this...
int drawdottedline(
	SDL_Surface *screenSurface,
	int x,
	int y,
	int endx,
	int endy,
	unsigned int color,
	short dots
	)
{

	int deltax = abs(endx-x), signx = x < endx ? 1 : -1;
	int deltay = abs(endy-y), signy = y < endy ? 1 : -1;
	int error = (deltax > deltay ? deltax : -deltay)/2, errorcompare;

	int i = 0;
	while(x != endx || y != endy) {
		if(i%dots == 0)
			plot(screenSurface, x, y, color);
		errorcompare = error;
		if(errorcompare > -deltax) { error -= deltay; x += signx; }
		if(errorcompare <  deltay) { error += deltax; y += signy; }
		i++;
	}

	return 0;
}
