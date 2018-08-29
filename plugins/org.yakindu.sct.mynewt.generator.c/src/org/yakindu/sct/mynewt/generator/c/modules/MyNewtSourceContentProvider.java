package org.yakindu.sct.mynewt.generator.c.modules;

import org.yakindu.sct.generator.c.SourceContentFragmentProvider;
import org.yakindu.sct.generator.c.files.StatemachineSource;
import org.yakindu.sct.mynewt.generator.c.transformation.MyNewtSourceFragment;


public class MyNewtSourceContentProvider extends SourceContentFragmentProvider {
	@Override
	public StatemachineSource get() {
		StatemachineSource source = super.get();
		source.getContentProviders().add(injector.getInstance(MyNewtSourceFragment.class));
		return source;
	}

}
