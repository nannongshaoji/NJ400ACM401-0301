/****************************************Copyright (c)**************************************************
**                               南大傲拓科技江苏股份有限公司
**                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/
#include "../lib/ls1b_spi.h"
#include "../lib/ls1b_pin.h"
#include "../lib/ls1b_delay.h"
#include "../lib/ls1b_public.h"
#include "config.h"
#include "CPLD.h"

ls1b_spi_info_t spi0_info={0};

#define MISO_Flag_Pin    (22)
#define SPI0_CLK_Pin     (24)
#define SPI0_MISO_Pin    (25)
#define SPI0_MOSI_Pin    (26)
#define SPI0_CS_Pin      (29)
#define SPI0_CS_L        spi_set_cs(&spi0_info,0);
#define SPI0_CS_H        spi_set_cs(&spi0_info,1);
/*************************************************************************************
*Function name	:
*Description	:
*
*Parameters 	:
*
*Returned		:
*************************************************************************************/
void CPLD_Init(void)
{
    //Pin Initialize 
    pin_set_purpose(SPI0_CLK_Pin,PIN_PURPOSE_OTHER);//clk
    pin_set_purpose(SPI0_MISO_Pin,PIN_PURPOSE_OTHER);//miso
    pin_set_purpose(SPI0_MOSI_Pin,PIN_PURPOSE_OTHER);//mosi
    pin_set_purpose(SPI0_CS_Pin,PIN_PURPOSE_OTHER);//cs

    //pin_set_purpose(MISO_Flag_Pin,PIN_PURPOSE_GPIO);//sdo_flag
    //gpio_set(MISO_Flag_Pin, gpio_level_high);

    //SPI0 Initialize
    spi0_info.SPIx			= LS1B_SPI_0;
    spi0_info.cs			= LS1B_SPI_CS_2;
    spi0_info.max_speed_hz	= 250000;
    spi0_info.cpol			= SPI_CPOL_1;
    spi0_info.cpha			= SPI_CPHA_1;


    spi_cs(LS1B_SPI_CS_0);
    delay_us(50);
    spi_cs(LS1B_SPI_CS_1);
    delay_us(50);
    spi_init(&spi0_info);


}
/*************************************************************************************
*Function name	:
*Description	:
*
*Parameters 	:
*
*Returned		:
*************************************************************************************/
void CPLD_Write(unsigned char addr,unsigned int data)
{ 
    SPI0_CS_L;
    //delay_us(20);
    addr &= ~(1<<7);
    spi_txrx_byte(&spi0_info,addr);
    spi_txrx_byte(&spi0_info,(unsigned char)(data>>24));
    spi_txrx_byte(&spi0_info,(unsigned char)(data>>16));
    spi_txrx_byte(&spi0_info,(unsigned char)(data>>8));
    spi_txrx_byte(&spi0_info,(unsigned char)(data>>0));
    //delay_us(20);
    SPI0_CS_H;
}
/*************************************************************************************
*Function name	:
*Description	:
*
*Parameters 	:
*
*Returned		:
*************************************************************************************/
unsigned int CPLD_Read(unsigned char addr)
{
    unsigned int ret = 0;
    SPI0_CS_L;
    //delay_us(20);
    addr |= (1<<7);
    spi_txrx_byte(&spi0_info,addr);
    ret = spi_txrx_byte(&spi0_info,0xFF);
    ret = (ret<<8) | (spi_txrx_byte(&spi0_info,0xFF));
    ret = (ret<<8) | (spi_txrx_byte(&spi0_info,0xFF));
    ret = (ret<<8) | (spi_txrx_byte(&spi0_info,0xFF));
    //delay_us(20);
    SPI0_CS_H;
    return ret;
}
/*************************************************************************************
*Function name	:
*Description	:
*
*Parameters 	:
*
*Returned		:
*************************************************************************************/
void CPLD_WriteConfig(unsigned char channo,s_config0 config,unsigned long s_value)
{		
    if(config.Bits.mode>HCM_MODE_0)
    {
        CPLD_Write(4*channo+1,s_value);
    }        
}
/********************************************* End of File *********************************************/
#if Debug==1
unsigned char s_flag=0;
void Test_SPI(void)
{
    if(s_flag < 2)
    {
        //CPLD_Write(0x04,s_flag+1);
        //delay_ms(1);
        myprintf("PTO_C_1 = %d\n",CPLD_Read(0x01));
        s_flag++;
    }   
}
#endif
