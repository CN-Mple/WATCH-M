#ifndef __BSP_MPU6050_H
#define __BSP_MPU6050_H

#include "stm32f10x.h"
#include "main.h"

#define SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)

#define ADDRESS_MPU6050	0xD0	//IIC写入时的地址字节数据，+1为读取
#define IICSPEED     	0x24


/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define MPU6050_GPIO_PORT_I2C		GPIOB			/* GPIO端口 */
#define MPU6050_RCC_I2C_PORT 		RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define MPU6050_I2C_SCL_PIN			GPIO_Pin_6			/* 连接到SCL时钟线的GPIO */
#define MPU6050_I2C_SDA_PIN			GPIO_Pin_7			/* 连接到SDA数据线的GPIO */


#define MPU6050_I2C_SCL_1()					GPIO_SetBits(MPU6050_GPIO_PORT_I2C, MPU6050_I2C_SCL_PIN)
#define MPU6050_I2C_SCL_0()					GPIO_ResetBits(MPU6050_GPIO_PORT_I2C, MPU6050_I2C_SCL_PIN)

#define MPU6050_I2C_SDA_1()					GPIO_SetBits(MPU6050_GPIO_PORT_I2C, MPU6050_I2C_SDA_PIN)
#define MPU6050_I2C_SDA_0()					GPIO_ResetBits(MPU6050_GPIO_PORT_I2C, MPU6050_I2C_SDA_PIN)

#define MPU6050_I2C_SDA_Read()				GPIO_ReadInputDataBit(MPU6050_GPIO_PORT_I2C, MPU6050_I2C_SDA_PIN)

void MPU6050_MPU6050_I2C_GPIO_Config(void);

void MPU6050_Init(void);
void Write_MPU6050(uint8_t addr, uint8_t Data);
void Read_MPU6050(uint8_t addr,uint8_t *p, uint8_t Num);


void MUP6050_ACCEL(void);

void MOD_Display(void);


#endif /* __BSP_MPU6050_H */
