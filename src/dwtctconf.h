
#ifndef DWTCTCONF_H
#define DWTCTCONF_H

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WINDOWS) || defined(WINDOWS) || defined(__MSDOS__)
#  ifdef DWTCT_EXPORT
#    define DWTCT_EI __declspec(dllexport)
#  else
#    define DWTCT_EI __declspec(dllimport)
#  endif
#elif defined(_GCC)
#  define DWTCT_EI __attribute__((visibility("default")))
#else
#  define DWTCT_EI 
#endif

#endif /* DWTCTCONF_H */
