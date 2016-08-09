#include <SDL/SDL.h>

int drawline(
	SDL_Surface *screenSurface,
	unsigned int width,
	unsigned int height,
	int startx,
	int starty,
	int endx,
	int endy,
	unsigned int color
	)
{
	if(
		   startx > width
		|| startx < 0
		|| endx > width
		|| endx < 0
		|| starty > height
		|| starty < 0
		|| endy > height
		|| endy < 0
		)
	{
		//out of bounds
		return 1;
	}

	//int x, y;
	//int deltax, deltay, error, deltaerror, delta;
	//deltax = endx - startx;
	//if(deltax == 0)
	//{
		////vertical line, ur fucked lol
		//return 1;
	//}
	//deltay = endy - starty;
	//delta = deltay - deltax;
	////error = -1.0;
	////deltaerror = abs(deltay/deltax);

	//for(x = startx; x < endx; x++)
	//{
		
		//((unsigned int*)screenSurface->pixels)[y + x*width] =
			//color;
		////error += deltaerror;
		//if(delta >= 0)
		//{
			//y++;
			//delta -= deltax;
			////error -= 1.0;
		//}
		//delta += deltay;
	//}

	int dx = abs(endx-startx), sx = startx<endx ? 1 : -1;
	int dy = abs(endy-starty), sy = starty<endy ? 1 : -1;
	int err = (dx>dy ? dx : -dy)/2, e2;

	for(;;){
		((unsigned int*)screenSurface->pixels)[startx + starty*width] =
			color;
		if (startx==endx && starty==endy) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; startx += sx; }
		if (e2 < dy) { err += dx; starty += sy; }
	}

	////problem
	//printf( "PROBLEM:\n"
		//"width  %u\n"
		//"height %u\n"
		//"startx %u\n"
		//"starty %u\n"
		//"endx,  %u\n"
		//"endy,  %u\n",
		//width,
		//height,
		//startx,
		//starty,
		//endx,
		//endy
		//);
	return 0;
}
