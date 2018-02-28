#include <stdlib.h>
#include <stdio.h>

#ifndef SURFACE_H
#define SURFACE_H

typedef struct rect {
	int x;
	int y;
	int w;
	int h;
} rect;

typedef struct surface {
	unsigned int flags;
	unsigned int format;
	int w, h;
	int pitch;
	void *pixels;

	int locked;
	void *lock_data;

	int refcount;
} surface;

surface* createRGBSurface(
	unsigned int flags,
	int width,
	int height,
	int depth,
	unsigned int rmask,
	unsigned int gmask,
	unsigned int bmask,
	unsigned int amask
);

void *new_surface(
	unsigned int width,
	unsigned int height,
	unsigned int flags
);

int fillRect(
	surface *surface,
	rect *rect,
	unsigned int color
);

void delay(unsigned int ms);

#endif // SURFACE_H
