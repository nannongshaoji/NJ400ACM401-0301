/****************************************Copyright (c)**************************************************
**                               南大傲拓科技江苏股份有限公司
**                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/
#ifndef _TIMER_H_
#define _TIMER_H_
#include "config.h"

typedef struct MODULE_CLK
 {
	uint8		M_NULL;//:8;		
	uint8		M_HOUR;//:8;
	uint8		MINUTE;//:8;
	uint8		SECOND;//:8;
	uint16		MS;//:16;

 }stcMODULE_CLK,*P_MODULE_CLK;

extern stcMODULE_CLK  MODULE_CLK;
extern uint16	TIMER_10MS_MARK;
extern uint16   MS10_CNT;
extern uint16	SYS_TIMER1;					//DECLARE SYS_TIMER1			BYTE    SLOW;
extern uint16	SYS_TIMER2;					//DECLARE SYS_TIMER2			BYTE    SLOW;
extern uint16	SYS_TIMER3;					//DECLARE SYS_TIMER3			BYTE    SLOW;
extern uint16	SYS_TIMER4;					//DECLARE SYS_TIMER4			BYTE    SLOW;
extern uint16	SYS_TIMER5;					//DECLARE SYS_TIMER5			BYTE    SLOW;
extern uint16	SYS_TIMER6;					//DECLARE SYS_TIMER6			BYTE    SLOW;
extern uint16	SYS_TIMER7;					//DECLARE SYS_TIMER7			BYTE    SLOW;
extern uint16	SYS_TIMER8;					//DECLARE SYS_TIMER8			BYTE    SLOW;
extern uint16	SYS_TIMER9;					//DECLARE SYS_TIMER9			BYTE    SLOW    PUBLIC;
extern uint16    SYS_TIMER10;				//DECLARE SYS_TIMER10			BYTE    SLOW    PUBLIC;

extern uint8	SYS_TIMER1_MARK;			//DECLARE SYS_TIMER1_MARK		BYTE    SLOW;
extern uint8	SYS_TIMER2_MARK;			//DECLARE SYS_TIMER2_MARK		BYTE    SLOW;
extern uint8	SYS_TIMER3_MARK;			//DECLARE SYS_TIMER3_MARK		BYTE    SLOW;
extern uint8	SYS_TIMER4_MARK;			//DECLARE SYS_TIMER4_MARK		BYTE    SLOW;
extern uint8	SYS_TIMER5_MARK;			//DECLARE SYS_TIMER5_MARK		BYTE    SLOW;
extern uint8	SYS_TIMER6_MARK;			//DECLARE SYS_TIMER6_MARK		BYTE    SLOW;
extern uint8	SYS_TIMER7_MARK;			//DECLARE SYS_TIMER7_MARK		BYTE    SLOW;
extern uint8	SYS_TIMER8_MARK;			//DECLARE SYS_TIMER8_MARK		BYTE    SLOW;
extern uint16	SYS_TIMER9_MARK;			//DECLARE SYS_TIMER9_MARK		BYTE    SLOW    PUBLIC;
extern uint8	SYS_TIMER10_MARK;			//DECLARE SYS_TIMER10_MARK		BYTE    SLOW    PUBLIC;


extern void Timer_Pwm0_Irqhandler(int IRQn, void *param) ;
extern void Init_Timer(void);
extern void IRQ_Time_1(void);
extern void CLK_SOFT_INIT(void);
extern void MS10_TASK(void);

#endif
/********************************************* End of File *********************************************/
