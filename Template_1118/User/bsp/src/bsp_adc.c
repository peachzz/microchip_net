/**
  ******************************************************************************
  * �ļ�����: bsp_adc.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ������оƬ�ڲ��¶Ȳɼ��ײ���������
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "bsp_adc.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
__IO uint16_t ADC_ConvertedValue;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

#if 0

	 Current_Temperature = (V25-ADC_ConvertedValue)/AVG_SLOPE+25;	
	  /* 10������ʾ */
    printf("The IC current temperature = %3d ��\n", Current_Temperature);	   
		
#endif 
/**
  * ��������: ����ADC����ģʽ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;	
  DMA_InitTypeDef DMA_InitStructure;

  /* ����ADCʱ��ΪPCLK2��8��Ƶ����9MHz */
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
  
	/* ��ADCʱ�� */
	ADC_APBxClock_FUN(ADC_CLK,ENABLE );
	/* ��DMAʱ�� */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
  /* ��λDMA������ */
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	/* ���� DMA ��ʼ���ṹ�� */
	/* �����ַΪ��ADC ���ݼĴ�����ַ */
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(ADCx->DR));	
	/* �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı��� */
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;	
	/* ����Դ�������� */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	
	/* ���ݳ��� */
	DMA_InitStructure.DMA_BufferSize = 1;	
	/* ����Ĵ���ֻ��һ������ַ���õ��� */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	/* �洢����ַ�̶� */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 	
	/* �������ݴ�СΪ���֣��������ֽ� */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	
	/* �ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	/* ѭ������ģʽ */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
	/* DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ�� */
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	
	/* ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢�� */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	
	/* ��ʼ��DMA */
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	
	/* ʹ�� DMA ͨ�� */
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
  
	/* ADC ģʽ���� */
	/* ֻʹ��һ��ADC�����ڵ�ģʽ */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
	/* ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ */
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 
	/* ����ת��ģʽ */
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	/* �����ⲿ����ת���������������� */
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	/* ת������Ҷ��� */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	/* ת��ͨ��1�� */
	ADC_InitStructure.ADC_NbrOfChannel = 1;			
	/* ��ʼ��ADC */
	ADC_Init(ADCx, &ADC_InitStructure);
	
	/* ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ55.5��ʱ������ */
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_239Cycles5);
		
  /* ʹ���¶ȴ��������ڲ��ο���ѹ  */ 
  ADC_TempSensorVrefintCmd(ENABLE);
  
  /* ʹ��ADC DMA ���� */
	ADC_DMACmd(ADCx, ENABLE);
  
	/* ����ADC ������ʼת�� */
	ADC_Cmd(ADCx, ENABLE);
	
	/* ��ʼ��ADC У׼�Ĵ���   */
	ADC_ResetCalibration(ADCx);
	/*�ȴ�У׼�Ĵ�����ʼ����� */
	while(ADC_GetResetCalibrationStatus(ADCx));	
	/* ADC��ʼУ׼*/
	ADC_StartCalibration(ADCx);
	/*�ȴ�У׼��� */
	while(ADC_GetCalibrationStatus(ADCx));
	
	/* ����û�в����ⲿ����������ʹ����������ADCת��  */
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}

/**
  * ��������: ADC��ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void ADCx_Init(void)
{
	ADCx_Mode_Config();	
}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/