/*
 * sc_ts_mynewt.h
 *
 *  Created on: 30.08.2018
 *      Author: sam anderson, axel terfloth
 */

#ifndef SC_TS_MYNEWT_H_
#define SC_TS_MYNEWT_H_

#include "../../../test/src/smlib/sc_timer_service.h"
#include "os/os.h"


#ifdef __cplusplus
extern "C" {
#endif

/*! file/ Implementation of YAKINDU SCT state machine timer service for Apache Mynewt using its os_callout API.
 */


/*!
 * @brief A struct to keep track of the additional info we will need to react
 * to a timer set by the state machine
 */
typedef struct
{
    struct os_callout co;				/**< The callout object being used by Newt to trigger the timer */
    sc_eventid evid;						/**< The event ID sent by the state machine on timer setup */
    uint32_t os_ticks;					/**< Number of os ticks. */
    sc_boolean periodic;					/**< Is this a repeating or one-shot timer */
    sc_ts_connection_t *connection;		/**< Pointer to the connection that receives the time event  */
} sc_callout_timer_t;


/*!
 * @brief A struct to keep track of the additional info we will need to react
 * to a timer set by the state machine
 */
typedef struct sc_callout_timer_service {
	sc_integer timer_count;		/**< The number of timers. */
	sc_callout_timer_t *timers;	/**< The array of timers. */
	struct os_eventq *queue;		/**< the event queue where the time events go. */
} sc_callout_timer_service_t;


/*! Initializes a timer service. */
extern void sc_callout_timer_service_init( sc_callout_timer_service_t *this,
										  sc_callout_timer_t *timers,
										  sc_integer timer_count,
										  struct os_eventq *queue);


/*! definition of timer service methods for this callout based timer implemetation. */
extern sc_timer_service_methods_t sc_callout_timer_service_methods;


#ifdef __cplusplus
}
#endif

#endif /* SC_TS_MYNEWT_H_ */
