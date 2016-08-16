#include <math.h>
//dealing with 2 and 3 dimensional vectors

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

vec2 add2(vec2 a, vec2 b)
{
	vec2 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return result;
}

float magnitude2(vec2 a)
{
	return sqrt((a.x*a.x) + (a.y*a.y));
}

vec2 unit2(vec2 a)
{
	float magnitude_a = magnitude2(a);
	return (vec2)
	{
		.x = a.x/magnitude_a,
		.y = a.y/magnitude_a
	};
}

vec3 add3(vec3 a, vec3 b)
{
	vec3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
