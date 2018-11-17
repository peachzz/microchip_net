#ifndef __COMMON_H__
#define	__COMMON_H__

#include "stm32f10x.h"

#include "bsp_led.h"

#include "bsp_beep.h"

#include "bsp_key.h"

#include "bsp_debug_usart.h"

#include "bsp_SysTick.h"

#include "bsp_GeneralTIM.h" 

#include "bsp_lcd.h"

#include "bsp_malloc.h"

#include "bsp_spi_flash.h"

#include "bsp_sram.h"

#include "bsp_wwdg.h"

#include "enc28j60.h"

#include "spi.h"

void Bsp_init(void);

#endif
