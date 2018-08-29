package org.yakindu.sct.mynewt.generator.c.transformation

import com.google.inject.Inject
import org.yakindu.sct.generator.c.extensions.EventNaming
import org.yakindu.sct.generator.c.submodules.eventdriven.EventDrivenEventCode
import org.yakindu.sct.model.sexec.ExecutionFlow
import org.yakindu.sct.model.stext.stext.EventDefinition

class MyNewtEventCode extends EventDrivenEventCode {
	@Inject extension MyNewtNaming
	@Inject extension EventNaming
	
	override interfaceIncomingEventRaiserBody(ExecutionFlow it, EventDefinition event) {
		'''
			«inEventStructName» *e = os_memblock_get(handle->pool);
			e->osev.ev_queued = 0;
			e->osev.ev_arg = (void*) handle;
			e->osev.ev_cb = «event.asProcess»;
			e->osev.ev_next.stqe_next = 0;
			«IF event.hasValue»
				e->value.«event.eventValueUnionMemberName» = value;
			«ENDIF»
			os_eventq_put(handle->queue, (struct os_event*) e);
		'''
	}
	
	def processEventFunctionSignature(EventDefinition it) {
		'''void «asProcess»(struct os_event * osev)'''		
	}
	
	def processEventFunction(EventDefinition it) {
		'''
			«processEventFunctionSignature»
			{
				«processEventFunctionBody»
			}
		'''
	}
	
	def declareProcessEventFunction(EventDefinition it) {
		'''«processEventFunctionSignature»;'''
	}
	
	def processEventFunctionBody(EventDefinition it) {
		'''
			«flow.type» * handle = osev->ev_arg;
			«IF hasValue»
				«flow.inEventStructName» * e = («flow.inEventStructName» *) osev;
				«type.targetLanguageName» value = e->value.«eventValueUnionMemberName»;
			«ENDIF»
			«super.interfaceIncomingEventRaiserBody(flow, it)»
		'''
	}
	
	def setTimeEventFunction(ExecutionFlow it) {
		'''
			«setTimeEventFunctionSignature»
		'''
	}
	
	def setTimeEventFunctionSignature(ExecutionFlow it) {
		'''void «setTimerFctID»(«scHandleDecl», const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic);'''
	}
	
}
