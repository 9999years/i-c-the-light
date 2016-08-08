#include <stdio.h>
#include <assert.h>
#include "color.h"

int main( int argc, char *argv[] )
{
	printf( "TESTING color.h\n"
		"---------------\n");
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
		"blue  = 0x%x (expected: 0xab)\n",
		d[RED], d[GREEN], d[BLUE]);
	assert(d[RED] == 0xdd);
	assert(d[GREEN] == 0x12);
	assert(d[BLUE] == 0xab);
	return 0;
}
