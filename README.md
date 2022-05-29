# pad-mandelbrot-set

Project for the subject *High Perforamnce Processing* (abbreviated to *PAD* in Portuguese). It consists on generating a Mandelbrot Set, but the key is that the generation must happen in a thread-based methodology, where each thread will be responsible for a given subset of the result.

## Running the single threaded Mandelbrot Set

Compile the program with:

> gcc -o mandelbrot -O4 mandelbrot.c

Usage:

> ./mandelbrot <xmin> <xmax> <ymin> <ymax> <maxiter> <xres> <out.ppm>

Example:

> ./mandelbrot 0.27085 0.27100 0.004640 0.004810 1000 1024 pic.ppm

## Running the multi threaded Mandelbrot Set

The program doesn't exist yet.
