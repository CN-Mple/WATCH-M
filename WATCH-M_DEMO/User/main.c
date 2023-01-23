#include "main.h"



int Handle(void (*Callback)());
void BSP_Config(void);

void BSP_Config(void)
{
	//����IO��ʼ��
	LED_GPIO_Config();
	KEY_GPIO_Config();
	USART_Config();
	IIC_GPIO_Config();
	BSP_POWER_Init();
//	DS18B20_GPIO_Config();
	BEEPER_GPIO_Config();
	MPU6050_MPU6050_I2C_GPIO_Config();
	
	//RTC����
	My_RTC_Config();
	MPU6050_Init();
	
	//�ر�LED
	GPIO_SetBits(LED_GPIO_PORT, LED_BULE_GPIO_Pin);
	GPIO_SetBits(LED_GPIO_PORT, LED_RED_GPIO_Pin);
	GPIO_SetBits(LED_GPIO_PORT, LED_GREEN_GPIO_Pin);
	
	//Һ���� ��ʼ��
	OLED_Init();
	//�����������
	printf("Holle World!\r\n");
	//��ʱ����ʼ��
	Time_Init();
	//����
	Fill_All(0);
}

int main()
{	
	BSP_Config();
	Mune_Init();
	
	while(1)
	{
		Handle(CurrentMune->Funcation);

		OLED_ReFresh(MOD);
	}
}

int Handle(void (*Callback)())
{
    //printf("Entering Handle Function. ");
    Callback();
    //printf("Leaving Handle Function. ");
	
	return 0;
}



