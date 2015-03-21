#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "dwtct.h"
#ifdef _OPENMP
#include <omp.h>
#endif

// 0 0 0 0|0 0|0 0 0 0
const uint32_t DWTCT_BOUNDARY_MASK    = 0x000f;
const uint32_t DWTCT_THREADED_MASK    = 0x0030;
// flags
const uint32_t DWTCT_PERIODIC = 0x0000;
const uint32_t DWTCT_SERIAL   = 0x0000;
const uint32_t DWTCT_OPENMP   = 0x0010;



template <typename Type>
static int dwtct_filtdown(Type *coef, const size_t n_coef, 
                    const Type *x, const size_t n_x, 
                    const dwtct_filt_param *f_par);


DWTCT_EI int dwtct_filtdown_d(double *coef, const size_t n_coef, 
                            const double *x, const size_t n_x, 
                            const dwtct_filt_param *f_par)
{
    return dwtct_filtdown(coef, n_coef, x, n_x, f_par);
}

DWTCT_EI int dwtct_filtdown_f(float *coef, const size_t n_coef, 
                            const float *x, const size_t n_x, 
                            const dwtct_filt_param *f_par)
{
    return dwtct_filtdown(coef, n_coef, x, n_x, f_par);
}

DWTCT_EI void dwtct_set_num_threads(int n)
{
    #ifdef _OPENMP
    omp_set_num_threads(n);
    #endif
    (void)n;
}

DWTCT_EI int dwtct_get_num_threads(void)
{
    #ifdef _OPENMP
    return omp_get_max_threads();
    #else
    return 1;
    #endif
}

DWTCT_EI int dwtct_compiled_with_openmp(void)
{
    #ifdef _OPENMP
    return 1;
    #else
    return 0;
    #endif
}

DWTCT_EI void testfunc(double d, int *a)
{
    int p = (int)d*5;
    *a = p;
    #ifdef DWTCT_EXPORT
    printf("exported\n");
    #else
    printf("not exported\n");
    #endif
    /*
    double *foo = malloc(sizeof(double)*50);
    printf("malloc %p\n", foo);
    if (foo)
    {
        printf("val %f\n", foo[30]);
        free(foo);
    }*/
}




// split the range [0, x_len>>1) into 3 parts where the indeces in the middle range
// are in [(x_shift + 1)>>1, (x_len + x_shift - f_len + 1)>>1)
// i.e. we want to satisfy 0 <= 2*i + j - s < N for all i and j
static void split_down_range(const int x_len, const int x_shift, const int f_len, 
                    int r_start[3], int r_end[3])
{
    assert(x_len > 0 && x_len%2 == 0);
    assert(x_shift => 0);
    assert(f_len > 0);

    const int r1 = (x_shift + 1)>>1;
    const int r2 = (x_len + x_shift - f_len + 1)>>1;
    if (r1 < r2)
    {
        r_start[0] = 0;
        r_end[0] = r1;
        r_start[1] = r_end[0];
        r_end[1] = r2;
        r_start[2] = r_end[1];
        r_end[2] = x_len>>1;
    }
    else 
    {
        r_start[0] = 0;
        r_end[0] = 0;
        r_start[1] = 0;
        r_end[1] = 0;
        r_start[2] = 0;
        r_end[2] = x_len>>1;
    }
}

template <typename Type>
static inline Type filter_main_loop_reg(const Type *x, const Type *f, 
                                        const int f_len, const int shift)
{
    Type sum = 0;

    for (int j = 0; j < f_len; ++j)
        sum += f[j] * x[j + shift];

    return sum;
}

template <typename Type>
static inline Type filter_main_loop_per_bounds(const Type *x, const Type *f, const int f_len, 
                                                const int shift, const int length)
{
    Type sum = 0;

    for (int j = 0; j < f_len; ++j)
        sum += f[j] * x[((j + shift) % length + length) % length];

    return sum;
}



// hard coded loops for some common filter lengths
template <typename Type>
static void filter_hardcoded_inbounds_loops(Type *coef, const Type *x, 
                                    const Type *filter, const int filt_len, 
                                    const int i_start, const int i_end, 
                                    const int x_shift)
{
    switch (filt_len)
    {
        case 2:
            for (int i = i_start; i < i_end; ++i)
            {
                int ind = (i<<1) - x_shift;
                coef[i] = filter[0] * x[ind] + filter[1] * x[ind + 1];
            }
            break;
        case 4:
            for (int i = i_start; i < i_end; ++i)
            {
                int ind = (i<<1) - x_shift;
                coef[i] =   filter[0] * x[ind    ] + filter[1] * x[ind + 1] + 
                            filter[2] * x[ind + 2] + filter[3] * x[ind + 3];
            }
            break;
        case 6:
            for (int i = i_start; i < i_end; ++i)
            {
                int ind = (i<<1) - x_shift;
                coef[i] =   filter[0] * x[ind    ] + filter[1] * x[ind + 1] + 
                            filter[2] * x[ind + 2] + filter[3] * x[ind + 3] + 
                            filter[4] * x[ind + 4] + filter[5] * x[ind + 5];
            }
            break;
        case 8:
            for (int i = i_start; i < i_end; ++i)
            {
                int ind = (i<<1) - x_shift;
                coef[i] =   filter[0] * x[ind    ] + filter[1] * x[ind + 1] + 
                            filter[2] * x[ind + 2] + filter[3] * x[ind + 3] + 
                            filter[4] * x[ind + 4] + filter[5] * x[ind + 5] + 
                            filter[6] * x[ind + 6] + filter[7] * x[ind + 7];
            }
            break;
        default:
            for (int i = i_start; i < i_end; ++i)
                coef[i] = filter_main_loop_reg(x, filter, filt_len, (i<<1) - x_shift);
            break;
    }
}

