/**
  ******************************************************************************
  * �ļ�����: bsp_key.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ���ض��������ײ���������
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
#include "bsp_key.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ���ذ���IO���ų�ʼ��.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����ʹ�ú궨�巽������������źţ����������ֲ��ֻҪ���޸�bsp_key.h
  *           �ļ���غ궨��Ϳ��Է����޸����š�
  */
void KEY_GPIO_Init(void)
{
   /* ����IOӲ����ʼ���ṹ����� */
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʹ��(����)KEY1���Ŷ�ӦIO�˿�ʱ�� */  
  RCC_APB2PeriphClockCmd(KEY1_RCC_CLOCKGPIO|KEY2_RCC_CLOCKGPIO, ENABLE);
   
  /* �趨KEY1��Ӧ����IO��� */
  GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;  
  /* �趨KEY1��Ӧ����IO�������ٶ� ��GPIO_Speed_50MHz */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  /* �趨KEY1��Ӧ����IOΪ��������ģʽ */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  /* ��ʼ��KEY1��Ӧ����IO */
  GPIO_Init(KEY1_GPIO, &GPIO_InitStructure);
 
  /* �趨KEY2��Ӧ����IO��� */
  GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;  
  /* ��ʼ��KEY2��Ӧ����IO */
  GPIO_Init(KEY2_GPIO, &GPIO_InitStructure);
}


void EXTI_Inits(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
	
	/* Enable the EXTI2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;				 //�ⲿ�ж�1
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     //��ռ���ȼ� 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			 //�����ȼ�0  
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //ʹ��
  NVIC_Init(&NVIC_InitStructure);
	
	
	 //��������AFIO�ⲿ�ж����üĴ���AFIO_EXTICR1������ѡ��EXTI2�ⲿ�жϵ�����Դ��PE2��
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);     //�ⲿ�ж�����AFIO--ETXI2

  EXTI_InitStructure.EXTI_Line = EXTI_Line1;					  //PE2 ��Ϊ���̵����ߡ����״̬
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			  //�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		  //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
}

void GPIOA1_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	         	 	//ENC28J60��������ж����� 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   	 		//�ڲ���������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * ��������: �򵥴ֱ�����ʱ����
  * �������: time����ʱʱ������
  * �� �� ֵ: ��
  * ˵    ���������ڰ����뵯�����̴��ڶ�����һ����Ҫ������������������򵥷���
  *           ���Ǽ���ʱ��
  */
void KEY_ScanDelay(void)
{  
  uint32_t i,j;
  for(i=0;i<100;++i)
    for(j=0;j<1000;++j){ }		
}

/**
  * ��������: ��ȡ����KEY1��״̬
  * �����������
  * �� �� ֵ: KEY_DOWN�����������£�
  *           KEY_UP  ������û������
  * ˵    �����ޡ�
  */
KEYState_TypeDef KEY1_StateRead(void)
{
  /* ��ȡ��ʱ����ֵ���ж��Ƿ��Ǳ�����״̬������Ǳ�����״̬���뺯���� */
  if(GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_PIN)==KEY1_DOWN_LEVEL)
  {
    /* ��ʱһС��ʱ�䣬�������� */
    KEY_ScanDelay();
    /* ��ʱʱ��������жϰ���״̬��������ǰ���״̬˵������ȷʵ������ */
    if(GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_PIN)==KEY1_DOWN_LEVEL)
    {
      /* �ȴ������������˳�����ɨ�躯�� */
      while(GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_PIN)==KEY1_DOWN_LEVEL);
       /* ����ɨ����ϣ�ȷ�����������£����ذ���������״̬ */
      return KEY_DOWN;
    }
  }
  /* ����û�����£�����û������״̬ */
  return KEY_UP;
}

/**
  * ��������: ��ȡ����KEY2��״̬
  * �����������
  * �� �� ֵ: KEY_DOWN�����������£�
  *           KEY_UP  ������û������
  * ˵    �����ޡ�
  */
