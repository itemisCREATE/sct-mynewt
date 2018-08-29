package org.yakindu.sct.mynewt.generator.c.modules;

import org.yakindu.sct.generator.c.HeaderContentFragmentProvider;
import org.yakindu.sct.generator.c.files.StatemachineHeader;
import org.yakindu.sct.mynewt.generator.c.transformation.MyNewtHeaderFragment;


public class MyNewtHeaderContentProvider extends HeaderContentFragmentProvider {
	@Override
	public StatemachineHeader get() {
		StatemachineHeader statemachineHeader = super.get();
		statemachineHeader.getContentProviders().add(injector.getInstance(MyNewtHeaderFragment.class));
		return statemachineHeader;
	}
}
