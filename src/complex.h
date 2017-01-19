#include <math.h>

#ifndef COMPLEX_H
#define COMPLEX_H

typedef struct complex {
	double a;
	double b;
} complex;

double complexabs(complex in);

void complexmult(complex *out, complex a, complex b);

void complexadd(complex *out, complex a, complex b);

int mandelbrot(complex c, int i);

double distmandelbrot(complex c, int iterations);

#endif //COMPLEX_H
