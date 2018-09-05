
#include "TimedMachine.h"
#include "../smlib/sc_types.h"

#include <stdlib.h>
#include <string.h>
/*! \file Implementation of the state machine 'TimedMachine'
*/

/* prototypes of all internal functions */
static sc_boolean check_main_region_WaitForEvents_tr0_tr0(const TimedMachine* handle);
static sc_boolean check_main_region_WaitForEvents_tr1_tr1(const TimedMachine* handle);
static sc_boolean check_main_region_WaitForAfter_tr0_tr0(const TimedMachine* handle);
static sc_boolean check_main_region_ProcessEvery_lr0_lr0(const TimedMachine* handle);
static sc_boolean check_main_region_ProcessEvery_tr0_tr0(const TimedMachine* handle);
static void effect_main_region_WaitForEvents_tr0(TimedMachine* handle);
static void effect_main_region_WaitForEvents_tr1(TimedMachine* handle);
static void effect_main_region_WaitForAfter_tr0(TimedMachine* handle);
static void effect_main_region_ProcessEvery_lr0_lr0(TimedMachine* handle);
static void effect_main_region_ProcessEvery_tr0(TimedMachine* handle);
static void enact_main_region_WaitForAfter(TimedMachine* handle);
static void enact_main_region_ProcessEvery(TimedMachine* handle);
static void exact_main_region_WaitForAfter(TimedMachine* handle);
static void exact_main_region_ProcessEvery(TimedMachine* handle);
static void enseq_main_region_WaitForEvents_default(TimedMachine* handle);
static void enseq_main_region_WaitForAfter_default(TimedMachine* handle);
static void enseq_main_region_ProcessEvery_default(TimedMachine* handle);
static void enseq_main_region_default(TimedMachine* handle);
static void exseq_main_region_WaitForEvents(TimedMachine* handle);
static void exseq_main_region_WaitForAfter(TimedMachine* handle);
static void exseq_main_region_ProcessEvery(TimedMachine* handle);
static void exseq_main_region(TimedMachine* handle);
static void react_main_region_WaitForEvents(TimedMachine* handle);
static void react_main_region_WaitForAfter(TimedMachine* handle);
static void react_main_region_ProcessEvery(TimedMachine* handle);
static void react_main_region__entry_Default(TimedMachine* handle);
static void clearInEvents(TimedMachine* handle);
static void clearOutEvents(TimedMachine* handle);


void timedMachine_init(TimedMachine* handle, sc_ts_connection_t *tsc, struct os_eventq *q, struct os_mempool *pool)
{
	handle->ts_conn = tsc;
	handle->queue = q;
	handle->pool = pool;
		sc_integer i;
	
		for (i = 0; i < TIMEDMACHINE_MAX_ORTHOGONAL_STATES; ++i)
		{
			handle->stateConfVector[i] = TimedMachine_last_state;
		}
		
		
		handle->stateConfVectorPosition = 0;
	
		clearInEvents(handle);
		clearOutEvents(handle);
	
		/* Default init sequence for statechart TimedMachine */
		handle->iface.counter = 0;
}

void timedMachine_enter(TimedMachine* handle)
{
	/* Default enter sequence for statechart TimedMachine */
	enseq_main_region_default(handle);
}

