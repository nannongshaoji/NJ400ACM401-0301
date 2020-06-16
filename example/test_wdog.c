/****************************************Copyright (c)**************************************************
**                               南大傲拓科技江苏股份有限公司
**                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/


#include "../lib/ls1b_public.h"
#include "../lib/ls1b_gpio.h"
#include "../lib/ls1b_delay.h"
#include "../lib/ls1b_irq.h"
#include "test_wdog.h"
#include "led.h"
/*************************************************************************************
*Function name	:wdg_init
*Description	:initialize the watch dog
*
*Parameters 	:None
*
*Returned		:None
*************************************************************************************/
void wdg_init(void)
{
	gpio_init(WDG_OE, gpio_mode_output);
	gpio_init(WDG_IN, gpio_mode_output);
}
/*************************************************************************************
*Function name	:wdg_enable
*Description	:enable the watch dog
*
*Parameters 	:None
*
*Returned		:None
*************************************************************************************/
void wdg_enable (void)
{
    gpio_set(WDG_OE, gpio_level_low);//置低
}
/*************************************************************************************
*Function name	:CLEAR_HARDWDOG
*Description	:Feed dog
*
*Parameters 	:None
*
*Returned		:None
*************************************************************************************/
void CLEAR_HARDWDOG(void)
{
	static volatile char dog=0;
	if(dog==0){
		dog=1;
		 gpio_set(WDG_IN, gpio_level_high);
	}
	else{
		dog=0;
		 gpio_set(WDG_IN, gpio_level_low);	
	}
}
/*************************************************************************************
*Function name	:test_wdog
*Description	:test
*
*Parameters 	:None
*
*Returned		:None
*************************************************************************************/
void test_wdog(void)
{
	int i=0;
	wdg_init();
	wdg_enable();
	for(i=0;i<5;i++){
		 CLEAR_HARDWDOG();//喂狗
		 delay_ms(500);
		 myprintf("\ni=%d\n",i);
		}
	for(i=0;i<5;i++){
		 CLEAR_HARDWDOG();//喂狗
		 delay_s(2);
		 myprintf("\ni=%d\n",i);
		}
  
    return ;
}
/********************************************* End of File *********************************************/
