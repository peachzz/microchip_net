#ifndef __RS485_USARTx_H__
#define	__RS485_USARTx_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include <stm32f10x.h>
#include <stdio.h>

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
// 485  ʹ������GPIO RE/DE�궨��
#define RS485_REDE_GPIO_ClockCmd               RCC_APB2PeriphClockCmd
#define RS485_REDE_GPIO_CLK                    RCC_APB2Periph_GPIOB     
#define RS485_REDE_PORT                        GPIOB   
#define RS485_REDE_PIN                         GPIO_Pin_2

#define RS485_USARTx                           USART3
#define RS485_USARTx_BAUDRATE                  115200

#define RS485_USARTx_ClockCmd                  RCC_APB1PeriphClockCmd
#define RS485_USARTx_CLK                       RCC_APB1Periph_USART3

#define RS485_USARTx_GPIO_ClockCmd             RCC_APB2PeriphClockCmd
#define RS485_USARTx_TX_PORT                   GPIOB   
#define RS485_USARTx_TX_PIN                    GPIO_Pin_10
#define RS485_USARTx_TX_CLK                    RCC_APB2Periph_GPIOB 

#define RS485_USARTx_RX_PORT                   GPIOB 
#define RS485_USARTx_RX_PIN                    GPIO_Pin_11
#define RS485_USARTx_RX_CLK                    RCC_APB2Periph_GPIOB

/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void RS485_USARTx_Config(void);

#endif /* __RS485_USARTx_H__ */

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
