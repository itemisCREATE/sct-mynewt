
#include "Blinklight.h"
#include "../smlib/sc_types.h"

#include <stdlib.h>
#include <string.h>
/*! \file Implementation of the state machine 'blinklight'
*/

/* prototypes of all internal functions */
static sc_boolean check_mr_Off_tr0_tr0(const Blinklight* handle);
static sc_boolean check_mr_Blinking_tr0_tr0(const Blinklight* handle);
static sc_boolean check_mr_Blinking_r_Light_tr0_tr0(const Blinklight* handle);
static sc_boolean check_mr_Blinking_r_Dark_tr0_tr0(const Blinklight* handle);
static void effect_mr_Off_tr0(Blinklight* handle);
static void effect_mr_Blinking_tr0(Blinklight* handle);
static void effect_mr_Blinking_r_Light_tr0(Blinklight* handle);
static void effect_mr_Blinking_r_Dark_tr0(Blinklight* handle);
static void enact_mr_Off(Blinklight* handle);
static void enact_mr_Blinking_r_Light(Blinklight* handle);
static void enact_mr_Blinking_r_Dark(Blinklight* handle);
static void exact_mr_Blinking_r_Light(Blinklight* handle);
static void exact_mr_Blinking_r_Dark(Blinklight* handle);
static void enseq_mr_Off_default(Blinklight* handle);
static void enseq_mr_Blinking_default(Blinklight* handle);
static void enseq_mr_Blinking_r_Light_default(Blinklight* handle);
static void enseq_mr_Blinking_r_Dark_default(Blinklight* handle);
static void enseq_mr_default(Blinklight* handle);
static void enseq_mr_Blinking_r_default(Blinklight* handle);
static void exseq_mr_Off(Blinklight* handle);
static void exseq_mr_Blinking(Blinklight* handle);
static void exseq_mr_Blinking_r_Light(Blinklight* handle);
static void exseq_mr_Blinking_r_Dark(Blinklight* handle);
static void exseq_mr(Blinklight* handle);
static void exseq_mr_Blinking_r(Blinklight* handle);
static void react_mr_Off(Blinklight* handle);
static void react_mr_Blinking_r_Light(Blinklight* handle);
static void react_mr_Blinking_r_Dark(Blinklight* handle);
static void react_mr__entry_Default(Blinklight* handle);
static void react_mr_Blinking_r__entry_Default(Blinklight* handle);
static void clearInEvents(Blinklight* handle);
static void clearOutEvents(Blinklight* handle);


void blinklight_init(Blinklight* handle, sc_ts_connection_t *tsc, struct os_eventq *q, struct os_mempool *pool)
{
	handle->ts_conn = tsc;
	handle->queue = q;
	handle->pool = pool;
		sc_integer i;
	
		for (i = 0; i < BLINKLIGHT_MAX_ORTHOGONAL_STATES; ++i)
		{
			handle->stateConfVector[i] = Blinklight_last_state;
		}
		
		
		handle->stateConfVectorPosition = 0;
	
		clearInEvents(handle);
		clearOutEvents(handle);
	
		/* Default init sequence for statechart blinklight */
		handle->iface.gpio = 0;
		handle->iface.period = 1000;
}

void blinklight_enter(Blinklight* handle)
{
	/* Default enter sequence for statechart blinklight */
	enseq_mr_default(handle);
}

