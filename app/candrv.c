/****************************************Copyright (c)**************************************************
**                               南大傲拓科技江苏股份有限公司
**                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/
#include "../lib/ls1b_regs.h"
#include "../lib/ls1b_public.h"
#include "../lib/ls1b_can.h"
#include "../lib/ls1b_delay.h"
#include "system_init.h"
#include "candrv.h"
#include "config.h"
#include "timer.h"
#include "string.h"

stcCAN_R        CAN_R[CAN_BUF_CNT];
stcCAN_S        CAN_S[CAN_BUF_CNTS];
stcAPP_R        APP_R[APP_BUF_CNT];
stcAPP_S        APP_S;
stcCAN_STATE    CAN_STATE;
stcCAN_R_BSP    CAN_R_BSP;
stcCAN_S_BSP    CAN_S_BSP;

uint8 CAN_ISR_PTR_R;
uint8 CAN_ISR_PTR_P;
uint8 FIRST_SETCLK;
uint8 IS_SOE;
uint8 CAN_IN_PTR;		
uint8 CAN_OUT_PTR;
/*************************************************************************************
*Function name	:CAN_APP_SOFT_INIT
*Description	:initialize the software
*
*Parameters		:None
*
*Returned		:0
*************************************************************************************/
uint32 CAN_APP_SOFT_INIT(void)		//??CANAPP.C?InitCAN()
{
	uint8 I;
	CAN_ISR_PTR_R=0;CAN_ISR_PTR_P=0;
	CAN_IN_PTR=0;CAN_OUT_PTR=0;
	APP_S.STATE=0;
	APP_S.MYDATA[1]=0;
	CAN_STATE.STATE=0; CAN_STATE.ERR_R=0; CAN_STATE.ERR_S=0;
	CAN_R_BSP.STATUS=0; CAN_R_BSP.SERIAL=0; CAN_R_BSP.COUNT=0; CAN_R_BSP.MYSIZE =0;
	CAN_S_BSP.FRAG_COUNT=0; CAN_S_BSP.SERIAL=0; CAN_S_BSP.FRAG_TOTAL=0; CAN_S_BSP.OVER_NUM =0;
	for(I=0;I<APP_BUF_CNT;I++)
	{
		APP_R[I].LETTER=0;
	}
	FIRST_SETCLK=0xFF;
	return 0;
}
/*************************************************************************************
*Function name	:CAN_INTR_READ
*Description	:CAN read
*
*Parameters		:IRQn-->Interrupt number(not used)
*				:param-->Parameter(not used)
*Returned		:None
*************************************************************************************/
void CAN_INTR_READ(int IRQn, void *param)
{
	uint8 FRAG_TYPE;
	uint16 FRAG_COUNT;
	uint16 MS_TMP;
	uint32 TemptID;
	CanRxMsg RxMessage;
	
	if(S_BITTST((uint8 *) &MODULE_STATE,7)==0xFF)
	{	
		ACT_LED_ON;
		SYS_TIMER9_MARK=0;
		SYS_TIMER9=SYS_TIMER9_INIT;
	}
	else 
	{
		ACT_LED_OFF;
	}

	volatile unsigned char* base0;
	base0 = (volatile unsigned char*)LS1B_REG_BASE_CAN0;
	CAN_Receive(base0, &RxMessage);	 
    	
	CAN_R[CAN_ISR_PTR_R].dLEN = RxMessage.DLC;
	TemptID=RxMessage.ExtId;
	CAN_R[CAN_ISR_PTR_R].dID4=TemptID&0xff;
	CAN_R[CAN_ISR_PTR_R].dID3=(TemptID>>8)&0xff;
	CAN_R[CAN_ISR_PTR_R].dID2=(TemptID>>16)&0xff;
	CAN_R[CAN_ISR_PTR_R].dID1=(TemptID>>24)&0xff;
	CAN_R[CAN_ISR_PTR_R].MYDATA[0]=	RxMessage.Data[0];
	CAN_R[CAN_ISR_PTR_R].MYDATA[1]=	RxMessage.Data[1];
	CAN_R[CAN_ISR_PTR_R].MYDATA[2]=	RxMessage.Data[2];
	CAN_R[CAN_ISR_PTR_R].MYDATA[3]=	RxMessage.Data[3];
	CAN_R[CAN_ISR_PTR_R].MYDATA[4]=	RxMessage.Data[4];
	CAN_R[CAN_ISR_PTR_R].MYDATA[5]=	RxMessage.Data[5];
	CAN_R[CAN_ISR_PTR_R].MYDATA[6]=	RxMessage.Data[6];
	CAN_R[CAN_ISR_PTR_R].MYDATA[7]=	RxMessage.Data[7];
	FRAG_TYPE = ((CAN_R[CAN_ISR_PTR_R].dID1>>6) & 0x02) | ((CAN_R[CAN_ISR_PTR_R].dID2>>7));
	FRAG_COUNT= CAN_R[CAN_ISR_PTR_R].dID3;
	FRAG_COUNT= ((FRAG_COUNT<<1) | (CAN_R[CAN_ISR_PTR_R].dID4>>7)) & 0x1FF;

	if ((FRAG_TYPE==0)&&(FRAG_COUNT==0x1FF)&&(CAN_R[CAN_ISR_PTR_R].MYDATA[1]==CODE_TIMESET)) 
	{
		MODULE_CLK.M_HOUR=CAN_R[CAN_ISR_PTR_R].MYDATA[3];
		MODULE_CLK.MINUTE=CAN_R[CAN_ISR_PTR_R].MYDATA[4];
		if((IS_SOE!=0xFF)||( FIRST_SETCLK==0xFF))	
		MODULE_CLK.SECOND=CAN_R[CAN_ISR_PTR_R].MYDATA[5];
		FIRST_SETCLK=0;
		memcpy(&MS_TMP,&(CAN_R[CAN_ISR_PTR_R].MYDATA[6]),2);
		MS10_CNT=MS_TMP/10;
	}
	else 												
	{
		if((MODULE_ADDR==(CAN_R[CAN_ISR_PTR_R].dID1&0x7F)))
		{
			CAN_ISR_PTR_R++;								
			if (CAN_ISR_PTR_R>=CAN_BUF_CNT)	 CAN_ISR_PTR_R=0;
		}
	}
}
/*************************************************************************************
*Function name	:CAN_APP_RECV
*Description	:CAN receive
*
*Parameters		:None
*				 
*Returned		:None
*************************************************************************************/
uint32 CAN_APP_RECV(void)		
{
    uint8 SERIAL;
    uint8 DST_NODE;
    uint8 SRC_NODE;
    uint8 FRAG_TYPE;
    uint32 FRAG_COUNT;
    uint32 FRAG_PTR;
    uint8 PACK_LEN;
    
	while(CAN_ISR_PTR_R!=CAN_ISR_PTR_P)
	{
		SERIAL	  = ((CAN_R[CAN_ISR_PTR_P].dID4>>3) & 0x0F);
		DST_NODE  = (CAN_R[CAN_ISR_PTR_P].dID1 & 0x7F);
		SRC_NODE  = (CAN_R[CAN_ISR_PTR_P].dID2 & 0x7F);
		PACK_LEN  = CAN_R[CAN_ISR_PTR_P].dLEN;
		FRAG_TYPE = (((CAN_R[CAN_ISR_PTR_P].dID1>>6) & 0x02) | (CAN_R[CAN_ISR_PTR_P].dID2>>7));
		FRAG_COUNT= CAN_R[CAN_ISR_PTR_P].dID3;
		FRAG_COUNT= ((FRAG_COUNT<<1) | (CAN_R[CAN_ISR_PTR_P].dID4>>7)) & 0x1FF;

		if((FRAG_TYPE==0) && (FRAG_COUNT==0x1FF))  /* only one frag */
		{	
			APP_R[CAN_IN_PTR].MYSIZE=PACK_LEN;
			APP_R[CAN_IN_PTR].NODE=SRC_NODE;
			memcpy(&(APP_R[CAN_IN_PTR].MYDATA[0]), &(CAN_R[CAN_ISR_PTR_P].MYDATA[0]), CAN_R[CAN_ISR_PTR_P].dLEN);////????

			APP_R[CAN_IN_PTR].LETTER=1;
			CAN_ISR_PTR_P=CAN_ISR_PTR_P+1;
			if( CAN_ISR_PTR_P>=CAN_BUF_CNT)	 CAN_ISR_PTR_P=0;
			CAN_IN_PTR=CAN_IN_PTR+1;
			if( CAN_IN_PTR>=APP_BUF_CNT	) CAN_IN_PTR=0;
			
			CAN_STATE.STATE&=0x7f;
			CAN_R_BSP.STATUS= 0;

			return 0;
		}	

		else /* size >8 bytes */
		{
			if((FRAG_TYPE==1) && (FRAG_COUNT==0))/* first frag */ 
			{	 
				APP_R[CAN_IN_PTR].LETTER=0;
				CAN_R_BSP.DST= DST_NODE;
				CAN_R_BSP.SRC= SRC_NODE;
				CAN_R_BSP.SERIAL= SERIAL;
				CAN_R_BSP.MYSIZE= 8;
				CAN_R_BSP.COUNT= 0;
				memcpy( &APP_R[CAN_IN_PTR].MYDATA[0],&CAN_R[CAN_ISR_PTR_P].MYDATA[0], 8);
				CAN_R_BSP.STATUS= 1;
			}	

			else/*middle or last frag*/
			{		
				if(FRAG_TYPE==2)  /* middle frag */
				{	
					if((CAN_R_BSP.STATUS==1) && (CAN_R_BSP.SRC==SRC_NODE) && (CAN_R_BSP.SERIAL==SERIAL)&& ((CAN_R_BSP.COUNT+1)==FRAG_COUNT)) 
					{		
						CAN_R_BSP.COUNT=CAN_R_BSP.COUNT+1;
						FRAG_PTR= CAN_R_BSP.MYSIZE;
						CAN_R_BSP.MYSIZE=CAN_R_BSP.MYSIZE+8;
						memcpy( &APP_R[CAN_IN_PTR].MYDATA[FRAG_PTR], &CAN_R[CAN_ISR_PTR_P].MYDATA[0],8);
					}		
					else		
					{	
						CAN_R_BSP.SRC= 0;
						CAN_R_BSP.SERIAL= 0;
						CAN_R_BSP.MYSIZE= 0;
						CAN_R_BSP.COUNT= 0;
						CAN_R_BSP.STATUS= 0;
					}		
				}		
				else		
				{		
					if(FRAG_TYPE==3) /* last frag */
					{		
						if((CAN_R_BSP.STATUS==1) && (CAN_R_BSP.SRC==SRC_NODE) && (CAN_R_BSP.SERIAL==SERIAL)&& ((CAN_R_BSP.COUNT+1)==FRAG_COUNT)) 
						{		
							CAN_R_BSP.COUNT=CAN_R_BSP.COUNT+1;
							FRAG_PTR= CAN_R_BSP.MYSIZE;
							CAN_R_BSP.MYSIZE=CAN_R_BSP.MYSIZE+PACK_LEN;
							memcpy(&APP_R[CAN_IN_PTR].MYDATA[FRAG_PTR],&CAN_R[CAN_ISR_PTR_P].MYDATA[0],PACK_LEN);
							APP_R[CAN_IN_PTR].MYSIZE=CAN_R_BSP.MYSIZE;
							APP_R[CAN_IN_PTR].NODE=SRC_NODE;
							APP_R[CAN_IN_PTR].LETTER=1;
							CAN_ISR_PTR_P=CAN_ISR_PTR_P+1;
							if(CAN_ISR_PTR_P>=CAN_BUF_CNT)	CAN_ISR_PTR_P=0;
							CAN_IN_PTR=CAN_IN_PTR+1;						
							if(CAN_IN_PTR>=APP_BUF_CNT) CAN_IN_PTR=0;		

							CAN_STATE.STATE&=0x7f;
							CAN_R_BSP.STATUS= 0;
							return 1;
						}			
						else		
						{		
							CAN_R_BSP.SRC= 0;
							CAN_R_BSP.SERIAL= 0;
							CAN_R_BSP.MYSIZE= 0;
							CAN_R_BSP.COUNT= 0;
							CAN_R_BSP.STATUS= 0;
						}	
					}	
					else	
					{	
						
					}	
				}
			}		
		}
		CAN_ISR_PTR_P=CAN_ISR_PTR_P+1;
		if( CAN_ISR_PTR_P>=CAN_BUF_CNT) CAN_ISR_PTR_P=0;
	}	
	return 0;
}

