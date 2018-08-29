package org.yakindu.sct.mynewt.generator.c.transformation

import com.google.inject.Inject
import org.yakindu.sct.generator.c.IGenArtifactConfigurations
import org.yakindu.sct.generator.c.IHeaderFragment
import org.yakindu.sct.model.sexec.ExecutionFlow
import org.yakindu.sct.model.sexec.extensions.SExecExtensions
import org.yakindu.sct.model.sgen.GeneratorEntry
import org.yakindu.sct.generator.core.types.ICodegenTypeSystemAccess
import org.yakindu.sct.generator.c.extensions.EventNaming

class MyNewtHeaderFragment implements IHeaderFragment {
	@Inject protected extension SExecExtensions
	@Inject protected extension ICodegenTypeSystemAccess
	@Inject protected extension MyNewtEventCode
	@Inject protected extension EventNaming
	@Inject protected extension MyNewtNaming
	@Inject protected extension MyNewtStatechartTypes
	
	override defines(ExecutionFlow it, GeneratorEntry entry, IGenArtifactConfigurations artifactConfigs) {
		'''
			#define «name.toUpperCase»_EVENT_POOL_BLOCK_SIZE sizeof(«inEventStructName»)
		'''
	}
	
	override fileComment(ExecutionFlow flow, GeneratorEntry entry, IGenArtifactConfigurations artifactConfigs) {
		''''''
	}
	
	override functions(ExecutionFlow it, GeneratorEntry entry, IGenArtifactConfigurations artifactConfigs) {
		'''
			«FOR event : incomingEvents»
				/*! Callback function of event «event.name» for the MyNewt event queue */
				extern «event.declareProcessEventFunction»
				
			«ENDFOR»
		'''
	}
	
	override includes(ExecutionFlow flow, GeneratorEntry entry, IGenArtifactConfigurations artifactConfigs) {
		'''
			#include "os/os_eventq.h"
			#include "os/os_mempool.h"
		'''
	}
	
	override types(ExecutionFlow it, GeneratorEntry entry, IGenArtifactConfigurations artifactConfigs) {
		'''
			«eventType»
		'''
	}
}