void blinklight_runCycle(Blinklight* handle)
{
	clearOutEvents(handle);
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < BLINKLIGHT_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++)
		{
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition])
		{
		case Blinklight_mr_Off:
		{
			react_mr_Off(handle);
			break;
		}
		case Blinklight_mr_Blinking_r_Light:
		{
			react_mr_Blinking_r_Light(handle);
			break;
		}
		case Blinklight_mr_Blinking_r_Dark:
		{
			react_mr_Blinking_r_Dark(handle);
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents(handle);
}

void blinklight_exit(Blinklight* handle)
{
	/* Default exit sequence for statechart blinklight */
	exseq_mr(handle);
}

sc_boolean blinklight_isActive(const Blinklight* handle)
{
	sc_boolean result = bool_false;
	int i;
	
	for(i = 0; i < BLINKLIGHT_MAX_ORTHOGONAL_STATES; i++)
	{
		result = result || handle->stateConfVector[i] != Blinklight_last_state;
	}
	
	return result;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean blinklight_isFinal(const Blinklight* handle)
{
   return bool_false;
}

void blinklight_raiseTimeEvent(Blinklight* handle, sc_eventid evid)
{
	if ( ((sc_intptr_t)evid) >= ((sc_intptr_t)&(handle->timeEvents))
		&&  ((sc_intptr_t)evid) < ((sc_intptr_t)&(handle->timeEvents)) + sizeof(BlinklightTimeEvents))
		{
		*(sc_boolean*)evid = bool_true;
		
		blinklight_runCycle(handle);
	}		
}

sc_boolean blinklight_isStateActive(const Blinklight* handle, BlinklightStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case Blinklight_mr_Off :
			result = (sc_boolean) (handle->stateConfVector[SCVI_BLINKLIGHT_MR_OFF] == Blinklight_mr_Off
			);
			break;
		case Blinklight_mr_Blinking :
			result = (sc_boolean) (handle->stateConfVector[SCVI_BLINKLIGHT_MR_BLINKING] >= Blinklight_mr_Blinking
				&& handle->stateConfVector[SCVI_BLINKLIGHT_MR_BLINKING] <= Blinklight_mr_Blinking_r_Dark);
			break;
		case Blinklight_mr_Blinking_r_Light :
			result = (sc_boolean) (handle->stateConfVector[SCVI_BLINKLIGHT_MR_BLINKING_R_LIGHT] == Blinklight_mr_Blinking_r_Light
			);
			break;
		case Blinklight_mr_Blinking_r_Dark :
			result = (sc_boolean) (handle->stateConfVector[SCVI_BLINKLIGHT_MR_BLINKING_R_DARK] == Blinklight_mr_Blinking_r_Dark
			);
			break;
		default:
			result = bool_false;
			break;
	}
	return result;
}

static void clearInEvents(Blinklight* handle)
{
	handle->iface.on_raised = bool_false;
	handle->iface.off_raised = bool_false;
	handle->timeEvents.blinklight_mr_Blinking_r_Light_tev0_raised = bool_false;
	handle->timeEvents.blinklight_mr_Blinking_r_Dark_tev0_raised = bool_false;
}

static void clearOutEvents(Blinklight* handle)
{
}

void blinklightIface_raise_on(Blinklight* handle)
{
	Blinklight_in_event *e = os_memblock_get(handle->pool);
	e->osev.ev_queued = 0;
	e->osev.ev_arg = (void*) handle;
	e->osev.ev_cb = blinklightIface_process_on;
	e->osev.ev_next.stqe_next = 0;
	os_eventq_put(handle->queue, (struct os_event*) e);
}
void blinklightIface_raise_off(Blinklight* handle)
{
	Blinklight_in_event *e = os_memblock_get(handle->pool);
	e->osev.ev_queued = 0;
	e->osev.ev_arg = (void*) handle;
	e->osev.ev_cb = blinklightIface_process_off;
	e->osev.ev_next.stqe_next = 0;
	os_eventq_put(handle->queue, (struct os_event*) e);
}


int32_t blinklightIface_get_gpio(const Blinklight* handle)
{
	return handle->iface.gpio;
}
void blinklightIface_set_gpio(Blinklight* handle, int32_t value)
{
	handle->iface.gpio = value;
}
uint32_t blinklightIface_get_period(const Blinklight* handle)
{
	return handle->iface.period;
}
void blinklightIface_set_period(Blinklight* handle, uint32_t value)
{
	handle->iface.period = value;
}

/* implementations of all internal functions */

static sc_boolean check_mr_Off_tr0_tr0(const Blinklight* handle)
{
	return handle->iface.on_raised;
}

static sc_boolean check_mr_Blinking_tr0_tr0(const Blinklight* handle)
{
	return handle->iface.off_raised;
}

static sc_boolean check_mr_Blinking_r_Light_tr0_tr0(const Blinklight* handle)
{
	return handle->timeEvents.blinklight_mr_Blinking_r_Light_tev0_raised;
}

static sc_boolean check_mr_Blinking_r_Dark_tr0_tr0(const Blinklight* handle)
{
	return handle->timeEvents.blinklight_mr_Blinking_r_Dark_tev0_raised;
}

static void effect_mr_Off_tr0(Blinklight* handle)
{
	exseq_mr_Off(handle);
	enseq_mr_Blinking_default(handle);
}

static void effect_mr_Blinking_tr0(Blinklight* handle)
{
	exseq_mr_Blinking(handle);
	enseq_mr_Off_default(handle);
}

static void effect_mr_Blinking_r_Light_tr0(Blinklight* handle)
{
	exseq_mr_Blinking_r_Light(handle);
	enseq_mr_Blinking_r_Dark_default(handle);
}

static void effect_mr_Blinking_r_Dark_tr0(Blinklight* handle)
{
	exseq_mr_Blinking_r_Dark(handle);
	enseq_mr_Blinking_r_Light_default(handle);
}

/* Entry action for state 'Off'. */
static void enact_mr_Off(Blinklight* handle)
{
	/* Entry action for state 'Off'. */
	hal_gpio_write(handle->iface.gpio, 0);
}

/* Entry action for state 'Light'. */
static void enact_mr_Blinking_r_Light(Blinklight* handle)
{
	/* Entry action for state 'Light'. */
	handle->ts_conn->ts_methods->start(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.blinklight_mr_Blinking_r_Light_tev0_raised), 
										handle->iface.period / 2,
										bool_false);
	hal_gpio_write(handle->iface.gpio, 1);
}

