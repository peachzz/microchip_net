/**
  ******************************************************************************
  * �ļ�����: bsp_spi_flash.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ���ش���FlashоƬ�ײ���������
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
#include "bsp_spi_flash.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256
#define W25X_WriteEnable		            0x06 
#define W25X_WriteDisable		            0x04 
#define W25X_ReadStatusReg		          0x05 
#define W25X_WriteStatusReg		          0x01 
#define W25X_ReadData			              0x03 
#define W25X_FastReadData		            0x0B 
#define W25X_FastReadDual		            0x3B 
#define W25X_PageProgram		            0x02 
#define W25X_BlockErase			            0xD8 
#define W25X_SectorErase		            0x20 
#define W25X_ChipErase			            0xC7 
#define W25X_PowerDown			            0xB9 
#define W25X_ReleasePowerDown	          0xAB 
#define W25X_DeviceID			              0xAB 
#define W25X_ManufactDeviceID   	      0x90 
#define W25X_JedecDeviceID		          0x9F 

#define WIP_Flag                        0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte                      0xFF
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
 
/**
  * ��������: ����FLASH��ʼ��
  * �������: ��
  * �� �� ֵ: uint32_t:���ش���Flash�ͺ�ID
  * ˵    ������ʼ������Flash�ײ�����GPIO��SPI����
  */
void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;	
  
  /* ʹ��GPIO��SPIʱ�� */
	FLASH_SPI_APBxClock_FUN ( FLASH_SPI_CLK, ENABLE );
  FLASH_SPI_SCK_APBxClock_FUN ( FLASH_SPI_SCK_CLK, ENABLE );
 	FLASH_SPI_MISO_APBxClock_FUN ( FLASH_SPI_MISO_CLK, ENABLE );
  FLASH_SPI_MOSI_APBxClock_FUN ( FLASH_SPI_MOSI_CLK, ENABLE );
  FLASH_SPI_CS_APBxClock_FUN ( FLASH_SPI_CS_CLK, ENABLE );
  
  /* ����SPI�������ţ�SCK ʱ������ */	
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(FLASH_SPI_SCK_PORT, &GPIO_InitStructure);

  /* ����SPI�������ţ�MISO ��������ӻ�������� */	
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
  GPIO_Init(FLASH_SPI_MISO_PORT, &GPIO_InitStructure);

  /* ����SPI�������ţ�MISO ��������ӻ��������� */	
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
  GPIO_Init(FLASH_SPI_MOSI_PORT, &GPIO_InitStructure);
  
  /* ����SPI�������ţ�CS ����FlashƬѡ���� */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_CS_PIN;	
  GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStructure);

  /* ���Ƚ��ô���Flash������Ҫ��������Flashʱ��ʹ�ܼ��� */
  FLASH_SPI_CS_DISABLE();

  /* SPI�������� */
  /* 
   * FLASHоƬ��
	 * ��CLK������ʱ��DIO���ݲ�������. 
   * ��CLK�½���ʱ��DIO�������������
	 * �ݴ�����CPOL CPHA 
   */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(FLASH_SPIx , &SPI_InitStructure);

  /* ʹ��SPI����  */
  SPI_Cmd(FLASH_SPIx , ENABLE);	
}

/**
  * ��������: ��������
  * �������: SectorAddr��������������ַ��Ҫ��Ϊ4096����
  * �� �� ֵ: ��
  * ˵    ��������Flash��С�������СΪ4KB(4096�ֽ�)����һ��������С��Ҫ���������
  *           Ϊ4096��������������FlashоƬд������֮ǰҪ���Ȳ����ռ䡣
  */
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd();
  /* �������� */
  /* ѡ����FLASH: CS�͵�ƽ */
  FLASH_SPI_CS_ENABLE();
  /* ������������ָ��*/
  SPI_FLASH_SendByte(W25X_SectorErase);
  /*���Ͳ���������ַ�ĸ�λ*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* ���Ͳ���������ַ����λ */
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* ���Ͳ���������ַ�ĵ�λ */
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* ���ô���FLASH: CS �ߵ�ƽ */
  FLASH_SPI_CS_DISABLE();
  /* �ȴ��������*/
  SPI_FLASH_WaitForWriteEnd();
}

/**
  * ��������: ������Ƭ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������������Flash��Ƭ�ռ�
  */
void SPI_FLASH_BulkErase(void)
{
 /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();

  /* ��Ƭ���� Erase */
  /* ѡ����FLASH: CS�͵�ƽ */
  FLASH_SPI_CS_ENABLE();
  /* ������Ƭ����ָ��*/
  SPI_FLASH_SendByte(W25X_ChipErase);
  /* ���ô���FLASH: CS�ߵ�ƽ */
  FLASH_SPI_CS_DISABLE();

  /* �ȴ��������*/
  SPI_FLASH_WaitForWriteEnd();
}

