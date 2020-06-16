// �������Դ��

#include <stdio.h>
#include <stdarg.h>
#include "ls1b_public.h"
#include "ls1b_regs.h"
#include "ls1b_pin.h"
#include "ls1b_uart.h"
#include "ls1b_clock.h"
#include "ls1b_irq.h"





// ���Դ�����Ϣ
ls1b_uart_info_t debug_uart_info = {0};


/*
 * ��ȡָ������ģ��Ļ���ַ
 * @UARTx ���ڱ��
 * @ret ����ַ
 */
void *uart_get_base(ls1b_uart_t UARTx)
{
    void *base = NULL;

    switch (UARTx)
    {
        case BLS1_UART0:
            base = (void *)BLS1_UART0_BASE;
            break;

        case BLS1_UART1:
            base = (void *)BLS1_UART1_BASE;
            break;
        
        case BLS1_UART2:
            base = (void *)BLS1_UART2_BASE;
            break;

		case BLS1_UART3:
            base = (void *)BLS1_UART3_BASE;
            break;

		case BLS1_UART4:
            base = (void *)BLS1_UART4_BASE;
            break;

		case BLS1_UART5:
            base = (void *)BLS1_UART5_BASE;
            break;
			
        default:
            break;
    }

    return base;
}


/*
 * ��ʼ��ָ���Ĵ���ģ��
 * @uart_info_p ����ģ����Ϣ
 */
extern unsigned char MODULE_STATE;
void uart_init(ls1b_uart_info_t *uart_info_p)
{
    void *uart_base = uart_get_base(uart_info_p->UARTx);
    unsigned long baudrate_div = 0;
	unsigned char lcr_value=0;
	unsigned int Buadrate_Back;
    // ��ֹ�����ж�
    reg_write_8(0,      uart_base + BLS1_UART_IER_OFFSET);
    
    // ����FIFO���ж�����TriggerΪ14�ֽڣ���շ��ͺͽ���FIFO������λ //20190919�ĳ�Trigger 1�ֽ�
    reg_write_8(0x03,   uart_base + BLS1_UART_FCR_OFFSET);
    //reg_write_8(0x07,   uart_base + BLS1_UART_FCR_OFFSET);
    // ���ò�����
    reg_write_8(0x80,   uart_base + BLS1_UART_LCR_OFFSET);

	//baudrate_div = clk_get_cpu_rate();
	//myprintf(" clk::%d  \r\n", baudrate_div);
	
    baudrate_div = 128000000 / 16 / uart_info_p->baudrate / 2;
    reg_write_8((baudrate_div >> 8) & 0xff, uart_base + BLS1_UART_MSB_OFFSET);
    reg_write_8(baudrate_div & 0xff,        uart_base + BLS1_UART_LSB_OFFSET);

    // ����λ��ֹͣλ��У��
    if(uart_info_p->databit==7)
    lcr_value=0x02;
	else if(uart_info_p->databit==6)
    lcr_value=0x01;
	else if(uart_info_p->databit==5)
    lcr_value=0x00;
	else lcr_value=0x03;

	if(uart_info_p->stopbit>1)
		lcr_value |=(1<<2);

	if(uart_info_p->parity)
	{
		lcr_value |=(1<<3);
		lcr_value |=((uart_info_p->parity-1)&0x01)<<4;	// 0����У�飻1��żУ��
	}

	Buadrate_Back = ((reg_read_8(uart_base + BLS1_UART_MSB_OFFSET)<<8)+(reg_read_8(uart_base + BLS1_UART_LSB_OFFSET)));
	if((128000000 / 16 / uart_info_p->baudrate / 2)==Buadrate_Back)//�ж��Ƿ���ȷд�벨����
	{
		//myprintf("Buadrate_Back=====%d\n",Buadrate_Back);
		//led_off(34);//����ϵ�
	}
	else
	{
		MODULE_STATE |=	0x02;//ģ���Լ����
		//led_on(34);//�����ϵ�
	}
	
    reg_write_8(lcr_value,   uart_base + BLS1_UART_LCR_OFFSET);
    // ʹ�ܽ��շ����ж�
	reg_write_8((1<<0)|(1<<1)|(1<<2) , uart_base + BLS1_UART_IER_OFFSET);	
    return ;
}


