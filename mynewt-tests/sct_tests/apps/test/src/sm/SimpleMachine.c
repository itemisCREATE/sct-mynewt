
#include "SimpleMachine.h"
#include "../smlib/sc_types.h"

#include <stdlib.h>
#include <string.h>
/*! \file Implementation of the state machine 'SimpleMachine'
*/

/* prototypes of all internal functions */
static sc_boolean check_main_region_StateA_tr0_tr0(const SimpleMachine* handle);
static sc_boolean check_main_region_StateB_tr0_tr0(const SimpleMachine* handle);
static sc_boolean check_main_region_StateC_lr0_lr0(const SimpleMachine* handle);
static sc_boolean check_main_region_StateC_lr1_lr1(const SimpleMachine* handle);
static sc_boolean check_main_region_StateC_tr0_tr0(const SimpleMachine* handle);
static void effect_main_region_StateA_tr0(SimpleMachine* handle);
static void effect_main_region_StateB_tr0(SimpleMachine* handle);
static void effect_main_region_StateC_lr0_lr0(SimpleMachine* handle);
static void effect_main_region_StateC_lr1_lr1(SimpleMachine* handle);
static void effect_main_region_StateC_tr0(SimpleMachine* handle);
static void enseq_main_region_StateA_default(SimpleMachine* handle);
static void enseq_main_region_StateB_default(SimpleMachine* handle);
static void enseq_main_region_StateC_default(SimpleMachine* handle);
static void enseq_main_region_default(SimpleMachine* handle);
static void exseq_main_region_StateA(SimpleMachine* handle);
static void exseq_main_region_StateB(SimpleMachine* handle);
static void exseq_main_region_StateC(SimpleMachine* handle);
static void exseq_main_region(SimpleMachine* handle);
static void react_main_region_StateA(SimpleMachine* handle);
static void react_main_region_StateB(SimpleMachine* handle);
static void react_main_region_StateC(SimpleMachine* handle);
static void react_main_region__entry_Default(SimpleMachine* handle);
static void clearInEvents(SimpleMachine* handle);
static void clearOutEvents(SimpleMachine* handle);


void simpleMachine_init(SimpleMachine* handle, struct os_eventq *q, struct os_mempool *pool)
{
	handle->queue = q;
	handle->pool = pool;
		sc_integer i;
	
		for (i = 0; i < SIMPLEMACHINE_MAX_ORTHOGONAL_STATES; ++i)
		{
			handle->stateConfVector[i] = SimpleMachine_last_state;
		}
		
		
		handle->stateConfVectorPosition = 0;
	
		clearInEvents(handle);
		clearOutEvents(handle);
	
		/* Default init sequence for statechart SimpleMachine */
		handle->iface.i = 0;
		handle->iface.b = bool_false;
}

void simpleMachine_enter(SimpleMachine* handle)
{
	/* Default enter sequence for statechart SimpleMachine */
	enseq_main_region_default(handle);
}

