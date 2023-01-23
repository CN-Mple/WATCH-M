#include "bsp_power.h"

uint16_t ADC_ValueConvertion;

void BSP_POWER_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(POWER_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = POWER_GPIO_PIN;
	
	GPIO_Init(POWER_GPIO_PORT, &GPIO_InitStructure);

}

static void ADC_MODE_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_APB2PeriphClockCmd(ADC_CLK, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_DeInit(ADC_DMA_CHANNEL);
	//���赽�Ĵ���
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADCx->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ValueConvertion;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	//�����ַ������ �洢��ַ����
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	// ��������С��Ӧ�õ�������Ŀ�ĵصĴ�С
	DMA_InitStructure.DMA_BufferSize = 1;
	//�洢�����洢����ʹ��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	//16λ ����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	//ѭ��ģʽ DMA�����ȼ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	
	ADC_Init(ADCx, &ADC_InitStructure);
	
		// ����ADCʱ�ӣΪPCLK2��8��Ƶ����9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// ����ADC ͨ����ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);

	
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);
	
	// ʹ��ADC DMA ����
	ADC_DMACmd(ADCx, ENABLE);
	// ����ADC ������ʼת��
	ADC_Cmd(ADCx, ENABLE);
	
	// ��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(ADCx);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADCx));
	
	// ADC��ʼУ׼
	ADC_StartCalibration(ADCx);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADCx));
	
	// ����û�в����ⲿ����������ʹ���������ADCת�� 
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}

void BSP_POWER_Init(void)
{
	BSP_POWER_GPIO_Config();
	ADC_MODE_Config();
}

void UI_Power_Display(void)
{
	uint8_t i = 0;
	int16_t Power_Posent = 0; 
	int16_t Power_num = 0;
	char buf[16];
	Power_num = ADC_ValueConvertion * 0.00335693359375;
	Power_Posent = ADC_ValueConvertion * 2.44140625 ;

	sprintf(buf,"%d%d.%d",Power_Posent/1000,Power_Posent/100%10,Power_Posent%10);
	buf[4] = '%';
	buf[5] = '\0';
	OLED_flash_String_8(7,17,buf);
	
	Grame[7][0] = 0xFF;
	for(i = 1; i < 15; i++)
	{
		Grame[7][i] = 0x81;
	}
	for(i = 1; i < Power_num; i++)
	{
		Grame[7][i] |= 0x7E;
	}
	Grame[7][15] = 0xFF;
	Grame[7][16] = 0x18;
}
