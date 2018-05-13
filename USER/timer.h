#ifndef _TIMER_H_
#define _TIMER_H_
#include "stm32f10x.h"
#include "appconfig.h"
#include "AtParser.h"

void TIM2_Config(void);

void inform_up_data(void);

void set_at_wait_time(u8 sec);

u8 is_wait_stop(void);

#endif
