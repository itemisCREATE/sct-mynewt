
#ifndef INTERNALEVENTMACHINE_H_
#define INTERNALEVENTMACHINE_H_

#include "../smlib/sc_types.h"
#include "../sct_import_header.h"
#include "os/os_eventq.h"
#include "os/os_mempool.h"

#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'InternalEventMachine'.
*/

#ifndef INTERNALEVENTMACHINE_EVENTQUEUE_BUFFERSIZE
#define INTERNALEVENTMACHINE_EVENTQUEUE_BUFFERSIZE 20
#endif
#ifndef SC_INVALID_EVENT_VALUE
#define SC_INVALID_EVENT_VALUE 0
#endif
/*! Define dimension of the state configuration vector for orthogonal states. */
#define INTERNALEVENTMACHINE_MAX_ORTHOGONAL_STATES 1


/*! Define indices of states in the StateConfVector */
#define SCVI_INTERNALEVENTMACHINE_MAIN_REGION_WAIT 0
#define SCVI_INTERNALEVENTMACHINE_MAIN_REGION_FILLARRAY 0
#define SCVI_INTERNALEVENTMACHINE_MAIN_REGION_FILLARRAY__REGION0_CHECK 0
#define SCVI_INTERNALEVENTMACHINE_MAIN_REGION_FILLARRAY__REGION0_SET 0
#define SCVI_INTERNALEVENTMACHINE_MAIN_REGION__FINAL_ 0
#define INTERNALEVENTMACHINE_EVENT_POOL_BLOCK_SIZE sizeof(InternalEventMachine_in_event)


/*
 * Enum of event names in the statechart.
 */
typedef enum  {
	internaleventmachine_invalid_event = SC_INVALID_EVENT_VALUE,
	internalEventMachineInternal_arr_full,
	internalEventMachineInternal_next
} internaleventmachine_event_name;

/*
 * Struct that represents a single event.
 */
typedef struct {
	internaleventmachine_event_name name;
} internaleventmachine_internal_event;

/*
 * Queue that holds the raised events.
 */
typedef struct internaleventmachine_eventqueue_s {
	internaleventmachine_internal_event events[INTERNALEVENTMACHINE_EVENTQUEUE_BUFFERSIZE];
	sc_integer pop_index;
	sc_integer push_index;
	sc_integer size;
} internaleventmachine_eventqueue;
/*! Enumeration of all states */ 
typedef enum
{
	InternalEventMachine_last_state,
	InternalEventMachine_main_region_Wait,
	InternalEventMachine_main_region_FillArray,
	InternalEventMachine_main_region_FillArray__region0_Check,
	InternalEventMachine_main_region_FillArray__region0_Set,
	InternalEventMachine_main_region__final_
} InternalEventMachineStates;



/*! Type definition of the data structure for the InternalEventMachineIface interface scope. */
typedef struct
{
	sc_boolean start_raised;
} InternalEventMachineIface;



/*! Type definition of the data structure for the InternalEventMachineInternal interface scope. */
typedef struct
{
	sc_boolean arr_full_raised;
	sc_boolean next_raised;
	integer i;
	sc_boolean b;
} InternalEventMachineInternal;




/*! 
 * Type definition of the data structure for the InternalEventMachine state machine.
 * This data structure has to be allocated by the client code. 
 */
typedef struct
{
	InternalEventMachineStates stateConfVector[INTERNALEVENTMACHINE_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
	InternalEventMachineIface iface;
	InternalEventMachineInternal internal;
	internaleventmachine_eventqueue internal_event_queue;
	struct os_eventq *queue;
	struct os_mempool *pool;
} InternalEventMachine;

typedef struct {
	struct os_event osev;
} InternalEventMachine_in_event;


/*! Initializes the InternalEventMachine state machine data structures. Must be called before first usage.*/
extern void internalEventMachine_init(InternalEventMachine* handle, struct os_eventq *q, struct os_mempool *pool);

/*! Activates the state machine */
extern void internalEventMachine_enter(InternalEventMachine* handle);

/*! Deactivates the state machine */
extern void internalEventMachine_exit(InternalEventMachine* handle);

/*! Performs a 'run to completion' step. */
extern void internalEventMachine_runCycle(InternalEventMachine* handle);


/*! Raises the in event 'start' that is defined in the default interface scope. */ 
extern void internalEventMachineIface_raise_start(InternalEventMachine* handle);


/*!
 * Checks whether the state machine is active (until 2.4.1 this method was used for states).
 * A state machine is active if it was entered. It is inactive if it has not been entered at all or if it has been exited.
 */
extern sc_boolean internalEventMachine_isActive(const InternalEventMachine* handle);

/*!
 * Checks if all active states are final. 
 * If there are no active states then the state machine is considered being inactive. In this case this method returns false.
 */
extern sc_boolean internalEventMachine_isFinal(const InternalEventMachine* handle);

/*! Checks if the specified state is active (until 2.4.1 the used method for states was called isActive()). */
extern sc_boolean internalEventMachine_isStateActive(const InternalEventMachine* handle, InternalEventMachineStates state);

/*! Callback function of event start for the MyNewt event queue */
extern void internalEventMachineIface_process_start(struct os_event * osev);


#ifdef __cplusplus
}
#endif 

#endif /* INTERNALEVENTMACHINE_H_ */
