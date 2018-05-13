#include "USART.h"
#include "Logs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AtParser.h"
#include "IAP.h"

unsigned char USART1_rx_buff[150];
unsigned char USART2_rx_buff[150];

u32 USART1_rx_index = 0;;
u32 USART2_rx_index = 0;

void NVIC_USART1_Config(void)
{
	NVIC_InitTypeDef NVIC_Init_Type_Structure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	NVIC_Init_Type_Structure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_Init_Type_Structure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Init_Type_Structure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init_Type_Structure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_Init_Type_Structure);
	
}

void NVIC_USART2_Config(void)
{
	NVIC_InitTypeDef NVIC_Init_Type_Structure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_Init_Type_Structure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_Init_Type_Structure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Init_Type_Structure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init_Type_Structure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_Init_Type_Structure);
	
}

void USART_Config(void)
{
	USART_InitTypeDef USART_Init_Type_Structure;
	GPIO_InitTypeDef	GPIO_Init_Type_Structure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	
	/***´®¿Ú1GPIO***/
	GPIO_Init_Type_Structure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init_Type_Structure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init_Type_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_Init_Type_Structure);
	
	GPIO_Init_Type_Structure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init_Type_Structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init_Type_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_Init_Type_Structure);
	
	
	/***´®¿Ú2GPIO***/
	GPIO_Init_Type_Structure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init_Type_Structure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init_Type_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_Init_Type_Structure);
	
	GPIO_Init_Type_Structure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init_Type_Structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init_Type_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_Init_Type_Structure);
	
	USART_Init_Type_Structure.USART_BaudRate = 115200;
	USART_Init_Type_Structure.USART_WordLength = USART_WordLength_8b;
	USART_Init_Type_Structure.USART_Parity = USART_Parity_No;
	USART_Init_Type_Structure.USART_StopBits = USART_StopBits_1;
	USART_Init_Type_Structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init_Type_Structure.USART_Mode 								= USART_Mode_Tx | USART_Mode_Rx;
	
	USART_Init(USART1,&USART_Init_Type_Structure);
	USART_Init(USART2,&USART_Init_Type_Structure);
	
	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_ClearFlag(USART1,USART_FLAG_RXNE);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	USART_ClearFlag(USART2,USART_FLAG_RXNE);
	
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	NVIC_USART1_Config();
	NVIC_USART2_Config();
	
	USART_Cmd(USART1,ENABLE);
	USART_Cmd(USART2,ENABLE);
	

}

void Send_data(const unsigned char *data,u32 len)
{
	int i = 0;
	
	USART1_rx_index = 0;
	
	for(;i < len ;i++)
	{
		USART1->SR;
		USART_SendData(USART1,*(data+i));
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	}
}

void USART1_IRQHandler(void)
{
	char data;
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET){
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		
		data = USART_ReceiveData(USART1);
		
		//if(data == '+') USART1_rx_index = 0;
		
		if(USART1_rx_index >= 150)
		{
			USART1_rx_index = 0;
		}
			
		USART1_rx_buff[USART1_rx_index++] = data;
		
	}
}

void USART2_IRQHandler(void)
{
	
	char data;
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		
		data = USART_ReceiveData(USART2);
		
		if(USART2_rx_index >= 150) USART2_rx_index = 0;
		
		USART2_rx_buff[USART2_rx_index++] = data;
		
		if(data == '\n')
		{
			USART2_rx_buff[USART2_rx_index] ='\0';
			USART1_rx_index = 0;
			Send_data(USART2_rx_buff,USART2_rx_index);
			USART2_rx_index = 0;
		}
	}
	
}

void usart_parser(void)
{
	
		USART1_rx_index = 0;
		d(USART1_rx_buff,strlen((const char *)USART1_rx_buff));
		memset(USART1_rx_buff,0,sizeof(USART1_rx_buff));
		//at_parser(USART1_rx_buff,strlen((const char *)USART1_rx_buff));
	
}


const char *INIT_GMS_AT_TABLE[] = 
{
	"AT\r\n",
	"AT+GTSET=\"IPRFMT\",2\r\n",
	"AT+CPIN?\r\n",
	"AT+CGREG?\r\n",
	"AT+MIPCALL=1,\"cmnet\"\r\n"
};

u8 execute_at(u8 tcount,u8 timeout,const char *atcmd,char *dstcmd)
{
	char *temp;
	int try_count = 0;
	
	try_count = tcount;
	set_at_wait_time(0);
	
		while(1)
	{
		
			memset(USART1_rx_buff,0,sizeof(USART1_rx_buff));
			Send_data((unsigned char *)atcmd,strlen((const char*)atcmd));
		
			set_at_wait_time(timeout);
			temp = strstr((const char *)USART1_rx_buff,(const char *)dstcmd);
		
			while(temp == NULL && is_wait_stop() != 0 && strstr((const char *)USART1_rx_buff,(const char *)"ERROR") == NULL)
			{
					temp = strstr((const char *)USART1_rx_buff,(const char *)dstcmd);
			}
		
			if(temp != NULL)
			{
				break;
			}
			else
			{
				if(try_count-- <= 0)
				{
					return (u8)1;
				}
			}
	}
	
	return (u8)0;
	
}

void AT_CMD_GSM_init(void)
{
	int r;

	r = execute_at(12,3,INIT_GMS_AT_TABLE[0],"OK");
	usart_parser();
	
	if(r != 0) return;
	
	delay_ms(1000);
	r = execute_at(3,3,INIT_GMS_AT_TABLE[1],"OK");
	usart_parser();
	
	if(r != 0) return;
	
	delay_ms(1000);
	r = execute_at(12,3,INIT_GMS_AT_TABLE[2],"+CPIN: READY");
	usart_parser();
	
	if(r != 0) return;
	
	delay_ms(1000);
	r = execute_at(12,30,INIT_GMS_AT_TABLE[3],"OK");
	usart_parser();
	
	if(r != 0) return;
	
	delay_ms(1000);
	r = execute_at(12,30,INIT_GMS_AT_TABLE[4],"OK");
	usart_parser();
}
		
u8  get_creg(void)
{
	char *temp;
	int try_count = 0;
	
	try_count = 12;
	set_at_wait_time(0);
	
		while(1)
	{
		
			Send_data((unsigned char *)"AT+CGREG?",strlen((const char*)"AT+CGREG?"));
		
			set_at_wait_time(30);
			temp = strstr((const char *)USART1_rx_buff,(const char *)"OK");
		
			while( temp == NULL && is_wait_stop() != 0 && strstr((const char *)USART1_rx_buff,(const char *)"+ERROR") == NULL)
			{
					temp = strstr((const char *)USART1_rx_buff,(const char *)"OK");
			}
		
			if(temp != NULL)
			{
				at_parser((unsigned char*)temp,strlen(temp));
				break;
			}
			else
			{
				if(try_count-- <= 0)
				{
					return (u8)1;
				}
			}
	}
	
	return 0;
}

void listener_USART_BUFF(void)
{
	char *temp;
	vu32 JumpAddress;
	
	temp = strstr((const char *)USART1_rx_buff,(const char *)"firmware update");
	
	if(temp != NULL)
	{
		start_IAP();
	}
}