/*************************************************************************************
*Function name	:CAN_SEND_BSP
*Description	:CAN send
*
*Parameters		:SEND_INDEX-->CAN send ID
*				 
*Returned		:0/2
*************************************************************************************/
uint32 CAN_SEND_BSP(uint8 SEND_INDEX)
{
	uint32 CAN_S_ID_Temp;
	CanTxMsg TxMessage;
	volatile unsigned char* base0;
	base0= (volatile unsigned char*)LS1B_REG_BASE_CAN0;
	if(SEND_INDEX>=CAN_BUF_CNTS)  return 0x02;
	
	CAN_S_ID_Temp=(CAN_S[SEND_INDEX].dID1<<24|((CAN_S[SEND_INDEX].dID2&0x000000ff)<<16)|((CAN_S[SEND_INDEX].dID3&0x000000ff)<<8)|(CAN_S[SEND_INDEX].dID4&0x000000ff));//

	/* transmit */
	TxMessage.ExtId = CAN_S_ID_Temp;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_EXT;
    TxMessage.DLC = CAN_S[SEND_INDEX].dLEN;;
    TxMessage.Data[0] = CAN_S[SEND_INDEX].MYDATA[0];
    TxMessage.Data[1] = CAN_S[SEND_INDEX].MYDATA[1];
	TxMessage.Data[2] = CAN_S[SEND_INDEX].MYDATA[2];
	TxMessage.Data[3] = CAN_S[SEND_INDEX].MYDATA[3];
	TxMessage.Data[4] = CAN_S[SEND_INDEX].MYDATA[4];
	TxMessage.Data[5] = CAN_S[SEND_INDEX].MYDATA[5];
	TxMessage.Data[6] = CAN_S[SEND_INDEX].MYDATA[6];
	TxMessage.Data[7] = CAN_S[SEND_INDEX].MYDATA[7];

	CAN_Transmit(base0, &TxMessage);
	return 0;
}

