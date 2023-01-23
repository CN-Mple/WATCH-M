#include "bsp_Beeper.h"

uint8_t Beeper_Flag;
uint8_t Beeper_Sound = 5;

/*----------------------------KEY----------------------------------*/

static void Key_Handle(void)
{
	if( Key_msg.press == 1)
	{
		Key_msg.press = 0;
		
		BEEPER_ON();
		
		if( Key_msg.long_press == 1 )
		{
			Key_msg.long_press = 0;
			if( Key_msg.id == 0 )
			{
				
			}
			else if(Key_msg.id == 1)
			{
				MUNE_RETURN();
			}
		}
		else
		{
			if( Key_msg.id == 0 )
			{
				if( Beeper_Sound < Beeper_Sound_max)
				Beeper_Sound++;
			}
			else if(Key_msg.id == 1)
			{
				if( Beeper_Sound > Beeper_Sound_min)
				Beeper_Sound--;
			}
		}
	}
}

/*-----------------------------------------------------------------------*/
void BEEPER_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(BEEPER_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = BEEPER_GPIO_PIN;
	GPIO_Init(BEEPER_GPIO_PORT, &GPIO_InitStructure);
}

void BEEPER_ON(void)
{
	if( Beeper_Sound != Beeper_Sound_min )
	{
		BEEPER_H();
		Beeper_Flag = 1;
	}
}


void BEEPER_Set(void)
{
	char buf[10];
	sprintf(buf,"sound:%d",Beeper_Sound);

	OLED_flash_String_8(0,0,buf);
	KEY(Key_Handle);
}
