
#ifndef TIMEDMACHINE_H_
#define TIMEDMACHINE_H_

#include "../smlib/sc_types.h"
#include "../smlib/sc_timer_service.h"
#include "os/os_eventq.h"
#include "os/os_mempool.h"

#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'TimedMachine'.
*/

#ifndef TIMEDMACHINE_EVENTQUEUE_BUFFERSIZE
#define TIMEDMACHINE_EVENTQUEUE_BUFFERSIZE 20
#endif
#ifndef SC_INVALID_EVENT_VALUE
#define SC_INVALID_EVENT_VALUE 0
#endif
/*! Define dimension of the state configuration vector for orthogonal states. */
#define TIMEDMACHINE_MAX_ORTHOGONAL_STATES 1

/*! Define maximum number of time events that can be active at once */
#define TIMEDMACHINE_MAX_PARALLEL_TIME_EVENTS 1

/*! Define indices of states in the StateConfVector */
#define SCVI_TIMEDMACHINE_MAIN_REGION_WAITFOREVENTS 0
#define SCVI_TIMEDMACHINE_MAIN_REGION_WAITFORAFTER 0
#define SCVI_TIMEDMACHINE_MAIN_REGION_PROCESSEVERY 0
#define TIMEDMACHINE_EVENT_POOL_BLOCK_SIZE sizeof(TimedMachine_in_event)


/*! Enumeration of all states */ 
typedef enum
{
	TimedMachine_last_state,
	TimedMachine_main_region_WaitForEvents,
	TimedMachine_main_region_WaitForAfter,
	TimedMachine_main_region_ProcessEvery
} TimedMachineStates;

/*! Type definition of the data structure for the TimedMachineIface interface scope. */
typedef struct
{
	sc_boolean testAfter_raised;
	sc_boolean testEvery_raised;
	sc_integer counter;
} TimedMachineIface;



/*! Type definition of the data structure for the TimedMachineTimeEvents interface scope. */
typedef struct
{
	sc_boolean timedMachine_main_region_WaitForAfter_tev0_raised;
	sc_boolean timedMachine_main_region_ProcessEvery_tev0_raised;
} TimedMachineTimeEvents;




/*! 
 * Type definition of the data structure for the TimedMachine state machine.
 * This data structure has to be allocated by the client code. 
 */
typedef struct
{
	TimedMachineStates stateConfVector[TIMEDMACHINE_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
	TimedMachineIface iface;
	TimedMachineTimeEvents timeEvents;
	sc_ts_connection_t *ts_conn;
	struct os_eventq *queue;
	struct os_mempool *pool;
} TimedMachine;

typedef struct {
	struct os_event osev;
} TimedMachine_in_event;


/*! Initializes the TimedMachine state machine data structures. Must be called before first usage.*/
extern void timedMachine_init(TimedMachine* handle, sc_ts_connection_t *tsc, struct os_eventq *q, struct os_mempool *pool);

/*! Activates the state machine */
extern void timedMachine_enter(TimedMachine* handle);

/*! Deactivates the state machine */
extern void timedMachine_exit(TimedMachine* handle);

/*! Performs a 'run to completion' step. */
extern void timedMachine_runCycle(TimedMachine* handle);

/*! Raises a time event. */
extern void timedMachine_raiseTimeEvent(TimedMachine* handle, sc_eventid evid);

/*! Raises the in event 'testAfter' that is defined in the default interface scope. */ 
extern void timedMachineIface_raise_testAfter(TimedMachine* handle);

/*! Raises the in event 'testEvery' that is defined in the default interface scope. */ 
extern void timedMachineIface_raise_testEvery(TimedMachine* handle);

/*! Gets the value of the variable 'counter' that is defined in the default interface scope. */ 
extern sc_integer timedMachineIface_get_counter(const TimedMachine* handle);
/*! Sets the value of the variable 'counter' that is defined in the default interface scope. */ 
extern void timedMachineIface_set_counter(TimedMachine* handle, sc_integer value);

/*!
 * Checks whether the state machine is active (until 2.4.1 this method was used for states).
 * A state machine is active if it was entered. It is inactive if it has not been entered at all or if it has been exited.
 */
extern sc_boolean timedMachine_isActive(const TimedMachine* handle);

/*!
 * Checks if all active states are final. 
 * If there are no active states then the state machine is considered being inactive. In this case this method returns false.
 */
extern sc_boolean timedMachine_isFinal(const TimedMachine* handle);

/*! Checks if the specified state is active (until 2.4.1 the used method for states was called isActive()). */
extern sc_boolean timedMachine_isStateActive(const TimedMachine* handle, TimedMachineStates state);

/*! Callback function of event testAfter for the MyNewt event queue */
extern void timedMachineIface_process_testAfter(struct os_event * osev);

/*! Callback function of event testEvery for the MyNewt event queue */
extern void timedMachineIface_process_testEvery(struct os_event * osev);


#ifdef __cplusplus
}
#endif 

#endif /* TIMEDMACHINE_H_ */