#ifdef _OPENMP
#define __PARALLEL_LOOP _Pragma("omp parallel for schedule(static)")
#else
#define __PARALLEL_LOOP
#endif

template <typename Type>
static void filter_hardcoded_inbounds_loops_omp(Type *coef, const Type *x, 
                                        const Type *filter, const int filt_len, 
                                        const int i_start, const int i_end, 
                                        const int x_shift)
{
    switch (filt_len)
    {
        case 2:
            __PARALLEL_LOOP
            for (int i = i_start; i < i_end; ++i)
            {
                int ind = (i<<1) - x_shift;
                coef[i] = filter[0] * x[ind] + filter[1] * x[ind + 1];
            }
            break;
        case 4:
            __PARALLEL_LOOP
            for (int i = i_start; i < i_end; ++i)
            {
                int ind = (i<<1) - x_shift;
                coef[i] =   filter[0] * x[ind    ] + filter[1] * x[ind + 1] + 
                            filter[2] * x[ind + 2] + filter[3] * x[ind + 3];
            }
            break;
        case 6:
            __PARALLEL_LOOP
            for (int i = i_start; i < i_end; ++i)
            {
                int ind = (i<<1) - x_shift;
                coef[i] =   filter[0] * x[ind    ] + filter[1] * x[ind + 1] + 
                            filter[2] * x[ind + 2] + filter[3] * x[ind + 3] + 
                            filter[4] * x[ind + 4] + filter[5] * x[ind + 5];
            }
            break;
        case 8:
            __PARALLEL_LOOP
            for (int i = i_start; i < i_end; ++i)
            {
                int ind = (i<<1) - x_shift;
                coef[i] =   filter[0] * x[ind    ] + filter[1] * x[ind + 1] + 
                            filter[2] * x[ind + 2] + filter[3] * x[ind + 3] + 
                            filter[4] * x[ind + 4] + filter[5] * x[ind + 5] + 
                            filter[6] * x[ind + 6] + filter[7] * x[ind + 7];
            }
            break;
        default:
            __PARALLEL_LOOP
            for (int i = i_start; i < i_end; ++i)
                coef[i] = filter_main_loop_reg(x, filter, filt_len, (i<<1) - x_shift);
            break;
    }
}

template <typename Type>
int dwtct_filtdown(Type *coef, const size_t n_coef, 
                    const Type *x, const size_t n_x, 
                    const dwtct_filt_param *f_par)
{
    if (coef == NULL || x == NULL || f_par == NULL || f_par->filter == NULL)
        return DWTCT_E_NULL_PTR;
    if (n_coef*2 < n_x || n_x % 2 != 0 || f_par->n_filt < 2)
        return DWTCT_E_ARRAY_SIZE;
    if (f_par->x_shift < 0 || (f_par->flags & DWTCT_BOUNDARY_MASK) != DWTCT_PERIODIC)
        return DWTCT_E_INVALID_PARAM;

    const int x_len = n_x;
    const int x_shift = f_par->x_shift;
    const Type *filter = (const Type *)f_par->filter;
    const int filt_len = f_par->n_filt;
    const int threaded = (f_par->flags & DWTCT_THREADED_MASK) == DWTCT_OPENMP;

    int r_start[3];
    int r_end[3];
    split_down_range(x_len, x_shift, filt_len, r_start, r_end);

    int i_start;
    int i_end;

    // out of bounds range 1
    i_start = r_start[0];
    i_end = r_end[0];
    for (int i = i_start; i < i_end; ++i)
        coef[i] = filter_main_loop_per_bounds(x, filter, filt_len, (i<<1) - x_shift, x_len);

    // the main inbounds loop where most of the time is spent
    i_start = r_start[1];
    i_end = r_end[1];
    const int no_opt = (x_len*filt_len < 400 || filt_len%2 == 1 || filt_len > 8);
    if (!threaded)
    {
        if (no_opt)
        {
            for (int i = i_start; i < i_end; ++i)
                coef[i] = filter_main_loop_reg(x, filter, filt_len, (i<<1) - x_shift);
        }
        else // try optimized code
        {
            filter_hardcoded_inbounds_loops(coef, x, filter, filt_len, i_start, i_end, x_shift);
        }
    }
    else
    {
        if (no_opt)
        {
            __PARALLEL_LOOP
            for (int i = i_start; i < i_end; ++i)
                coef[i] = filter_main_loop_reg(x, filter, filt_len, (i<<1) - x_shift);
        }
        else // try optimized code
        {
            filter_hardcoded_inbounds_loops_omp(coef, x, filter, filt_len, i_start, i_end, x_shift);
        }
    }

    // out of bounds range 2 (no restriction)
    i_start = r_start[2];
    i_end = r_end[2];
    for (int i = i_start; i < i_end; ++i)
        coef[i] = filter_main_loop_per_bounds(x, filter, filt_len, (i<<1) - x_shift, x_len);

    return DWTCT_SUCCESS;
}
