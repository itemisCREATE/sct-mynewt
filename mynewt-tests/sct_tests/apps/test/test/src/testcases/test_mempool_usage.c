/*
 * test_mempool_usage.c
 *
 *  Created on: 05.09.2018
 *      Author: terfloth
 */


#include "testutil/testutil.h"
#include "../test_support.h"
#include "../../src/sm/SimpleMachine.h"


#define QUEUE_SIZE 5

struct os_mempool test_pool;
os_membuf_t test_mem_buffer[OS_MEMPOOL_SIZE(QUEUE_SIZE, SIMPLEMACHINE_EVENT_POOL_BLOCK_SIZE)];
SimpleMachine simpleMachine;


/*!
 * Checks if the state machine properly gets events from the event pool when these are raised and if they
 * are properly put back if they are processed.
 */
TEST_CASE(test_mempool_usage_event_recycling) {

	os_mempool_init(&test_pool, 5, SIMPLEMACHINE_EVENT_POOL_BLOCK_SIZE, &test_mem_buffer, "test event pool");
	simpleMachine_init(&simpleMachine, os_eventq_dflt_get(), &test_pool);
	TEST_ASSERT(test_pool.mp_num_free == 5);

	simpleMachine_enter(&simpleMachine);
	TEST_ASSERT(test_pool.mp_num_free == 5);

	simpleMachineIface_raise_e1(&simpleMachine);
	TEST_ASSERT(test_pool.mp_num_free == 4);

	get_and_call_next_os_event();
	TEST_ASSERT(test_pool.mp_num_free == 5);

	simpleMachineIface_raise_e1(&simpleMachine);
	simpleMachineIface_raise_e1(&simpleMachine);
	simpleMachineIface_raise_e1(&simpleMachine);
	TEST_ASSERT(test_pool.mp_num_free == 2);

	get_and_call_all_os_events();
	TEST_ASSERT(test_pool.mp_num_free == 5);
}


/*!
 * Checks the state machine pool usage by processing a large number of events on the state machine.
 */
TEST_CASE(test_mempool_usage_iterations) {

	os_mempool_init(&test_pool, 5, SIMPLEMACHINE_EVENT_POOL_BLOCK_SIZE, &test_mem_buffer, "test event pool");
	simpleMachine_init(&simpleMachine, os_eventq_dflt_get(), &test_pool);

	simpleMachine_enter(&simpleMachine);

	for (int i = 0; i < 10000; ++i) {
		simpleMachineIface_raise_e1(&simpleMachine);
		simpleMachineIface_raise_e2(&simpleMachine);
		simpleMachineIface_raise_i_e(&simpleMachine, i);
		simpleMachineIface_raise_b_e(&simpleMachine, !simpleMachineIface_get_b(&simpleMachine));
		get_and_call_all_os_events();
	}

	TEST_ASSERT(test_pool.mp_num_free == 5);
	TEST_ASSERT(test_pool.mp_min_free == 1);
}


TEST_SUITE(test_mempool_usage) {
	test_mempool_usage_event_recycling();
	test_mempool_usage_iterations();
}

