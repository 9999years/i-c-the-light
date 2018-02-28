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
//base path for file  writing
char outfile_base[256];

void saveframe(surface *screen);

int handleevents(surface *screen);

//int main(int argc, char *argv[]);
#endif //MAIN_H
