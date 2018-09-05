package org.yakindu.sct.mynewt.generator.c.transformation

import org.yakindu.sct.generator.c.submodules.StatemachineSourceFragment
import org.yakindu.sct.model.sexec.ExecutionFlow
import org.yakindu.sct.model.sgen.GeneratorEntry
import org.yakindu.sct.generator.c.IGenArtifactConfigurations

class MyNewtStatemachineSourceFragment extends StatemachineSourceFragment {
	
	
	/**
	 * TODO: This code should not be required. Make check if the module.clinet (required header) is required more generic.
	 */
	override CharSequence includes(ExecutionFlow it, GeneratorEntry entry, extension IGenArtifactConfigurations artifactConfigs) 
	'''
		#include "«(module.h).relativeTo(module.c)»"
		#include "«(typesModule.h).relativeTo(module.c)»"
		«IF !it.operations.empty»
			#include "«(module.client.h).relativeTo(module.c)»"
		«ENDIF»
		
		#include <stdlib.h>
		#include <string.h>
		«IF modOnReal»#include <math.h>«ENDIF»
		/*! \file Implementation of the state machine '«name»'
		*/
	'''
	
	
}