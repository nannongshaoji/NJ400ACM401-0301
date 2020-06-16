/*************************************************************************
 *
 * 中断相关的函数
 * 龙芯1b中中断是一种类型的异常，
 * 可以理解为cpu中有几种异常，而中断是其中一种异常下面的一个子类型
 * 龙芯1b的异常分为四级
 * 第一级: 各种情况下异常向量的总入口
 * 第二级: 各个异常的入口，其中ExcCode=0的异常为外设中断的总入口
 * 第三级: 外设中断的每组的总入口(龙芯1b将所有外设分为五组)
 * 第四级: 每个外设中断的入口
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


// 每个异常有0x80字节的空间
#define EXCEPTION_HANDER_MAX_SIZE               (0x80)

// 正常运行时，异常的入口基地址
// 正常运行时，STATUS寄存器的BEV=0，cpu采用RAM空间的异常入口
// 0x80000000地址处不经TLB映射，但缓存
#define EXCEPTION_RAM_EBASE                     (0x80000000)

// 异常的最大个数
#define EXCEPTION_MAX_NUM                       (32)
// 中断的最大个数
#define IRQ_MAX_NUM                             (LS1B_NR_IRQS)

// 中断配置寄存器
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


// 异常处理函数
unsigned long exception_handlers[EXCEPTION_MAX_NUM];

// 中断处理函数
irq_desc_t irq_handlers[IRQ_MAX_NUM];



// 汇编实现的函数
extern void irq_disable_all(void);
extern void irq_enable_all(void);
extern void general_exception(void);
extern void handle_int(void);
extern void handle_reserved(void);


// 设置整个异常向量的处理函数
// @offset 异常向量总入口的偏移
// @src_addr 异常向量总入口处理函数的首地址
void irq_set_exception_vector_handler(unsigned long offset, void *src_addr, unsigned long size)
{
    unsigned long dst_addr;   // 异常入口

    dst_addr = EXCEPTION_RAM_EBASE+offset;
    memcpy((void *)dst_addr, src_addr, size);

    // 先回写dcache，再作废icache
    // memcpy之后，现在异常向量总入口的指令位于dcache中，需要回写到内存，
    // 并作废相应icache，作废后当有中断发生时，才会从内存重新加载新代码到icache，这样新代码就生效了
    dcache_writeback_invalidate_range(dst_addr, dst_addr + size);
    icache_invalidate_range(dst_addr, dst_addr + size);

    return ;
}



// 设置一个异常的处理函数
// @n 协处理器0的cause寄存器的[2,6]位，即ExcCode
// @addr 异常处理函数的首地址
void irq_set_one_exception_handler(int n, void *addr)
{
    unsigned long handler = (unsigned long)addr;
    exception_handlers[n] = handler;

    return ;
}


/*
 * 默认的中断处理函数
 * @IRQn 中断号
 * @param 参数
 */
void irq_default_handler(int IRQn, void *param)
{
    myprintf("unhandled irq %d occured!\r\n", IRQn);
    return ;
}


/*
 * 使能指定中断
 * @IRQn 中断号
 */
void irq_enable(int IRQn)
{
    (ls1b_hw0_icregs + (IRQn >> 5))->int_en |= (1 << (IRQn & 0x1f));
    return ;
}


/*
 * 禁止指定中断
 * @IRQn 中断号
 */
void irq_disable(int IRQn)
{
    (ls1b_hw0_icregs + (IRQn >> 5))->int_en &= ~(1 << (IRQn & 0x1f));
    return ;
}


/*
 * 设置中断处理函数
 * @IRQn 中断号
 * @new_handler 新设置的中断处理函数
 * @param 传递给中断处理函数的参数
 * @ret 之前的中断处理函数
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


// 初始化中断(默认为电平触发方式)
void irq_init(void)
{
    volatile struct ls1b_intc_regs *intc_regs = NULL;
    int i;
    int IRQn;

    // 禁止中断:设置龙芯1B里面的中断配置寄存器
    for (i=0; i<4; i++)     // 龙芯1B的中断分为4组
    {
        intc_regs = ls1b_hw0_icregs+i;
        intc_regs->int_en   = 0x0;          // disable
        intc_regs->int_pol  = -1;           // must be done here
        intc_regs->int_edge = 0x00000000;   // 电平触发
        intc_regs->int_clr  = 0xffffffff;   // 清中断
    }

    // 设置默认的中断处理函数
    for (IRQn = 0; IRQn < IRQ_MAX_NUM; IRQn++)
    {
        irq_handlers[IRQn].handler  = irq_default_handler;
        irq_handlers[IRQn].param    = 0;
    }

    return ;
}


// 初始化中断
void exception_init(void)
{
	int i;

	// 禁止中断
	irq_disable_all();

	// 初始化高速缓存
	cache_init();
   //myprintf("1");
	// 初始化中断
	irq_init();

   //myprintf("2");
	// 设置整个异常向量的处理函数
	irq_set_exception_vector_handler(0x180, &general_exception, EXCEPTION_HANDER_MAX_SIZE);
	irq_set_exception_vector_handler(0x200, &general_exception, EXCEPTION_HANDER_MAX_SIZE);

	// 设置各个异常的处理函数
	for (i=0; i<EXCEPTION_MAX_NUM; i++)
	{
	    irq_set_one_exception_handler(i, handle_reserved);
	}
	irq_set_one_exception_handler(0, handle_int);

  // myprintf("3");
	// 先回写整个dcache，再作废整个icache
	dcache_writeback_invalidate_all();
	icache_invalidate_all();

   //myprintf("4");
	// 使能中断
	irq_enable_all();
	//myprintf("cpu_status=0x%x\n",read_c0_status());
   //myprintf("5");
	return ;
}


/*
 * 执行中断处理流程
 * @IRQn 中断号
 */
void ls1b_do_IRQ(int IRQn)
{
    irq_handler_t irq_handler = NULL;
    void *param = NULL;

    irq_handler = irq_handlers[IRQn].handler;
    param       = irq_handlers[IRQn].param;

    // 执行中断处理函数
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

    // 执行中断处理函数
    irq = ffs(intstatus) - 1;
    ls1b_do_IRQ((n<<5) + irq);

    /* ack interrupt */
    (ls1b_hw0_icregs+n)->int_clr |= (1 << irq);
    
    return ;
}


// 中断分发函数
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
        // 其它情况不处理
    }


    return ;
}



