
#include "Directionblinker.h"
#include "../smlib/sc_types.h"

#include <stdlib.h>
#include <string.h>
/*! \file Implementation of the state machine 'directionblinker'
*/

/* prototypes of all internal functions */
static sc_boolean check_main_region_Off_tr0_tr0(const Directionblinker* handle);
static sc_boolean check_main_region_Off_tr1_tr1(const Directionblinker* handle);
static sc_boolean check_main_region_Right_tr0_tr0(const Directionblinker* handle);
static sc_boolean check_main_region_Right_tr1_tr1(const Directionblinker* handle);
static sc_boolean check_main_region_Left_tr0_tr0(const Directionblinker* handle);
static sc_boolean check_main_region_Left_tr1_tr1(const Directionblinker* handle);
static void effect_main_region_Off_tr0(Directionblinker* handle);
static void effect_main_region_Off_tr1(Directionblinker* handle);
static void effect_main_region_Right_tr0(Directionblinker* handle);
static void effect_main_region_Right_tr1(Directionblinker* handle);
static void effect_main_region_Left_tr0(Directionblinker* handle);
static void effect_main_region_Left_tr1(Directionblinker* handle);
static void enact_main_region_Off(Directionblinker* handle);
static void enact_main_region_Right(Directionblinker* handle);
static void enact_main_region_Left(Directionblinker* handle);
static void exact_main_region_Right(Directionblinker* handle);
static void exact_main_region_Left(Directionblinker* handle);
static void enseq_main_region_Off_default(Directionblinker* handle);
static void enseq_main_region_Right_default(Directionblinker* handle);
static void enseq_main_region_Left_default(Directionblinker* handle);
static void enseq_main_region_default(Directionblinker* handle);
static void exseq_main_region_Off(Directionblinker* handle);
static void exseq_main_region_Right(Directionblinker* handle);
static void exseq_main_region_Left(Directionblinker* handle);
static void exseq_main_region(Directionblinker* handle);
static void react_main_region_Off(Directionblinker* handle);
static void react_main_region_Right(Directionblinker* handle);
static void react_main_region_Left(Directionblinker* handle);
static void react_main_region__entry_Default(Directionblinker* handle);
static void clearInEvents(Directionblinker* handle);
static void clearOutEvents(Directionblinker* handle);


void directionblinker_init(Directionblinker* handle, sc_ts_connection_t *tsc, struct os_eventq *q, struct os_mempool *pool)
{
	handle->ts_conn = tsc;
	handle->queue = q;
	handle->pool = pool;
		sc_integer i;
	
		for (i = 0; i < DIRECTIONBLINKER_MAX_ORTHOGONAL_STATES; ++i)
		{
			handle->stateConfVector[i] = Directionblinker_last_state;
		}
		
		
		handle->stateConfVectorPosition = 0;
	
		clearInEvents(handle);
		clearOutEvents(handle);
	
}

void directionblinker_enter(Directionblinker* handle)
{
	/* Default enter sequence for statechart directionblinker */
	enseq_main_region_default(handle);
}

