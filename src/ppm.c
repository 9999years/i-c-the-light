//writing ppm images

#include "ppm.h"

int writeppm(
	char *filename,
	int width,
	int height,
	unsigned int image[],
	unsigned char opacity[]
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
	fprintf(file,
		"P7\n"
		"WIDTH %d\n"
		"HEIGHT %d\n"
		"DEPTH 4"
		"MAXVAL 65535\n"
		"TUPLTYPE RGBA_ALPHA\n"
		"ENDHDR\n",
		width, height
	);
	for(i = 0; i < height; i++) {
		for(j = 0; j < width; j++) {
			fprintf(file, "%x%x ",
				image  [j + i*width],
				opacity[j + i*width]
			);
		}
		fprintf(file, "\n");
	}
	fclose(file);
	return 0;
}
