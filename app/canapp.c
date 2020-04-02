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
#include "com_cpu.h"
#include "canapp.h"
#include "candrv.h"

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
	uint8 I,OCCNO;
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
			else if(APP_R[CAN_OUT_PTR].MYSIZE==99) //3+96
			{		
				for(I=0;I<HCM_CH_NUM;I++)
				{
					memcpy(&config0[I].Word,&APP_R[CAN_OUT_PTR].MYDATA[3+I*12],2);
					memcpy(&hcm_data[I].hcm_s_value0,&APP_R[CAN_OUT_PTR].MYDATA[5+I*12],4);
					memcpy(&hcm_data[I].hcm_s_value1,&APP_R[CAN_OUT_PTR].MYDATA[9+I*12],4);
					memcpy(&hcm_data[I].hcm_s_value2,&APP_R[CAN_OUT_PTR].MYDATA[13+I*12],2);

				}
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
				APP_S.MYSIZE=3+HCM_CH_NUM*10;
				APP_S.MYDATA[0]=MODULE_ADDR;
				APP_S.MYDATA[1]=CODE_AIVAL;
				APP_S.MYDATA[2]=MODULE_STATE;
				for(I=0;I<HCM_CH_NUM;I++)//上传HCM信息
				{
				  	memcpy(&APP_S.MYDATA[3+I*10],&status0[I].Word,2);
					memcpy(&APP_S.MYDATA[5+I*10],&hcm_data[I].hcm_hst_value,4);
					memcpy(&APP_S.MYDATA[9+I*10],&hcm_data[I].hcm_hst_c_value,4);
				}
				APP_S.STATE=1;
			}
		}
		
			/* 开出*/
		if(APP_R[CAN_OUT_PTR].MYDATA[1]==CODE_DOSET && APP_R[CAN_OUT_PTR].MYSIZE==4+APP_R[CAN_OUT_PTR].MYDATA[3]*3)			
		{				
			for(I=0;I<APP_R[CAN_OUT_PTR].MYDATA[3];I++)   
			{		
				OCCNO=APP_R[CAN_OUT_PTR].MYDATA[4+I*3]&0x7F;
//				if( (APP_R[CAN_OUT_PTR].MYDATA[4+I*3]&0x80)==0x80)
//				{
//					DODATA |= 1<< (OCCNO-1);
//				}
//				else 
//				{
//					DODATA &= ~(1<< (OCCNO-1));
//				}
				//memcpy(&DO_PULSE[OCCNO-1],&(APP_R[CAN_OUT_PTR].MYDATA[5+I*3]),2);
				
			}	
			APP_S.NODE=APP_R[CAN_OUT_PTR].NODE;
			APP_S.MYSIZE=8;
			APP_S.MYDATA[0]=MODULE_ADDR;
			APP_S.MYDATA[1]=CODE_ACK;
			APP_S.MYDATA[2]=MODULE_STATE;
			APP_S.MYDATA[3]=CODE_DOSET;
			APP_S.MYDATA[4]=0;
			APP_S.MYDATA[5]=0;
			APP_S.MYDATA[6]=0;
			APP_S.MYDATA[7]=0;
			APP_S.STATE=1;
		}
		/* 模出 */
		if(APP_R[CAN_OUT_PTR].MYDATA[1]==CODE_AOSET && APP_R[CAN_OUT_PTR].MYSIZE==(4+APP_R[CAN_OUT_PTR].MYDATA[3]*6))
		{
			for( I=0;I< APP_R[CAN_OUT_PTR].MYDATA[3];I++)
			{
				OCCNO=APP_R[CAN_OUT_PTR].MYDATA[4+I*6];
				if(( S_BITTST(&MODULE_STATE,7)==0xFF))
				{
					switch(APP_R[CAN_OUT_PTR].MYDATA[5+I*6])
					{
						case AOSET_CODE_RESET: //复位指令
							aoset_state_byte[OCCNO-1] |=STATE_RESET;
							break;
						case AOSET_CODE_START: //启动指令
							aoset_state_byte[OCCNO-1] |=STATE_START;
							break;
						case AOSET_CODE_STOP: //停止指令
							aoset_state_byte[OCCNO-1] |=STATE_STOP;
							break;
						case AOSET_CODE_TIME_CYCLE:
//						case AOSET_CODE_MODE3_YUZHI:
							aoset_state_byte[OCCNO-1] |=STATE_SET_S_VALUE;
							memcpy(&hcm_data[OCCNO-1].hcm_s_value0,&APP_R[CAN_OUT_PTR].MYDATA[6+I*6],4);
							break;
						case AOSET_CODE_BIJIAO:
							memcpy(&hcm_data[OCCNO-1].hcm_s_value1,&APP_R[CAN_OUT_PTR].MYDATA[6+I*6],4);
							break;
						case AOSET_CODE_ZHIHOU:
							memcpy(&hcm_data[OCCNO-1].hcm_s_value2,&APP_R[CAN_OUT_PTR].MYDATA[6+I*6],4);
							break;
					}					
					AO_FLAG |=(1<<(OCCNO-1));
//					{
//						AO_FLAG |=(1<<(OCCNO-1));
//					 	AO_DATA_NOW[OCCNO-1].OUTPUT=AO_DATA_BUF[OCCNO-1].OUTPUT;
//					 	AO_DATA_PRE[OCCNO-1].OUTPUT=AO_DATA_BUF[OCCNO-1].OUTPUT;
//					 
//					}
				}
			}
			APP_S.NODE=APP_R[CAN_OUT_PTR].NODE;
			APP_S.MYSIZE=8;
			APP_S.MYDATA[0]=MODULE_ADDR;
			APP_S.MYDATA[1]=CODE_ACK;
			APP_S.MYDATA[2]=MODULE_STATE;
			APP_S.MYDATA[3]=CODE_AOSET;
			APP_S.MYDATA[4]=0;
			APP_S.MYDATA[5]=0;
			APP_S.MYDATA[6]=0;
			APP_S.MYDATA[7]=0;
			APP_S.STATE=1;
			
		}
		//事件状态位处理
		if(APP_R[CAN_OUT_PTR].MYDATA[1]==CODE_ACK)
		{
			for(I=0;I<HCM_CH_NUM;I++)
			{			
				if(APP_R[CAN_OUT_PTR].MYDATA[2+I]&0x01)
					status0[I].Bits.event_chan =0;
			}
		}
	}	
}
/********************************************* End of File *********************************************/
