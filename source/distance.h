//distance estimation functions
#include <stdio.h>
//sin functions
#include <math.h>
#include <float.h>
//vectors
#include "vector.h"
//fclamp
#include "common.h"
#include "quaternion.h"
#include "logging.h"

FILE *logfile;

#ifndef DISTANCE_H
#define DISTANCE_H

//distance from point p to circle of radius r centered at c
float distcircle(vec2 p, vec2 c, float r);

//distance to line ab from p
//http://stackoverflow.com/a/1501725/5719760
float distline2(vec2 p, vec2 a, vec2 b);

//b specifies the box dimensions
//nesting function calls for basic addition makes me feel like
//c was not the best choice for this project
float distbox(vec3 p, vec3 b);

//point, center, radius
float distsphere(vec3 p, vec3 c, float r);

//TODO make the center param do something
float disttorus(vec3 p, vec3 c, float thickness, float radius);

//who needs orientation?
float distground(vec3 p);

//union
float opu(float a, float b);

//subtraction
//requires signed distance functions!!!!!
float ops(float a, float b);

//intersection
float opi(float a, float b);

//displace a point by a wobbly sine shape
float opwobble2(vec2 point, float orig, float freq, float amp);

float opwobble3(vec3 point, float orig, float freq, float amp);

float oprepeat3(/*vec3 point, vec3 period*/);

float distserpenski(vec3 pos);

float distancejulia(vec3 pos, quaternion c, int iterations);

float distancemandlebulb(vec3 pos, int iterations);

#endif //DISTANCE_H
