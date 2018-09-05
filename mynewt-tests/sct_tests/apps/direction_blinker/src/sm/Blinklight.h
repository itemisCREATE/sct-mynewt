
#ifndef BLINKLIGHT_H_
#define BLINKLIGHT_H_

#include "../smlib/sc_types.h"
#include "../../../../repos/apache-mynewt-core/hw/hal/include/hal/hal_gpio.h"
#include "../smlib/sc_timer_service.h"
#include "os/os_eventq.h"
#include "os/os_mempool.h"

#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'blinklight'.
*/

#ifndef BLINKLIGHT_EVENTQUEUE_BUFFERSIZE
#define BLINKLIGHT_EVENTQUEUE_BUFFERSIZE 20
#endif
#ifndef SC_INVALID_EVENT_VALUE
#define SC_INVALID_EVENT_VALUE 0
#endif
/*! Define dimension of the state configuration vector for orthogonal states. */
#define BLINKLIGHT_MAX_ORTHOGONAL_STATES 1

/*! Define maximum number of time events that can be active at once */
#define BLINKLIGHT_MAX_PARALLEL_TIME_EVENTS 1

/*! Define indices of states in the StateConfVector */
#define SCVI_BLINKLIGHT_MR_OFF 0
#define SCVI_BLINKLIGHT_MR_BLINKING 0
#define SCVI_BLINKLIGHT_MR_BLINKING_R_LIGHT 0
#define SCVI_BLINKLIGHT_MR_BLINKING_R_DARK 0
#define BLINKLIGHT_EVENT_POOL_BLOCK_SIZE sizeof(Blinklight_in_event)


/*! Enumeration of all states */ 
typedef enum
{
	Blinklight_last_state,
	Blinklight_mr_Off,
	Blinklight_mr_Blinking,
	Blinklight_mr_Blinking_r_Light,
	Blinklight_mr_Blinking_r_Dark
} BlinklightStates;





/*! Type definition of the data structure for the BlinklightIface interface scope. */
typedef struct
{
	sc_boolean on_raised;
	sc_boolean off_raised;
	int32_t gpio;
	uint32_t period;
} BlinklightIface;



/*! Type definition of the data structure for the BlinklightTimeEvents interface scope. */
typedef struct
{
	sc_boolean blinklight_mr_Blinking_r_Light_tev0_raised;
	sc_boolean blinklight_mr_Blinking_r_Dark_tev0_raised;
} BlinklightTimeEvents;




/*! 
 * Type definition of the data structure for the Blinklight state machine.
 * This data structure has to be allocated by the client code. 
 */
typedef struct
{
	BlinklightStates stateConfVector[BLINKLIGHT_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
	BlinklightIface iface;
	BlinklightTimeEvents timeEvents;
	sc_ts_connection_t *ts_conn;
	struct os_eventq *queue;
	struct os_mempool *pool;
} Blinklight;

typedef struct {
	struct os_event osev;
} Blinklight_in_event;


/*! Initializes the Blinklight state machine data structures. Must be called before first usage.*/
extern void blinklight_init(Blinklight* handle, sc_ts_connection_t *tsc, struct os_eventq *q, struct os_mempool *pool);

/*! Activates the state machine */
extern void blinklight_enter(Blinklight* handle);

/*! Deactivates the state machine */
extern void blinklight_exit(Blinklight* handle);

/*! Performs a 'run to completion' step. */
extern void blinklight_runCycle(Blinklight* handle);

/*! Raises a time event. */
extern void blinklight_raiseTimeEvent(Blinklight* handle, sc_eventid evid);

/*! Raises the in event 'on' that is defined in the default interface scope. */ 
extern void blinklightIface_raise_on(Blinklight* handle);

/*! Raises the in event 'off' that is defined in the default interface scope. */ 
extern void blinklightIface_raise_off(Blinklight* handle);

/*! Gets the value of the variable 'gpio' that is defined in the default interface scope. */ 
extern int32_t blinklightIface_get_gpio(const Blinklight* handle);
/*! Sets the value of the variable 'gpio' that is defined in the default interface scope. */ 
extern void blinklightIface_set_gpio(Blinklight* handle, int32_t value);
/*! Gets the value of the variable 'period' that is defined in the default interface scope. */ 
extern uint32_t blinklightIface_get_period(const Blinklight* handle);
/*! Sets the value of the variable 'period' that is defined in the default interface scope. */ 
extern void blinklightIface_set_period(Blinklight* handle, uint32_t value);

/*!
 * Checks whether the state machine is active (until 2.4.1 this method was used for states).
 * A state machine is active if it was entered. It is inactive if it has not been entered at all or if it has been exited.
 */
extern sc_boolean blinklight_isActive(const Blinklight* handle);

/*!
 * Checks if all active states are final. 
 * If there are no active states then the state machine is considered being inactive. In this case this method returns false.
 */
extern sc_boolean blinklight_isFinal(const Blinklight* handle);

/*! Checks if the specified state is active (until 2.4.1 the used method for states was called isActive()). */
extern sc_boolean blinklight_isStateActive(const Blinklight* handle, BlinklightStates state);

/*! Callback function of event on for the MyNewt event queue */
extern void blinklightIface_process_on(struct os_event * osev);

/*! Callback function of event off for the MyNewt event queue */
extern void blinklightIface_process_off(struct os_event * osev);


#ifdef __cplusplus
}
#endif 

#endif /* BLINKLIGHT_H_ */
