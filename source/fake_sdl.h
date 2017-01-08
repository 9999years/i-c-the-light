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
*
* So... the original SDL source says I have to include this (and I'm happy to)
* but honestly, except for SDL_CreateRGBSurface this file is entirely stub
* functions, and even that is... minimal, at 22 lines.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef FAKE_SDL_H
#define FAKE_SDL_H
#ifndef SDL_QUIT

typedef void *SDL_Window;

typedef struct SDL_Rect {
	int x;
	int y;
	int w;
	int h;
} SDL_Rect;

typedef struct SDL_Surface {
	unsigned int flags;
	unsigned int format;
	int w, h;
	int pitch;
	void *pixels;

	void *userdata;

	int locked;
	void *lock_data;

	SDL_Rect clip_rect;

	void *map;

	int refcount;
} SDL_Surface;

typedef struct SDL_Keysym {
	int scancode;
	int sym;
	unsigned int mod;
} SDL_Keysym;

typedef struct SDL_KeyboardEvent {
	unsigned int type;
	unsigned int timestamp;
	unsigned int windowID;
	unsigned int state;
	unsigned int repeat;
	unsigned int padding2;
	unsigned int padding3;
	SDL_Keysym keysym;
} SDL_KeyboardEvent;

typedef union SDL_Event
{
    unsigned int type;
    SDL_KeyboardEvent key;
    unsigned int padding[56];
} SDL_Event;

enum {
	SDL_KEYDOWN,
	SDL_QUIT,
	SDLK_ESCAPE,
	SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f, SDLK_g, SDLK_h, SDLK_i,
	SDLK_j, SDLK_k, SDLK_l, SDLK_m, SDLK_n, SDLK_o, SDLK_p, SDLK_q, SDLK_r,
	SDLK_s, SDLK_t, SDLK_u, SDLK_v, SDLK_w, SDLK_x, SDLK_y, SDLK_z,
	SDL_INIT_VIDEO,
	SDL_WINDOWPOS_UNDEFINED,
	SDL_WINDOW_SHOWN,
	SDL_BIG_EDIAN,
	SDL_BYTEORDER,
};

int SDL_Quit()
{
	return 0;
}

int SDL_UpdateWindowSurface()
{
	return 0;
}

int SDL_Init(int flags)
{
	return 0;
}

int SDL_DestroyWindow(SDL_Window *window)
{
	return 0;
}

char *SDL_GetError()
{
	return 0;
}

SDL_Surface *SDL_CreateRGBSurface(
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
	SDL_Surface *surface;

	(void)flags;

	surface = (SDL_Surface *)calloc(1, sizeof(*surface));
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

void *SDL_CreateWindow(
	char *title,
	unsigned int xwindowpos,
	unsigned int ywindowpos,
	unsigned int width,
	unsigned int height,
	unsigned int flags
)
{
	printf("Fake SDL library used, window NOT being created\n");
	unsigned int rmask, gmask, bmask, amask;
#define SCREEN_BIT_DEPTH 32
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
	return SDL_CreateRGBSurface(
		0,
		width, height,
		SCREEN_BIT_DEPTH,
		rmask, gmask, bmask, amask
	);
}

void *SDL_GetWindowSurface(SDL_Window window)
{
	//it was a surface all along!
	return (SDL_Surface *)window;
}

int SDL_FillRect(
	SDL_Surface *surface,
	SDL_Rect *rect,
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

void SDL_Delay(unsigned int ms)
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

void *SDL_PollEvent(void *event)
{
	return NULL;
}

#endif
#endif //FAKE_SDL_H