void timedMachine_runCycle(TimedMachine* handle)
{
	clearOutEvents(handle);
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < TIMEDMACHINE_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++)
		{
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition])
		{
		case TimedMachine_main_region_WaitForEvents:
		{
			react_main_region_WaitForEvents(handle);
			break;
		}
		case TimedMachine_main_region_WaitForAfter:
		{
			react_main_region_WaitForAfter(handle);
			break;
		}
		case TimedMachine_main_region_ProcessEvery:
		{
			react_main_region_ProcessEvery(handle);
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents(handle);
}

void timedMachine_exit(TimedMachine* handle)
{
	/* Default exit sequence for statechart TimedMachine */
	exseq_main_region(handle);
}

sc_boolean timedMachine_isActive(const TimedMachine* handle)
{
	sc_boolean result = bool_false;
	int i;
	
	for(i = 0; i < TIMEDMACHINE_MAX_ORTHOGONAL_STATES; i++)
	{
		result = result || handle->stateConfVector[i] != TimedMachine_last_state;
	}
	
	return result;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean timedMachine_isFinal(const TimedMachine* handle)
{
   return bool_false;
}

void timedMachine_raiseTimeEvent(TimedMachine* handle, sc_eventid evid)
{
	if ( ((sc_intptr_t)evid) >= ((sc_intptr_t)&(handle->timeEvents))
		&&  ((sc_intptr_t)evid) < ((sc_intptr_t)&(handle->timeEvents)) + sizeof(TimedMachineTimeEvents))
		{
		*(sc_boolean*)evid = bool_true;
		
		timedMachine_runCycle(handle);
	}		
}

sc_boolean timedMachine_isStateActive(const TimedMachine* handle, TimedMachineStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case TimedMachine_main_region_WaitForEvents :
			result = (sc_boolean) (handle->stateConfVector[SCVI_TIMEDMACHINE_MAIN_REGION_WAITFOREVENTS] == TimedMachine_main_region_WaitForEvents
			);
			break;
		case TimedMachine_main_region_WaitForAfter :
			result = (sc_boolean) (handle->stateConfVector[SCVI_TIMEDMACHINE_MAIN_REGION_WAITFORAFTER] == TimedMachine_main_region_WaitForAfter
			);
			break;
		case TimedMachine_main_region_ProcessEvery :
			result = (sc_boolean) (handle->stateConfVector[SCVI_TIMEDMACHINE_MAIN_REGION_PROCESSEVERY] == TimedMachine_main_region_ProcessEvery
			);
			break;
		default:
			result = bool_false;
			break;
	}
	return result;
}

static void clearInEvents(TimedMachine* handle)
{
	handle->iface.testAfter_raised = bool_false;
	handle->iface.testEvery_raised = bool_false;
	handle->timeEvents.timedMachine_main_region_WaitForAfter_tev0_raised = bool_false;
	handle->timeEvents.timedMachine_main_region_ProcessEvery_tev0_raised = bool_false;
}

static void clearOutEvents(TimedMachine* handle)
{
}

void timedMachineIface_raise_testAfter(TimedMachine* handle)
{
	TimedMachine_in_event *e = os_memblock_get(handle->pool);
	e->osev.ev_queued = 0;
	e->osev.ev_arg = (void*) handle;
	e->osev.ev_cb = timedMachineIface_process_testAfter;
	e->osev.ev_next.stqe_next = 0;
	os_eventq_put(handle->queue, (struct os_event*) e);
}
void timedMachineIface_raise_testEvery(TimedMachine* handle)
{
	TimedMachine_in_event *e = os_memblock_get(handle->pool);
	e->osev.ev_queued = 0;
	e->osev.ev_arg = (void*) handle;
	e->osev.ev_cb = timedMachineIface_process_testEvery;
	e->osev.ev_next.stqe_next = 0;
	os_eventq_put(handle->queue, (struct os_event*) e);
}


sc_integer timedMachineIface_get_counter(const TimedMachine* handle)
{
	return handle->iface.counter;
}
void timedMachineIface_set_counter(TimedMachine* handle, sc_integer value)
{
	handle->iface.counter = value;
}

/* implementations of all internal functions */

static sc_boolean check_main_region_WaitForEvents_tr0_tr0(const TimedMachine* handle)
{
	return handle->iface.testAfter_raised;
}

static sc_boolean check_main_region_WaitForEvents_tr1_tr1(const TimedMachine* handle)
{
	return handle->iface.testEvery_raised;
}

static sc_boolean check_main_region_WaitForAfter_tr0_tr0(const TimedMachine* handle)
{
	return handle->timeEvents.timedMachine_main_region_WaitForAfter_tev0_raised;
}

static sc_boolean check_main_region_ProcessEvery_lr0_lr0(const TimedMachine* handle)
{
	return (handle->iface.counter < 5) ? bool_true : bool_false;
}

static sc_boolean check_main_region_ProcessEvery_tr0_tr0(const TimedMachine* handle)
{
	return ((handle->timeEvents.timedMachine_main_region_ProcessEvery_tev0_raised) && (handle->iface.counter == 5)) ? bool_true : bool_false;
}

static void effect_main_region_WaitForEvents_tr0(TimedMachine* handle)
{
	exseq_main_region_WaitForEvents(handle);
	enseq_main_region_WaitForAfter_default(handle);
}

static void effect_main_region_WaitForEvents_tr1(TimedMachine* handle)
{
	exseq_main_region_WaitForEvents(handle);
	enseq_main_region_ProcessEvery_default(handle);
}

static void effect_main_region_WaitForAfter_tr0(TimedMachine* handle)
{
	exseq_main_region_WaitForAfter(handle);
	enseq_main_region_WaitForEvents_default(handle);
}

static void effect_main_region_ProcessEvery_lr0_lr0(TimedMachine* handle)
{
	handle->iface.counter += 1;
}

static void effect_main_region_ProcessEvery_tr0(TimedMachine* handle)
{
	exseq_main_region_ProcessEvery(handle);
	enseq_main_region_WaitForEvents_default(handle);
}

/* Entry action for state 'WaitForAfter'. */
static void enact_main_region_WaitForAfter(TimedMachine* handle)
{
	/* Entry action for state 'WaitForAfter'. */
	handle->ts_conn->ts_methods->start(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.timedMachine_main_region_WaitForAfter_tev0_raised), 
										100,
										bool_false);
}

/* Entry action for state 'ProcessEvery'. */
static void enact_main_region_ProcessEvery(TimedMachine* handle)
{
	/* Entry action for state 'ProcessEvery'. */
	handle->ts_conn->ts_methods->start(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.timedMachine_main_region_ProcessEvery_tev0_raised), 
										10,
										bool_true);
}

