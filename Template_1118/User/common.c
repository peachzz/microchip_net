#include "common.h"

void Bsp_init(void)
{
	 /* ��ʼ��LED�� */
	LED_GPIO_Init();
	
   /* ��ʼ��ϵͳ�δ�ʱ�� */  
	SysTick_Init();
	
	 /* ��ʼ�����Դ��� */  
	DEBUG_USART_Init();
	DEBUG_INFO("DEBUG_USART_Init sucessful\n");
	/* ��ʼ��ENC28j60 spi  */  
	SPI1_Init();
	ENC28J60_EXTI_Inits();
	DEBUG_INFO("SPI1_Init sucessful\n");
	
	 
}
