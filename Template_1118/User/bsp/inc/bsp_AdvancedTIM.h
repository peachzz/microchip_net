#ifndef __ADVANCED_TIM_H__
#define __ADVANCED_TIM_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
/********************�߼���ʱ��TIM�������壬ֻ��TIM1 & TIM8************/
#define ADVANCED_TIMx                 TIM1
#define ADVANCED_TIM_APBxClock_FUN    RCC_APB2PeriphClockCmd
#define ADVANCED_TIM_CLK              RCC_APB2Periph_TIM1
#define ADVANCED_TIM_IRQ              TIM1_UP_IRQn
#define ADVANCED_TIM_INT_FUN          TIM1_UP_IRQHandler

//#define ADVANCED_TIMx                 TIM8
//#define ADVANCED_TIM_APBxClock_FUN    RCC_APB2PeriphClockCmd
//#define ADVANCED_TIM_CLK              RCC_APB2Periph_TIM8
//#define ADVANCED_TIM_IRQ              TIM8_UP_IRQn
//#define ADVANCED_TIM_INT_FUN          TIM8_UP_IRQHandler

/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/

void ADVANCED_TIMx_Configuration(void);

#endif	/* __ADVANCED_TIM_H__ */
