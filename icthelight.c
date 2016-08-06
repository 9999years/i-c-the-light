#include <stdio.h>
#include <math.h>
//#include <unistd.h>
//#include <string.h>

#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define PORTABLE_BITMAP 1
#define PORTABLE_GRAYMAP 2
#define PORTABLE_PIXMAP 3

int writeppm(
	char *filename,
	int width,
	int height,
	unsigned char image[width][height],
	int filetype
	)
{
	FILE *file = fopen(filename, "w");
	if(file == NULL)
	{
		printf("file open failure!\n");
	}
	fprintf(file, "P%d\n%d %d\n%d\n", filetype, width, height, 256);
	int i = 0, j = 0;
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < width; j++)
		{
			fprintf(file, "%d ", image[i][j]);
		}
		fprintf(file, "\n");
	}
	return 0;
}

int main( int argc, char *argv[] )
{
	unsigned char image[256][256];
	int i = 0, j = 0;
	for(i = 0; i < ARRAYSIZE(image); i++)
	{
		for(j = 0; j < ARRAYSIZE(image[0]); j++)
		{
			image[i][j] = floor((sin((float)i/32)+cos((float)j/32))*256);
			//if(i + j < 100)
			//{
				//image[i][j] = 255;
			//}
			//else
			//{
				//image[i][j] = 0;
			//}
		}
	}
	printf("image initialized!\n");
	writeppm("image.pgm", 256, 256, image, PORTABLE_GRAYMAP);
	printf("image written!\n");
	return 0;
}
