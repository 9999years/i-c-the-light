//writing ppm images

#include <stdio.h>
#include "color.h"

#ifndef PPM_H
#define PPM_H

int writeppm(
	char *filename,
	int width,
	int height,
	unsigned int image[]
	)
{
	FILE *file = fopen(filename, "w");
	if(file == NULL) {
		printf("file open failure!\n");
		return 1;
	}
	fprintf(file, "P3\n%d %d\n%d\n", width, height, 0xff);
	int i = 0, j = 0;
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
