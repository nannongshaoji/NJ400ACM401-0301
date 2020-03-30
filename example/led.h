/****************************************Copyright (c)**************************************************
**                               南大傲拓科技江苏股份有限公司
**                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/

#ifndef _LED_H_
#define _LED_H_

// 初始化led
// @led_gpio led所在gpio引脚
extern void led_init(unsigned int led_gpio);

// 点亮led
// @led_gpio led所在gpio引脚
extern void led_on(unsigned int led_gpio);

// 熄灭led
// @led_gpio
extern void led_off(unsigned int led_gpio);
extern void led_init_all (void);



#endif
/********************************************* End of File *********************************************/
