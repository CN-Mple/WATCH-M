#include "my_action.h"


#define Data_Length 4096 



enum PLAYER_STATE
{
	PLAYER_STOP,
	PLAYER_CONTINU,
	PLAYER_END,
	STATE_NUM,
};

uint8_t PLAYER_STATING = PLAYER_STOP;

//char ReadBuffer[Data_Length]={0};        /* �������� */

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
				
			}
		}
		else
		{
			if( Key_msg.id == 0 )
			{
				PLAYER_STATING = ( PLAYER_STATING + 1 ) % 2;
			}
			else if(Key_msg.id == 1)
			{
				PLAYER_STATING = PLAYER_END;
			}
		}
	}
}

void ACTION(void)
{
	FIL fnew;													/* �ļ����� */
	FRESULT res_sd;                /* �ļ�������� */
	UINT fnum;
	/*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
	printf("****** ���������ļ���ȡ����... ******\r\n");
	res_sd = f_open(&fnew, "0:badapple.ebm", FA_OPEN_EXISTING | FA_READ); 	 
	if(res_sd == FR_OK)
	{
		PLAYER_STATING = PLAYER_STOP;
		LED_GREEN;
		printf("�����ļ��ɹ���\r\n");
		while(1)
		{
			KEY(Key_Handle);
			switch(PLAYER_STATING)
			{
				case PLAYER_STOP:	break;
				case PLAYER_CONTINU:
									res_sd = f_read(&fnew, Grame, 1024, &fnum);
									if(fnum != 1024)
									{
										f_close(&fnew);
										PLAYER_STATING = PLAYER_END;
									}
									else
									{
										printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ( char* ) Grame);
										OLED_ReFresh(MOD);
									}break;
				case PLAYER_END:	f_close(&fnew);break;
			}
			if( PLAYER_STATING == PLAYER_END )
			break;
		}
		MUNE_RETURN();
	}
	else
	{
		LED_RED;
		printf("�������ļ�ʧ�ܡ�\r\n");
	}
}
