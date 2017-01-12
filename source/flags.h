#ifndef FLAGS_H
#define FLAGS_H

//for bit flags
#define B0  0x0001
#define B1  0x0002
#define B2  0x0004
#define B3  0x0008
#define B4  0x0010
#define B5  0x0020
#define B6  0x0040
#define B7  0x0080
#define B8  0x0100
#define B9  0x0200
#define B10 0x0400
#define B11 0x0800
#define B12 0x1000
#define B13 0x2000
#define B14 0x4000
#define B15 0x8000

#define CONVERT_IMMEDIATELY B0
#define USER_QUATERNION B1
#define QUIT_IMMEDIATELY B2

int flags;

#define FLAG(F) (flags & F)

#endif //FLAGS_H
