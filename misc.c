#include <stdio.h>
#include <time.h>
#include "defs.h"

long get_time_ms(void) {
	return (long) ((clock() / CLOCKS_PER_SEC ) * 1000);
}
