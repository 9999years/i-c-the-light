#include <math.h>

#define RED 0
#define GREEN 1
#define BLUE 2
#define CHANNELS 3

typedef unsigned char BYTE;

typedef BYTE rgbcolor[3];

unsigned int colortoint(rgbcolor color)
{
	return color[BLUE] + color[GREEN]*0x100 + color[RED]*0x10000;
}

void inttocolor(rgbcolor colorarray, unsigned int color)
{
	colorarray[BLUE]  = (color)       & 0xff;
	colorarray[GREEN] = (color >> 8)  & 0xff;
	colorarray[RED]   = (color >> 16) & 0xff;
	return;
}

BYTE clamp(float value) //define a function to bound and round the input float value to 0-255
{
	if(value < 0)
		return 0;
	if(value > 0xff)
		return 0xff;
	return (BYTE)value;
}

void shifthue(rgbcolor out, rgbcolor in, const float fHue)
{
	const float cosA = cos(fHue*3.14159265f/180); //convert degrees to radians
	const float sinA = sin(fHue*3.14159265f/180); //convert degrees to radians
	//calculate the rotation matrix, only depends on Hue
	float matrix[3][3] = {{cosA + (1.0f - cosA) / 3.0f, 1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA, 1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA},
		{1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA, cosA + 1.0f/3.0f*(1.0f - cosA), 1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA},
		{1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA, 1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA, cosA + 1.0f/3.0f * (1.0f - cosA)}};
	//Use the rotation matrix to convert the RGB directly
	out[RED] = clamp(in[RED]*matrix[0][0] + in[GREEN]*matrix[0][1] + in[BLUE]*matrix[0][2]);
	out[GREEN] = clamp(in[RED]*matrix[1][0] + in[GREEN]*matrix[1][1] + in[BLUE]*matrix[1][2]);
	out[BLUE] = clamp(in[RED]*matrix[2][0] + in[GREEN]*matrix[2][1] + in[BLUE]*matrix[2][2]);
	return;
}
