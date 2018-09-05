#include "../../../test/test/src/test_support.h"

#include "os/os.h"
#include "os/os_eventq.h"


void get_and_call_next_os_event()
{
	printf("get_and_call_next_os_event...\n");

	struct os_event *ev;
	ev = os_eventq_get_no_wait(os_eventq_dflt_get());
	if(ev != NULL)
	{
		if (ev->ev_cb != NULL)
		{
			printf("execute %d\n", (int)ev);
			ev->ev_cb(ev);
		}
		else {
			printf("polled %d\n", (int)ev);
		}
	}
}


struct os_eventq *queues[1];

void get_and_call_next_os_event_timeout(os_time_t timeout)
{
	printf("get_and_call_next_os_event_timeout ...\n");

	queues[0] = os_eventq_dflt_get();
	struct os_event *ev = os_eventq_poll(queues, 1, timeout);

	if(ev != NULL)
	{
		if (ev->ev_cb != NULL)
		{
			printf("execute %d\n", (int)ev);
			ev->ev_cb(ev);
		}
		else {
			printf("polled %d\n", (int)ev);
		}
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
