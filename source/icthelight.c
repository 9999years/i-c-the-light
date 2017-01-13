//repo: https://github.com/9999years/i-c-the-light
//rebecca turner
#include "icthelight.h"

//global distance estimator
float de(vec3 pos)
{
	//quaternion c = constq(-0.2F, 0.6F, 0.2F, 0.2F);
	//quaternion c = constq(-0.137F, -0.630F, -0.475F, -0.046);
	//quaternion c = constq(-0.213F, -0.0410F, -0.563F, -0.560);
	//return distancejulia(pos, juliaconstant, iterations);
	//return distserpenski(pos);
	return
		//disttorus(pos, const3(0.0F, 0.0F, 0.0F), 2.0F, 5.5F);
		//distsphere(pos, const3(50.0F, 0.0F, 50.0F), 10.0F);
		opwobble3(
			pos,
			distsphere(pos, const3(0.0F, 0.0F, 0.0F), 1.0F),
			0.3F,
			0.8F
		);
		//distsphere(pos, sphere, 10.0F);
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
	/* here's how im setting up the axes (right-handed)
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
	 */

#define MAX_DISTANCE 0.1F
#define MIN_DISTANCE 0.001F
#define BOUNDING_RADIUS 5.0F

	printf(
	"\n\n"
"====== %.5d ==================================================================="
	"\n", lframe
	);

	const float timef = (float)(lframe * TAU) / FRAMES_IN_ROTATION;
	//const float timef = randf(0.0F, TAU);
	unsigned long int timeint = time(NULL);
	printf("time: %f\n", timef);
	const float sintime = sin(timef);
	const float costime = cos(timef);
	//const float tworoottwo = sqrt(2.0F) / 2.0F;
	//how big the viewport is
	//const float viewport_size = 2.75F;
	const float viewport_size = 3.5F;
	SDL_FillRect(screen, NULL, 0xaaeeff);
	//SDL_FillRect(screen, NULL, 0x000000);

	//stores values to calculate colors
	//i can set exposure by figuring out the minimum/maximum of this array
	float *values = (float *)malloc(sizeof(float) * screen->w * screen->h);
	//the coordinates of matching keys in `values`, for plotting
	int *coords = (int *)malloc(sizeof(int) * screen->w * screen->h);
	//the length of both of those arrays
	int coordslen = 0;

	if(values == NULL) {
		printf("no memory for values!\n");
		return;
	}
	if(coords == NULL) {
		printf("no memory for keys!\n");
		return;
	}

	//screen aspect ratio
	//const float aspect = (float)screen->w / (float)screen->h;

	//portion of viewport to render
	//this is multiplied by the viewport vectors to get a point on the
	//viewport the ray is shot through
	float wfrac, hfrac;

	//focal length of the camera
	//longer = more zoomed in
	float focallength = 20.0F;

	//width of the camera (horiz. line at the center of the viewport)
	vec3 viewport_width = const3(
		viewport_size * costime,
		viewport_size * sintime,
		0.0F
	);

	//height of the camera (vert. line at the center of the viewport)
	vec3 viewport_height = const3(0.0F, 0.0F, viewport_size);

	//offset of the center of the viewport from the origin
	//essentially the camera position
	vec3 viewport_ofs = const3(
		2.0F * cos(timef - HALF_PI),
		2.0F * sin(timef - HALF_PI),
		0.0F
	);


	//point the ray will travel through, other than the camera
	vec3 ray_through;

	//camera
	//the rays are shot from here through the viewport
	//it's backwards and perpendicular from the plane
	//containing the viewport
	//viewport_ofs + (perp to the w & height of the vp) * focal len
	vec3 camera = add3(
		viewport_ofs,
		mult3s(
			unit3(perp3(viewport_width, viewport_height)),
			focallength
		)
	);

	//direction for the ray to travel in
	vec3 ray_rot;

	//the current position to be measured by the distance estimator
	//generally camera + distance in the direction of ray_rot
	vec3 measure_pos;

	//ray origin (location on the viewport plane)
	vec3 ray_orig;

	//distance from measure_pos
	float distance;
	//total distance travelled, i hope i hope i hope
	float totaldistance;

	//preview value
	float predist;

	//vec3 light = fromdirection3(timef + 1.0F, timef, 1.0F);
	vec3 light = fromdirection3(1.6F, 0.6F, 1.0F);

	//steps to march
	const int steps = 512;

	if(~flags & USER_QUATERNION) {
		juliaconstant = constq(
			randf(-1.0F, 1.0F),
			randf(-1.0F, 1.0F),
			randf(-1.0F, 1.0F),
			randf(-1.0F, 1.0F)
		);
	}

	printf("quaternion is <%.6f, %.6f, %.6f, %.6f>\n",
		juliaconstant.r,
		juliaconstant.a,
		juliaconstant.b,
		juliaconstant.c
	);

	printf("rendering values (%dx%d)\n", screen->w, screen->h);

	int i, j, k;
	for(i = 0; i < screen->h; i++) {
	for(j = 0; j < screen->w; j++) {
		//new sample, new distance
		distance = 0.0F;
		totaldistance = 0.0F;

		//what portion of the viewport are we rendering to?
		//-0.5 to 0.5 because viewport_ofs represents the center
		//of the viewport and viewport_width and _height represent
		//whole, not half widths of the viewport size
		wfrac = scale(j, 0, screen->w, -0.5F, 0.5F);
		hfrac = scale(i, 0, screen->h, -0.5F, 0.5F);

		//multiply that by width / height to get a real location
		ray_through = add3(
			mult3s(viewport_width, wfrac),
			mult3s(viewport_height, hfrac)
		);

		//where the ray starts (on the viewport)
		ray_orig = add3(ray_through, viewport_ofs);

		//a unit vec from the camera through the viewport
		ray_rot = unit3(through3(
			camera,
			add3(ray_through, viewport_ofs)
		));

		for(k = 0; k < steps; k++) {
			measure_pos = add3(
				ray_orig,
				mult3s(
					ray_rot,
					totaldistance
				)
			);

			predist = de(measure_pos);

			totaldistance += distance =
				fmin(predist, MAX_DISTANCE) * 0.8F;

			if(distance <= MIN_DISTANCE) {
				coords[coordslen] = j + i * screen->w;
				values[coordslen] =
					//(float)k;
					(1.5F - (float)k / (float)steps) *
					blinnphong(camera, measure_pos, ray_rot, light);
				coordslen++;
				break;
			} else if(totaldistance >= BOUNDING_RADIUS
				|| !isfinite(distance)) {
				//we're done here
				break;
			}
		}
	}
	}

	struct limits limit = getlimits(values, coordslen);

	printf("limits: min: %.3f, max: %.3f)\n",
		limit.min, limit.max
	);

	for(i = 0; i < coordslen; i++) {
		//plot the i-th coord on the screen
		plot(
			screen,
			coords[i] % screen->w,
			coords[i] / screen->h,
			//turn the value from min to max to a byte color
			colortoint(graytocolor(bclamp(
				scale(values[i], limit.min, limit.max, 0, 255)
				//add a bit of noise
				+ random(-3, 3)
			)))
		);
			//various other render methods
			//kept here for... posteriority?
			//colortoint(graytocolor(bclamp(k * 6)))
			//dot3(norm, light)
			//255.0F * (float)k / (float)steps
			//0.01F / (totaldistance + 0.00003F)
			//blinnphong(camera, measure_pos, ray_rot, light)
			//distance <= 2.0F ? 0xffffff : 0x000000

	}

	//clear your mind
	free(values);
	free(coords);
	return;
}
