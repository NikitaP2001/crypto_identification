#include <stdlib.h>

#include <gmp.h>
#include <check.h>

#include "gq.h"

#define MODULE_NAME "GQ"
#define CORE_NAME "Core"

START_TEST(init_wrong)
{
        struct gq_params params = {0};
        ck_assert(!gq_init(&params, 123));
}
END_TEST

START_TEST(init_valid)
{
        mpz_t n;
        mpz_init(n);
        struct gq_params params = {0};

        ck_assert(gq_init(&params, GQ_NLEN_3072));

        ck_assert(gmpt_isprime(params.p));

        ck_assert_int_eq(mpz_sizeinbase(params.p, 2), GQ_NLEN_3072 / 2);
        ck_assert(gmpt_isprime(params.q));
        ck_assert_int_eq(mpz_sizeinbase(params.q, 2), GQ_NLEN_3072 / 2);

        mpz_mul(n, params.q, params.p);
        ck_assert_int_eq(mpz_cmp(n, params.n), 0);
        ck_assert_int_eq(mpz_sizeinbase(params.n, 2), GQ_NLEN_3072);

        gq_free(&params);
        mpz_clear(n);
}
END_TEST

Suite *ffs_suite()
{
        Suite *s = NULL;
        TCase *tc_core = NULL;
        s = suite_create(MODULE_NAME);
        tc_core = tcase_create(CORE_NAME);

        tcase_add_test(tc_core, init_wrong);
        tcase_add_test(tc_core, init_valid);
        
        suite_add_tcase(s, tc_core);
        return s;
}


int main()
{
        int number_failed = 0;       
        Suite *s_ffs = NULL;
        SRunner *sr = NULL;

        s_ffs = ffs_suite();
        
        sr = srunner_create(s_ffs);
        
        srunner_run_all(sr, CK_NORMAL);
        number_failed = srunner_ntests_failed(sr);
        srunner_free(sr);

        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
