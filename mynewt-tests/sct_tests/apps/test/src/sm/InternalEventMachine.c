
#include "InternalEventMachine.h"
#include "../smlib/sc_types.h"

#include <stdlib.h>
#include <string.h>
/*! \file Implementation of the state machine 'InternalEventMachine'
*/

/* prototypes of all internal functions */
static sc_boolean check_main_region_Wait_tr0_tr0(const InternalEventMachine* handle);
static sc_boolean check_main_region_FillArray_tr0_tr0(const InternalEventMachine* handle);
static sc_boolean check_main_region_FillArray__region0_Check_tr0_tr0(const InternalEventMachine* handle);
static sc_boolean check_main_region_FillArray__region0_Set_tr0_tr0(const InternalEventMachine* handle);
static void effect_main_region_Wait_tr0(InternalEventMachine* handle);
static void effect_main_region_FillArray_tr0(InternalEventMachine* handle);
static void effect_main_region_FillArray__region0_Check_tr0(InternalEventMachine* handle);
static void effect_main_region_FillArray__region0_Set_tr0(InternalEventMachine* handle);
static void enact_main_region_FillArray__region0_Check(InternalEventMachine* handle);
static void enact_main_region_FillArray__region0_Set(InternalEventMachine* handle);
static void enseq_main_region_Wait_default(InternalEventMachine* handle);
static void enseq_main_region_FillArray_default(InternalEventMachine* handle);
static void enseq_main_region_FillArray__region0_Check_default(InternalEventMachine* handle);
static void enseq_main_region_FillArray__region0_Set_default(InternalEventMachine* handle);
static void enseq_main_region__final__default(InternalEventMachine* handle);
static void enseq_main_region_default(InternalEventMachine* handle);
static void enseq_main_region_FillArray__region0_default(InternalEventMachine* handle);
static void exseq_main_region_Wait(InternalEventMachine* handle);
static void exseq_main_region_FillArray(InternalEventMachine* handle);
static void exseq_main_region_FillArray__region0_Check(InternalEventMachine* handle);
static void exseq_main_region_FillArray__region0_Set(InternalEventMachine* handle);
static void exseq_main_region__final_(InternalEventMachine* handle);
static void exseq_main_region(InternalEventMachine* handle);
static void exseq_main_region_FillArray__region0(InternalEventMachine* handle);
static void react_main_region_Wait(InternalEventMachine* handle);
static void react_main_region_FillArray__region0_Check(InternalEventMachine* handle);
static void react_main_region_FillArray__region0_Set(InternalEventMachine* handle);
static void react_main_region__final_(InternalEventMachine* handle);
static void react_main_region__entry_Default(InternalEventMachine* handle);
static void react_main_region_FillArray__region0__entry_Default(InternalEventMachine* handle);
static void clearInEvents(InternalEventMachine* handle);
static void clearOutEvents(InternalEventMachine* handle);
static void internaleventmachine_eventqueue_init(internaleventmachine_eventqueue * eq);
static sc_integer internaleventmachine_eventqueue_size(internaleventmachine_eventqueue * eq);
static internaleventmachine_internal_event internaleventmachine_eventqueue_pop(internaleventmachine_eventqueue * eq);
static sc_boolean internaleventmachine_eventqueue_push(internaleventmachine_eventqueue * eq, internaleventmachine_internal_event ev);

static void internalEventMachine_dispatch_event(InternalEventMachine* handle, const internaleventmachine_internal_event * event);

static void internaleventmachine_internal_event_init(internaleventmachine_internal_event * ev, internaleventmachine_event_name name);
static void internalEventMachine_add_event_to_queue(InternalEventMachine* handle, internaleventmachine_event_name name);


