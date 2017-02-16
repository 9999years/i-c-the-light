#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef FAKE_Screen_H
#define FAKE_Screen_H

typedef struct Screen_Rect {
	int x;
	int y;
	int w;
	int h;
} Screen_Rect;

typedef struct Screen_Surface {
	int w, h;
	int pitch;
	void *pixels;
} Screen_Surface;

Screen_Surface *Screen_CreateRGBSurface(
	int width,
	int height
);

int Screen_FillRect(
	Screen_Surface *surface,
	unsigned int color
);

void Screen_Delay(unsigned int ms);

#endif //FAKE_Screen_H
