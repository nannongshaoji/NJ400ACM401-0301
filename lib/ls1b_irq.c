/*************************************************************************
 *
 * �ж���صĺ���
 * ��о1b���ж���һ�����͵��쳣��
 * �������Ϊcpu���м����쳣�����ж�������һ���쳣�����һ��������
 * ��о1b���쳣��Ϊ�ļ�
 * ��һ��: ����������쳣�����������
 * �ڶ���: �����쳣����ڣ�����ExcCode=0���쳣Ϊ�����жϵ������
 * ������: �����жϵ�ÿ��������(��о1b�����������Ϊ����)
 * ���ļ�: ÿ�������жϵ����
 *
 *************************************************************************/

#include <stdio.h>
#include <string.h>
#include "ls1b_public.h"
#include "ls1b_mipsregs.h"
#include "ls1b_clock.h"
#include "ls1b_gpio.h"
#include "ls1b_cache.h"
#include "ls1b_sys_tick.h"
#include "ls1b_irq.h"
#include "ls1b_regs.h"


// ÿ���쳣��0x80�ֽڵĿռ�
#define EXCEPTION_HANDER_MAX_SIZE               (0x80)

// ��������ʱ���쳣����ڻ���ַ
// ��������ʱ��STATUS�Ĵ�����BEV=0��cpu����RAM�ռ���쳣���
// 0x80000000��ַ������TLBӳ�䣬������
#define EXCEPTION_RAM_EBASE                     (0x80000000)

// �쳣��������
#define EXCEPTION_MAX_NUM                       (32)
// �жϵ�������
#define IRQ_MAX_NUM                             (LS1B_NR_IRQS)

// �ж����üĴ���
#define LS1B_INTREG_BASE 			(0xbfd01040)


struct ls1b_intc_regs
{
	volatile unsigned int int_isr;
	volatile unsigned int int_en;
	volatile unsigned int int_set;
	volatile unsigned int int_clr;		/* offset 0x10*/
	volatile unsigned int int_pol;
   	volatile unsigned int int_edge;		/* offset 0 */
}; 
struct ls1b_intc_regs volatile *ls1b_hw0_icregs 
    = (struct ls1b_intc_regs volatile *)LS1B_INTREG_BASE;


// �쳣������
unsigned long exception_handlers[EXCEPTION_MAX_NUM];

// �жϴ�����
irq_desc_t irq_handlers[IRQ_MAX_NUM];



// ���ʵ�ֵĺ���
extern void irq_disable_all(void);
extern void irq_enable_all(void);
extern void general_exception(void);
extern void handle_int(void);
extern void handle_reserved(void);


// ���������쳣�����Ĵ�����
// @offset �쳣��������ڵ�ƫ��
// @src_addr �쳣��������ڴ��������׵�ַ
void irq_set_exception_vector_handler(unsigned long offset, void *src_addr, unsigned long size)
{
    unsigned long dst_addr;   // �쳣���

    dst_addr = EXCEPTION_RAM_EBASE+offset;
    memcpy((void *)dst_addr, src_addr, size);

    // �Ȼ�дdcache��������icache
    // memcpy֮�������쳣��������ڵ�ָ��λ��dcache�У���Ҫ��д���ڴ棬
    // ��������Ӧicache�����Ϻ����жϷ���ʱ���Ż���ڴ����¼����´��뵽icache�������´������Ч��
    dcache_writeback_invalidate_range(dst_addr, dst_addr + size);
    icache_invalidate_range(dst_addr, dst_addr + size);

    return ;
}



// ����һ���쳣�Ĵ�����
// @n Э������0��cause�Ĵ�����[2,6]λ����ExcCode
// @addr �쳣���������׵�ַ
void irq_set_one_exception_handler(int n, void *addr)
{
    unsigned long handler = (unsigned long)addr;
    exception_handlers[n] = handler;

    return ;
}


/*
 * Ĭ�ϵ��жϴ�����
 * @IRQn �жϺ�
 * @param ����
 */
void irq_default_handler(int IRQn, void *param)
{
    myprintf("unhandled irq %d occured!\r\n", IRQn);
    return ;
}


/*
 * ʹ��ָ���ж�
 * @IRQn �жϺ�
 */
void irq_enable(int IRQn)
{
    (ls1b_hw0_icregs + (IRQn >> 5))->int_en |= (1 << (IRQn & 0x1f));
    return ;
}


/*
 * ��ָֹ���ж�
 * @IRQn �жϺ�
 */
void irq_disable(int IRQn)
{
    (ls1b_hw0_icregs + (IRQn >> 5))->int_en &= ~(1 << (IRQn & 0x1f));
    return ;
}