void internalEventMachine_init(InternalEventMachine* handle, struct os_eventq *q, struct os_mempool *pool)
{
	handle->queue = q;
	handle->pool = pool;
		sc_integer i;
	
		for (i = 0; i < INTERNALEVENTMACHINE_MAX_ORTHOGONAL_STATES; ++i)
		{
			handle->stateConfVector[i] = InternalEventMachine_last_state;
		}
		
		
		handle->stateConfVectorPosition = 0;
	
		clearInEvents(handle);
		clearOutEvents(handle);
	
		/* Default init sequence for statechart InternalEventMachine */
		handle->internal.i = 0;
		handle->internal.b = bool_false;
	internaleventmachine_eventqueue_init(&(handle->internal_event_queue));
}

void internalEventMachine_enter(InternalEventMachine* handle)
{
	/* Default enter sequence for statechart InternalEventMachine */
	enseq_main_region_default(handle);
}

void internalEventMachine_runCycle(InternalEventMachine* handle)
{
	clearOutEvents(handle);
	
	internaleventmachine_internal_event currentEvent = internaleventmachine_eventqueue_pop(&(handle->internal_event_queue));
	
	do {
		internalEventMachine_dispatch_event(handle, &currentEvent);
		for (handle->stateConfVectorPosition = 0;
			handle->stateConfVectorPosition < INTERNALEVENTMACHINE_MAX_ORTHOGONAL_STATES;
			handle->stateConfVectorPosition++)
			{
				
			switch (handle->stateConfVector[handle->stateConfVectorPosition])
			{
			case InternalEventMachine_main_region_Wait:
			{
				react_main_region_Wait(handle);
				break;
			}
			case InternalEventMachine_main_region_FillArray__region0_Check:
			{
				react_main_region_FillArray__region0_Check(handle);
				break;
			}
			case InternalEventMachine_main_region_FillArray__region0_Set:
			{
				react_main_region_FillArray__region0_Set(handle);
				break;
			}
			case InternalEventMachine_main_region__final_:
			{
				react_main_region__final_(handle);
				break;
			}
			default:
				break;
			}
		}
		
		clearInEvents(handle);
	} while((currentEvent = internaleventmachine_eventqueue_pop(&(handle->internal_event_queue))).name != internaleventmachine_invalid_event);
	
}

void internalEventMachine_exit(InternalEventMachine* handle)
{
	/* Default exit sequence for statechart InternalEventMachine */
	exseq_main_region(handle);
}

sc_boolean internalEventMachine_isActive(const InternalEventMachine* handle)
{
	sc_boolean result = bool_false;
	int i;
	
	for(i = 0; i < INTERNALEVENTMACHINE_MAX_ORTHOGONAL_STATES; i++)
	{
		result = result || handle->stateConfVector[i] != InternalEventMachine_last_state;
	}
	
	return result;
}

sc_boolean internalEventMachine_isFinal(const InternalEventMachine* handle)
{
	return (handle->stateConfVector[0] == InternalEventMachine_main_region__final_);

}

sc_boolean internalEventMachine_isStateActive(const InternalEventMachine* handle, InternalEventMachineStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case InternalEventMachine_main_region_Wait :
			result = (sc_boolean) (handle->stateConfVector[SCVI_INTERNALEVENTMACHINE_MAIN_REGION_WAIT] == InternalEventMachine_main_region_Wait
			);
			break;
		case InternalEventMachine_main_region_FillArray :
			result = (sc_boolean) (handle->stateConfVector[SCVI_INTERNALEVENTMACHINE_MAIN_REGION_FILLARRAY] >= InternalEventMachine_main_region_FillArray
				&& handle->stateConfVector[SCVI_INTERNALEVENTMACHINE_MAIN_REGION_FILLARRAY] <= InternalEventMachine_main_region_FillArray__region0_Set);
			break;
		case InternalEventMachine_main_region_FillArray__region0_Check :
			result = (sc_boolean) (handle->stateConfVector[SCVI_INTERNALEVENTMACHINE_MAIN_REGION_FILLARRAY__REGION0_CHECK] == InternalEventMachine_main_region_FillArray__region0_Check
			);
			break;
		case InternalEventMachine_main_region_FillArray__region0_Set :
			result = (sc_boolean) (handle->stateConfVector[SCVI_INTERNALEVENTMACHINE_MAIN_REGION_FILLARRAY__REGION0_SET] == InternalEventMachine_main_region_FillArray__region0_Set
			);
			break;
		case InternalEventMachine_main_region__final_ :
			result = (sc_boolean) (handle->stateConfVector[SCVI_INTERNALEVENTMACHINE_MAIN_REGION__FINAL_] == InternalEventMachine_main_region__final_
			);
			break;
		default:
			result = bool_false;
			break;
	}
	return result;
}

