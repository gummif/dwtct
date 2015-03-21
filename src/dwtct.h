
#ifndef DWTCT_H
#define DWTCT_H

#include "dwtctconf.h"
#include "inttypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct d_dwtct_filt_param {
    const void      *filter;    // the filter
    size_t          n_filt;     // length of filter
    unsigned int    x_shift;    // shift of x
    uint32_t        flags;      // default: periodic, serial, 1 thread (not used)
} dwtct_filt_param;

// masks and flags
const uint32_t DWTCT_DEFAULT_FLAGS = 0;

extern const uint32_t DWTCT_BOUNDARY_MASK;
extern const uint32_t DWTCT_PERIODIC;

extern const uint32_t DWTCT_THREADED_MASK;
extern const uint32_t DWTCT_SERIAL;
extern const uint32_t DWTCT_OPENMP;

extern const uint32_t DWTCT_NUM_THREADS_MASK;
extern const uint32_t DWTCT_1_THREAD;
extern const uint32_t DWTCT_2_THREAD;
extern const uint32_t DWTCT_4_THREAD;
extern const uint32_t DWTCT_8_THREAD;

enum dwtct_error_code {
    DWTCT_SUCCESS = 0,
    DWTCT_E_INVALID_PARAM,
    DWTCT_E_ARRAY_SIZE,
    DWTCT_E_NULL_PTR
};

// API

/*!
 * Perform periodic filtering and downsampling. Arrays should not overlap.
 * 
 * @param coef      result of filtering
 * @param n_coef    length of coef
 * @param x         input signal to be filtered
 * @param n_x       length of x
 * @param f_par     the filter and other parameters
 * @returns         0 on success, else an error code
 */
DWTCT_EI int dwtct_filtdown_d(double *coef, const size_t n_coef, 
                            const double *x, const size_t n_x, 
                            const dwtct_filt_param *f_par);

DWTCT_EI int dwtct_filtdown_f(float *coef, const size_t n_coef, 
                            const float *x, const size_t n_x, 
                            const dwtct_filt_param *f_par);



//! Set the number of threads to use.
DWTCT_EI void dwtct_set_num_threads(int n);
//! Get the max number of threads.
DWTCT_EI int dwtct_get_num_threads(void);
//! Returns 0 or 1 depending on compile settings.
DWTCT_EI int dwtct_compiled_with_openmp(void);



DWTCT_EI void testfunc(double d, int *a);


#ifdef __cplusplus
}
#endif


#endif /* DWTCT_H */
