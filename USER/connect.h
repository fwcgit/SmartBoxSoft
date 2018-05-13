#ifndef _CONNECT_H_
#define _CONNECT_H_
#include "USART.h"
#include "stm32f10x.h"
#include "delay.h"
#include "AtParser.h"
#include "string.h"

void gsm_mode_config(void);

void connect_server(void);

void close_connect(void);

void enable_net_at(void);

#endif
