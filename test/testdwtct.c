#include <stdio.h>
#include <math.h>
#include "dwtct.h"
#include "dwtct_base.h"

#define N_FILT 20
#define N_COEF 512
#define N_X 1024


int equal_d(double *x, double *y, int n, double tol)
{
    for (int i = 0; i < n; ++i)
    {
        if (fabs(x[i] - y[i]) > tol)
            return 0;
    }
    return 1;
}

int main()
{
    printf("\n::TESTING\n");

    printf("Compiled with OpenMP: %d\n", dwtct_compiled_with_openmp());
    printf("Number of threads: %d\n", dwtct_get_num_threads());
    printf("\n");

    const double tol = 1e-12;

    double f[N_FILT] = {0.0};
    double x[N_X] = {0.0};
    double coef[N_COEF] = {0.0};
    double coef_base[N_COEF] = {0.0};
    dwtct_filt_param par;
    par.filter = f;

    for (int i = 0; i < N_X; ++i)
        x[i] = ((i%30) * 1.3 - 14.2);

    for (size_t n_filt = 2; n_filt < N_FILT; ++n_filt)
    {
        for (int x_shift = 0; x_shift < 8; ++x_shift)
        {
            for (size_t n_x = 2; n_x < N_X; n_x += 2)
            {
                for (int i_flag = 0; i_flag < 2; ++i_flag)
                {
                    uint32_t flags = DWTCT_DEFAULT_FLAGS;
                    if (i_flag == 0)
                        flags |= DWTCT_SERIAL;
                    else
                        flags |= DWTCT_OPENMP;

                    size_t n_coef = n_x>>1;
                    par.n_filt = n_filt;
                    par.x_shift = x_shift;
                    par.flags = DWTCT_DEFAULT_FLAGS;

                    int ret = dwtct_filtdown_d(coef, n_coef, x, n_x, &par);
                    int ret_base = dwtct_filtdown_base_d(coef_base, n_coef, x, n_x, &par);
                    int eq = equal_d(coef, coef_base, n_coef, tol);

                    if (ret || ret_base || !eq)
                    {
                        printf("dwtct_filtdown_d:\n");
                        printf("ret: %d, ret_base: %d, eq: %d, n_filt: %d, x_shift: %d, n_x: %d, n_coef: %d, i_flag: %d\n", 
                            ret, ret_base, eq, (int)n_filt, x_shift, (int)n_x, (int)n_coef, (int)i_flag);
                        goto FAILED0;
                    }
                }
            }
        }
    }
/*
    // Haar wavelet
    double f_s[2] = {1.0, 1.0};
    double f_w[2] = {1.0, -1.0};
    double x[6] = {0.5, -1.0, 2.5, 2.5, 1.0, 1.0};
    double coef[6] = {0.0};
    double coef_base[6] = {0.0};

    dwtct_filt_param par;
    par.filter = f_s;
    par.n_filt = 2;
    par.x_shift = 0;
    par.flags = DWTCT_DEFAULT_FLAGS;

    const double tol = 1e-12;
    int ret = 0;

    if (dwtct_filtdown_d(coef, 3, x, 6, &par) ||
        dwtct_filtdown_base_d(coef_base, 3, x, 6, &par) ||
        !equal_d(coef, coef_base, 6, tol))
    {
        goto FAILED0;
    }

    ret = dwtct_filtdown_d(coef, 3, x, 6, &par);
    printf("ret: %d\n", ret);
    for (int i = 0; i < 3; ++i)
        printf("c_s[%d]: %3.2f\n", i, coef[i]);

    // try multithreading
    par.flags = DWTCT_DEFAULT_FLAGS | DWTCT_OPENMP;
    par.filter = f_w;
    ret = dwtct_filtdown_d(coef + 3, 3, x, 6, &par);
    printf("ret: %d\n", ret);
    for (int i = 0; i < 3; ++i)
        printf("c_w[%d]: %3.2f\n", i, coef[i + 3]);
*/

    printf("\n::TESTING: PASSED\n");
    return 0;

FAILED0:
    printf("\n::TESTING: FAILED\n");
    return 1;
}