/* Exit action for state 'WaitForAfter'. */
static void exact_main_region_WaitForAfter(TimedMachine* handle)
{
	/* Exit action for state 'WaitForAfter'. */
	handle->ts_conn->ts_methods->stop(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.timedMachine_main_region_WaitForAfter_tev0_raised));
}

/* Exit action for state 'ProcessEvery'. */
static void exact_main_region_ProcessEvery(TimedMachine* handle)
{
	/* Exit action for state 'ProcessEvery'. */
	handle->ts_conn->ts_methods->stop(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.timedMachine_main_region_ProcessEvery_tev0_raised));
}

/* 'default' enter sequence for state WaitForEvents */
static void enseq_main_region_WaitForEvents_default(TimedMachine* handle)
{
	/* 'default' enter sequence for state WaitForEvents */
	handle->stateConfVector[0] = TimedMachine_main_region_WaitForEvents;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state WaitForAfter */
static void enseq_main_region_WaitForAfter_default(TimedMachine* handle)
{
	/* 'default' enter sequence for state WaitForAfter */
	enact_main_region_WaitForAfter(handle);
	handle->stateConfVector[0] = TimedMachine_main_region_WaitForAfter;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state ProcessEvery */
static void enseq_main_region_ProcessEvery_default(TimedMachine* handle)
{
	/* 'default' enter sequence for state ProcessEvery */
	enact_main_region_ProcessEvery(handle);
	handle->stateConfVector[0] = TimedMachine_main_region_ProcessEvery;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
static void enseq_main_region_default(TimedMachine* handle)
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default(handle);
}

/* Default exit sequence for state WaitForEvents */
static void exseq_main_region_WaitForEvents(TimedMachine* handle)
{
	/* Default exit sequence for state WaitForEvents */
	handle->stateConfVector[0] = TimedMachine_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state WaitForAfter */
static void exseq_main_region_WaitForAfter(TimedMachine* handle)
{
	/* Default exit sequence for state WaitForAfter */
	handle->stateConfVector[0] = TimedMachine_last_state;
	handle->stateConfVectorPosition = 0;
	exact_main_region_WaitForAfter(handle);
}

/* Default exit sequence for state ProcessEvery */
static void exseq_main_region_ProcessEvery(TimedMachine* handle)
{
	/* Default exit sequence for state ProcessEvery */
	handle->stateConfVector[0] = TimedMachine_last_state;
	handle->stateConfVectorPosition = 0;
	exact_main_region_ProcessEvery(handle);
}

/* Default exit sequence for region main region */
static void exseq_main_region(TimedMachine* handle)
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of TimedMachine.main_region) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case TimedMachine_main_region_WaitForEvents :
		{
			exseq_main_region_WaitForEvents(handle);
			break;
		}
		case TimedMachine_main_region_WaitForAfter :
		{
			exseq_main_region_WaitForAfter(handle);
			break;
		}
		case TimedMachine_main_region_ProcessEvery :
		{
			exseq_main_region_ProcessEvery(handle);
			break;
		}
		default: break;
	}
}

/* The reactions of state WaitForEvents. */
static void react_main_region_WaitForEvents(TimedMachine* handle)
{
	/* The reactions of state WaitForEvents. */
	if (check_main_region_WaitForEvents_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_WaitForEvents_tr0(handle);
	}  else
	{
		if (check_main_region_WaitForEvents_tr1_tr1(handle) == bool_true)
		{ 
			effect_main_region_WaitForEvents_tr1(handle);
		} 
	}
}

/* The reactions of state WaitForAfter. */
static void react_main_region_WaitForAfter(TimedMachine* handle)
{
	/* The reactions of state WaitForAfter. */
	if (check_main_region_WaitForAfter_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_WaitForAfter_tr0(handle);
	} 
}

/* The reactions of state ProcessEvery. */
static void react_main_region_ProcessEvery(TimedMachine* handle)
{
	/* The reactions of state ProcessEvery. */
	if (check_main_region_ProcessEvery_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_ProcessEvery_tr0(handle);
	}  else
	{
		if (check_main_region_ProcessEvery_lr0_lr0(handle) == bool_true)
		{ 
			effect_main_region_ProcessEvery_lr0_lr0(handle);
		} 
	}
}

/* Default react sequence for initial entry  */
static void react_main_region__entry_Default(TimedMachine* handle)
{
	/* Default react sequence for initial entry  */
	enseq_main_region_WaitForEvents_default(handle);
}


void timedMachineIface_process_testAfter(struct os_event * osev)
{
	TimedMachine * handle = osev->ev_arg;
	handle->iface.testAfter_raised = bool_true;
	
	timedMachine_runCycle(handle);
	os_memblock_put(handle->pool, osev);
}

void timedMachineIface_process_testEvery(struct os_event * osev)
{
	TimedMachine * handle = osev->ev_arg;
	handle->iface.testEvery_raised = bool_true;
	
	timedMachine_runCycle(handle);
	os_memblock_put(handle->pool, osev);
}

