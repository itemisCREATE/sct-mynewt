package org.yakindu.sct.mynewt.generator.c.transformation

import com.google.inject.Inject
import org.yakindu.sct.generator.c.submodules.eventdriven.EventDrivenStatechartTypes
import org.yakindu.sct.model.sexec.ExecutionFlow

class MyNewtStatechartTypes extends EventDrivenStatechartTypes {
	@Inject protected extension MyNewtNaming
	
	override statemachineStructContent(ExecutionFlow it) {
		'''
			«super.statemachineStructContent(it)»
			«IF timed»
				sc_ts_connection_t *ts_conn;
			«ENDIF»
			struct os_eventq *queue;
			struct os_mempool *pool;
		'''
	}
	
	override valueUnionEvents(ExecutionFlow it) {
		super.valueUnionEvents(it) + incomingEvents.filter[hasValue]
	}
	
	def CharSequence eventType(ExecutionFlow it) {
		'''
			typedef struct {
				struct os_event osev;
				«IF hasIncomingEventsWithValue»
					«eventValueUnionName» value;
				«ENDIF»
			} «inEventStructName»;
		'''
	}	
	
}
