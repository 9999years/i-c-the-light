//functions for dealing with 2 and 3 dimensional vectors
//functions for 2d vectors end with 2
//functions for 3d vectors end with 3
//functions that operate with a scalar end with s
//eg mult2() multiplies two vec2s
//and mult2s() would multiply a vec2 by a scalar
//comparable functions are listed in the order of 2, 3, 2s, 3s
//(ascending dimensions with scalar variants listed last)

/*
 * here's how im setting up the axis (right-handed)
 *      z
 *      |
 *      |
 *      |
 *      |
 *      |
 *      +----------- y
 *     /
 *    /
 *   /
 *  /
 * x
 *
 */

#include <math.h>

#ifndef VECTOR_H
#define VECTOR_H

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
//i have no clue if this works tbh
vec3 fromdirection3(float xy, float yz, float magn)
{
	vec3 ret;
	ret.x = magn * cos(yz) * sin(xy) ;
	ret.y = magn * cos(yz) * cos(xy) ;
	ret.z = magn * sin(yz);
	return ret;
}

//returns the point a given distance along an input vector
//as an offset from the initial vector
//IMPORTANT: if `in` is't a unit vector you're not
//guarenteed the return value will be `distance` long!!!!!
vec2 distalong2(vec2 in, float distance)
{
	vec2 ret;
	ret.x = distance * in.x;
	ret.y = distance * in.y;
	return ret;
}

vec3 distalong3(vec3 in, float distance)
{
	vec3 ret;
	ret.x = distance * in.x;
	ret.y = distance * in.y;
	ret.z = distance * in.z;
	return ret;
}

//perpendicular vectors
//swap components, invert parity of one
vec2 perp2(vec2 in)
{
	vec2 ret;
	ret.x = -in.y;
	ret.y =  in.x;
	return ret;
}

//vector perpendicular to `in`
//almost certainly broken
vec3 perp3(vec3 in)
{
	vec3 ret;
	ret.x = in.z;
	ret.y = in.z;
	ret.z = in.x - in.y;
	//if in = <-1, 1, 0>, it creates a 0 vector
	if((ret.x == 0.0F) && (ret.y == 0.0F) && (ret.z == 0.0F)) {
		ret.x = -in.y - in.z;
		ret.y =  in.x;
		ret.z =  in.x;
	}
	return ret;
}

//invert
vec2 inv2(vec2 in)
{
	vec2 ret;
	ret.x = -in.x;
	ret.y = -in.y;
	return ret;
}

vec3 inv3(vec3 in)
{
	vec3 ret;
	ret.x = -in.x;
	ret.y = -in.y;
	ret.z = -in.z;
	return ret;
}

//max, component wise
vec2 maxvec2(vec2 a, vec2 b)
{
	vec2 ret;
	ret.x = fmax(a.x, b.x);
	ret.y = fmax(a.y, b.y);
	return ret;
}

vec3 maxvec3(vec3 a, vec3 b)
{
	vec3 ret;
	ret.x = fmax(a.x, b.x);
	ret.y = fmax(a.y, b.y);
	ret.z = fmax(a.z, b.z);
	return ret;
}

//max, scalar variant
vec2 maxvec2s(vec2 a, float b)
{
	vec2 ret;
	ret.x = fmax(a.x, b);
	ret.y = fmax(a.y, b);
	return ret;
}

vec3 maxvec3s(vec3 a, float b)
{
	vec3 ret;
	ret.x = fmax(a.x, b);
	ret.y = fmax(a.y, b);
	ret.z = fmax(a.z, b);
	return ret;
}

//min, component wise
vec2 minvec2(vec2 a, vec2 b)
{
	vec2 ret;
	ret.x = fmin(a.x, b.x);
	ret.y = fmin(a.y, b.y);
	return ret;
}

vec3 minvec3(vec3 a, vec3 b)
{
	vec3 ret;
	ret.x = fmin(a.x, b.x);
	ret.y = fmin(a.y, b.y);
	ret.z = fmin(a.z, b.z);
	return ret;
}

//min, s variant
vec2 minvec2s(vec2 a, float b)
{
	vec2 ret;
	ret.x = fmin(a.x, b);
	ret.y = fmin(a.y, b);
	return ret;
}

vec3 minvec3s(vec3 a, float b)
{
	vec3 ret;
	ret.x = fmin(a.x, b);
	ret.y = fmin(a.y, b);
	ret.z = fmin(a.z, b);
	return ret;
}

//absolute value of a vector
vec2 absvec2(vec2 a)
{
	vec2 ret;
	ret.x = a.x > 0 ? a.x : -a.x;
	ret.y = a.y > 0 ? a.y : -a.y;
	return ret;
}

vec3 absvec3(vec3 a)
{
	vec3 ret;
	ret.x = a.x > 0 ? a.x : -a.x;
	ret.y = a.y > 0 ? a.y : -a.y;
	ret.z = a.z > 0 ? a.z : -a.z;
	return ret;
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

//useful for avoiding spurious sqrt() calls
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

vec3 mult3(vec3 a, vec3 b)
{
	vec3 ret;
	ret.x = a.x * b.x;
	ret.y = a.y * b.y;
	ret.z = a.z * b.z;
	return ret;
}

//scalar
vec3 mult3s(vec3 a, float s)
{
	vec3 ret;
	ret.x = a.x * s;
	ret.y = a.y * s;
	ret.z = a.z * s;
	return ret;
}

vec2 mult2s(vec2 a, float s)
{
	vec2 ret;
	ret.x = a.x * s;
	ret.y = a.y * s;
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

vec3 div3(vec3 a, vec3 b)
{
	vec3 ret;
	ret.x = a.x / b.x;
	ret.y = a.y / b.y;
	ret.z = a.z / b.z;
	return ret;
}

vec2 div2s(vec2 a, float s)
{
	vec2 ret;
	ret.x = a.x / s;
	ret.y = a.y / s;
	return ret;
}

vec3 div3s(vec3 a, float s)
{
	vec3 ret;
	ret.x = a.x / s;
	ret.y = a.y / s;
	ret.z = a.z / s;
	return ret;
}

//average
vec2 avg2(vec2 a, vec2 b)
{
	vec2 ret;
	ret.x = (a.x + b.x) / 2;
	ret.y = (a.y + b.y) / 2;
	return ret;
}

vec3 avg3(vec3 a, vec3 b)
{
	vec3 ret;
	ret.x = (a.x + b.x) / 2;
	ret.y = (a.y + b.y) / 2;
	ret.z = (a.z + b.z) / 2;
	return ret;
}

//dot product
float dot2(vec2 a, vec2 b)
{
	return (a.x * b.x) + (a.y * b.y);
}

float dot3(vec3 a, vec3 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
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

vec3 unit3(vec3 a)
{
	float magnitude = magn3(a);
	vec3 ret;
	ret.x = a.x/magnitude;
	ret.y = a.y/magnitude;
	ret.z = a.z/magnitude;
	return ret;
}
#endif //VECTOR_H
