#include <stdio.h>
#include "dwtct.h"

DWTCT_EI void testfunc(double d, int *a)
{
	int p = (int)d*5;
	*a = p;
	#ifdef DWTCT_EXPORT
	printf("exported\n");
	#else
	printf("not exported\n");
	#endif
}
