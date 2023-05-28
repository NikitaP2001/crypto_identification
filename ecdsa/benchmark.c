#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <sys/time.h>

#include <gmptools.h>
#include "ecdsa.h"

_Bool benchmark_round(size_t p, double *t_prov, double *t_ver)
{
        mpz_t e, s, r;
        mpz_init(e);
        mpz_init(s);
        mpz_init(r);
        struct ecdsa_params params = {0};
        struct member_keys keys = {0};

        ecdsa_init(&params, p);

        ecdsa_keys_load(&params, &keys);

        double time = get_current_time();
        gmpt_rndmod(e, params.p);
        *t_ver += get_current_time() - time;

        time = get_current_time();
        ecdsa_sign(&params, &keys, e, r, s);
        *t_prov += get_current_time() - time;

        time = get_current_time();
        assert(ecdsa_verify(&params, keys.Q, r, s, e));
        *t_ver += get_current_time() - time;

        ecdsa_keys_destroy(&keys);
        ecdsa_free(&params);
        mpz_clear(e);
        mpz_clear(r);
        mpz_clear(s);
        return true;
}


void benchmark(size_t p, size_t times, double *t_prov, double *t_ver)
{
        *t_prov = 0;
        *t_ver = 0;
        for (size_t i = 0; i < times; i++)
                benchmark_round(p, t_prov, t_ver);
        *t_prov /= times;
        *t_ver /= times;
}


int main(int argc, char *argv[])
{
        double t_prov = 0, t_ver = 0;

        if (argc > 1 && strcmp(argv[1], "runempty") == 0) {

                print_private_usage();

        } else if (argc > 1 && strcmp(argv[1], "round") == 0) {

                benchmark_round(ECDSA_P521, &t_prov, &t_ver);

                print_private_usage();

        } else {

                benchmark(ECDSA_P256, 100, &t_prov, &t_ver);
                t_prov *= 1e6;
                t_ver *= 1e6;
                printf("P-256\n");
                printf("\n\ravg time prove: %.1f\n", t_prov);
                printf("\n\ravg time verify: %.1f\n\n", t_ver);
                printf("\n\r total time: %.1f\n\n", t_ver + t_prov);

                benchmark(ECDSA_P384, 100, &t_prov, &t_ver);
                t_prov *= 1e6;
                t_ver *= 1e6;
                printf("P-384\n");
                printf("\n\ravg time prove: %.1f\n", t_prov);
                printf("\n\ravg time verify: %.1f\n\n", t_ver);
                printf("\n\r total time: %.1f\n\n", t_ver + t_prov);

                benchmark(ECDSA_P521, 100, &t_prov, &t_ver);
                t_prov *= 1e6;
                t_ver *= 1e6;
                printf("P-521\n");
                printf("\n\ravg time prove: %.1f\n", t_prov);
                printf("\n\ravg time verify: %.1f\n\n", t_ver);
                printf("\n\r total time: %.1f\n\n", t_ver + t_prov);
        }

        exit(0);
}