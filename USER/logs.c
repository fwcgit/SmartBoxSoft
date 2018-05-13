#include "logs.h"

void d(unsigned char *log,u8 len)
{
	u16 i = 0;
	USART2->SR;
	
	while(i < len)
	{
		USART_SendData(USART2,*(log+i));
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET);
		i++;
	}
	
	
}


int fputc(int ch,FILE *file)
{
	USART2->SR;
	USART_SendData(USART2,(unsigned char)ch);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET);
	return ch;
	
}
