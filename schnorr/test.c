#include <stdlib.h>
#include <gmp.h>

#include <check.h>

#include "schnorr.h"
#include "sha256.h"

#define MODULE_NAME "DSA"
#define CORE_NAME "Core"


START_TEST(domain_p_1024)
{
        struct schnorr_params params = {0};
        _Bool result = schnorr_init(&params, DSA_L_1024);
        ck_assert(result);
        ck_assert_int_eq(mpz_sizeinbase(params.p, 2), DSA_L_1024);

        /* is definitely prime */
        ck_assert_int_ne(mpz_probab_prime_p(params.p, 50), 0);

        schnorr_free(&params);
}
END_TEST

START_TEST(domain_q_1024)
{
        struct schnorr_params params = {0};
        _Bool result = schnorr_init(&params, DSA_L_1024);
        ck_assert(result);

        ck_assert_int_eq(mpz_sizeinbase(params.q, 2), DSA_N_160);

        ck_assert_int_ne(mpz_probab_prime_p(params.q, 50), 0);

        mpz_sub_ui(params.p, params.p, 1);
        mpz_mod(params.q, params.p, params.q);
        ck_assert_int_eq(mpz_cmp_ui(params.q, 0), 0);

        schnorr_free(&params);
}
END_TEST

START_TEST(domain_p_2048)
{
        struct schnorr_params params = {0};
        _Bool result = schnorr_init(&params, DSA_L_2048);
        ck_assert(result);
        ck_assert_int_eq(mpz_sizeinbase(params.p, 2), DSA_L_2048);

        /* is definitely prime */
        ck_assert_int_ne(mpz_probab_prime_p(params.p, 50), 0);

        schnorr_free(&params);
}
END_TEST

START_TEST(domain_q_2048)
{
        struct schnorr_params params = {0};
        _Bool result = schnorr_init(&params, DSA_L_2048);
        ck_assert(result);

        ck_assert_int_eq(mpz_sizeinbase(params.q, 2), DSA_N_224);

        ck_assert_int_ne(mpz_probab_prime_p(params.q, 50), 0);

        mpz_sub_ui(params.p, params.p, 1);
        mpz_mod(params.q, params.p, params.q);
        ck_assert_int_eq(mpz_cmp_ui(params.q, 0), 0);

        schnorr_free(&params);
}
END_TEST

START_TEST(domain_p_3072)
{
        struct schnorr_params params = {0};
        _Bool result = schnorr_init(&params, DSA_L_3072);
        ck_assert(result);
        ck_assert_int_eq(mpz_sizeinbase(params.p, 2), DSA_L_3072);

        /* is definitely prime */
        ck_assert_int_ne(mpz_probab_prime_p(params.p, 50), 0);

        schnorr_free(&params);
}
END_TEST

START_TEST(domain_q_3072)
{
        struct schnorr_params params = {0};
        _Bool result = schnorr_init(&params, DSA_L_3072);
        ck_assert(result);

        ck_assert_int_eq(mpz_sizeinbase(params.q, 2), DSA_N_256);

        ck_assert_int_ne(mpz_probab_prime_p(params.q, 50), 0);

        mpz_sub_ui(params.p, params.p, 1);
        mpz_mod(params.q, params.p, params.q);
        ck_assert_int_eq(mpz_cmp_ui(params.q, 0), 0);

        schnorr_free(&params);
}
END_TEST

#define SHA_TEST_TIMES 50
START_TEST(sha256_same)
{
        mpz_t msg;
        mpz_init(msg);

        const size_t bitlen = 64 * CHAR_BIT * 6;
        for (int i = 0; i < SHA_TEST_TIMES; i++) {
                uint32_t state_msg[SHA256_STDWCNT], state_msg2[SHA256_STDWCNT], bufsize = 0;
                schnorr_random(msg, bitlen);

                uint8_t *buffer = schnorr_export(msg, &bufsize);
                sha256_state_init(state_msg);
                sha256_process_x86(state_msg, buffer, bufsize);

                uint8_t *buffer2 = schnorr_export(msg, &bufsize);
                sha256_state_init(state_msg2);
                sha256_process_x86(state_msg2, buffer2, bufsize);
                for (int sti = 0; sti < SHA256_STDWCNT; sti++)
                        ck_assert_int_eq(state_msg[sti], state_msg2[sti]);
                free(buffer2);
                free(buffer);
        }
        mpz_clear(msg);
}
END_TEST

