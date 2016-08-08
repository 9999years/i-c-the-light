#include <stdio.h>
//#include <math.h>
//#include <time.h>
//#include <stdbool.h>

#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))

int main( int argc, char *argv[] )
{
	unsigned int a = 0xfe12ba;
	printf("%x\n", a);
	//unsigned int b[3];
	//printf("%x,%x,%x\n", b[0], b[1], b[2]);
	//b[0] = a-(a%0xffff);
	int n = 0;
	for(n = 0; n < sizeof(int)-1; n++)
	{
		printf("%x\n",
			(a >> (8*n)) & 0xff
			);
	}
	//printf("%x\n",a%0xffff);
	//printf("%x\n",a-a%0xffff);
	//b[1] = a%0xffff;
	//b[2] = a%0xff;
	//printf("%x,%x,%x\n", b[0], b[1], b[2]);
	return 0;
}
