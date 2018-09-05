
#ifndef SIMPLEMACHINE_H_
#define SIMPLEMACHINE_H_

#include "../smlib/sc_types.h"
#include "os/os_eventq.h"
#include "os/os_mempool.h"

#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'SimpleMachine'.
*/

#ifndef SIMPLEMACHINE_EVENTQUEUE_BUFFERSIZE
#define SIMPLEMACHINE_EVENTQUEUE_BUFFERSIZE 20
#endif
#ifndef SC_INVALID_EVENT_VALUE
#define SC_INVALID_EVENT_VALUE 0
#endif
/*! Define dimension of the state configuration vector for orthogonal states. */
#define SIMPLEMACHINE_MAX_ORTHOGONAL_STATES 1


/*! Define indices of states in the StateConfVector */
#define SCVI_SIMPLEMACHINE_MAIN_REGION_STATEA 0
#define SCVI_SIMPLEMACHINE_MAIN_REGION_STATEB 0
#define SCVI_SIMPLEMACHINE_MAIN_REGION_STATEC 0
#define SIMPLEMACHINE_EVENT_POOL_BLOCK_SIZE sizeof(SimpleMachine_in_event)

/*
 * Union of all possible event value types.
 */
typedef union {
	sc_integer simpleMachineIface_i_e_value;
	sc_boolean simpleMachineIface_b_e_value;
} simplemachine_event_value;

/*! Enumeration of all states */ 
typedef enum
{
	SimpleMachine_last_state,
	SimpleMachine_main_region_StateA,
	SimpleMachine_main_region_StateB,
	SimpleMachine_main_region_StateC
} SimpleMachineStates;

/*! Type definition of the data structure for the SimpleMachineIface interface scope. */
typedef struct
{
	sc_boolean e1_raised;
	sc_boolean e2_raised;
	sc_boolean i_e_raised;
	sc_integer i_e_value;
	sc_boolean b_e_raised;
	sc_boolean b_e_value;
	sc_integer i;
	sc_boolean b;
} SimpleMachineIface;




/*! 
 * Type definition of the data structure for the SimpleMachine state machine.
 * This data structure has to be allocated by the client code. 
 */
typedef struct
{
	SimpleMachineStates stateConfVector[SIMPLEMACHINE_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
	SimpleMachineIface iface;
	struct os_eventq *queue;
	struct os_mempool *pool;
} SimpleMachine;

typedef struct {
	struct os_event osev;
	simplemachine_event_value value;
} SimpleMachine_in_event;


/*! Initializes the SimpleMachine state machine data structures. Must be called before first usage.*/
extern void simpleMachine_init(SimpleMachine* handle, struct os_eventq *q, struct os_mempool *pool);

/*! Activates the state machine */
extern void simpleMachine_enter(SimpleMachine* handle);

/*! Deactivates the state machine */
extern void simpleMachine_exit(SimpleMachine* handle);

/*! Performs a 'run to completion' step. */
extern void simpleMachine_runCycle(SimpleMachine* handle);


/*! Raises the in event 'e1' that is defined in the default interface scope. */ 
extern void simpleMachineIface_raise_e1(SimpleMachine* handle);

/*! Raises the in event 'e2' that is defined in the default interface scope. */ 
extern void simpleMachineIface_raise_e2(SimpleMachine* handle);

/*! Raises the in event 'i_e' that is defined in the default interface scope. */ 
extern void simpleMachineIface_raise_i_e(SimpleMachine* handle, sc_integer value);

/*! Raises the in event 'b_e' that is defined in the default interface scope. */ 
extern void simpleMachineIface_raise_b_e(SimpleMachine* handle, sc_boolean value);

/*! Gets the value of the variable 'i' that is defined in the default interface scope. */ 
extern sc_integer simpleMachineIface_get_i(const SimpleMachine* handle);
/*! Sets the value of the variable 'i' that is defined in the default interface scope. */ 
extern void simpleMachineIface_set_i(SimpleMachine* handle, sc_integer value);
/*! Gets the value of the variable 'b' that is defined in the default interface scope. */ 
extern sc_boolean simpleMachineIface_get_b(const SimpleMachine* handle);
/*! Sets the value of the variable 'b' that is defined in the default interface scope. */ 
extern void simpleMachineIface_set_b(SimpleMachine* handle, sc_boolean value);

/*!
 * Checks whether the state machine is active (until 2.4.1 this method was used for states).
 * A state machine is active if it was entered. It is inactive if it has not been entered at all or if it has been exited.
 */
extern sc_boolean simpleMachine_isActive(const SimpleMachine* handle);

/*!
 * Checks if all active states are final. 
 * If there are no active states then the state machine is considered being inactive. In this case this method returns false.
 */
extern sc_boolean simpleMachine_isFinal(const SimpleMachine* handle);

/*! Checks if the specified state is active (until 2.4.1 the used method for states was called isActive()). */
extern sc_boolean simpleMachine_isStateActive(const SimpleMachine* handle, SimpleMachineStates state);

/*! Callback function of event e1 for the MyNewt event queue */
extern void simpleMachineIface_process_e1(struct os_event * osev);

/*! Callback function of event e2 for the MyNewt event queue */
extern void simpleMachineIface_process_e2(struct os_event * osev);

/*! Callback function of event i_e for the MyNewt event queue */
extern void simpleMachineIface_process_i_e(struct os_event * osev);

/*! Callback function of event b_e for the MyNewt event queue */
extern void simpleMachineIface_process_b_e(struct os_event * osev);


#ifdef __cplusplus
}
#endif 

#endif /* SIMPLEMACHINE_H_ */
