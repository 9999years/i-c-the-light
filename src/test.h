#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "color.h"
#include "surface.h"
#include "line.h"
#include "ppm.h"
#include "vector.h"
#include "quaternion.h"

#ifndef TEST_H
#define TEST_H

#define COLOR_RED 0xff0000
#define COLOR_BLACK 0x000000
#define COLOR_BLUE 0x0000ff
#define COLOR_WHITE 0xffffff
#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define TESTING(function) printf("\nTESTING %s:\n", function);

void printscreen(surface *screen, unsigned int exclude);

void testsection(char name[]);

void screentests(surface *screen, window *window, int width, int height);

int main(/*int argc, char *argv[]*/);

#endif //TEST_H
