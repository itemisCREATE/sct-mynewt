/*
 * sc_ts_mynewt.c
 *
 *  Created on: 30.08.2018
 *      Author: terfloth
 */

#include "sc_types.h"
#include "sc_ts_mynewt.h"

#include <stdio.h>


/*!
 * @brief A callback to be called by the OS when a timer set by the state
 * machine expires
 * @param ev Pointer to the context variable for the timer. In this case, it
 * will be or type @ref sm_timer_t.
 */
static void raise_time_event(struct os_event *ev)
{
    /* we passed the sm_timer_t in as an the context var for the callout, so
     * cast that to the right variable type.
     */
    sc_callout_timer_t* timer = (sc_callout_timer_t*)ev;

//    printf("time event raised: %d \n", (int) timer->evid);

	// TODO: ENTER SYNC SECTION

    sc_ts_connection_t *evconn = timer->connection;
    sc_eventid evid = timer->evid;

    if(timer->periodic)
    {
        // set timer again
        os_callout_reset(&timer->co, timer->os_ticks);
    }
    else
    {
    		// mark the timer for reuse
		timer->evid = NULL;
		timer->connection = NULL;
    }

	// TODO: EXIT SYNC SECTION

    // raise the time event on the state machine.
    evconn->sm_callback(evconn->sm_handle, evid);

}


static void start_timer (struct sc_ts_connection *connection,
				        const sc_eventid evid,
						const sc_integer time_ms,
						const sc_boolean periodic)
{

	// TODO: ENTER SYNC SECTION

	sc_callout_timer_service_t *this = (sc_callout_timer_service_t *) (connection->ts_handle);

	/* go through all timers ... */
	for (int i = 0; i < this->timer_count; i++) {

		/* ... until an unused is found. */
		if (this->timers[i].evid == NULL) {
			sc_callout_timer_t *timer = &(this->timers[i]);

			/* set timer properties */
			timer->evid = evid;
			timer->periodic = periodic;
			timer->connection = connection;

		    // find out how many ticks are in time_ms milliseconds
		    os_time_ms_to_ticks(time_ms, &(timer->os_ticks));
		    os_callout_init(&timer->co, this->queue, raise_time_event, (void*)timer);
		    os_callout_reset(&timer->co, timer->os_ticks);

//		    printf("scheduled timer: %d - %d cb: %d/%d\n",
//		    			(int) timer->evid,
//					(int) &(timer->co),
//					(int)timer->co.c_ev.ev_cb,
//					(int)raise_time_event);

			break;
		}
	}

	// TODO: EXIT SYNC SECTION

}


static void stop_timer (struct sc_ts_connection *connection,
				       const sc_eventid evid)
{
	// TODO: ENTER SYNC SECTION

	sc_callout_timer_service_t *this = (sc_callout_timer_service_t *) (connection->ts_handle);
	int i;

	/* go through all timers ... */
	for (i = 0; i < this->timer_count; i++) {
		// ... and find the specified one.
		if (this->timers[i].evid == evid) {
			sc_callout_timer_t *timer = &(this->timers[i]);

			// stop callout
		    os_callout_stop(&(timer->co));

		    // mark timer as unused.
			timer->evid = NULL;
			timer->connection = NULL;

			break;
		}
	}

	// TODO: ENTER SYNC SECTION
}


/*! Definition of timer service method table that maps the timer service interface functions to
 * this concrete implementation for callout based timers. */
sc_timer_service_methods_t sc_callout_timer_service_methods =
	{
		.start = start_timer,
		.stop  = stop_timer
	};


/*! Initializes a timer service with a set of timers. */
void sc_callout_timer_service_init (sc_callout_timer_service_t *this,
								    sc_callout_timer_t *timers,
									sc_integer count,
									struct os_eventq *queue)
{
	 this->timers = timers;
	 this->timer_count = count;
	 for (int i=0; i<count; i++) {
		 this->timers[i].evid = 0;
		 this->timers[i].os_ticks = 0;
		 this->timers[i].periodic = bool_false;
		 this->timers[i].connection = null;
	 }
	 this->queue = queue;
}

