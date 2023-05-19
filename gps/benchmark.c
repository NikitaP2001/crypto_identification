#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <sys/time.h>

#include "dsa.h"


double get_current_time() {
	struct timeval t;
	gettimeofday(&t, 0);
	return t.tv_sec + t.tv_usec*1e-6;
}

int main()
{
        

        exit(0);
}