/****************************************Copyright (c)**************************************************
**                               南大傲拓科技江苏股份有限公司
**                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/
#ifndef _MR25H256_H_
#define _MR25H256_H_
#include "../lib/ls1b_spi.h"
#include "../lib/ls1b_delay.h"
#include "../lib/ls1b_pin.h"
#include "../lib/ls1b_gpio.h"
#include "config.h"
#include "MY_define.h" 

#define WREN    0x06
#define WRDI    0x04
#define RDSR    0x05
#define WRSR    0x01
#define READ    0x03
#define WRITE   0x02
#define SLEEP   0xb9
#define WAKE    0xab
#define DUMMY   0xff



/* SPI接口定义 */
#define SPI0_CLK_Pin     	(24)
#define SPI0_MISO_Pin    	(25)
#define SPI0_MOSI_Pin    	(26)
#define SPI0_CS1_Pin      	(28)
#define SPI0_CS1_L        	{spi_set_cs(&spi0_info_MR25H256,0); delay_us(10);}
#define SPI0_CS1_H        	{delay_us(10); spi_set_cs(&spi0_info_MR25H256,1);}

/*SPI Operate*/
void MR25H256_SPI_PIN_Init(void);
void SPI0_PIN_Init(void);
unsigned char SPI_MRAM_WR(unsigned char data);
unsigned char MRAM_RDSR(void);
void MRAM_WREN(void);
void MRAM_WRDI(void);
void MRAM_WRSR(unsigned char cmd);
void MRAM_READ(unsigned char * Recv, unsigned short Addr, unsigned short Len);
void MRAM_WRITE(unsigned char * Send, unsigned short Addr, unsigned short Len);
void MRAM_SLEEP(void);
void MRAM_WAKE(void);
extern INT8U DL_Save_Data(void);
extern INT8U DL_Read_Data(void);
extern int Read_Config_Pagram(_RTU_Config*RTU_Config);

#endif
/********************************************************************************************************
**                                              End Of File
********************************************************************************************************/
