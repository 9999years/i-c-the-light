# I C the Light

*a distance estimating ray marcher written in C*

On ray marching (from [Syntopia](http://blog.hvidtfeldts.net/index.php/2011/06/distance-estimated-3d-fractals-part-i/)):

> Classic raytracing shoots one (or more) rays per pixel and calculates where the
> rays intersect the geometry in the scene. Normally the geometry is described by
> a set of primitives, like triangles or spheres, and some kind of spatial
> acceleration structure is used to quickly identify which primitives intersect
> the rays.
>
> Distance Estimation, on the other hand, is a ray marching technique.
>
> Instead of calculating the exact intersection between the camera ray and the
> geometry, you proceed in small steps along the ray and check how close you are
> to the object you are rendering. When you are closer than a certain threshold,
> you stop. In order to do this, you must have a function that tells you how
> close you are to the object: a Distance Estimator. The value of the distance
> estimator tells you how large a step you are allowed to march along the ray,
> since you are guaranteed not to hit anything within this radius.

## Directory

### Code

The code lives in `source/`, and includes:

#### `color.h`

Functions for converting colors between `(unsigned int) 0xFFFFFF` and `unsigned char *{0xFF, 0xFF, 0xFF}` formats.

Exposes `rgbcolor`, `colortoint(rgbcolor color)`, and `inttocolor(rgbcolor colorarray, unsigned int color)`

Is it inconsistent that `colortoint()` returns the int color but `inttocolor()` requires a pointer to an existing `rgbcolor`?
Maybe.
Will it change in the future?
Probably.

#### `test.c`

Unit tests. Currently only tests `color.h`.

#### `icthelight.c`

The beef of the code lives here.
Can be redirected to a log file for some supplementary output.

#### `ppm.h`

Code for writing [ppm images](https://en.m.wikipedia.org/wiki/Netpbm_format).

### Misc

#### `license.txt`

A copy of the MIT license; see below.

#### `Makefile`

Super simple makefile, crafted by hand.

#### `readme.md`

This file.

#### `output/`

Output will be written here, e.g. `image1470681018.ppm`, stamped with the [Unix epoch](https://en.wikipedia.org/wiki/Unix_time).

## Compiling

You’ll need SDL2 files in the SDL folder and a copy of SDL2.dll

As of August 2016, I C the Light is compiled with SDL 2.0.4, which can be downloaded [here](https://www.libsdl.org/download-2.0.php).

## Branches

Approximately ordered from most to least stable.

`master`:
(Usually) stable builds and major progress.

(Maybe some day) `beta`:
Unstable builds but not necessarily a horrible mess.
Will generally be far ahead from `master`.

`mess`:
Commits almost certainly guaranteed not to work.
Do not check these out; they’re just for me to store work for later.

## Coming soon...

* Binary downloads of the SDL DLLs from my website
* Gallery of output on my website

## License

I C the Light is licensed under the MIT license, a copy of which is included in `license.txt`.