/**
  * ��������: ������FLASH��ҳд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * �������: pBuffer����д�����ݵ�ָ��
  *           WriteAddr��д���ַ
  *           NumByteToWrite��д�����ݳ��ȣ�����С�ڵ���SPI_FLASH_PerWritePageSize
  * �� �� ֵ: ��
  * ˵    ��������Flashÿҳ��СΪ256���ֽ�
  */
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();

   /* Ѱ�Ҵ���FLASH: CS�͵�ƽ */
  FLASH_SPI_CS_ENABLE();
  /* д��дָ��*/
  SPI_FLASH_SendByte(W25X_PageProgram);
  /*����д��ַ�ĸ�λ*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*����д��ַ����λ*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*����д��ַ�ĵ�λ*/
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     //printf("Err: SPI_FLASH_PageWrite too large!\n");
  }

  /* д������*/
  while (NumByteToWrite--)
  {
     /* ���͵�ǰҪд����ֽ����� */
    SPI_FLASH_SendByte(*pBuffer);
     /* ָ����һ�ֽ����� */
    pBuffer++;
  }

  /* ���ô���FLASH: CS �ߵ�ƽ */
  FLASH_SPI_CS_DISABLE();

  /* �ȴ�д�����*/
  SPI_FLASH_WaitForWriteEnd();
}

/**
  * ��������: ������FLASHд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * �������: pBuffer����д�����ݵ�ָ��
  *           WriteAddr��д���ַ
  *           NumByteToWrite��д�����ݳ���
  * �� �� ֵ: ��
  * ˵    �����ú���������������д�����ݳ���
  */
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % SPI_FLASH_PageSize;
  count = SPI_FLASH_PageSize - Addr;
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

  if (Addr == 0) /* ����ַ�� SPI_FLASH_PageSize ����  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /* ����ַ�� SPI_FLASH_PageSize ������ */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
      {
        temp = NumOfSingle - count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

/**
  * ��������: �Ӵ���Flash��ȡ����
  * �������: pBuffer����Ŷ�ȡ�����ݵ�ָ��
  *           ReadAddr����ȡ����Ŀ���ַ
  *           NumByteToRead����ȡ���ݳ���
  * �� �� ֵ: ��
  * ˵    �����ú����������������ȡ���ݳ���
  */
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
  /* ѡ����FLASH: CS�͵�ƽ */
  FLASH_SPI_CS_ENABLE();

  /* ���� �� ָ�� */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* ���� �� ��ַ��λ */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* ���� �� ��ַ��λ */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* ���� �� ��ַ��λ */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) /* ��ȡ���� */
  {
     /* ��ȡһ���ֽ�*/
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* ָ����һ���ֽڻ����� */
    pBuffer++;
  }

  /* ���ô���FLASH: CS �ߵ�ƽ */
  FLASH_SPI_CS_DISABLE();
}

/**
  * ��������: ��ȡ����Flash�ͺŵ�ID
  * �������: ��
  * �� �� ֵ: u32������Flash���ͺ�ID
  * ˵    ����  FLASH_ID      IC�ͺ�      �洢�ռ��С         
                0xEF3015      W25X16        2M byte
                0xEF4015	    W25Q16        4M byte
                0XEF4017      W25Q64        8M byte
                0XEF4018      W25Q128       16M byte  (YS-F1Pro������Ĭ������)
  */
u32 SPI_FLASH_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* ѡ����FLASH: CS�͵�ƽ */
  FLASH_SPI_CS_ENABLE();

  /* ���������ȡоƬ�ͺ�ID */
  SPI_FLASH_SendByte(W25X_JedecDeviceID);

  /* �Ӵ���Flash��ȡһ���ֽ����� */
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

  /* �Ӵ���Flash��ȡһ���ֽ����� */
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

  /* �Ӵ���Flash��ȡһ���ֽ����� */
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

  /* ���ô���Flash��CS�ߵ�ƽ */
  FLASH_SPI_CS_DISABLE();
  
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
  return Temp;
}

/**
  * ��������: ��ȡ����Flash�豸ID
  * �������: ��
  * �� �� ֵ: u32������Flash���豸ID
  * ˵    ����
  */
