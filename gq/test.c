#include <stdlib.h>

#include <gmp.h>
#include <check.h>

#include "ffs.h"

#define MODULE_NAME "FFS"
#define CORE_NAME "Core"

START_TEST(dummy_test)
{
        
}
END_TEST

Suite *ffs_suite()
{
        Suite *s = NULL;
        TCase *tc_core = NULL;
        s = suite_create(MODULE_NAME);
        tc_core = tcase_create(CORE_NAME);

        tcase_add_test(tc_core, dummy_test);
        
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
