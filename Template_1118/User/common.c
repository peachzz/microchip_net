#include "common.h"

void Bsp_init(void)
{
	 /* ��ʼ��LED�� */
  LED_GPIO_Init();
  
   /* ��ʼ��ϵͳ�δ�ʱ�� */  
  SysTick_Init();
	
	 /* ��ʼ�����Դ��� */  
	DEBUG_USART_Init();
}