#include "complex.h"

double complexabs(complex in)
{
	return sqrt(in.a * in.a + in.b * in.b);
}

void complexmult(complex *out, complex a, complex b)
{
	(*out).a = a.a * b.a - a.b * b.b;
	(*out).b = a.a * b.b + a.b * b.a;
	return;
}

void complexadd(complex *out, complex a, complex b)
{
	(*out).a = a.a + b.a;
	(*out).b = a.b + b.b;
	return;
}

int mandelbrot(complex c, int i)
{
	complex tmp, z;
	z = c;
	while(i --> 0)
	{
		//square z & store in tmp, add c and store in z
		complexmult(&tmp, z, z);
		complexadd(&z, tmp, c);
	}
	double ret = complexabs(z);
	if(!isfinite(ret))
		ret = 0xff;
	return ret >= 2.0F ? 0 : 1;
}

double distmandelbrot(complex c, int iterations)
{
	complex z  = {.a = 0.0F, .b = 0.0F};
	complex dz = {.a = 0.0F, .b = 0.0F};
	complex tmp;

	//const complex one = {.a = 1.0F, .b = 0.0F};
	//const complex two = {.a = 2.0F, .b = 0.0F};

	long double msqr;
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
