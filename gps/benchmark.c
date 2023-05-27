#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <sys/time.h>

#include <gmptools.h>
#include "gps.h"


void benchmark_round(size_t keysize, double *t_proover, double *t_verifier)
{
        mpz_t s, v, x, x_ver, r, e, y;
        mpz_init(s);
        mpz_init(v);
        mpz_init(x);
        mpz_init(x_ver);
        mpz_init(r);
        mpz_init(e);
        mpz_init(y);

        struct schnorr_params params = {0};
        gps_init(&params, keysize);

        gps_user_keys(s, v, &params);
        gmp_printf("s: %Zd\n", s);
        gmp_printf("v: %Zd\n", v);

        double time;
        /* assume this is precomputed on coupon*/

        gps_preprocess(&params, x, r);
        gmp_printf("x: %Zd\n", x);
        gmp_printf("r: %Zd\n", r);

        /* A->B: v, x */
        /* B generates challenge e */
        time = get_current_time();
        gmpt_rndmod(e, params.q);
        *t_verifier = get_current_time() - time;
        /* B->A: e */
        time = get_current_time();
        gps_sign(y, r, s, e);
        *t_proover += get_current_time() - time;
        /* A->B: y */
        time = get_current_time();
        gps_verify(&params, x_ver, y, v, e);
        assert(mpz_cmp(x, x_ver) == 0);
        *t_verifier += get_current_time() - time;

        gps_free(&params);

        mpz_clear(v);
        mpz_clear(s);
        mpz_clear(x);
        mpz_clear(x_ver);
        mpz_clear(r);
        mpz_clear(e);
        mpz_clear(y);
}


/* note: in schnorr_rndmod params.q should not be used instead - special parameter */
void benchmark(size_t times, size_t keysize, double *t_proover, double *t_verifier)
{
        *t_proover = 0;
        *t_verifier = 0;
        for (size_t round = 0; round < times; round++) {
                double round_prover = 0, round_verifier = 0;
                benchmark_round(keysize, &round_prover, &round_verifier);
                *t_proover += round_prover; 
                *t_verifier += round_verifier;
        }
        *t_proover /= times;
        *t_verifier /= times;
}

int main(int argc, char *argv[])
{
        double time_prov, time_verify;

        if (argc > 1 && strcmp(argv[1], "round") == 0) {
                benchmark_round(GPS_L_1024, &time_prov, &time_verify);
                benchmark_round(GPS_L_2048, &time_prov, &time_verify);
                benchmark_round(GPS_L_3072, &time_prov, &time_verify);
        } else {
                benchmark(100, GPS_L_1024, &time_prov, &time_verify);

                printf("Keysize %d \n", GPS_L_1024);
                printf("\tTotal prover time: %.7f sec\n", time_prov);
                printf("\tTotal verifier time: %.7f sec\n", time_verify);

                benchmark(100, GPS_L_2048, &time_prov, &time_verify);

                printf("\n\rKeysize %d \n", GPS_L_2048);
                printf("\tTotal prover time: %.7f sec\n", time_prov);
                printf("\tTotal verifier time: %.7f sec\n", time_verify);

                benchmark(100, GPS_L_3072, &time_prov, &time_verify);

                printf("\n\rKeysize %d \n", GPS_L_3072);
                printf("\tTotal prover time: %.7f sec\n", time_prov);
                printf("\tTotal verifier time: %.7f sec\n", time_verify);
        }
        

        exit(0);
}