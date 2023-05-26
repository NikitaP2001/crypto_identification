#include <stdlib.h>

#include <gmp.h>
#include <check.h>

#include "ecdsa.h"

#define MODULE_NAME "ECDSA"
#define CORE_NAME "Core"

START_TEST(params_interval)
{
        struct ecdsa_params params = {0};
        ecdsa_init(&params);

        ck_assert_int_ge(mpz_cmp_ui(params.a, 0), 0);
        ck_assert_int_lt(mpz_cmp(params.a, params.p), 0);
        ck_assert_int_ge(mpz_cmp_ui(params.b, 0), 0);
        ck_assert_int_lt(mpz_cmp(params.b, params.p), 0);
        ck_assert_int_ge(mpz_cmp_ui(params.G.x, 0), 0);
        ck_assert_int_lt(mpz_cmp(params.G.y, params.p), 0);

        ecdsa_free(&params);
}
END_TEST


START_TEST(params_curve)
{
        struct ecdsa_params params = {0};
        ecdsa_init(&params);

        mpz_powm_ui(params.a, params.a, 3, params.p);
        mpz_mul_ui(params.a, params.a, 4);
        mpz_powm_ui(params.b, params.b, 2, params.p);
        mpz_mul_ui(params.b, params.b, 27);
        mpz_add(params.b, params.a, params.b);
        mpz_mod(params.b, params.b, params.p);
        ck_assert_int_ne(mpz_cmp_ui(params.b, 0), 0);

        ecdsa_free(&params);
}
END_TEST

START_TEST(points_curve)
{
        mpz_t temp;
        struct ecdsa_params params = {0};
        ecdsa_init(&params);        
        mpz_init(temp);

        mpz_powm_ui(params.G.x, params.G.y, 2, params.p);
        mpz_powm_ui(temp, params.G.x, 3, params.p);
        mpz_mul(params.G.x, params.a, params.G.x);
        mpz_add(temp, temp, params.G.x);
        mpz_add(temp, temp, params.b);
        mpz_mod(temp, temp, params.p);
        ck_assert_int_eq(mpz_cmp(params.G.x, temp), 0);

        mpz_clear(temp);
        ecdsa_free(&params);
}
END_TEST

START_TEST(params_n)
{
        struct ecdsa_params params = {0};
        ecdsa_init(&params);        

        ck_assert_int_ge(mpz_sizeinbase(params.n, 2), 160);
        mpz_sqrt(params.p, params.p);
        mpz_mul_ui(params.p, params.p, 4);
        ck_assert_int_gt(mpz_cmp(params.n, params.p), 0);

        ecdsa_free(&params);
}
END_TEST


Suite *ecdsa_suite()
{
        Suite *s = NULL;
        TCase *tc_core = NULL;
        s = suite_create(MODULE_NAME);
        tc_core = tcase_create(CORE_NAME);

        tcase_add_test(tc_core, params_interval);
        tcase_add_test(tc_core, params_curve);
        tcase_add_test(tc_core, points_curve);
        tcase_add_test(tc_core, params_n);
        
        suite_add_tcase(s, tc_core);
        return s;
}


int main()
{
        int number_failed = 0;       
        Suite *s_ecdsa = NULL;
        SRunner *sr = NULL;

        s_ecdsa = ecdsa_suite();
        
        sr = srunner_create(s_ecdsa);
        
        srunner_run_all(sr, CK_NORMAL);
        number_failed = srunner_ntests_failed(sr);
        srunner_free(sr);

        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
