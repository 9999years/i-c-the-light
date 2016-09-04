#include <math.h>

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
