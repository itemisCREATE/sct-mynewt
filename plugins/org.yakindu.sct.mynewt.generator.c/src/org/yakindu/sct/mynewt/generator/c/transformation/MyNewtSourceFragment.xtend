package org.yakindu.sct.mynewt.generator.c.transformation

import com.google.inject.Inject
import org.yakindu.sct.generator.c.IGenArtifactConfigurations
import org.yakindu.sct.generator.c.ISourceFragment
import org.yakindu.sct.model.sexec.ExecutionFlow
import org.yakindu.sct.model.sgen.GeneratorEntry
import org.yakindu.sct.model.sexec.extensions.SExecExtensions

class MyNewtSourceFragment implements ISourceFragment {
	@Inject protected extension SExecExtensions
	@Inject protected extension MyNewtEventCode
	
	override declarations(ExecutionFlow it, GeneratorEntry entry, IGenArtifactConfigurations artifactConfigs) {
		''''''
	}
	
	override fileComment(ExecutionFlow flow, GeneratorEntry entry, IGenArtifactConfigurations artifactConfigs) {
		''''''
	}
	
	override implementations(ExecutionFlow it, GeneratorEntry entry, IGenArtifactConfigurations artifactConfigs) {
		'''
			«FOR event : incomingEvents»
				«processEventFunction(event)»
				
			«ENDFOR»
		'''
	}
	
	override includes(ExecutionFlow flow, GeneratorEntry entry, IGenArtifactConfigurations artifactConfigs) {
		''''''
	}
	
}
