#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <stm32f10x.h>

#define MCU_FLASH_SIZE 256

#if MCU_FLASH_SIZE < 256
	#define PAGE_SIZE 1024
	#define USART_RX_SIZE 1024 + 1 + 1 + 2
#else
	#define PAGE_SIZE 2048
	#define USART_RX_SIZE 2048 + 1 + 1 + 2
#endif	

#define UP_DATA_TIME_SEC 300
#define IAP_ApplitionAddr  (u32)(0x08000000)
#define FLASH_ApplitionAddr (u32)(0x08004000)
#endif
