/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: DACʵ�����Ҳ����
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
#include "bsp/dac/bsp_dac.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
#define DAC_DHR12RD_Address      0x40007420 

/* ˽�б��� ------------------------------------------------------------------*/
/* �������� ---------------------------------------------------------*/
const uint16_t CH_1[32] = {
	2448,2832,3186,3496,3751,3940,4057,4095,4057,3940,
	3751,3496,3186,2832,2448,2048,1648,1264,910,600,345,
	156,39,0,39,156,345,600,910,1264,1648,2048
};

uint32_t SJ_1[32];
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ʹ��DAC��ʱ�ӣ���ʼ��GPIO
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void DAC_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef  DAC_InitStructure;

  /* ʹ��GPIOAʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	/* ʹ��DACʱ�� */	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
  /* DAC��GPIO���ã�ģ������ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* ����DAC ͨ��1 */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;						//ʹ��TIM2��Ϊ����Դ
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	//��ʹ�ò��η�����
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;	//��ʹ��DAC�������
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  /* ʹ��ͨ��1 ��PA4��� */
  DAC_Cmd(DAC_Channel_1, ENABLE);

  /* ʹ��DAC��DMA���� */
  DAC_DMACmd(DAC_Channel_1, ENABLE);
}

/**
  * ��������: ����TIM
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void DAC_TIM_Config(void)
{	
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	
	/* ʹ��TIM2ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
  /* TIM2������ʱ������ */
  TIM_TimeBaseStructure.TIM_Period = 7200;       									//��ʱ���� 20  
  TIM_TimeBaseStructure.TIM_Prescaler = 1000;       							//Ԥ��Ƶ������Ƶ 72M / (0+1) = 72M
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    						//ʱ�ӷ�Ƶϵ��
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//���ϼ���ģʽ
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* ����TIM2����Դ */
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

	/* ʹ��TIM2 */
  TIM_Cmd(TIM2, ENABLE);

}

/**
  * ��������: ����DMA
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void DAC_DMA_Config(void)
{	
	DMA_InitTypeDef  DMA_InitStructure;

	/* ʹ��DMA2ʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	
	/* ����DMA2 */
  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12RD_Address;					//�������ݵ�ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&SJ_1 ;				        //�ڴ����ݵ�ַ DualSine12bit
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;											//���ݴ��䷽���ڴ�������
  DMA_InitStructure.DMA_BufferSize = 32;																	//�����СΪ32�ֽ�	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//�������ݵ�ַ�̶�	
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									//�ڴ����ݵ�ַ����
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	//������������Ϊ��λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;					//�ڴ���������Ϊ��λ	
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;													//ѭ��ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;											//��DMAͨ�����ȼ�
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														//���ڴ����ڴ�ģʽ	

  DMA_Init(DMA2_Channel3, &DMA_InitStructure);
  
  /* ʹ��DMA2-3ͨ�� */
  DMA_Cmd(DMA2_Channel3, ENABLE);
  
}

/**
  * ��������: DAC��ʼ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void DAC_Mode_Init(void)
{
	uint32_t Idx = 0;  

	DAC_Config();
	DAC_TIM_Config();
	DAC_DMA_Config();
	
	/* ������Ҳ������ݣ�˫ͨ���Ҷ���*/
  for (Idx = 0; Idx < 32; Idx++)
  {
    SJ_1[Idx] =CH_1[Idx];
  }
}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
