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

int initializelogfile();

#endif //LOGGING_H
