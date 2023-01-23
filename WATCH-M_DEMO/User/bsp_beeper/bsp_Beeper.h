#ifndef __BSP_BEEPER_H
#define __BSP_BEEPER_H


#include "main.h"

#define BEEPER_GPIO_CLK		RCC_APB2Periph_GPIOC
#define BEEPER_GPIO_PORT	GPIOC
#define BEEPER_GPIO_PIN		GPIO_Pin_0


#define BEEPER_H()				GPIOC->BSRR |= BEEPER_GPIO_PIN
#define BEEPER_L()				GPIOC->BRR  |= BEEPER_GPIO_PIN

#define Beeper_Sound_max	5
#define Beeper_Sound_min	0

extern uint8_t Beeper_Flag;
extern uint8_t Beeper_Sound;

void BEEPER_GPIO_Config(void);
void BEEPER_ON(void);
void BEEPER_Set(void);


#endif /* __MY_BEEPER_H */