u32 SPI_FLASH_ReadDeviceID(void)
{
  u32 Temp = 0;

  /* ѡ����FLASH: CS�͵�ƽ */
  FLASH_SPI_CS_ENABLE();

  /* ���������ȡоƬ�豸ID * */
  SPI_FLASH_SendByte(W25X_DeviceID);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  
  /* �Ӵ���Flash��ȡһ���ֽ����� */
  Temp = SPI_FLASH_SendByte(Dummy_Byte);

  /* ���ô���Flash��CS�ߵ�ƽ */
  FLASH_SPI_CS_DISABLE();

  return Temp;
}

/**
  * ��������: ����������ȡ���ݴ�
  * �������: ReadAddr����ȡ��ַ
  * �� �� ֵ: ��
  * ˵    ����Initiates a read data byte (READ) sequence from the Flash.
  *           This is done by driving the /CS line low to select the device,
  *           then the READ instruction is transmitted followed by 3 bytes
  *           address. This function exit and keep the /CS line low, so the
  *           Flash still being selected. With this technique the whole
  *           content of the Flash is read with a single READ instruction.
  */
void SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  FLASH_SPI_CS_ENABLE();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
}

/**
  * ��������: �Ӵ���Flash��ȡһ���ֽ�����
  * �������: ��
  * �� �� ֵ: u8����ȡ��������
  * ˵    ����This function must be used only if the Start_Read_Sequence
  *           function has been previously called.
  */
u8 SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}

/**
  * ��������: ������Flash��ȡд��һ���ֽ����ݲ�����һ���ֽ�����
  * �������: byte������������
  * �� �� ֵ: u8�����յ�������
  * ˵    ������
  */
u8 SPI_FLASH_SendByte(u8 byte)
{
  /* ѭ���ȴ�ֱ��SPI ���ݼĴ���DRΪ�գ�����DR�Ĵ�����Ϊ��ʱ�����ȴ� */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_TXE) == RESET);

  /* ͨ��SPI���跢��һ���ֽ����� */
  SPI_I2S_SendData(FLASH_SPIx , byte);

  /* �ȴ����յ����� */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_RXNE) == RESET);

  /* ��ȡSPI���߽��յ�һ���ֽ����ݲ����� */
  return SPI_I2S_ReceiveData(FLASH_SPIx );
}

/**
  * ��������: ������Flash��ȡд�����(16bit)���ݲ����հ�������
  * �������: byte������������
  * �� �� ֵ: u16�����յ�������
  * ˵    ������
  */
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
  /* ѭ���ȴ�ֱ��SPI ���ݼĴ���DRΪ�գ�����DR�Ĵ�����Ϊ��ʱ�����ȴ� */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_TXE) == RESET);

  /* ͨ��SPI���跢�Ͱ������� */
  SPI_I2S_SendData(FLASH_SPIx , HalfWord);

  /* �ȴ����յ����� */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_RXNE) == RESET);

  /* ��ȡSPI���߽��յ��������ݲ����� */
  return SPI_I2S_ReceiveData(FLASH_SPIx );
}

/**
  * ��������: ʹ�ܴ���Flashд����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void SPI_FLASH_WriteEnable(void)
{
  /* ѡ����FLASH: CS�͵�ƽ */
  FLASH_SPI_CS_ENABLE();

  /* �������дʹ�� */
  SPI_FLASH_SendByte(W25X_WriteEnable);

  /* ���ô���Flash��CS�ߵ�ƽ */
  FLASH_SPI_CS_DISABLE();
}

/**
  * ��������: �ȴ�����д�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����Polls the status of the Write In Progress (WIP) flag in the
  *           FLASH's status  register  and  loop  until write  opertaion
  *           has completed.
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;

  /* Select the FLASH: Chip Select low */
  FLASH_SPI_CS_ENABLE();

  /* Send "Read Status Register" instruction */
  SPI_FLASH_SendByte(W25X_ReadStatusReg);

  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	 
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  FLASH_SPI_CS_DISABLE();
}


/**
  * ��������: �������ģʽ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void SPI_Flash_PowerDown(void)   
{ 
  /* Select the FLASH: Chip Select low */
  FLASH_SPI_CS_ENABLE();

  /* Send "Power Down" instruction */
  SPI_FLASH_SendByte(W25X_PowerDown);

  /* Deselect the FLASH: Chip Select high */
  FLASH_SPI_CS_DISABLE();
}   

/**
  * ��������: ���Ѵ���Flash
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void SPI_Flash_WAKEUP(void)   
{
  /* Select the FLASH: Chip Select low */
  FLASH_SPI_CS_ENABLE();

  /* Send "Power Down" instruction */
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);

  /* Deselect the FLASH: Chip Select high */
  FLASH_SPI_CS_DISABLE(); 
}   
   
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
