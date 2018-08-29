package org.yakindu.sct.mynewt.generator.c.transformation

import org.yakindu.sct.generator.c.extensions.Naming
import org.yakindu.sct.model.sexec.ExecutionFlow
import org.yakindu.sct.model.stext.stext.EventDefinition

class MyNewtNaming extends Naming {
	def asProcess(EventDefinition it) {
		accessFunction("process")
	}
	
	def inEventStructName(ExecutionFlow it) {
		'''«type»_in_event'''
	}
}
