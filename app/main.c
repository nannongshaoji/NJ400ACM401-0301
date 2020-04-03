/****************************************Copyright (c)**************************************************
**                               南大傲拓科技江苏股份有限公司
**                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/
#include "../example/test_wdog.h"
#include "../lib/ls1b_public.h"
#include "../lib/ls1b_delay.h"
#include "../lib/ls1b_irq.h"
#include "system_init.h"
#include "com_cpu.h"
#include "candrv.h"
#include "canapp.h"
#include "timer.h"
#include "config.h"
#include "CPLD.h"

uint8 MODULE_POINTNUM=8;
uint8 MODULE_STATE;
uint8 MODULE_ADDR;
uint8 MODULE_TYPE;
uint8 cpld_write_flag=0;
uint32 filter32value=0;
uint8 AO_FLAG=0;

struct callvectors *callvec;
void System_Info_Print(void);
void System_Init(void);
/*************************************************************************************
*Function name	:main()
*Description	:
*
*Parameters		:
*				:
*Returned		:
*************************************************************************************/

int main(int argc, char **argv, char **env, struct callvectors *cv)
{
	uint8 i;
    callvec = cv;// 这条语句之后才能使用pmon提供的打印功能
	System_Info_Print();
	System_Init();

	while (1)
	{
		//下发cpu传来的配置
		if(cpld_write_flag==1)
		{
			filter32value=0;
			i=0;
			while(i<HCM_CH_NUM)
			{			
				CPLD_WriteConfig(i,config0[i],hcm_data[i].hcm_s_value0);

				filter32value |= (config0[i].Bits.filter2<<(16+2*i)); //aux滤波
				filter32value |= (config0[i].Bits.filter0<<(2*i));	//s滤波
				if(config0[i].Bits.filter1) //如果用的双向，BIN滤波加上
					filter32value |= (config0[i].Bits.filter1<<(2*i+2));
				
				if(config0[i].Bits.mode>=HCM_MODE_5) //双向模式,下一通道不能用
					i+=2;
				else i++;
				delay_us(10);	
			}
			CPLD_Write(Filter_reg,filter32value);
			cpld_write_flag=0;	
		}
/*			
		//定时HCM处理
		if(SYS_TIMER7_MARK==0xFF ) 
		{
			SYS_TIMER7_MARK=0;
			SYS_TIMER7=SYS_TIMER7_INIT;
			if(( S_BITTST(&MODULE_STATE,7)==0xFF))
				hcm_process();
		}	
*/	
		//10ms定时任务
		if(TIMER_10MS_MARK==0xFF) 
		{
			if(( S_BITTST(&MODULE_STATE,7)==0xFF))
				hcm_read_mode0_data();//10ms处理测频模式0数据
			MS10_TASK();
			TIMER_10MS_MARK=0;
		}
		//R灯
		if(SYS_TIMER6_MARK==0xFF ) 
		{
			SYS_TIMER6_MARK=0;
			RUN_LED_OFF;
		}
		//A灯通讯中断灭
		if( SYS_TIMER9_MARK==0xFFFF )
		{
			SYS_TIMER9_MARK=0;
			ACT_LED_OFF;			
		}	
		//WDG 
		if( SYS_TIMER8_MARK==0xFF) 
		{
			SYS_TIMER8_MARK=0;
			SYS_TIMER8=SYS_TIMER8_INIT;
			CLEAR_HARDWDOG();		
		}
		//CAN_COM
		CAN_APP_RECV();
		while(CAN_OUT_PTR!=CAN_IN_PTR)
		{
			if(APP_R[CAN_OUT_PTR].LETTER==1)
			{
				PROCESS_MSG();
				APP_R[CAN_OUT_PTR].LETTER=0;
			}							
			CAN_OUT_PTR=CAN_OUT_PTR+1;
			if(CAN_OUT_PTR>=APP_BUF_CNT)CAN_OUT_PTR=0;//APP_BUF_CNT
			if(APP_S.STATE==1) 	CAN_APP_SEND();
			else APP_S.STATE=0;			
		}		
	}	
    return(0);
}


/********************************************************************************************************
 * 函数名称：
 * 函数描述：
 * 输入参数：
 * 输出参数：
 * *****************************************************************************************************/
void System_Init(void)
{
	NA_GPIO_Init();
	NA_Modnle_Init();
	exception_init();
	Init_Timer();

	CPLD_Init();
	NA_CAN_Init();
	wdg_init();
	wdg_enable();
}
/********************************************************************************************************
 * 函数名称：
 * 函数描述：
 * 输入参数：
 * 输出参数：
 * *****************************************************************************************************/
void System_Info_Print(void)
{
    myprintf("*************************************************\n");
    myprintf("*  Copyright (C) 2020,NA.  All Rights Reserved  *\n");
    myprintf("* Module    :NJ400HCM-0801\t\t\t*\n");
    myprintf("* Modify    :zhangjie\t\t\t\t*\n");
    myprintf("* Date      :%s   %s\t\t*\n",__DATE__,__TIME__);
    myprintf("* Version   :%s\t\t\t\t*\n",Version);
    myprintf("*************************************************\n");
}
/********************************************* End of File *********************************************/
