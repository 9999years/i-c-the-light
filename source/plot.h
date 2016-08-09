#include <SDL/SDL.h>

int plot(
	SDL_Surface *screenSurface,
	int x, int y,
	int width, int height,
	unsigned int color
	)
{
	//printf( "x,y: (%d, %d)\n"
		//"w,h: (%d, %d)\n",
		//x, y, width, height);
	if(
		   x >= width
		|| x < 0
		|| y >= height
		|| y < 0
		)
	{
		//out of bounds
		return 1;
	}
	((unsigned int*)screenSurface->pixels)[x + y*width] =
		color;
	return 0;
}
