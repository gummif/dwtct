#include <stdio.h>
#include "dwtct.h"

int main()
{
	double val = 3;
	int ret = 0;
	testfunc(val, &ret);
	printf("ret: %d\n", ret);
	return 0;
}