/*
 * �����жϴ�����
 * @IRQn �жϺ�
 * @new_handler �����õ��жϴ�����
 * @param ���ݸ��жϴ������Ĳ���
 * @ret ֮ǰ���жϴ�����
 */
irq_handler_t irq_install(int IRQn, irq_handler_t new_handler, void *param)
{
    irq_handler_t old_handler = NULL;

    if((0 <= IRQn) && (IRQ_MAX_NUM > IRQn))
    {
        old_handler = irq_handlers[IRQn].handler;
        
        irq_handlers[IRQn].handler  = new_handler;
        irq_handlers[IRQn].param    = param;
    }

    return old_handler;
}


// ��ʼ���ж�(Ĭ��Ϊ��ƽ������ʽ)
void irq_init(void)
{
    volatile struct ls1b_intc_regs *intc_regs = NULL;
    int i;
    int IRQn;

    // ��ֹ�ж�:������о1B������ж����üĴ���
    for (i=0; i<4; i++)     // ��о1B���жϷ�Ϊ4��
    {
        intc_regs = ls1b_hw0_icregs+i;
        intc_regs->int_en   = 0x0;          // disable
        intc_regs->int_pol  = -1;           // must be done here
        intc_regs->int_edge = 0x00000000;   // ��ƽ����
        intc_regs->int_clr  = 0xffffffff;   // ���ж�
    }

    // ����Ĭ�ϵ��жϴ�����
    for (IRQn = 0; IRQn < IRQ_MAX_NUM; IRQn++)
    {
        irq_handlers[IRQn].handler  = irq_default_handler;
        irq_handlers[IRQn].param    = 0;
    }

    return ;
}


// ��ʼ���ж�
void exception_init(void)
{
	int i;

	// ��ֹ�ж�
	irq_disable_all();

	// ��ʼ�����ٻ���
	cache_init();
   //myprintf("1");
	// ��ʼ���ж�
	irq_init();

   //myprintf("2");
	// ���������쳣�����Ĵ�����
	irq_set_exception_vector_handler(0x180, &general_exception, EXCEPTION_HANDER_MAX_SIZE);
	irq_set_exception_vector_handler(0x200, &general_exception, EXCEPTION_HANDER_MAX_SIZE);

	// ���ø����쳣�Ĵ�����
	for (i=0; i<EXCEPTION_MAX_NUM; i++)
	{
	    irq_set_one_exception_handler(i, handle_reserved);
	}
	irq_set_one_exception_handler(0, handle_int);

  // myprintf("3");
	// �Ȼ�д����dcache������������icache
	dcache_writeback_invalidate_all();
	icache_invalidate_all();

   //myprintf("4");
	// ʹ���ж�
	irq_enable_all();
	//myprintf("cpu_status=0x%x\n",read_c0_status());
   //myprintf("5");
	return ;
}


/*
 * ִ���жϴ�������
 * @IRQn �жϺ�
 */
void ls1b_do_IRQ(int IRQn)
{
    irq_handler_t irq_handler = NULL;
    void *param = NULL;

    irq_handler = irq_handlers[IRQn].handler;
    param       = irq_handlers[IRQn].param;

    // ִ���жϴ�����
    irq_handler(IRQn, param);

    return ;
}


void ls1b_irq_dispatch(int n)
{
    unsigned int intstatus, irq;

    /* Receive interrupt signal, compute the irq */
    intstatus = (ls1b_hw0_icregs+n)->int_isr & (ls1b_hw0_icregs+n)->int_en;
    if (0 == intstatus)
        return ;

    // ִ���жϴ�����
    irq = ffs(intstatus) - 1;
    ls1b_do_IRQ((n<<5) + irq);

    /* ack interrupt */
    (ls1b_hw0_icregs+n)->int_clr |= (1 << irq);
    
    return ;
}


// �жϷַ�����
void plat_irq_dispatch(void)
{

    unsigned int pending;

    pending = read_c0_cause() & read_c0_status() & ST0_IM;

    if (pending & CAUSEF_IP7)
    {
	sys_tick_handler();
    }
    else if (pending & CAUSEF_IP2)
    {
    	ls1b_irq_dispatch(0);
    }
    else if (pending & CAUSEF_IP3)
    {
    	ls1b_irq_dispatch(1);
    }
    else if (pending & CAUSEF_IP4)
    {
    	ls1b_irq_dispatch(2);
    }
    else if (pending & CAUSEF_IP5)
    {

    	ls1b_irq_dispatch(3);
    }
    else
    {
        // �������������
    }


    return ;
}



