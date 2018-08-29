#include "cdomain_sct_tests.h"
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
os_membuf_t mem_buffer[OS_MEMPOOL_SIZE(QUEUE_SIZE, CDOMAIN_EVENT_POOL_BLOCK_SIZE)];
CDomain machine;

int arr[ARR_SIZE];

TEST_CASE_DECL(test_basic)
TEST_CASE_DECL(test_eventqueue)

TEST_SUITE(cdomain_tests) {
	test_basic();
	test_eventqueue();
}

#if MYNEWT_VAL(SELFTEST)
int
main(int argc, char **argv)
{
    /* Initialize all packages. */
    sysinit();

    /* Call test suite functions here. */
    cdomain_tests();

    /* Indicate whether all test cases passed. */
    return tu_any_failed;
}
#endif
