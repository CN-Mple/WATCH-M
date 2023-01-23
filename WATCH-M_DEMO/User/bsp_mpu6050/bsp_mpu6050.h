#ifndef __BSP_MPU6050_H
#define __BSP_MPU6050_H

#include "stm32f10x.h"
#include "main.h"

#define SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
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
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I		0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)

#define ADDRESS_MPU6050	0xD0	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ
#define IICSPEED     	0x24


/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define MPU6050_GPIO_PORT_I2C		GPIOB			/* GPIO�˿� */
#define MPU6050_RCC_I2C_PORT 		RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define MPU6050_I2C_SCL_PIN			GPIO_Pin_6			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define MPU6050_I2C_SDA_PIN			GPIO_Pin_7			/* ���ӵ�SDA�����ߵ�GPIO */


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
