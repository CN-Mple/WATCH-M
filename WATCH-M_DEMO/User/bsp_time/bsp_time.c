#include "bsp_time.h"

static void NVIC_TIME_Config(void)
{
	NVIC_InitTypeDef NVIC_structure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_structure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_structure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_structure.NVIC_IRQChannelSubPriority = 3;
	NVIC_structure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_structure);
}

void Timex_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBasestructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	TIM_TimeBasestructure.TIM_Period = 9999;//10ms
	TIM_TimeBasestructure.TIM_Prescaler = 71;
	
	TIM_TimeBaseInit(TIM6, &TIM_TimeBasestructure);

	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM6, ENABLE);
}



void Time_Init(void)
{
	NVIC_TIME_Config();
	Timex_Config();
}


