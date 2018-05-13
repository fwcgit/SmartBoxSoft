#include "stm32f10x.h"
#include "USART.h"
#include "connect.h"
#include "AtParser.h"
#include "Logs.h"
#include "delay.h"
#include "timer.h"
#include <stdio.h>
#include "md5.h"
#include "base64.h"

extern u32 rx_index;

u8 is_up_date = 0;

u8 is_parser_data = 0;


void get_data_up(void)
{
	if(is_server_connect())
	{
		
	}
	else
	{
		connect_server();
	}
}

int main(void)
{

	 unsigned char encrypt[] = "admin";    //21232f297a57a5a743894a0e4a801fc3
   unsigned char decrypt[16];
	unsigned char buffer[128];
	size_t len;
  const char *src = "MTIzNDU=";
	
	int i = 0;
	mbedtls_md5_context md5_ctx;
	
	GPIO_InitTypeDef	GPIO_Init_Type_Structure;

	NVIC_SetVectorTable(NVIC_VectTab_FLASH,0X4000);
	__enable_irq();
	
	USART_Config();
	TIM2_Config();

	GPIO_Init_Type_Structure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init_Type_Structure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init_Type_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_Init_Type_Structure);
	
	GPIOA->BSRR = 1 << 8;
	
	delay_init(72);
	//Send_data("ET0\r\n",5);
	
	
    
    mbedtls_md5_init(&md5_ctx);
    mbedtls_md5_starts(&md5_ctx);
    mbedtls_md5_update(&md5_ctx, encrypt, strlen((char *)encrypt));
    mbedtls_md5_finish(&md5_ctx, decrypt);
    mbedtls_md5_free(&md5_ctx);    
		for (i = 0; i<16; i++)
    {
        printf("%02x", decrypt[i]);
    }
		
		mbedtls_base64_decode(buffer,sizeof(buffer),&len,(unsigned char*)src,8);
		
		d(buffer,len);
		
		//AT_CMD_GSM_init();
	
	while(1)
	{
		#if 1
		GPIOA->BSRR = 1<<8;
		delay_ms(100);
		GPIOA->BRR = 1<<8;
		delay_ms(100);
		
		#endif
		//Send_data("AT\r\n",4);
		
		listener_USART_BUFF();
		
	}
}


