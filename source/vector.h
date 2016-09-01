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

//from x to y, y to z
//eg pointing straight along y-axis would be
//xy = 1, yz = 0
vec3 fromdirection3(float xy, float yz, float magn)
{
	vec3 ret;
	ret.x = magn * sin(xy) * cos(yz);
	ret.y = magn * cos(xy) * cos(yz);
	ret.z = magn * sin(yz);
	return ret;
}

//returns the point a given distance along an input vector
//as an offset from the initial vector
//IMPORTANT: if `in` is't a unit vector you're not
//guarenteed ret will be `distance` long!!!!!
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

float distsqr2(vec2 a, vec2 b)
{
	return
		  ((b.x - a.x) * (b.x - a.x))
		+ ((b.y - a.y) * (b.y - a.y));
}

float distsqr3(vec3 a, vec3 b)
{
	return
		  ((b.x - a.x) * (b.x - a.x))
		+ ((b.y - a.y) * (b.y - a.y))
		+ ((b.z - a.z) * (b.z - a.z));
}

//addition
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

//subtraction
vec2 sub2(vec2 a, vec2 b)
{
	vec2 ret;
	ret.x = a.x - b.x;
	ret.y = a.y - b.y;
	return ret;
}

vec3 sub3(vec3 a, vec3 b)
{
	vec3 ret;
	ret.x = a.x - b.x;
	ret.y = a.y - b.y;
	ret.z = a.z - b.z;
	return ret;
}

//multiplication
vec2 mult2(vec2 a, vec2 b)
{
	vec2 ret;
	ret.x = a.x * b.x;
	ret.y = a.y * b.y;
	return ret;
}

vec2 mult2scalar(vec2 a, float s)
{
	vec2 ret;
	ret.x = a.x * s;
	ret.y = a.y * s;
	return ret;
}

vec3 mult3(vec3 a, vec3 b)
{
	vec3 ret;
	ret.x = a.x * b.x;
	ret.y = a.y * b.y;
	ret.z = a.z * b.z;
	return ret;
}

vec3 mult3scalar(vec3 a, float s)
{
	vec3 ret;
	ret.x = a.x * s;
	ret.y = a.y * s;
	ret.z = a.z * s;
	return ret;
}

//division
vec2 div2(vec2 a, vec2 b)
{
	vec2 ret;
	ret.x = a.x / b.x;
	ret.y = a.y / b.y;
	return ret;
}

vec2 div2scalar(vec2 a, float s)
{
	vec2 ret;
	ret.x = a.x / s;
	ret.y = a.y / s;
	return ret;
}

vec3 div3(vec3 a, vec3 b)
{
	vec3 ret;
	ret.x = a.x / b.x;
	ret.y = a.y / b.y;
	ret.z = a.z / b.z;
	return ret;
}

vec3 div3scalar(vec3 a, float s)
{
	vec3 ret;
	ret.x = a.x / s;
	ret.y = a.y / s;
	ret.z = a.z / s;
	return ret;
}

//dot product
float dot2(vec2 a, vec2 b)
{
	return (a.x * b.x) + (a.y * b.y);
}

//returns the magnitude of a vector
float magn2(vec2 a)
{
	return sqrt((a.x*a.x) + (a.y*a.y));
}

float magn3(vec3 a)
{
	return sqrt((a.x*a.x) + (a.y*a.y) + (a.z*a.z));
}

//returns the unit vector for any given vector
vec2 unit2(vec2 a)
{
	float magnitude = magn2(a);
	vec2 ret;
	ret.x = a.x/magnitude;
	ret.y = a.y/magnitude;
	return ret;
}

