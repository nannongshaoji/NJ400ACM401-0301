/****************************************Copyright (c)**************************************************
**                               ??????????????
**                                        ???
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/
#include "../lib/ls1b_gpio.h"
#include "../app/config.h"

/********************************************************************************************************
*Function name	:led_init
*Description	:initialize the LED
*
*Parameters 	:led_gpio-->LED gpio_pin
*				 
*Returned		:None
********************************************************************************************************/
void led_init(unsigned int led_gpio)
{
    gpio_init(led_gpio, gpio_mode_output);
    gpio_set(led_gpio, gpio_level_high);       //Ö¸Ê¾µÆÄ¬ÈÏÏ¨Ãð
    return ;
}
/********************************************************************************************************
*Function name	:led_init_all
*Description	:initialize the all LEDs
*
*Parameters 	:None
*				 
*Returned		:None
*******************************************************************************************************/
void led_init_all (void)
{
	//led_init(RUN_LED);
	//led_init(ACK_LED);
	//led_init(FAULT_LED);
	return ;
}
/********************************************************************************************************
*Function name	:led_on
*Description	:
*
*Parameters 	:led_gpio-->LED gpio_pin
*				 
*Returned		:None
********************************************************************************************************/
void led_on(unsigned int led_gpio)
{
    gpio_set(led_gpio, gpio_level_low);
    return ;
}
/********************************************************************************************************
*Function name	:led_off
*Description	:
*
*Parameters 	:led_gpio-->LED gpio_pin
*				 
*Returned		:None
********************************************************************************************************/
void led_off(unsigned int led_gpio)
{
    gpio_set(led_gpio, gpio_level_high);
    return ;
}
/********************************************* End of File *********************************************/
