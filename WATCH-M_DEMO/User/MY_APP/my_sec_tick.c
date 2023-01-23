#include "my_sec_tick.h"


extern uint16_t arram_Count;
extern uint8_t arram_Flag;

uint8_t STOP;
uint8_t sec_tick_state;
uint16_t sec_tick_buf[4] = {0};

#define STOPSTATE	0
#define STARTSTATE	1
#define RUNSTATE1	2
#define RUNSTATE2	3
#define RUNSTATE3	4
#define RUNSTATE4	5

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
				switch(sec_tick_state)
				{
					case STOPSTATE:arram_Flag = 1;												sec_tick_state = STARTSTATE;break;
					case STARTSTATE:sec_tick_buf[0] = arram_Count;								sec_tick_state = RUNSTATE1;break;
					case RUNSTATE1:sec_tick_buf[1] = arram_Count;								sec_tick_state = RUNSTATE2;break;
					case RUNSTATE2:sec_tick_buf[2] = arram_Count;								sec_tick_state = RUNSTATE3;break;
					case RUNSTATE3:sec_tick_buf[3] = arram_Count;								sec_tick_state = RUNSTATE4;break;
					case RUNSTATE4:arram_Flag = 0;arram_Count = 0;								sec_tick_state = STOPSTATE;break;
				}
				memset(Grame,0,8*128);
			}
			else if(Key_msg.id == 1)
			{
				if(STOP++ % 2)
				{
					arram_Flag = 0;
				}
				else
				{
					arram_Flag = 1;
				}
				
			}
		}
	}
}

void MIAOBIAO(void)
{
	uint8_t i;
	char buf[16];
	sprintf(buf,"%d%d%d%d%d%d",arram_Count/6000/10,arram_Count/6000%10,arram_Count%6000/100/10,arram_Count%6000/100%10,arram_Count%100/10,arram_Count%100%10);
	
	Show_16X32_Number_volatile(0,112,(buf[5] - 48) * 4,NUM);
	Show_16X32_Number_volatile(0, 96,(buf[4] - 48) * 4,NUM);
	
	Show_16X32_Number_volatile(0, 64,(buf[3] - 48) * 4,NUM);
	Show_16X32_Number_volatile(0, 48,(buf[2] - 48) * 4,NUM);
	
	Show_16X32_Number_volatile(0,32,40,NUM);
	
	Show_16X32_Number_volatile(0, 16,(buf[1] - 48) * 4,NUM);
	Show_16X32_Number_volatile(0,  0,(buf[0] - 48) * 4,NUM);
	
	for(i = 1; i< sec_tick_state; i++)
	{
		sprintf(buf,"%d:  %d%d:%d%d:%d%d",i,sec_tick_buf[i - 1]/6000/10,sec_tick_buf[i - 1]/6000%10,\
												sec_tick_buf[i - 1]%6000/100/10,sec_tick_buf[i - 1]%6000/100%10,\
												sec_tick_buf[i - 1]%100/10,sec_tick_buf[i - 1]%100%10);
		OLED_flash_String_8(i + 3,0,buf);
	}
	Draw_square(0,63,127,32);
	KEY(Key_Handle);
}

