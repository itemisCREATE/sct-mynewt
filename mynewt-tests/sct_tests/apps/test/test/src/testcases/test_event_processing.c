#include "testutil/testutil.h"
#include "../test_support.h"
#include "../../src/sm/SimpleMachine.h"

#define QUEUE_SIZE 5


struct os_mempool pool;
os_membuf_t mem_buffer[OS_MEMPOOL_SIZE(QUEUE_SIZE, SIMPLEMACHINE_EVENT_POOL_BLOCK_SIZE)];
SimpleMachine simpleMachine;


void simple_sct_tests_init()
{
	os_mempool_init(&pool, QUEUE_SIZE, SIMPLEMACHINE_EVENT_POOL_BLOCK_SIZE, &mem_buffer, "test event pool");
	simpleMachine_init(&simpleMachine, os_eventq_dflt_get(), &pool);
}


TEST_CASE(test_change_state) {
	simple_sct_tests_init();
	simpleMachine_enter(&simpleMachine);
	TEST_ASSERT_FATAL(simpleMachine_isActive(&simpleMachine));
	TEST_ASSERT_FATAL(simpleMachine_isStateActive(&simpleMachine, SimpleMachine_main_region_StateA));
	simpleMachineIface_raise_e1(&simpleMachine);
	TEST_ASSERT(simpleMachine_isStateActive(&simpleMachine, SimpleMachine_main_region_StateA));
	get_and_call_next_os_event();
	TEST_ASSERT(simpleMachine_isStateActive(&simpleMachine, SimpleMachine_main_region_StateB));
}


TEST_CASE(test_change_state_two_stage) {
	simple_sct_tests_init();
	simpleMachine_enter(&simpleMachine);
	TEST_ASSERT_FATAL(simpleMachine_isActive(&simpleMachine));
	TEST_ASSERT_FATAL(simpleMachine_isStateActive(&simpleMachine, SimpleMachine_main_region_StateA));

	simpleMachineIface_raise_e2(&simpleMachine);
	get_and_call_next_os_event();
	// e2 should only make the state switch from B to C.
	TEST_ASSERT(simpleMachine_isStateActive(&simpleMachine, SimpleMachine_main_region_StateA));

	simpleMachineIface_raise_e1(&simpleMachine);
	get_and_call_next_os_event();
	TEST_ASSERT(simpleMachine_isStateActive(&simpleMachine, SimpleMachine_main_region_StateB));

	simpleMachineIface_raise_e2(&simpleMachine);
	get_and_call_next_os_event();
	TEST_ASSERT(simpleMachine_isStateActive(&simpleMachine, SimpleMachine_main_region_StateC));
}


TEST_CASE(test_change_state_event_queue_order) {
	simple_sct_tests_init();
	simpleMachine_enter(&simpleMachine);
	TEST_ASSERT_FATAL(simpleMachine_isActive(&simpleMachine));
	TEST_ASSERT_FATAL(simpleMachine_isStateActive(&simpleMachine, SimpleMachine_main_region_StateA));

	simpleMachineIface_raise_e1(&simpleMachine);
	simpleMachineIface_raise_e2(&simpleMachine);
	get_and_call_next_os_event();
	get_and_call_next_os_event();

	TEST_ASSERT(simpleMachine_isStateActive(&simpleMachine, SimpleMachine_main_region_StateC));
}


TEST_CASE(test_change_state_event_queue_order2) {
	simple_sct_tests_init();
	simpleMachine_enter(&simpleMachine);
	TEST_ASSERT_FATAL(simpleMachine_isActive(&simpleMachine));
	TEST_ASSERT_FATAL(simpleMachine_isStateActive(&simpleMachine, SimpleMachine_main_region_StateA));

	simpleMachineIface_raise_e2(&simpleMachine);
	simpleMachineIface_raise_e1(&simpleMachine);
	get_and_call_next_os_event();
	get_and_call_next_os_event();

	TEST_ASSERT(simpleMachine_isStateActive(&simpleMachine, SimpleMachine_main_region_StateB));
}


TEST_CASE(test_event_payload)
{
	simple_sct_tests_init();
	simpleMachine_enter(&simpleMachine);
	TEST_ASSERT_FATAL(simpleMachine_isActive(&simpleMachine));
	TEST_ASSERT_FATAL(simpleMachine_isStateActive(&simpleMachine, SimpleMachine_main_region_StateA));

	simpleMachineIface_raise_e1(&simpleMachine);
	simpleMachineIface_raise_e2(&simpleMachine);
	simpleMachineIface_raise_b_e(&simpleMachine, true);
	simpleMachineIface_raise_i_e(&simpleMachine, 42);

	get_and_call_all_os_events();

	TEST_ASSERT_FATAL(simpleMachine_isStateActive(&simpleMachine, SimpleMachine_main_region_StateC));
	TEST_ASSERT(simpleMachineIface_get_b(&simpleMachine) == true);
	TEST_ASSERT(simpleMachineIface_get_i(&simpleMachine) == 42);
}



TEST_SUITE(test_event_processing) {
	test_change_state();
	test_change_state_two_stage();
	test_change_state_event_queue_order();
	test_change_state_event_queue_order2();
	test_event_payload();
}
