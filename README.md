# DWTCT

DWTCT (libdwtct) is a C library (implemented in C++) that provides the core transforms needed to compute floating-point wavelet transforms. This library aims to provide fast, lightweight and flexible functions on which other libraries or code (in any language with C-call capabilities) can build higher level transforms.

The library is compiled with OpenMP as default. An input parameter tells the library whether to use the parallel threaded code or not, and the number of threads to use can be set with a function call. See `src/dwtct.h` for the API.


## Core transforms

Periodic downsampling filter:

![downfilter](http://gummif.github.io/dwtct/filter_down.png)

Periodic upsampling filter:
*not ready*

Periodic lifting scheme:
*not ready*


## Install

From the `dwtct`directory execute `make` and `sudo make install`. A test program can be compiled with `make testprog` and run with `make runtest`.


## Requires

* gcc
* make

