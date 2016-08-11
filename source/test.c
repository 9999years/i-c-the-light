#include <SDL/SDL.h>
#include <stdio.h>
#include <assert.h>
#include "color.h"
#include "plot.h"
#include "ppm.h"

int main(int argc, char *argv[])
{
	printf( "TESTING color.h\n"
		"---------------\n\n");
	rgbcolor c =
	{
		0xdd,
		0x12,
		0xab
	};
	unsigned int a = colortoint(c);
	printf( "TESTING colortoint:\n"
		"color =   {0xdd, 0x12, 0xab}\n"
		"int =     0x%x\n"
		"expected: 0xdd12ab\n\n", a);
	assert(a == 0xdd12ab);

	rgbcolor d;
	inttocolor(d, a);
	printf( "TESTING inttocolor:\n"
		"int   = 0xdd12ab\n"
		"red   = 0x%x (expected: 0xdd)\n"
		"green = 0x%x (expected: 0x12)\n"
		"blue  = 0x%x (expected: 0xab)\n\n",
		d[RED], d[GREEN], d[BLUE]);
	assert(d[RED]   == 0xdd);
	assert(d[GREEN] == 0x12);
	assert(d[BLUE]  == 0xab);

	//this is really a misuse of the color type
	d[RED]   = clamp(400.4);
	d[GREEN] = clamp(-21.4959);
	d[BLUE]  = clamp(0x40);
	c[RED]   = clamp(0);
	c[GREEN] = clamp(0xff);

	printf( "TESTING clamp:\n"
		"400.4:    %3d (expected: 255)\n"
		"-21.4959: %3d (expected: 0)\n"
		"0x40:     %3x (expected: 40)\n"
		"0:        %3d (expected: 0)\n"
		"0xff:     %3x (expected: 0)\n\n",
		d[RED], d[GREEN], d[BLUE], c[RED], c[GREEN]
		);
	assert(d[RED]   == 0xff);
	assert(d[GREEN] == 0);
	assert(d[BLUE]  == 0x40);
	assert(c[RED]   == 0x0);
	assert(c[GREEN] == 0xff);

	c[GREEN] = 0x11;
	shifthue(d, c, 10);
	printf( "TESTING shifthue:\n"
		"in:       0x%2.2x%2.2x%2.2x\n"
		"out:      0x%2.2x%2.2x%2.2x\n"
		"expected: 0x1000ab\n\n",
		c[RED], c[GREEN], c[BLUE],
		d[RED], d[GREEN], d[BLUE]
		);
	assert(
		   d[RED] == 0x10
		&& d[GREEN] == 0x00
		&& d[BLUE] == 0xab
		);

	c[RED]   = 0xff;
	c[GREEN] = 0x00;
	c[BLUE]  = 0x00;
	shifthue(d, c, 180);
	printf( "in:       0x%2.2x%2.2x%2.2x\n"
		"out:      0x%2.2x%2.2x%2.2x\n"
		"expected: 0x00a9aa\n\n",
		c[RED], c[GREEN], c[BLUE],
		d[RED], d[GREEN], d[BLUE]
		);
	assert(
		   d[RED]   == 0x00
		&& d[GREEN] == 0xa9
		&& d[BLUE]  == 0xaa
		);

	printf( "TESTING ppm.h\n"
		"-------------\n\n");

	char *filename = "TEST_PPM.ppm";
	char filetype = PORTABLE_PIXMAP;
	int width = 4;
	int height = 3;
	unsigned int image[12] =
	{
		0x002244, 0x446688, 0x88aabb, 0xbbaaff,
		0x123456, 0x789abc, 0xdef012, 0x456789,
		0xbbbbbb, 0x888888, 0x444444, 0x000000
	};

	printf("writing test file: %s\n", filename);

	int result =
		writeppm(filename, filetype, width, height, image);

	assert(result == 0);

	printf("write success!\n");

	//what we expect to have been written
	char *expected =
		"P3\n"
		"4 3\n"
		"255\n"
		"0 34 68 68 102 136 136 170 187 187 170 255 \n"
		"18 52 86 120 154 188 222 240 18 69 103 137 \n"
		"187 187 187 136 136 136 68 68 68 0 0 0 \n";

	printf("reading %s\n", filename);
	FILE *imagein;
	imagein = fopen(filename, "r");

	char readchar;
	int i = 0;
	for(i = 0; (readchar = fgetc(imagein)) != EOF; i++)
	{
		assert(readchar == expected[i]);
	}
	printf("\n%s wrote as expected!\n", filename);

	printf( "TESTING plot.h\n"
		"--------------\n\n"
		);

	SDL_Surface *screenSurface;
	unsigned int rmask, gmask, bmask, amask;

	//i had no clue this was even a thing. what's the amask for my code?
	//i dont understand anything
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif
	screenSurface->w = 4;
	screenSurface->h = 3;
	screenSurface->pixels = &image;

	plot(screenSurface, 0, 0, 0xffffff);

	printf( "\n---------------\n"
		"ALL TESTS PASSED\n"
		"consider running `make clean` to delete generated test files\n");
	return 0;
}
