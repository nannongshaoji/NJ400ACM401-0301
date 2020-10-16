/****************************************Copyright (c)**************************************************
***                               南大傲拓科技江苏股份有限公司
****                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/
#include "../example/test_wdog.h"
#include "../lib/ls1b_public.h"
#include "../lib/ls1b_delay.h"
#include "../lib/ls1b_irq.h"
#include "system_init.h"
#include "candrv.h"
#include "canapp.h"
#include "timer.h"
#include "config.h"
#include "MR25H256.h"
#include "ATT7022EU.h"
#include "MY_define.h" 


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

#define MHH_DBUG (1)
unsigned short ABCCurrent[20]={0};   //ABC三相电流和数组
unsigned short ABCptr=0;   //ABC三相电流和数组指针


void Init_config(void)
{
	RTU_Config.Head=PACKAGEHEAD;
	RTU_Config.IsOK=0;
	RTU_Config.FileLen=0;
	RTU_Config.PageNum=0;
	RTU_Config.Config.AVoltage_Check=0;
	RTU_Config.Config.BVoltage_Check=0;
	RTU_Config.Config.CVoltage_Check=0;
	RTU_Config.Config.ACurrent_Check=0;
	RTU_Config.Config.BCurrent_Check=0;
	RTU_Config.Config.CCurrent_Check=0;
	RTU_Config.Config.APower_Check=0;
	RTU_Config.Config.BPower_Check=0;
	RTU_Config.Config.CPower_Check=0;
	RTU_Config.Config.RS485Addr=1;
	RTU_Config.Config.RS485Baudrate=0;
	RTU_Config.Config.CurrentRatio=1;//20
	RTU_Config.Config.VoltRatio=1;
	RTU_Config.Config.FilterLevel=4;
	RTU_Config.Config.CGQGroupNo=1;
	RTU_Config.Config.CGQIndexNo=0;
	RTU_Config.Config.DLSleepTime=10;
}
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
    callvec = cv;// 这条语句之后才能使用pmon提供的打印功能
	System_Info_Print();
	System_Init();

	while (1)
	{
		
		ATT7022EU_Thread();//ATT7022E处理服务
		//10ms定时任务
		if(TIMER_10MS_MARK==0xFF) 
		{
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
			fxms_flag=1;
			DL_Save_Data();
			print_power_data();
							
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
	int ret=0,i;
	
	unsigned char Recv[400],Trans[100];
	NA_GPIO_Init();
	NA_Modnle_Init();
	exception_init();
	Init_Timer();
	myprintf("*************************1************************\n");
	MR25H256_SPI_PIN_Init();
	ATT7022EU_SPI_PIN_Init();
	NA_CAN_Init();

	ret=Read_Config_Pagram(&RTU_Config); //从FLASH里读配置
	if(ret==0)
	{
		Init_config();//若失败，赋初始值
		MRAM_WRITE((unsigned char *)(&RTU_Config.Head), Main_Procedure_Buf_Addr, sizeof(_RTU_Config));
	}
	myprintf("ret=%d\n",ret);
	memcpy(&DL_Config.buf[3],&RTU_Config.Config.RS485Addr,100);
	ATT7022EU_init();
	myprintf("*************************2************************\n");
	DL_Config.Config.Version=0x100;
	DL_Config.Config.ManufactureCode=12;
	f1s_event=1;//RTC时钟开机读取
	DL_Config.Config.Zigbee_data=0;
	DL_Config.Config.RS485_data=0;
	DL_Config.Config.DLCheckMode=0;
	if(DL_Config.Config.DLSleepTime<10) DL_Config.Config.DLSleepTime=10;	
	DL_Read_Data();
	myprintf("*************************3************************\n");
	wdg_init();
	wdg_enable();
	myprintf("*************************4************************\n");
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
    myprintf("* Module    :NJ400ACM-0301\t\t\t*\n");
    myprintf("* Modify    :Miao Huanhuan\t\t\t\t*\n");
    myprintf("* Date      :%s   %s\t\t*\n",__DATE__,__TIME__);
    myprintf("* Version   :%s\t\t\t\t*\n",VERSION);
    myprintf("*************************************************\n");
}
/********************************************* End of File *********************************************/
