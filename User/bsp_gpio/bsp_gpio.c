#include "bsp_gpio.h"


void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED_GPIO_CLOCK, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = LED_BULE_GPIO_Pin | LED_GREEN_GPIO_Pin |LED_RED_GPIO_Pin;
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
}
