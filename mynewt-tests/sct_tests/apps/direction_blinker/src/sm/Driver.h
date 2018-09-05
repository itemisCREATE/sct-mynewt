
#ifndef DRIVER_H_
#define DRIVER_H_

#include "../smlib/sc_types.h"
#include "Directionblinker.h"
#include "../smlib/sc_timer_service.h"
#include "os/os_eventq.h"
#include "os/os_mempool.h"

#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'driver'.
*/

#ifndef DRIVER_EVENTQUEUE_BUFFERSIZE
#define DRIVER_EVENTQUEUE_BUFFERSIZE 20
#endif
#ifndef SC_INVALID_EVENT_VALUE
#define SC_INVALID_EVENT_VALUE 0
#endif
/*! Define dimension of the state configuration vector for orthogonal states. */
#define DRIVER_MAX_ORTHOGONAL_STATES 1

/*! Define maximum number of time events that can be active at once */
#define DRIVER_MAX_PARALLEL_TIME_EVENTS 1

/*! Define indices of states in the StateConfVector */
#define SCVI_DRIVER_MAIN_REGION_STEP1 0
#define SCVI_DRIVER_MAIN_REGION_STEP2 0
#define SCVI_DRIVER_MAIN_REGION_STEP3 0
#define DRIVER_EVENT_POOL_BLOCK_SIZE sizeof(Driver_in_event)


/*! Enumeration of all states */ 
typedef enum
{
	Driver_last_state,
	Driver_main_region_Step1,
	Driver_main_region_Step2,
	Driver_main_region_Step3
} DriverStates;



/*! Type definition of the data structure for the DriverIface interface scope. */
typedef struct
{
	Directionblinker * blinker;
} DriverIface;



/*! Type definition of the data structure for the DriverTimeEvents interface scope. */
typedef struct
{
	sc_boolean driver_main_region_Step1_tev0_raised;
	sc_boolean driver_main_region_Step2_tev0_raised;
	sc_boolean driver_main_region_Step3_tev0_raised;
} DriverTimeEvents;




/*! 
 * Type definition of the data structure for the Driver state machine.
 * This data structure has to be allocated by the client code. 
 */
typedef struct
{
	DriverStates stateConfVector[DRIVER_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
	DriverIface iface;
	DriverTimeEvents timeEvents;
	sc_ts_connection_t *ts_conn;
	struct os_eventq *queue;
	struct os_mempool *pool;
} Driver;

typedef struct {
	struct os_event osev;
} Driver_in_event;


/*! Initializes the Driver state machine data structures. Must be called before first usage.*/
extern void driver_init(Driver* handle, sc_ts_connection_t *tsc, struct os_eventq *q, struct os_mempool *pool);

/*! Activates the state machine */
extern void driver_enter(Driver* handle);

/*! Deactivates the state machine */
extern void driver_exit(Driver* handle);

/*! Performs a 'run to completion' step. */
extern void driver_runCycle(Driver* handle);

/*! Raises a time event. */
extern void driver_raiseTimeEvent(Driver* handle, sc_eventid evid);

/*! Gets the value of the variable 'blinker' that is defined in the default interface scope. */ 
extern Directionblinker * driverIface_get_blinker(const Driver* handle);
/*! Sets the value of the variable 'blinker' that is defined in the default interface scope. */ 
extern void driverIface_set_blinker(Driver* handle, Directionblinker * value);

/*!
 * Checks whether the state machine is active (until 2.4.1 this method was used for states).
 * A state machine is active if it was entered. It is inactive if it has not been entered at all or if it has been exited.
 */
extern sc_boolean driver_isActive(const Driver* handle);

/*!
 * Checks if all active states are final. 
 * If there are no active states then the state machine is considered being inactive. In this case this method returns false.
 */
extern sc_boolean driver_isFinal(const Driver* handle);

/*! Checks if the specified state is active (until 2.4.1 the used method for states was called isActive()). */
extern sc_boolean driver_isStateActive(const Driver* handle, DriverStates state);


#ifdef __cplusplus
}
#endif 

#endif /* DRIVER_H_ */
