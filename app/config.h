/****************************************Copyright (c)**************************************************
**                               �ϴ���ؿƼ����չɷ����޹�˾
**                                        �з���
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_
#include "../lib/ls1b_gpio.h"

#define Debug(x)   myprintf("* File %s  Error %d: %s *\n",__FILE__,__LINE__,x)
/********************************************************************************************************
 * Version      Author      Date        Description
 * V1.0         Zhangjie    2020/02/26  The Basic Version
 * 
 * 
 * 
********************************************************************************************************/
#define  VERSION            "V1.0"

typedef unsigned char  INT8U;                   /* �޷���8λ���ͱ���                        */
typedef signed   char  INT8S;                   /* �з���8λ���ͱ���                        */
typedef unsigned short INT16U;                  /* �޷���16λ���ͱ���                       */
typedef signed   short INT16S;                  /* �з���16λ���ͱ���                       */
typedef unsigned int   INT32U;                  /* �޷���32λ���ͱ���                       */
typedef signed   int   INT32S,int32;            /* �з���32λ���ͱ���                       */
typedef float          FP32;                    /* �����ȸ�������32λ���ȣ�                 */
typedef double         FP64;                    /* ˫���ȸ�������64λ���ȣ�                 */
typedef unsigned char  uint8;                   /* �޷���8λ���ͱ���                        */
typedef unsigned short uint16;                  /* �޷���16λ���ͱ���                       */
typedef unsigned int   uint32;                  /* �޷���32λ���ͱ���                       */

#define USE_CAN1
/* #define USE_CAN2 */
#ifdef  USE_CAN1
  #define CANx CAN1
#else /* USE_CAN2 */
  #define CANx CAN2
#endif  /* USE_CAN1 */

#define NOP __asm__("nop")


#define NJ400HCM0801      (0xa2)
#define NJ400HCM0811      (0xa4)
#define NJ400ACM0301	  (0xC9)
//LED PIN
#define RUN_LED_PIN       (17)//(23)//
#define ACT_LED_PIN       (15)//(22)//
#define FAULT_LED_PIN     (13)
#define RUN_LED_ON        gpio_set(RUN_LED_PIN,0);
#define RUN_LED_OFF       gpio_set(RUN_LED_PIN,1);
#define ACT_LED_ON        gpio_set(ACT_LED_PIN,0);
#define ACT_LED_OFF       gpio_set(ACT_LED_PIN,1);
#define FAULT_LED_ON      gpio_set(FAULT_LED_PIN,0);
#define FAULT_LED_OFF     gpio_set(FAULT_LED_PIN,1);
//CAN ADDR PIN
#define CANADDR_ID0_PIN   (42)
#define CANADDR_ID1_PIN   (43)
#define CANADDR_ID2_PIN   (50)
#define CANADDR_ID3_PIN   (58)
#define CANADDR_ID4_PIN   (59)
#define CANADDR_ID5_PIN   (60)
#define CANADDR_ID6_PIN   (61)

#define CAN1_ENABLE_PIN   (6)
#define CAN2_ENABLE_PIN   (4)
#define CS_READ_CANADDR_PIN   (20)
//wdg
//#define HARD_WARE_WDG_PIN   //GPIO_Pin_5 


#define SYS_TIMER1_INIT     0			
#define SYS_TIMER2_INIT     47			
#define SYS_TIMER3_INIT     5987		
#define SYS_TIMER4_INIT     5			
#define SYS_TIMER5_INIT     50		
#define SYS_TIMER6_INIT     50			
#define SYS_TIMER7_INIT     30			
#define SYS_TIMER8_INIT     100			
#define SYS_TIMER9_INIT     200			
#define SYS_TIMER10_INIT    1000		

		/*hj CCU-->IO */
#define CODE_IOLOAD 	    1			//DECLARE  CODE_IOLOAD		LITERALLY		'1';
#define CODE_IOPOLL 	    2			//DECLARE  CODE_IOPOLL		LITERALLY		'2';
#define CODE_DOSET 		    3			//DECLARE  CODE_DOSET		  LITERALLY		'3';
#define CODE_TIMESET	    4			//DECLARE  CODE_TIMESET		LITERALLY		'4';
#define CODE_AOSET		    5
		/*hj IO-->CCU   */
#define CODE_DIVAL 		    31		//DECLARE  CODE_DIVAL		LITERALLY		'31';
#define CODE_SOE 		      32		//DECLARE  CODE_SOE		  LITERALLY		'32';
#define CODE_AIVAL 		    33		//DECLARE  CODE_AIVAL		LITERALLY		'33';
#define CODE_DOVAL 		    34		//DECLARE  CODE_DOVAL		LITERALLY		'34';
#define CODE_ACK 		      35		//DECLARE  CODE_ACK	


extern uint8 MODULE_STATE;
extern uint8 MODULE_TYPE;
extern uint8 MODULE_ADDR;
extern uint8 cpld_write_flag;
extern uint8 AO_FLAG;

#endif
/********************************************* End of File *********************************************/
