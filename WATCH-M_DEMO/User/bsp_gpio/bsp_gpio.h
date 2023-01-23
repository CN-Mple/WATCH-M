#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include "stm32f10x.h"


#define LED_GPIO_CLOCK		RCC_APB2Periph_GPIOB
#define LED_GPIO_PORT		GPIOB

#define LED_BULE_GPIO_Pin		GPIO_Pin_1
#define LED_GREEN_GPIO_Pin		GPIO_Pin_0
#define LED_RED_GPIO_Pin		GPIO_Pin_5

#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态
/* 定义控制 IO 的宏 */

#define LED_BULE_TOGGLE  digitalToggle(LED_GPIO_PORT,LED_BULE_GPIO_Pin)
#define LED_RED_TOGGLE   digitalToggle(LED_GPIO_PORT,LED_RED_GPIO_Pin)
#define LED_GREEN_TOGGLE digitalToggle(LED_GPIO_PORT,LED_GREEN_GPIO_Pin)


void LED_GPIO_Config(void);






#endif /* __BSP_GPIO_H */
