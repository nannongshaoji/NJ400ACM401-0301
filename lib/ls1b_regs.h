// ÁúÐ¾1cÍâÉè¼Ä´æÆ÷


#ifndef __OPENLOONGSON_LS1B_REGS_H
#define __OPENLOONGSON_LS1B_REGS_H




// Ê±ÖÓÏà¹Ø¼Ä´æÆ÷µØÖ·
#define LS1B_START_FREQ                     (0xbfe78030)
#define LS1B_CLK_DIV_PARAM                  (0xbfe78034)


// gpioÏà¹Ø¼Ä´æÆ÷µØÖ·
#define LS1B_GPIO_CFG0                      (0xbfd010c0)
#define LS1B_GPIO_EN0                       (0xbfd010d0)
#define LS1B_GPIO_IN0                       (0xbfd010e0)
#define LS1B_GPIO_OUT0                      (0xbfd010f0)

#define LS1B_GPIO_CFG1                      (0xbfd010c4)
#define LS1B_GPIO_EN1                       (0xbfd010d4)
#define LS1B_GPIO_IN1                       (0xbfd010e4)
#define LS1B_GPIO_OUT1                      (0xbfd010f4)



// ¸´ÓÃÏà¹Ø¼Ä´æÆ÷
#define LS1B_GPIO_MUX_CTRL0            (0xbfd00420)
#define LS1B_GPIO_MUX_CTRL1            (0xbfd00424)


// PWM¼Ä´æÆ÷Æ«ÒÆ
#define LS1B_PWM_CNTR                       (0x0)
#define LS1B_PWM_HRC                        (0x4)
#define LS1B_PWM_LRC                        (0x8)
#define LS1B_PWM_CTRL                       (0xC)
// PWM»ùµØÖ·
#define LS1B_REG_BASE_PWM0                  (0xbfe5c000)
#define LS1B_REG_BASE_PWM1                  (0xbfe5c010)
#define LS1B_REG_BASE_PWM2                  (0xbfe5c020)
#define LS1B_REG_BASE_PWM3                  (0xbfe5c030)
//CAN»ùµØÖ·
#define  LS1B_REG_BASE_CAN0				    (0xbfe50000)
#define  LS1B_REG_BASE_CAN1					(0xbfe54000)



// ÖÐ¶ÏÅäÖÃ¼Ä´æÆ÷
#define LS1B_INT0_SR                        (0xbfd01040)
#define LS1B_INT0_EN                        (0xbfd01044)
#define LS1B_INT0_SET                       (0xbfd01048)
#define LS1B_INT0_CLR                       (0xbfd0104c)
#define LS1B_INT0_POL                       (0xbfd01050)
#define LS1B_INT0_EDGE                      (0xbfd01054)

#define LS1B_INT1_SR                        (0xbfd01058)
#define LS1B_INT1_EN                        (0xbfd0105c)
#define LS1B_INT1_SET                       (0xbfd01060)
#define LS1B_INT1_CLR                       (0xbfd01064)
#define LS1B_INT1_POL                       (0xbfd01068)
#define LS1B_INT1_EDGE                      (0xbfd0106c)

#define LS1B_INT2_SR                        (0xbfd01070)
#define LS1B_INT2_EN                        (0xbfd01074)
#define LS1B_INT2_SET                       (0xbfd01078)
#define LS1B_INT2_CLR                       (0xbfd0107c)
#define LS1B_INT2_POL                       (0xbfd01080)
#define LS1B_INT2_EDGE                      (0xbfd01084)

#define LS1B_INT3_SR                        (0xbfd01088)
#define LS1B_INT3_EN                        (0xbfd0108c)
#define LS1B_INT3_SET                       (0xbfd01090)
#define LS1B_INT3_CLR                       (0xbfd01094)
#define LS1B_INT3_POL                       (0xbfd01098)
#define LS1B_INT3_EDGE                      (0xbfd0109c)



// I2C¼Ä´æÆ÷
#define LS1B_I2C0_BASE                       (0xbfe58000)
#define LS1B_I2C1_BASE                      (0xbfe68000)
#define LS1B_I2C2_BASE                      (0xbfe70000)


// ´®¿Ú¼Ä´æÆ÷
#define BLS1_UART0_BASE                     (0xbfe40000)
#define BLS1_UART1_BASE                     (0xbfe44000)
#define BLS1_UART2_BASE                     (0xbfe48000)
#define BLS1_UART3_BASE                     (0xbfe4c000)
#define BLS1_UART4_BASE                     (0xbfe6c000)
#define BLS1_UART5_BASE                     (0xbfe7c000)

// SPI¼Ä´æÆ÷
#define	LS1B_SPI0_BASE						(0xbfe80000)
#define	LS1B_SPI1_BASE						(0xbfec0000)


#endif

