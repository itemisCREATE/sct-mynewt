package org.yakindu.sct.mynewt.generator.c.modules;

import org.yakindu.sct.generator.c.files.StatemachineHeader;
import org.yakindu.sct.generator.c.files.StatemachineSource;
import org.yakindu.sct.generator.c.submodules.APIGenerator;
import org.yakindu.sct.generator.c.submodules.EventCode;
import org.yakindu.sct.generator.c.submodules.StatechartTypes;
import org.yakindu.sct.generator.c.submodules.eventdriven.EventDrivenAPIGenerator;
import org.yakindu.sct.generator.c.submodules.eventdriven.EventDrivenEventCode;
import org.yakindu.sct.generator.c.submodules.eventdriven.EventDrivenStatechartTypes;
import org.yakindu.sct.generator.core.IGeneratorModule;
import org.yakindu.sct.model.sgen.GeneratorEntry;
import org.yakindu.sct.mynewt.generator.c.transformation.MyNewtAPIGenerator;
import org.yakindu.sct.mynewt.generator.c.transformation.MyNewtEventCode;
import org.yakindu.sct.mynewt.generator.c.transformation.MyNewtStatechartTypes;

import com.google.inject.Binder;

public class MyNewtCodeGeneratorBaseModule implements IGeneratorModule {

	@Override
	public void configure(GeneratorEntry entry, Binder binder) {
		binder.bind(StatemachineHeader.class).toProvider(MyNewtHeaderContentProvider.class);
		binder.bind(StatemachineSource.class).toProvider(MyNewtSourceContentProvider.class);
		binder.bind(EventCode.class).to(MyNewtEventCode.class);
		binder.bind(APIGenerator.class).to(MyNewtAPIGenerator.class);
		binder.bind(EventCode.class).to(MyNewtEventCode.class);
		binder.bind(StatechartTypes.class).to(MyNewtStatechartTypes.class);
		binder.bind(EventDrivenAPIGenerator.class).to(MyNewtAPIGenerator.class);
		binder.bind(EventDrivenEventCode.class).to(MyNewtEventCode.class);
		binder.bind(EventDrivenStatechartTypes.class).to(MyNewtStatechartTypes.class);
	}

}
