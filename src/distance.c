//distance estimation functions
#include "distance.h"

//distance from point p to circle of radius r centered at c
float distcircle(vec2 p, vec2 c, float r)
{
	float o, a;
	a = p.x - c.x;
	o = p.y - c.y;
	//dist to c
	return sqrt((o * o) + (a * a)) - r;
}

//distance to line ab from p
//http://stackoverflow.com/a/1501725/5719760
float distline2(vec2 p, vec2 a, vec2 b)
{
	//length = |b-a|^2 -  avoid a sqrt
	const float length = distsqr2(a, b);
	//avoid a = b case & divide by zero
	if(length == 0.0F) return dist2(p, a);
	const vec2 pa = sub2(p, a);
	const vec2 ba = sub2(b, a);
	//Consider the line extending the segment, parameterized as
	//a + t (b - a)
	//we find projection of point c onto the line.
	//It falls where t = [(p-a) . (b-a)] / |b-a|^2
	//we clamp t from [0,1] to handle points outside the segment ab
	const float t = fclamp(
		dot2(pa, ba) / length,
		0, 1);
	//projection falls on the segment
	const vec2 projection = add2(a, mult2s(ba, t));
	return dist2(
		p,
		projection
	);
}

//b specifies the box dimensions
//nesting function calls for basic addition makes me feel like
//c was not the best choice for this project
float distbox(vec3 p, vec3 b)
{
	//magnitude(max(abs(p) - b, 0))
	//vec3 ret;
	//ret = sub3(
		//absvec3(p),
		//b
	//);
	//ret = maxvec3s(ret, 0.0F);
	return magn3(
		maxvec3s(
			sub3(
				absvec3(p),
				b
			),
			0.0F
		)
	);
}

//point, radius
float distsphere(vec3 p, float r)
{
	return (magn3(p) - r);
}

//TODO make the center param do something
float disttorus(vec3 p, vec3 c, float thickness, float radius)
{
	p = sub3(p, c);
	vec2 q;
	q.x = magn2((vec2){.x = p.x, .y = p.z}) - radius;
	q.y = p.y;
	return magn2(q) - thickness;
}

//who needs orientation?
float distground(vec3 p)
{
	return p.y;
}

//union
float opu(float a, float b)
{
	return min(a, b);
}

//subtraction
//requires signed distance functions!!!!!
float ops(float a, float b)
{
	return max(-a, b);
}

//intersection
float opi(float a, float b)
{
	return max(a, b);
}

//displace a point by a wobbly sine shape
float opwobble2(vec2 point, float freq, float amp)
{
	return amp
		* sin(point.x / freq)
		* sin(point.y / freq);
}

float opwobble3(vec3 point, float freq, float amp)
{
	return amp
		* sin(point.x / freq)
		* sin(point.y / freq)
		* sin(point.z / freq);
}

float oprepeat3(/*vec3 point, vec3 period*/)
{
	//this function, obviously, does nothing
	return -10000.0F;
}

float distserpenski(vec3 pos)
{
	const int iterations = 10;
	const float bailout = 100.0F, scale = 2.0F;
	float r = dot3(pos, pos);
	int n;
	//vec3 offset = const3(1, 1, 1);
	vec3 tmp;
	float tmpf;
	for(n = 0; n < iterations && r < bailout; n++) {
		if(pos.x + pos.y < 0) {
			tmpf  = -pos.y;
			pos.y = -pos.x;
			pos.x = tmpf;
		}
		if(pos.x + pos.z < 0) {
			tmpf  = -pos.z;
			pos.z = -pos.x;
			pos.x = tmpf;
		}
		if(pos.y + pos.z < 0) {
			tmpf  = -pos.z;
			pos.z = -pos.y;
			pos.y = tmpf;
		}
		//tmp = sub3(
			//mult3s(pos, scale),
			//mult3s(offset, scale - 1.0F)
		//);
		tmp = sub3s(
			mult3s(pos, scale),
			scale - 1.0F
		);
		pos = tmp;
		r = dot3(pos, pos);
	}
	return (sqrt(r) - 2.0F) * pow(scale, -(float)n);
	//return magn3(pos)
		//* pow(scale, -(float)n);
}

float distancejulia(vec3 pos, quaternion c, int iterations)
{
	float distance;
	int i;
	//fprintf(logfile,
		//"\npos:\n"
		//"    %.2f\n"
		//"    %.2f\n"
		//"    %.2f\n",
		//pos.x, pos.y, pos.z
	//);
	//keep one component fixed to view a 3d "slice" of the 4d fractal
	quaternion q = constq(pos.x, pos.y, pos.z, 0.0F);
	//q prime, the running derivative of q
	//why does this start at real 1? "it's complicated"
	quaternion qp = constq(1.0F, 0.0F, 0.0F, 0.0F);
	quaternion tmp;
	float last = -FLT_MAX;
	for(i = 0; i < iterations; i++) {
		//q' = 2*q*q'
		tmp = multq(q, qp);
		qp = multqs(tmp, 2.0F);

		//q = q^2 + c
		tmp = addq(sqrq(q), c);
		q = tmp;

		//if the distance decreases, get HYPED
		//if(dotq(q) < last) {
			//printf(logfile, "\nDECREASED!!\n");
		//}

		last = dotq(q);

		//fprintf(logfile, "\ni = %d, q:\n", i);
		//dumpquaternion(q);
		//fprintf(logfile, "q':\n");
		//dumpquaternion(qp);
		//fprintf(logfile, "|q|: %f\n", last);

		if(last > 16.0F)
			break;
	}
	//|q| log(|q|)
	//------------ = distance
	//   2|q'|
	float qmag = magnq(q);
	distance = (qmag * log(qmag)) / (2.0F * magnq(qp));
	//fprintf(logfile, "final distance:\n"
		//"    %.2f\n", distance);
	return distance;
}

float distmandlebulb(vec3 pos, int iterations)
{
	printf("don't call this!");
	return INFINITY;
}
