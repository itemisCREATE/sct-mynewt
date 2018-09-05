
#include "Blink.h"
#include "sc_types.h"

#include <stdlib.h>
#include <string.h>
/*! \file Implementation of the state machine 'blink'
*/

/* prototypes of all internal functions */
static sc_boolean check_mr_Off_tr0_tr0(const Blink* handle);
static sc_boolean check_mr_Blinking_tr0_tr0(const Blink* handle);
static sc_boolean check_mr_Blinking_r_Light_tr0_tr0(const Blink* handle);
static sc_boolean check_mr_Blinking_r_Dark_tr0_tr0(const Blink* handle);
static void effect_mr_Off_tr0(Blink* handle);
static void effect_mr_Blinking_tr0(Blink* handle);
static void effect_mr_Blinking_r_Light_tr0(Blink* handle);
static void effect_mr_Blinking_r_Dark_tr0(Blink* handle);
static void enact_mr_Off(Blink* handle);
static void enact_mr_Blinking_r_Light(Blink* handle);
static void enact_mr_Blinking_r_Dark(Blink* handle);
static void exact_mr_Blinking_r_Light(Blink* handle);
static void exact_mr_Blinking_r_Dark(Blink* handle);
static void enseq_mr_Off_default(Blink* handle);
static void enseq_mr_Blinking_default(Blink* handle);
static void enseq_mr_Blinking_r_Light_default(Blink* handle);
static void enseq_mr_Blinking_r_Dark_default(Blink* handle);
static void enseq_mr_default(Blink* handle);
static void enseq_mr_Blinking_r_default(Blink* handle);
static void exseq_mr_Off(Blink* handle);
static void exseq_mr_Blinking(Blink* handle);
static void exseq_mr_Blinking_r_Light(Blink* handle);
static void exseq_mr_Blinking_r_Dark(Blink* handle);
static void exseq_mr(Blink* handle);
static void exseq_mr_Blinking_r(Blink* handle);
static void react_mr_Off(Blink* handle);
static void react_mr_Blinking_r_Light(Blink* handle);
static void react_mr_Blinking_r_Dark(Blink* handle);
static void react_mr__entry_Default(Blink* handle);
static void react_mr_Blinking_r__entry_Default(Blink* handle);
static void clearInEvents(Blink* handle);
static void clearOutEvents(Blink* handle);


void blink_init(Blink* handle, sc_ts_connection_t *tsc, struct os_eventq *q, struct os_mempool *pool)
{
	handle->ts_conn = tsc;
	handle->queue = q;
	handle->pool = pool;
		sc_integer i;
	
		for (i = 0; i < BLINK_MAX_ORTHOGONAL_STATES; ++i)
		{
			handle->stateConfVector[i] = Blink_last_state;
		}
		
		
		handle->stateConfVectorPosition = 0;
	
		clearInEvents(handle);
		clearOutEvents(handle);
	
		/* Default init sequence for statechart blink */
		handle->iface.gpio = 0;
		handle->iface.period = 1000;
}

void blink_enter(Blink* handle)
{
	/* Default enter sequence for statechart blink */
	enseq_mr_default(handle);
}

