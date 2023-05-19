#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <sys/time.h>

#include "schnorr.h"




double get_current_time() {
	struct timeval t;
	gettimeofday(&t, 0);
	return t.tv_sec + t.tv_usec*1e-6;
}

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
        schnorr_init(&params, keysize);

        schnorr_user_keys(s, v, &params);

        double prv_start = get_current_time();
        schnorr_preprocess(&params, x, r);
        *t_proover = get_current_time() - prv_start;
        /* A->B: v, x */
        /* B generates challenge e */
        double ver_start = get_current_time();
        schnorr_rndmod(e, params.q);
        *t_verifier = get_current_time() - ver_start;
        /* B->A: e */
        prv_start = get_current_time();
        schnorr_sign(&params, y, r, s, e);
        *t_proover += get_current_time() - prv_start;
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

int main()
{
        double time_prov, time_verify;

        benchmark(100, DSA_L_1024, &time_prov, &time_verify);

        printf("Keysize %d \n", DSA_L_1024);
        printf("\tTotal prover time: %f sec\n", time_prov);
        printf("\tTotal verifier time: %f sec\n", time_verify);

        benchmark(100, DSA_L_2048, &time_prov, &time_verify);

        printf("\n\rKeysize %d \n", DSA_L_2048);
        printf("\tTotal prover time: %f sec\n", time_prov);
        printf("\tTotal verifier time: %f sec\n", time_verify);

        benchmark(100, DSA_L_3072, &time_prov, &time_verify);

        printf("\n\rKeysize %d \n", DSA_L_3072);
        printf("\tTotal prover time: %f sec\n", time_prov);
        printf("\tTotal verifier time: %f sec\n", time_verify);

        exit(0);
}