//repo here: https://github.com/9999years/i-c-the-light
//MIT/expat license
//rebecca turner
//consult ../readme.md

//logging, file out
#include <stdio.h>
#include <io.h>
//rand
#include <stdlib.h>
//sin functions
#include <math.h>
//timestamp for file names
#include <time.h>
#include <string.h>

//project files
#include "common.h"
//#include "color.h"
//#include "ppm.h"
//#include "line.h"
#include "vector.h"
#include "distance.h"
#include "quaternion.h"
#include "logging.h"

//Screen dimension constants
#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 200

//globals
int frame = 0;
FILE *logfile;
FILE *plotfile;

//global distance estimator
float de(vec3 pos)
{
	return distancejulia(pos, constq(-0.2F, 0.6F, 0.2F, 0.2F));
}

char *getline(void)
{
	char *line = malloc(100), *linep = line;
	size_t lenmax = 100, len = lenmax;
	int c;

	if(line == NULL)
		return NULL;

	for(;;) {
		c = fgetc(stdin);
		if(c == EOF)
			break;

		if(--len == 0) {
			len = lenmax;
			char * linen = realloc(linep, lenmax *= 2);

			if(linen == NULL) {
				free(linep);
				return NULL;
			}
			line = linen + (line - linep);
			linep = linen;
		}

		if((*line++ = c) == '\n')
			break;
	}
	*line = '\0';
	return linep;
}

double getnumber(char *message)
{
	printf("%s", message);
	return strtod(getline(), NULL);
}

int main(/*int argc, char* args[]*/)
{
	printf("Hello!\n");
	const time_t unixtime = time(NULL);
	printf( "I C the Light, kinda, Rebecca Turner (MIT/Expat)\n"
		"%s\n", ctime(&unixtime)
	);

	initializelogfile();

	float x, y, z;
	while(1) {
		x = getnumber("x = ");
		y = getnumber("y = ");
		z = getnumber("z = ");
		printf("dist to julia set:\n\t%f\n",
			de(const3(x, y, z))
		);
	}

	return 0;
}
