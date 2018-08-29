package org.yakindu.sct.mynewt.generator.c.transformation

import org.yakindu.sct.generator.c.submodules.eventdriven.EventDrivenAPIGenerator
import org.yakindu.sct.model.sexec.ExecutionFlow

class MyNewtAPIGenerator extends EventDrivenAPIGenerator {
	
	override protected initSignature(ExecutionFlow it) {
		'''void «initFctID»(«scHandleDecl», struct os_eventq *q, struct os_mempool *pool)'''
	}
	
	override protected initFunctionBody(ExecutionFlow it) {
		'''
			handle->queue = q;
			handle->pool = pool;
			«super.initFunctionBody(it)»
		'''
		
	}
	
}
