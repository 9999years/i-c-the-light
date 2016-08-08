#include <stdio.h>
#include <assert.h>
#include "color.h"

int main( int argc, char *argv[] )
{
	printf( "TESTING color.h\n"
		"---------------\n");
	rgbcolor c =
	{
		.red = 0xdd,
		.green = 0x12,
		.blue = 0xab
	};
	unsigned int a = colortoint(c);
	printf( "TESTING colortoint:\n"
		"red = 0xdd, green = 0x12, blue = 0xab\n"
		"a =       0x%x\n"
		"expected: 0xdd12ab\n\n", a);
	assert(a == 0xdd12ab);

	rgbcolor d = inttocolor(a);
	printf( "TESTING inttocolor:\n"
		"int   = 0xdd12ab\n"
		"red   = 0x%x (expected: 0xdd)\n"
		"green = 0x%x (expected: 0x12)\n"
		"blue  = 0x%x (expected: 0xab)\n",
		d.red, d.green, d.blue);
	assert(d.red = 0xdd);
	assert(d.green = 0x12);
	assert(d.blue = 0xab);
	return 0;
}
