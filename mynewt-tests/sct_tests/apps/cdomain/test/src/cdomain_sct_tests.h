#ifndef CDOMAIN_SCT_TESTS
#define CDOMAIN_SCT_TESTS

#include "sysinit/sysinit.h"
#include "syscfg/syscfg.h"
#include "testutil/testutil.h"
#include "os/os.h"
#include "os/os_eventq.h"
#include "os/os_mempool.h"
#include "CDomain.h"

#define QUEUE_SIZE 10

void cdomain_tests_init();
void get_and_call_next_os_event();
void get_and_call_all_os_events();

extern struct os_mempool pool;
extern os_membuf_t mem_buffer[OS_MEMPOOL_SIZE(QUEUE_SIZE, CDOMAIN_EVENT_POOL_BLOCK_SIZE)];
extern CDomain machine;

#endif
