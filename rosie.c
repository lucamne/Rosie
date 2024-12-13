#include "defs.h"
#include "square_lookup.h"

#include <stdio.h>

int main()
{
	// AllInit();
	// printf("%d", sq120ToSq64[35]);
	
	for (int i = 0; i < BRD_SQ_NUM; i++) {
		if (i % 10 == 0) printf("\n");
		printf("%5d", sq120ToSq64[i]);
	}

	printf("\n");
	printf("\n");
	
	for (int i = 0; i < 64; i++) {
		if (i % 8 == 0) printf("\n");
		printf("%5d", sq64ToSq120[i]);
	}
	printf("\n");

	return 0;
}
