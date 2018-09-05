
#include "../../src/sm/TimedMachine.h"
#include "../../src/smlib/sc_timer_service.h"
#include "../../src/smlib/sc_ts_mynewt.h"
#include "../../src/test_support.h"
#include "os/os.h"
#include "testutil/testutil.h"
#include "../test_support.h"


#define QUEUE_SIZE 5


struct os_mempool pool;
os_membuf_t mem_buffer[OS_MEMPOOL_SIZE(QUEUE_SIZE, TIMEDMACHINE_EVENT_POOL_BLOCK_SIZE)];

TimedMachine state_machine;


sc_callout_timer_t timers[TIMEDMACHINE_MAX_PARALLEL_TIME_EVENTS];
sc_callout_timer_service_t timer_service;

sc_ts_connection_t ts_connection =
	{
		.ts_handle = &timer_service,
		.ts_methods = &sc_callout_timer_service_methods,
		.sm_handle = &state_machine,
		.sm_callback = (sc_time_event_callback_fp) timedMachine_raiseTimeEvent
	};


static void setup() {

	os_mempool_init(&pool, QUEUE_SIZE, TIMEDMACHINE_EVENT_POOL_BLOCK_SIZE, &mem_buffer, "TimedMachine event pool");
	sc_callout_timer_service_init(&timer_service, timers, TIMEDMACHINE_MAX_PARALLEL_TIME_EVENTS, os_eventq_dflt_get());
	timedMachine_init(&state_machine, &ts_connection, os_eventq_dflt_get(), &pool);


}


TEST_CASE_TASK(test_timer_service_after) {

	setup();

	timedMachine_enter(&state_machine);
	TEST_ASSERT_FATAL(timedMachine_isActive(&state_machine));
	TEST_ASSERT_FATAL(timedMachine_isStateActive(&state_machine, TimedMachine_main_region_WaitForEvents));
	TEST_ASSERT_FATAL(timer_service.timers[0].evid == 0);

	timedMachineIface_raise_testAfter(&state_machine);
	TEST_ASSERT_FATAL(timedMachine_isStateActive(&state_machine, TimedMachine_main_region_WaitForAfter));
	TEST_ASSERT_FATAL(timer_service.timers[0].evid != 0);

	os_time_delay(OS_TICKS_PER_SEC/10);

	TEST_ASSERT_FATAL(timedMachine_isStateActive(&state_machine, TimedMachine_main_region_WaitForEvents));
	TEST_ASSERT_FATAL(timer_service.timers[0].evid == 0);

}

TEST_CASE_TASK(test_timer_service_every) {

	setup();

	timedMachine_enter(&state_machine);
	TEST_ASSERT(timedMachine_isActive(&state_machine));
	TEST_ASSERT(timedMachine_isStateActive(&state_machine, TimedMachine_main_region_WaitForEvents));
	TEST_ASSERT(timer_service.timers[0].evid == 0);

	timedMachineIface_raise_testEvery(&state_machine);
	TEST_ASSERT(timedMachine_isStateActive(&state_machine, TimedMachine_main_region_ProcessEvery));
	TEST_ASSERT(timer_service.timers[0].evid != 0);
	TEST_ASSERT(timedMachineIface_get_counter(&state_machine) == 0);

	os_time_delay(OS_TICKS_PER_SEC/100);
	TEST_ASSERT(timedMachine_isStateActive(&state_machine, TimedMachine_main_region_ProcessEvery));
	TEST_ASSERT(timedMachineIface_get_counter(&state_machine) == 1);

	os_time_delay(OS_TICKS_PER_SEC/10);

	TEST_ASSERT(timedMachine_isStateActive(&state_machine, TimedMachine_main_region_WaitForEvents));
	TEST_ASSERT(timer_service.timers[0].evid == 0);
	TEST_ASSERT(timedMachineIface_get_counter(&state_machine) == 5);
}


TEST_SUITE(test_timer_service) {
	test_timer_service_after();
	test_timer_service_every();
}

