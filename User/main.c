#include "main.h"

FATFS fs;													/* FatFs文件系统对象 */

FRESULT res_sd;                /* 文件操作结果 */


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
	
	//在外部SD卡挂载文件系统，文件系统挂载时会对SDIO设备初始化
	res_sd = f_mount(&fs,"0:",1);
	
	/*----------------------- 格式化测试 ---------------------------*/  
	/* 如果没有文件系统就格式化创建创建文件系统 */
	if(res_sd == FR_NO_FILESYSTEM)
	{
		printf("》SD卡还没有文件系统，即将进行格式化...\r\n");
	/* 格式化 */
		res_sd=f_mkfs("0:",0,0);							
		
		if(res_sd == FR_OK)
		{
			printf("》SD卡已成功格式化文件系统。\r\n");
	  /* 格式化后，先取消挂载 */
			res_sd = f_mount(NULL,"0:",1);			
	  /* 重新挂载	*/			
			res_sd = f_mount(&fs,"0:",1);
		}
		else
		{
			LED_RED;
			printf("《《格式化失败。》》\r\n");
			while(1);
		}
	}
  else if(res_sd!=FR_OK)
  {
	printf("！！SD卡挂载文件系统失败。(%d)\r\n",res_sd);
	printf("！！可能原因：SD卡初始化不成功。\r\n");
		while(1);
  }
  else
  {
	printf("》文件系统挂载成功，可以进行读写测试\r\n");
  }
	
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



