#include <stdlib.h>

#include <gmp.h>
#include <check.h>

#include "sha256.h"
#include "gmptools.h"

#define MODULE_NAME "gmptools"
#define CORE_NAME "Core"

#define SHA_TEST_TIMES 50
START_TEST(sha256_same)
{
        mpz_t msg;
        mpz_init(msg);

        const size_t bitlen = 64 * CHAR_BIT * 6;
        for (int i = 0; i < SHA_TEST_TIMES; i++) {
                struct sha256_state state_msg, state_msg2;
                uint32_t bufsize = 0;
                gmpt_random(msg, bitlen);

                uint8_t *buffer = gmpt_export(msg, &bufsize);
                sha256_state_init(&state_msg);
                sha256_process_x86(&state_msg, buffer, bufsize);

                uint8_t *buffer2 = gmpt_export(msg, &bufsize);
                sha256_state_init(&state_msg2);
                sha256_process_x86(&state_msg2, buffer2, bufsize);
                for (int sti = 0; sti < SHA256_STDWCNT; sti++)
                        ck_assert_int_eq(state_msg.hash_data[sti], state_msg2.hash_data[sti]);
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
                struct sha256_state state_msg, state_msg2;
                uint32_t bufsize = 0;
                gmpt_random(msg, bitlen);

                uint8_t* buffer = gmpt_export(msg, &bufsize);
                sha256_state_init(&state_msg);
                sha256_process_x86(&state_msg, buffer, bufsize);

                mpz_sub_ui(msg, msg, 1);
                uint8_t *buffer2 = gmpt_export(msg, &bufsize);
                sha256_state_init(&state_msg2);
                sha256_process_x86(&state_msg2, buffer2, bufsize);
                for (int sti = 0; sti < SHA256_STDWCNT; sti++)
                        ck_assert_int_ne(state_msg.hash_data[sti], state_msg2.hash_data[sti]);
                free(buffer2);
                free(buffer);
        }
        mpz_clear(msg);
}
END_TEST

START_TEST(import_export)
{
        mpz_t rnd, rcopy;
        mpz_init(rnd);

        gmpt_random(rnd, 1234);
        mpz_init_set(rcopy, rnd);

        uint32_t count = 0;
        uint8_t *buf = gmpt_export(rnd, &count);
        gmpt_import(rnd, buf, count);
        ck_assert_int_eq(mpz_cmp(rnd, rcopy), 0);

        free(buf);
        mpz_clear(rcopy);
        mpz_clear(rnd);
}
END_TEST

Suite *gmpt_suite()
{
        Suite *s = NULL;
        TCase *tc_core = NULL;
        s = suite_create(MODULE_NAME);
        tc_core = tcase_create(CORE_NAME);

        tcase_add_test(tc_core, sha256_same);
        tcase_add_test(tc_core, sha256_differ);
        tcase_add_test(tc_core, import_export);
        
        suite_add_tcase(s, tc_core);
        return s;
}



int main()
{
        int number_failed = 0;       
        Suite *s_gmpt = NULL;
        SRunner *sr = NULL;

        s_gmpt = gmpt_suite();
        
        sr = srunner_create(s_gmpt);
        
        srunner_run_all(sr, CK_NORMAL);
        number_failed = srunner_ntests_failed(sr);
        srunner_free(sr);

        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
