//function for drawing lines

#include <SDL/SDL.h>
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
	)
{
	//code from https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
	//changed to suck a lot less but the implementation is from there
	//who writes `for(;;)`????

	int deltax = abs(endx-x), signx = x < endx ? 1 : -1;
	int deltay = abs(endy-y), signy = y < endy ? 1 : -1;
	int error = (deltax > deltay ? deltax : -deltay)/2, errorcompare;

	while(x != endx || y != endy) {
		plot(screen, x, y, color);
		errorcompare = error;
		if(errorcompare > -deltax) { error -= deltay; x += signx; }
		if(errorcompare <  deltay) { error += deltax; y += signy; }
	}

	return 0;
}

int plotcirclepoints(
	SDL_Surface *screen,
	int cx,
	int cy,
	int x,
	int y,
	unsigned int color
	)
{
	//plot points in 8 octants each iteration
	plot(screen, cx + x, cy + y, color); //1
	plot(screen, cx + y, cy + x, color); //2
	plot(screen, cx - y, cy + x, color); //3
	plot(screen, cx - x, cy + y, color); //4
	plot(screen, cx - x, cy - y, color); //5
	plot(screen, cx + y, cy - x, color); //7
	plot(screen, cx - y, cy - x, color); //6
	plot(screen, cx + x, cy - y, color); //8
	return 0;
}

int drawcircle(
	SDL_Surface *screen,
	int cx, //center coords
	int cy,
	int r,
	unsigned int color
	)
{
	int x = r,
	    y = 0,
	    deltax = 1 - 2 * r,
	    deltay = 1,
	    error = 0;
	while(x >= y) {
		plotcirclepoints(screen, cx, cy, x, y, color);
		y++;
		error += deltay;
		deltay += 2;
		if(2 * error + deltax > 0) {
			x--;
			error += deltax;
			deltax += 2;
		}
	}
	//impl. from http://web.engr.oregonstate.edu/~sllu/bcircle.pdf
	//A Fast Bresenham Type Algorithm For Drawing Circles, John Kennedy
	return 0;
}
#endif //LINE_H
