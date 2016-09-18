#include <math.h>

typedef struct complex {
	double a;
	double b;
} complex;

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

int mandlebrot(complex c, int i)
{
	complex tmp, z;
	z = c;
	while(i --> 0)
	{
		//square z & store in tmp, add c and store in z
		complexmult(&tmp, z, z);
		complexadd(&z, tmp, c);
		//printf("(i = %d) %f + %fi\n", i, z.a, z.b);
		//printf("complex: %f\n", complexabs(z));
	}
	float ret = complexabs(z);
	if(!isfinite(ret))
		ret = 0xff;
	return ret >= 2 ? 0 : 1;
}
