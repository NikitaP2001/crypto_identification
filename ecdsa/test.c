#include <stdbool.h>
#include <stdlib.h>

#include <gmp.h>
#include <check.h>

#include <gmptools.h>
#include "ecdsa.h"

#define MODULE_NAME "ECDSA"
#define CORE_NAME "Core"

static void test_params_interval(struct ecdsa_params *params)
{
        ck_assert_int_ge(mpz_cmp_ui(params->a, 0), 0);
        ck_assert_int_lt(mpz_cmp(params->a, params->p), 0);
        ck_assert_int_ge(mpz_cmp_ui(params->b, 0), 0);
        ck_assert_int_lt(mpz_cmp(params->b, params->p), 0);
        ck_assert_int_ge(mpz_cmp_ui(params->G.x, 0), 0);
        ck_assert_int_lt(mpz_cmp(params->G.y, params->p), 0);
}

START_TEST(params_interval)
{
        struct ecdsa_params params = {0};
        ecdsa_init(&params, ECDSA_P256);
        test_params_interval(&params);
        ecdsa_free(&params);
        ecdsa_init(&params, ECDSA_P384);
        test_params_interval(&params);
        ecdsa_free(&params);
        ecdsa_init(&params, ECDSA_P521);
        test_params_interval(&params);
        ecdsa_free(&params);
}
END_TEST

static void test_params_curve(struct ecdsa_params *params)
{
        mpz_powm_ui(params->a, params->a, 3, params->p);
        mpz_mul_ui(params->a, params->a, 4);
        mpz_powm_ui(params->b, params->b, 2, params->p);
        mpz_mul_ui(params->b, params->b, 27);
        mpz_add(params->b, params->a, params->b);
        mpz_mod(params->b, params->b, params->p);
        ck_assert_int_ne(mpz_cmp_ui(params->b, 0), 0);
}

START_TEST(params_curve)
{
        struct ecdsa_params params = {0};
        ecdsa_init(&params, ECDSA_P256);
        test_params_curve(&params);
        ecdsa_free(&params);

        ecdsa_init(&params, ECDSA_P384);
        test_params_curve(&params);
        ecdsa_free(&params);

        ecdsa_init(&params, ECDSA_P521);
        test_params_curve(&params);
        ecdsa_free(&params);
}
END_TEST

static void test_points_curve(struct ecdsa_params *params)
{
        mpz_t temp;
        
        mpz_init(temp);

        mpz_powm_ui(params->G.y, params->G.y, 2, params->p);
        mpz_powm_ui(temp, params->G.x, 3, params->p);
        mpz_mul(params->G.x, params->a, params->G.x);
        mpz_add(temp, temp, params->G.x);
        mpz_add(temp, temp, params->b);
        mpz_mod(temp, temp, params->p);
        ck_assert_int_eq(mpz_cmp(params->G.y, temp), 0);

        mpz_clear(temp);
}

START_TEST(points_curve)
{
        struct ecdsa_params params = {0};
        ecdsa_init(&params, ECDSA_P256);
        test_points_curve(&params);
        ecdsa_free(&params);

        ecdsa_init(&params, ECDSA_P384);
        test_points_curve(&params);
        ecdsa_free(&params);

        ecdsa_init(&params, ECDSA_P521);
        test_points_curve(&params);
        ecdsa_free(&params);
}
END_TEST

START_TEST(params_n)
{
        struct ecdsa_params params = {0};
        ecdsa_init(&params, ECDSA_P256);
        ck_assert_int_ge(mpz_sizeinbase(params.n, 2), 160);
        mpz_sqrt(params.p, params.p);
        mpz_mul_ui(params.p, params.p, 4);
        ck_assert_int_gt(mpz_cmp(params.n, params.p), 0);
        ecdsa_free(&params);
}
END_TEST

static _Bool test_sign(size_t p, _Bool valid, _Bool ldkeys)
{
        _Bool result = false;
        mpz_t e, s, r;
        mpz_init(e);
        mpz_init(s);
        mpz_init(r);
        struct ecdsa_params params = {0};
        struct member_keys keys = {0};

        ecdsa_init(&params, p);

        if (ldkeys)
                ecdsa_keys_load(&params, &keys);
        else
                ecdsa_keys_create(&params, &keys);

        gmpt_rndmod(e, params.p);

        ecdsa_sign(&params, &keys, e, r, s);

        if (!valid)
                gmpt_rndmod(e, params.p);

        result = ecdsa_verify(&params, keys.Q, r, s, e);

        ecdsa_keys_destroy(&keys);
        ecdsa_free(&params);
        mpz_clear(e);
        mpz_clear(r);
        mpz_clear(s);
        return result;
}

START_TEST(sign_valid)
{
        ck_assert(test_sign(ECDSA_P256, true, false));
        ck_assert(test_sign(ECDSA_P256, true, true));
        ck_assert(test_sign(ECDSA_P384, true, false));
        ck_assert(test_sign(ECDSA_P384, true, true));
        ck_assert(test_sign(ECDSA_P521, true, false));
        ck_assert(test_sign(ECDSA_P521, true, true));
}

START_TEST(sign_invalid)
{
        ck_assert(!test_sign(ECDSA_P256, false, false));
        ck_assert(!test_sign(ECDSA_P256, false, true));
        ck_assert(!test_sign(ECDSA_P384, false, false));
        ck_assert(!test_sign(ECDSA_P384, false, true));
        ck_assert(!test_sign(ECDSA_P521, false, false));
        ck_assert(!test_sign(ECDSA_P521, false, true));
}


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
        tcase_add_test(tc_core, sign_valid);
        tcase_add_test(tc_core, sign_invalid);
        
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
