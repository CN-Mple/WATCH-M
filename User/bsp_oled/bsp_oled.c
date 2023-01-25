#include "bsp_oled.h"

uint8_t Grame[8][128] = {0};
uint8_t MOD = 0;

void IIC_GPIO_Config(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	
	/* 使能 I2C_GPIO 时钟 */
	RCC_APB2PeriphClockCmd(RCC_I2C_PORT, ENABLE);
	
	
	/* 配置SCL SDA引脚 开漏输出 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);
	
	I2C_Stop();
}

static void Delay(void)
{
	uint8_t i;
	for(i = 0; i < 10; i++);
}

void I2C_Start(void)
{
	I2C_SDA_1();
	I2C_SCL_1();
	Delay();
	I2C_SDA_0();
	Delay();
	I2C_SCL_0();
	Delay();
}

void I2C_Stop(void)
{
	I2C_SDA_0();
	I2C_SCL_1();
	Delay();
	I2C_SDA_1();
}

static void Send_Byte(uint8_t Data)
{
	uint8_t i = 0;
	for(i = 0; i < 8; i++)
	{
		if(Data & 0x80)
		{
			I2C_SDA_1();		//	SDA写1
		}
		else
		{
			I2C_SDA_0();		//	SDA写0
		}
		Delay();
		I2C_SCL_1();			//拉高SCL
		Delay();
		I2C_SCL_0();			//拉低SCL
		if(i == 7)
		{
			I2C_SDA_1();		//释放总线
		}
		Data <<= 1;				//左移下一位
	}
}

uint8_t I2C_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_1();	/* CPU释放SDA总线 */
	Delay();
	I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	Delay();
	if (I2C_SDA_Read())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0();
	Delay();
	return re;
}

void I2C_Send_Ack(uint8_t ACK)			//ACK = 1 表示应答可继续发送数据
{
	I2C_SCL_0();
	if (ACK)
	{
		I2C_SDA_0();
	}
	else
	{
		I2C_SDA_1();
	}
	Delay();
	I2C_SCL_1();
	Delay();
	I2C_SCL_0();
	I2C_SDA_1();
	Delay();
}


void OLED_CMD(uint8_t cmd)
{
	I2C_Start();
	
	Send_Byte(0x78);
	I2C_WaitAck();
	
	Send_Byte(0x00);
	I2C_WaitAck();
	
	Send_Byte(cmd);
	I2C_WaitAck();
	
	I2C_Stop();
}

void OLED_DATA(uint8_t Data)
{
	I2C_Start();
	
	Send_Byte(0x78);
	I2C_WaitAck();
	
	Send_Byte(0x40);
	I2C_WaitAck();
	
	Send_Byte(Data);
	I2C_WaitAck();
	
	I2C_Stop();
}

