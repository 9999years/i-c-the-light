//repo here: https://github.com/9999years/i-c-the-light
//MIT/expat license
//rebecca turner
//consult ../readme.md

#include "repl.h"

//global distance estimator
float de(vec3 pos)
{
	return distancejulia(pos, constq(-0.2F, 0.6F, 0.2F, 0.2F), 64);
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

	//float x, y, z;
	//int min, max;
	while(1) {
		//x = getnumber("x = ");
		//y = getnumber("y = ");
		//z = getnumber("z = ");
		//printf("dist to julia set:\n\t%f\n",
			//de(const3(x, y, z))
		//);
		//min = getnumber("min = ");
		//max = getnumber("max = ");
		printf("%d, ", random(-2, 2));
	}

	return 0;
}
