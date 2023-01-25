#include "my_soundset.h"

static uint8_t option = 5;
static uint8_t select = 1;
static uint8_t select_target = 1;
static uint8_t in = 0;


static uint8_t sound_dex = 0;

static void APP_IDO(void);
	
uint8_t SOUND_SET(uint8_t num);
uint8_t SOUND_LVL4(uint8_t num);
uint8_t SOUND_LVL2(uint8_t num);
uint8_t SOUND_LVL0(uint8_t num);

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
				APP_IDO();
			}
		}
		else
		{
			if( Key_msg.id == 0 )
			{
				
				if(select_target > 1)
				{
					select_target--;
				}
				
			}
			else if(Key_msg.id == 1)
			{
				if(select_target < option)
				{
					select_target++;
				}
			}
		}
	}
}

static char y = 48;

#define UI_Min		16		//速度
#define UI_Mid		48
#define UI_Max		96

static void UI_MENU_ICON(uint8_t Arry[][128])
{
	if(in == 0)
	{
		in = 1;
		select = option;
		select_target = 1;
	}
	if(select != select_target || y != UI_Mid)
	{
		if(select < select_target)
		{
			if(y <= UI_Min)
			{
				select++;
				y = UI_Mid;
			}
			else
			{
				y -= UI_Min;
			}
			
		}
		else if(select > select_target)
		{
			
			if(y >= UI_Max)
			{
				select--;
				y = UI_Mid;
			}
			else
			{
				y += UI_Min;
			}
		}
		else
		{
			if(y > UI_Mid)
			{
				y -= UI_Min;
			}
			else
			{
				y += UI_Min;
			}
		}
	}
	sound_dex = 3 - Beeper_Sound/2;
	//五个应用图标  两个预备态
	if(select > 1)
	{
		if(select == 2)
		OLED_flash_APPlication(2,y - 48,select - 2 + sound_dex,Arry);
		else
		OLED_flash_APPlication(2,y - 48,select - 2,Arry);
		if(select > 2)
		{
			
			if(select == 3)
			OLED_flash_APPlication(2,y - 96,select - 3 + sound_dex,Arry);
			else
			OLED_flash_APPlication(2,y - 96,select - 3,Arry);
		}
	}
	if(select == 1)
	{
		OLED_flash_APPlication(2,y,select - 1 + sound_dex,Arry);
	}
	else
	OLED_flash_APPlication(2,y,select - 1,Arry);
	
	if(select < option)
	{
		OLED_flash_APPlication(2,y + 48,select,Arry);
		if(select < option - 1)
		{
			OLED_flash_APPlication(2,y + 96,select + 1,Arry);
		}
	}
	
	Select_Windows();
	
	switch(select)
	{
		case 1:OLED_flash_String_8(7,0,"SOUND_SET");break;
		case 2:OLED_flash_String_8(7,0,"SOUND_LVL4");break;
		case 3:OLED_flash_String_8(7,0,"SOUND_LVL2");break;
		case 4:OLED_flash_String_8(7,0,"SOUND_LVL0");break;
		case 5:OLED_flash_String_8(7,0,"Exit");break;
	}
	
	KEY(Key_Handle);
}

static void APP_IDO(void)
{
	uint8_t num = 1;
	
	SOUND_SET(num++);
	SOUND_LVL4(num++);
	SOUND_LVL2(num++);
	SOUND_LVL0(num++);
	if(num++ == select)
	{
		in = 0;
		MUNE_RETURN();
	}
}

void SOUND(void)
{
	char buf[16];
	memset(Grame,0,8*128);
	
	sprintf(buf,"<SOUND SET>");
	OLED_flash_String_8(0,20,buf);
	
	UI_MENU_ICON(SOUND_MUNE);
}
/*应用函数*/
uint8_t SOUND_SET(uint8_t num)
{
	if(num != select)
		return 1;
	Beeper_Sound = ( Beeper_Sound + 1 ) % 7;
	return 0;
}

uint8_t SOUND_LVL4(uint8_t num)
{
	if(num != select)
		return 1;
	Beeper_Sound = 4;
	return 0;
}

uint8_t SOUND_LVL2(uint8_t num)
{
	if(num != select)
		return 1;
	Beeper_Sound = 2;
	return 0;
}

uint8_t SOUND_LVL0(uint8_t num)
{
	if(num != select)
		return 1;
	Beeper_Sound = 0;
	return 0;
}

