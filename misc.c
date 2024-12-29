#include "defs.h"
#include <stdio.h>
#include <time.h>

long get_time_ms(void) {
	return (long) (((float) clock() / CLOCKS_PER_SEC) * 1000.0f);
}
