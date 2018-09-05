
#include "Driver.h"
#include "../smlib/sc_types.h"

#include <stdlib.h>
#include <string.h>
/*! \file Implementation of the state machine 'driver'
*/

/* prototypes of all internal functions */
static sc_boolean check_main_region_Step1_tr0_tr0(const Driver* handle);
static sc_boolean check_main_region_Step2_tr0_tr0(const Driver* handle);
static sc_boolean check_main_region_Step3_tr0_tr0(const Driver* handle);
static void effect_main_region_Step1_tr0(Driver* handle);
static void effect_main_region_Step2_tr0(Driver* handle);
static void effect_main_region_Step3_tr0(Driver* handle);
static void enact_main_region_Step1(Driver* handle);
static void enact_main_region_Step2(Driver* handle);
static void enact_main_region_Step3(Driver* handle);
static void exact_main_region_Step1(Driver* handle);
static void exact_main_region_Step2(Driver* handle);
static void exact_main_region_Step3(Driver* handle);
static void enseq_main_region_Step1_default(Driver* handle);
static void enseq_main_region_Step2_default(Driver* handle);
static void enseq_main_region_Step3_default(Driver* handle);
static void enseq_main_region_default(Driver* handle);
static void exseq_main_region_Step1(Driver* handle);
static void exseq_main_region_Step2(Driver* handle);
static void exseq_main_region_Step3(Driver* handle);
static void exseq_main_region(Driver* handle);
static void react_main_region_Step1(Driver* handle);
static void react_main_region_Step2(Driver* handle);
static void react_main_region_Step3(Driver* handle);
static void react_main_region__entry_Default(Driver* handle);
static void clearInEvents(Driver* handle);
static void clearOutEvents(Driver* handle);


void driver_init(Driver* handle, sc_ts_connection_t *tsc, struct os_eventq *q, struct os_mempool *pool)
{
	handle->ts_conn = tsc;
	handle->queue = q;
	handle->pool = pool;
		sc_integer i;
	
		for (i = 0; i < DRIVER_MAX_ORTHOGONAL_STATES; ++i)
		{
			handle->stateConfVector[i] = Driver_last_state;
		}
		
		
		handle->stateConfVectorPosition = 0;
	
		clearInEvents(handle);
		clearOutEvents(handle);
	
}

void driver_enter(Driver* handle)
{
	/* Default enter sequence for statechart driver */
	enseq_main_region_default(handle);
}

void driver_runCycle(Driver* handle)
{
	clearOutEvents(handle);
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < DRIVER_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++)
		{
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition])
		{
		case Driver_main_region_Step1:
		{
			react_main_region_Step1(handle);
			break;
		}
		case Driver_main_region_Step2:
		{
			react_main_region_Step2(handle);
			break;
		}
		case Driver_main_region_Step3:
		{
			react_main_region_Step3(handle);
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents(handle);
}

void driver_exit(Driver* handle)
{
	/* Default exit sequence for statechart driver */
	exseq_main_region(handle);
}

sc_boolean driver_isActive(const Driver* handle)
{
	sc_boolean result = bool_false;
	int i;
	
	for(i = 0; i < DRIVER_MAX_ORTHOGONAL_STATES; i++)
	{
		result = result || handle->stateConfVector[i] != Driver_last_state;
	}
	
	return result;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean driver_isFinal(const Driver* handle)
{
   return bool_false;
}

void driver_raiseTimeEvent(Driver* handle, sc_eventid evid)
{
	if ( ((sc_intptr_t)evid) >= ((sc_intptr_t)&(handle->timeEvents))
		&&  ((sc_intptr_t)evid) < ((sc_intptr_t)&(handle->timeEvents)) + sizeof(DriverTimeEvents))
		{
		*(sc_boolean*)evid = bool_true;
		
		driver_runCycle(handle);
	}		
}

sc_boolean driver_isStateActive(const Driver* handle, DriverStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case Driver_main_region_Step1 :
			result = (sc_boolean) (handle->stateConfVector[SCVI_DRIVER_MAIN_REGION_STEP1] == Driver_main_region_Step1
			);
			break;
		case Driver_main_region_Step2 :
			result = (sc_boolean) (handle->stateConfVector[SCVI_DRIVER_MAIN_REGION_STEP2] == Driver_main_region_Step2
			);
			break;
		case Driver_main_region_Step3 :
			result = (sc_boolean) (handle->stateConfVector[SCVI_DRIVER_MAIN_REGION_STEP3] == Driver_main_region_Step3
			);
			break;
		default:
			result = bool_false;
			break;
	}
	return result;
}

static void clearInEvents(Driver* handle)
{
	handle->timeEvents.driver_main_region_Step1_tev0_raised = bool_false;
	handle->timeEvents.driver_main_region_Step2_tev0_raised = bool_false;
	handle->timeEvents.driver_main_region_Step3_tev0_raised = bool_false;
}

static void clearOutEvents(Driver* handle)
{
}



Directionblinker * driverIface_get_blinker(const Driver* handle)
{
	return handle->iface.blinker;
}
void driverIface_set_blinker(Driver* handle, Directionblinker * value)
{
	handle->iface.blinker = value;
}

/* implementations of all internal functions */

static sc_boolean check_main_region_Step1_tr0_tr0(const Driver* handle)
{
	return handle->timeEvents.driver_main_region_Step1_tev0_raised;
}

static sc_boolean check_main_region_Step2_tr0_tr0(const Driver* handle)
{
	return handle->timeEvents.driver_main_region_Step2_tev0_raised;
}

static sc_boolean check_main_region_Step3_tr0_tr0(const Driver* handle)
{
	return handle->timeEvents.driver_main_region_Step3_tev0_raised;
}

static void effect_main_region_Step1_tr0(Driver* handle)
{
	exseq_main_region_Step1(handle);
	enseq_main_region_Step2_default(handle);
}

static void effect_main_region_Step2_tr0(Driver* handle)
{
	exseq_main_region_Step2(handle);
	enseq_main_region_Step3_default(handle);
}

static void effect_main_region_Step3_tr0(Driver* handle)
{
	exseq_main_region_Step3(handle);
	enseq_main_region_Step1_default(handle);
}

/* Entry action for state 'Step1'. */
static void enact_main_region_Step1(Driver* handle)
{
	/* Entry action for state 'Step1'. */
	handle->ts_conn->ts_methods->start(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.driver_main_region_Step1_tev0_raised), 
										2 * 1000,
										bool_false);
}

