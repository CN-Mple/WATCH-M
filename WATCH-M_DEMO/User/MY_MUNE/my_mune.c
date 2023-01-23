#include "my_mune.h"

/*----目录节点----*/
Menu_t *Time_Menu;

Menu_t *Dialogo;
/*----------------*/

/*----控制句柄----*/

Menu_t *Tool;
Menu_t *Test_1;
Menu_t *Test_2;
Menu_t *Test_3;
Menu_t *Test_4;

Menu_t *Game;
Menu_t *Tcs;
Menu_t *Game2;
Menu_t *Game3;
Menu_t *Game4;
Menu_t *Game5;
Menu_t *Game6;
Menu_t *Game7;
Menu_t *Game8;
Menu_t *Game9;
Menu_t *Game10;


Menu_t *Temper;
Menu_t *Display;
Menu_t *Sec_tick;
Menu_t *Mpu6050;

Menu_t *Back;
/*----------------*/

/*----参数节点----*/
Menu_t *pxMune;
/*----------------*/

/*----功能节点----*/

Menu_t *CurrentMune;
Menu_t *Pre_SelectMune;
Menu_t *SelectMune;
/*----------------*/


//static void UI_Draw_square(uint8_t length,uint8_t weighth);

static void Key_Handle(void);
static void UI_MUNE_LIST(void);
static void UI_MENU_ICON(uint8_t Arry[][128]);

void MUNE_RETURN(void)//返回菜单
{
	if(CurrentMune->Menu_Previous_Item != NULL)
	{
		UI_Screen();
		
		SelectMune = CurrentMune;
		CurrentMune = CurrentMune->Menu_Previous_Item;

		Pre_SelectMune = SelectMune;
		while(Pre_SelectMune->Menu_Equal_Next_Item != NULL)
		{
			Pre_SelectMune = Pre_SelectMune->Menu_Equal_Next_Item;
		}
		memset(Grame,0,8*128);
	}
}

void MUNE_INTO(void)//进入菜单
{
	if( SelectMune != NULL )
	{
		UI_Screen();

		CurrentMune = SelectMune;
		
		SelectMune = CurrentMune->Menu_Next_Item;
		Pre_SelectMune = SelectMune;
		while(Pre_SelectMune->Menu_Equal_Next_Item != NULL)
		{
			Pre_SelectMune = Pre_SelectMune->Menu_Equal_Next_Item;
		}
		memset(Grame,0,8*128);
	}
}

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
				MUNE_RETURN();
			}
			else if(Key_msg.id == 1)
			{
				MUNE_INTO();
			}
		}
		else
		{
			if( Key_msg.id == 0 )
			{
				if(SelectMune->Menu_Equal_Previous_Item != NULL)
				{
					SelectMune = SelectMune->Menu_Equal_Previous_Item;
				}
			}
			else if(Key_msg.id == 1)
			{
				if(SelectMune->Menu_Equal_Next_Item != NULL)
				{
					SelectMune = SelectMune->Menu_Equal_Next_Item;
				}
			}
		}
	}
}

static void Delay(__IO uint32_t nCount) //简单的延时函数
{
	for (; nCount != 0; nCount--);
}


void Mune_Init(void)
{
	//创建根节点
	Time_Menu =  Create_RootMenu( "Time           ", WATCH_MUNE);
	Dialogo =  Create_NextMenu(Time_Menu,"Dialogo      ", MAIN_MUNE_DISPLAY);
	
	{
		//创建二级节点
		Tool	= Create_NextMenu( 	Dialogo,      "Tool           ",SETTING_MUNE_DISPLAY);
		Game 	= Create_Equal_NextMenu( Tool,    "Game           ",GAME_MUNE_DISPLAY);
		Display = Create_Equal_NextMenu( Game,    "MOD_Display    ",MOD_Display);
		Sec_tick= Create_Equal_NextMenu( Display, "Sec_tick       ",MIAOBIAO);
		Back 	= Create_Equal_NextMenu( Sec_tick,"ComeBack       ",ComeBack);
	}
	{
		//创建三级节点
		Test_1	= Create_NextMenu	   ( Tool,  "Edition        ",Edition_Funcation);
		Test_2	= Create_Equal_NextMenu( Test_1,"Time_Set       ",Time_Set_Funcation);
		Test_3	= Create_Equal_NextMenu( Test_2,"Sound_Set      ",BEEPER_Set);
		Test_4	= Create_Equal_NextMenu( Test_3,"Exit           ",ComeBack);
	}
	{
		Tcs 	= Create_NextMenu( Game		  ,"Game1            ",TCS_Game);
		Game2 	= Create_Equal_NextMenu( Tcs,  "Game2          ",VoidFunction);
		Game3 	= Create_Equal_NextMenu( Game2,"Game3          ",VoidFunction);
		Game4 	= Create_Equal_NextMenu( Game3,"Game4          ",VoidFunction);
		Game5 	= Create_Equal_NextMenu( Game4,"Game5          ",VoidFunction);
		Game6 	= Create_Equal_NextMenu( Game5,"Game6          ",VoidFunction);
		Game7 	= Create_Equal_NextMenu( Game6,"Game7          ",VoidFunction);
		Game8 	= Create_Equal_NextMenu( Game7,"Game8          ",VoidFunction);
		Game9 	= Create_Equal_NextMenu( Game8,"Game9          ",VoidFunction);
		Game10 	= Create_Equal_NextMenu( Game9,"Back           ",ComeBack);
	}
	
	CurrentMune = Time_Menu;
	SelectMune = Time_Menu->Menu_Next_Item;
	CurrentMune->Menu_Next_Item = SelectMune;
}

