//writing ppm images

#include <stdio.h>
#include "color.h"

#ifndef PPM_H
#define PPM_H

#define PPM_WRITE_ALL_BLACK 2
#define PPM_FILE_OPEN_FAILURE 1

int writeppm(
	char *filename,
	int width,
	int height,
	unsigned int image[],
	unsigned char opacity[]
);
#endif //PPM_H
