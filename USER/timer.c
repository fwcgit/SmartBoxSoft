#include "timer.h"

int sec_count = 0;
int check_ount = 0;
int at_wait_time = 1;

/***
一秒一次中断
**/
void TIM2_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInit_Structurn;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_DeInit(TIM2);
	
	TIM_TimeBaseInit_Structurn.TIM_Period = 2000 - 1;
	TIM_TimeBaseInit_Structurn.TIM_Prescaler = 36000-1;
	TIM_TimeBaseInit_Structurn.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit_Structurn.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInit_Structurn);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	TIM_ITConfig(TIM2,TIM_FLAG_Update,ENABLE);
	
	
	
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
	 
	 
	 TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler(void)
{
		
	if(TIM_GetITStatus(TIM2,TIM_FLAG_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
	
		if(sec_count++ >= 10)
		{
			sec_count = 0;
			
			if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_8))
			{
					GPIOA->BRR = 1 << 8;
			}
			else
			{
					GPIOA->BSRR = 1 << 8;
			}
		}
		
		if(check_ount++ >= 3)
		{

			check_ount = 0;
		}
		
		if( at_wait_time > 0 )
			at_wait_time--;
	}

}

void set_at_wait_time(u8 sec)
{
	at_wait_time = sec;
}

u8 is_wait_stop(void)
{
	return at_wait_time;
}
