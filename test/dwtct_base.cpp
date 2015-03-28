#include <stdio.h>
#include "dwtct_base.h"

template <typename Type>
int dwtct_filtdown_base(Type *coef, const size_t n_coef, 
                        const Type *x, const size_t n_x, 
                        const dwtct_filt_param *f_par)
{
	if (n_coef*2 < n_x)
        return DWTCT_E_ARRAY_SIZE;
	const int x_len = n_x;
    const int x_shift = f_par->x_shift;
    const Type *filter = (const Type *)f_par->filter;
    const int filt_len = f_par->n_filt;

    if ((f_par->flags & DWTCT_BOUNDARY_MASK) == DWTCT_PERIODIC)
    {
	    for (int i = 0; i < x_len>>1; ++i)
	    {
	        Type sum = 0;
		    for (int j = 0; j < filt_len; ++j)
		        sum += filter[j] * x[(((i<<1) + j - x_shift) % x_len + x_len) % x_len];

		    coef[i] = sum;
	    }
	}
	return DWTCT_SUCCESS;
}

int dwtct_filtdown_base_d(double *coef, const size_t n_coef, 
	                    const double *x, const size_t n_x, 
	                    const dwtct_filt_param *f_par)
{
    return dwtct_filtdown_base(coef, n_coef, x, n_x, f_par);
}

int dwtct_filtdown_base_f(float *coef, const size_t n_coef, 
	                    const float *x, const size_t n_x, 
	                    const dwtct_filt_param *f_par)
{
    return dwtct_filtdown_base(coef, n_coef, x, n_x, f_par);
}
