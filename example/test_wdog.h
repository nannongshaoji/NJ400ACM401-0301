/****************************************Copyright (c)**************************************************
**                               南大傲拓科技江苏股份有限公司
**                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/

#ifndef _TEST_WDOG_H_
#define _TEST_WDOG_H_
#define WDG_OE (35)
#define WDG_IN (36)

extern void wdg_init(void);
extern void wdg_enable (void);
extern void CLEAR_HARDWDOG(void);
extern void test_wdog(void);
#endif
/********************************************* End of File *********************************************/
