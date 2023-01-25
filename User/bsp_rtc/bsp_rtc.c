#include "bsp_rtc.h"

static uint8_t x_dex = 0;
static uint8_t x_target = 0;
static uint8_t y_dex = 55;
static uint8_t y_target = 55;

#define FEBRUARY		2
#define	STARTOFTIME		1970
#define SECDAY			86400L           /*  һ���ж���s */
#define SECYR			(SECDAY * 365)
#define	leapyear(year)		((year) % 4 == 0)
#define	days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define	days_in_month(a) 	(month_days[(a) - 1])

static int month_days[12] = {	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
char const *en_WEEK_STR[] = { "Sun","Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
__IO uint32_t TimeDisplay = 0;

struct rtc_time systmtime=
{
0,0,0,18,1,2023,0
};


static void UI_Draw_square(uint8_t length,uint8_t weighth);

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
				Time_Adjust(&systmtime);
				MUNE_RETURN();
			}
		}
		else
		{
			if( Key_msg.id == 0 )
			{
				if(y_target < 8)
					y_target = 55;
				else
				y_target = (y_target - 8) % 55;
				
			}
			else if(Key_msg.id == 1)
			{
				switch(( 55 - y_dex ) / 8)
				{
					case 0:systmtime.tm_year = ( systmtime.tm_year + 1 ) % 2040;break;
					case 1:systmtime.tm_mon = ( systmtime.tm_mon + 1 ) % 13;if(systmtime.tm_mon == 0)systmtime.tm_mon = 1;break;
					case 2:systmtime.tm_mday = ( systmtime.tm_mday + 1 ) % 32;if(systmtime.tm_mday == 0)systmtime.tm_mday = 1;break;
					case 3:systmtime.tm_wday = ( systmtime.tm_wday + 1 ) % 7;break;
					case 4:systmtime.tm_hour = ( systmtime.tm_hour + 1 ) % 24;break;
					case 5:systmtime.tm_min = ( systmtime.tm_min + 1 ) % 60;break;
					case 6:systmtime.tm_sec = ( systmtime.tm_sec + 1 ) % 60;break;
					default:break;
				}
			}
		}
	}
}



static void My_RTC_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	
	NVIC_Init(&NVIC_InitStruct);
}

static void RTC_Configuration(void)
{
	/* ʹ�� PWR �� Backup ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* �������� Backup ���� */
	PWR_BackupAccessCmd(ENABLE);
	
	/* ��λ Backup ���� */
//	BKP_DeInit();/*****/
	
//ʹ���ⲿʱ�ӻ����ڲ�ʱ�ӣ���bsp_rtc.h�ļ����壩	
//ʹ���ⲿʱ��ʱ������Щ����¾�������
//������Ʒ��ʱ�򣬺����׳����ⲿ����������������̫�ɿ�	
#ifdef 	RTC_CLOCK_SOURCE_LSE
	/* ʹ�� LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* �ȴ� LSE ׼���� */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{}
	
	/* ѡ�� LSE ��Ϊ RTC ʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* ʹ�� RTC ʱ�� */
	RCC_RTCCLKCmd(ENABLE);
	
	/* �ȴ� RTC �Ĵ��� ͬ��
	 * ��ΪRTCʱ���ǵ��ٵģ��ڻ�ʱ���Ǹ��ٵģ�����Ҫͬ��
	 */
	RTC_WaitForSynchro();
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	
	/* ʹ�� RTC ���ж� */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	
	/* ���� RTC ��Ƶ: ʹ RTC ����Ϊ1s  */
	/* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */
	RTC_SetPrescaler(32767); 
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	
#else

	/* ʹ�� LSI */
	RCC_LSICmd(ENABLE);

	/* �ȴ� LSI ׼���� */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}
	
	/* ѡ�� LSI ��Ϊ RTC ʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
	/* ʹ�� RTC ʱ�� */
	RCC_RTCCLKCmd(ENABLE);
	
	/* �ȴ� RTC �Ĵ��� ͬ��
	 * ��ΪRTCʱ���ǵ��ٵģ��ڻ�ʱ���Ǹ��ٵģ�����Ҫͬ��
	 */
	RTC_WaitForSynchro();
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	
	/* ʹ�� RTC ���ж� */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	
	/* ���� RTC ��Ƶ: ʹ RTC ����Ϊ1s ,LSIԼΪ40KHz */
	/* RTC period = RTCCLK/RTC_PR = (40 KHz)/(40000-1+1) = 1HZ */	
	RTC_SetPrescaler(40000-1); 
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
#endif
}

 /*���㹫��*/
