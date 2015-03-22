//! @file dwtct.h
#ifndef DWTCT_H
#define DWTCT_H

#include "dwtctconf.h"
#include "inttypes.h"

#ifdef __cplusplus
extern "C" {
#endif

//! Parameters for filtering
typedef struct d_dwtct_filt_param {
    const void      *filter;    //!< the filter
    size_t          n_filt;     //!< length of filter
    unsigned int    x_shift;    //!< shift of the signal x
    uint32_t        flags;      //!< @see DWTCT_DEFAULT_FLAGS
} dwtct_filt_param;

/*! @brief Default flags (periodic, serial)
 *
 * The default flags are guaranteed to be zero.
 * Usage example: 
 * @code{.c}
 * flags = DWTCT_DEFAULT_FLAGS | DWTCT_PERIODIC | DWTCT_OPENMP;
 * @endcode
 * Check thread options:
 * @code{.c}
 * (flags & DWTCT_THREADED_MASK) == DWTCT_OPENMP
 * @endcode
 */
const uint32_t DWTCT_DEFAULT_FLAGS = 0;
extern const uint32_t DWTCT_BOUNDARY_MASK;  //!< Boundary conditions mask
extern const uint32_t DWTCT_PERIODIC;       //!< Boundary conditions flag
extern const uint32_t DWTCT_THREADED_MASK;  //!< Multi-threading mask
extern const uint32_t DWTCT_SERIAL;         //!< Multi-threading flag
extern const uint32_t DWTCT_OPENMP;         //!< Multi-threading flag. See dwtct_set_num_threads()

//! Error codes
enum dwtct_error_code {
    DWTCT_SUCCESS = 0,
    DWTCT_E_INVALID_PARAM,
    DWTCT_E_ARRAY_SIZE,
    DWTCT_E_NULL_PTR
};


// API

/*!
 * @brief Perform filtering and downsampling. 
 * 
 * @param coef      result of filtering
 * @param n_coef    length of coef
 * @param x         input signal to be filtered
 * @param n_x       length of x
 * @param f_par     the filter and other parameters
 * @returns         0 on success, else an error code
 *
 * Arrays should not overlap. The length of the signal \f$x\f$  should be even. 
 * The downsampling filter transform computes
 * \f[
 * c[i] = \sum_{j=0}^{n-1} f[j] \; x[2i + j - s]
 * \f]
 * where \f$x\f$ is the input signal, \f$f\f$ is the filter, \f$c\f$ are the output coefficients, 
 * \f$n\f$ is the length of \f$f\f$, \f$s\geq0\f$ is the shift of \f$x\f$.
 */
DWTCT_EI int dwtct_filtdown_d(double *coef, const size_t n_coef, 
                            const double *x, const size_t n_x, 
                            const dwtct_filt_param *f_par);
//! @see dwtct_filtdown_d()
DWTCT_EI int dwtct_filtdown_f(float *coef, const size_t n_coef, 
                            const float *x, const size_t n_x, 
                            const dwtct_filt_param *f_par);

//! @brief Set the number of threads to use.
DWTCT_EI void dwtct_set_num_threads(int n);

//! @brief Get the max number of threads.
DWTCT_EI int dwtct_get_num_threads(void);

//! Get compile settings (returns 0 or 1).
DWTCT_EI int dwtct_compiled_with_openmp(void);


//! sss
#ifdef __cplusplus
}
#endif


#endif /* DWTCT_H */
