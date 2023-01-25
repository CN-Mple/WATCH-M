#include "bsp_Beeper.h"

uint8_t Beeper_Flag;
uint8_t Beeper_Sound = 5;


void BEEPER_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(BEEPER_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = BEEPER_GPIO_PIN;
	GPIO_Init(BEEPER_GPIO_PORT, &GPIO_InitStructure);
}

void BEEPER_ON(void)
{
	if( Beeper_Sound != Beeper_Sound_min )
	{
		BEEPER_H();
		Beeper_Flag = 1;
	}
}


