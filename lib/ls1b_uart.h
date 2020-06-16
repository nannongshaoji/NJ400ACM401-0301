// �������ͷ�ļ�


#ifndef __OPENLOONGSON_UART_H
#define __OPENLOONGSON_UART_H


// ���ڸ��Ĵ�����Ի���ַ��ƫ��
#define BLS1_UART_DAT_OFFSET            (0)
#define BLS1_UART_IER_OFFSET            (1)
#define BLS1_UART_IIR_OFFSET            (2)
#define BLS1_UART_FCR_OFFSET            (2)
#define BLS1_UART_LCR_OFFSET            (3)
#define BLS1_UART_MCR_OFFSET            (4)
#define BLS1_UART_LSR_OFFSET            (5)
#define BLS1_UART_MSR_OFFSET            (6)

#define BLS1_UART_LSB_OFFSET            (0)     // ��Ƶ������1
#define BLS1_UART_MSB_OFFSET            (1)     // ��Ƶ������2


// ������·״̬�Ĵ�����λ��
#define BLS1_UART_LSR_TE                (1 << 6)
#define BLS1_UART_LSR_TFE               (1 << 5)


// ��ӡ����Ĵ�С
#define BLS1_UART_PRINT_BUF_SIZE        (256)



// ����ģ����
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


// ������Ϣ
typedef struct
{
    ls1b_uart_t 	UARTx;          //����ģ����
    unsigned int 	baudrate;       //������
    unsigned char   parity;			//У��λ
	unsigned char   databit;		//����λ	
	unsigned char   stopbit;		//ֹͣλ 
}ls1b_uart_info_t;




/*
 * ��ʼ������2
 */
void uart2_init(void);
void uart_init(ls1b_uart_info_t *uart_info_p);


/*
 * �ڴ���2�ϴ�ӡ�ַ���
 * @str ����ӡ���ַ���
 */
void uart2_print(const char *str);
ls1b_uart_t uart_irqn_to_uartx(int IRQn);
void uart_putc(ls1b_uart_t uartx, unsigned char ch);




#endif

