#include <math.h>

float distcircle(vec2 point, vec2 center, float radius)
{
	float o, a;
	a = point.x - center.x;
	o = point.y - center.y;
	return sqrt((o*o)+(a*a)) - radius;
}
