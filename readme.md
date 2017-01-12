# I C the Light

### a distance estimating ray marcher written in C

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

Supplementary information and reference links can be found at
[becca.ooo/i-c-the-light](https://becca.ooo/i-c-the-light/).

## Directory

### Code

The code lives in `source/`, as you might expect. The render loop and `main()`
are in `icthelight.c`.

Images will be written to `output/`.

## Compiling / Running

To compile with SDL, you’ll need a copy of SDL 2.0.4, which can be downloaded
[here](https://www.libsdl.org/download-2.0.php). I don't expect this to change
any time in the near future, so don't worry if this gets a bit out of date.

To compile *without* SDL, comment out `#define USE_REAL_SDL` in
`source/sdl.h` to add a set of stub functions that let I C the Light
compile without error (you will get a bunch of unused function parameter
warnings, though). Obviously, a window won’t be created, but writing the images
still works fine.

To run, you’ll need a copy of `SDL2.dll` to live in the same directory as the
executable, which can be found in the runtime binaries section
[here](https://www.libsdl.org/download-2.0.php).

If you compiled with SDL, run with `--window` or `-w` to generate a
window for output display.

## Branches

####`master`

Anything generally stable that represents more than incremental progress.

#### `mess`

Alpha work in various states of disrepair.
Might not compile, might be terribly broken.

## License

I C the Light is licensed under the MIT/expat license, a copy of which is
included in `license.txt`.