void directionblinker_runCycle(Directionblinker* handle)
{
	clearOutEvents(handle);
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < DIRECTIONBLINKER_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++)
		{
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition])
		{
		case Directionblinker_main_region_Off:
		{
			react_main_region_Off(handle);
			break;
		}
		case Directionblinker_main_region_Right:
		{
			react_main_region_Right(handle);
			break;
		}
		case Directionblinker_main_region_Left:
		{
			react_main_region_Left(handle);
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents(handle);
}

void directionblinker_exit(Directionblinker* handle)
{
	/* Default exit sequence for statechart directionblinker */
	exseq_main_region(handle);
}

sc_boolean directionblinker_isActive(const Directionblinker* handle)
{
	sc_boolean result = bool_false;
	int i;
	
	for(i = 0; i < DIRECTIONBLINKER_MAX_ORTHOGONAL_STATES; i++)
	{
		result = result || handle->stateConfVector[i] != Directionblinker_last_state;
	}
	
	return result;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean directionblinker_isFinal(const Directionblinker* handle)
{
   return bool_false;
}

void directionblinker_raiseTimeEvent(Directionblinker* handle, sc_eventid evid)
{
	if ( ((sc_intptr_t)evid) >= ((sc_intptr_t)&(handle->timeEvents))
		&&  ((sc_intptr_t)evid) < ((sc_intptr_t)&(handle->timeEvents)) + sizeof(DirectionblinkerTimeEvents))
		{
		*(sc_boolean*)evid = bool_true;
		
		directionblinker_runCycle(handle);
	}		
}

sc_boolean directionblinker_isStateActive(const Directionblinker* handle, DirectionblinkerStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case Directionblinker_main_region_Off :
			result = (sc_boolean) (handle->stateConfVector[SCVI_DIRECTIONBLINKER_MAIN_REGION_OFF] == Directionblinker_main_region_Off
			);
			break;
		case Directionblinker_main_region_Right :
			result = (sc_boolean) (handle->stateConfVector[SCVI_DIRECTIONBLINKER_MAIN_REGION_RIGHT] == Directionblinker_main_region_Right
			);
			break;
		case Directionblinker_main_region_Left :
			result = (sc_boolean) (handle->stateConfVector[SCVI_DIRECTIONBLINKER_MAIN_REGION_LEFT] == Directionblinker_main_region_Left
			);
			break;
		default:
			result = bool_false;
			break;
	}
	return result;
}

static void clearInEvents(Directionblinker* handle)
{
	handle->iface.left_raised = bool_false;
	handle->iface.right_raised = bool_false;
	handle->iface.off_raised = bool_false;
	handle->timeEvents.directionblinker_main_region_Right_tev0_raised = bool_false;
	handle->timeEvents.directionblinker_main_region_Left_tev0_raised = bool_false;
}

static void clearOutEvents(Directionblinker* handle)
{
}

void directionblinkerIface_raise_left(Directionblinker* handle)
{
	Directionblinker_in_event *e = os_memblock_get(handle->pool);
	e->osev.ev_queued = 0;
	e->osev.ev_arg = (void*) handle;
	e->osev.ev_cb = directionblinkerIface_process_left;
	e->osev.ev_next.stqe_next = 0;
	os_eventq_put(handle->queue, (struct os_event*) e);
}
void directionblinkerIface_raise_right(Directionblinker* handle)
{
	Directionblinker_in_event *e = os_memblock_get(handle->pool);
	e->osev.ev_queued = 0;
	e->osev.ev_arg = (void*) handle;
	e->osev.ev_cb = directionblinkerIface_process_right;
	e->osev.ev_next.stqe_next = 0;
	os_eventq_put(handle->queue, (struct os_event*) e);
}
void directionblinkerIface_raise_off(Directionblinker* handle)
{
	Directionblinker_in_event *e = os_memblock_get(handle->pool);
	e->osev.ev_queued = 0;
	e->osev.ev_arg = (void*) handle;
	e->osev.ev_cb = directionblinkerIface_process_off;
	e->osev.ev_next.stqe_next = 0;
	os_eventq_put(handle->queue, (struct os_event*) e);
}


Blinklight * directionblinkerIface_get_leftBL(const Directionblinker* handle)
{
	return handle->iface.leftBL;
}
void directionblinkerIface_set_leftBL(Directionblinker* handle, Blinklight * value)
{
	handle->iface.leftBL = value;
}
Blinklight * directionblinkerIface_get_rightBL(const Directionblinker* handle)
{
	return handle->iface.rightBL;
}
void directionblinkerIface_set_rightBL(Directionblinker* handle, Blinklight * value)
{
	handle->iface.rightBL = value;
}

/* implementations of all internal functions */

static sc_boolean check_main_region_Off_tr0_tr0(const Directionblinker* handle)
{
	return handle->iface.right_raised;
}

static sc_boolean check_main_region_Off_tr1_tr1(const Directionblinker* handle)
{
	return handle->iface.left_raised;
}

static sc_boolean check_main_region_Right_tr0_tr0(const Directionblinker* handle)
{
	return handle->iface.left_raised;
}

static sc_boolean check_main_region_Right_tr1_tr1(const Directionblinker* handle)
{
	return (handle->iface.off_raised || handle->timeEvents.directionblinker_main_region_Right_tev0_raised) ? bool_true : bool_false;
}

static sc_boolean check_main_region_Left_tr0_tr0(const Directionblinker* handle)
{
	return (handle->iface.off_raised || handle->timeEvents.directionblinker_main_region_Left_tev0_raised) ? bool_true : bool_false;
}

static sc_boolean check_main_region_Left_tr1_tr1(const Directionblinker* handle)
{
	return handle->iface.right_raised;
}

static void effect_main_region_Off_tr0(Directionblinker* handle)
{
	exseq_main_region_Off(handle);
	enseq_main_region_Right_default(handle);
}

static void effect_main_region_Off_tr1(Directionblinker* handle)
{
	exseq_main_region_Off(handle);
	enseq_main_region_Left_default(handle);
}

static void effect_main_region_Right_tr0(Directionblinker* handle)
{
	exseq_main_region_Right(handle);
	enseq_main_region_Left_default(handle);
}

static void effect_main_region_Right_tr1(Directionblinker* handle)
{
	exseq_main_region_Right(handle);
	enseq_main_region_Off_default(handle);
}

static void effect_main_region_Left_tr0(Directionblinker* handle)
{
	exseq_main_region_Left(handle);
	enseq_main_region_Off_default(handle);
}

static void effect_main_region_Left_tr1(Directionblinker* handle)
{
	exseq_main_region_Left(handle);
	enseq_main_region_Right_default(handle);
}

/* Entry action for state 'Off'. */
static void enact_main_region_Off(Directionblinker* handle)
{
	/* Entry action for state 'Off'. */
	blinklightIface_raise_off(handle->iface.leftBL);
	blinklightIface_raise_off(handle->iface.rightBL);
}

/* Entry action for state 'Right'. */
static void enact_main_region_Right(Directionblinker* handle)
{
	/* Entry action for state 'Right'. */
	handle->ts_conn->ts_methods->start(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.directionblinker_main_region_Right_tev0_raised), 
										5 * 1000,
										bool_false);
	blinklightIface_raise_on(handle->iface.rightBL);
}

