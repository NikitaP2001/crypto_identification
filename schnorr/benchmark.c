#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <sys/time.h>

#include <gmptools.h>
#include "schnorr.h"


void benchmark_round(size_t keysize, double *t_prover, double *t_verifier)
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
        schnorr_init(&params, keysize);

        schnorr_load_keys(s, v, &params);

        double prv_start = get_current_time();
        schnorr_preprocess(&params, x, r);
        *t_prover = get_current_time() - prv_start;
        /* A->B: v, x */
        /* B generates challenge e */
        double ver_start = get_current_time();
        gmpt_rndmod(e, params.q);
        *t_verifier = get_current_time() - ver_start;
        /* B->A: e */
        prv_start = get_current_time();
        schnorr_sign(&params, y, r, s, e);
        *t_prover += get_current_time() - prv_start;
        /* A->B: y */
        ver_start = get_current_time();
        schnorr_verify(&params, x_ver, y, v, e);
        assert(mpz_cmp(x, x_ver) == 0);
        *t_verifier += get_current_time() - ver_start;

        schnorr_free(&params);

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
        double time_prov = 0, time_verify = 0;
        if (argc > 1 && strcmp(argv[1], "runempty") == 0) {

                print_private_usage();

        } else if (argc > 1 && strcmp(argv[1], "round") == 0) {

                benchmark_round(DSA_L_3072, &time_prov, &time_verify);

                print_private_usage();

        } else {

                benchmark(100, DSA_L_1024, &time_prov, &time_verify);
                time_prov *= 1e6;
                time_verify *= 1e6;

                printf("Keysize %d \n", DSA_L_1024);
                printf("\tTotal prover time: %.1f sec\n", time_prov);
                printf("\tTotal verifier time: %.1f sec\n", time_verify);
                printf("\tTotal time: %.1f sec\n", time_verify + time_prov);

                benchmark(100, DSA_L_2048, &time_prov, &time_verify);
                time_prov *= 1e6;
                time_verify *= 1e6;
                printf("\n\rKeysize %d \n", DSA_L_2048);
                printf("\tTotal prover time: %.1f sec\n", time_prov);
                printf("\tTotal verifier time: %.1f sec\n", time_verify);
                printf("\tTotal time: %.1f sec\n", time_verify + time_prov);

                benchmark(100, DSA_L_3072, &time_prov, &time_verify);
                time_prov *= 1e6;
                time_verify *= 1e6;
                printf("\n\rKeysize %d \n", DSA_L_3072);
                printf("\tTotal prover time: %.1f sec\n", time_prov);
                printf("\tTotal verifier time: %.1f sec\n", time_verify);
                printf("\tTotal time: %.1f sec\n", time_verify + time_prov);

        }

        exit(0);
}