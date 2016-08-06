#include <stdio.h>
#include <math.h>
//#include <unistd.h>
//#include <string.h>

#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))

enum {
	PORTABLE_BITMAP = 1,
	PORTABLE_GRAYMAP = 2,
	PORTABLE_PIXMAP = 3
};

enum { RED, GREEN, BLUE };

int clamp(
	int value,
	int min,
	int max
	)
{
	if(value < min)
	{
		return min;
	}
	else if(value > max)
	{
		return max;
	}
	else
	{
		return value;
	}
}

int writeppm(
	char *filename,
	char filetype,
	int width,
	int height,
	int channels,
	unsigned char image[width][height][channels]
	)
{
	FILE *file = fopen(filename, "w");
	if(file == NULL)
	{
		printf("file open failure!\n");
	}
	fprintf(file, "P%d\n%d %d\n%d\n", filetype, width, height, 256);
	int i = 0, j = 0, k = 0;
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < width; j++)
		{
			for(k = 0; k < channels; k++)
			{
				fprintf(file, "%d ", image[i][j][k]);
			}
		}
		fprintf(file, "\n");
	}
	return 0;
}

int main( int argc, char *argv[] )
{
	unsigned char image[256][256][3];
	int i = 0, j = 0;
	for(i = 0; i < ARRAYSIZE(image); i++)
	{
		for(j = 0; j < ARRAYSIZE(image[0]); j++)
		{
			image[i][j][RED] =
				clamp(floor(
				( sin( 5 - (double)j / 32 )
				+ 1 )
				* 64),
				0, 0xff);

			image[i][j][GREEN] =
				clamp(
				floor(
				( sin( 16 + (double)j / 32 )
				+ 1 )
				* ( cos( 4 + (double)i / 32 )
				+ 1 )
				* 64),
				0, 0xff);

			image[i][j][BLUE] =
				clamp(
				floor(
				( sin( 4 + (double)i / 32 )
				+ 1 )
				/ ( sin( (double)j / 32 )
				+ 1 )
				* 32),
				0, 0xff);
				//floor(
				//( cos( (double)i / 32 )
				//+ 1 )
				//* 128);
		}
	}
	printf("image initialized!\n");
	writeppm("image.ppm", PORTABLE_PIXMAP, 256, 256, 3, image);
	printf("image written!\n");
	return 0;
}
