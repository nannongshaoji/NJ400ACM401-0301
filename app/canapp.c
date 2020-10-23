/****************************************Copyright (c)**************************************************
**                               南大傲拓科技江苏股份有限公司
**                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/
#include "../lib/ls1b_regs.h"
#include "../lib/ls1b_irq.h"
#include "../lib/ls1b_public.h"
#include "../lib/ls1b_delay.h"
#include "../lib/ls1b_can.h"
#include "system_init.h"
#include "string.h"
#include "config.h"
#include "canapp.h"
#include "candrv.h"
#include "MY_define.h" 
/*************************************************************************************
*Function name	:NA_CAN_Init
*Description	:CAN initialize
*
*Parameters		:None
*
*Returned		:None
*************************************************************************************/
void NA_CAN_Init(void)
{
	uint32 RETCODE,K=0;
	myprintf("\n[%s]\n",__FUNCTION__);
	volatile unsigned char* base0;
	base0= (volatile unsigned char*)LS1B_REG_BASE_CAN0;
	//volatile unsigned char* base1;
	//base1= (volatile unsigned char*)LS1B_REG_BASE_CAN1;	
	RETCODE=CANInit(base0);	
  	while((RETCODE!=0)&&(K<3))
	{
		delay_us(1);
		RETCODE=CANInit(base0);
		K++;
	}	  
	irq_install(LS1B_CAN0_IRQ, CAN_INTR_READ, 0);
	receive_irq_enable(base0);
	irq_enable(LS1B_CAN0_IRQ);
	CAN_APP_SOFT_INIT();		
}
/*************************************************************************************
*Function name	:
*Description	:
*
*Parameters		:None
*
*Returned		:None
*************************************************************************************/
void PROCESS_MSG(void)
{
	
	if(APP_R[CAN_OUT_PTR].MYDATA[0]==MODULE_ADDR)
	{
		if(APP_R[CAN_OUT_PTR].MYDATA[1]==CODE_IOLOAD) //3+16
		{
			if(APP_R[CAN_OUT_PTR].MYDATA[2]!=MODULE_TYPE)
			{	
				MODULE_STATE|=0x04;//模块类型不匹配
				MODULE_STATE&=0x7F;//模块参数未加载
				APP_S.MYDATA[4]=1;
			}		
			else if(APP_R[CAN_OUT_PTR].MYSIZE>=8) 
			{		
									
		
				MODULE_STATE|=0x80;//模块参数已加载
				MODULE_STATE&=0xFB;//模块类型匹配
				APP_S.MYDATA[4]=0;
				cpld_write_flag=1; //加载成功写cpld
			}	
			else return;	
			APP_S.NODE=APP_R[CAN_OUT_PTR].NODE;
			APP_S.MYSIZE=8;
			APP_S.MYDATA[0]=MODULE_ADDR;//
			APP_S.MYDATA[1]=CODE_ACK;//35
			APP_S.MYDATA[2]=MODULE_STATE;//
			APP_S.MYDATA[3]=CODE_IOLOAD;//1

			APP_S.MYDATA[5]=0;
			APP_S.MYDATA[6]=0;
			APP_S.MYDATA[7]=0;
			APP_S.STATE=1;
		}
		/* poll */
		if(APP_R[CAN_OUT_PTR].MYDATA[1]==CODE_IOPOLL && APP_R[CAN_OUT_PTR].MYSIZE==8)//CPU读取数据		
		{	
			if(	APP_S.MYDATA[1]!=CODE_SOE )
			{
				APP_S.NODE=APP_R[CAN_OUT_PTR].NODE;
				APP_S.MYSIZE=3+32*2;
				APP_S.MYDATA[0]=MODULE_ADDR;
				APP_S.MYDATA[1]=CODE_AIVAL;
				APP_S.MYDATA[2]=MODULE_STATE;
				DL_Data.Data.AVoltage_Valid/=20;
				DL_Data.Data.BVoltage_Valid/=20;
				DL_Data.Data.CVoltage_Valid/=20;
				if((DL_Data.Data.AVoltage_Valid<100)&&(DL_Data.Data.BVoltage_Valid<100)&&(DL_Data.Data.CVoltage_Valid<100))
				{
					DL_Data.Data.PowerNetFreq=0;
					DL_Data.Data.AngleA =0  ; 
					DL_Data.Data.AngleB =0 ;
					DL_Data.Data.AngleC=0   ; 
					DL_Data.Data.AngleUAB=0   ;
					DL_Data.Data.AngleUAC=0   ; 
					DL_Data.Data.AngleUBC=0   ; 
				}
				memcpy(&APP_S.MYDATA[3],&(DL_Data.Data.ACurrent_Valid),32*2);
				DL_Data.Data.AVoltage_Valid*=20;
				DL_Data.Data.BVoltage_Valid*=20;
				DL_Data.Data.CVoltage_Valid*=20;
				APP_S.STATE=1;
			}
		}

		//事件状态位处理
		if(APP_R[CAN_OUT_PTR].MYDATA[1]==CODE_ACK)
		{
		
		
		}
	}	
}
/********************************************* End of File *********************************************/
