#include <stdio.h>
#include "dwtct.h"

int main()
{
	printf("\n::TESTING\n");
	double val = 3;
	int ret = 0;
	testfunc(val, &ret);
	printf("ret: %d\n", ret);

	double a = 2.2;
	double b = 2.5;
	dwtct_filt_param p;
	p.filter = NULL;
	p.x_shift = 3;
	p.x_shift = 4;
	p.filter = &a;
	p.filter = &b;
	p=p;
	printf("\n::TESTING DONE\n");
	return 0;
}
