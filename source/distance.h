//distance estimation functions
#include <math.h>
#include "trig.h"

float min(float a, float b) {
	return a < b ? a : b;
}

float max(float a, float b) {
	return a > b ? a : b;
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
