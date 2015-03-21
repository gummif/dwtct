#include <stdio.h>
#include "dwtct.h"

int main()
{
	printf("\n::TESTING\n");

	double f_s[2] = {1.0, 1.0};
	double f_w[2] = {1.0, -1.0};
	double x[6] = {0.5, -1.0, 2.5, 2.5, 1.0, 1.0};
	double coef[6] = {0.0};

	dwtct_filt_param par;
	par.filter = f_s;
	par.n_filt = 2;
	par.x_shift = 0;
	par.flags = DWTCT_DEFAULT_FLAGS;

	int ret = 0;

	ret = dwtct_filtdown_d(coef, 3, x, 6, &par);
	printf("ret: %d\n", ret);
	for (int i = 0; i < 3; ++i)
		printf("c_s[%d]: %3.2f\n", i, coef[i]);

	par.filter = f_w;
	ret = dwtct_filtdown_d(coef + 3, 3, x, 6, &par);
	printf("ret: %d\n", ret);
	for (int i = 0; i < 3; ++i)
		printf("c_w[%d]: %3.2f\n", i, coef[i + 3]);

	printf("\n::TESTING DONE\n");
	return 0;
}
