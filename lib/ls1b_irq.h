

#ifndef __IRQ_H
#define __IRQ_H


// �жϺ�

#define LS1B_UART0_IRQ	2 
#define LS1B_UART1_IRQ	3
#define LS1B_UART2_IRQ	4
#define LS1B_UART3_IRQ	5
#define LS1B_CAN0_IRQ	6
#define LS1B_CAN1_IRQ	7
#define LS1B_SPI0_IRQ	8
#define LS1B_SPI1_IRQ	9
#define LS1B_AC97_IRQ	10
#define LS1B_DMA0_IRQ	13
#define LS1B_DMA1_IRQ	14
#define LS1B_DMA2_IRQ   15
#define LS1B_PWM0_IRQ	17
#define LS1B_PWM1_IRQ	18
#define LS1B_PWM2_IRQ	19
#define LS1B_PWM3_IRQ	20
#define LS1B_RTC_INT0_IRQ  21
#define LS1B_RTC_INT1_IRQ  22
#define LS1B_RTC_INT2_IRQ  23
#define LS1B_TOY_INT0_IRQ  24
#define LS1B_TOY_INT1_IRQ  25
#define LS1B_TOY_INT2_IRQ  26
#define LS1B_RTC_TICK_IRQ  27
#define LS1B_TOY_TICK_IRQ  28
#define LS1B_UART4_IRQ  29
#define LS1B_UART5_IRQ  30



#define LS1B_EHCI_IRQ	(32+0)
#define LS1B_OHCI_IRQ	(32+1)
#define LS1B_MAC0_IRQ    (32+2)
#define LS1B_MAC1_IRQ    (32+3)


#define LS1B_GPIO_IRQ 64
#define LS1B_GPIO_FIRST_IRQ 64
#define LS1B_GPIO_IRQ_COUNT 64
#define LS1B_GPIO_LAST_IRQ  (LS1B_GPIO_FIRST_IRQ + LS1B_GPIO_IRQ_COUNT-1)


#define LS1B_LAST_IRQ 127

// ��о1B���жϷ�Ϊ4 �飬ÿ��32��
#define LS1B_NR_IRQS    (32*4)


// GPIO��ź��жϺ�֮��Ļ���ת��
#define LS1B_GPIO_TO_IRQ(GPIOn)     (LS1B_GPIO_FIRST_IRQ + (GPIOn))
#define LS1B_IRQ_TO_GPIO(IRQn)      ((IRQn) - LS1B_GPIO_FIRST_IRQ)


// �����жϴ�����
typedef void (*irq_handler_t)(int IRQn, void *param);
typedef struct irq_desc
{
    irq_handler_t   handler;
    void            *param;
}irq_desc_t;


// ��ʼ���쳣
void exception_init(void);


/*
 * ʹ��ָ���ж�
 * @IRQn �жϺ�
 */
void irq_enable(int IRQn);


/*
 * ��ָֹ���ж�
 * @IRQn �жϺ�
 */
void irq_disable(int IRQn);


/*
 * �����жϴ�����
 * @IRQn �жϺ�
 * @new_handler �����õ��жϴ�����
 * @param ���ݸ��жϴ������Ĳ���
 * @ret ֮ǰ���жϴ�����
 */
irq_handler_t irq_install(int IRQn, irq_handler_t new_handler, void *param);




#endif


