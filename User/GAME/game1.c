#include "game1.h"

extern uint8_t Grame[8][128];
static uint8_t Continue = 0;

static uint8_t plat_x = 61;
static uint8_t plat_y = 0;
static uint8_t plat_x_target = 61;
static uint8_t plat_y_target = 0;
static uint8_t plat_length = 10;

static uint8_t plat_speed = 3;

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
				if(plat_x_target > plat_speed)
					plat_x_target = plat_x_target - plat_speed;
			}
			else if(Key_msg.id == 1)
			{
				if(plat_x_target < 127 - plat_speed - plat_length)
					plat_x_target = plat_x_target + plat_speed;
				else
					plat_x_target = 127 - plat_length;
			}
		}
	}
}

static void Delay(__IO uint32_t nCount) //简单的延时函数
{
	for (; nCount != 0; nCount--);
}

void GAME1_INIT(void)
{
	uint8_t i,j;
	memset(Grame,0,8*128);
	for(i = 0; i < 2; i++)
	{
		for(j = 0; j < 128; j++)
		{
			Grame[i][j] = 0xFF;
		}
	}
	Draw_line(plat_x,plat_y,plat_x + plat_length,plat_y);
}

void DRAW_PLAT(void)
{
	Draw_line(plat_x,plat_y,plat_x + plat_length,plat_y);
}
void CLEAR_PLAT(void)
{
	Clear_line(plat_x,plat_y,plat_x + plat_length,plat_y);
}

static void UI_PLAT(void)
{
	if(plat_x != plat_x_target)
	{
		CLEAR_PLAT();
		if(plat_x < plat_x_target)
		{
			if( plat_speed < plat_x_target - plat_x)
			plat_x = plat_x + plat_speed;
			else
			plat_x = plat_x_target;
		}
		else if(plat_x > plat_x_target)
		{
			if( plat_speed < plat_x - plat_x_target)
			plat_x = plat_x - plat_speed;
			else
			plat_x = plat_x_target;
		}
	}
	DRAW_PLAT();
}

void GAME1(void)
{
		if( Continue == 0 )
		{
			GAME1_INIT();
			Continue = 1;
		}
		UI_PLAT();
		OLED_ReFresh(MOD);

		KEY(Key_Handle);
}




