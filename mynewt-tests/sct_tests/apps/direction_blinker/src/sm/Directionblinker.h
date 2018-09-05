
#ifndef DIRECTIONBLINKER_H_
#define DIRECTIONBLINKER_H_

#include "../smlib/sc_types.h"
#include "Blinklight.h"
#include "../smlib/sc_timer_service.h"
#include "os/os_eventq.h"
#include "os/os_mempool.h"

#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'directionblinker'.
*/

#ifndef DIRECTIONBLINKER_EVENTQUEUE_BUFFERSIZE
#define DIRECTIONBLINKER_EVENTQUEUE_BUFFERSIZE 20
#endif
#ifndef SC_INVALID_EVENT_VALUE
#define SC_INVALID_EVENT_VALUE 0
#endif
/*! Define dimension of the state configuration vector for orthogonal states. */
#define DIRECTIONBLINKER_MAX_ORTHOGONAL_STATES 1

/*! Define maximum number of time events that can be active at once */
#define DIRECTIONBLINKER_MAX_PARALLEL_TIME_EVENTS 1

/*! Define indices of states in the StateConfVector */
#define SCVI_DIRECTIONBLINKER_MAIN_REGION_OFF 0
#define SCVI_DIRECTIONBLINKER_MAIN_REGION_RIGHT 0
#define SCVI_DIRECTIONBLINKER_MAIN_REGION_LEFT 0
#define DIRECTIONBLINKER_EVENT_POOL_BLOCK_SIZE sizeof(Directionblinker_in_event)


/*! Enumeration of all states */ 
typedef enum
{
	Directionblinker_last_state,
	Directionblinker_main_region_Off,
	Directionblinker_main_region_Right,
	Directionblinker_main_region_Left
} DirectionblinkerStates;



/*! Type definition of the data structure for the DirectionblinkerIface interface scope. */
typedef struct
{
	sc_boolean left_raised;
	Blinklight * leftBL;
	sc_boolean right_raised;
	Blinklight * rightBL;
	sc_boolean off_raised;
} DirectionblinkerIface;



/*! Type definition of the data structure for the DirectionblinkerTimeEvents interface scope. */
typedef struct
{
	sc_boolean directionblinker_main_region_Right_tev0_raised;
	sc_boolean directionblinker_main_region_Left_tev0_raised;
} DirectionblinkerTimeEvents;




/*! 
 * Type definition of the data structure for the Directionblinker state machine.
 * This data structure has to be allocated by the client code. 
 */
typedef struct
{
	DirectionblinkerStates stateConfVector[DIRECTIONBLINKER_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
	DirectionblinkerIface iface;
	DirectionblinkerTimeEvents timeEvents;
	sc_ts_connection_t *ts_conn;
	struct os_eventq *queue;
	struct os_mempool *pool;
} Directionblinker;

typedef struct {
	struct os_event osev;
} Directionblinker_in_event;


/*! Initializes the Directionblinker state machine data structures. Must be called before first usage.*/
extern void directionblinker_init(Directionblinker* handle, sc_ts_connection_t *tsc, struct os_eventq *q, struct os_mempool *pool);

/*! Activates the state machine */
extern void directionblinker_enter(Directionblinker* handle);

/*! Deactivates the state machine */
extern void directionblinker_exit(Directionblinker* handle);

/*! Performs a 'run to completion' step. */
extern void directionblinker_runCycle(Directionblinker* handle);

/*! Raises a time event. */
extern void directionblinker_raiseTimeEvent(Directionblinker* handle, sc_eventid evid);

/*! Raises the in event 'left' that is defined in the default interface scope. */ 
extern void directionblinkerIface_raise_left(Directionblinker* handle);

/*! Gets the value of the variable 'leftBL' that is defined in the default interface scope. */ 
extern Blinklight * directionblinkerIface_get_leftBL(const Directionblinker* handle);
/*! Sets the value of the variable 'leftBL' that is defined in the default interface scope. */ 
extern void directionblinkerIface_set_leftBL(Directionblinker* handle, Blinklight * value);
/*! Raises the in event 'right' that is defined in the default interface scope. */ 
extern void directionblinkerIface_raise_right(Directionblinker* handle);

/*! Gets the value of the variable 'rightBL' that is defined in the default interface scope. */ 
extern Blinklight * directionblinkerIface_get_rightBL(const Directionblinker* handle);
/*! Sets the value of the variable 'rightBL' that is defined in the default interface scope. */ 
extern void directionblinkerIface_set_rightBL(Directionblinker* handle, Blinklight * value);
/*! Raises the in event 'off' that is defined in the default interface scope. */ 
extern void directionblinkerIface_raise_off(Directionblinker* handle);


/*!
 * Checks whether the state machine is active (until 2.4.1 this method was used for states).
 * A state machine is active if it was entered. It is inactive if it has not been entered at all or if it has been exited.
 */
extern sc_boolean directionblinker_isActive(const Directionblinker* handle);

/*!
 * Checks if all active states are final. 
 * If there are no active states then the state machine is considered being inactive. In this case this method returns false.
 */
extern sc_boolean directionblinker_isFinal(const Directionblinker* handle);

/*! Checks if the specified state is active (until 2.4.1 the used method for states was called isActive()). */
extern sc_boolean directionblinker_isStateActive(const Directionblinker* handle, DirectionblinkerStates state);

/*! Callback function of event left for the MyNewt event queue */
extern void directionblinkerIface_process_left(struct os_event * osev);

/*! Callback function of event right for the MyNewt event queue */
extern void directionblinkerIface_process_right(struct os_event * osev);

/*! Callback function of event off for the MyNewt event queue */
extern void directionblinkerIface_process_off(struct os_event * osev);


#ifdef __cplusplus
}
#endif 

#endif /* DIRECTIONBLINKER_H_ */