//static uint8_t x_dex = 0;
//static uint8_t x_target = 0;
//static uint8_t y_dex = 55;
//static uint8_t y_target = 55;


//static void UI_Draw_square(uint8_t length,uint8_t weighth)//绘制边框
//{
//	if(x_dex != x_target || y_dex != y_target)		//动态
//	{
//		Clear_square(x_dex,y_dex,length,weighth);	//消除
//		if(x_dex < x_target)
//		{
//			x_dex++;
//		}
//		else if(x_dex > x_target)
//		{
//			x_dex--;
//		}
//		if(y_dex < y_target)
//		{
//			y_dex++;
//		}
//		else if(y_dex > y_target)
//		{
//			y_dex--;
//		}
//		Draw_square(x_dex,y_dex,length,weighth);	//重构
//	}
//	else											//静态
//	{
//		Draw_square(x_dex,y_dex,length,weighth);
//	}
//}

static void UI_MUNE_LIST(void)
{
	static uint8_t Num;
	uint8_t i;
	uint8_t dex_x = 0;

	pxMune = CurrentMune->Menu_Next_Item;
	while(pxMune->Menu_Equal_Next_Item != NULL)
	{
		pxMune = pxMune->Menu_Equal_Next_Item;
	}
	Num = pxMune->ID;
	
	pxMune = CurrentMune->Menu_Next_Item;
	
	for(i = 1; i < Num + 1; i++)//箭头绘制
	{
		if( i == SelectMune->ID)
		{
			if(i < 8)
			{
				OLED_flash_String_8( SelectMune->ID,0,">");
			}
			else
			{
				OLED_flash_String_8( 7,0,">");
				dex_x = i - 7;
			}
		}
		else
		{
			OLED_flash_String_8( i,0," ");
		}
	}
	
	while(dex_x--)
	{
		pxMune = pxMune->Menu_Equal_Next_Item;
	}
	for(i = 1; i < Num; i++)
	{
		
		OLED_flash_String_8(i,8,pxMune->Name);
		if(pxMune->Menu_Equal_Next_Item == NULL)break;
		pxMune = pxMune->Menu_Equal_Next_Item;
	}
	
//	UI_Draw_square(64,8);
	KEY(Key_Handle);
}


char y = 48;

#define UI_Min		16		//速度
#define UI_Mid		48
#define UI_Max		96

