#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))

enum {
	PORTABLE_BITMAP = 1,
	PORTABLE_GRAYMAP = 2,
	PORTABLE_PIXMAP = 3
};

enum { RED, GREEN, BLUE };

unsigned long int epoch()
{
	return time(NULL);
}

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

int autoscale(
	int width,
	int height,
	int channels,
	unsigned char image[width][height][channels],
	int min,
	int max
	)
{
	int i = 0, j = 0, k = 0;
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			for(k = 0; k < channels; k++)
			{
				image[i][j][k] -= min;
				image[i][j][k] *= 0xff/(max-min);
				image[i][j][k] = clamp(image[i][j][k], 0, 0xff);
			}
		}
	}
	return 0;
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
	//printf("time is %lu seconds", (unsigned long)time(NULL));
	unsigned char image[256][256][3];
	int i = 0, j = 0, k = 0, min = 255, max = 0;
	for(i = 0; i < ARRAYSIZE(image); i++)
	{
		for(j = 0; j < ARRAYSIZE(image[0]); j++)
		{
			image[i][j][RED] =
				(i*(j/32))*255;
				//(i%32) & (j%32);

			image[i][j][GREEN] =
				0;
			image[i][j][BLUE] =
				0;

			for(k = 0; k < 3; k++)
			{
				min = image[i][j][k] < min ? image[i][j][k] : min;
				max = image[i][j][k] > min ? image[i][j][k] : max;
			}
		}
	}

	printf("min = %d, max = %d\n", min, max);

	autoscale(256, 256, 3, image, min, max);

	char filename[256];
	sprintf(filename, "output/image%lu.ppm", (unsigned long int)time(NULL));
	printf("image initialized!\nwriting to %s\n", filename);
	writeppm(filename, PORTABLE_PIXMAP, 256, 256, 3, image);
	printf("image written!\n");
	return 0;
}
