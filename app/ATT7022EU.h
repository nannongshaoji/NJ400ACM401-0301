#ifndef _ATT7022EU_H_
#define _ATT7022EU_H_
#include "../lib/ls1b_spi.h"
#include "../lib/ls1b_delay.h"
#include "../lib/ls1b_pin.h"
#include "../lib/ls1b_gpio.h"
#include "../lib/ls1b_public.h"
#include "config.h"

#define ADJUST_PIN   		(56)
#define CLEAR_PIN   		(4)
#define ADJUST_START_LED   		(11)
#define CLEAR_START_LED   		(9)

/* SPI接口定义 */
#define SPI0_CLK_Pin     	(24)
#define SPI0_MISO_Pin    	(25)
#define SPI0_MOSI_Pin    	(26)
#define SPI0_CS2_Pin      	(29)
#define SPI0_CS2_L        	{spi_set_cs(&spi0_info_ATT7022EU,0); delay_us(10);}
#define SPI0_CS2_H        	{delay_us(10); spi_set_cs(&spi0_info_ATT7022EU,1);}
extern void SPI1_Init(void);
extern unsigned int ATT7022EU_Read_Reg(unsigned char RegAddr);
extern void ATT7022EU_Write_Reg(unsigned char RegAddr,unsigned int Value);
extern void ATT7022EU_Reset(void);
extern unsigned int ATT7022EU_Read_Data(void);
extern void ATT7022EU_Thread(void);
extern void ATT7022EU_init(void);
extern void ATT7022EU_SPI_MODE_Init0(void);
extern void adjust_touch_init(void);
extern void ATT7022EU_SPI_PIN_Init(void);
extern volatile BOOL read_adjust_flag(void);
extern void print_power_data (void);

#endif
