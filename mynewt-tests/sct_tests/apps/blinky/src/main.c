/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <assert.h>
#include <string.h>

#include "sysinit/sysinit.h"
#include "os/os.h"
#include "bsp/bsp.h"
#include "hal/hal_gpio.h"
#ifdef ARCH_sim
#include "mcu/mcu_sim.h"
#endif

#include "Blink.h"
#include "sc_ts_mynewt.h"


static volatile int g_task1_loops;

// SETTING UP THE STATE MACHINES...

// Step 1: Allocate resources that are required by the state machines.
int g_led_pin;

// Step 2: Allocate the state machines.
Blink blinker;

// Step 3: Allocate the mempool and appropriate mem buffer for state machine events.
#define QUEUED_BLINK_EVENT_COUNT 5
struct os_mempool pool;
os_membuf_t mem_buffer[OS_MEMPOOL_SIZE(QUEUED_BLINK_EVENT_COUNT, sizeof(Blink_in_event))];

// Step 4: Allocate a timer service. And a buffer for timers.
// Use the Mynewt callout based timer service here.
// The maximum number of parallel time events is provided state machine header (BLINK_MAX_PARALLEL_TIME_EVENTS)
sc_callout_timer_t timers[BLINK_MAX_PARALLEL_TIME_EVENTS];
sc_callout_timer_service_t timer_service;

// Step 5: Define connections between state machines and timer service.
sc_ts_connection_t ts_connection =
			{
				.ts_handle = &timer_service,
				.ts_methods = &sc_callout_timer_service_methods,
				.sm_handle = &blinker,
				.sm_callback = (sc_time_event_callback_fp) blink_raiseTimeEvent
			};



/**
 * main
 *
 * The main task for the project. This function initializes packages,
 * and then blinks the BSP LED in a loop.
 *
 * @return int NOTE: this function should never return!
 */
int
main(int argc, char **argv)
{
    int rc;

#ifdef ARCH_sim
    mcu_sim_parse_args(argc, argv);
#endif

    sysinit();

    // Step 6: Initialize the resources
    g_led_pin = LED_BLINK_PIN;
    hal_gpio_init_out(g_led_pin, 0);

    // Step 7: Initialize the callout based timer service
    // Here the default event queue is used.
	sc_callout_timer_service_init(&timer_service, timers, BLINK_MAX_PARALLEL_TIME_EVENTS, os_eventq_dflt_get());

	// Step 8: Initialize the memeory pool for the state machine events
    os_mempool_init(&pool, QUEUED_BLINK_EVENT_COUNT, sizeof(Blink_in_event), &mem_buffer, "blink event pool");

    // Step 9: Initialize the state machines.
	blink_init(&blinker, &ts_connection, os_eventq_dflt_get(), &pool);
    blinkIface_set_gpio(&blinker, g_led_pin);

    // Step 10: Activate the state machines by entering them.
    blink_enter(&blinker);


    // Step 11: Use the state machines
    while (1) {
        ++g_task1_loops;


        /* switch on first blinker. */
        blinkIface_raise_on(&blinker);


        /* Wait one second */
        os_time_delay(OS_TICKS_PER_SEC/10);


        struct os_event *ev;
        do {
        		ev = os_eventq_get_no_wait(os_eventq_dflt_get());
        		if((ev != NULL) && ev->ev_cb != NULL) {
        			ev->ev_cb(ev);
        		}
        } while ( ev != NULL);

    }
    assert(0);

    return rc;
}