/*************************************************************************************
*Function name	:SEND_REQ_BSP
*Description	:send can frag
*
*Parameters		:None
*				 
*Returned		:0/0x31
*************************************************************************************/
uint32 SEND_REQ_BSP(void)
{
	uint8 DST;
	uint8 LAST_FRAG;
	uint32 MYSIZE;
	uint32 FRAG_NUM;
	uint32 I;
	
	DST = APP_S.NODE;
	MYSIZE=  APP_S.MYSIZE;
	CAN_S_BSP.DST= DST;
	CAN_S_BSP.FRAG_COUNT=0;
	CAN_S_BSP.FRAG_TOTAL=0;
	CAN_S_BSP.OVER_NUM=0;
  if(MYSIZE<=8) 
	{	
		CAN_S_BSP.FRAG_COUNT=0;//
		CAN_S_BSP.FRAG_TOTAL=0;//
		CAN_S[0].dLEN=MYSIZE;
		CAN_S[0].dID1=DST&0x7F; /*FragType=00B*/
		CAN_S[0].dID2=MODULE_ADDR;//0;	/* do not care */
		CAN_S[0].dID3=0xFF;
		CAN_S[0].dID4=0x80;
		memcpy( &CAN_S[0].MYDATA[0],&APP_S.MYDATA[0], 8);
	}	
	else
	{	
		/*msg_size>8*/
		/*tot frag=frag_num+1*/
		if(MYSIZE>BUF_LEN)  return 0x31;
		FRAG_NUM=(MYSIZE-1)/8;
		if((MYSIZE%8)==0)LAST_FRAG=8;
		else LAST_FRAG=(MYSIZE%8);
		CAN_S_BSP.FRAG_TOTAL=FRAG_NUM;
		CAN_S_BSP.SERIAL=CAN_S_BSP.SERIAL+1;

		for(I=0;I<=FRAG_NUM;I++)	
 		{
 			if(I==0) /* first frag */
			{ 
				CAN_S[I].dLEN=8;
				CAN_S[I].dID1=DST&0x7F; /*FragType=01B*/
		    	CAN_S[I].dID2=0x80|MODULE_ADDR;	/* do not care */
      			CAN_S[I].dID3=0x00;
      			CAN_S[I].dID4=(CAN_S_BSP.SERIAL & 0x0F)<<3;
				memcpy( &CAN_S[I].MYDATA[0],&APP_S.MYDATA[0], 8);
			}	
			else
			{	
				if(I==FRAG_NUM)/*last frag*/
				{		 
					CAN_S[I].dLEN=LAST_FRAG;
				  	CAN_S[I].dID1=(DST&0x7F)|0x80; /*FragType=11B*/
		      		CAN_S[I].dID2=0x80|MODULE_ADDR;	/* do not care */
      				CAN_S[I].dID3=(I>>1);
      				CAN_S[I].dID4=((CAN_S_BSP.SERIAL&0x0F)<< 3)|((I&0x01)<<7);
					memcpy(&CAN_S[I].MYDATA[0],&APP_S.MYDATA[I*8],LAST_FRAG);
				}	
				else/*middle frag*/
				{	 
					CAN_S[I].dLEN=8;
				  	CAN_S[I].dID1=(DST&0x7F)|0x80; /*FragType=10B*/
		      		CAN_S[I].dID2=0x00|MODULE_ADDR;	/* do not care */
      				CAN_S[I].dID3=(I>>1);
      				CAN_S[I].dID4=((CAN_S_BSP.SERIAL&0x0F)<<3)|((I&0x01)<<7);
					memcpy(&CAN_S[I].MYDATA[0],&APP_S.MYDATA[I*8],8);
				}    
			}	
		}
	}	
	return 0;
}

