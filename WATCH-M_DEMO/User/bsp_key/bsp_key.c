#include "bsp_key.h"

uint8_t long_press_tick = 0;

Key_Struct Key[2] = {0,0,0,0,0,0,0,0};
Key_msg_Struct Key_msg = {0};

static uint8_t Key_Get_Val(uint8_t i)
{
	uint8_t val = 0;
	switch(i)
	{
		case 0:val = GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN);break;
		
		case 1:val = GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN);break;
		
		default:val = GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN);break;
	}
	return val;
}

void Key_Scan(void)
{
	uint8_t i = 0;
	for( i = 0; i < 2; i++ )						//��ѯɨ�谴��״̬�Ƿ�ı�
	{
		Key[i].val = Key_Get_Val(i);
		if(Key[i].val != Key[i].last_val)
		{
			Key[i].last_val = Key[i].val;
			Key[i].change = 1;						//��Ǹı�
		}
	}
}

void Key_Proc(void)
{
	uint8_t i = 0;
	for(i = 0; i < 2; i++)
	{
		if( Key[i].change == 1 )					//ִ�б��
		{
			Key[i].change = 0;
			if( Key[i].val == 1 )				//�������״̬ ��ʼ��ʱ
			{
				Key_msg.id = i;
				long_press_tick = 50;
			}
			else									//���� �۲쳤����־
			{
				if( Key[i].long_press_flag == 1 )
				{
					Key[i].long_press_flag = 0;
					long_press_tick = 0;
				}
				else								//�Ѿ����� ��ʱ���� ���ϱ���Ϣ
				{
					long_press_tick = 1;
				}
				Key_msg.press = 1;
			}
		}
	}
}

void Key_Tick_CallBack(void)
{
	uint8_t i = 0;
	if( long_press_tick != 0 )
	{
		long_press_tick--;
		if( long_press_tick == 0 )
		{
			for( i = 0; i < 2; i++ )
			{
				if(Key[i].val == 1)//��ʱ����ʱ��Ȼ���ڰ���״̬ �ð���������־��λ �ϱ�����״̬
				{
					Key_msg.id = i;
//					Key_msg.press = 1;
					Key_msg.update_flag = 1;
					Key_msg.long_press = 1;
					Key[i].long_press_flag = 1;
				}
			}
		}
	}
	
}

void Key_Message_UP(void)
{
	if( Key_msg.update_flag == 1 )
	{
		Key_msg.update_flag = 0;
		printf("\r\nKey_msg.id = %d \r\nKey_msg.long_press = %d ",Key_msg.id,Key_msg.long_press);

	}
}



void KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;	
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);
}

uint8_t KEY(void(*Key_Function)())
{
	uint8_t State = 0;
	Key_Proc();
	Key_Message_UP();
	Key_Function();
	return State;
}

