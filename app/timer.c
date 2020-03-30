/****************************************Copyright (c)**************************************************
**                               南大傲拓科技江苏股份有限公司
**                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/
#include "../lib/ls1b_irq.h"
#include "../lib/ls1b_timer.h"
#include "system_init.h"
#include "timer.h"
#include "CPLD.h"
#include "com_cpu.h"

timer_info_t timer_info={0};
stcMODULE_CLK  MODULE_CLK;

uint8	TIMER_10MS_MARK;
uint16  MS10_CNT;
uint8	SYS_TIMER1;					//DECLARE SYS_TIMER1			BYTE    SLOW;
uint8	SYS_TIMER2;					//DECLARE SYS_TIMER2			BYTE    SLOW;
uint16	SYS_TIMER3;					//DECLARE SYS_TIMER3			BYTE    SLOW;
uint8	SYS_TIMER4;					//DECLARE SYS_TIMER4			BYTE    SLOW;
uint8	SYS_TIMER5;					//DECLARE SYS_TIMER5			BYTE    SLOW;
uint8	SYS_TIMER6;					//DECLARE SYS_TIMER6			BYTE    SLOW;
uint8	SYS_TIMER7;					//DECLARE SYS_TIMER7			BYTE    SLOW;
uint8	SYS_TIMER8;					//DECLARE SYS_TIMER8			BYTE    SLOW;
uint16	SYS_TIMER9;					//DECLARE SYS_TIMER9			BYTE    SLOW    PUBLIC;
uint16   SYS_TIMER10;				//DECLARE SYS_TIMER10			BYTE    SLOW    PUBLIC;

uint8	SYS_TIMER1_MARK;			//DECLARE SYS_TIMER1_MARK		BYTE    SLOW;
uint8	SYS_TIMER2_MARK;			//DECLARE SYS_TIMER2_MARK		BYTE    SLOW;
uint8	SYS_TIMER3_MARK;			//DECLARE SYS_TIMER3_MARK		BYTE    SLOW;
uint8	SYS_TIMER4_MARK;			//DECLARE SYS_TIMER4_MARK		BYTE    SLOW;
uint8	SYS_TIMER5_MARK;			//DECLARE SYS_TIMER5_MARK		BYTE    SLOW;
uint8	SYS_TIMER6_MARK;			//DECLARE SYS_TIMER6_MARK		BYTE    SLOW;
uint8	SYS_TIMER7_MARK;			//DECLARE SYS_TIMER7_MARK		BYTE    SLOW;
uint8	SYS_TIMER8_MARK;			//DECLARE SYS_TIMER8_MARK		BYTE    SLOW;
uint16	SYS_TIMER9_MARK;			//DECLARE SYS_TIMER9_MARK		BYTE    SLOW    PUBLIC;
uint8	SYS_TIMER10_MARK;			//DECLARE SYS_TIMER10_MARK		BYTE    SLOW    PUBLIC;
/*************************************************************************************
*Function name	:timer_pwm0_irqhandler
*Description	:interrupt server function
*
*Parameters		:IRQn-->Interrupt number
*				:param-->Parameter	 
*Returned		:None
*************************************************************************************/
void Timer_Pwm0_Irqhandler(int IRQn, void *param)
{		
	timer_restart(&timer_info);
	IRQ_Time_1();
}
/*************************************************************************************
*Function name	:init_timer
*Description	:
*
*Parameters		:None
*				 
*Returned		:None
*************************************************************************************/
void Init_Timer(void)
{		
	timer_info.timer = TIMER_PWM0;	
	timer_info.time_ns = 10000*1000;//10ms	
	timer_init(&timer_info);	
	irq_install(LS1B_PWM0_IRQ, Timer_Pwm0_Irqhandler, NULL);    
	irq_enable(LS1B_PWM0_IRQ);
}
/*************************************************************************************
*Function name	:IRQ_Time_1
*Description	:Interrupt service function
*
*Parameters 	:None
*
*Returned		:None
*************************************************************************************/
void IRQ_Time_1(void)
{
    uint8 i;
    MS10_CNT++;

    if (MS10_CNT>=100) 
    {
        RUN_LED_ON;
        if(S_BITTST(&MODULE_STATE,7)==0xFF) SYS_TIMER6=SYS_TIMER6_INIT;
        MS10_CNT=0;
    }
    TIMER_10MS_MARK=0xFF;
    /*测频模式为提高测量精度，需要在定时中断中读取CPLD*/
    for(i=0;i<HCM_CH_NUM;i++) //测频模式的时候，中断中读取CPLD 20171226
    {
	 	if(config0[i].Bits.mode!=HCM_MODE_0)
	 		continue;
	 	if(status0[i].Bits.start_stop_bit==1)
	 	{	
	 		if(config0[i].Bits.mode==HCM_MODE_0)//测频模式
	 		{
	 			measure_10hz_data[i][measure_10hz_index[i]]=CPLD_Read(i*4+2);
	 			measure_10hz_index[i]++;
	 			if(measure_10hz_index[i]>=MAX_NUM_HZ)
	 				measure_10hz_index[i]=0;
	 		}
	 	}
    }
}
/*************************************************************************************
*Function name	:CLK_SOFT_INIT
*Description	:
*
*Parameters 	:None
*
*Returned		:None
*************************************************************************************/
void CLK_SOFT_INIT(void)
{
	MODULE_CLK.SECOND=0;
	MODULE_CLK.MINUTE=0;
	MODULE_CLK.M_HOUR=0;
	MODULE_CLK.MS=0;
	SYS_TIMER1_MARK=0;
	SYS_TIMER2_MARK=0;
	SYS_TIMER3_MARK=0;
	SYS_TIMER4_MARK=0;
	SYS_TIMER5_MARK=0;
	SYS_TIMER6_MARK=0;
	SYS_TIMER7_MARK=0;
	SYS_TIMER8_MARK=0;
	SYS_TIMER9_MARK=0;
	SYS_TIMER10_MARK=0;
	SYS_TIMER1=SYS_TIMER1_INIT;
	SYS_TIMER2=SYS_TIMER2_INIT;
	SYS_TIMER3=SYS_TIMER3_INIT;
	SYS_TIMER4=SYS_TIMER4_INIT;
	SYS_TIMER5=SYS_TIMER5_INIT;
	SYS_TIMER6=0;
	SYS_TIMER7=SYS_TIMER7_INIT;
	SYS_TIMER8=SYS_TIMER8_INIT;
	SYS_TIMER9=SYS_TIMER9_INIT;
	SYS_TIMER10=SYS_TIMER10_INIT;
	TIMER_10MS_MARK=0;
	MS10_CNT=0;
}
/*************************************************************************************
*Function name	:
*Description	:
*
*Parameters 	:None
*
*Returned		:None
*************************************************************************************/
void MS10_TASK(void)
{
	if (SYS_TIMER1!=0)  
	{
		if (--SYS_TIMER1==0)  SYS_TIMER1_MARK=0xff;
	}	
	if (SYS_TIMER2!=0)
	{ 
		if (--SYS_TIMER2==0)  SYS_TIMER2_MARK=0xff;
	}	
	if (SYS_TIMER3!=0)
	{  
		if (--SYS_TIMER3==0)  SYS_TIMER3_MARK=0xff;
	}	
	if (SYS_TIMER4!=0) 
	{
		if (--SYS_TIMER4==0)  SYS_TIMER4_MARK=0xff;
	}	
	if (SYS_TIMER5!=0)
	{ 
		if (--SYS_TIMER5==0)  SYS_TIMER5_MARK=0xff;
	}	
	if (SYS_TIMER6!=0) 
	{ 
		if (--SYS_TIMER6==0)  SYS_TIMER6_MARK=0xff;
	}	
	if (SYS_TIMER7!=0) 
	{ 
		if (--SYS_TIMER7==0)  SYS_TIMER7_MARK=0xff;
	}
	if (SYS_TIMER8!=0) 
	{ 
		if (--SYS_TIMER8==0)  SYS_TIMER8_MARK=0xff;
	}	
	if (SYS_TIMER9!=0) 
	{ 
		if (--SYS_TIMER9==0)  SYS_TIMER9_MARK=0xffff;
	}	
	if (SYS_TIMER10!=0) 
	{ 
		if (--SYS_TIMER10==0)  SYS_TIMER10_MARK=0xff;
	}
		
}
/********************************************* End of File *********************************************/
