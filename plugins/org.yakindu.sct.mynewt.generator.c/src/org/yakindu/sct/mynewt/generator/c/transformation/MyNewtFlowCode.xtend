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

import org.yakindu.sct.generator.c.FlowCode
import org.yakindu.sct.model.sexec.ScheduleTimeEvent
import org.yakindu.sct.model.sexec.UnscheduleTimeEvent
import com.google.inject.Inject
import org.yakindu.sct.generator.c.extensions.Naming
import org.yakindu.sct.model.sexec.naming.INamingService
import org.yakindu.sct.generator.c.CExpressionsGenerator

/**
 * Use timer service connection for starting and stopping timers instead of dedicated timer call backs.
 * 
 * @author Axel Terfloth
 */
class MyNewtFlowCode extends FlowCode {

	@Inject extension Naming
	@Inject extension INamingService
	@Inject extension CExpressionsGenerator
	

	override dispatch CharSequence code(ScheduleTimeEvent it) '''
		«stepComment»
		«scHandle»->ts_conn->ts_methods->start(«scHandle»->ts_conn, 
											(sc_eventid) &(«scHandle»->timeEvents.«timeEvent.shortName»_raised), 
											«it.timeValue.code»,
											«IF timeEvent.periodic»bool_true«ELSE»bool_false«ENDIF»);
	'''

	override dispatch CharSequence code(UnscheduleTimeEvent it) '''
		«stepComment»
		«scHandle»->ts_conn->ts_methods->stop(«scHandle»->ts_conn, 
											(sc_eventid) &(«scHandle»->timeEvents.«timeEvent.shortName»_raised));
	'''
	
	
}