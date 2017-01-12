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
#include <stdio.h>
#include <math.h>
#include "logging.h"
#ifndef VECTOR_H
#define VECTOR_H
typedef struct vec2 vec2;
typedef struct vec3 vec3;
typedef struct vec4 vec4;
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
void dump3(vec3 a);

//construction methods
vec2 const2(float x, float y);

vec3 const3(float x, float y, float z);

//returns a vector in a given direction
//takes in an angle and a magnitude
vec2 fromdirection2(float xy, float magn);

//from x to y, y to z
//eg pointing straight along y-axis would be
//xy = 1, yz = 0
//i have no clue if this works tbh
vec3 fromdirection3(float xy, float yz, float magn);

//returns the magnitude of a vector
float magn2(vec2 a);

float magn3(vec3 a);

//returns the unit vector for any given vector
vec2 unit2(vec2 a);

vec3 unit3(vec3 a);

//vector through two points
//not a unit vector!
//this is just Δx, Δy, etc...
//a more semantic b - a
vec2 through2(vec2 a, vec2 b);

vec3 through3(vec3 a, vec3 b);

//perpendicular vectors
//these are the same length as the input vector, probably
//swap components, invert parity of one
vec2 perp2(vec2 in);

//vector perpendicular to `a` and `b`
//this is just a cross product
//math.SE: https://goo.gl/pdn98S
vec3 perp3(vec3 a, vec3 b);

//invert
vec2 inv2(vec2 in);

vec3 inv3(vec3 in);

//max, component wise
vec2 maxvec2(vec2 a, vec2 b);

vec3 maxvec3(vec3 a, vec3 b);

//max, scalar variant
vec2 maxvec2s(vec2 a, float b);

vec3 maxvec3s(vec3 a, float b);

//min, component wise
vec2 minvec2(vec2 a, vec2 b);

vec3 minvec3(vec3 a, vec3 b);

//min, s variant
vec2 minvec2s(vec2 a, float b);

vec3 minvec3s(vec3 a, float b);

//absolute value of a vector
vec2 absvec2(vec2 a);

vec3 absvec3(vec3 a);

//distance between two points.
//i mean vectors. haha
float dist2(vec2 a, vec2 b);

float dist3(vec3 a, vec3 b);

//useful for avoiding spurious sqrt() calls
float distsqr2(vec2 a, vec2 b);

float distsqr3(vec3 a, vec3 b);

//addition
vec2 add2(vec2 a, vec2 b);

vec3 add3(vec3 a, vec3 b);

vec3 add3s(vec3 a, float b);

//subtraction
vec2 sub2(vec2 a, vec2 b);

vec3 sub3(vec3 a, vec3 b);

vec3 sub3s(vec3 a, float b);

//multiplication
vec2 mult2(vec2 a, vec2 b);

vec3 mult3(vec3 a, vec3 b);

//scalar
vec3 mult3s(vec3 a, float s);

vec2 mult2s(vec2 a, float s);

//division
vec2 div2(vec2 a, vec2 b);

vec3 div3(vec3 a, vec3 b);

vec2 div2s(vec2 a, float s);

vec3 div3s(vec3 a, float s);

//average
vec2 avg2(vec2 a, vec2 b);

vec3 avg3(vec3 a, vec3 b);

//dot product
float dot2(vec2 a, vec2 b);

float dot3(vec3 a, vec3 b);
#endif //VECTOR_H
