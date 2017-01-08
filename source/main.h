//repo: https://github.com/9999years/i-c-the-light
//MIT/expat license
//rebecca turner
//consult ../readme.md

//render code
#include "icthelight.h"
#include "fake_sdl.h"

#ifndef MAIN_H
#define MAIN_H

//globals
FILE *plotfile;

void saveframe(SDL_Surface *screen);

int handleevents(SDL_Surface *screen);

int main(int argc, char *argv[]);
#endif //MAIN_H
