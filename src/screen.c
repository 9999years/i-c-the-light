#include "screen.h"

#ifndef SCREEN_H
#define SCREEN_H

int Screen_DestroyWindow(Screen_Window *window)
{
	return 0;
}

Screen_Surface *Screen_CreateRGBSurface(
	int width,
	int height
)
{
	Screen_Surface *surface;

	surface = (Screen_Surface *)calloc(1, sizeof(*surface));
	if (surface == NULL) {
		printf("Surface allocation failed! Out of memory.\n");
		return NULL;
	}

	surface->format = 0;
	surface->w = width;
	surface->h = height;
	surface->pitch = surface->w * sizeof(unsigned int);

	if(surface->w && surface->h) {
		surface->pixels = malloc(surface->h * surface->pitch);
		if(surface->pixels == NULL) {
			printf("Pixel allocation failed! Out of memory.\n");
			return NULL;
		}
	}
	surface->map = NULL;
	surface->refcount = 1;

	return surface;
}

int Screen_FillRect(
	Screen_Surface *surface,
	unsigned int color
)
{
	//start x, y, end x, y, indicies
	int sx, sy, ex, ey, i, j;
	if(rect == NULL) {
		sx = sy = 0;
		ex = surface->w;
		ey = surface->h;
	}
	for(i = sy; i < ey; i++) {
	for(j = sx; j < ex; j++) {
		((unsigned int *)surface->pixels)[j + i * surface->w] = color;
	}
	}
	return 0;
}

void Delay(unsigned int ms)
{
#ifdef Sleep
	Sleep(ms);
#elif defined(nanosleep)
	unsigned int s = ms / 1000;
	unsigned int ns = (ms % 1000) * 1000000;
	struct timespec spec = {
		tv_sec = (time_t)s;
		tv_nsec = (long)ns;
	};
	nanosleep(spec, NULL);
#endif
	return;
}

#endif //SCREEN_H
