#ifndef _AT_PARSER_H_
#define _AT_PARSER_H_
#include "stm32f10x.h"
#include "USART.h"
#include "Logs.h"
#include <stdio.h>
#include "connect.h"

u8 is_network(void);

void at_creg_par(unsigned char *atcmd,u16 len);

u8 mem_cmp(unsigned char *dst,unsigned char *src,u16 len);

void at_parser(unsigned char *atcmd,u16 len);

unsigned char* check_valid(unsigned char *atcmd,u16 *len);

void net_init(u8 val);

void read_at_cmd(u8 cmd);

u8 is_server_connect(void);

u8 gsm_ready(void);

void rece_data(unsigned char *data,u16);

void usart_parser(void);

#endif
