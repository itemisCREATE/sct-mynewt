/**
 * Copyright (c) 2018 committers of YAKINDU and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * 	committers of YAKINDU - initial API and implementation
 * 
 */
package org.yakindu.sct.mynewt.generator.c.transformation

import org.yakindu.sct.generator.c.CGenerator
import org.yakindu.sct.model.sexec.ExecutionFlow
import org.yakindu.sct.model.sgen.GeneratorEntry
import org.yakindu.sct.generator.c.IGenArtifactConfigurations
import com.google.inject.Inject
import org.yakindu.sct.generator.c.files.Types
import org.yakindu.sct.generator.c.files.StatemachineHeader
import org.yakindu.sct.generator.c.files.StatemachineSource
import org.yakindu.sct.generator.c.files.StatemachineRequiredHeader
import org.yakindu.sct.model.sexec.extensions.SExecExtensions
import org.yakindu.sct.generator.c.extensions.Naming
import org.yakindu.sct.generator.core.library.ICoreLibraryHelper
import org.yakindu.sct.generator.c.extensions.GenmodelEntries
import org.yakindu.sct.mynewt.generator.c.files.TimerServiceHeader

/**
 * Configure required generation artifacts for Apache Mynewt C generator.
 * 
 * @author Axel Terfloth
 */
class MyNewtCGenerator extends CGenerator {
	
	@Inject extension Types types
	@Inject extension TimerServiceHeader timerService
	@Inject extension StatemachineHeader statemachineHeader
	@Inject extension StatemachineSource statemachineSource
	@Inject extension StatemachineRequiredHeader statemachineRequiredHeader
	@Inject extension SExecExtensions
	@Inject extension GenmodelEntries
	@Inject extension Naming
	@Inject extension ICoreLibraryHelper
	
	
	
	
	override protected initGenerationArtifacts(ExecutionFlow it, GeneratorEntry entry,
		IGenArtifactConfigurations locations) {
		locations.configure(flow.typesModule.h, entry.libraryOutput, types, getSkipLibraryFiles(entry))

		if(timed) {
			locations.configure(timerService.module.h, entry.libraryOutput, timerService, getSkipLibraryFiles(entry))
		}

		locations.configure(flow.module.h, entry.headerOutput, statemachineHeader)
		locations.configure(flow.module.c, entry.sourceOutput, statemachineSource)
		if (!flow.operations.empty || entry.tracingEnterState || entry.tracingExitState) {
			locations.configure(flow.module.client.h, entry.headerOutput, statemachineRequiredHeader)
		}
	}
	
}