static void clearInEvents(InternalEventMachine* handle)
{
	handle->iface.start_raised = bool_false;
	handle->internal.arr_full_raised = bool_false;
	handle->internal.next_raised = bool_false;
}

static void clearOutEvents(InternalEventMachine* handle)
{
}

void internalEventMachineIface_raise_start(InternalEventMachine* handle)
{
	InternalEventMachine_in_event *e = os_memblock_get(handle->pool);
	e->osev.ev_queued = 0;
	e->osev.ev_arg = (void*) handle;
	e->osev.ev_cb = internalEventMachineIface_process_start;
	e->osev.ev_next.stqe_next = 0;
	os_eventq_put(handle->queue, (struct os_event*) e);
}



/* implementations of all internal functions */

static sc_boolean check_main_region_Wait_tr0_tr0(const InternalEventMachine* handle)
{
	return handle->iface.start_raised;
}

static sc_boolean check_main_region_FillArray_tr0_tr0(const InternalEventMachine* handle)
{
	return handle->internal.arr_full_raised;
}

static sc_boolean check_main_region_FillArray__region0_Check_tr0_tr0(const InternalEventMachine* handle)
{
	return handle->internal.next_raised;
}

static sc_boolean check_main_region_FillArray__region0_Set_tr0_tr0(const InternalEventMachine* handle)
{
	return handle->internal.next_raised;
}

static void effect_main_region_Wait_tr0(InternalEventMachine* handle)
{
	exseq_main_region_Wait(handle);
	enseq_main_region_FillArray_default(handle);
}

static void effect_main_region_FillArray_tr0(InternalEventMachine* handle)
{
	exseq_main_region_FillArray(handle);
	enseq_main_region__final__default(handle);
}

static void effect_main_region_FillArray__region0_Check_tr0(InternalEventMachine* handle)
{
	exseq_main_region_FillArray__region0_Check(handle);
	enseq_main_region_FillArray__region0_Set_default(handle);
}

static void effect_main_region_FillArray__region0_Set_tr0(InternalEventMachine* handle)
{
	exseq_main_region_FillArray__region0_Set(handle);
	enseq_main_region_FillArray__region0_Check_default(handle);
}

/* Entry action for state 'Check'. */
static void enact_main_region_FillArray__region0_Check(InternalEventMachine* handle)
{
	/* Entry action for state 'Check'. */
	if ((handle->internal.i < ARR_SIZE) ? bool_true : bool_false == bool_true)
	{ 
		internalEventMachine_add_event_to_queue(handle, internalEventMachineInternal_next);
	} 
	if ((handle->internal.i == ARR_SIZE) ? bool_true : bool_false == bool_true)
	{ 
		internalEventMachine_add_event_to_queue(handle, internalEventMachineInternal_arr_full);
	} 
}

/* Entry action for state 'Set'. */
static void enact_main_region_FillArray__region0_Set(InternalEventMachine* handle)
{
	/* Entry action for state 'Set'. */
	arr[handle->internal.i] = handle->internal.i;
	handle->internal.i++;
	internalEventMachine_add_event_to_queue(handle, internalEventMachineInternal_next);
}

