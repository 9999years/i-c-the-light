CC = gcc
CFLAGS = -Wall --std=c99 #-fdiagnostics-color
INCLUDE_CURRENT =  -I.
# ??
NO_CONSOLE = -mwindows
RM=rm

icthelight: main.c icthelight.c color.c common.c complex.c distance.c icthelight.c logging.c plot.c ppm.c quaternion.c vector.c surface.c
	ctags *.c *.h
	$(CC) $(CFLAGS) $(INCLUDE_CURRENT) -o $@ $^

test: test.c
	$(CC) $? $(CFLAGS) -o $@

convert:
	mogrify -format png ../output/*.ppm
	-$(RM) ..\output\*.ppm 2>/dev/null

#for the windows users in the crowd
cp:
	chcp 65001

clean:
	-$(RM) ./*.exe -rf 2>/dev/null
	-$(RM) icthelight 2>/dev/null
	-$(RM) TEST_PPM.ppm 2>/dev/null
	-$(RM) tags 2>/dev/null
	-$(RM) ./log/* -rf 2>/dev/null;
