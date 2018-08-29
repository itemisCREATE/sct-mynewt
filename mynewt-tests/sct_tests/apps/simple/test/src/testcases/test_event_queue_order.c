#include "simple_sct_tests.h"

TEST_CASE(test_change_state_event_queue_order) {
	simple_sct_tests_init();
	simpleSct_enter(&simpleSct);
	TEST_ASSERT_FATAL(simpleSct_isActive(&simpleSct));
	TEST_ASSERT_FATAL(simpleSct_isStateActive(&simpleSct, SimpleSct_main_region_StateA));

	simpleSctIface_raise_e1(&simpleSct);
	simpleSctIface_raise_e2(&simpleSct);
	get_and_call_next_os_event();
	get_and_call_next_os_event();

	TEST_ASSERT(simpleSct_isStateActive(&simpleSct, SimpleSct_main_region_StateC));
}
