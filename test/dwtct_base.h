
#ifndef DWTCT_BASE_H
#define DWTCT_BASE_H

#include "dwtct.h"

#ifdef __cplusplus
extern "C" {
#endif


// Unoptimized base methods to compare with the optimized versions

int dwtct_filtdown_base_d(double *coef, const size_t n_coef, 
                        const double *x, const size_t n_x, 
                        const dwtct_filt_param *f_par);

int dwtct_filtdown_base_f(float *coef, const size_t n_coef, 
                        const float *x, const size_t n_x, 
                        const dwtct_filt_param *f_par);

#ifdef __cplusplus
}
#endif


#endif /* DWTCT_BASE_H */
