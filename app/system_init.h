/****************************************Copyright (c)**************************************************
**                               南大傲拓科技江苏股份有限公司
**                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/
#ifndef _SYSTEM_INIT_H_
#define _SYSTEM_INIT_H_
#include "config.h"

extern void NA_GPIO_Init(void);
extern void NA_Modnle_Init(void);
extern uint32 S_BITTST(uint8 *P,uint8 NO);
extern uint32 S_BITTST32(uint32 *P,uint8 NO);
extern void S_BITCLR(uint8 *P,uint8 NO);
extern void S_BITSET(uint8 *P,uint8 NO);
#endif

/********************************************* End of File *********************************************/
