#include <math.h>
//dealing with 2 and 3 dimensional vectors
//functions for 2d vectors end with 2
//functions for 3d vectors end with 3

#define max(a,b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a > _b ? _a : _b; })

#define min(a,b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a < _b ? _a : _b; })

typedef struct vec2 vec2;
typedef struct vec3 vec3;

struct vec2
{
	float x;
	float y;
};

struct vec3
{
	float x;
	float y;
	float z;
};

vec2 fromdirection2(float radians)
{
	vec2 ret;
	ret.x = cos(radians);
	ret.y = sin(radians);
	return ret;
}

//returns the point a given distance along an input vector
//as an offset from the initial vector
vec2 distalong2(vec2 in, float distance)
{
	vec2 ret;
	ret.x = distance * in.x;
	ret.y = distance * in.y;
	return ret;
}

//distance between two points.
//i mean vectors. haha
//this code is ugly but i didnt want to use extra memory
float dist2(vec2 a, vec2 b)
{
	return sqrt(((b.x - a.x) * (b.x - a.x)) + ((b.y - a.y) * (b.y - a.y)));
}

vec2 add2(vec2 a, vec2 b)
{
	vec2 ret;
	ret.x = a.x + b.x;
	ret.y = a.y + b.y;
	return ret;
}

float magnitude2(vec2 a)
{
	return sqrt((a.x*a.x) + (a.y*a.y));
}

vec2 unit2(vec2 a)
{
	float magnitude = magnitude2(a);
	vec2 ret;
	ret.x = a.x/magnitude;
	ret.y = a.y/magnitude;
	return ret;
}

vec3 add3(vec3 a, vec3 b)
{
	vec3 ret;
	ret.x = a.x + b.x;
	ret.y = a.y + b.y;
	ret.z = a.z + b.z;
	return ret;
}
