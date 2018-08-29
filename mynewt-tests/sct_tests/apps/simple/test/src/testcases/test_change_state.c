#include "simple_sct_tests.h"

TEST_CASE(test_change_state) {
	simple_sct_tests_init();
	simpleSct_enter(&simpleSct);
	TEST_ASSERT_FATAL(simpleSct_isActive(&simpleSct));
	TEST_ASSERT_FATAL(simpleSct_isStateActive(&simpleSct, SimpleSct_main_region_StateA));
	simpleSctIface_raise_e1(&simpleSct);
	TEST_ASSERT(simpleSct_isStateActive(&simpleSct, SimpleSct_main_region_StateA));
	get_and_call_next_os_event();
	TEST_ASSERT(simpleSct_isStateActive(&simpleSct, SimpleSct_main_region_StateB));
}
