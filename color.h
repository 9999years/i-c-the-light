#define RED 0
#define GREEN 1
#define BLUE 2
#define CHANNELS 3

typedef unsigned char rgbcolor[3];

//struct rgbcolor {
	//unsigned char red;
	//unsigned char green;
	//unsigned char blue;
//};

unsigned int colortoint(rgbcolor color)
{
	return color[BLUE] + color[GREEN]*0x100 + color[RED]*0x10000;
}

int inttocolor(rgbcolor colorarray, unsigned int color)
{
	colorarray[BLUE]  = (color)       & 0xff;
	colorarray[GREEN] = (color >> 8)  & 0xff;
	colorarray[RED]   = (color >> 16) & 0xff;
	return 0;
}