void simpleMachine_runCycle(SimpleMachine* handle)
{
	clearOutEvents(handle);
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < SIMPLEMACHINE_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++)
		{
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition])
		{
		case SimpleMachine_main_region_StateA:
		{
			react_main_region_StateA(handle);
			break;
		}
		case SimpleMachine_main_region_StateB:
		{
			react_main_region_StateB(handle);
			break;
		}
		case SimpleMachine_main_region_StateC:
		{
			react_main_region_StateC(handle);
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents(handle);
}

void simpleMachine_exit(SimpleMachine* handle)
{
	/* Default exit sequence for statechart SimpleMachine */
	exseq_main_region(handle);
}

sc_boolean simpleMachine_isActive(const SimpleMachine* handle)
{
	sc_boolean result = bool_false;
	int i;
	
	for(i = 0; i < SIMPLEMACHINE_MAX_ORTHOGONAL_STATES; i++)
	{
		result = result || handle->stateConfVector[i] != SimpleMachine_last_state;
	}
	
	return result;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean simpleMachine_isFinal(const SimpleMachine* handle)
{
   return bool_false;
}

sc_boolean simpleMachine_isStateActive(const SimpleMachine* handle, SimpleMachineStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case SimpleMachine_main_region_StateA :
			result = (sc_boolean) (handle->stateConfVector[SCVI_SIMPLEMACHINE_MAIN_REGION_STATEA] == SimpleMachine_main_region_StateA
			);
			break;
		case SimpleMachine_main_region_StateB :
			result = (sc_boolean) (handle->stateConfVector[SCVI_SIMPLEMACHINE_MAIN_REGION_STATEB] == SimpleMachine_main_region_StateB
			);
			break;
		case SimpleMachine_main_region_StateC :
			result = (sc_boolean) (handle->stateConfVector[SCVI_SIMPLEMACHINE_MAIN_REGION_STATEC] == SimpleMachine_main_region_StateC
			);
			break;
		default:
			result = bool_false;
			break;
	}
	return result;
}

static void clearInEvents(SimpleMachine* handle)
{
	handle->iface.e1_raised = bool_false;
	handle->iface.e2_raised = bool_false;
	handle->iface.i_e_raised = bool_false;
	handle->iface.b_e_raised = bool_false;
}

static void clearOutEvents(SimpleMachine* handle)
{
}

void simpleMachineIface_raise_e1(SimpleMachine* handle)
{
	SimpleMachine_in_event *e = os_memblock_get(handle->pool);
	e->osev.ev_queued = 0;
	e->osev.ev_arg = (void*) handle;
	e->osev.ev_cb = simpleMachineIface_process_e1;
	e->osev.ev_next.stqe_next = 0;
	os_eventq_put(handle->queue, (struct os_event*) e);
}
void simpleMachineIface_raise_e2(SimpleMachine* handle)
{
	SimpleMachine_in_event *e = os_memblock_get(handle->pool);
	e->osev.ev_queued = 0;
	e->osev.ev_arg = (void*) handle;
	e->osev.ev_cb = simpleMachineIface_process_e2;
	e->osev.ev_next.stqe_next = 0;
	os_eventq_put(handle->queue, (struct os_event*) e);
}
void simpleMachineIface_raise_i_e(SimpleMachine* handle, sc_integer value)
{
	SimpleMachine_in_event *e = os_memblock_get(handle->pool);
	e->osev.ev_queued = 0;
	e->osev.ev_arg = (void*) handle;
	e->osev.ev_cb = simpleMachineIface_process_i_e;
	e->osev.ev_next.stqe_next = 0;
	e->value.simpleMachineIface_i_e_value = value;
	os_eventq_put(handle->queue, (struct os_event*) e);
}
void simpleMachineIface_raise_b_e(SimpleMachine* handle, sc_boolean value)
{
	SimpleMachine_in_event *e = os_memblock_get(handle->pool);
	e->osev.ev_queued = 0;
	e->osev.ev_arg = (void*) handle;
	e->osev.ev_cb = simpleMachineIface_process_b_e;
	e->osev.ev_next.stqe_next = 0;
	e->value.simpleMachineIface_b_e_value = value;
	os_eventq_put(handle->queue, (struct os_event*) e);
}


sc_integer simpleMachineIface_get_i(const SimpleMachine* handle)
{
	return handle->iface.i;
}
void simpleMachineIface_set_i(SimpleMachine* handle, sc_integer value)
{
	handle->iface.i = value;
}
sc_boolean simpleMachineIface_get_b(const SimpleMachine* handle)
{
	return handle->iface.b;
}
void simpleMachineIface_set_b(SimpleMachine* handle, sc_boolean value)
{
	handle->iface.b = value;
}

/* implementations of all internal functions */

static sc_boolean check_main_region_StateA_tr0_tr0(const SimpleMachine* handle)
{
	return handle->iface.e1_raised;
}

static sc_boolean check_main_region_StateB_tr0_tr0(const SimpleMachine* handle)
{
	return handle->iface.e2_raised;
}

static sc_boolean check_main_region_StateC_lr0_lr0(const SimpleMachine* handle)
{
	return handle->iface.i_e_raised;
}

static sc_boolean check_main_region_StateC_lr1_lr1(const SimpleMachine* handle)
{
	return handle->iface.b_e_raised;
}

static sc_boolean check_main_region_StateC_tr0_tr0(const SimpleMachine* handle)
{
	return handle->iface.e1_raised;
}

static void effect_main_region_StateA_tr0(SimpleMachine* handle)
{
	exseq_main_region_StateA(handle);
	enseq_main_region_StateB_default(handle);
}

static void effect_main_region_StateB_tr0(SimpleMachine* handle)
{
	exseq_main_region_StateB(handle);
	enseq_main_region_StateC_default(handle);
}

static void effect_main_region_StateC_lr0_lr0(SimpleMachine* handle)
{
	handle->iface.i = handle->iface.i_e_value;
}

static void effect_main_region_StateC_lr1_lr1(SimpleMachine* handle)
{
	handle->iface.b = handle->iface.b_e_value;
}

static void effect_main_region_StateC_tr0(SimpleMachine* handle)
{
	exseq_main_region_StateC(handle);
	enseq_main_region_StateA_default(handle);
}

/* 'default' enter sequence for state StateA */
static void enseq_main_region_StateA_default(SimpleMachine* handle)
{
	/* 'default' enter sequence for state StateA */
	handle->stateConfVector[0] = SimpleMachine_main_region_StateA;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state StateB */
static void enseq_main_region_StateB_default(SimpleMachine* handle)
{
	/* 'default' enter sequence for state StateB */
	handle->stateConfVector[0] = SimpleMachine_main_region_StateB;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state StateC */
static void enseq_main_region_StateC_default(SimpleMachine* handle)
{
	/* 'default' enter sequence for state StateC */
	handle->stateConfVector[0] = SimpleMachine_main_region_StateC;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
static void enseq_main_region_default(SimpleMachine* handle)
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default(handle);
}

/* Default exit sequence for state StateA */
static void exseq_main_region_StateA(SimpleMachine* handle)
{
	/* Default exit sequence for state StateA */
	handle->stateConfVector[0] = SimpleMachine_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state StateB */
static void exseq_main_region_StateB(SimpleMachine* handle)
{
	/* Default exit sequence for state StateB */
	handle->stateConfVector[0] = SimpleMachine_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state StateC */
static void exseq_main_region_StateC(SimpleMachine* handle)
{
	/* Default exit sequence for state StateC */
	handle->stateConfVector[0] = SimpleMachine_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for region main region */
static void exseq_main_region(SimpleMachine* handle)
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of SimpleMachine.main_region) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case SimpleMachine_main_region_StateA :
		{
			exseq_main_region_StateA(handle);
			break;
		}
		case SimpleMachine_main_region_StateB :
		{
			exseq_main_region_StateB(handle);
			break;
		}
		case SimpleMachine_main_region_StateC :
		{
			exseq_main_region_StateC(handle);
			break;
		}
		default: break;
	}
}

/* The reactions of state StateA. */
static void react_main_region_StateA(SimpleMachine* handle)
{
	/* The reactions of state StateA. */
	if (check_main_region_StateA_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_StateA_tr0(handle);
	} 
}

/* The reactions of state StateB. */
static void react_main_region_StateB(SimpleMachine* handle)
{
	/* The reactions of state StateB. */
	if (check_main_region_StateB_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_StateB_tr0(handle);
	} 
}

/* The reactions of state StateC. */
static void react_main_region_StateC(SimpleMachine* handle)
{
	/* The reactions of state StateC. */
	if (check_main_region_StateC_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_StateC_tr0(handle);
	}  else
	{
		if (check_main_region_StateC_lr0_lr0(handle) == bool_true)
		{ 
			effect_main_region_StateC_lr0_lr0(handle);
		} 
		if (check_main_region_StateC_lr1_lr1(handle) == bool_true)
		{ 
			effect_main_region_StateC_lr1_lr1(handle);
		} 
	}
}

/* Default react sequence for initial entry  */
static void react_main_region__entry_Default(SimpleMachine* handle)
{
	/* Default react sequence for initial entry  */
	enseq_main_region_StateA_default(handle);
}


void simpleMachineIface_process_e1(struct os_event * osev)
{
	SimpleMachine * handle = osev->ev_arg;
	handle->iface.e1_raised = bool_true;
	
	simpleMachine_runCycle(handle);
	os_memblock_put(handle->pool, osev);
}

void simpleMachineIface_process_e2(struct os_event * osev)
{
	SimpleMachine * handle = osev->ev_arg;
	handle->iface.e2_raised = bool_true;
	
	simpleMachine_runCycle(handle);
	os_memblock_put(handle->pool, osev);
}

void simpleMachineIface_process_i_e(struct os_event * osev)
{
	SimpleMachine * handle = osev->ev_arg;
	SimpleMachine_in_event * e = (SimpleMachine_in_event *) osev;
	sc_integer value = e->value.simpleMachineIface_i_e_value;
	handle->iface.i_e_value = value;
	handle->iface.i_e_raised = bool_true;
	
	simpleMachine_runCycle(handle);
	os_memblock_put(handle->pool, osev);
}

void simpleMachineIface_process_b_e(struct os_event * osev)
{
	SimpleMachine * handle = osev->ev_arg;
	SimpleMachine_in_event * e = (SimpleMachine_in_event *) osev;
	sc_boolean value = e->value.simpleMachineIface_b_e_value;
	handle->iface.b_e_value = value;
	handle->iface.b_e_raised = bool_true;
	
	simpleMachine_runCycle(handle);
	os_memblock_put(handle->pool, osev);
}

