//repo: https://github.com/9999years/i-c-the-light
//rebecca turner
#include "icthelight.h"

//global distance estimator
float de(vec3 pos)
{
	//return distancejulia(pos, juliaconstant, iterations);
	return distsphere(pos, 1.0F) + opwobble3(pos, 3.33F, 0.8F);
}

//returns a normal
//why does this work????
vec3 getnormal(vec3 pos, float samplesize)
{
	vec3 ret;
	vec3 xunit = {
		.x = samplesize,
		.y = 0.0F,
		.z = 0.0F
	};
	vec3 yunit = {
		.x = 0.0F,
		.y = samplesize,
		.z = 0.0F
	};
	vec3 zunit = {
		.x = 0.0F,
		.y = 0.0F,
		.z = samplesize
	};
	ret.x = de(add3(pos, xunit)) - de(sub3(pos, xunit));
	ret.y = de(add3(pos, yunit)) - de(sub3(pos, yunit));
	ret.z = de(add3(pos, zunit)) - de(sub3(pos, zunit));
	return unit3(ret);
}

//takes a light vector and a position vector
//the position vector is the distance between the camera and the point on the
//surface (vec V here:
//https://en.m.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model#Description
//can usually be found with )
float blinnphong(vec3 cam, vec3 pos, vec3 rot, vec3 light)
{
	//k_s: specular constant
#define k_s 1.0F
	//k_d: diffuse constant
#define k_d 0.5F
	//k_a: ambient constant
#define k_a 0.0F
	//α: shininess constant (larger = smaller highlight)
#define alpha 4.0F
	//i_s: specular intensity
#define specular_intensity 1.0F
	//i_d: diffuse intensity
#define diffuse_intensity 1.0F
	//i_a: ambient intensity
#define ambient_intensity 0.0F
	//L: light vector, from surface to light
	//N: normal
	//R: reflection vector
	//V: viewer vector
	//phong:
	//k_a * i_a + k_d * (L · N) * i_d + k_s * (R · V)^α * i_s
	//blinn-phong:
	//k_a * i_a + k_d * (L · N) * i_d + k_s * (N · H)^α * i_s
	cam = unit3(sub3(cam, pos));
	//pos = unit3(pos);
	//vec3 normal = getnormal(sub3(pos, mult3s(unit3(rot), 2.0F)), 10.0F);
	vec3 normal = getnormal(pos, 0.01F);
	//vec3 halfway = div3s(add3(cam, light), magn3(add3(cam, light)));
	//vec3 halfway = avg3(cam, light);
	vec3 halfway = unit3(add3(cam, light));
	float ret =
		dot3(light, normal) * diffuse_intensity
		+ pow(dot3(normal, halfway), alpha) * specular_intensity;
	//ret *= randf(0.0F, 1.F);
	return ret;
}

void render(SDL_Surface *screen, const int lframe)
{
	printf(
	"\n\n"
"====== %.5d ==================================================================="
	"\n", lframe
	);
	//double width = 60.0F / ((double)frame + 20.0F);
	//         3
	//w = -----------
	//         (x/20 - 5)
	//    1 + 2
	//double width = 3.0F / (1.0F + pow(2.0F, (double)lframe / 20.0F - 5.0F));
	//double width = HEIGHT
		//* sin(
			//(PI * (double)lframe) / ANIMATION_LENGTH + HALF_PI
		//) + BASELINE_OFFSET;

	//low bottom sin from (x1, y1) to (x2, y2)
	//
	//                            π(x - x1)     π        2
	//       (y1 - y2) * ( sin( ( --------- ) + - ) + 1 )
	//                             x2 - x1      2
	// f(x) = -------------------------------------------- + y2
	//                             4
	//
	//1: (0, 3)
	//2: (400, 0.00003)
	//specialized case where x1 = 0 lets us cut out some crud
	//         π(x - x1)               πx
	//notably, --------- simplifies to --
	//          x2 - x1                x2
	//

#define y1 3.0F
#define y2 0.00004F
#define WIDTH_RANGE 2.99996F

	//i SWEAR this works
	double width = (
		WIDTH_RANGE * pow(
			sin((PI * (float)lframe) / ANIMATION_LENGTH + HALF_PI)
			+ 1.0F,
		2.0F)
		) / 4.0F + y2;

	double aspect = (double)screen->w / (double)screen->h;
	double height = width / aspect;

	complex center = {
		.a = -0.743643135F,
		.b = 0.131825963F
	};

	printf("w / h: %f / %f\n", width, height);

	//probably rounds to the nearest 4
	iterations = ((256 + 2 * frame) / 4) * 4;
	printf("iterations: %d\n", iterations);

	//clear screen
	SDL_FillRect(screen, NULL, 0x000000);

	//const double threshold = 0.1F;
	complex point;
	double dist;
	int i, j;
	for(i = 0; i < screen->h; i++) {
	for(j = 0; j < screen->w; j++) {
		point.a = scale((double)j, 0, screen->w,
			center.a - width, center.a + width);
		point.b = scale((double)i, 0, screen->h,
			center.b - height, center.b + height);
		dist = distmandelbrot(point, iterations);
		if(dist >= 0.0) {
			//check the distance estimate against the iterative approach to weed out holes
			dist = mandelbrot(point, iterations) ? 0.0F : dist;
			plot(screen, j, i,
				colortoint(graytocolor(bclamp(
				300000.0F * (dist / width)
				)))
			);
		}
	}
	}

	return;
}
