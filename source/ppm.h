//writing ppm images

#include <stdio.h>
#include "color.h"

#define PPM_WRITE_ALL_BLACK 2
#define PPM_FILE_OPEN_FAILURE 1

#ifndef PPM_H
#define PPM_H

int writeppm(
	char *filename,
	int width,
	int height,
	unsigned int image[]
	)
{
	//run through image, check to make sure it's not all black
	int i, j;
	char allblack = 1;
	for(i = 0; i < height; i++) {
		for(j = 0; j < width; j++) {
			if(image[j + i * width] != 0x000000)
				allblack = 0;
		}
	}

	if(allblack == 1)
	{
		printf("image is ALL BLACK, not writing!\n");
		return PPM_WRITE_ALL_BLACK;
	}

	FILE *file = fopen(filename, "w");
	if(file == NULL) {
		printf("file open failure!\n");
		return PPM_FILE_OPEN_FAILURE;
	}
	fprintf(file, "P3\n%d %d\n%d\n", width, height, 0xff);
	struct rgbcolor pixel;
	for(i = 0; i < height; i++) {
		for(j = 0; j < width; j++) {
			pixel = inttocolor(image[j + i*width]);
			fprintf(file, "%d ", pixel.r);
			fprintf(file, "%d ", pixel.g);
			fprintf(file, "%d ", pixel.b);
		}
		fprintf(file, "\n");
	}
	fclose(file);
	return 0;
}
#endif //PPM_H