/*************************************************************************************
*Function name	:CAN_APP_SEND
*Description	:
*
*Parameters		:None
*				 
*Returned		:0/0x62/0x61
*************************************************************************************/
uint32 CAN_APP_SEND(void)
{
	uint32 RETCODE,I=0;
	if( (APP_S.STATE!=1) && (APP_S.STATE!=2) ) 	return 0x61;

	if(APP_S.STATE==1) 
	{	
		APP_S.STATE=2;
		RETCODE=SEND_REQ_BSP();/* call */
		CAN_SEND_BSP(I);
		if(RETCODE!=0) 
		{	
			APP_S.STATE=3;
			return 0x62;
		}	
	}	
	if((APP_S.STATE==2)&&(CAN_S_BSP.FRAG_TOTAL!=0)) 
	{	
		for(I=1;I<(CAN_S_BSP.FRAG_TOTAL+1);I++) 					
		
		{		
			CAN_S_BSP.FRAG_COUNT=I;
			RETCODE=0;
APP_S_AGAIN:if(RETCODE==0)  RETCODE=CAN_SEND_BSP(I);
			if(RETCODE!=0) 
			{	
				CAN_S_BSP.OVER_NUM++;
	    		if(CAN_S_BSP.OVER_NUM>MAX_ERR_S) /* fail */
				{	 
					APP_S.STATE=3;
					CAN_STATE.STATE|=0x02;
					CAN_STATE.STATE|=0x80;
					CAN_STATE.ERR_S++;
				}	
				else
				{	
					delay_us(10);
					goto APP_S_AGAIN;
				}	
				
			}
			else CAN_S_BSP.OVER_NUM = 0;
		}
	}	
	APP_S.STATE=0;
	CAN_STATE.STATE&=0xfd;
	CAN_STATE.STATE&=0x7f;
	return 0;
}
/********************************************* End of File *********************************************/
