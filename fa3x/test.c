#include <stdlib.h>

#include <gmp.h>
#include <check.h>

#include "fa3x.h"

#define MODULE_NAME "FA3x"
#define CORE_NAME "Core"

START_TEST(rnd_hash_valid)
{
        struct controller_params params = {0};
        struct member_params a_params = {0}, b_params = {0};
        fa3x_key_t vr = {0}, pr = {0}, r_a = {0}, r_b = {0};
        fa3x_controller_init(&params, 100);

        fa3x_controller_add_memeber(&params, &a_params);
        fa3x_controller_add_memeber(&params, &b_params);

        fa3x_member_random(vr);
        fa3x_member_random(pr);

        fa3x_member_hash(r_a, vr, pr);
        fa3x_member_hash(r_b, vr, pr);
        ck_assert_int_eq(memcmp(r_a, r_b, sizeof(fa3x_key_t)), 0);
}
END_TEST

START_TEST(rnd_hash_invalid)
{
        struct controller_params params = {0};
        struct member_params a_params = {0}, b_params = {0};
        fa3x_key_t vr = {0}, pr = {0}, r_a = {0}, r_b = {0};
        fa3x_controller_init(&params, 100);

        fa3x_controller_add_memeber(&params, &a_params);
        fa3x_controller_add_memeber(&params, &b_params);

        fa3x_member_random(vr);
        fa3x_member_random(pr);

        fa3x_member_hash(r_a, vr, pr);
        
        fa3x_member_random(pr);
        fa3x_member_hash(r_b, vr, pr);

        ck_assert_int_ne(memcmp(r_a, r_b, sizeof(fa3x_key_t)), 0);
}
END_TEST

START_TEST(auth_true)
{
        struct controller_params params = {0};
        struct member_params a_params = {0}, b_params = {0};
        fa3x_key_t vr = {0}, pr = {0}, r = {0};
        fa3x_key_t in1 = {0}, in2 = {0};
        fa3x_controller_init(&params, 100);

        fa3x_controller_add_memeber(&params, &a_params);
        fa3x_controller_add_memeber(&params, &b_params);

        fa3x_member_random(vr);
        fa3x_member_random(pr);

        fa3x_member_hash(r, vr, pr);
        fa3x_member_in(&a_params, r, in1, in2);

        ck_assert(fa3x_member_auth(b_params.N, r, b_params.id, in1, in2));
}
END_TEST

START_TEST(auth_wrongid_prover)
{
        struct controller_params params = {0};
        struct member_params a_params = {0}, b_params = {0};
        fa3x_key_t vr = {0}, pr = {0}, r = {0};
        fa3x_key_t in1 = {0}, in2 = {0};
        fa3x_controller_init(&params, 100);

        fa3x_controller_add_memeber(&params, &a_params);
        fa3x_controller_add_memeber(&params, &b_params);

        fa3x_member_random(vr);
        fa3x_member_random(pr);

        fa3x_member_hash(r, vr, pr);
        fa3x_member_random(a_params.id);
        fa3x_member_in(&a_params, r, in1, in2);

        ck_assert(!fa3x_member_auth(b_params.N, r, b_params.id, in1, in2));
}
END_TEST

START_TEST(auth_wrongid_verifier)
{
        struct controller_params params = {0};
        struct member_params a_params = {0}, b_params = {0};
        fa3x_key_t vr = {0}, pr = {0}, r = {0};
        fa3x_key_t in1 = {0}, in2 = {0};
        fa3x_controller_init(&params, 100);

        fa3x_controller_add_memeber(&params, &a_params);
        fa3x_controller_add_memeber(&params, &b_params);

        fa3x_member_random(vr);
        fa3x_member_random(pr);

        fa3x_member_hash(r, vr, pr);
        fa3x_member_in(&a_params, r, in1, in2);

        fa3x_member_random(b_params.id);
        ck_assert(!fa3x_member_auth(b_params.N, r, b_params.id, in1, in2));
}
END_TEST

Suite *fa3x_suite()
{
        Suite *s = NULL;
        TCase *tc_core = NULL;
        s = suite_create(MODULE_NAME);
        tc_core = tcase_create(CORE_NAME);

        tcase_add_test(tc_core, rnd_hash_valid);
        tcase_add_test(tc_core, rnd_hash_invalid);
        tcase_add_test(tc_core, auth_true);
        tcase_add_test(tc_core, auth_wrongid_prover);
        tcase_add_test(tc_core, auth_wrongid_verifier);
        
        suite_add_tcase(s, tc_core);
        return s;
}


int main()
{
        int number_failed = 0;       
        Suite *s_fa3x = NULL;
        SRunner *sr = NULL;

        s_fa3x = fa3x_suite();
        
        sr = srunner_create(s_fa3x);
        
        srunner_run_all(sr, CK_NORMAL);
        number_failed = srunner_ntests_failed(sr);
        srunner_free(sr);

        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