void OLED_Init(void)
{
/*------------------------------------------------------------*/
	OLED_CMD(0xAE);   //display off
	OLED_CMD(0x20);	//Set Memory Addressing Mode	
	OLED_CMD(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	OLED_CMD(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	OLED_CMD(0xc0);	//Set COM Output Scan Direction SX c8
	OLED_CMD(0x00);//---set low column address
	OLED_CMD(0x10);//---set high column address
	OLED_CMD(0x40);//--set start line address
	OLED_CMD(0x81);//--set contrast control register
	OLED_CMD(0xdf);
	OLED_CMD(0xa0);//--set segment re-map 0 to 127	ZY a8
	OLED_CMD(0xa6);//--set normal display
	OLED_CMD(0xa8);//--set multiplex ratio(1 to 64)
	OLED_CMD(0x3F);//
	OLED_CMD(0xa4);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	OLED_CMD(0xd3);//-set display offset
	OLED_CMD(0x00);//-not offset
	OLED_CMD(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_CMD(0xf0);//--set divide ratio
	OLED_CMD(0xd9);//--set pre-charge period
	OLED_CMD(0x22); //
	OLED_CMD(0xda);//--set com pins hardware configuration
	OLED_CMD(0x12);
	OLED_CMD(0xdb);//--set vcomh
	OLED_CMD(0x20);//0x20,0.77xVcc
	OLED_CMD(0x8d);//--set DC-DC enable
	OLED_CMD(0x14);//
	OLED_CMD(0xaf);//--turn on oled panel 
/*------------------------------------------------------------*/
}

/*-------------------设定坐标---------------------------------*/
void Set_Pos(uint8_t x, uint8_t y)
{
	OLED_CMD(0xb0 + x);
	OLED_CMD(0X00 + (y % 16));
	OLED_CMD(0X10 + (y / 16));
}

/*-------------------填充图片---------------------------------*/
void OLED_Frame_Fast(uint8_t Data[])
{
	uint8_t i = 0,j = 0; 
	unsigned int k = 0;
	for(i = 0;i < 8;i++)
	{
		OLED_CMD(0xb0 + i);			//page0-page7
		OLED_CMD(0x00);				//low column start address
		OLED_CMD(0x10);				//high column start address
		I2C_Start();
	
		Send_Byte(0x78);
		I2C_WaitAck();
		
		Send_Byte(0x40);
		I2C_WaitAck();
		
		for(j = 0;j < 128;j++)
		{
			Send_Byte(Data[k++]);
			I2C_WaitAck();
		}
		
		
		I2C_Stop();
	}
	
}

///*-------------------16 * 16 快速显示---------------------------*/
//void OLED_16_Fast(uint8_t x,uint8_t y,uint8_t A[])
//{
//	uint8_t i = 0;
//	uint8_t j = 0;
//	uint8_t k = 0;
//	
//	for(k = 0;k < 2;k++)
//	{
//		Set_Pos(x, y);
//		x++;
//		I2C_Start();
//	
//		Send_Byte(0x78);
//		I2C_WaitAck();
//		
//		Send_Byte(0x40);
//		I2C_WaitAck();
//		
//		for(i = 0;i < 16;i++)
//		{
//			Send_Byte(A[j++]);
//			I2C_WaitAck();
//		}
//		I2C_Stop();
//	}
//}

///*-------------------8 * 8 快速显示------------------------------*/
//void OLED_8_Fast(uint8_t x,uint8_t y,uint8_t A[])
//{
//	uint8_t i = 0;
//	
//	Set_Pos(x, y);
//	I2C_Start();
//	
//	Send_Byte(0x78);
//	I2C_WaitAck();
//	
//	Send_Byte(0x40);
//	I2C_WaitAck();
//	
//	for(i = 0;i < 8;i++)
//	{
//		Send_Byte(A[i]);
//		I2C_WaitAck();
//	}
//	I2C_Stop();
//}

///*-------------------8 * 8 数字显示------------------------------*/
//void OLED_Num_8_Fast(uint8_t x,uint8_t y,uint8_t n,uint8_t A[][8])
//{
//	uint8_t i = 0;
//	uint8_t j = 0;
//	Set_Pos(x, y);
//	I2C_Start();
//	
//	Send_Byte(0x78);
//	I2C_WaitAck();
//	
//	Send_Byte(0x40);
//	I2C_WaitAck();
//	
//	for(i = 0;i < 8;i++)
//	{
//		Send_Byte(A[n][j++]);
//		I2C_WaitAck();
//	}
//	I2C_Stop();
//}


/*-------------------填充Data---------------------------------*/
void Fill_All(uint8_t fill_Data) 
{
	uint8_t i,j;
	for(i = 0;i < 8;i++)
	{
		OLED_CMD(0xb0 + i);			//page0-page7
		OLED_CMD(0x00);				//low column start address
		OLED_CMD(0x10);				//high column start address
		for(j = 0;j < 128;j++)
		{
			OLED_DATA(fill_Data);
		}
	}
}

/*-------------------刷新---------------------------------*/
void OLED_ReFresh(uint8_t MOD)
{
	uint8_t x = 0,y = 0; 
	for(x = 0;x < 8;x++)
	{
		OLED_CMD(0xb0 + x);			//page0-page7
		OLED_CMD(0x00);				//low column start address
		OLED_CMD(0x10);				//high column start address
		I2C_Start();
	
		Send_Byte(0x78);
		I2C_WaitAck();
		
		Send_Byte(0x40);
		I2C_WaitAck();
		
		for(y = 0;y < 128;y++)
		{
			if(MOD)
			Send_Byte(~Grame[x][y]);
			else
			Send_Byte(Grame[x][y]);
			I2C_WaitAck();
		}
		I2C_Stop();
	}
	
}

//void OLED_8_16Num_Fast(uint8_t x,uint8_t y,uint8_t n,uint8_t A[][16])
//{

//	uint8_t i = 0;
//	uint8_t j = 0;
//	uint8_t k = 0;
//	
//	for(k = 0;k < 2;k++)
//	{
//		Set_Pos(x, y);
//		x++;
//		I2C_Start();
//	
//		Send_Byte(0x78);
//		I2C_WaitAck();
//		
//		Send_Byte(0x40);
//		I2C_WaitAck();
//		
//		for(i = 0;i < 8;i++)
//		{
//			Send_Byte(A[n][j++]);
//			I2C_WaitAck();
//		}
//		I2C_Stop();
//	}
//}

//uint8_t OLED_Show_Char_16(uint8_t x,uint8_t y,uint8_t C)
//{
//	uint8_t n;
//	if(C < 32 || C > 126)
//	{
//		return 0;
//	}
//	n = C - 32;
//	OLED_8_16Num_Fast(x,y,n,Char_16);
//	
//	return 1;
//}

//void OLED_Show_String_16(uint8_t x,uint8_t y,char *C)
//{
//	while(*C != '\0')
//	{
//		if(x > 6)
//		{
//			x = 0;
//		}
//		if(y > 120)
//		{
//			y = 0;
//			x = x + 2;
//		}
//		OLED_Show_Char_16(x,y,*C);
//		y = y + 8;
//		C++;
//	}
//}

//uint8_t OLED_Show_Char_8(uint8_t x,uint8_t y,uint8_t C)
//{
//	uint8_t n;
//	if(C < 32 || C > 126)
//	{
//		return 0;
//	}
//	n = C - 32;
//	OLED_Num_8_Fast(x,y,n,Char_8);
//	
//	return 1;
//}

//void OLED_Show_String_8(uint8_t x,uint8_t y,char *C)
//{
//	while(*C != '\0')
//	{
//		if(x > 7)
//		{
//			x = 0;
//		}
//		if(y > 120)
//		{
//			y = 0;
//			x = x + 1;
//		}
//		OLED_Show_Char_8(x,y,*C);
//		y = y + 8;
//		C++;
//	}
//}

uint8_t Pot_State(uint8_t x,uint8_t y)
{
	uint8_t y_page,y_dex;
	if(y >= 64)
		return 0;
	y_page = ( 63 - y ) / 8;
	y_dex = ( 63 - y ) % 8;
	if(Grame[y_page][x] | 0x01 << y_dex)
	return 1;
	else
	return 0;
}

uint8_t Draw_pot(uint8_t x,uint8_t y)
{
	uint8_t y_page,y_dex;
	if(y >= 64)
		return 1;
	y_page = ( 63 - y ) / 8;
	y_dex = ( 63 - y ) % 8;
	Grame[y_page][x] |= 0x01 << y_dex;
	
	return 0;
}

uint8_t Clear_pot(uint8_t x,uint8_t y)
{
	uint8_t y_page,y_dex;
	if(y >= 64)
		return 1;
	y_page = ( 63 - y ) / 8;
	y_dex = ( 63 - y ) % 8;
	Grame[y_page][x] &= ~( 0x01 << y_dex );
	
	return 0;
}

static uint8_t Pos_Target(uint8_t *x,uint8_t x_target)
{
	uint8_t i;
	i = (*x);
	if((*x) < x_target)
	{
		(*x)++;
	}
	else
	{
		(*x)--;
	}
	return i;
}

uint8_t Draw_line(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end)
{
	uint8_t x = x_start;
	uint8_t y = y_start;
	
	float x_dex = x_end - x_start;
	float y_dex = y_end - y_start;

	float k = y_dex / x_dex;
	float b = (float)( (float)y_start - (float)( k * (float)x_start ) );
	
	if(x_start != x_end)
	{
		while(Pos_Target(&x,x_end) != x_end)
		{
			Draw_pot(x ,y);
			y = (uint8_t)( (float)x * k + b );
		}
	}
	else
	{
		while(Pos_Target(&y_start,y_end) != y_end)
		{
			Draw_pot(x_start,y_start);
		}
	}
	return 0;
}
uint8_t Clear_line(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end)
{
	uint8_t x = x_start;
	uint8_t y = y_start;
	
	float x_dex = x_end - x_start;
	float y_dex = y_end - y_start;

	float k = y_dex / x_dex;
	float b = (float)( (float)y_start - (float)( k * (float)x_start ) );
	
	if(x_start != x_end)
	{
		while(Pos_Target(&x,x_end) != x_end)
		{
			Clear_pot(x ,y);
			y = (uint8_t)( (float)x * k + b );
		}
	}
	else
	{
		while(Pos_Target(&y_start,y_end) != y_end)
		{
			Clear_pot(x_start,y_start);
		}
	}
	return 0;
}


void Draw_square(uint8_t x_start,uint8_t y_start,uint8_t length,uint8_t weighth)
{
	weighth = weighth - 1;
	Draw_line(x_start,y_start,x_start+length,y_start);
	
	Draw_line(x_start,y_start,x_start,y_start - weighth);
	Draw_line(x_start + length,y_start,x_start + length,y_start - weighth);
	
	Draw_line(x_start,y_start - weighth,x_start + length,y_start - weighth);
}

void Clear_square(uint8_t x_start,uint8_t y_start,uint8_t length,uint8_t weighth)
{
	weighth = weighth - 1;
	
	Clear_line(x_start,y_start,x_start+length,y_start);
	
	Clear_line(x_start,y_start,x_start,y_start - weighth);
	Clear_line(x_start + length,y_start,x_start + length,y_start - weighth);
	
	Clear_line(x_start,y_start - weighth,x_start + length,y_start - weighth);
}

/*-------------------Flash------------------------------*/
void OLED_flash_8_Fast(uint8_t x,uint8_t y,uint8_t n,uint8_t A[][8])
{
	uint8_t i = 0;
//	uint8_t k = 0;
	uint8_t j = 0;
	for(i = 0;i < 8;i++)
	{
		Grame[x][y++] = A[n][j++];
//		for(k = 0;k < 8;k++)
//		{
//			if( A[n][j] | ( 0x01 << k ) )
//			Draw_pot(y ,x);
//		}
	}
}

uint8_t OLED_flash_Show_Char_8(uint8_t x,uint8_t y,uint8_t C)
{
	uint8_t n;
	if(C < 32 || C > 126)
	{
		return 0;
	}
	n = C - 32;
	OLED_flash_8_Fast(x,y,n,Char_8);
	
	return 1;
}

void OLED_flash_String_8(uint8_t x,uint8_t y,char *C)
{
	while(*C != '\0')
	{
		if(x > 7)
		{
			break;
//			x = 0;
		}
		if(y > 120)
		{
			y = 0;
			x = x + 1;
		}
		OLED_flash_Show_Char_8(x,y,*C);
		y = y + 8;
		C++;
	}
}
/*----------------------------------------------------------*/
///*----------------------------------------------------------*/

//void OLED_flash_8_16_Num_Fast(uint8_t x,uint8_t y,uint8_t n,uint8_t A[][16])
//{

//	uint8_t i = 0;
//	uint8_t j = 0;
//	uint8_t k = 0;
//	uint8_t y_bak = y;
//	for(k = 0;k < 2;k++)
//	{
//		y = y_bak;
//		for(i = 0;i < 8;i++)
//		{
//			Grame[x][y++] = A[n][j++];
//		}
//		x++;
//	}
//}

//uint8_t OLED_flash_Show_Char_16(uint8_t x,uint8_t y,uint8_t C)
//{
//	uint8_t n;
//	if(C < 32 || C > 126)
//	{
//		return 0;
//	}
//	n = C - 32;
//	OLED_flash_8_16_Num_Fast(x,y,n,Char_16);
//	
//	return 1;
//}

//void OLED_flash_Show_String_16(uint8_t x,uint8_t y,char *C)
//{
//	while(*C != '\0')
//	{
//		if(x > 6)
//		{
//			x = 0;
//		}
//		if(y > 120)
//		{
//			y = 0;
//			x = x + 2;
//		}
//		OLED_flash_Show_Char_16(x,y,*C);
//		y = y + 8;
//		C++;
//	}
//}
/*----------------------------------------------------------*/

uint8_t OLED_flash_APPlication(uint8_t x,char y,uint8_t n,uint8_t A[][128])
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t k = 0;
	char y_bak = y;
	
	for(k = 0;k < 4;k++)
	{
		y = y_bak;
		for(i = 0;i < 32;i++)
		{
			if(y >= 0 && y <= 127)
			Grame[x][y] = A[n][j];
			y++;
			j++;
		}
		x++;
	}
	return 1;
}

void Select_Windows(void)
{
	uint8_t i;

	for(i = 48; i < 80; i++)
	{
		Grame[1][i] |= 0x80;
		Grame[6][i] |= 0x01;
	}
	Grame[2][45] |= 0x0C;
	Grame[2][46] |= 0x02;
	Grame[2][47] |= 0x01;
	
	Grame[2][80] |= 0x01;
	Grame[2][81] |= 0x02;
	Grame[2][82] |= 0x0C;
	
	Grame[5][45] |= 0x30;
	Grame[5][46] |= 0x40;
	Grame[5][47] |= 0x80;
	
	Grame[5][80] |= 0x80;
	Grame[5][81] |= 0x40;
	Grame[5][82] |= 0x30;
}

void Show_16X32_Number_static(uint8_t y,uint8_t n,uint8_t A[][16])
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t k = 0;
	
	uint8_t x = 2;
	uint8_t y_bak = y;
	for(k = 0;k < 4;k++)
	{
		y = y_bak;
		for(i = 0;i < 16;i++)
		{
			Grame[x][y++] = A[n][j++];
		}
		x++;
	}
}

void Show_16X32_Number_volatile(uint8_t x,uint8_t y,uint8_t n,uint8_t A[][16])
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t k = 0;
	
	uint8_t y_bak = y;
	for(k = 0;k < 4;k++)
	{
		y = y_bak;
		for(i = 0;i < 16;i++)
		{
			Grame[x][y++] = A[n][j++];
		}
		x++;
	}
}

