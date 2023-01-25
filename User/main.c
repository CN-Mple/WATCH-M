#include "main.h"

FATFS fs;													/* FatFs�ļ�ϵͳ���� */

FRESULT res_sd;                /* �ļ�������� */


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
	
	//���ⲿSD�������ļ�ϵͳ���ļ�ϵͳ����ʱ���SDIO�豸��ʼ��
	res_sd = f_mount(&fs,"0:",1);
	
	/*----------------------- ��ʽ������ ---------------------------*/  
	/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
	if(res_sd == FR_NO_FILESYSTEM)
	{
		printf("��SD����û���ļ�ϵͳ���������и�ʽ��...\r\n");
	/* ��ʽ�� */
		res_sd=f_mkfs("0:",0,0);							
		
		if(res_sd == FR_OK)
		{
			printf("��SD���ѳɹ���ʽ���ļ�ϵͳ��\r\n");
	  /* ��ʽ������ȡ������ */
			res_sd = f_mount(NULL,"0:",1);			
	  /* ���¹���	*/			
			res_sd = f_mount(&fs,"0:",1);
		}
		else
		{
			LED_RED;
			printf("������ʽ��ʧ�ܡ�����\r\n");
			while(1);
		}
	}
  else if(res_sd!=FR_OK)
  {
	printf("����SD�������ļ�ϵͳʧ�ܡ�(%d)\r\n",res_sd);
	printf("��������ԭ��SD����ʼ�����ɹ���\r\n");
		while(1);
  }
  else
  {
	printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
  }
	
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



