/**
  ******************************************************************************
  * �ļ�����: bsp_iap.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: IAP�ײ�����ʵ��
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
#include "bsp/IAP/bsp_iap.h"
#include "bsp/stmflash/stm_flash.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
struct STRUCT_IAP_RECIEVE strAppBin  __attribute__ ( ( at ( 0x20001000 ) ) ) = { {0}, 0 };

static uint16_t ulBuf_Flash_App [ 1024 ];

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
void IAP_Write_App_Bin ( uint32_t ulStartAddr, uint8_t * pBin_DataBuf, uint32_t ulBufLength )
{
	uint16_t us, usCtr=0, usTemp;
	uint32_t ulAdd_Write = ulStartAddr;                                //��ǰд��ĵ�ַ
	uint8_t * pData = pBin_DataBuf;
	
	for ( us = 0; us < ulBufLength; us += 2 )
	{						    
		usTemp =  ( uint16_t ) pData[1]<<8;
		usTemp += ( uint16_t ) pData[0];	  
		pData += 2;                                                      //ƫ��2���ֽ�
		ulBuf_Flash_App [ usCtr ++ ] = usTemp;	    
		if ( usCtr == 1024 )
		{
			usCtr = 0;
			STMFLASH_Write ( ulAdd_Write, ulBuf_Flash_App, 1024 );	
			ulAdd_Write += 2048;                                           //ƫ��2048  16=2*8.����Ҫ����2.
		}
	}
	if ( usCtr ) 
    STMFLASH_Write ( ulAdd_Write, ulBuf_Flash_App, usCtr );//������һЩ�����ֽ�д��ȥ.  
}

__asm void MSR_MSP ( uint32_t ulAddr ) 
{
    MSR MSP, r0 			                   //set Main Stack value
    BX r14
}


//��ת��Ӧ�ó����
//ulAddr_App:�û�������ʼ��ַ.
void IAP_ExecuteApp ( uint32_t ulAddr_App )
{
	pIapFun_TypeDef pJump2App; 
	
	if ( ( ( * ( vu32 * ) ulAddr_App ) & 0x2FFE0000 ) == 0x20000000 )	  //���ջ����ַ�Ƿ�Ϸ�.
	{ 
		pJump2App = ( pIapFun_TypeDef ) * ( vu32 * ) ( ulAddr_App + 4 );	//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP ( * ( vu32 * ) ulAddr_App );					                            //��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		pJump2App ();								                                    	//��ת��APP.
	}
}		

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
