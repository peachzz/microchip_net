#ifndef __BSP_SRAM_H__
#define __BSP_SRAM_H__															    

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
//ʹ��NOR/SRAM�� Bank1.sector3
//��IS61LV51216/IS62WV51216,��ַ�߷�ΧΪA0~A18
#define Bank1_SRAM3_ADDR    ((uint32_t)(0x68000000))		

/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/		  
void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumHalfwordToWrite);
void FSMC_SRAM_ReadBuffer(uint8_t* pBuffer,uint32_t ReadAddr,uint32_t NumHalfwordToRead);

void fsmc_sram_test_write(uint8_t data,uint32_t addr);
uint8_t fsmc_sram_test_read(uint32_t addr);

#endif  /* __BSP_SRAM_H__ */

