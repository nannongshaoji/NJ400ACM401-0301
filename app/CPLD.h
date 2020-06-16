/****************************************Copyright (c)**************************************************
**                               南大傲拓科技江苏股份有限公司
**                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/
#ifndef _CPLD_H_
#define _CPLD_H_
#include "com_cpu.h"
/* Register */
#define HST0_Ctrl       0x00  	    //高速计数控制寄存器HST0_Ctrl
#define HST0_S_Value    0x01        //起始值
#define HST0_Value      0x02	    //当前值
#define HST0_C_Value    0x03  	    //捕获值

#define HST1_Ctrl       0x04  	    //高速计数控制寄存器HST1_Ctrl
#define HST1_S_Value    0x05        //起始值
#define HST1_Value      0x06		//当前值
#define HST1_C_Value    0x07  	    //捕获值

#define HST2_Ctrl       0x08  	    //高速计数控制寄存器HST2_Ctrl
#define HST2_S_Value    0x09        //起始值
#define HST2_Value      0x0A		//当前值
#define HST2_C_Value    0x0B        //捕获值

#define HST3_Ctrl       0x0C  	    //高速计数控制寄存器HST3_Ctrl
#define HST3_S_Value    0x0D        //起始值
#define HST3_Value      0x0E		//当前值
#define HST3_C_Value    0x0F  	    //捕获值

#define HST4_Ctrl       0x10  	    //高速计数控制寄存器HST4_Ctrl
#define HST4_S_Value    0x11        //起始值
#define HST4_Value      0x12		//当前值
#define HST4_C_Value    0x13        //捕获值

#define HST5_Ctrl       0x14  	    //高速计数控制寄存器HST5_Ctrl
#define HST5_S_Value    0x15        //起始值
#define HST5_Value      0x16		//当前值
#define HST5_C_Value    0x17  	    //捕获值

#define HST6_Ctrl       0x18  	    //高速计数控制寄存器HST6_Ctrl
#define HST6_S_Value    0x19        //起始值
#define HST6_Value      0x1A		//当前值
#define HST6_C_Value    0x1B  	    //捕获值

#define HST7_Ctrl       0x1C  	    //高速计数控制寄存器HST7_Ctrl
#define HST7_S_Value    0x1D        //起始值
#define HST7_Value      0x1E		//当前值
#define HST7_C_Value    0x1F  	    //捕获值

#define Filter_reg      0x1E		//滤波寄存器

/* HST_Ctrl */
#define HST_Rst     0x00	        //Reset
#define HST_EN      0x80            //Enable
#define HST_Start   0x40            //Start
#define HST_Stop    0x00            //Stop
#define HST_X1      0x00            //1 multiple frequency
#define HST_X4      0x10            //4 multiple frequency

extern void CPLD_Init(void);
extern void CPLD_Write(unsigned char addr,unsigned int data);
extern unsigned int CPLD_Read(unsigned char addr);
extern void CPLD_WriteConfig(unsigned char channo,s_config0 config,unsigned long s_value);

extern void Test_SPI(void);
#endif
/********************************************* End of File *********************************************/
