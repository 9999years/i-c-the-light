//repo: https://github.com/9999years/i-c-the-light
//MIT/expat license
//rebecca turner
//consult ../readme.md


//logging, file out
#include <stdio.h>
#include <io.h>
//rand
#include <stdlib.h>
//sin functions
#include <math.h>
//timestamp for file names
#include <time.h>

//display
#include "sdl.h"

//project files
#include "common.h"
#include "color.h"
#include "ppm.h"
#include "vector.h"
#include "distance.h"
#include "quaternion.h"
#include "logging.h"
#include "plot.h"

#ifndef ICTHELIGHT_H
#define ICTHELIGHT_H

#define FRAMES_IN_ROTATION 60.0F

//global
int frame;

//global distance estimator
float de(vec2 pos, const int lframe, float r);

//returns a normal
vec2 getnormal(vec2 pos, float samplesize);

void render(SDL_Surface *screen, const int lframe);
#endif //ICTHELIGHT_H