void GregorianDay(struct rtc_time * tm)
{
	int leapsToDate;
	int lastYear;
	int day;
	int MonthOffset[] = { 0,31,59,90,120,151,181,212,243,273,304,334 };

	lastYear=tm->tm_year-1;

	/*����ӹ�ԪԪ�굽������ǰһ��֮��һ�������˶��ٸ�����*/
	leapsToDate = lastYear/4 - lastYear/100 + lastYear/400;      

     /*������������һ��Ϊ���꣬�Ҽ������·���2��֮����������1�����򲻼�1*/
	if((tm->tm_year%4==0) &&
	   ((tm->tm_year%100!=0) || (tm->tm_year%400==0)) &&
	   (tm->tm_mon>2)) {
		/*
		 * We are past Feb. 29 in a leap year
		 */
		day=1;
	} else {
		day=0;
	}

	day += lastYear*365 + leapsToDate + MonthOffset[tm->tm_mon-1] + tm->tm_mday; /*����ӹ�ԪԪ��Ԫ������������һ���ж�����*/

	tm->tm_wday=day%7;
}

void to_tm(u32 tim, struct rtc_time * tm)
{
	register u32    i;
	register long   hms, day;

	day = tim / SECDAY;			/* �ж����� */
	hms = tim % SECDAY;			/* �����ʱ�䣬��λs */

	/* Hours, minutes, seconds are easy */
	tm->tm_hour = hms / 3600;
	tm->tm_min = (hms % 3600) / 60;
	tm->tm_sec = (hms % 3600) % 60;

	/* Number of years in days */ /*�����ǰ��ݣ���ʼ�ļ������Ϊ1970��*/
	for (i = STARTOFTIME; day >= days_in_year(i); i++) {
		day -= days_in_year(i);
	}
	tm->tm_year = i;

	/* Number of months in days left */ /*���㵱ǰ���·�*/
	if (leapyear(tm->tm_year)) {
		days_in_month(FEBRUARY) = 29;
	}
	for (i = 1; day >= days_in_month(i); i++) {
		day -= days_in_month(i);
	}
	days_in_month(FEBRUARY) = 28;
	tm->tm_mon = i;

	/* Days are what is left over (+1) from all that. *//*���㵱ǰ����*/
	tm->tm_mday = day + 1;

	/*
	 * Determine the day of week
	 */
	GregorianDay(tm);
	
}

u32 mktimev(struct rtc_time *tm)
{
	if (0 >= (int) (tm->tm_mon -= 2)) {	/* 1..12 -> 11,12,1..10 */
		tm->tm_mon += 12;		/* Puts Feb last since it has leap day */
		tm->tm_year -= 1;
	}

	return (((
		(u32) (tm->tm_year/4 - tm->tm_year/100 + tm->tm_year/400 + 367*tm->tm_mon/12 + tm->tm_mday) +
			tm->tm_year*365 - 719499
	    )*24 + tm->tm_hour /* now have hours */
	  )*60 + tm->tm_min /* now have minutes */
	)*60 + tm->tm_sec; /* finally seconds */	 
}

void Time_Adjust(struct rtc_time *tm)
{
	
			/* RTC ���� */
		RTC_Configuration();

	  /* �ȴ�ȷ����һ�β������ */
	  RTC_WaitForLastTask();
		  
	  /* �������� */
	  GregorianDay(tm);

	  /* �����ڼ���ʱ�����д�뵽RTC�����Ĵ��� */
	  RTC_SetCounter(mktimev(tm)-TIME_ZOOM);

	  /* �ȴ�ȷ����һ�β������ */
	  RTC_WaitForLastTask();
}

