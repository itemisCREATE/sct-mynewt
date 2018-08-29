#include "cdomain_sct_tests.h"

TEST_CASE(test_basic) {
	cdomain_tests_init();
	cDomain_enter(&machine);
	TEST_ASSERT_FATAL(cDomain_isActive(&machine));
	TEST_ASSERT_FATAL(cDomain_isStateActive(&machine, CDomain_main_region_Wait));
}
