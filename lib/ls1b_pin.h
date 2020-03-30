// 引脚功能(普通gpio，pwm，复用等)相关接口

#ifndef __OPENLOONGSON_PIN_H
#define __OPENLOONGSON_PIN_H


// 引脚用途
typedef enum
{
    PIN_PURPOSE_GPIO = 0,               // 引脚用作普通gpio
    PIN_PURPOSE_OTHER,                  // 引脚用作其它功能(非gpio)
}pin_purpose_t;


// 引脚复用
typedef enum
{
	LCD_USE_UART0=0,
	LCD_USE_UART15,
	LCD_USE_UART0_DAT,
	GMAC1_USE_UART0,
	GMAC1_USE_UART1,
	NAND1_USE_PWM01=6,
	NAND1_USE_PWM23,
	NAND1_USE_UART1_CTS,
	NAND1_USE_UART1_DAT,
	NAND1_USE_UART4,
	NAND1_USE_UART5,
	NAND2_USE_PWM01,
	NAND2_USE_PWM23,
	NAND2_USE_UART1_CTS,
	NAND2_USE_UART1_DAT,
	NAND2_USE_UART4,
	NAND2_USE_UART5,
	NAND3_USE_PWM01,
	NAND3_USE_PWM23,
	NAND3_USE_UART1_CTS,
	NAND3_USE_UART1_DAT,
	NAND3_USE_UART4,
	NAND3_USE_UART5,   
	I2C1_USE_CAN0,
	I2C2_USE_CAN1,
	UART1_USE_LCD0_5_6_11,
	UART0_USE_PWM01,
	UART0_USE_PWM23,
	GMAC0_USE_PWM01,
	GMAC1_USE_PWM23,
	GMAC0_USE_TX_CLK,
	GMAC1_USE_TX_CLK,
	UART1_2_USE_CAN0,
	UART1_3_USE_CAN1,
	USE_SHUT=43,
	GMAC0_SHUT,
	GMAC1_SHUT,
	DDR32TO16EN=48,
	DISABLE_DDR_CONFSPACE=52,
	SPI1_USE_CAN,
	SPI1_CS_USE_PWM01,
}pin_remap_t;


/*
 * 把指定pin设置为指定用途(普通gpio，非gpio)
 * @gpio gpio引脚编号
 * @purpose 用途
 */
void pin_set_purpose(unsigned int gpio, pin_purpose_t purpose);


/*
 * 设置复用功能
 * @remap为复用代码
 */
void pin_set_remap(pin_remap_t remap);


#endif