/* Entry action for state 'Step2'. */
static void enact_main_region_Step2(Driver* handle)
{
	/* Entry action for state 'Step2'. */
	handle->ts_conn->ts_methods->start(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.driver_main_region_Step2_tev0_raised), 
										8 * 1000,
										bool_false);
	directionblinkerIface_raise_left(handle->iface.blinker);
}

/* Entry action for state 'Step3'. */
static void enact_main_region_Step3(Driver* handle)
{
	/* Entry action for state 'Step3'. */
	handle->ts_conn->ts_methods->start(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.driver_main_region_Step3_tev0_raised), 
										6 * 1000,
										bool_false);
	directionblinkerIface_raise_right(handle->iface.blinker);
}

/* Exit action for state 'Step1'. */
static void exact_main_region_Step1(Driver* handle)
{
	/* Exit action for state 'Step1'. */
	handle->ts_conn->ts_methods->stop(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.driver_main_region_Step1_tev0_raised));
}

/* Exit action for state 'Step2'. */
static void exact_main_region_Step2(Driver* handle)
{
	/* Exit action for state 'Step2'. */
	handle->ts_conn->ts_methods->stop(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.driver_main_region_Step2_tev0_raised));
}

/* Exit action for state 'Step3'. */
static void exact_main_region_Step3(Driver* handle)
{
	/* Exit action for state 'Step3'. */
	handle->ts_conn->ts_methods->stop(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.driver_main_region_Step3_tev0_raised));
}

/* 'default' enter sequence for state Step1 */
static void enseq_main_region_Step1_default(Driver* handle)
{
	/* 'default' enter sequence for state Step1 */
	enact_main_region_Step1(handle);
	handle->stateConfVector[0] = Driver_main_region_Step1;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Step2 */
static void enseq_main_region_Step2_default(Driver* handle)
{
	/* 'default' enter sequence for state Step2 */
	enact_main_region_Step2(handle);
	handle->stateConfVector[0] = Driver_main_region_Step2;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Step3 */
static void enseq_main_region_Step3_default(Driver* handle)
{
	/* 'default' enter sequence for state Step3 */
	enact_main_region_Step3(handle);
	handle->stateConfVector[0] = Driver_main_region_Step3;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
static void enseq_main_region_default(Driver* handle)
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default(handle);
}

/* Default exit sequence for state Step1 */
static void exseq_main_region_Step1(Driver* handle)
{
	/* Default exit sequence for state Step1 */
	handle->stateConfVector[0] = Driver_last_state;
	handle->stateConfVectorPosition = 0;
	exact_main_region_Step1(handle);
}

/* Default exit sequence for state Step2 */
static void exseq_main_region_Step2(Driver* handle)
{
	/* Default exit sequence for state Step2 */
	handle->stateConfVector[0] = Driver_last_state;
	handle->stateConfVectorPosition = 0;
	exact_main_region_Step2(handle);
}

/* Default exit sequence for state Step3 */
static void exseq_main_region_Step3(Driver* handle)
{
	/* Default exit sequence for state Step3 */
	handle->stateConfVector[0] = Driver_last_state;
	handle->stateConfVectorPosition = 0;
	exact_main_region_Step3(handle);
}

/* Default exit sequence for region main region */
static void exseq_main_region(Driver* handle)
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of driver.main_region) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case Driver_main_region_Step1 :
		{
			exseq_main_region_Step1(handle);
			break;
		}
		case Driver_main_region_Step2 :
		{
			exseq_main_region_Step2(handle);
			break;
		}
		case Driver_main_region_Step3 :
		{
			exseq_main_region_Step3(handle);
			break;
		}
		default: break;
	}
}

/* The reactions of state Step1. */
static void react_main_region_Step1(Driver* handle)
{
	/* The reactions of state Step1. */
	if (check_main_region_Step1_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_Step1_tr0(handle);
	} 
}

/* The reactions of state Step2. */
static void react_main_region_Step2(Driver* handle)
{
	/* The reactions of state Step2. */
	if (check_main_region_Step2_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_Step2_tr0(handle);
	} 
}

/* The reactions of state Step3. */
static void react_main_region_Step3(Driver* handle)
{
	/* The reactions of state Step3. */
	if (check_main_region_Step3_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_Step3_tr0(handle);
	} 
}

/* Default react sequence for initial entry  */
static void react_main_region__entry_Default(Driver* handle)
{
	/* Default react sequence for initial entry  */
	enseq_main_region_Step1_default(handle);
}


