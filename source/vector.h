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

//returns a vector in a given direction
//takes in an angle and a magnitude
vec2 fromdirection2(float xy, float magn)
{
	vec2 ret;
	ret.x = magn * cos(xy);
	ret.y = magn * sin(xy);
	return ret;
}

vec3 fromdirection3(float xy, float yz, float magn)
{
	vec3 ret;
	ret.x = magn * sin(xy) * cos(yz);
	ret.y = magn * cos(xy) * cos(xz);
	ret.z = magn * sin(yz);
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

vec3 distalong3(vec3 in, float distance)
{

}

//distance between two points.
//i mean vectors. haha
float dist2(vec2 a, vec2 b)
{
	return sqrt(
		  ((b.x - a.x) * (b.x - a.x))
		+ ((b.y - a.y) * (b.y - a.y))
		);
}

float dist3(vec3 a, vec3 b)
{
	return sqrt(
		  ((b.x - a.x) * (b.x - a.x))
		+ ((b.y - a.y) * (b.y - a.y))
		+ ((b.z - a.z) * (b.z - a.z))
		);
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