KEYState_TypeDef KEY2_StateRead(void)
{
  /* ��ȡ��ʱ����ֵ���ж��Ƿ��Ǳ�����״̬������Ǳ�����״̬���뺯���� */
  if(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_PIN)==KEY2_DOWN_LEVEL)
  {
    /* ��ʱһС��ʱ�䣬�������� */
    KEY_ScanDelay();
    /* ��ʱʱ��������жϰ���״̬��������ǰ���״̬˵������ȷʵ������ */
    if(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_PIN)==KEY2_DOWN_LEVEL)
    {
      /* �ȴ������������˳�����ɨ�躯�� */
      while(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_PIN)==KEY2_DOWN_LEVEL);
       /* ����ɨ����ϣ�ȷ�����������£����ذ���������״̬ */
      return KEY_DOWN;
    }
  }
  /* ����û�����£�����û������״̬ */
  return KEY_UP;
}

#if 0

/**
  * ��������: ����KEY1��Ϊ�ж����Ų�ʹ���ж�
  * �����������
  * �� �� ֵ: ��
  * ˵    ������������KEY1Ϊ�������жϣ��鿴������ԭ��ͼ��֪��KEY1��û������ʱPA0Ϊ
  *           �͵�ƽ(�����������ý��)�����Ե����ǰ��°���ʱ����һ���ӵ͵�ƽ��Ϊ��
  *           ��ƽ���̣�stm32f103оƬ���԰�����̼����������������������Դ��Ϳ�
  *           ����stm32�Ѽ�⵽���ƽ�仯��Ϣ���߸����ǣ��Ӷ�����֪�������ǵĶ���
  *           Ч����������һ����������
  */
void KEY1_EXIT_Config(void)
{
  /* ����IOӲ����ʼ���ṹ����� */
  GPIO_InitTypeDef GPIO_InitStructure;
  /* �����ⲿ�ж��߳�ʼ���ṹ����� */
  EXTI_InitTypeDef EXTI_InitStructure;
  /* ����Ƕ�������жϿ�������ʼ���ṹ����� */
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ϊ����IO�����жϹ�����Ҫʹ�ܸ��ù���ʱ�� */
  KEY1_RCC_CLOCKCMD(KEY1_RCC_CLOCKGPIO | KEY1_RCC_CLOCKAFIO,ENABLE);
  
  /* �趨KEY1��Ӧ����IO��� */
  GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;  
  /* �趨KEY1��Ӧ����IO�������ٶ� ��GPIO_Speed_50MHz */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  /* �趨KEY1��Ӧ����IOΪ��������ģʽ */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  /* ��ʼ��KEY1��Ӧ����IO */
  GPIO_Init(KEY1_GPIO, &GPIO_InitStructure);

  /* ѡ��PA0��Ϊ�ж�����Դ */
  GPIO_EXTILineConfig(KEY1_GPIO_PORTSOURCE,KEY1_GPIO_PINSOURCE);
  
  /* KEY1��Ӧ�Ķ��� */
  EXTI_InitStructure.EXTI_Line=KEY1_EXITLINE;
  /* �ⲿ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  /* �½��ش�����ʽ */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* ʹ���ж� */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  /* ����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ��� */
  EXTI_Init(&EXTI_InitStructure);
  
  /* ѡ���ж����ȼ�������Ϊ4����ռʽ���ȼ���4�������ȼ������Բο�misc.h�ļ��˽�������� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ʹ��KEY1���ڵ��ⲿ�ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannel = KEY1_IRQCHANNEL;
  /* ������ռʽ���ȼ�Ϊ2 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; 
  /* ���������ȼ�Ϊ3 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
  /* ʹ���ⲿ�ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
  /* ��ʼ������Ƕ�������жϿ����� */
  NVIC_Init(&NVIC_InitStructure); 
}

