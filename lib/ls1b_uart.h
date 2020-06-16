// 串口相关头文件


#ifndef __OPENLOONGSON_UART_H
#define __OPENLOONGSON_UART_H


// 串口各寄存器相对基地址的偏移
#define BLS1_UART_DAT_OFFSET            (0)
#define BLS1_UART_IER_OFFSET            (1)
#define BLS1_UART_IIR_OFFSET            (2)
#define BLS1_UART_FCR_OFFSET            (2)
#define BLS1_UART_LCR_OFFSET            (3)
#define BLS1_UART_MCR_OFFSET            (4)
#define BLS1_UART_LSR_OFFSET            (5)
#define BLS1_UART_MSR_OFFSET            (6)

#define BLS1_UART_LSB_OFFSET            (0)     // 分频锁存器1
#define BLS1_UART_MSB_OFFSET            (1)     // 分频锁存器2


// 串口线路状态寄存器的位域
#define BLS1_UART_LSR_TE                (1 << 6)
#define BLS1_UART_LSR_TFE               (1 << 5)


// 打印缓存的大小
#define BLS1_UART_PRINT_BUF_SIZE        (256)



// 串口模块编号
typedef enum
{
    BLS1_UART0 = 0,
    BLS1_UART1,
    BLS1_UART2,
    BLS1_UART3,
    BLS1_UART4,
    BLS1_UART5,
    BLS1_UART6,
    BLS1_UART7,
    BLS1_UART8,
    BLS1_UART9,
    BLS1_UART10,
    BLS1_UART11
}ls1b_uart_t;


// 串口信息
typedef struct
{
    ls1b_uart_t 	UARTx;          //串口模块编号
    unsigned int 	baudrate;       //波特率
    unsigned char   parity;			//校验位
	unsigned char   databit;		//数据位	
	unsigned char   stopbit;		//停止位 
}ls1b_uart_info_t;




/*
 * 初始化串口2
 */
void uart2_init(void);
void uart_init(ls1b_uart_info_t *uart_info_p);


/*
 * 在串口2上打印字符串
 * @str 待打印的字符串
 */
void uart2_print(const char *str);
ls1b_uart_t uart_irqn_to_uartx(int IRQn);
void uart_putc(ls1b_uart_t uartx, unsigned char ch);




#endif

