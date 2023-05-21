#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <sys/time.h>

#include "fa3x.h"

static double get_current_time() {
	struct timeval t;
	gettimeofday(&t, 0);
	return t.tv_sec + t.tv_usec*1e-6;
}

int main()
{
        struct controller_params params = {0};
        struct member_params a_params = {0}, b_params = {0};
        fa3x_key_t vr = {0}, pr = {0}, r = {0};
        fa3x_key_t in1 = {0}, in2 = {0};
        fa3x_controller_init(&params, 10);

        fa3x_controller_add_memeber(&params, &a_params);
        fa3x_controller_add_memeber(&params, &b_params);

        fa3x_member_random(vr);

        double time = get_current_time();

        fa3x_member_random(pr);
        fa3x_member_hash(r, vr, pr);
        fa3x_member_in(&a_params, r, in1, in2);

        fa3x_member_hash(r, vr, pr);
        assert(fa3x_member_auth(b_params.N, r, b_params.id, in1, in2));

        printf("%f", get_current_time() - time);

        exit(0);
}