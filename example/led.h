/****************************************Copyright (c)**************************************************
**                               �ϴ���ؿƼ����չɷ����޹�˾
**                                        �з���
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/

#ifndef _LED_H_
#define _LED_H_

// ��ʼ��led
// @led_gpio led����gpio����
extern void led_init(unsigned int led_gpio);

// ����led
// @led_gpio led����gpio����
extern void led_on(unsigned int led_gpio);

// Ϩ��led
// @led_gpio
extern void led_off(unsigned int led_gpio);
extern void led_init_all (void);



#endif
/********************************************* End of File *********************************************/