/* Entry action for state 'Dark'. */
static void enact_mr_Blinking_r_Dark(Blinklight* handle)
{
	/* Entry action for state 'Dark'. */
	handle->ts_conn->ts_methods->start(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.blinklight_mr_Blinking_r_Dark_tev0_raised), 
										handle->iface.period / 2,
										bool_false);
	hal_gpio_write(handle->iface.gpio, 0);
}

/* Exit action for state 'Light'. */
static void exact_mr_Blinking_r_Light(Blinklight* handle)
{
	/* Exit action for state 'Light'. */
	handle->ts_conn->ts_methods->stop(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.blinklight_mr_Blinking_r_Light_tev0_raised));
}

/* Exit action for state 'Dark'. */
static void exact_mr_Blinking_r_Dark(Blinklight* handle)
{
	/* Exit action for state 'Dark'. */
	handle->ts_conn->ts_methods->stop(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.blinklight_mr_Blinking_r_Dark_tev0_raised));
}

/* 'default' enter sequence for state Off */
static void enseq_mr_Off_default(Blinklight* handle)
{
	/* 'default' enter sequence for state Off */
	enact_mr_Off(handle);
	handle->stateConfVector[0] = Blinklight_mr_Off;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Blinking */
static void enseq_mr_Blinking_default(Blinklight* handle)
{
	/* 'default' enter sequence for state Blinking */
	enseq_mr_Blinking_r_default(handle);
}

/* 'default' enter sequence for state Light */
static void enseq_mr_Blinking_r_Light_default(Blinklight* handle)
{
	/* 'default' enter sequence for state Light */
	enact_mr_Blinking_r_Light(handle);
	handle->stateConfVector[0] = Blinklight_mr_Blinking_r_Light;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Dark */
static void enseq_mr_Blinking_r_Dark_default(Blinklight* handle)
{
	/* 'default' enter sequence for state Dark */
	enact_mr_Blinking_r_Dark(handle);
	handle->stateConfVector[0] = Blinklight_mr_Blinking_r_Dark;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region mr */
static void enseq_mr_default(Blinklight* handle)
{
	/* 'default' enter sequence for region mr */
	react_mr__entry_Default(handle);
}

/* 'default' enter sequence for region r */
static void enseq_mr_Blinking_r_default(Blinklight* handle)
{
	/* 'default' enter sequence for region r */
	react_mr_Blinking_r__entry_Default(handle);
}

/* Default exit sequence for state Off */
static void exseq_mr_Off(Blinklight* handle)
{
	/* Default exit sequence for state Off */
	handle->stateConfVector[0] = Blinklight_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state Blinking */
static void exseq_mr_Blinking(Blinklight* handle)
{
	/* Default exit sequence for state Blinking */
	exseq_mr_Blinking_r(handle);
}

/* Default exit sequence for state Light */
static void exseq_mr_Blinking_r_Light(Blinklight* handle)
{
	/* Default exit sequence for state Light */
	handle->stateConfVector[0] = Blinklight_last_state;
	handle->stateConfVectorPosition = 0;
	exact_mr_Blinking_r_Light(handle);
}

/* Default exit sequence for state Dark */
static void exseq_mr_Blinking_r_Dark(Blinklight* handle)
{
	/* Default exit sequence for state Dark */
	handle->stateConfVector[0] = Blinklight_last_state;
	handle->stateConfVectorPosition = 0;
	exact_mr_Blinking_r_Dark(handle);
}

/* Default exit sequence for region mr */
static void exseq_mr(Blinklight* handle)
{
	/* Default exit sequence for region mr */
	/* Handle exit of all possible states (of blinklight.mr) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case Blinklight_mr_Off :
		{
			exseq_mr_Off(handle);
			break;
		}
		case Blinklight_mr_Blinking_r_Light :
		{
			exseq_mr_Blinking_r_Light(handle);
			break;
		}
		case Blinklight_mr_Blinking_r_Dark :
		{
			exseq_mr_Blinking_r_Dark(handle);
			break;
		}
		default: break;
	}
}

/* Default exit sequence for region r */
static void exseq_mr_Blinking_r(Blinklight* handle)
{
	/* Default exit sequence for region r */
	/* Handle exit of all possible states (of blinklight.mr.Blinking.r) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case Blinklight_mr_Blinking_r_Light :
		{
			exseq_mr_Blinking_r_Light(handle);
			break;
		}
		case Blinklight_mr_Blinking_r_Dark :
		{
			exseq_mr_Blinking_r_Dark(handle);
			break;
		}
		default: break;
	}
}

/* The reactions of state Off. */
static void react_mr_Off(Blinklight* handle)
{
	/* The reactions of state Off. */
	if (check_mr_Off_tr0_tr0(handle) == bool_true)
	{ 
		effect_mr_Off_tr0(handle);
	} 
}

/* The reactions of state Light. */
static void react_mr_Blinking_r_Light(Blinklight* handle)
{
	/* The reactions of state Light. */
	if (check_mr_Blinking_r_Light_tr0_tr0(handle) == bool_true)
	{ 
		effect_mr_Blinking_r_Light_tr0(handle);
	}  else
	{
		if (check_mr_Blinking_tr0_tr0(handle) == bool_true)
		{ 
			effect_mr_Blinking_tr0(handle);
		} 
	}
}

/* The reactions of state Dark. */
static void react_mr_Blinking_r_Dark(Blinklight* handle)
{
	/* The reactions of state Dark. */
	if (check_mr_Blinking_r_Dark_tr0_tr0(handle) == bool_true)
	{ 
		effect_mr_Blinking_r_Dark_tr0(handle);
	}  else
	{
		if (check_mr_Blinking_tr0_tr0(handle) == bool_true)
		{ 
			effect_mr_Blinking_tr0(handle);
		} 
	}
}

/* Default react sequence for initial entry  */
static void react_mr__entry_Default(Blinklight* handle)
{
	/* Default react sequence for initial entry  */
	enseq_mr_Off_default(handle);
}

/* Default react sequence for initial entry  */
static void react_mr_Blinking_r__entry_Default(Blinklight* handle)
{
	/* Default react sequence for initial entry  */
	enseq_mr_Blinking_r_Light_default(handle);
}


void blinklightIface_process_on(struct os_event * osev)
{
	Blinklight * handle = osev->ev_arg;
	handle->iface.on_raised = bool_true;
	
	blinklight_runCycle(handle);
	os_memblock_put(handle->pool, osev);
}

void blinklightIface_process_off(struct os_event * osev)
{
	Blinklight * handle = osev->ev_arg;
	handle->iface.off_raised = bool_true;
	
	blinklight_runCycle(handle);
	os_memblock_put(handle->pool, osev);
}

