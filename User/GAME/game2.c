#include "game2.h"

extern uint8_t Grame[8][128];

#define Head	( Snake_Length - 1 )
#define Tail	( 0 )
//蛇身最长20 食物坐标
Snake Pos[20];
Snake Food;
//蛇身初始化 长度2
uint8_t Snake_Length = 2;
//定义初始坐标
char Key_x = 1;
char Key_y = 0;
//关卡 1
static uint8_t Continue = 0;
//食物状态标志
uint8_t Food_State = Food_Reset;

uint8_t Current_State = RIGHT;

void Draw_Snake_Body(uint8_t Parament)
{
	uint8_t i;
	#if (MODE == MODE_POINT)
	Grame[7 - ( ( Pos[i].y) / 8 )][Pos[i].x] |= 0x80 >> (Pos[i].y % 8);
	#else
	for(i = 0; i < 8; i++)
	{
		Grame[7 - Pos[Parament].y][Pos[Parament].x * 8 + i] = 0xFF;
	}
	#endif
}

void Init_Snake(void)
{
	uint8_t i;
	
	Fill_All(0);
	memset(Grame,0,8*128);
	Snake_Length = 2;
	
	Pos[Head].x = 1;
	Pos[Head].y = 1;
	Pos[Tail].x = 0;
	Pos[Tail].y = 1;
	
	Food.x = 5;
	Food.y = 5;
	Food_State = Food_Reset;
	for(i = 0; i < Snake_Length; i++)
	{
		Draw_Snake_Body(i);
	}
	
	#if (MODE == MODE_POINT)
		Grame[7 - ( ( Food.y) / 8 )][Food.x] |= 0x80 >> (Food.y % 8);
	#else
	for(i = 0; i < 8; i++)
	{
		Grame[7 - Food.y][Food.x * 8 + i] = 0;
	}
	#endif
}

void Draw_Food(void)
{
	
	#if (MODE == MODE_POINT)
		Grame[7 - ( ( Food.y) / 8 )][Food.x] |= 0x80 >> (Food.y % 8);
	#else
	uint8_t i;
	for(i = 0; i < 8; i++)
	{
		Grame[7 - Food.y][Food.x * 8 + i] = 0xFF;
	}
	#endif
}

void Set_Food(void)
{
	if( Food_State == Food_Reset )
	{
		Food.x = (Food.x + 1)% Y_MAX;
		Food.y = (Food.y + 1)% Y_MAX;
		
		Food_State = Food_Set;
		Draw_Food();
	}
}

void Clear_Snake_Tail(void)
{
	#if (MODE == MODE_POINT)
	Grame[7 - ( ( Pos[Tail].y) / 8 )][Pos[Tail].x] &= ~( 0x80 >> (Pos[Tail].y % 8) );
	#else
	uint8_t i;
	for(i = 0; i < 8; i++)
	{
		Grame[7 - Pos[Tail].y][Pos[Tail].x * 8 + i] = 0;
	}
	#endif
}




void Draw_Snake(void)
{
	uint8_t i;
	if(Continue == 1)
	{
		for(i = 0;i < Head;i++)
		{
			if( Pos[i].x == Pos[Head].x && Pos[i].y == Pos[Head].y ) 
			{
				Continue = 0;
			}
		}
	}
	if(Continue == 1)
	{
		if( ( ( Pos[Head].x ) == Food.x ) && ( ( Pos[Head].y ) == Food.y ))
		{
			Snake_Length++;
			Pos[Head].x = ( Pos[Head - 1].x + Key_x ) % X_MAX;
			Pos[Head].y = ( Pos[Head - 1].y + Key_y ) % Y_MAX;
			
			Food_State = Food_Reset;
		}
		else
		{
			Clear_Snake_Tail();
			for(i = Tail; i < Head; i++)
			{
				Pos[i].x = Pos[i + 1].x;
				Pos[i].y = Pos[i + 1].y;
			}
			Pos[Head].x = ( Pos[Head].x + Key_x ) % X_MAX;
			Pos[Head].y = ( Pos[Head].y + Key_y ) % Y_MAX;
			
			for(i = 0; i < Snake_Length; i++)
			{
				Draw_Snake_Body(i);
			}
		}
	}
}

static void Delay(__IO uint32_t nCount) //简单的延时函数
{
	for (; nCount != 0; nCount--);
}


void Key_Handle(void)
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
				if(Current_State != UP)
				{
					Key_x = 0;
					Key_y = -1;
					Current_State = DOWN;
				}
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
				if(Current_State != RIGHT)
				{
					Key_x = -1;
					Key_y = 0;
					Current_State = LEFT;
				}
			}
			else if(Key_msg.id == 1)
			{
				if(Current_State != LEFT)
				{
					Key_x = 1;
					Key_y = 0;
					Current_State = RIGHT;
				}
			}
		}
	}
}

void GAME2(void)
{
		if( Continue == 0 )
		{
			Init_Snake();
			Continue = 1;
		}
		Set_Food();
		Draw_Snake();
		OLED_ReFresh(MOD);
		Delay(500000);
		KEY(Key_Handle);
}

