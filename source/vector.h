//functions for dealing with 2 and 3 dimensional vectors
//functions for 2d vectors end with 2
//functions for 3d vectors end with 3
#include <math.h>

typedef struct vec2 vec2;
typedef struct vec3 vec3;

//2d vector
struct vec2
{
	float x;
	float y;
};

//3d vector
struct vec3
{
	float x;
	float y;
	float z;
};

//sqrt function
//dont ask me why or how this works
unsigned int root(unsigned int x)
{
	unsigned int a, b;
	b = x;
	a = x = 0x3f;
	x = b / x;
	a = x = (x + a) >> 1;
	x = b / x;
	a = x = (x + a) >> 1;
	x = b / x;
	x = (x + a) >> 1;
	return x;
}

//returns a unit director pointing in a given direction
vec2 fromdirection2(float radians)
{
	vec2 ret;
	ret.x = cos(radians);
	ret.y = sin(radians);
	return ret;
}

//returns a unit vector pointing in a given direction
//takes an angle in the xy plane and an angle in the
//xz plane
vec3 fromdirection3(float xy, float xz)
{
	vec3 ret;
	ret.x = cos(xy);
	ret.y = sin(xy);
	//3d is basically 2d so this is fine
	ret.z = 0.0F;
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
//this code is ugly but acceptable
float dist2(vec2 a, vec2 b)
{
	return sqrt(((b.x - a.x) * (b.x - a.x)) + ((b.y - a.y) * (b.y - a.y)));
}

//adds 2 vectors
vec2 add2(vec2 a, vec2 b)
{
	vec2 ret;
	ret.x = a.x + b.x;
	ret.y = a.y + b.y;
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

//returns the magnitude of a vector
float magnitude2(vec2 a)
{
	return sqrt((a.x*a.x) + (a.y*a.y));
}

//returns the unit vector for any given vector
vec2 unit2(vec2 a)
{
	float magnitude = magnitude2(a);
	vec2 ret;
	ret.x = a.x/magnitude;
	ret.y = a.y/magnitude;
	return ret;
}