void My_RTC_Config(void)
{
	My_RTC_NVIC_Config();
	
	/* ʹ�� PWR �� Backup ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); // ����PWR��BKP��ʱ��

	PWR_BackupAccessCmd(ENABLE); // ʹ��BKP�Ĵ���������
	
	if (BKP_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
	{
		printf("\r\nBKP = %d",BKP_ReadBackupRegister(RTC_BKP_DRX));
		Time_Adjust(&systmtime);
		/* ʹ�� PWR �� Backup ʱ�� */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); // ����PWR��BKP��ʱ��

		PWR_BackupAccessCmd(ENABLE); // ʹ��BKP�Ĵ���������
		/*��BKP_DR1�Ĵ���д���־��˵��RTC��������*/
		BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	}
	else
	{
		/* ʹ�� PWR �� Backup ʱ�� */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

		/* �������� Backup ���� */
		PWR_BackupAccessCmd(ENABLE);

		/*LSE��������������ʱ��*/

		#ifdef RTC_CLOCK_SOURCE_LSI		
		/* ʹ�� LSI */
		RCC_LSICmd(ENABLE);

		/* �ȴ� LSI ׼���� */
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
		{}
		#endif

		/*����Ƿ��������*/
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
		printf("\r\n\r\n Power On Reset occurred....");
		}
		/*����Ƿ�Reset��λ*/
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
		printf("\r\n\r\n External Reset occurred....");
		}

		printf("\r\n No need to configure RTC....");

		/*�ȴ��Ĵ���ͬ��*/
		RTC_WaitForSynchro();

		/*����RTC���ж�*/
		RTC_ITConfig(RTC_IT_SEC, ENABLE);

		/*�ȴ��ϴ�RTC�Ĵ���д�������*/
		RTC_WaitForLastTask();
	}
	/*������ʱ������꣬������У��ʱ�������PC13*/
	#ifdef RTCClockOutput_Enable
	/* ʹ�� PWR �� Backup ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	/* �������� Backup ���� */
	PWR_BackupAccessCmd(ENABLE);

	/* ��ֹ Tamper ���� */
	/* Ҫ��� RTCCLK/64 �� Tamper ����,  tamper ���ܱ����ֹ */	
	BKP_TamperPinCmd(DISABLE); 

	/* ʹ�� RTC ʱ������� Tamper ���� */
	BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
	#endif

	/* �����λ��־ flags */
	RCC_ClearFlag();
	while(TimeDisplay == 0);
}

void Time_Display(uint32_t TimeVar,struct rtc_time *tm)
{
	   uint32_t BJ_TimeVar;	

	   /*  �ѱ�׼ʱ��ת��Ϊ����ʱ��*/
	   BJ_TimeVar =TimeVar + TIME_ZOOM;

	   to_tm(BJ_TimeVar, tm);/*�Ѷ�ʱ����ֵת��Ϊ����ʱ��*/	
}


void Time_Set_Funcation(void)
{
	char buf[10];
	
	sprintf(buf,"<TIME__SET>");
	OLED_flash_String_8(0,20,buf);
	
	sprintf(buf,"year:%4d",systmtime.tm_year);
	OLED_flash_String_8(1, 0,buf);
	sprintf(buf,"mon :  %2d",systmtime.tm_mon);
	OLED_flash_String_8(2, 0,buf);
	sprintf(buf,"mday:  %2d",systmtime.tm_mday);
	OLED_flash_String_8(3, 0,buf);
	sprintf(buf,"wday:  %2d",systmtime.tm_wday);
	OLED_flash_String_8(4, 0,buf);
	sprintf(buf,"hour:  %2d",systmtime.tm_hour);
	OLED_flash_String_8(5, 0,buf);
	sprintf(buf,"min :  %2d",systmtime.tm_min);
	OLED_flash_String_8(6, 0,buf);
	sprintf(buf,"sec :  %2d",systmtime.tm_sec);
	OLED_flash_String_8(7, 0,buf);
	
	UI_Draw_square(72,8);
	
	KEY(Key_Handle);
}

static void UI_Draw_square(uint8_t length,uint8_t weighth)
{
	if(x_dex != x_target || y_dex != y_target)		//��̬
	{
		Clear_square(x_dex,y_dex,length,weighth);	//����
		if(x_dex < x_target)
		{
			x_dex++;
		}
		else if(x_dex > x_target)
		{
			x_dex--;
		}
		if(y_dex < y_target)
		{
			y_dex++;
		}
		else if(y_dex > y_target)
		{
			y_dex--;
		}
		Draw_square(x_dex,y_dex,length,weighth);	//�ع�
	}
	else											//��̬
	{
		Draw_square(x_dex,y_dex,length,weighth);
	}
}
