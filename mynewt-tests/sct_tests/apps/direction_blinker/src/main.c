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

#include "sm/Blinklight.h"
#include "sm/Directionblinker.h"
#include "sm/Driver.h"
#include "smlib/sc_ts_mynewt.h"


/*!
 *
 * \file This example application implements a direction blinker that consists of four interaction state machines.
 * All run on the same event queue and share the same timer service.
 *
 */


static volatile int g_task1_loops;
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))


// SETTING UP THE STATE MACHINES...

// Step 1: Allocate resources that are required by the state machines.
// Use two gpios for LEDs in this example
int g_led_pin_1;
int g_led_pin_2;

// Step 2: Allocate the state machines.
// use four instances of three types here ...
Blinklight light_left;
Blinklight light_right;
Directionblinker blinker;
Driver driver;

// Step 3: Allocate the mempool and appropriate mem buffer for state machine events.
#define EVENT_POOL_SIZE 10
#define EVENT_SIZE MAX(sizeof(Blinklight_in_event), sizeof(Directionblinker_in_event))
struct os_mempool event_pool;
os_membuf_t event_buffer[OS_MEMPOOL_SIZE(EVENT_POOL_SIZE, EVENT_SIZE)];


// Step 4: Allocate a timer service. And a buffer for timers.
// Use the Mynewt callout based timer service here.
// The maximum number of parallel time events is provided state machine header (XX_MAX_PARALLEL_TIME_EVENTS)
// Totally allocate the sum of these values of all state machines.
#define TIMER_COUNT (BLINKLIGHT_MAX_PARALLEL_TIME_EVENTS * 2 + DIRECTIONBLINKER_MAX_PARALLEL_TIME_EVENTS + DRIVER_MAX_PARALLEL_TIME_EVENTS )
sc_callout_timer_t timers[TIMER_COUNT];
sc_callout_timer_service_t timer_service;

// Step 5: Define connections between state machines and timer service.
// There is one connection instance for each state machine.
// Here the connections are statically allocated.
sc_ts_connection_t ts_connections[] =
		{
			{
				.ts_handle = &timer_service,
				.ts_methods = &sc_callout_timer_service_methods,
				.sm_handle = &light_left,
				.sm_callback = (sc_time_event_callback_fp) blinklight_raiseTimeEvent
			},
			{
				.ts_handle = &timer_service,
				.ts_methods = &sc_callout_timer_service_methods,
				.sm_handle = &light_right,
				.sm_callback = (sc_time_event_callback_fp) blinklight_raiseTimeEvent
			},
			{
				.ts_handle = &timer_service,
				.ts_methods = &sc_callout_timer_service_methods,
				.sm_handle = &blinker,
				.sm_callback = (sc_time_event_callback_fp) directionblinker_raiseTimeEvent
			},
			{
				.ts_handle = &timer_service,
				.ts_methods = &sc_callout_timer_service_methods,
				.sm_handle = &driver,
				.sm_callback = (sc_time_event_callback_fp) driver_raiseTimeEvent
			}
		};



/**
 * main
 *
 * The main task for the project. This function initializes packages,
 * and then blinks the BSP LED in a loop.
 *
 * @return int NOTE: this function should never return!
 */
int main(int argc, char **argv)
{
    int rc;

#ifdef ARCH_sim
    mcu_sim_parse_args(argc, argv);
#endif

    sysinit();

    // Step 6: Initialize the resources
    g_led_pin_1 = LED_BLINK_PIN;
    g_led_pin_2 = 0x2;

    hal_gpio_init_out(g_led_pin_1, 0);
    hal_gpio_init_out(g_led_pin_2, 0);


    // Step 7: Initialize the callout based timer service
    // Here the default event queue is used.
	sc_callout_timer_service_init(&timer_service, timers, TIMER_COUNT, os_eventq_dflt_get());


	// Step 8: Initialize the memory pool for the state machine events
    os_mempool_init(&event_pool, EVENT_POOL_SIZE, EVENT_SIZE, &event_buffer, "blinker event pool");


    // Step 9: Initialize the state machines.
	blinklight_init(&light_left, &ts_connections[0], os_eventq_dflt_get(), &event_pool);
    blinklightIface_set_gpio(&light_left, g_led_pin_1);

	blinklight_init(&light_right, &ts_connections[1], os_eventq_dflt_get(), &event_pool);
    blinklightIface_set_gpio(&light_right, g_led_pin_2);

    directionblinker_init(&blinker, &ts_connections[2], os_eventq_dflt_get(), &event_pool);
    directionblinkerIface_set_leftBL(&blinker, &light_left);
    directionblinkerIface_set_rightBL(&blinker, &light_right);

    driver_init(&driver, &ts_connections[3], os_eventq_dflt_get(), &event_pool);
    driverIface_set_blinker(&driver, &blinker);


    // Step 10: Activate the state machines by entering them.
    blinklight_enter(&light_left);
    blinklight_enter(&light_right);
    directionblinker_enter(&blinker);
    driver_enter(&driver);


    // Step 11: run the statemachines just by processing the events on the event queue.
    while (1) {
        ++g_task1_loops;

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