/*
 * �ж�FIFO�Ƿ�Ϊ��
 * @uart_info_p ����ģ����Ϣ
 * @ret TRUE or FALSE
 */
BOOL uart_is_transmit_empty(ls1b_uart_t uartx)
{
	void *uart_base = uart_get_base(uartx);
	unsigned char status = reg_read_8(uart_base + BLS1_UART_LSR_OFFSET);
 
	if (status & (BLS1_UART_LSR_TE | BLS1_UART_LSR_TFE))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



/*
 * ����һ���ֽ�
 * @uartx ���ں�
 * @ch �����͵��ַ���
 */
void uart_putc(ls1b_uart_t uartx, unsigned char ch)
{
    void *uart_base = uart_get_base(uartx);

    // �ȴ�
    while (FALSE == uart_is_transmit_empty(uartx));

    // ����
    reg_write_8(ch, uart_base + BLS1_UART_DAT_OFFSET);

    return ;
}


/*
 * ��ӡһ���ַ�����ָ������
 * @uart_info_p ����ģ����Ϣ
 * @str ����ӡ���ַ���
 */
void uart_print(ls1b_uart_t uartx, const char *str)
{
	while ('\0' != *str)				// �ж��Ƿ�Ϊ�ַ���������
	{
		uart_putc(uartx, *str);   // ����һ���ַ�
		str++;
	}

	return ;
}



/*
 * ��ʼ������2
 */
/*void uart2_init(void)
{
    unsigned int tx_gpio = 37;
    unsigned int rx_gpio = 36;

    // ���ø���
    pin_set_remap(tx_gpio, PIN_REMAP_SECOND);
    pin_set_remap(rx_gpio, PIN_REMAP_SECOND);
    
    // ��ʼ����ؼĴ���
    debug_uart_info.UARTx = BLS1_UART2;
    debug_uart_info.baudrate = 115200;
    uart_init(&debug_uart_info);

    return ;
}*/


/*
 * �ڴ���2�ϴ�ӡ�ַ���
 * @str ����ӡ���ַ���
 */
void uart2_print(const char *str)
{
    uart_print(BLS1_UART2, str);
    return ;
}


/*
 * �ο�STM32���ض���printf��������֪��Ϊʲô��ʧ�ܣ�����һ�����������¼����
 *
 * �ض���c�⺯��printf�����Դ���
 * �ض�����ָ�û��Լ���дc�Ŀ⺯��
 * ����������鵽�û���д����c�⺯����ͬ���ֵĺ���ʱ��
 * ���Ȳ����û���д�ĺ�����������ʵ���˶Կ⺯�����޸ģ����ض���
 
int fputc(int ch, FILE *f)
{
    uart_putc(&debug_uart_info, (unsigned char)ch);
    return ch;
}
 */

 inline ls1b_uart_t uart_irqn_to_uartx(int IRQn)
 {
	 ls1b_uart_t uartx;
	 
	 switch (IRQn)
	 {
		 /* ����UART00��UART01���жϺŻ���ȷ��
		 case BLS1_UART00_IRQ:
			 uartx = BLS1_UART00;
			 break;
		 case BLS1_UART01_IRQ:
			 uartx = BLS1_UART01;
			 break;
		*/
  
		 case LS1B_UART1_IRQ:
			 uartx = BLS1_UART1;
			 break;
			 
		 case LS1B_UART2_IRQ:
			 uartx = BLS1_UART2;
			 break;
		 
		 case LS1B_UART3_IRQ:
			 uartx = BLS1_UART3;
			 break;
		 
		 case LS1B_UART4_IRQ:
			 uartx = BLS1_UART4;
			 break;
		 
		 case LS1B_UART5_IRQ:
			 uartx = BLS1_UART5;
			 break;
		 
		 
		 default:
			 uartx = BLS1_UART2;
			 break;
	 }
  
	 return uartx;
 }



