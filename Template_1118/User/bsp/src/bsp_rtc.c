/**
  ******************************************************************************
  * �ļ�����: bsp_rtc.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ʵʱʱ�ӵײ���������
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "bsp/usart/bsp_debug_usart.h"
#include  "bsp/rtc/bsp_rtc.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/*���ڣ���Ф������ASCII��*/
uint8_t const *WEEK_STR[] = {"��", "һ", "��", "��", "��", "��", "��"};
uint8_t const *zodiac_sign[] = {"��", "��", "ţ", "��", "��", "��", "��", "��", "��", "��", "��", "��"};

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
void Time_Adjust(struct rtc_time *tm);
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ����RTC���жϵ����ж����ȼ�Ϊ1�������ȼ�Ϊ0
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void RTC_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * ��������: �򵥵���ʱ����
  * �������: time����ʱʱ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void Delay(uint16_t time)
{
  uint16_t i;
  while(--time)
  {
    for(i=0;i<2000;i++);
  }
}

/**
  * ��������: ����RTC
  * �������: ��
  * �� �� ֵ: 1��RTC���óɹ�������������0��RTC�޷�����ʹ��
  * ˵    ������
  */
static uint8_t RTC_Configuration(void)
{
  uint32_t count=0;
  
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	
	/* Reset Backup Domain */
	BKP_DeInit();
	
	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
     Delay(100);
     count++;
     if(count>4000)return 0;
  }
	
	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);
	
	/* Wait for RTC registers synchronization 
	 * ��ΪRTCʱ���ǵ��ٵģ��ڻ�ʱ���Ǹ��ٵģ�����Ҫͬ��
	 */
	RTC_WaitForSynchro();
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Enable the RTC Second */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
  
  return 1;
}

/**
  * ��������: ��鲢����RTC
  * �������: tm:���ڶ�ȡRTCʱ��Ľṹ��ָ��
  * �� �� ֵ: ��
  * ˵    ������
  */
uint8_t RTC_CheckAndConfig(struct rtc_time *tm)
{
  /* ����RTC���жϵ����ж����ȼ� */
  RTC_NVIC_Config();
  
  /*������ʱ��鱸�ݼĴ���BKP_DR1��������ݲ���0xA5A5,
	  ������������ʱ�䲢ѯ���û�����ʱ��*/
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		printf("RTC not yet configured....\n");

		/* RTC Configuration */
		if(RTC_Configuration()!=1)
      return 0;
		
		printf("RTC configured....\n");

		/* Adjust time by users typed on the hyperterminal */
		Time_Adjust(tm);

		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
	  /*��������������ʱ��*/
		/*����Ƿ��������*/
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
		    printf("Power On Reset occurred....\n");
		}
		/*����Ƿ�Reset��λ*/
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			printf("External Reset occurred....\n");
		}
	
		printf("No need to configure RTC....\n");
		
		/*�ȴ��Ĵ���ͬ��*/
		RTC_WaitForSynchro();
		
		/*����RTC���ж�*/
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		
		/*�ȴ��ϴ�RTC�Ĵ���д�������*/
		RTC_WaitForLastTask();
	}
	   /*������ʱ������꣬������У��ʱ�������PC13*/
	#ifdef RTCClockOutput_Enable
	  /* Enable PWR and BKP clocks */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	  /* Allow access to BKP Domain */
	  PWR_BackupAccessCmd(ENABLE);
	
	  /* Disable the Tamper Pin */
	  BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
	                                 functionality must be disabled */
	
	  /* Enable RTC Clock Output on Tamper Pin */
	  BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
	#endif
	
	  /* Clear reset flags */
	  RCC_ClearFlag();
  
  return 1;
}

/**
  * ��������: �Ӵ��ڵ������ֻ�ȡ����ֵ(��ASCII��ת��Ϊ����)
  * �������: value �û��ڳ����ն����������ֵ
  * �� �� ֵ: �����ַ���ASCII���Ӧ����ֵ
  * ˵    ����������ר����RTC��ȡʱ�䣬��������������Ӧ�ã�Ҫ�޸�һ��
  */
static uint8_t USART_Scanf(uint32_t value)
{
  uint32_t index = 0;
  uint32_t tmp[2] = {0, 0};

  while (index < 2)
  {
    /* Loop until RXNE = 1 */
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET)
    {
    }  
    tmp[index++] = (USART_ReceiveData(DEBUG_USARTx));
    if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))   /*����0��9��ASCII��Ϊ0x30��0x39*/
    { 		  
      printf("Please enter valid number between 0 and 9 -->:  \n");
      index--; 		 
    }
  }
  
  /* ���������ַ���ASCII��ת��Ϊ����*/
  index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
  
  /* Checks */
  if (index > value)
  {
    printf("Please enter valid number between 0 and %d\n", value);
    return 0xFF;
  }
  return index;
}

