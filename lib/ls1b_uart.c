// 串口相关源码

#include <stdio.h>
#include <stdarg.h>
#include "ls1b_public.h"
#include "ls1b_regs.h"
#include "ls1b_pin.h"
#include "ls1b_uart.h"
#include "ls1b_clock.h"
#include "ls1b_irq.h"





// 调试串口信息
ls1b_uart_info_t debug_uart_info = {0};


/*
 * 获取指定串口模块的基地址
 * @UARTx 串口编号
 * @ret 基地址
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
 * 初始化指定的串口模块
 * @uart_info_p 串口模块信息
 */
extern unsigned char MODULE_STATE;
void uart_init(ls1b_uart_info_t *uart_info_p)
{
    void *uart_base = uart_get_base(uart_info_p->UARTx);
    unsigned long baudrate_div = 0;
	unsigned char lcr_value=0;
	unsigned int Buadrate_Back;
    // 禁止所有中断
    reg_write_8(0,      uart_base + BLS1_UART_IER_OFFSET);
    
    // 接收FIFO的中断申请Trigger为14字节，清空发送和接收FIFO，并复位 //20190919改成Trigger 1字节
    reg_write_8(0x03,   uart_base + BLS1_UART_FCR_OFFSET);
    //reg_write_8(0x07,   uart_base + BLS1_UART_FCR_OFFSET);
    // 设置波特率
    reg_write_8(0x80,   uart_base + BLS1_UART_LCR_OFFSET);

	//baudrate_div = clk_get_cpu_rate();
	//myprintf(" clk::%d  \r\n", baudrate_div);
	
    baudrate_div = 128000000 / 16 / uart_info_p->baudrate / 2;
    reg_write_8((baudrate_div >> 8) & 0xff, uart_base + BLS1_UART_MSB_OFFSET);
    reg_write_8(baudrate_div & 0xff,        uart_base + BLS1_UART_LSB_OFFSET);

    // 数据位，停止位，校验
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
		lcr_value |=((uart_info_p->parity-1)&0x01)<<4;	// 0：奇校验；1：偶校验
	}

	Buadrate_Back = ((reg_read_8(uart_base + BLS1_UART_MSB_OFFSET)<<8)+(reg_read_8(uart_base + BLS1_UART_LSB_OFFSET)));
	if((128000000 / 16 / uart_info_p->baudrate / 2)==Buadrate_Back)//判断是否正确写入波特率
	{
		//myprintf("Buadrate_Back=====%d\n",Buadrate_Back);
		//led_off(34);//灭故障灯
	}
	else
	{
		MODULE_STATE |=	0x02;//模块自检故障
		//led_on(34);//亮故障灯
	}
	
    reg_write_8(lcr_value,   uart_base + BLS1_UART_LCR_OFFSET);
    // 使能接收发送中断
	reg_write_8((1<<0)|(1<<1)|(1<<2) , uart_base + BLS1_UART_IER_OFFSET);	
    return ;
}


/*
 * 判断FIFO是否为空
 * @uart_info_p 串口模块信息
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
 * 发送一个字节
 * @uartx 串口号
 * @ch 待发送的字符串
 */
void uart_putc(ls1b_uart_t uartx, unsigned char ch)
{
    void *uart_base = uart_get_base(uartx);

    // 等待
    while (FALSE == uart_is_transmit_empty(uartx));

    // 发送
    reg_write_8(ch, uart_base + BLS1_UART_DAT_OFFSET);

    return ;
}


/*
 * 打印一个字符串到指定串口
 * @uart_info_p 串口模块信息
 * @str 待打印的字符串
 */
void uart_print(ls1b_uart_t uartx, const char *str)
{
	while ('\0' != *str)				// 判断是否为字符串结束符
	{
		uart_putc(uartx, *str);   // 发送一个字符
		str++;
	}

	return ;
}



/*
 * 初始化串口2
 */
/*void uart2_init(void)
{
    unsigned int tx_gpio = 37;
    unsigned int rx_gpio = 36;

    // 设置复用
    pin_set_remap(tx_gpio, PIN_REMAP_SECOND);
    pin_set_remap(rx_gpio, PIN_REMAP_SECOND);
    
    // 初始化相关寄存器
    debug_uart_info.UARTx = BLS1_UART2;
    debug_uart_info.baudrate = 115200;
    uart_init(&debug_uart_info);

    return ;
}*/


/*
 * 在串口2上打印字符串
 * @str 待打印的字符串
 */
void uart2_print(const char *str)
{
    uart_print(BLS1_UART2, str);
    return ;
}


/*
 * 参考STM32中重定向printf函数，不知道为什么会失败，，待一会继续调，记录备忘
 *
 * 重定向c库函数printf到调试串口
 * 重定向是指用户自己重写c的库函数
 * 当连接器检查到用户编写了与c库函数相同名字的函数时，
 * 优先采用用户编写的函数，这样就实现了对库函数的修改，即重定向
 
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
		 /* 串口UART00和UART01的中断号还待确定
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



