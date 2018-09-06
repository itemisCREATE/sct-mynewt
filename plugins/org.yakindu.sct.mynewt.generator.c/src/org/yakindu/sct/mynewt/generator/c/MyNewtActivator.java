package org.yakindu.sct.mynewt.generator.c;

import org.osgi.framework.BundleActivator;
import org.osgi.framework.BundleContext;

public class MyNewtActivator implements BundleActivator {

	private static BundleContext context;

	static BundleContext getContext() {
		return context;
	}

	public void start(BundleContext bundleContext) throws Exception {
		MyNewtActivator.context = bundleContext;
	}

	public void stop(BundleContext bundleContext) throws Exception {
		MyNewtActivator.context = null;
	}

}
