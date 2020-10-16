/****************************************Copyright (c)**************************************************
**                               南大傲拓科技江苏股份有限公司
**                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/
#include "MR25H256.h"
#include "MY_define.h" 
#include <stdio.h>
#include <string.h>
#include "config.h"

ls1b_spi_info_t spi0_info_MR25H256={0};
/*******************************************************************************
* Function Name  : MR25H256_Init
* Description    : Configures the SPI BUS.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MR25H256_SPI_PIN_Init(void)
{

   //Pin Initialize 
    pin_set_purpose(SPI0_CLK_Pin,PIN_PURPOSE_OTHER);//clk
    pin_set_purpose(SPI0_MISO_Pin,PIN_PURPOSE_OTHER);//miso
    pin_set_purpose(SPI0_MOSI_Pin,PIN_PURPOSE_OTHER);//mosi
    pin_set_purpose(SPI0_CS1_Pin,PIN_PURPOSE_OTHER);//cs

}

void MR25H256_Init(void)
{
    //SPI0 Initialize
    spi0_info_MR25H256.SPIx			= LS1B_SPI_0;
    spi0_info_MR25H256.cs			= LS1B_SPI_CS_1;
    spi0_info_MR25H256.max_speed_hz	= 125000;
    spi0_info_MR25H256.cpol			= SPI_CPOL_1;
    spi0_info_MR25H256.cpha			= SPI_CPHA_1;

    spi_init(&spi0_info_MR25H256);
}
/*******************************************************************************
* Function Name  : SPI_MRAM_WR
* Description    : Write and Read data with SPI BUS.
* Input          : uint8_t data --- the send command
* Output         : uint8_t recv --- the receive data
* Return         : None
*******************************************************************************/
unsigned char SPI_MRAM_WR(unsigned char data)
{
    unsigned char recv;
    recv = spi_txrx_byte(&spi0_info_MR25H256,data);
    return (recv);
}
/*******************************************************************************
* Function Name  : MRAM_RDSR
* Description    : The Read Status Register (RDSR) command allows the Status Register to be read.
* Input          : None
* Output         : None
* Return         : uint8_t status --- Read the status of the MR25H256.
*******************************************************************************/
unsigned char MRAM_RDSR(void)
{
    unsigned char status;

    SPI0_CS1_L;
    SPI_MRAM_WR(RDSR);
    status = SPI_MRAM_WR(DUMMY);
    SPI0_CS1_H;
    return status;
}
/*******************************************************************************
* Function Name  : SPI_Write_Read
* Description    : Sets the Write Enable Latch (WEL) bit in the status register to 1.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MRAM_WREN(void)
{
    SPI0_CS1_L;
    SPI_MRAM_WR(WREN);
    SPI0_CS1_H;
}
/*******************************************************************************
* Function Name  : MRAM_WRDI
* Description    : Resets the WEL bit in the status register to 0.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MRAM_WRDI(void)
{
    SPI0_CS1_L;
    SPI_MRAM_WR(WRDI);
    SPI0_CS1_H;
}
/*******************************************************************************
* Function Name  : MRAM_WRSR
* Description    : New values to be written to the Status Register.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MRAM_WRSR(unsigned char cmd)
{
    MRAM_WREN();
    SPI0_CS1_L;
    SPI_MRAM_WR(WRSR);
    SPI_MRAM_WR(cmd);
    SPI0_CS1_H;
    MRAM_WRDI();
}
/*******************************************************************************
* Function Name  : MRAM_READ
* Description    : The Read Data Bytes (READ) command allows data bytes to be
                   read starting at an address specified by the 16-bit address.
* Input          : uint16_t Addr
                   uint16_t Len
* Output         : uint8_t * Recv
* Return         : None
*******************************************************************************/
void MRAM_READ(unsigned char * Recv, unsigned short Addr, unsigned short Len)
{
    unsigned short i;
	MR25H256_Init();
    SPI0_CS1_L;
    SPI_MRAM_WR(READ);
    SPI_MRAM_WR((Addr>>8)&0xff);
    SPI_MRAM_WR(Addr&0xff);

    for(i=0;i<Len;i++)
    {
        Recv[i] = SPI_MRAM_WR(DUMMY);
    }
    SPI0_CS1_H;
}
/*******************************************************************************
* Function Name  : MRAM_WRITE
* Description    : The Write Data Bytes (WRITE) command allows data bytes to
                   be written starting at an address specified by	the	16-bit
                   address.
* Input          : uint8_t * Send
                   uint16_t Addr
                   uint16_t Len
* Output         : None
* Return         : None
*******************************************************************************/
void MRAM_WRITE(unsigned char * Send, unsigned short Addr, unsigned short Len)
{
    unsigned short i;
	MR25H256_Init();
    MRAM_WREN();
    SPI0_CS1_L;
    SPI_MRAM_WR(WRITE);
    SPI_MRAM_WR((Addr>>8)&0xff);
    SPI_MRAM_WR(Addr&0xff);

    for(i=0;i<Len;i++)
    {
        SPI_MRAM_WR(Send[i]);
    }

    SPI0_CS1_H;
    MRAM_WRDI();
}
/*******************************************************************************
* Function Name  : MRAM_SLEEP
* Description    : The Enter Sleep Mode (SLEEP).
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MRAM_SLEEP(void)
{
    SPI0_CS1_L;
    SPI_MRAM_WR(SLEEP);
    SPI0_CS1_H;
}
/*******************************************************************************
* Function Name  : MRAM_WAKE
* Description    : 	Exit Sleep Mode (WAKE).
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MRAM_WAKE(void)
{
    SPI0_CS1_L;
    SPI_MRAM_WR(WAKE);
    SPI0_CS1_H;
}
INT8U DL_Save_Data(void)
{
	INT8U ret=0;
	INT8U Buf[50];
	memcpy(&Buf[0],&DL_Data.Data.SumActivePower,8);
	MRAM_WRITE(Buf, POWER_NWDATA_Addr, 8);
	return ret;
}

INT8U DL_Read_Data(void)
{
	INT8U ret=0;
	INT8U Buf[50];
	MRAM_READ(Buf, POWER_NWDATA_Addr, 8);
	memcpy(&SumActivePower_reg,&Buf[0],4);
	memcpy(&SumReactivePower_reg,&Buf[4],4);

	return ret;
}
int Read_Config_Pagram(_RTU_Config *RTU_Config)
{
    INT16U getHead;   
	INT16U Flag;
	INT8U Buf[300];
    /*Is head matched*/ 
	MRAM_READ(Buf, Main_Procedure_Buf_Addr, 300);  
	memcpy(&getHead,&Buf[0],2);
    if( PACKAGEHEAD != getHead )
    {
        return 0;
    }
		Flag =Buf[2] ;
		RTU_Config->Head=getHead;
    	RTU_Config->IsOK=Flag;
		RTU_Config->FileLen=0;
		RTU_Config->PageNum=0;
		//RTU_Config->RTU_DL_Config.AVoltage_Check=(*(uint16_t*)(Main_Procedure_Buf_Addr+12));
		memcpy(&RTU_Config->Config.AVoltage_Check,&Buf[12],200);
		if(Flag!=0)
		{
			RTU_Config->IsOK=0;
			MRAM_WRITE((INT8U *)(&RTU_Config->Head), Main_Procedure_Buf_Addr, sizeof(_RTU_Config));
		}
    return 1;
}


/********************************************************************************************************
**                                              End Of File
********************************************************************************************************/