/* Entry action for state 'Left'. */
static void enact_main_region_Left(Directionblinker* handle)
{
	/* Entry action for state 'Left'. */
	handle->ts_conn->ts_methods->start(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.directionblinker_main_region_Left_tev0_raised), 
										5 * 1000,
										bool_false);
	blinklightIface_raise_on(handle->iface.leftBL);
}

/* Exit action for state 'Right'. */
static void exact_main_region_Right(Directionblinker* handle)
{
	/* Exit action for state 'Right'. */
	handle->ts_conn->ts_methods->stop(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.directionblinker_main_region_Right_tev0_raised));
	blinklightIface_raise_off(handle->iface.rightBL);
}

/* Exit action for state 'Left'. */
static void exact_main_region_Left(Directionblinker* handle)
{
	/* Exit action for state 'Left'. */
	handle->ts_conn->ts_methods->stop(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.directionblinker_main_region_Left_tev0_raised));
	blinklightIface_raise_off(handle->iface.leftBL);
}

/* 'default' enter sequence for state Off */
static void enseq_main_region_Off_default(Directionblinker* handle)
{
	/* 'default' enter sequence for state Off */
	enact_main_region_Off(handle);
	handle->stateConfVector[0] = Directionblinker_main_region_Off;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Right */
static void enseq_main_region_Right_default(Directionblinker* handle)
{
	/* 'default' enter sequence for state Right */
	enact_main_region_Right(handle);
	handle->stateConfVector[0] = Directionblinker_main_region_Right;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Left */
static void enseq_main_region_Left_default(Directionblinker* handle)
{
	/* 'default' enter sequence for state Left */
	enact_main_region_Left(handle);
	handle->stateConfVector[0] = Directionblinker_main_region_Left;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
static void enseq_main_region_default(Directionblinker* handle)
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default(handle);
}

/* Default exit sequence for state Off */
static void exseq_main_region_Off(Directionblinker* handle)
{
	/* Default exit sequence for state Off */
	handle->stateConfVector[0] = Directionblinker_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state Right */
static void exseq_main_region_Right(Directionblinker* handle)
{
	/* Default exit sequence for state Right */
	handle->stateConfVector[0] = Directionblinker_last_state;
	handle->stateConfVectorPosition = 0;
	exact_main_region_Right(handle);
}

/* Default exit sequence for state Left */
static void exseq_main_region_Left(Directionblinker* handle)
{
	/* Default exit sequence for state Left */
	handle->stateConfVector[0] = Directionblinker_last_state;
	handle->stateConfVectorPosition = 0;
	exact_main_region_Left(handle);
}

/* Default exit sequence for region main region */
static void exseq_main_region(Directionblinker* handle)
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of directionblinker.main_region) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case Directionblinker_main_region_Off :
		{
			exseq_main_region_Off(handle);
			break;
		}
		case Directionblinker_main_region_Right :
		{
			exseq_main_region_Right(handle);
			break;
		}
		case Directionblinker_main_region_Left :
		{
			exseq_main_region_Left(handle);
			break;
		}
		default: break;
	}
}

