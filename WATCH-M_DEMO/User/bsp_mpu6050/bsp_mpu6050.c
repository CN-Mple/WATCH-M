#include "bsp_mpu6050.h"

uint16_t ACCEL[3] = {0};


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
//				if(SelectMune->Menu_Equal_Previous_Item != NULL)
//				{
//					SelectMune = SelectMune->Menu_Equal_Previous_Item;
//				}
				MOD = (MOD + 1) % 2;
			}
			else if(Key_msg.id == 1)
			{
//				if(SelectMune->Menu_Equal_Next_Item != NULL)
//				{
//					SelectMune = SelectMune->Menu_Equal_Next_Item;
//				}
			}
		}
	}
}

static void Delay(void)
{
	uint8_t i;
	for(i = 0; i < 10; i++);
}

void MPU6050_I2C_Start(void)
{
	MPU6050_I2C_SDA_1();
	MPU6050_I2C_SCL_1();
	Delay();
	MPU6050_I2C_SDA_0();
	Delay();
	MPU6050_I2C_SCL_0();
	Delay();
}

void MPU6050_I2C_Stop(void)
{
	MPU6050_I2C_SDA_0();
	MPU6050_I2C_SCL_1();
	Delay();
	MPU6050_I2C_SDA_1();
}

static void Send_Byte(uint8_t Data)
{
	uint8_t i = 0;
	for(i = 0; i < 8; i++)
	{
		if(Data & 0x80)
		{
			MPU6050_I2C_SDA_1();		//	SDAд1
		}
		else
		{
			MPU6050_I2C_SDA_0();		//	SDAд0
		}
		Delay();
		MPU6050_I2C_SCL_1();			//����SCL
		Delay();
		MPU6050_I2C_SCL_0();			//����SCL
		if(i == 7)
		{
			MPU6050_I2C_SDA_1();		//�ͷ�����
		}
		Data <<= 1;				//������һλ
	}
}

static uint8_t Read_Byte(void)
{
	uint8_t i = 0;
	uint8_t Value = 0;
	for(i = 0; i < 8; i++)
	{
		Value <<= 1;
		MPU6050_I2C_SCL_1();
		Delay();
		if(MPU6050_I2C_SDA_Read())
		{
			Value++;
		}
		MPU6050_I2C_SCL_0();
		Delay();
	}
	return Value;
}

uint8_t MPU6050_I2C_WaitAck(void)
{
	uint8_t re;

	MPU6050_I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	Delay();
	MPU6050_I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	Delay();
	if (MPU6050_I2C_SDA_Read())	/* CPU��ȡSDA����״̬ */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	MPU6050_I2C_SCL_0();
	Delay();
	return re;
}

void MPU6050_I2C_Send_Ack(uint8_t ACK)			//ACK = 1 ��ʾӦ��ɼ�����������
{
	MPU6050_I2C_SCL_0();
	if (ACK)
	{
		MPU6050_I2C_SDA_0();
	}
	else
	{
		MPU6050_I2C_SDA_1();
	}
	Delay();
	MPU6050_I2C_SCL_1();
	Delay();
	MPU6050_I2C_SCL_0();
	MPU6050_I2C_SDA_1();
	Delay();
}

void Write_MPU6050(uint8_t addr, uint8_t Data)
{
	MPU6050_I2C_Start();
	
	Send_Byte(ADDRESS_MPU6050);
	MPU6050_I2C_WaitAck();

	Send_Byte(addr);
	MPU6050_I2C_WaitAck();
	
	Send_Byte(Data);
	MPU6050_I2C_WaitAck();
	
	MPU6050_I2C_Stop();

}

void Read_MPU6050(uint8_t addr,uint8_t *p, uint8_t Num)
{
	uint8_t i = 0;
//	do
//	{
//		MPU6050_I2C_Start();
//	
//		Send_Byte(Slave_Addr);
//		
//	}
//	while(MPU6050_I2C_WaitAck());
	
	MPU6050_I2C_Start();
	
	Send_Byte(ADDRESS_MPU6050);
	MPU6050_I2C_WaitAck();
	
	Send_Byte(addr);
	MPU6050_I2C_WaitAck();
	
//	MPU6050_I2C_Stop();
	
	MPU6050_I2C_Start();

	Send_Byte(ADDRESS_MPU6050 + 1);
	MPU6050_I2C_WaitAck();
	
	for(i = 0;i < (Num - 1);i++)
	{
		*p = Read_Byte();
		MPU6050_I2C_Send_Ack(1);
		p++;
	}
	*p = Read_Byte();
	MPU6050_I2C_Send_Ack(0);

	MPU6050_I2C_Stop();
}

void MPU6050_MPU6050_I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	
	/* ʹ�� MPU6050_I2C_GPIO ʱ�� */
	RCC_APB2PeriphClockCmd(MPU6050_RCC_I2C_PORT, ENABLE);
	
	
	/* ����SCL SDA���� ��©��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  	GPIO_InitStructure.GPIO_Pin = MPU6050_I2C_SCL_PIN | MPU6050_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(MPU6050_GPIO_PORT_I2C, &GPIO_InitStructure);
	
	MPU6050_I2C_Stop();
}


void MPU6050_Init(void)
{
	uint8_t my_address;
	
	Write_MPU6050(PWR_MGMT_1, 0x00);	//�������״̬
	Write_MPU6050(SMPLRT_DIV, 0x07);  //������125hz
	Write_MPU6050(CONFIG, 0x04);      //21HZ�˲� ��ʱA8.5ms G8.3ms  �˴�ȡֵӦ�൱ע�⣬��ʱ��ϵͳ�������Ϊ��
	Write_MPU6050(GYRO_CONFIG, 0x08); //������500��/S 65.5LSB/g
	Write_MPU6050(ACCEL_CONFIG, 0x00);//���ٶ�+-4g  8192LSB/g
	
	Read_MPU6050(WHO_AM_I, &my_address, 1);
	printf("%d\r\n",my_address);
	if(my_address == 0x68)
	printf("mpu6050 Init Successful\r\n");
}


void MUP6050_ACCEL(void)
{
	uint8_t buf[6];
	char xbuf[16];
	char ybuf[16];
	char zbuf[16];
	Read_MPU6050(ACCEL_XOUT_H, buf, 6);
	
    ACCEL[0] = (buf[0] << 8) | buf[1];
    ACCEL[1] = (buf[2] << 8) | buf[3];
    ACCEL[2] = (buf[4] << 8) | buf[5];
	
	sprintf(xbuf,"x = %d",ACCEL[0]);
	sprintf(ybuf,"y = %d",ACCEL[1]);
	sprintf(zbuf,"z = %d",ACCEL[2]);
	
	OLED_flash_String_8(0,30,xbuf);
	OLED_flash_String_8(2,30,ybuf);
	OLED_flash_String_8(4,30,zbuf);
	KEY(Key_Handle);
}

void MOD_Display(void)
{
	char buf[16];
	sprintf(buf,"MOD = %d",MOD);
	OLED_flash_String_8(0,0,buf);
	KEY(Key_Handle);
}
