package org.yakindu.sct.mynewt.generator.c.modules;

import org.yakindu.sct.generator.c.CCodeGeneratorModule;
import org.yakindu.sct.model.sgen.GeneratorEntry;

import com.google.inject.Module;
import com.google.inject.util.Modules;

public class MyNewtCodeGeneratorModule extends CCodeGeneratorModule {
	@Override
	protected Module getModule(GeneratorEntry entry) {
		return Modules.override(super.getModule(entry)).with(moduleAdapter(new MyNewtCodeGeneratorBaseModule(), entry));
	}
}