/* The reactions of state Off. */
static void react_main_region_Off(Directionblinker* handle)
{
	/* The reactions of state Off. */
	if (check_main_region_Off_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_Off_tr0(handle);
	}  else
	{
		if (check_main_region_Off_tr1_tr1(handle) == bool_true)
		{ 
			effect_main_region_Off_tr1(handle);
		} 
	}
}

/* The reactions of state Right. */
static void react_main_region_Right(Directionblinker* handle)
{
	/* The reactions of state Right. */
	if (check_main_region_Right_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_Right_tr0(handle);
	}  else
	{
		if (check_main_region_Right_tr1_tr1(handle) == bool_true)
		{ 
			effect_main_region_Right_tr1(handle);
		} 
	}
}

/* The reactions of state Left. */
static void react_main_region_Left(Directionblinker* handle)
{
	/* The reactions of state Left. */
	if (check_main_region_Left_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_Left_tr0(handle);
	}  else
	{
		if (check_main_region_Left_tr1_tr1(handle) == bool_true)
		{ 
			effect_main_region_Left_tr1(handle);
		} 
	}
}

/* Default react sequence for initial entry  */
static void react_main_region__entry_Default(Directionblinker* handle)
{
	/* Default react sequence for initial entry  */
	enseq_main_region_Off_default(handle);
}


void directionblinkerIface_process_left(struct os_event * osev)
{
	Directionblinker * handle = osev->ev_arg;
	handle->iface.left_raised = bool_true;
	
	directionblinker_runCycle(handle);
	os_memblock_put(handle->pool, osev);
}

void directionblinkerIface_process_right(struct os_event * osev)
{
	Directionblinker * handle = osev->ev_arg;
	handle->iface.right_raised = bool_true;
	
	directionblinker_runCycle(handle);
	os_memblock_put(handle->pool, osev);
}

void directionblinkerIface_process_off(struct os_event * osev)
{
	Directionblinker * handle = osev->ev_arg;
	handle->iface.off_raised = bool_true;
	
	directionblinker_runCycle(handle);
	os_memblock_put(handle->pool, osev);
}

