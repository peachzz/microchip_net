#include "common.h"

void Bsp_init(void)
{
	 /* 初始化LED灯 */
  LED_GPIO_Init();
  
   /* 初始化系统滴答定时器 */  
  SysTick_Init();
	
	 /* 初始化调试串口 */  
	DEBUG_USART_Init();
}