/**
  * ��������: ʱ�������������RTCʱ�ӽṹ��
  * �������: tm�����ڶ�ȡRTCʱ��Ľṹ��ָ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void Time_Regulate(struct rtc_time *tm)
{
#if 0
  tm->tm_year = 2015;
  tm->tm_mon  =   10;
  tm->tm_mday =    4;	
  tm->tm_hour =   10;	 
  tm->tm_min  =   15;	 
  tm->tm_sec  =   46;
#else
  u32 Tmp_YY = 0xFF, Tmp_MM = 0xFF, Tmp_DD = 0xFF, Tmp_HH = 0xFF, Tmp_MI = 0xFF, Tmp_SS = 0xFF;

  printf("=========================Time Settings==================\n");

  printf("���������(Please Set Years):  20\n");

  while (Tmp_YY == 0xFF)
  {
    Tmp_YY = USART_Scanf(99);
  }

  printf("��ݱ�����Ϊ:  20%0.2d\n", Tmp_YY);

  tm->tm_year = Tmp_YY+2000;

  Tmp_MM = 0xFF;

  printf("�������·�(Please Set Months):  \n");

  while (Tmp_MM == 0xFF)
  {
    Tmp_MM = USART_Scanf(12);
  }

  printf("�·ݱ�����Ϊ:  %d\n", Tmp_MM);

  tm->tm_mon= Tmp_MM;

  Tmp_DD = 0xFF;

  printf("����������(Please Set Dates):  \n");

  while (Tmp_DD == 0xFF)
  {
    Tmp_DD = USART_Scanf(31);
  }

  printf("���ڱ�����Ϊ:  %d\n", Tmp_DD);

  tm->tm_mday= Tmp_DD;

  Tmp_HH  = 0xFF;

  printf("������ʱ��(Please Set Hours):  \n");

  while (Tmp_HH == 0xFF)
  {
    Tmp_HH = USART_Scanf(23);
  }

  printf("ʱ�ӱ�����Ϊ:  %d\n", Tmp_HH );

  tm->tm_hour= Tmp_HH;
    
  Tmp_MI = 0xFF;

  printf("���������(Please Set Minutes):  \n");

  while (Tmp_MI == 0xFF)
  {
    Tmp_MI = USART_Scanf(59);
  }

  printf("���ӱ�����Ϊ:  %d\n", Tmp_MI);

  tm->tm_min= Tmp_MI;

  Tmp_SS = 0xFF;

  printf("����������(Please Set Seconds):  \n");

  while (Tmp_SS == 0xFF)
  {
    Tmp_SS = USART_Scanf(59);
  }

  printf("���ӱ�����Ϊ:  %d\n", Tmp_SS);

  tm->tm_sec= Tmp_SS;  
#endif
}

/**
  * ��������: ʱ�����ʵ��
  * �������: tm�����ڶ�ȡRTCʱ��Ľṹ��ָ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void Time_Adjust(struct rtc_time *tm)
{
	  /* Wait until last write operation on RTC registers has finished */
	  RTC_WaitForLastTask();
	
	  /* Get time entred by the user on the hyperterminal */
	  Time_Regulate(tm);
	  
	  /* Get wday */
	  GregorianDay(tm);

	  /* �޸ĵ�ǰRTC�����Ĵ������� */
	  RTC_SetCounter(mktimev(tm));

	  /* Wait until last write operation on RTC registers has finished */
	  RTC_WaitForLastTask();
}

/**
  * ��������: ��ʾ��ǰʱ��ֵ
  * �������: TimeVar RTC����ֵ����λΪ s��tm�����ڶ�ȡRTCʱ��Ľṹ��ָ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void Time_Display(uint32_t TimeVar,struct rtc_time *tm)
{
   static uint8_t FirstDisplay = 1;
   uint32_t BJ_TimeVar;
   uint8_t str[15]; // �ַ����ݴ�  	

   /*  �ѱ�׼ʱ��ת��Ϊ����ʱ��*/
   BJ_TimeVar =TimeVar + 8*60*60;

   to_tm(BJ_TimeVar, tm);/*�Ѷ�ʱ����ֵת��Ϊ����ʱ��*/	

  if((!tm->tm_hour && !tm->tm_min && !tm->tm_sec)  || (FirstDisplay))
  {
      
    GetChinaCalendar((u16)tm->tm_year, (u8)tm->tm_mon, (u8)tm->tm_mday, str);	
      printf("����������%0.2d%0.2d,%0.2d,%0.2d\n", str[0], str[1], str[2],  str[3]);

    GetChinaCalendarStr((u16)tm->tm_year,(u8)tm->tm_mon,(u8)tm->tm_mday,str);
      printf("����ũ����%s\n", str);

   if(GetJieQiStr((u16)tm->tm_year, (u8)tm->tm_mon, (u8)tm->tm_mday, str))
      printf("����ũ����%s\n", str);

    FirstDisplay = 0;
  }	 	  	
  /* ���ʱ���������ʱ�� */
  printf(" UNIXʱ��� = %d ��ǰʱ��Ϊ: %d��(%s��) %d�� %d�� (����%s)  %0.2d:%0.2d:%0.2d\n",TimeVar,
                    tm->tm_year, zodiac_sign[(tm->tm_year-3)%12], tm->tm_mon, tm->tm_mday, 
                    WEEK_STR[tm->tm_wday], tm->tm_hour, 
                    tm->tm_min, tm->tm_sec);
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
