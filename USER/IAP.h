#ifndef _IAP_H_
#define _IAP_H_
#include "appconfig.h"
#include "USART.h"

void listener_USART1_data(void);

void IAP_RESET(void);

void rece_ack(void);

void start_app(void);

void start_IAP(void);

#endif
