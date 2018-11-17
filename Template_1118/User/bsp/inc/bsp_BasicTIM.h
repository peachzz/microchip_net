#ifndef __BASIC_TIM_H__
#define __BASIC_TIM_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/

/********************������ʱ��TIM�������壬ֻ��TIM6 & TIM7************/
// ���ʹ��TIM7��ע�͵�����꼴��
#define USE_TIM6

#ifdef  USE_TIM6  // ʹ�û�����ʱ��TIM6
#define BASIC_TIMx                 TIM6
#define BASIC_TIM_APBxClock_FUN    RCC_APB1PeriphClockCmd
#define BASIC_TIM_CLK              RCC_APB1Periph_TIM6
#define BASIC_TIM_IRQ              TIM6_IRQn
#define BASIC_TIM_INT_FUN          TIM6_IRQHandler

#else  // ʹ�û�����ʱ��TIM7

#define BASIC_TIMx                 TIM7
#define BASIC_TIM_APBxClock_FUN    RCC_APB1PeriphClockCmd
#define BASIC_TIM_CLK              RCC_APB1Periph_TIM7
#define BASIC_TIM_IRQ              TIM7_IRQn
#define BASIC_TIM_INT_FUN          TIM7_IRQHandler

#endif

/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/

void BASIC_TIMx_Configuration(void);

#endif	/* __BASIC_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/