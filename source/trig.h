#define PI_SQ   9.86960440108F
#define TAU     6.28318530717F
#define PI      3.14159265359F
#define HALF_PI 1.57079632679F

float fsin(float x)
{
	x = fmod(x, TAU);
	const float sign = x > PI || x < -PI ? -1 : 1;

	x = fmod(x, PI);

	const float B =  4/PI;
	const float C = -4/(PI_SQ);

	float y = B * x + C * x * fabsf(x);

	const float P = 0.225;

	//Q = 0.775
	//Q * y + P * y * abs(y)
	y = P * (y * fabsf(y) - y) + y;

	return y * sign;
}

float fcos(float x)
{
	x += HALF_PI;
	x = fmod(x, TAU);
	const float sign = x > PI || x < -PI ? -1 : 1;

	x = fmod(x, PI);

	const float B =  4/PI;
	const float C = -4/(PI_SQ);

	float y = B * x + C * x * fabsf(x);

	const float P = 0.225;

	//Q = 0.775
	//Q * y + P * y * abs(y)
	y = P * (y * fabsf(y) - y) + y;

	return y * sign;
}