START_TEST(sha256_differ)
{
        mpz_t msg;
        mpz_init(msg);

        const size_t bitlen = 64 * CHAR_BIT * 6;
        for (int i = 0; i < SHA_TEST_TIMES; i++) {
                uint32_t state_msg[SHA256_STDWCNT], state_msg2[SHA256_STDWCNT], bufsize = 0;
                schnorr_random(msg, bitlen);

                uint8_t* buffer = schnorr_export(msg, &bufsize);
                sha256_state_init(state_msg);
                sha256_process_x86(state_msg, buffer, bufsize);

                mpz_sub_ui(msg, msg, 1);
                uint8_t *buffer2 = schnorr_export(msg, &bufsize);
                sha256_state_init(state_msg2);
                sha256_process_x86(state_msg2, buffer2, bufsize);
                for (int sti = 0; sti < SHA256_STDWCNT; sti++)
                        ck_assert_int_ne(state_msg[sti], state_msg2[sti]);
                free(buffer2);
                free(buffer);
        }
        mpz_clear(msg);
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
        schnorr_init(&params, DSA_L_1024);

        schnorr_user_keys(s, v, &params);

        schnorr_preprocess(&params, x, r);

        mpz_powm(params.g, params.g, r, params.p);
        ck_assert_int_eq(mpz_cmp(params.g, x), 0);

        schnorr_free(&params);
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
        schnorr_init(&params, DSA_L_1024);

        schnorr_user_keys(s, v, &params);

        schnorr_preprocess(&params, x, r);
        
        schnorr_rndmod(e, params.q);

        schnorr_sign(&params, y, r, s, e);
        mpz_mul(s, s, e);
        mpz_add(s, s, r);
        mpz_mod(s, s, params.q);
        ck_assert_int_eq(mpz_cmp(y, s) , 0);

        schnorr_free(&params);

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
        schnorr_init(&params, DSA_L_1024);

        schnorr_user_keys(s, v, &params);

        for (int i = 0; i < VERIFY_TIMES; i++) {
                schnorr_preprocess(&params, x, r);
                
                schnorr_rndmod(e, params.q);

                schnorr_sign(&params, y, r, s, e);
                
                schnorr_verify(&params, x_ver, y, v, e);
                ck_assert_int_eq(mpz_cmp(x, x_ver) , 0);
        }
        schnorr_free(&params);

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
        schnorr_init(&params, DSA_L_1024);

        schnorr_user_keys(s, v, &params);

        for (int i = 0; i < VERIFY_TIMES; i++) {
                schnorr_random(s, mpz_sizeinbase(s, 2));
                schnorr_preprocess(&params, x, r);
                
                schnorr_rndmod(e, params.q);

                schnorr_sign(&params, y, r, s, e);
                
                schnorr_verify(&params, x_ver, y, v, e);
                ck_assert_int_ne(mpz_cmp(x, x_ver) , 0);
        }
        schnorr_free(&params);

        mpz_clear(v);
        mpz_clear(s);
        mpz_clear(x);
        mpz_clear(x_ver);
        mpz_clear(r);
        mpz_clear(e);
        mpz_clear(y);
}
END_TEST

Suite *dsa_suite()
{
        Suite *s = NULL;
        TCase *tc_core = NULL;
        s = suite_create(MODULE_NAME);
        tc_core = tcase_create(CORE_NAME);

        tcase_add_test(tc_core, domain_p_1024);
        tcase_add_test(tc_core, domain_q_1024);
        tcase_add_test(tc_core, domain_p_2048);
        tcase_add_test(tc_core, domain_q_2048);
        tcase_add_test(tc_core, domain_p_3072);
        tcase_add_test(tc_core, domain_q_3072);
        tcase_add_test(tc_core, sha256_same);
        tcase_add_test(tc_core, sha256_differ);
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
        Suite *s_dsa = NULL;
        SRunner *sr = NULL;

        s_dsa = dsa_suite();
        
        sr = srunner_create(s_dsa);
        
        srunner_run_all(sr, CK_NORMAL);
        number_failed = srunner_ntests_failed(sr);
        srunner_free(sr);

        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
