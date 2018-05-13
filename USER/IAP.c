#include "IAP.h"
#include "flash.h"
#include <string.h>

u32 start_addr = FLASH_ApplitionAddr;

typedef void (*IAPFun)(void);
IAPFun Jump_To_Application;


void start_IAP(void)
{
		vu32 JumpAddress;
		write_flash_short_data(0X08032800,0Xff80);
		
		__disable_irq();
	
		JumpAddress = *(__IO uint32_t*) (IAP_ApplitionAddr + 4);  
		Jump_To_Application = (IAPFun) (JumpAddress);  

		__set_MSP(*(__IO uint32_t*) IAP_ApplitionAddr);  
		Jump_To_Application();
	
		__enable_irq();
}


