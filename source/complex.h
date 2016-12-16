#include <math.h>

#ifndef COMPLEX_H
#define COMPLEX_H

typedef struct complex {
	double a;
	double b;
} complex;

//handy for avoiding sqrt calls
double complexabssqr(complex in)
{
	return in.a * in.a + in.b * in.b;
}

//absolute / normal / modulo
double complexabs(complex in)
{
	return sqrt(in.a * in.a + in.b * in.b);
}

complex complexmult(complex a, complex b)
{
	complex ret;
	ret.a = a.a * b.a - a.b * b.b;
	ret.b = a.a * b.b + a.b * b.a;
	return ret;
}

complex complexmultscalar(complex a, float b)
{
	complex ret;
	ret.a = a.a * b;
	ret.b = a.b * b;
	return ret;
}

complex complexadd(complex a, complex b)
{
	complex ret;
	ret.a = a.a + b.a;
	ret.b = a.b + b.b;
	return ret;
}

complex complexsub(complex a, complex b)
{
	complex ret;
	ret.a = a.a - b.a;
	ret.b = a.b - b.b;
	return ret;
}

float distmandlebrot(complex c, int iterations)
{
	complex z  = {.a = 0.0F, .b = 0.0F};
	complex dz = {.a = 0.0F, .b = 0.0F};
	complex tmp;

	//const complex one = {.a = 1.0F, .b = 0.0F};
	//const complex two = {.a = 2.0F, .b = 0.0F};

	float msqr;
	int i;
	for(i = 0; i < iterations; i++) {
		//z = z^2 + c
		//ergo
		//dz = 2 * z * dz + 1

		//dz = complexmult(z, dz);

		//z * dz
		tmp.a = z.a * dz.a - z.b * dz.b;
		tmp.b = z.a * dz.b + z.b * dz.a;
		//dz = complexmultscalar(dz, 2.0F);
		dz.a = tmp.a * 2.0F + 1.0F;
		dz.b = tmp.b * 2.0F;

		//z = z * z + c
		tmp.a = z.a * z.a - z.b * z.b;
		tmp.b = z.a * z.b + z.b * z.a;
		//z = complexadd(complexmult(z, z), c);
		z.a = tmp.a + c.a;
		z.b = tmp.b + c.b;

		//msqr = complexabssqr(z);
		msqr = z.a * z.a + z.b * z.b;
		if(msqr > 1024.0F) {
			break;
		}
	}

	//G/|G'|
	return sqrt(msqr / (dz.a * dz.a + dz.b * dz.b)) * log(msqr);
}
#endif //COMPLEX_H
