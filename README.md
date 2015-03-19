# DWTCT

DWTCT (libdwtct) is a C library (witten in C++) that provides the core transforms needed to compute wavelet transforms. The library is compiles with OpenMP as default. An input parameter tells the library whether to use parallel threaded code or not, and the number of threads to use can be set with a function all. See `src/dwtct.h` for the API.


## Core transforms

Periodic downsampling filter:
![downfilter](http://gummif.github.io/dwtct/master/filter_down.png)

Periodic upsampling filter:
*not ready*


## Requires

* gcc
* make

