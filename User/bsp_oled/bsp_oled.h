#ifndef __BSP_OLED_H
#define __BSP_OLED_H

#include "stm32f10x.h"
#include "stdio.h"
#include "bsp_Font.h"
#include "main.h"

extern uint8_t Grame[8][128];
extern uint8_t MOD;
/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define GPIO_PORT_I2C		GPIOB			/* GPIO�˿� */
#define RCC_I2C_PORT 		RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define I2C_SCL_PIN			GPIO_Pin_11			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_PIN			GPIO_Pin_10			/* ���ӵ�SDA�����ߵ�GPIO */


#define I2C_SCL_1()					GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)
#define I2C_SCL_0()					GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)

#define I2C_SDA_1()					GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)
#define I2C_SDA_0()					GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)

#define I2C_SDA_Read()				GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)


void IIC_GPIO_Config(void);

void I2C_Start(void);
void I2C_Stop(void);

void Send_Byte(uint8_t Data);
uint8_t Read_Byte(void);

uint8_t I2C_WaitAck(void);
void I2C_Send_Ack(uint8_t ACK);

void Write_EE_Byte(uint8_t addr, uint8_t Data);	
uint8_t Read_EE_Byte(uint8_t addr);
 
void Write_N_Byte(uint8_t ADDR, uint8_t *p, uint8_t Num);
void Read_N_Byte(uint8_t ADDR, uint8_t *p, uint8_t Num);


void OLED_CMD(uint8_t cmd);

void OLED_Init(void);
void Set_Pos(uint8_t x, uint8_t y);
void OLED_Frame_Fast(uint8_t Data[]);

void OLED_ReFresh(uint8_t MOD);
void Fill_All(uint8_t fill_Data);

void OLED_16_Fast(uint8_t x,uint8_t y,uint8_t A[]);
void OLED_8_Fast(uint8_t x,uint8_t y,uint8_t A[]);

/*--------------------------------------------------------------------------------------------*/
void OLED_Num_8_Fast(uint8_t x,uint8_t y,uint8_t n,uint8_t A[][8]);
void OLED_8_16Num_Fast(uint8_t x,uint8_t y,uint8_t n,uint8_t A[][16]);

uint8_t OLED_Show_Char_16(uint8_t x,uint8_t y,uint8_t C);
void OLED_Show_String_16(uint8_t x,uint8_t y,char *C);

uint8_t OLED_Show_Char_8(uint8_t x,uint8_t y,uint8_t C);
void OLED_Show_String_8(uint8_t x,uint8_t y,char *C);

void OLED_APPlication(uint8_t x,uint8_t y,uint8_t n,uint8_t A[][128]);

/*--------------------------------------------------------------------------------------------*/
void OLED_flash_8_Fast(uint8_t x,uint8_t y,uint8_t n,uint8_t A[][8]);
uint8_t OLED_flash_Show_Char_8(uint8_t x,uint8_t y,uint8_t C);
void OLED_flash_String_8(uint8_t x,uint8_t y,char *C);

void OLED_flash_8_16_Num_Fast(uint8_t x,uint8_t y,uint8_t n,uint8_t A[][16]);
uint8_t OLED_flash_Show_Char_16(uint8_t x,uint8_t y,uint8_t C);
void OLED_flash_Show_String_16(uint8_t x,uint8_t y,char *C);

uint8_t OLED_flash_APPlication(uint8_t x,char y,uint8_t n,uint8_t A[][128]);

void Select_Windows(void);

void Show_16X32_Number_static(uint8_t y,uint8_t n,uint8_t A[][16]);
void Show_16X32_Number_volatile(uint8_t x,uint8_t y,uint8_t n,uint8_t A[][16]);

void UI__16X32_Number(uint8_t y,uint8_t num_1,uint8_t num_2,uint8_t count,uint8_t A[][16]);
void UI_Screen(void);

uint8_t Pot_State(uint8_t x,uint8_t y);
uint8_t Draw_pot(uint8_t x,uint8_t y);
uint8_t Draw_line(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end);
uint8_t Clear_line(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end);

void Draw_square(uint8_t x_start,uint8_t y_start,uint8_t length,uint8_t weighth);
void Clear_square(uint8_t x_start,uint8_t y_start,uint8_t length,uint8_t weighth);






#endif

