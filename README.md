# DWTCT

DWTCT (libdwtct) is a C library (written in C++) that provides the core transforms needed to compute wavelet transforms. The library is compiled with OpenMP as default. An input parameter tells the library whether to use the parallel threaded code or not, and the number of threads to use can be set with a function call. See `src/dwtct.h` for the API.


## Core transforms

Periodic downsampling filter:

![downfilter](http://gummif.github.io/dwtct/filter_down.png)

Periodic upsampling filter:
*not ready*

Periodic lifting scheme:
*not ready*

## Requires

* gcc
* make

