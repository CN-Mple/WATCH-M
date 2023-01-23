#include "main.h"



int Handle(void (*Callback)());
void BSP_Config(void);

void BSP_Config(void)
{
	//外设IO初始化
	LED_GPIO_Config();
	KEY_GPIO_Config();
	USART_Config();
	IIC_GPIO_Config();
	BSP_POWER_Init();
//	DS18B20_GPIO_Config();
	BEEPER_GPIO_Config();
	MPU6050_MPU6050_I2C_GPIO_Config();
	
	//RTC配置
	My_RTC_Config();
	MPU6050_Init();
	
	//关闭LED
	GPIO_SetBits(LED_GPIO_PORT, LED_BULE_GPIO_Pin);
	GPIO_SetBits(LED_GPIO_PORT, LED_RED_GPIO_Pin);
	GPIO_SetBits(LED_GPIO_PORT, LED_GREEN_GPIO_Pin);
	
	//液晶屏 初始化
	OLED_Init();
	//串口输出测试
	printf("Holle World!\r\n");
	//定时器初始化
	Time_Init();
	//清屏
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



