//function for drawing lines

#include "plot.h"

int drawline(
	surface *screen,
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
