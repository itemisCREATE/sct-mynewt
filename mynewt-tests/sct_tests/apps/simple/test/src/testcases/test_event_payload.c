#include "simple_sct_tests.h"

TEST_CASE(test_event_payload)
{
	simple_sct_tests_init();
	simpleSct_enter(&simpleSct);
	TEST_ASSERT_FATAL(simpleSct_isActive(&simpleSct));
	TEST_ASSERT_FATAL(simpleSct_isStateActive(&simpleSct, SimpleSct_main_region_StateA));

	simpleSctIface_raise_e1(&simpleSct);
	simpleSctIface_raise_e2(&simpleSct);
	simpleSctIface_raise_b_e(&simpleSct, true);
	simpleSctIface_raise_i_e(&simpleSct, 42);

	get_and_call_all_os_events();

	TEST_ASSERT_FATAL(simpleSct_isStateActive(&simpleSct, SimpleSct_main_region_StateC));
	TEST_ASSERT(simpleSctIface_get_b(&simpleSct) == true);
	TEST_ASSERT(simpleSctIface_get_i(&simpleSct) == 42);
}
