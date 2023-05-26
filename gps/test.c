#include <stdlib.h>
#include <gmp.h>

#include <check.h>

#include <gmptools.h>
#include "gps.h"

#define MODULE_NAME "GPS"
#define CORE_NAME "Core"


START_TEST(domain_p_1024)
{
        struct schnorr_params params = {0};
        _Bool result = gps_init(&params, GPS_L_1024);
        ck_assert(result);
        ck_assert_int_eq(mpz_sizeinbase(params.p, 2), GPS_L_1024);

        /* is definitely prime */
        ck_assert_int_ne(mpz_probab_prime_p(params.p, 50), 0);

        gps_free(&params);
}
END_TEST

START_TEST(domain_q_1024)
{
        struct schnorr_params params = {0};
        _Bool result = gps_init(&params, GPS_L_1024);
        ck_assert(result);

        ck_assert_int_eq(mpz_sizeinbase(params.q, 2), GPS_N_160);

        ck_assert_int_ne(mpz_probab_prime_p(params.q, 50), 0);

        mpz_sub_ui(params.p, params.p, 1);
        mpz_mod(params.q, params.p, params.q);
        ck_assert_int_eq(mpz_cmp_ui(params.q, 0), 0);

        gps_free(&params);
}
END_TEST

START_TEST(domain_g_1024)
{
        mpz_t g_powq;
        mpz_init(g_powq);
        struct schnorr_params params = {0};
        _Bool result = gps_init(&params, GPS_L_1024);
        ck_assert(result);

        mpz_powm(g_powq, params.g, params.q, params.p);
        ck_assert_int_eq(mpz_cmp_ui(g_powq, 1), 0);
        mpz_clear(g_powq);
}

START_TEST(domain_p_2048)
{
        struct schnorr_params params = {0};
        _Bool result = gps_init(&params, GPS_L_2048);
        ck_assert(result);
        ck_assert_int_eq(mpz_sizeinbase(params.p, 2), GPS_L_2048);

        /* is definitely prime */
        ck_assert_int_ne(mpz_probab_prime_p(params.p, 50), 0);

        gps_free(&params);
}
END_TEST

START_TEST(domain_q_2048)
{
        struct schnorr_params params = {0};
        _Bool result = gps_init(&params, GPS_L_2048);
        ck_assert(result);

        ck_assert_int_eq(mpz_sizeinbase(params.q, 2), GPS_N_224);

        ck_assert_int_ne(mpz_probab_prime_p(params.q, 50), 0);

        mpz_sub_ui(params.p, params.p, 1);
        mpz_mod(params.q, params.p, params.q);
        ck_assert_int_eq(mpz_cmp_ui(params.q, 0), 0);

        gps_free(&params);
}
END_TEST

START_TEST(domain_p_3072)
{
        struct schnorr_params params = {0};
        _Bool result = gps_init(&params, GPS_L_3072);
        ck_assert(result);
        ck_assert_int_eq(mpz_sizeinbase(params.p, 2), GPS_L_3072);

        /* is definitely prime */
        ck_assert_int_ne(mpz_probab_prime_p(params.p, 50), 0);

        gps_free(&params);
}
END_TEST

START_TEST(domain_q_3072)
{
        struct schnorr_params params = {0};
        _Bool result = gps_init(&params, GPS_L_3072);
        ck_assert(result);

        ck_assert_int_eq(mpz_sizeinbase(params.q, 2), GPS_N_256);

        ck_assert_int_ne(mpz_probab_prime_p(params.q, 50), 0);

        mpz_sub_ui(params.p, params.p, 1);
        mpz_mod(params.q, params.p, params.q);
        ck_assert_int_eq(mpz_cmp_ui(params.q, 0), 0);

        gps_free(&params);
}
END_TEST


START_TEST(preprocess)
{
        mpz_t x, r, s, v;
        mpz_init(x);
        mpz_init(r);
        mpz_init(s);
        mpz_init(v);

        struct schnorr_params params = {0};
        gps_init(&params, GPS_L_1024);

        gps_user_keys(s, v, &params);

        gps_preprocess(&params, x, r);

        mpz_powm(params.g, params.g, r, params.p);
        ck_assert_int_eq(mpz_cmp(params.g, x), 0);

        gps_free(&params);
        mpz_clear(x);
        mpz_clear(r);
        mpz_clear(s);
        mpz_clear(v);
}
END_TEST

