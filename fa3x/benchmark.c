#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include <sys/time.h>

#include <gmptools.h>
#include "fa3x.h"


_Bool benchmark_round(size_t N, double *t_prov, double *t_ver)
{
        struct controller_params params = {0};
        struct member_keys a_params = {0}, b_params = {0};
        fa3x_key_t vr = {0}, pr = {0}, ra = {0}, rb = {0};
        fa3x_key_t in1 = {0}, in2 = {0};

        fa3x_controller_init(&params, N);
        fa3x_controller_add_memeber(&params, &a_params);
        fa3x_controller_add_memeber(&params, &b_params);

        double time = get_current_time();
        fa3x_member_random(vr);
        *t_ver += get_current_time() - time;

        time = get_current_time();
        fa3x_member_random(pr);
        fa3x_member_hash(ra, vr, pr);
        fa3x_member_in(&a_params, ra, in1, in2);
        *t_prov += get_current_time() - time;

        time = get_current_time();
        fa3x_member_hash(rb, vr, pr);
        assert(memcmp(ra, rb, sizeof(fa3x_key_t)) == 0);
        assert(fa3x_member_auth(b_params.N, rb, b_params.id, in1, in2));
        *t_ver += get_current_time() - time;
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

                benchmark_round(10, &t_prov, &t_ver);

                print_private_usage();

        } else {
                for (int i = 10; i < 10e5; i *= 10) {

                        benchmark(i, 100, &t_prov, &t_ver);
                        t_prov *= 1e6;
                        t_ver *= 1e6;
                        printf("N = %d", i);
                        printf("\n\ravg time prove: %.1f\n", t_prov);
                        printf("\n\ravg time verify: %.1f\n\n", t_ver);
                        printf("\n\rtotal time: %.1f\n\n", t_ver + t_prov);
                }
        }
        
        

        exit(0);
}