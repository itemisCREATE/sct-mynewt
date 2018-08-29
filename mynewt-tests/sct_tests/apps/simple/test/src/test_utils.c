#include "simple_sct_tests.h"

void simple_sct_tests_init()
{
	os_mempool_init(&pool, QUEUE_SIZE, SIMPLESCT_EVENT_POOL_BLOCK_SIZE, &mem_buffer, "test event pool");
	simpleSct_init(&simpleSct, os_eventq_dflt_get(), &pool);
}

void get_and_call_next_os_event()
{
	struct os_event *ev;
	ev = os_eventq_get_no_wait(os_eventq_dflt_get());
	if((ev != NULL) && ev->ev_cb != NULL) {
		ev->ev_cb(ev);
	}
}

void get_and_call_all_os_events()
{
	for(struct os_event *ev = os_eventq_get_no_wait(os_eventq_dflt_get()); ev != NULL; ev = os_eventq_get_no_wait(os_eventq_dflt_get()))
	{
		if(ev->ev_cb != NULL) {
			ev->ev_cb(ev);
		}
	}
}