/* 'default' enter sequence for state Wait */
static void enseq_main_region_Wait_default(InternalEventMachine* handle)
{
	/* 'default' enter sequence for state Wait */
	handle->stateConfVector[0] = InternalEventMachine_main_region_Wait;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state FillArray */
static void enseq_main_region_FillArray_default(InternalEventMachine* handle)
{
	/* 'default' enter sequence for state FillArray */
	enseq_main_region_FillArray__region0_default(handle);
}

/* 'default' enter sequence for state Check */
static void enseq_main_region_FillArray__region0_Check_default(InternalEventMachine* handle)
{
	/* 'default' enter sequence for state Check */
	enact_main_region_FillArray__region0_Check(handle);
	handle->stateConfVector[0] = InternalEventMachine_main_region_FillArray__region0_Check;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Set */
static void enseq_main_region_FillArray__region0_Set_default(InternalEventMachine* handle)
{
	/* 'default' enter sequence for state Set */
	enact_main_region_FillArray__region0_Set(handle);
	handle->stateConfVector[0] = InternalEventMachine_main_region_FillArray__region0_Set;
	handle->stateConfVectorPosition = 0;
}

/* Default enter sequence for state null */
static void enseq_main_region__final__default(InternalEventMachine* handle)
{
	/* Default enter sequence for state null */
	handle->stateConfVector[0] = InternalEventMachine_main_region__final_;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
static void enseq_main_region_default(InternalEventMachine* handle)
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default(handle);
}

/* 'default' enter sequence for region null */
static void enseq_main_region_FillArray__region0_default(InternalEventMachine* handle)
{
	/* 'default' enter sequence for region null */
	react_main_region_FillArray__region0__entry_Default(handle);
}

/* Default exit sequence for state Wait */
static void exseq_main_region_Wait(InternalEventMachine* handle)
{
	/* Default exit sequence for state Wait */
	handle->stateConfVector[0] = InternalEventMachine_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state FillArray */
static void exseq_main_region_FillArray(InternalEventMachine* handle)
{
	/* Default exit sequence for state FillArray */
	exseq_main_region_FillArray__region0(handle);
}

/* Default exit sequence for state Check */
static void exseq_main_region_FillArray__region0_Check(InternalEventMachine* handle)
{
	/* Default exit sequence for state Check */
	handle->stateConfVector[0] = InternalEventMachine_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state Set */
static void exseq_main_region_FillArray__region0_Set(InternalEventMachine* handle)
{
	/* Default exit sequence for state Set */
	handle->stateConfVector[0] = InternalEventMachine_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for final state. */
static void exseq_main_region__final_(InternalEventMachine* handle)
{
	/* Default exit sequence for final state. */
	handle->stateConfVector[0] = InternalEventMachine_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for region main region */
static void exseq_main_region(InternalEventMachine* handle)
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of InternalEventMachine.main_region) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case InternalEventMachine_main_region_Wait :
		{
			exseq_main_region_Wait(handle);
			break;
		}
		case InternalEventMachine_main_region_FillArray__region0_Check :
		{
			exseq_main_region_FillArray__region0_Check(handle);
			break;
		}
		case InternalEventMachine_main_region_FillArray__region0_Set :
		{
			exseq_main_region_FillArray__region0_Set(handle);
			break;
		}
		case InternalEventMachine_main_region__final_ :
		{
			exseq_main_region__final_(handle);
			break;
		}
		default: break;
	}
}

/* Default exit sequence for region null */
static void exseq_main_region_FillArray__region0(InternalEventMachine* handle)
{
	/* Default exit sequence for region null */
	/* Handle exit of all possible states (of InternalEventMachine.main_region.FillArray._region0) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case InternalEventMachine_main_region_FillArray__region0_Check :
		{
			exseq_main_region_FillArray__region0_Check(handle);
			break;
		}
		case InternalEventMachine_main_region_FillArray__region0_Set :
		{
			exseq_main_region_FillArray__region0_Set(handle);
			break;
		}
		default: break;
	}
}

/* The reactions of state Wait. */
static void react_main_region_Wait(InternalEventMachine* handle)
{
	/* The reactions of state Wait. */
	if (check_main_region_Wait_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_Wait_tr0(handle);
	} 
}

/* The reactions of state Check. */
static void react_main_region_FillArray__region0_Check(InternalEventMachine* handle)
{
	/* The reactions of state Check. */
	if (check_main_region_FillArray__region0_Check_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_FillArray__region0_Check_tr0(handle);
	}  else
	{
		if (check_main_region_FillArray_tr0_tr0(handle) == bool_true)
		{ 
			effect_main_region_FillArray_tr0(handle);
		} 
	}
}

/* The reactions of state Set. */
static void react_main_region_FillArray__region0_Set(InternalEventMachine* handle)
{
	/* The reactions of state Set. */
	if (check_main_region_FillArray__region0_Set_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_FillArray__region0_Set_tr0(handle);
	}  else
	{
		if (check_main_region_FillArray_tr0_tr0(handle) == bool_true)
		{ 
			effect_main_region_FillArray_tr0(handle);
		} 
	}
}

/* The reactions of state null. */
static void react_main_region__final_(InternalEventMachine* handle)
{
}

/* Default react sequence for initial entry  */
static void react_main_region__entry_Default(InternalEventMachine* handle)
{
	/* Default react sequence for initial entry  */
	enseq_main_region_Wait_default(handle);
}

/* Default react sequence for initial entry  */
static void react_main_region_FillArray__region0__entry_Default(InternalEventMachine* handle)
{
	/* Default react sequence for initial entry  */
	enseq_main_region_FillArray__region0_Check_default(handle);
}


static void internaleventmachine_internal_event_init(internaleventmachine_internal_event * ev, internaleventmachine_event_name name)
{
	ev->name = name;
}

static void internaleventmachine_eventqueue_init(internaleventmachine_eventqueue * eq)
{
	eq->push_index = 0;
	eq->pop_index = 0;
	eq->size = 0;
}

static sc_integer internaleventmachine_eventqueue_size(internaleventmachine_eventqueue * eq)
{
	return eq->size;
}

static internaleventmachine_internal_event internaleventmachine_eventqueue_pop(internaleventmachine_eventqueue * eq)
{
	internaleventmachine_internal_event event;
	if(internaleventmachine_eventqueue_size(eq) <= 0) {
		internaleventmachine_internal_event_init(&event, internaleventmachine_invalid_event);
	}
	else {
		event = eq->events[eq->pop_index];
		
		if(eq->pop_index < INTERNALEVENTMACHINE_EVENTQUEUE_BUFFERSIZE - 1) {
			eq->pop_index++;
		} 
		else {
			eq->pop_index = 0;
		}
		eq->size--;
	}
	return event;
}

static sc_boolean internaleventmachine_eventqueue_push(internaleventmachine_eventqueue * eq, internaleventmachine_internal_event ev)
{
	if(internaleventmachine_eventqueue_size(eq) == INTERNALEVENTMACHINE_EVENTQUEUE_BUFFERSIZE) {
		return false;
	}
	else {
		eq->events[eq->push_index] = ev;
		
		if(eq->push_index < INTERNALEVENTMACHINE_EVENTQUEUE_BUFFERSIZE - 1) {
			eq->push_index++;
		}
		else {
			eq->push_index = 0;
		}
		eq->size++;
		
		return true;
	}
}

static void internalEventMachine_add_event_to_queue(InternalEventMachine* handle, internaleventmachine_event_name name)
{
	internaleventmachine_internal_event event;
	internaleventmachine_internal_event_init(&event, name);
	internaleventmachine_eventqueue_push(&(handle->internal_event_queue), event);
}


static void internalEventMachine_dispatch_event(InternalEventMachine* handle, const internaleventmachine_internal_event * event) {
	switch(event->name) {
		case internalEventMachineInternal_arr_full:
		{
			handle->internal.arr_full_raised = bool_true;
			break;
		}
		case internalEventMachineInternal_next:
		{
			handle->internal.next_raised = bool_true;
			break;
		}
		default:
			break;
	}
}
void internalEventMachineIface_process_start(struct os_event * osev)
{
	InternalEventMachine * handle = osev->ev_arg;
	handle->iface.start_raised = bool_true;
	
	internalEventMachine_runCycle(handle);
	os_memblock_put(handle->pool, osev);
}