void UI__16X32_Number(uint8_t y,uint8_t num_1,uint8_t num_2,uint8_t count,uint8_t A[][16])
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t k = 0;
	uint8_t x = 2;
	uint8_t y_bak = y;
	
	uint8_t buf[4][16];
	
	for(i = 0; i < ( 4 - count ); i++)
	{
		for(j = 0; j < 16; j++)
		{
			buf[i][j] = A[( num_1*4 + count + i )][j];
		}
	}

	for(i = 0; i < count; i++)
	{
		for(j = 0; j < 16; j++)
		{
			buf[( 4 - count ) + i][j] = A[num_2*4 + i][j];
		}
	}

	for(k = 0;k < 4;k++)
	{
		y = y_bak;
		for(i = 0;i < 16;i++)
		{
			Grame[x][y++] = buf[k][i];
		}
		x++;
	}
}

void UI_Screen(void)
{
	uint8_t i = 0;
	uint8_t x = 0;
	uint8_t y = 0;
	for(i = 0; i < 8 ; i++)
	{
		memset(Grame,0,i*128);
		for(x = 7; x > 0; x--)
		{
			for(y = 0;y < 128;y++)
			{
				Grame[x][y] = Grame[x - 1][y];
			}
		}
		memset(Grame,0,128);
		OLED_ReFresh(MOD);
	}
}




