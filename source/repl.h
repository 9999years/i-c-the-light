//repo here: https://github.com/9999years/i-c-the-light
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
#include <string.h>

//project files
#include "common.h"
//#include "color.h"
//#include "ppm.h"
//#include "line.h"
#include "vector.h"
#include "distance.h"
#include "quaternion.h"
#include "logging.h"

#ifndef REPL_H
#define REPL_H

//globals
int frame = 0;

char *getline(void);

double getnumber(char *message);

int main(/*int argc, char* args[]*/);
#endif //REPL_H
