
#ifndef BLINK_H_
#define BLINK_H_

#include "sc_types.h"
#include "../../../repos/apache-mynewt-core/hw/hal/include/hal/hal_gpio.h"
#include "sc_timer_service.h"
#include "os/os_eventq.h"
#include "os/os_mempool.h"

#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'blink'.
*/

#ifndef BLINK_EVENTQUEUE_BUFFERSIZE
#define BLINK_EVENTQUEUE_BUFFERSIZE 20
#endif
#ifndef SC_INVALID_EVENT_VALUE
#define SC_INVALID_EVENT_VALUE 0
#endif
/*! Define dimension of the state configuration vector for orthogonal states. */
#define BLINK_MAX_ORTHOGONAL_STATES 1

/*! Define maximum number of time events that can be active at once */
#define BLINK_MAX_PARALLEL_TIME_EVENTS 1

/*! Define indices of states in the StateConfVector */
#define SCVI_BLINK_MR_OFF 0
#define SCVI_BLINK_MR_BLINKING 0
#define SCVI_BLINK_MR_BLINKING_R_LIGHT 0
#define SCVI_BLINK_MR_BLINKING_R_DARK 0
#define BLINK_EVENT_POOL_BLOCK_SIZE sizeof(Blink_in_event)


/*! Enumeration of all states */ 
typedef enum
{
	Blink_last_state,
	Blink_mr_Off,
	Blink_mr_Blinking,
	Blink_mr_Blinking_r_Light,
	Blink_mr_Blinking_r_Dark
} BlinkStates;





/*! Type definition of the data structure for the BlinkIface interface scope. */
typedef struct
{
	sc_boolean on_raised;
	sc_boolean off_raised;
	int32_t gpio;
	uint32_t period;
} BlinkIface;



/*! Type definition of the data structure for the BlinkTimeEvents interface scope. */
typedef struct
{
	sc_boolean blink_mr_Blinking_r_Light_tev0_raised;
	sc_boolean blink_mr_Blinking_r_Dark_tev0_raised;
} BlinkTimeEvents;




/*! 
 * Type definition of the data structure for the Blink state machine.
 * This data structure has to be allocated by the client code. 
 */
typedef struct
{
	BlinkStates stateConfVector[BLINK_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
	BlinkIface iface;
	BlinkTimeEvents timeEvents;
	sc_ts_connection_t *ts_conn;
	struct os_eventq *queue;
	struct os_mempool *pool;
} Blink;

typedef struct {
	struct os_event osev;
} Blink_in_event;


/*! Initializes the Blink state machine data structures. Must be called before first usage.*/
extern void blink_init(Blink* handle, sc_ts_connection_t *tsc, struct os_eventq *q, struct os_mempool *pool);

/*! Activates the state machine */
extern void blink_enter(Blink* handle);

/*! Deactivates the state machine */
extern void blink_exit(Blink* handle);

/*! Performs a 'run to completion' step. */
extern void blink_runCycle(Blink* handle);

/*! Raises a time event. */
extern void blink_raiseTimeEvent(Blink* handle, sc_eventid evid);

/*! Raises the in event 'on' that is defined in the default interface scope. */ 
extern void blinkIface_raise_on(Blink* handle);

/*! Raises the in event 'off' that is defined in the default interface scope. */ 
extern void blinkIface_raise_off(Blink* handle);

/*! Gets the value of the variable 'gpio' that is defined in the default interface scope. */ 
extern int32_t blinkIface_get_gpio(const Blink* handle);
/*! Sets the value of the variable 'gpio' that is defined in the default interface scope. */ 
extern void blinkIface_set_gpio(Blink* handle, int32_t value);
/*! Gets the value of the variable 'period' that is defined in the default interface scope. */ 
extern uint32_t blinkIface_get_period(const Blink* handle);
/*! Sets the value of the variable 'period' that is defined in the default interface scope. */ 
extern void blinkIface_set_period(Blink* handle, uint32_t value);

/*!
 * Checks whether the state machine is active (until 2.4.1 this method was used for states).
 * A state machine is active if it was entered. It is inactive if it has not been entered at all or if it has been exited.
 */
extern sc_boolean blink_isActive(const Blink* handle);

/*!
 * Checks if all active states are final. 
 * If there are no active states then the state machine is considered being inactive. In this case this method returns false.
 */
extern sc_boolean blink_isFinal(const Blink* handle);

/*! Checks if the specified state is active (until 2.4.1 the used method for states was called isActive()). */
extern sc_boolean blink_isStateActive(const Blink* handle, BlinkStates state);

/*! Callback function of event on for the MyNewt event queue */
extern void blinkIface_process_on(struct os_event * osev);

/*! Callback function of event off for the MyNewt event queue */
extern void blinkIface_process_off(struct os_event * osev);


#ifdef __cplusplus
}
#endif 

#endif /* BLINK_H_ */
