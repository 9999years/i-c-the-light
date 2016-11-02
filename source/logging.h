//logging, file out
#include <stdio.h>
//rand
#include <stdlib.h>
//timestamp for file names
#include <time.h>

#ifndef LOGGING_H
#define LOGGING_H

FILE *logfile;
FILE *plotfile;

int initializelogfile()
{
	const time_t unixtime = time(NULL);
	char filename[256] = "./log/UNINITIALIZED.log";
	sprintf(filename, "./log/icthelight-%lu.log",
		(unsigned long int)unixtime);
	logfile = fopen(filename, "w");
	if(logfile == NULL) {
		printf( "Log file open failure! (%s)\n"
			"Check that ./log/ exists?\n",
			filename);
		return -1;
	}
	fprintf(logfile, "# I C the Light, run %lu\n\n"
		"%s\n",
		(unsigned long int)unixtime,
		ctime(&unixtime)
	);
	sprintf(filename, "./log/plot-icthelight-%lu.data",
		(unsigned long int)unixtime);
	plotfile = fopen(filename, "w");
	if(plotfile == NULL) {
		printf( "Plot file open failure! (%s)\n"
			"Check that ./log/ exists?\n",
			filename);
		return -1;
	}
	return 1;
}
#endif //LOGGING_H
