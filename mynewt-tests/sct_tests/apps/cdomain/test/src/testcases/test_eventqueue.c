#include "cdomain_sct_tests.h"


TEST_CASE(test_eventqueue) {
	cdomain_tests_init();
	cDomain_enter(&machine);
	TEST_ASSERT_FATAL(cDomain_isActive(&machine));
	TEST_ASSERT_FATAL(cDomain_isStateActive(&machine, CDomain_main_region_Wait));

	cDomainIface_raise_start(&machine);

	TEST_ASSERT_FATAL(cDomain_isStateActive(&machine, CDomain_main_region_Wait));

	get_and_call_all_os_events();

	TEST_ASSERT_FATAL(cDomain_isFinal(&machine));

	bool array_correct = true;

	for(int i = 0; i < ARR_SIZE; i++) {
		array_correct = array_correct && (arr[i] == i);
	}

	TEST_ASSERT(array_correct);
}
