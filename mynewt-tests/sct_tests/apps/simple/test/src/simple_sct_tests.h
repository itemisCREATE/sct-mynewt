#ifndef SIMPLE_SCT_TESTS
#define SIMPLE_SCT_TESTS

#include "sysinit/sysinit.h"
#include "syscfg/syscfg.h"
#include "testutil/testutil.h"
#include "os/os.h"
#include "os/os_eventq.h"
#include "os/os_mempool.h"
#include "SimpleSct.h"

#define QUEUE_SIZE 10

void simple_sct_tests_init();
void get_and_call_next_os_event();
void get_and_call_all_os_events();

extern struct os_mempool pool;
extern os_membuf_t mem_buffer[OS_MEMPOOL_SIZE(QUEUE_SIZE, SIMPLESCT_EVENT_POOL_BLOCK_SIZE)];

extern SimpleSct simpleSct;

#endif
