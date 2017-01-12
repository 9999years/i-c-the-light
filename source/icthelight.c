//repo: https://github.com/9999years/i-c-the-light
//rebecca turner
#include "icthelight.h"

//global distance estimator
float de(vec3 pos)
{
	return distancejulia(pos, juliaconstant, iterations);
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
	float width = 60.0F / ((float)frame + 20.0F);
	float aspect = (float)screen->w / (float)screen->h;
	float height = width / aspect;

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

	//const float threshold = 0.1F;
	complex point;
	float dist;
	int i, j;
	for(i = 0; i < screen->h; i++) {
	for(j = 0; j < screen->w; j++) {
		point.a = scale((float)j, 0, screen->w,
			center.a - width, center.a + width);
		point.b = scale((float)i, 0, screen->h,
			center.b - height, center.b + height);
		dist = distmandelbrot(point, iterations);
		if(dist >= 0.0) {
			dist = mandelbrot(point, iterations) ? 0.0F : dist;
			plot(screen, j, i,
				colortoint(graytocolor(bclamp(
				100000.0F * (dist / width)
				)))
			);
		}
	}
	}

	return;
}
