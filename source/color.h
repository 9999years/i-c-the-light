//functions for converting colors between uint format (0x123456)
//and rgbcolor format ({.r = 0x12, .g = 0x34, .b = 0x56})
//also for generating colors, etc

#ifndef COLOR_H
#define COLOR_H

#define CHANNELS 3

typedef unsigned char byte;

struct rgbcolor {
	byte r;
	byte g;
	byte b;
} rgbcolor;

//converts an rgbcolor to a uint
unsigned int colortoint(struct rgbcolor color);

//converts a uint to an rgbcolor
struct rgbcolor inttocolor(unsigned int color);

struct rgbcolor rgb(float r, float g, float b);

//returns the equivalent gray for any byte value 0-0xff
struct rgbcolor graytocolor(byte gray);

//clamps a float to 0-0xff
byte bclamp(float value);

struct rgbcolor addcolor(struct rgbcolor a, struct rgbcolor b);

struct rgbcolor avgcolor(struct rgbcolor a, struct rgbcolor b);

//interpolates between a and b
//b_interp is the percent of b to use
//eg b_interp = 1 --> 0% a, 100% b
//if b_interp is > 1 or < 0 you're gonna get some wacky nonsense
struct rgbcolor lerpcolor(struct rgbcolor a, struct rgbcolor b, float b_interp);

//this function only *approximates* a hue shift
//don't use it for anything exact
//it's also not associative
struct rgbcolor shifthue(struct rgbcolor in, const float fHue);
#endif //COLOR_H
