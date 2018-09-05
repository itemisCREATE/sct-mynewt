#include "testutil/testutil.h"


/**
 * Depending on the type of package, there are different
 * compilation rules for this directory.  This comment applies
 * to packages of type "unittest."  For other types of packages,
 * please view the documentation at http://mynewt.apache.org/.
 *
 * Put source files in this directory.  All files that have a *.c
 * ending are recursively compiled in the src/ directory and its
 * descendants.  The exception here is the arch/ directory, which
 * is ignored in the default compilation.
 *
 * The arch/<your-arch>/ directories are manually added and
 * recursively compiled for all files that end with either *.c
 * or *.a.  Any directories in arch/ that don't match the
 * architecture being compiled are not compiled.
 *
 * Architecture is set by the BSP/MCU combination.
 */


#if MYNEWT_VAL(SELFTEST)

TEST_SUITE_DECL(test_event_processing);
TEST_SUITE_DECL(test_internal_event_queue);
TEST_SUITE_DECL(test_mempool_usage);
TEST_SUITE_DECL(test_timer_service);

TEST_SUITE(test_all) {
    test_event_processing();
    test_internal_event_queue();
    test_mempool_usage();
    test_timer_service();
}


int
main(int argc, char **argv)
{
    ts_config.ts_print_results = 1;

    /* Initialize all packages. */
    sysinit();

    /* Call test suite functions here. */
    test_all();

    /* Indicate whether all test cases passed. */
    return tu_any_failed;
}
#endif
