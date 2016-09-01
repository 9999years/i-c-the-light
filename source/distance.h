//distance estimation functions
#include <math.h>
#include "trig.h"

//I Can't Believe It's Not Sin(x)
float parabola(float x)
{
	x -= 1;
	return (-x * x) + 1;
}

float min(float a, float b)
{
	return a < b ? a : b;
}

float max(float a, float b)
{
	return a > b ? a : b;
}

float fclamp(float val, float fmin, float fmax)
{
	return min(max(val, fmin), fmax);
}

//displace a point by a wobbly sine shape
float sindisplace2(vec2 point, float orig, float freq, float amp)
{
	return amp * sin((float)point.x / freq) * sin((float)point.y / freq) + orig;
}

//distance from a circle
float distcircle(vec2 point, vec2 center, float radius)
{
	float o, a;
	a = point.x - center.x;
	o = point.y - center.y;
	//return root(o * o + a * a) - radius;
	return sqrt((o*o)+(a*a)) - radius;
}

//distance to line ab from c
//http://stackoverflow.com/a/1501725/5719760
float distline2(vec2 a, vec2 b, vec2 c)
{
	//length = |b-a|^2 -  avoid a sqrt
	const float length = distsqr2(a, b);
	//avoid a = b case & divide by zero
	if(length == 0.0F) return dist2(c, a);
	const vec2 ca = sub2(c, a);
	const vec2 ba = sub2(b, a);
	//Consider the line extending the segment, parameterized as
	//a + t (b - a)
	//we find projection of point c onto the line.
	//It falls where t = [(c-a) . (b-a)] / |b-a|^2
	//we clamp t from [0,1] to handle points outside the segment ab.
	const float t =
		fclamp(
			dot2(ca, ba) / length
		, 0, 1);
	//projection falls on the segment
	const vec2 projection =
		add2(
			a,
			mult2scalar(
				ba,
				t
			)
		);
	return dist2(
		c,
		projection
		);
	//return dist2(c, projection);
}

//union
float opu(float a, float b)
{
	return min(a, b);
}

//subtraction
float ops(float a, float b)
{
	return max(-a, b);
}

//intersection
float opi(float a, float b)
{
	return max(a, b);
}