START_TEST(sign_e)
{
        mpz_t s, v, x, r, e, y;
        mpz_init(s);
        mpz_init(v);
        mpz_init(x);
        mpz_init(r);
        mpz_init(e);
        mpz_init(y);

        struct schnorr_params params = {0};
        gps_init(&params, GPS_L_1024);

        gps_user_keys(s, v, &params);

        gps_preprocess(&params, x, r);
        
        gmpt_rndmod(e, params.q);

        gps_sign(y, r, s, e);
        mpz_mul(s, s, e);
        mpz_add(s, s, r);
        ck_assert_int_eq(mpz_cmp(y, s) , 0);

        gps_free(&params);

        mpz_clear(v);
        mpz_clear(s);
        mpz_clear(x);
        mpz_clear(r);
        mpz_clear(e);
        mpz_clear(y);
}
END_TEST

#define VERIFY_TIMES 10
START_TEST(verify_true)
{
        mpz_t s, v, x, r, e, y, x_ver;
        mpz_init(s);
        mpz_init(v);
        mpz_init(x);
        mpz_init(x_ver);
        mpz_init(r);
        mpz_init(e);
        mpz_init(y);

        struct schnorr_params params = {0};
        gps_init(&params, GPS_L_1024);

        gps_user_keys(s, v, &params);

        for (int i = 0; i < VERIFY_TIMES; i++) {
                gps_preprocess(&params, x, r);
                
                gmpt_rndmod(e, params.q);

                gps_sign(y, r, s, e);
                
                gps_verify(&params, x_ver, y, v, e);
                ck_assert_int_eq(mpz_cmp(x, x_ver) , 0);
        }
        gps_free(&params);

        mpz_clear(v);
        mpz_clear(s);
        mpz_clear(x);
        mpz_clear(x_ver);
        mpz_clear(r);
        mpz_clear(e);
        mpz_clear(y);
}
END_TEST

START_TEST(verify_fakes)
{
        mpz_t s, v, x, r, e, y, x_ver;
        mpz_init(s);
        mpz_init(v);
        mpz_init(x);
        mpz_init(x_ver);
        mpz_init(r);
        mpz_init(e);
        mpz_init(y);

        struct schnorr_params params = {0};
        gps_init(&params, GPS_L_1024);

        gps_user_keys(s, v, &params);

        for (int i = 0; i < VERIFY_TIMES; i++) {
                gmpt_random(s, mpz_sizeinbase(s, 2));
                gps_preprocess(&params, x, r);
                
                gmpt_rndmod(e, params.q);

                gps_sign(y, r, s, e);
                
                gps_verify(&params, x_ver, y, v, e);
                ck_assert_int_ne(mpz_cmp(x, x_ver) , 0);
        }
        gps_free(&params);

        mpz_clear(v);
        mpz_clear(s);
        mpz_clear(x);
        mpz_clear(x_ver);
        mpz_clear(r);
        mpz_clear(e);
        mpz_clear(y);
}
END_TEST

Suite *gps_suite()
{
        Suite *s = NULL;
        TCase *tc_core = NULL;
        s = suite_create(MODULE_NAME);
        tc_core = tcase_create(CORE_NAME);

        tcase_add_test(tc_core, domain_p_1024);
        tcase_add_test(tc_core, domain_g_1024);
        tcase_add_test(tc_core, domain_q_1024);
        tcase_add_test(tc_core, domain_p_2048);
        tcase_add_test(tc_core, domain_q_2048);
        tcase_add_test(tc_core, domain_p_3072);
        tcase_add_test(tc_core, domain_q_3072);
        tcase_add_test(tc_core, preprocess);
        tcase_add_test(tc_core, sign_e);
        tcase_add_test(tc_core, verify_true);
        tcase_add_test(tc_core, verify_fakes);
        
        suite_add_tcase(s, tc_core);
        return s;
}



int main()
{
        int number_failed = 0;       
        Suite *s_gps = NULL;
        SRunner *sr = NULL;

        s_gps = gps_suite();
        
        sr = srunner_create(s_gps);
        
        srunner_run_all(sr, CK_NORMAL);
        number_failed = srunner_ntests_failed(sr);
        srunner_free(sr);

        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
