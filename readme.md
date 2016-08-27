# I C the Light

### a distance estimating ray marcher written in C*

*“it’s a ray marcher!”*

*guarenteed to work in at least two dimensions or your money back!*

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

Some supplimentary information and reference links can be found at [becca.ooo/i-c-the-light](https://becca.ooo/i-c-the-light/).

## Directory

### Code

The code lives in `source/`, and includes:

#### `color.h`

Functions for converting colors between `(unsigned int) 0xFFFFFF` and `unsigned char *{0xFF, 0xFF, 0xFF}` formats.

#### `test.c`

Unit tests.

I have a tentative 100% test coverage, in that 100% of the functions are covered—there’s definitely the chance that edge cases are not covered.

#### `icthelight.c`

The beef of the code lives here.
Can be redirected to a log file for some supplementary output.

#### `ppm.h`

Code for writing [ppm images](https://en.m.wikipedia.org/wiki/Netpbm_format).

#### `line.h`

`drawline()`, which does more or less what you’d expect.
Uses [Bresenham’s algorithm](https://en.m.wikipedia.org/wiki/Bresenham%27s_line_algorithm).

#### `plot.h`

`plot()`, which sets a point to a given color.

Might support an alpha channel in the future.

#### `vector.h`

Various functions for doing math with vectors as well as the `vec2` and `vec3` types.

#### `distance.h`

Distance functions. Start with `dist`.

### Misc

#### `license.txt`

A copy of the MIT/expat license; see below.

#### `Makefile`

Super simple makefile, crafted by hand.

#### `readme.md`

This file.

#### `output/`

Output will be written here, e.g. `image1470681018.ppm`, stamped with the [Unix epoch](https://en.wikipedia.org/wiki/Unix_time).

Run `make convert` in, uh, `source/` to convert these images to `.png`s and then delete the `.ppm`s.

## Compiling

You’ll need SDL2 files in the SDL folder and a copy of SDL2.dll

As of August 2016, I C the Light is compiled with SDL 2.0.4, which can be downloaded [here](https://www.libsdl.org/download-2.0.php).

## Branches

`master`:
Anything generally stable that represents more than incremental progress.

`mess`:
Alpha work in various states of disrepair.
Do not check these out unless you know what you’re doing.

## Todo

Check `source/todo.txt` for some shorter term goals.

## License

I C the Light is licensed under the MIT/expat license, a copy of which is included in `license.txt`.
