#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <gmptools.h>
#include "ffs.h"


void benchmark_round(double *t_prov, double *t_ver, size_t bit_size)
{
        mpz_t E, X, Y, R;
        mpz_init(E);
        mpz_init(X);
        mpz_init(Y);
        mpz_init(R);
        struct ffs_params params = {0};
        struct ffs_member_params mparams = {0};
        ffs_init(&params, bit_size);

        ffs_keys_create(&mparams, params.n);

        for (size_t round = 0; round < params.t; round++) {

                double time = get_current_time();
                ffs_commitment(X, R, params.n);
                *t_prov += get_current_time() - time;

                time = get_current_time();
                gmpt_random(E, params.k);

                time = get_current_time();
                ffs_solve(&mparams, Y, E, R, params.n);
                *t_prov += get_current_time() - time;

                time = get_current_time();
                assert(ffs_verify(mparams.i_arr, X, Y, E, params.n));
                *t_ver += get_current_time() - time;
        }
        
        ffs_keys_destroy(&mparams);
        ffs_free(&params);
        mpz_clear(E); 
        mpz_clear(X); 
        mpz_clear(Y); 
        mpz_clear(R); 
}


void benchmark(size_t bit_size, size_t times, double *t_prov, double *t_ver)
{
        *t_prov = 0;
        *t_ver = 0;
        for (size_t i = 0; i < times; i++)
                benchmark_round(t_prov, t_ver, bit_size);
        *t_prov /= times;
        *t_ver /= times;
}

int main(int argc, char *argv[])
{
        double t_prov = 0, t_ver = 0;
        if (argc > 1 && strcmp(argv[1], "runempty") == 0) {

                print_private_usage();

        } else if (argc > 1 && strcmp(argv[1], "round") == 0) {

                benchmark_round(&t_prov, &t_ver, FFS_NLEN_3072);

                print_private_usage();

        } else {
                benchmark(FFS_NLEN_1024, 100, &t_prov, &t_ver);
                printf("N = %d", FFS_NLEN_1024);
                printf("\n\ravg time prove: %.7f\n", t_prov);
                printf("\n\ravg time verify: %.7f\n\n", t_ver);

                benchmark(FFS_NLEN_2048, 100, &t_prov, &t_ver);
                printf("N = %d", FFS_NLEN_2048);
                printf("\n\ravg time prove: %.7f\n", t_prov);
                printf("\n\ravg time verify: %.7f\n\n", t_ver);

                benchmark(FFS_NLEN_3072, 100, &t_prov, &t_ver);
                printf("N = %d", FFS_NLEN_3072);
                printf("\n\ravg time prove: %.7f\n", t_prov);
                printf("\n\ravg time verify: %.7f\n\n", t_ver);
        }

        exit(0);
}