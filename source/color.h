#include <math.h>

#define RED 0
#define GREEN 1
#define BLUE 2
#define CHANNELS 3

typedef unsigned char BYTE;

typedef BYTE rgbcolor[3];

typedef short hslcolor[3];

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

BYTE clamp(float value)
{
	if(value < 0)
		return 0;
	else if(value > 0xff)
		return 0xff;
	return (BYTE)value;
}

void shifthue(rgbcolor out, const rgbcolor in, const float fHue)
{
	const float cosA = cos(fHue*3.14159265f/180);
	const float sinA = sin(fHue*3.14159265f/180);
	//printf("cos: %f\nsin: %f\nhue: %f\n", cosA, sinA, fHue);
	//calculate the rotation matrix, only depends on Hue
	float matrix[CHANNELS][CHANNELS] =
	{
		{
			     cosA + (1.0f - cosA) / 3.0f,
			1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA,
			1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA
		},
		{
			       1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA,
			cosA + 1.0f/3.0f * (1.0f - cosA),
			       1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA
		},
		{
			       1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA,
			       1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA,
			cosA + 1.0f/3.0f * (1.0f - cosA)
		}
	};

	//Use the rotation matrix to convert the RGB directly
	out[RED] = clamp(
		  in[RED]   * matrix[RED][RED]
		+ in[GREEN] * matrix[RED][GREEN]
		+ in[BLUE]  * matrix[RED][BLUE]
		);

	out[GREEN] = clamp(
		  in[RED]   * matrix[GREEN][RED]
		+ in[GREEN] * matrix[GREEN][GREEN]
		+ in[BLUE]  * matrix[GREEN][BLUE]
		);

	out[BLUE] = clamp(
		  in[RED]   * matrix[BLUE][RED]
		+ in[GREEN] * matrix[BLUE][GREEN]
		+ in[BLUE]  * matrix[BLUE][BLUE]
		);

	return;
}
