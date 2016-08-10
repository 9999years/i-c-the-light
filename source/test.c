#include <stdio.h>
#include <assert.h>
#include "color.h"
#include "ppm.h"

int main( int argc, char *argv[] )
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
		"red = 0xdd, green = 0x12, blue = 0xab\n"
		"a =       0x%x\n"
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
		"400.4:    %d (expected: 255)\n"
		"-21.4959: %d (expected: 0)\n"
		"0x40:     %x (expected: 40)\n"
		"0:        %d (expected: 0)\n"
		"0xff:     %x (expected: 0)\n\n",
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
		"IN:  %x %x %x\n"
		"OUT: %x %x %X\n\n",
		c[RED], c[GREEN], c[BLUE],
		d[RED], d[GREEN], d[BLUE]
		);

	c[RED]   = 0xff;
	c[GREEN] = 0x00;
	c[BLUE]  = 0x00;
	shifthue(d, c, 180);
	printf( "IN:  %x %x %x\n"
		"OUT: %x %x %X\n\n",
		c[RED], c[GREEN], c[BLUE],
		d[RED], d[GREEN], d[BLUE]
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

	printf( "\n---------------\n"
		"ALL TESTS PASSED\n"
		"consider running `make clean` to delete generated test files\n");
	return 0;
}