void blink_runCycle(Blink* handle)
{
	clearOutEvents(handle);
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < BLINK_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++)
		{
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition])
		{
		case Blink_mr_Off:
		{
			react_mr_Off(handle);
			break;
		}
		case Blink_mr_Blinking_r_Light:
		{
			react_mr_Blinking_r_Light(handle);
			break;
		}
		case Blink_mr_Blinking_r_Dark:
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

void blink_exit(Blink* handle)
{
	/* Default exit sequence for statechart blink */
	exseq_mr(handle);
}

sc_boolean blink_isActive(const Blink* handle)
{
	sc_boolean result = bool_false;
	int i;
	
	for(i = 0; i < BLINK_MAX_ORTHOGONAL_STATES; i++)
	{
		result = result || handle->stateConfVector[i] != Blink_last_state;
	}
	
	return result;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean blink_isFinal(const Blink* handle)
{
   return bool_false;
}

void blink_raiseTimeEvent(Blink* handle, sc_eventid evid)
{
	if ( ((sc_intptr_t)evid) >= ((sc_intptr_t)&(handle->timeEvents))
		&&  ((sc_intptr_t)evid) < ((sc_intptr_t)&(handle->timeEvents)) + sizeof(BlinkTimeEvents))
		{
		*(sc_boolean*)evid = bool_true;
		
		blink_runCycle(handle);
	}		
}

sc_boolean blink_isStateActive(const Blink* handle, BlinkStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case Blink_mr_Off :
			result = (sc_boolean) (handle->stateConfVector[SCVI_BLINK_MR_OFF] == Blink_mr_Off
			);
			break;
		case Blink_mr_Blinking :
			result = (sc_boolean) (handle->stateConfVector[SCVI_BLINK_MR_BLINKING] >= Blink_mr_Blinking
				&& handle->stateConfVector[SCVI_BLINK_MR_BLINKING] <= Blink_mr_Blinking_r_Dark);
			break;
		case Blink_mr_Blinking_r_Light :
			result = (sc_boolean) (handle->stateConfVector[SCVI_BLINK_MR_BLINKING_R_LIGHT] == Blink_mr_Blinking_r_Light
			);
			break;
		case Blink_mr_Blinking_r_Dark :
			result = (sc_boolean) (handle->stateConfVector[SCVI_BLINK_MR_BLINKING_R_DARK] == Blink_mr_Blinking_r_Dark
			);
			break;
		default:
			result = bool_false;
			break;
	}
	return result;
}

static void clearInEvents(Blink* handle)
{
	handle->iface.on_raised = bool_false;
	handle->iface.off_raised = bool_false;
	handle->timeEvents.blink_mr_Blinking_r_Light_tev0_raised = bool_false;
	handle->timeEvents.blink_mr_Blinking_r_Dark_tev0_raised = bool_false;
}

static void clearOutEvents(Blink* handle)
{
}

void blinkIface_raise_on(Blink* handle)
{
	Blink_in_event *e = os_memblock_get(handle->pool);
	e->osev.ev_queued = 0;
	e->osev.ev_arg = (void*) handle;
	e->osev.ev_cb = blinkIface_process_on;
	e->osev.ev_next.stqe_next = 0;
	os_eventq_put(handle->queue, (struct os_event*) e);
}
void blinkIface_raise_off(Blink* handle)
{
	Blink_in_event *e = os_memblock_get(handle->pool);
	e->osev.ev_queued = 0;
	e->osev.ev_arg = (void*) handle;
	e->osev.ev_cb = blinkIface_process_off;
	e->osev.ev_next.stqe_next = 0;
	os_eventq_put(handle->queue, (struct os_event*) e);
}


int32_t blinkIface_get_gpio(const Blink* handle)
{
	return handle->iface.gpio;
}
void blinkIface_set_gpio(Blink* handle, int32_t value)
{
	handle->iface.gpio = value;
}
uint32_t blinkIface_get_period(const Blink* handle)
{
	return handle->iface.period;
}
void blinkIface_set_period(Blink* handle, uint32_t value)
{
	handle->iface.period = value;
}

/* implementations of all internal functions */

static sc_boolean check_mr_Off_tr0_tr0(const Blink* handle)
{
	return handle->iface.on_raised;
}

static sc_boolean check_mr_Blinking_tr0_tr0(const Blink* handle)
{
	return handle->iface.off_raised;
}

static sc_boolean check_mr_Blinking_r_Light_tr0_tr0(const Blink* handle)
{
	return handle->timeEvents.blink_mr_Blinking_r_Light_tev0_raised;
}

static sc_boolean check_mr_Blinking_r_Dark_tr0_tr0(const Blink* handle)
{
	return handle->timeEvents.blink_mr_Blinking_r_Dark_tev0_raised;
}

static void effect_mr_Off_tr0(Blink* handle)
{
	exseq_mr_Off(handle);
	enseq_mr_Blinking_default(handle);
}

static void effect_mr_Blinking_tr0(Blink* handle)
{
	exseq_mr_Blinking(handle);
	enseq_mr_Off_default(handle);
}

static void effect_mr_Blinking_r_Light_tr0(Blink* handle)
{
	exseq_mr_Blinking_r_Light(handle);
	enseq_mr_Blinking_r_Dark_default(handle);
}

static void effect_mr_Blinking_r_Dark_tr0(Blink* handle)
{
	exseq_mr_Blinking_r_Dark(handle);
	enseq_mr_Blinking_r_Light_default(handle);
}

/* Entry action for state 'Off'. */
static void enact_mr_Off(Blink* handle)
{
	/* Entry action for state 'Off'. */
	hal_gpio_write(handle->iface.gpio, 0);
}

/* Entry action for state 'Light'. */
static void enact_mr_Blinking_r_Light(Blink* handle)
{
	/* Entry action for state 'Light'. */
	handle->ts_conn->ts_methods->start(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.blink_mr_Blinking_r_Light_tev0_raised), 
										handle->iface.period,
										bool_false);
	hal_gpio_write(handle->iface.gpio, 1);
}

/* Entry action for state 'Dark'. */
static void enact_mr_Blinking_r_Dark(Blink* handle)
{
	/* Entry action for state 'Dark'. */
	handle->ts_conn->ts_methods->start(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.blink_mr_Blinking_r_Dark_tev0_raised), 
										handle->iface.period,
										bool_false);
	hal_gpio_write(handle->iface.gpio, 0);
}

/* Exit action for state 'Light'. */
static void exact_mr_Blinking_r_Light(Blink* handle)
{
	/* Exit action for state 'Light'. */
	handle->ts_conn->ts_methods->stop(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.blink_mr_Blinking_r_Light_tev0_raised));
}

