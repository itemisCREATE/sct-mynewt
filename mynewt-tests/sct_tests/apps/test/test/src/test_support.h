/*
 * test_support.h
 *
 *  Created on: 03.09.2018
 *      Author: terfloth
 */

#ifndef TEST_SUPPORT_H_
#define TEST_SUPPORT_H_

#include "os/os.h"
void get_and_call_next_os_event();
void get_and_call_next_os_event_timeout(os_time_t timeout);
void get_and_call_all_os_events();


#endif /* TEST_SUPPORT_H_ */
