typedef struct rgbcolor rgbcolor;

struct rgbcolor {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

unsigned int colortoint(rgbcolor color)
{
	return color.blue + color.green*0x100 + color.red*0x10000;
}

rgbcolor inttocolor(unsigned int color)
{
	rgbcolor colorstruct =
	{
		.blue  = (color)       & 0xff,
		.green = (color >> 8)  & 0xff,
		.red   = (color >> 16) & 0xff
	};
	return colorstruct;
}