/* Exit action for state 'Dark'. */
static void exact_mr_Blinking_r_Dark(Blink* handle)
{
	/* Exit action for state 'Dark'. */
	handle->ts_conn->ts_methods->stop(handle->ts_conn, 
										(sc_eventid) &(handle->timeEvents.blink_mr_Blinking_r_Dark_tev0_raised));
}

/* 'default' enter sequence for state Off */
static void enseq_mr_Off_default(Blink* handle)
{
	/* 'default' enter sequence for state Off */
	enact_mr_Off(handle);
	handle->stateConfVector[0] = Blink_mr_Off;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Blinking */
static void enseq_mr_Blinking_default(Blink* handle)
{
	/* 'default' enter sequence for state Blinking */
	enseq_mr_Blinking_r_default(handle);
}

/* 'default' enter sequence for state Light */
static void enseq_mr_Blinking_r_Light_default(Blink* handle)
{
	/* 'default' enter sequence for state Light */
	enact_mr_Blinking_r_Light(handle);
	handle->stateConfVector[0] = Blink_mr_Blinking_r_Light;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Dark */
static void enseq_mr_Blinking_r_Dark_default(Blink* handle)
{
	/* 'default' enter sequence for state Dark */
	enact_mr_Blinking_r_Dark(handle);
	handle->stateConfVector[0] = Blink_mr_Blinking_r_Dark;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region mr */
static void enseq_mr_default(Blink* handle)
{
	/* 'default' enter sequence for region mr */
	react_mr__entry_Default(handle);
}

/* 'default' enter sequence for region r */
static void enseq_mr_Blinking_r_default(Blink* handle)
{
	/* 'default' enter sequence for region r */
	react_mr_Blinking_r__entry_Default(handle);
}

/* Default exit sequence for state Off */
static void exseq_mr_Off(Blink* handle)
{
	/* Default exit sequence for state Off */
	handle->stateConfVector[0] = Blink_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state Blinking */
static void exseq_mr_Blinking(Blink* handle)
{
	/* Default exit sequence for state Blinking */
	exseq_mr_Blinking_r(handle);
}

/* Default exit sequence for state Light */
static void exseq_mr_Blinking_r_Light(Blink* handle)
{
	/* Default exit sequence for state Light */
	handle->stateConfVector[0] = Blink_last_state;
	handle->stateConfVectorPosition = 0;
	exact_mr_Blinking_r_Light(handle);
}

/* Default exit sequence for state Dark */
static void exseq_mr_Blinking_r_Dark(Blink* handle)
{
	/* Default exit sequence for state Dark */
	handle->stateConfVector[0] = Blink_last_state;
	handle->stateConfVectorPosition = 0;
	exact_mr_Blinking_r_Dark(handle);
}

/* Default exit sequence for region mr */
static void exseq_mr(Blink* handle)
{
	/* Default exit sequence for region mr */
	/* Handle exit of all possible states (of blink.mr) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case Blink_mr_Off :
		{
			exseq_mr_Off(handle);
			break;
		}
		case Blink_mr_Blinking_r_Light :
		{
			exseq_mr_Blinking_r_Light(handle);
			break;
		}
		case Blink_mr_Blinking_r_Dark :
		{
			exseq_mr_Blinking_r_Dark(handle);
			break;
		}
		default: break;
	}
}

/* Default exit sequence for region r */
static void exseq_mr_Blinking_r(Blink* handle)
{
	/* Default exit sequence for region r */
	/* Handle exit of all possible states (of blink.mr.Blinking.r) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case Blink_mr_Blinking_r_Light :
		{
			exseq_mr_Blinking_r_Light(handle);
			break;
		}
		case Blink_mr_Blinking_r_Dark :
		{
			exseq_mr_Blinking_r_Dark(handle);
			break;
		}
		default: break;
	}
}

/* The reactions of state Off. */
static void react_mr_Off(Blink* handle)
{
	/* The reactions of state Off. */
	if (check_mr_Off_tr0_tr0(handle) == bool_true)
	{ 
		effect_mr_Off_tr0(handle);
	} 
}

/* The reactions of state Light. */
static void react_mr_Blinking_r_Light(Blink* handle)
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
static void react_mr_Blinking_r_Dark(Blink* handle)
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
static void react_mr__entry_Default(Blink* handle)
{
	/* Default react sequence for initial entry  */
	enseq_mr_Off_default(handle);
}

/* Default react sequence for initial entry  */
static void react_mr_Blinking_r__entry_Default(Blink* handle)
{
	/* Default react sequence for initial entry  */
	enseq_mr_Blinking_r_Light_default(handle);
}


void blinkIface_process_on(struct os_event * osev)
{
	Blink * handle = osev->ev_arg;
	handle->iface.on_raised = bool_true;
	
	blink_runCycle(handle);
	os_memblock_put(handle->pool, osev);
}

void blinkIface_process_off(struct os_event * osev)
{
	Blink * handle = osev->ev_arg;
	handle->iface.off_raised = bool_true;
	
	blink_runCycle(handle);
	os_memblock_put(handle->pool, osev);
}