static void UI_MENU_ICON(uint8_t Arry[][128])
{
	if(SelectMune != Pre_SelectMune || y != UI_Mid)
	{
		if(Pre_SelectMune->ID < SelectMune->ID)
		{
			if(y <= UI_Min)
			{
				Pre_SelectMune = Pre_SelectMune->Menu_Equal_Next_Item;
				y = UI_Mid;
			}
			else
			{
				y -= UI_Min;
			}
			
		}
		else if(Pre_SelectMune->ID > SelectMune->ID)
		{
			
			if(y >= UI_Max)
			{
				Pre_SelectMune = Pre_SelectMune->Menu_Equal_Previous_Item;
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
	//五个应用图标  两个预备态
	if(Pre_SelectMune->Menu_Equal_Previous_Item != NULL)
	{
		OLED_flash_APPlication(2,y - 48,Pre_SelectMune->Menu_Equal_Previous_Item->ID - 1,Arry);
		if(Pre_SelectMune->Menu_Equal_Previous_Item->Menu_Equal_Previous_Item != NULL)
		{
			OLED_flash_APPlication(2,y - 96,Pre_SelectMune->Menu_Equal_Previous_Item->Menu_Equal_Previous_Item->ID - 1,Arry);
		}
	}
	
	
	OLED_flash_APPlication(2,y,Pre_SelectMune->ID - 1,Arry);
	
	if(Pre_SelectMune->Menu_Equal_Next_Item != NULL)
	{
		OLED_flash_APPlication(2,y + 48,Pre_SelectMune->Menu_Equal_Next_Item->ID - 1,Arry);
		if(Pre_SelectMune->Menu_Equal_Next_Item->Menu_Equal_Next_Item != NULL)
		{
			OLED_flash_APPlication(2,y + 96,Pre_SelectMune->Menu_Equal_Next_Item->Menu_Equal_Next_Item->ID - 1,Arry);
		}
	}
	
	
	Select_Windows();
	
	OLED_flash_String_8(7,0,Pre_SelectMune->Name);

	KEY(Key_Handle);
}


void WATCH_MUNE(void)
{
	static uint8_t num_count;
	static uint8_t power_count;
	static uint8_t UI_NUM = 0;
	static char Time_bak[6] = "232324";
	static char Time[6] = "232323";
	uint8_t i = 0;
	char buf[16];
	
	sprintf(buf,"%s %2d %2d %4d",en_WEEK_STR[systmtime.tm_wday],systmtime.tm_mday,systmtime.tm_mon,systmtime.tm_year);
	OLED_flash_String_8(0,0,buf);

	if (TimeDisplay == 1)
	{
		sprintf(Time_bak,"%d%d%d%d%d%d",systmtime.tm_hour/10,systmtime.tm_hour%10,systmtime.tm_min/10,systmtime.tm_min%10,systmtime.tm_sec/10,systmtime.tm_sec%10);
		Time_Display( RTC_GetCounter(),&systmtime);
		sprintf(Time,"%d%d%d%d%d%d",systmtime.tm_hour/10,systmtime.tm_hour%10,systmtime.tm_min/10,systmtime.tm_min%10,systmtime.tm_sec/10,systmtime.tm_sec%10);
		
		for(i = 0; i < 6; i++)
		{
			if(Time_bak[i] != Time[i])
			{
				UI_NUM |= ( 0x01 << i );
			}
		}
		if(Time[5] % 2)
		{
			Show_16X32_Number_static(32,40,NUM);
		}
		else
		{
			Show_16X32_Number_static(32,44,NUM);
		}
		TimeDisplay = 0;
	}
	if(UI_NUM & 0x3F)//检测 数字变动 运行动态显示
	{
		if(num_count++ == 4)
		{
			num_count = 0;
			UI_NUM &= 0x00;
		}
		if(UI_NUM & 0x20)
		{
			UI__16X32_Number(112,( Time_bak[5] - 48 ),( Time[5] - 48 ),num_count,NUM);
		}
		if(UI_NUM & 0x10)
		{
			UI__16X32_Number(96,( Time_bak[4] - 48 ),( Time[4] - 48 ),num_count,NUM);
		}
		if(UI_NUM & 0x08)
		{
			UI__16X32_Number(64,( Time_bak[3] - 48 ),( Time[3] - 48 ),num_count,NUM);
		}
		if(UI_NUM & 0x04)
		{
			UI__16X32_Number(48,( Time_bak[2] - 48 ),( Time[2] - 48 ),num_count,NUM);
		}
		if(UI_NUM & 0x02)
		{
			UI__16X32_Number(16,( Time_bak[1] - 48 ),( Time[1] - 48 ),num_count,NUM);
		}
		if(UI_NUM & 0x01)
		{
			UI__16X32_Number(0,( Time_bak[0] - 48 ),( Time[0] - 48 ),num_count,NUM);
		}
		Delay(300000);
	}
	else//若否 静态显示
	{
		Show_16X32_Number_static(112,(Time[5] - 48) * 4,NUM);
		Show_16X32_Number_static( 96,(Time[4] - 48) * 4,NUM);
		Show_16X32_Number_static( 64,(Time[3] - 48) * 4,NUM);
		Show_16X32_Number_static( 48,(Time[2] - 48) * 4,NUM);
		Show_16X32_Number_static( 16,(Time[1] - 48) * 4,NUM);
		Show_16X32_Number_static(  0,(Time[0] - 48) * 4,NUM);
	}
	if(power_count++ == 10)
	{
		power_count = 0;
		UI_Power_Display();
	}

	KEY(Key_Handle);
}

void MAIN_MUNE_DISPLAY(void)
{
	char buf[16];
	memset(Grame,0,8*128);
	
	sprintf(buf,"<MAIN MUNE>");
	OLED_flash_String_8(0,20,buf);
	
	UI_MENU_ICON(MAIN_MUNE);
}

void GAME_MUNE_DISPLAY(void)
{
	char buf[16];
	
	sprintf(buf,"<GAME MUNE>");
	OLED_flash_String_8(0,20,buf);
	
	UI_MUNE_LIST();
}

void SETTING_MUNE_DISPLAY(void)
{
	char buf[16];

	memset(Grame,0,8*128);
	
	sprintf(buf,"<SETTING MUNE>");
	OLED_flash_String_8(0,8,buf);
	
	UI_MENU_ICON(SETTING_MUNE);
}


void VoidFunction(void)
{
	OLED_flash_String_8(0,0,"function is void");
	OLED_flash_String_8(5,0,"coming back...");
	OLED_ReFresh(MOD);
	Delay(9000000);
	ComeBack();
}

void ComeBack(void)
{
	if(CurrentMune->Menu_Previous_Item->Menu_Previous_Item != NULL)
	{	
		SelectMune = CurrentMune->Menu_Previous_Item;
		CurrentMune = CurrentMune->Menu_Previous_Item->Menu_Previous_Item;

		Pre_SelectMune = SelectMune;
		while(Pre_SelectMune->Menu_Equal_Next_Item != NULL)
		{
			Pre_SelectMune = Pre_SelectMune->Menu_Equal_Next_Item;
		}
		memset(Grame,0,8*128);
	}
}


