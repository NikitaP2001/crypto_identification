#include <stdlib.h>
#include <stdio.h>

#include <sys/time.h>

#include "ffs.h"

_Bool benchmark_round(size_t N, double *t_prov, double *t_ver)
{
        
}


void benchmark(size_t N, size_t times, double *t_prov, double *t_ver)
{
        
}

int main()
{
        for (int i = 10; i < 10e5; i *= 10) {
                double t_prov = 0, t_ver = 0;         

                benchmark(i, 100, &t_prov, &t_ver);
                printf("N = %d", i);
                printf("\n\ravg time prove: %f\n", t_prov);
                printf("\n\ravg time verify: %f\n\n", t_ver);
        }
        

        exit(0);
}