/**
  * ��������: ����KEY2��Ϊ�ж����Ų�ʹ���ж�
  * �����������
  * �� �� ֵ: ��
  * ˵    ������������KEY2Ϊ�½����жϣ��鿴������ԭ��ͼ��֪��KEY2��û������ʱPC0Ϊ
  *           �ߵ�ƽ(�����������ý��)�����Ե����ǰ��°���ʱ����һ���Ӹߵ�ƽ��Ϊ��
  *           ��ƽ���̣�stm32f103оƬ���԰�����̼����������������������Դ��Ϳ�
  *           ����stm32�Ѽ�⵽���ƽ�仯��Ϣ���߸����ǣ��Ӷ�����֪�������ǵĶ���
  *           Ч����������һ����������
  */
void KEY2_EXIT_Config(void)
{
  /* ����IOӲ����ʼ���ṹ����� */
  GPIO_InitTypeDef GPIO_InitStructure;
  /* �����ⲿ�ж��߳�ʼ���ṹ����� */
  EXTI_InitTypeDef EXTI_InitStructure;
  /* ����Ƕ�������жϿ�������ʼ���ṹ����� */
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ϊ����IO�����жϹ�����Ҫʹ�ܸ��ù���ʱ�� */
  KEY2_RCC_CLOCKCMD(KEY2_RCC_CLOCKGPIO | KEY2_RCC_CLOCKAFIO,ENABLE);
  
  /* �趨KEY2��Ӧ����IO��� */
  GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;  
  /* �趨KEY2��Ӧ����IO�������ٶ� ��GPIO_Speed_50MHz */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  /* �趨KEY2��Ӧ����IOΪ��������ģʽ */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  /* ��ʼ��KEY2��Ӧ����IO */
  GPIO_Init(KEY2_GPIO, &GPIO_InitStructure);

  /* ѡ��KEY2��Ϊ�ж�����Դ */
  GPIO_EXTILineConfig(KEY2_GPIO_PORTSOURCE,KEY2_GPIO_PINSOURCE);
  
  /* KEY2��Ӧ���ж��� */
  EXTI_InitStructure.EXTI_Line=KEY2_EXITLINE;
  /* �ⲿ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  /* �½��ش�����ʽ */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* ʹ���ж� */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  /* ����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ��� */
  EXTI_Init(&EXTI_InitStructure);
  
  /* ѡ���ж����ȼ�������Ϊ4����ռʽ���ȼ���4�������ȼ������Բο�misc.h�ļ��˽�������� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ʹ��KEY2���ڵ��ⲿ�ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannel = KEY2_IRQCHANNEL;
  /* ������ռʽ���ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; 
  /* ���������ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  /* ʹ���ⲿ�ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
  /* ��ʼ������Ƕ�������жϿ����� */
  NVIC_Init(&NVIC_InitStructure); 
}


/**
  * ��������: KEY1�жϷ�����
  * �����������
  * �� �� ֵ: ��
  * ˵    ������stm32f103��⵽�½����źź���Զ������Ӧ���жϷ����������ǿ�����
  *           ��������ʵ��һЩ����
  */
void KEY1_IRQHANDLER(void)
{
  /* ȷ���Ƿ������EXTI Line�ж� */
	if(EXTI_GetITStatus(KEY1_EXITLINE) != RESET)
	{
		/* LED1�Ʒ�ת	*/	
		LED1_TOGGLE;
    /* �������� */
    BEEP_ON;
    /* ����жϱ�־λ	*/
		EXTI_ClearITPendingBit(KEY1_EXITLINE);     
	}
}

/**
  * ��������: KEY2�жϷ�����
  * �����������
  * �� �� ֵ: ��
  * ˵    ������stm32f103��⵽�½����źź���Զ������Ӧ���жϷ����������ǿ�����
  *           ��������ʵ��һЩ����
  */
void KEY2_IRQHANDLER(void)
{
  /* ȷ���Ƿ������EXTI Line�ж� */
	if(EXTI_GetITStatus(KEY2_EXITLINE) != RESET)
	{
    /* LED2�Ʒ�ת	*/	
    LED2_TOGGLE;
    /* ���������� */
    BEEP_OFF;
    /* ����жϱ�־λ	*/
		EXTI_ClearITPendingBit(KEY2_EXITLINE);     
	}
}
#endif
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
