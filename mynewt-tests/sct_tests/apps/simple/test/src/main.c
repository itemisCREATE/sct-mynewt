#include "simple_sct_tests.h"
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

struct os_mempool pool;
os_membuf_t mem_buffer[OS_MEMPOOL_SIZE(QUEUE_SIZE, SIMPLESCT_EVENT_POOL_BLOCK_SIZE)];
SimpleSct simpleSct;

TEST_CASE_DECL(test_change_state);
TEST_CASE_DECL(test_change_state_two_stage);
TEST_CASE_DECL(test_change_state_event_queue_order);
TEST_CASE_DECL(test_change_state_event_queue_order2);
TEST_CASE_DECL(test_event_payload);

TEST_SUITE(simple_sct_tests) {
	test_change_state();
	test_change_state_two_stage();
	test_change_state_event_queue_order();
	test_change_state_event_queue_order2();
	test_event_payload();
}

#if MYNEWT_VAL(SELFTEST)
int
main(int argc, char **argv)
{
    ts_config.ts_print_results = 1;
    /* Initialize all packages. */
    sysinit();


    /* Call test suite functions here. */
    simple_sct_tests();

    /* Indicate whether all test cases passed. */
    return tu_any_failed;
}
#endif
