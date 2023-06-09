#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <sys/time.h>

#include <gmptools.h>
#include "gq.h"

_Bool benchmark_round(size_t N, double *t_prov, double *t_ver)
{
        mpz_t a, r, c, z;
        mpz_init(a);
        mpz_init(r);
        mpz_init(c);
        mpz_init(z);
        struct gq_params t_params = {0};        
        struct member_keys m_params = {0};

        assert(gq_init(&t_params, N));
        gq_load_keys(&t_params, &m_params);

        double time = get_current_time();
        gq_commitment(&m_params, a, r);
        *t_prov += get_current_time() - time;

        time = get_current_time();
        gmpt_rndmod(c, t_params.e);
        *t_ver += get_current_time() - time;

        time = get_current_time();
        gq_solve(&m_params, z, r, c);
        *t_prov += get_current_time() - time;

        time = get_current_time();
        assert(gq_verify(z, m_params.e, a, m_params.y, c, m_params.n));
        *t_ver += get_current_time() - time;

        gq_keys_destroy(&m_params);
        gq_free(&t_params);
        mpz_clear(a);
        mpz_clear(r);
        mpz_clear(c);
        mpz_clear(z);
        return true;
}

void benchmark(size_t N, size_t times, double *t_prov, double *t_ver)
{
        *t_prov = 0;
        *t_ver = 0;

        for (size_t i = 0; i < times; i++)
                assert(benchmark_round(N, t_prov, t_ver));

        *t_prov /= times;
        *t_ver /= times;
}


int main(int argc, char *argv[])
{
        double t_prov = 0, t_ver = 0;
        if (argc > 1 && strcmp(argv[1], "runempty") == 0) {

                print_private_usage();

        } else if (argc > 1 && strcmp(argv[1], "round") == 0) {

                benchmark_round(GQ_NLEN_3072, &t_prov, &t_ver);

                print_private_usage();

        } else {
                benchmark(GQ_NLEN_1024, 100, &t_prov, &t_ver);
                t_prov *= 1e6;
                t_ver *= 1e6;
                printf("N = %d", GQ_NLEN_1024);
                printf("\n\ravg time prove: %.1f\n", t_prov);
                printf("\n\ravg time verify: %.1f\n\n", t_ver);
                printf("\n\rtotal time: %.1f\n\n", t_ver + t_prov);

                benchmark(GQ_NLEN_2048, 100, &t_prov, &t_ver);
                t_prov *= 1e6;
                t_ver *= 1e6;
                printf("N = %d", GQ_NLEN_2048);
                printf("\n\ravg time prove: %.1f\n", t_prov);
                printf("\n\ravg time verify: %.1f\n\n", t_ver);
                printf("\n\rtotal time: %.1f\n\n", t_ver + t_prov);

                benchmark(GQ_NLEN_3072, 100, &t_prov, &t_ver);
                t_prov *= 1e6;
                t_ver *= 1e6;
                printf("N = %d", GQ_NLEN_3072);
                printf("\n\ravg time prove: %.1f\n", t_prov);
                printf("\n\ravg time verify: %.1f\n\n", t_ver);
                printf("\n\rtotal time: %.1f\n\n", t_ver + t_prov);
        }

        exit(0);
}