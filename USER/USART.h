#ifndef _USART_H_
#define _USART_H_
#include "stm32f10x.h"
#include "timer.h"
void USART_Config(void);
void Send_data(const unsigned char *data,u32 len);
void AT_CMD_GSM_init(void);
void listener_USART_BUFF(void);
u8  get_creg(void);
#endif

