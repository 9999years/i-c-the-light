#ifndef GLOBALS_H
#define GLOBALS_H

//base path for file  writing
char outfile_base[256];

//frame count, usually passed as lframe to functions
int frame;

//number of aa samples
int antialias;

//julia constant
quaternion juliaconstant;

//iteration count
int iterations;

#endif //GLOBALS_H
