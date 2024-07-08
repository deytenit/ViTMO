package info.kgeorgiy.ja.eremin.i18n.test;

import java.io.PrintWriter;
import org.junit.platform.engine.discovery.ClassNameFilter;
import org.junit.platform.engine.discovery.DiscoverySelectors;
import org.junit.platform.launcher.*;
import org.junit.platform.launcher.core.LauncherDiscoveryRequestBuilder;
import org.junit.platform.launcher.core.LauncherFactory;
import org.junit.platform.launcher.listeners.SummaryGeneratingListener;
import org.junit.platform.launcher.listeners.TestExecutionSummary;

/**
 * Test launcher for all tests in the package.
 *
 * <p>Usage: java -cp <classpath> info.kgeorgiy.ja.eremin.i18n.test.Tester
 *
 * @author Vladimir Eremin
 */
public class Tester {
  /**
   * Main method for launching tests.
   *
   * @param args command line arguments (ignored)
   */
  public static void main(String[] args) {
    LauncherDiscoveryRequest request =
        LauncherDiscoveryRequestBuilder.request()
            .selectors(DiscoverySelectors.selectPackage(Tester.class.getPackageName()))
            .filters(ClassNameFilter.includeClassNamePatterns(".*Test"))
            .build();

    Launcher launcher = LauncherFactory.create();
    SummaryGeneratingListener listener = new SummaryGeneratingListener();

    launcher.execute(request, listener);

    TestExecutionSummary summary = listener.getSummary();
    summary.printTo(new PrintWriter(System.out));
  }
}
