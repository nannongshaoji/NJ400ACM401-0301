/****************************************Copyright (c)**************************************************
**                               南大傲拓科技江苏股份有限公司
**                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/
#ifndef _CANDRV_H_
#define _CANDRV_H_

#include "config.h"

#define	CAN_BUF_CNT 128
#define	CAN_BUF_CNTS 32
#define	APP_BUF_CNT 8
#define	BUF_LEN 256
#define	MAX_ERR_S 1

#define CAN_ID_STD      	(0)           
#define CAN_ID_EXT      	(1)
#define CAN_RTR_DATA    	(0)       
#define CAN_RTR_REMOTE   	(1)

typedef	struct	_CAN_R_
 {
	uint8		dLEN;			
	uint8		dID1;		
	uint8		dID2;		
	uint8		dID3;
	uint8		dID4;
	uint8		dPAD1;
	uint8		dPAD2;
	uint8		dPAD3;
	uint8		MYDATA[8];
 }stcCAN_R,*P_stcCAN_R;

extern stcCAN_R  CAN_R[CAN_BUF_CNT];

typedef	struct	_CAN_S_
 {
	uint8		dLEN;			
	uint8		dID1;		
	uint8		dID2;		
	uint8		dID3;
	uint8		dID4;
	uint8		dPAD1;
	uint8		dPAD2;
	uint8		dPAD3;
	uint8		MYDATA[8];
 }stcCAN_S,*P_stcCAN_S;


typedef struct
{
  uint32 StdId;                          
  uint32 ExtId;                         
  uint8  IDE;                                                
  uint8  RTR;                                                   
  uint8  DLC;                                                    
  uint8  Data[8];                         
} CanTxMsg;
typedef struct
{
  uint32 StdId;                         
  uint32 ExtId;                          
  uint8  IDE;                           
  uint8  RTR;     
  uint8  DLC;    
  uint8  Data[8]; 
  uint8  FMI;    
} CanRxMsg;


extern stcCAN_S  CAN_S[CAN_BUF_CNTS];

typedef	struct	_APP_R_
 {
	uint16		MYSIZE;			
	uint8		NODE;	
	uint8		LETTER;	
	uint8		MYDATA[BUF_LEN];
 }stcAPP_R,*P_stcAPP_R;
extern stcAPP_R  APP_R[APP_BUF_CNT];

 typedef	struct	_APP_S_
 {
	uint16		MYSIZE;			
	uint8		NODE;	
	uint8		STATE;	
	uint8		MYDATA[BUF_LEN];
 }stcAPP_S,*P_stcAPP_S;

extern stcAPP_S  APP_S;
 
typedef	struct _CAN_STATE_		
{
	uint8 S_NULL;				//NULL	BYTE,
	uint8 STATE;				//STATE	BYTE,
	uint16 ERR_R;				//ERR_R	WORD,
	uint16 ERR_S; 				//ERR_S	WORD)  SLOW PUBLIC;
}stcCAN_STATE,*P_stcCAN_STATE;

extern stcCAN_STATE CAN_STATE;

typedef	struct	_CAN_R_BSP_
 {
	uint16		COUNT;
	uint8		STATUS;	
	uint8		DST;	
	uint8		SRC;	
	uint8		SERIAL;
	uint16		MYSIZE;
 }stcCAN_R_BSP,*P_stcCAN_R_BSP;

extern stcCAN_R_BSP  CAN_R_BSP;



typedef	struct	_CAN_S_BSP_
 {
	uint8		DST;		
	uint8		SERIAL;
	uint16		FRAG_COUNT;
	uint16		FRAG_TOTAL;
	uint16		OVER_NUM;

 }stcCAN_S_BSP,*P_stcCAN_S_BSP;

extern stcCAN_S_BSP  CAN_S_BSP;

extern uint8 CAN_IN_PTR;		
extern uint8 CAN_OUT_PTR; 
//extern void CAN_INTR_READ(void);
extern void CAN_INTR_READ(int IRQn, void *param);
extern uint32 CAN_APP_SOFT_INIT(void);
extern uint32 CAN_APP_RECV(void);
extern uint32 CAN_APP_SEND(void);
#endif
/********************************************* End of File *********************************************/
