#include <math.h>

#define max(a,b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a > _b ? _a : _b; })

#define min(a,b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a < _b ? _a : _b; })

float distcircle(vec2 point, vec2 center, float radius)
{
	float o, a;
	a = point.x - center.x;
	o = point.y - center.y;
	return sqrt((o*o)+(a*a)) - radius;
}

float opu(float a, float b)
{
	return min(a, b);
}

float ops(float a, float b)
{
	return max(-a, b);
}

float opi(float a, float b)
{
	return max(a, b);
}
