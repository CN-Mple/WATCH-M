#include "my_edition.h"

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
				
			}
			else if(Key_msg.id == 1)
			{
				
			}
		}
	}
}

void Edition_Funcation(void)
{
	OLED_flash_String_8(0,0,"Name:MY_Watch");
	OLED_flash_String_8(1,0,"Edition:<1.0>");
	OLED_flash_String_8(2,0,"Function:    ");
	OLED_flash_String_8(3,0,"Tool         ");
	OLED_flash_String_8(4,0,"Time         ");
	OLED_flash_String_8(5,0,"Game         ");
	OLED_flash_String_8(6,0,"sec_tick     ");
	OLED_flash_String_8(7,0,"Back         ");
	KEY(Key_Handle);
}
