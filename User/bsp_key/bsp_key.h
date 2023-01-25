#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f10x.h"
#include "bsp_usart.h"
#include "./led/bsp_led.h"
#include "main.h"

#define KEY_ON 1
#define KEY_OFF 0



#define    KEY1_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY1_GPIO_PORT    GPIOA			   
#define    KEY1_GPIO_PIN		 GPIO_Pin_0

#define    KEY2_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    KEY2_GPIO_PORT    GPIOC		   
#define    KEY2_GPIO_PIN		  GPIO_Pin_13

void KEY_GPIO_Config(void);

uint8_t KEY_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);

typedef struct Key_State_Struct
{
	uint8_t 	last_val;			//��¼ֵ
	uint8_t 	val;				//��ǰֵ

	uint8_t 	change;				//�����־
	uint8_t 	long_press_flag;	//������־
}Key_Struct;

typedef struct Key_Message_Struct
{
	uint8_t 	id;					//����id��
	uint8_t 	press;				//����
	uint8_t 	update_flag;		//�ϱ���־
	uint8_t 	long_press;			//������־
}Key_msg_Struct;


extern Key_msg_Struct Key_msg;

#define 	Key_Get(GPIOx, GPIO_Pin)			GPIO_ReadInputDataBit(GPIOx, GPIO_Pin)

void Key_Scan(void);
void Key_Proc(void);
void Key_Tick_CallBack(void);
void Key_Message_UP(void);

uint8_t KEY(void(*Key_Function)(void));



#endif /* __BSP_KEY_H */
