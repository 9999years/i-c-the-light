//toggle this line to toggle sdl inclusion
#define USE_REAL_SDL

#ifdef USE_REAL_SDL
//#pragma message "compiling with REAL sdl"
#include <SDL/SDL.h>
#undef main
#else
//#pragma message "compiling with FAKE sdl"
#include "fake_sdl.h"
#endif
