#include "common.h"

void Bsp_init(void)
{
	 /* 初始化LED灯 */
	LED_GPIO_Init();
	
   /* 初始化系统滴答定时器 */  
	SysTick_Init();
	
	 /* 初始化调试串口 */  
	DEBUG_USART_Init();
	DEBUG_INFO("DEBUG_USART_Init sucessful\n");
	/* 初始化ENC28j60 spi  */  
	SPI1_Init();
	ENC28J60_EXTI_Inits();
	DEBUG_INFO("SPI1_Init sucessful\n");
	
	 
}
