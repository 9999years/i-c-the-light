//repo: https://github.com/9999years/i-c-the-light
//MIT/expat license
//rebecca turner
//consult ../readme.md

#ifndef MAIN_H
#define MAIN_H

#define SCREEN_HEIGHT 500
#define SCREEN_WIDTH  500

//render code
#include "icthelight.h"

//globals
FILE *plotfile;

void saveframe(SDL_Surface *screen);

int handleevents(SDL_Surface *screen);

//int main(int argc, char *argv[]);
#endif //MAIN_H
