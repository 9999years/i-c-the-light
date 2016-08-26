#include <stdio.h>

enum {
	PORTABLE_BITMAP = 1,
	PORTABLE_GRAYMAP = 2,
	PORTABLE_PIXMAP = 3
};

int writeppm(
	char *filename,
	char filetype,
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
	fprintf(file, "P%d\n%d %d\n%d\n", filetype, width, height, 0xff);
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
