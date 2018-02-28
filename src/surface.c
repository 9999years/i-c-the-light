/*
* So... the original SDL source says I have to include this (and I'm happy to)
* but honestly, except createRGBSurface is the only function that resembles an
* SDL original.
*/

/*
* Simple DirectMedia Layer
* Copyright (C) 1997-2016 Sam Lantinga <slouken@libsdl.org>
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "surface.h"

surface* createRGBSurface(
	unsigned int flags,
	int width,
	int height,
	int depth,
	unsigned int rmask,
	unsigned int gmask,
	unsigned int bmask,
	unsigned int amask
)
{
	surface* surf;

	(void)flags;

	surf = (surface*) calloc(1, sizeof(*surf));
	if (surf == NULL) {
		printf("Surface allocation failed! Out of memory.\n");
		return NULL;
	}

	surf->format = 0;
	surf->w = width;
	surf->h = height;
	surf->pitch = surf->w * sizeof(unsigned int);

	if(surf->w && surf->h) {
		surf->pixels = malloc(surf->h * surf->pitch);
		if(surf->pixels == NULL) {
			printf("Pixel allocation failed! Out of memory.\n");
			return NULL;
		}
	}
	surf->refcount = 1;

	return surf;
}

void *new_surface(
	unsigned int width,
	unsigned int height,
	unsigned int flags
)
{
	unsigned int rmask, gmask, bmask, amask;
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
	return (void *) createRGBSurface(
		flags,
		width, height,
		32, // bit depth
		rmask, gmask, bmask, amask
	);
}

int fillRect(
	surface* surf,
	rect *rect,
	unsigned int color
)
{
	//start x, y, end x, y, indicies
	int sx, sy, ex, ey, i, j;
	if(rect == NULL) {
		sx = sy = 0;
		ex = surf->w;
		ey = surf->h;
	}
	for(i = sy; i < ey; i++) {
	for(j = sx; j < ex; j++) {
		((unsigned int *)surf->pixels)[j + i * surf->w] = color;
	}
	}
	return 0;
}

void delay(unsigned int ms)
{
#ifdef Sleep
	Sleep(ms);
#elif defined(nanosleep)
	int s = ms / 1000;
	int ns = (ms % 1000) * 1000000;
	struct timespec spec = {
		tv_sec = (time_t)s;
		tv_nsec = (long)ns;
	};
	nanosleep(spec, NULL);
#endif
	return;
}
