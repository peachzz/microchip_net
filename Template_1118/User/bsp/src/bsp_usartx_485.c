/**
  ******************************************************************************
  * �ļ�����: bsp_usartx_485.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ����RS485�ײ���������
  *           ��������Ϊ��������Ϊ�ӻ�����������һ����
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "bsp/485/bsp_usartx_485.h"
#include <stdarg.h>

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: USARTx GPIO ����,����ģʽ����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void RS485_USARTx_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config RS485_USARTx clock */
	RS485_USARTx_GPIO_ClockCmd(RS485_USARTx_TX_CLK|RS485_USARTx_RX_CLK, ENABLE);
	RS485_REDE_GPIO_ClockCmd(RS485_REDE_GPIO_CLK, ENABLE);
	RS485_USARTx_ClockCmd(RS485_USARTx_CLK, ENABLE);

	/* RS485_USARTx GPIO config */
   /* Configure RS485_USARTx Tx  as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = RS485_USARTx_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS485_USARTx_TX_PORT, &GPIO_InitStructure);
	    
  /* Configure RS485_USARTx Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = RS485_USARTx_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(RS485_USARTx_RX_PORT, &GPIO_InitStructure);
  
	/* RS485���շ���ʹ��GPIO */
  GPIO_InitStructure.GPIO_Pin = RS485_REDE_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS485_REDE_PORT, &GPIO_InitStructure);
	  
	/* USART������������ */
  /* USART������:115200 */
	USART_InitStructure.USART_BaudRate = RS485_USARTx_BAUDRATE;
	/* USART�ֳ�(��Чλ)��8λ */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	/* USARTֹͣλ��1λ */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/* USARTУ��λ���� */
  USART_InitStructure.USART_Parity = USART_Parity_No ;
	/* USARTӲ������������(Ӳ���źſ��ƴ���ֹͣ)���� */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/* USART����ģʽʹ�ܣ�������պͷ��� */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* ��ʼ��USART */
  USART_Init(RS485_USARTx, &USART_InitStructure); 
  /* ʹ��USART */
  USART_Cmd(RS485_USARTx, ENABLE);
}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
