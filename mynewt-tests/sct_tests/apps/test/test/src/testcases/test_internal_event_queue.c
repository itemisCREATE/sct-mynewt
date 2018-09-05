#include "testutil/testutil.h"
#include "../test_support.h"
#include "../../src/sm/InternalEventMachine.h"
#include "../../src/sct_import_header.h"

#define QUEUE_SIZE 5

struct os_mempool pool;
os_membuf_t mem_buffer[OS_MEMPOOL_SIZE(QUEUE_SIZE, INTERNALEVENTMACHINE_EVENT_POOL_BLOCK_SIZE)];
InternalEventMachine state_machine;
int arr[ARR_SIZE];


void setup()
{
	os_mempool_init(&pool, QUEUE_SIZE, INTERNALEVENTMACHINE_EVENT_POOL_BLOCK_SIZE, &mem_buffer, "test event pool");
	internalEventMachine_init(&state_machine, os_eventq_dflt_get(), &pool);
}



TEST_CASE(test_internal_eventqueue_processing) {
	setup();
	internalEventMachine_enter(&state_machine);
	TEST_ASSERT(internalEventMachine_isActive(&state_machine));
	TEST_ASSERT(internalEventMachine_isStateActive(&state_machine, InternalEventMachine_main_region_Wait));

	internalEventMachineIface_raise_start(&state_machine);

	TEST_ASSERT(internalEventMachine_isStateActive(&state_machine, InternalEventMachine_main_region_Wait));

	get_and_call_all_os_events();

	TEST_ASSERT(internalEventMachine_isFinal(&state_machine));

	bool array_correct = true;

	for(int i = 0; i < ARR_SIZE; i++) {
		array_correct = array_correct && (arr[i] == i);
	}

	TEST_ASSERT(array_correct);
}


TEST_SUITE(test_internal_event_queue) {

	test_internal_eventqueue_processing();
}
