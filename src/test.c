#include "test.h"

void printscreen(surface *screen, unsigned int exclude)
{
	int i, j;
	unsigned int color;
	for(i = 0; i < screen->h; i++) {
		for(j = 0; j < screen->w; j++) {
			if((color = getpixel(screen, j, i)) != exclude) {
				printf("(%d, %d): 0x%6.6x\n",
					j, i, color);
			}
		}
	}
	return;
}

void testsection(char name[])
{
	//just prints the test section headers
	printf(
		"\n\n"
		"TESTING %s\n"
		"==============================================================================="
		"\n\n",
		name
	);
}

void screescreensts(surface *screen, window *window, int width, int height)
{
	fillRect(screen, NULL, COLOR_WHITE);

	char coords[9];

	printf(
		"screen initialization success!\n"
	);

	TESTING("getpixel (single test)");
	printf("(1,2) = %d (0xffffff expected)\n", getpixel(screen, 1, 2));

	TESTING("drawline (line.h) and getpixel (plot.h)");
	printf(
		"NOTE: it's difficult to test getpixel alone or drawline alone---\n"
		"to verify results of drawing, pixels have to be checked. If these\n"
		"fail, good luck debugging!\n\n"
	);

	drawline(screen, 1, 1, 12, 8, COLOR_RED);

	printf(
		"line drawn.\n"
		"asserting (1, 1), (2, 2), (2, 3), (3, 4), (4, 5), (4, 6),\n"
		"(5, 7), (5, 8), (6, 9), (7, 10), and (7, 11) are red\n"
	);
	//printscreen(screen, COLOR_WHITE);

	int xcoords[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	int ycoords[11] = {1, 2, 2, 3, 4, 4, 5, 5, 6,  7,  7};
	int ncoords = ARRAYSIZE(xcoords);
	int i = 0;
	unsigned int color;
	for(i = 0; i < ncoords; i++) {
		sprintf(coords, "(%d, %d)", xcoords[i], ycoords[i]);
		printf(
			"%9s = 0x%6.6x (0xff0000 expected)\n",
			coords, color = getpixel(screen, xcoords[i], ycoords[i])
		);
		assert(color == COLOR_RED);
	}

	printf("\nasserting various other coordinates are white, testing getpixel edge cases\n");

	xcoords[0] = 5;   ycoords[0] = 1;
	xcoords[1] = 10;  ycoords[1] = 0;
	xcoords[2] = 11;  ycoords[2] = 32;
	xcoords[3] = 20;  ycoords[3] = 8;
	xcoords[4] = 80;  ycoords[4] = 4;
	xcoords[5] = 120; ycoords[5] = 12;
	xcoords[6] = -55; ycoords[6] = 0;
	ncoords = 7;
	for(i = 0; i < ncoords; i++) {
		sprintf(coords, "(%d, %d)", xcoords[i], ycoords[i]);
		printf(
			"%9s = 0x%6.6x (0xffffff expected)\n",
			coords, color = getpixel(screen, xcoords[i], ycoords[i])
		);
		assert(color == COLOR_WHITE);
	}

	printf("\ntesting a vertical line\n");
	drawline(screen, 6, 0, 6, 9, COLOR_BLACK);
	xcoords[0] = 6; ycoords[0] = 0;
	xcoords[1] = 6; ycoords[1] = 5;
	xcoords[2] = 6; ycoords[2] = 8;
	ncoords = 3;
	for(i = 0; i < ncoords; i++) {
		sprintf(coords, "(%d, %d)", xcoords[i], ycoords[i]);
		printf(
			"%9s = 0x%6.6x (0x000000 expected)\n",
			coords, color = getpixel(screen, xcoords[i], ycoords[i])
		);
		assert(color == COLOR_BLACK);
	}

	printf("\ntesting a horizontal line\n");
	drawline(screen, 0, 5, 119, 5, COLOR_BLUE);
	xcoords[0] = 0;   ycoords[0] = 5;
	xcoords[1] = 6;   ycoords[1] = 5;
	xcoords[2] = 24;  ycoords[2] = 5;
	xcoords[3] = 118; ycoords[3] = 5;
	ncoords = 4;
	for(i = 0; i < ncoords; i++) {
		sprintf(coords, "(%d, %d)", xcoords[i], ycoords[i]);
		printf(
			"%9s = 0x%6.6x (0x0000ff expected)\n",
			coords, color = getpixel(screen, xcoords[i], ycoords[i])
		);
		assert(color == COLOR_BLUE);
	}

	printf("\nTESTING plot with random colors:\n");
	//unit tests with random variables?
	//yes please! they're seeded with a constant, so they're
	//the same each run!
	int x, y;
	struct rgbcolor a;
	srand(1471103252);
	for(i = 0; i < 25; i++) {
		a.r = rand()%0xff;
		a.g = rand()%0xff;
		a.b = rand()%0xff;
		x = rand()%width;
		y = rand()%height;
		plot(screen, x, y, colortoint(a));
		sprintf(coords, "(%d, %d)", x, y);
		printf(
			"%9s = 0x%6.6x (0x%6.6x expected)\n",
			coords, getpixel(screen, x, y), colortoint(a)
		);

	}

	char filename[256] = "output/UNINITIALIZED.ppm";
	sprintf(filename, "../output/image%lu.ppm", (unsigned long int)time(NULL));
	if(writeppm(filename, width, height, screen->pixels)
		!= 0)
		printf("image write error!\n");
	//printscreen(screen, 0x000000);
	updateWindowSurface(window);
	//printf("5 sec delay\n");
	//delay(5000);
	//printf("done!\n");
}

int WinMain(/*int argc, char *argv[]*/)
{
	testsection("color.h");
	{
		struct rgbcolor c = {
			0xdd,
			0x12,
			0xab
		};
		unsigned int a = colortoint(c);
		TESTING("colortoint");
		printf(
			"color =   {0xdd, 0x12, 0xab}\n"
			"int =     0x%x\n"
			"expected: 0xdd12ab\n\n", a);
		assert(a == 0xdd12ab);

		struct rgbcolor d;
		d = inttocolor(a);
		TESTING("inttocolor");
		printf(
			"int   = 0xdd12ab\n"
			"red   = 0x%x (expected: 0xdd)\n"
			"green = 0x%x (expected: 0x12)\n"
			"blue  = 0x%x (expected: 0xab)\n\n",
			d.r, d.g, d.b);
		assert(d.r   == 0xdd);
		assert(d.g == 0x12);
		assert(d.b  == 0xab);

		//this is really a misuse of the color type
		d.r = bclamp(400.4);
		d.g = bclamp(-21.4959);
		d.b = bclamp(0x40);
		c.r = bclamp(0);
		c.g = bclamp(0xff);

		TESTING("bclamp");
		printf(
			"400.4:    %3d (expected: 255)\n"
			"-21.4959: %3d (expected: 0)\n"
			"0x40:     %3x (expected: 40)\n"
			"0:        %3d (expected: 0)\n"
			"0xff:     %3x (expected: 0)\n\n",
			d.r, d.g, d.b, c.r, c.g
		);
		assert(d.r == 0xff);
		assert(d.g == 0);
		assert(d.b == 0x40);
		assert(c.r == 0x0);
		assert(c.g == 0xff);

		c.g = 0x11;
		d = shifthue(c, 10);

		//the expected results here might not be what youd think
		//that's because the matrix transform im using only approximates
		//hue shifting. these verify the algorithm hasn't changed more than
		//they verify that it's "correct"
		TESTING("shifthue");
		printf(
			"in:       0x%2.2x%2.2x%2.2x\n"
			"out:      0x%2.2x%2.2x%2.2x\n"
			"expected: 0x1000ab\n\n",
			c.r, c.g, c.b,
			d.r, d.g, d.b
		);
		assert(
			   d.r == 0x10
			&& d.g == 0x00
			&& d.b == 0xab
		);

		c.r = 0xff;
		c.g = 0x00;
		c.b = 0x00;
		d = shifthue(c, 180);
		printf( "in:       0x%2.2x%2.2x%2.2x\n"
			"out:      0x%2.2x%2.2x%2.2x\n"
			"expected: 0x00a9aa\n",
			c.r, c.g, c.b,
			d.r, d.g, d.b
		);
		assert(
			   d.r == 0x00
			&& d.g == 0xa9
			&& d.b == 0xaa
		);
	}

	testsection("ppm.h");
	{
		char *filename = "TEST_PPM.ppm";
		int width = 4;
		int height = 3;
		unsigned int image[12] = {
			0x002244, 0x446688, 0x88aabb, 0xbbaaff,
			0x123456, 0x789abc, 0xdef012, 0x456789,
			0xbbbbbb, 0x888888, 0x444444, 0x000000
		};

		printf("writing test file: %s\n", filename);

		int result =
			writeppm(filename, width, height, image);

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
		for(i = 0; (readchar = fgetc(imagein)) != EOF; i++) {
			assert(readchar == expected[i]);
		}
		printf("\n%s wrote as expected!\n", filename);
	}

	testsection("plot.h");
	{
		surface* screen = NULL;
		//Create window
		int width = 120, height = 10;
		screen = new_surface(width, height, 0);
		if(screen == NULL) {
			printf( "Window could not be initialized!\n"
				"Note that this is probably a problem with your"
				" environment rather than a failed test.\n")
		} else {
			//Get window surface
			screentests(screen, window, width, height);
			//printf("width: %d\nheight: %d\n", screen->w, screen->h);
			//fill black
		}
	}

	testsection("vector.h");
	{
		TESTING("2d vector functions");
		vec2 a = {.x =  10, .y = 14};
		vec2 b = {.x = -13, .y = 2};
		TESTING("add");
		vec2 c = add2(a, b);
		printf(
			"a     = {%.0f, %.0f}\n"
			"b     = {%.0f, %.0f}\n"
			"a + b = {%.0f, %.0f} ({-3, 16} expected)\n",
			a.x, a.y,
			b.x, b.y,
			c.x, c.y
		);
		assert((c.x == -3) && (c.y == 16));
		TESTING("perp3");
		vec3 d = const3(1.0F, 0.0F, 0.0F);
		vec3 e = const3(0.0F, 1.0F, 0.0F);
		vec3 perp = perp3(d, e);
		printf(
			"a    = {%f, %f, %f}\n"
			"b    = {%f, %f, %f}\n"
			"perp = {%f, %f, %f}\n",
			d.x, d.y, d.z,
			e.x, e.y, e.z,
			perp.x, perp.y, perp.z
		);
	}

	testsection("quaternion.h");
	{
		TESTING("quaternion multiplication");
		quaternion one = constq(
			5.0F,
			11.0F,
			-17.0F,
			31.0F
		);
		quaternion two = constq(
			7.0F,
			-83.0F,
			41.0F,
			-29.0F
		);
		quaternion result = multq(one, two);
		printf(
			"one =\n"
			"  %.2f\n"
			"+ %.2fi\n"
			"+ %.2fj\n"
			"+ %.2fk\n\n",
			one.r, one.a, one.b, one.c
		);
		printf(
			"two =\n"
			"  %.2f\n"
			"+ %.2fi\n"
			"+ %.2fj\n"
			"+ %.2fk\n\n",
			two.r, two.a, two.b, two.c
		);
		printf(
			"result = one * two =\n"
			"  %.2f  \t(expected  2544.00)\n"
			"+ %.2fi \t(expected -1116.00)\n"
			"+ %.2fj \t(expected -2168.00)\n"
			"+ %.2fk \t(expected -888.00)\n\n",
			result.r, result.a, result.b, result.c
		);
		assert(
			   (int)result.r == 2544
			&& (int)result.a == -1116
			&& (int)result.b == -2168
			&& (int)result.c == -888
		);

		TESTING("quaternion squaring");
		result = sqrq(one);
		printf(
			"one =\n"
			"  %.2f\n"
			"+ %.2fi\n"
			"+ %.2fj\n"
			"+ %.2fk\n\n",
			one.r, one.a, one.b, one.c
		);
		printf(
			"result = one^2\n"
			"  %.2f  \t(expected -1346.00)\n"
			"+ %.2fi \t(expected 110.00)\n"
			"+ %.2fj \t(expected -170.00)\n"
			"+ %.2fk \t(expected 310.00)\n\n",
			result.r, result.a, result.b, result.c
		);
		assert(
			   (int)result.r == -1346
			&& (int)result.a == 110
			&& (int)result.b == -170
			&& (int)result.c == 310
		);
	}

	printf( "\n"
		"================\n"
		"ALL TESTS PASSED\n"
		"if you're reading this, it means you didn't break the build!\n"
		"congrats! :-)\n"
		"consider running `make clean` to delete generated test files\n");
	return 0;
}
