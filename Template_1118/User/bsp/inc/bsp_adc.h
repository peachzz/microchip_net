#ifndef __ADC_H__
#define	__ADC_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
/********************ADC����ͨ�������ţ�����**************************/
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd

#define    ADCx                          ADC1
#define    ADC_CLK                       RCC_APB2Periph_ADC1
#define    ADC_DMA_CHANNEL               DMA1_Channel1
#define    ADC_CHANNEL                   ADC_Channel_16

/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void ADCx_Init(void);

#endif /* __ADC_H__ */

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/