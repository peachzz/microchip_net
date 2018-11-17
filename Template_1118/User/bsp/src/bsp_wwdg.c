 /**
  ******************************************************************************
  * �ļ�����: bsp_wwdg.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ���ڿ��Ź��ײ���������
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
#include "bsp_wwdg.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: WWDG �ж����ȼ���ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void WWDG_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  /* ѡ���ж����ȼ�������Ϊ2����ռʽ���ȼ���2�������ȼ������Բο�misc.h�ļ��˽�������� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	/* ʹ��WWDG���ڵ��ⲿ�ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
	/* ������ռʽ���ȼ�Ϊ0 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	/* ���������ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* ʹ���ⲿ�ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * ��������: WWDG����
  * �������: tr ���ݼ���ʱ����ֵ�� ȡֵ��ΧΪ��0x7f~0x40
  *           wr ������ֵ��ȡֵ��ΧΪ��0x7f~0x40
  *           prv��Ԥ��Ƶ��ֵ��ȡֵ������
  *              ���� WWDG_Prescaler_1: WWDG counter clock = (PCLK1/4096)/1
  *              ���� WWDG_Prescaler_2: WWDG counter clock = (PCLK1/4096)/2
  *              ���� WWDG_Prescaler_4: WWDG counter clock = (PCLK1/4096)/4
  *              ���� WWDG_Prescaler_8: WWDG counter clock = (PCLK1/4096)/8
  * �� �� ֵ: ��
  * ˵    ������
  */
void WWDG_Config(uint8_t tr, uint8_t wr, uint32_t prv)
{	
	/* ���� WWDG ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	/* ���õݼ���������ֵ */
	WWDG_SetCounter( tr );
	
	/* ����Ԥ��Ƶ����ֵ */
	WWDG_SetPrescaler( prv );
	
	/* �����ϴ���ֵ */
	WWDG_SetWindowValue( wr );
	
	/* ���ü�������ֵ��ʹ��WWDG */
	WWDG_Enable(WWDG_CNT);	
	
	/* �����ǰ�����жϱ�־λ */
	WWDG_ClearFlag();	
	/* ����WWDG�ж����ȼ� */
	WWDG_NVIC_Config();	
	/* ��WWDG �ж� */
	WWDG_EnableIT();
}

/**
  * ��������: ι��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void WWDG_Feed(void)
{
	/* ι����ˢ�µݼ���������ֵ�����ó����WDG_CNT=0X7F */
	WWDG_SetCounter( WWDG_CNT );
}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
