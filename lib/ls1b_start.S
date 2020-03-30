/*************************************************************************
 *
 * 用汇编实现的启动相关函数
 *
 *************************************************************************/

/*
 * 汇编文件的扩展名必须是大写的S，不能用小写的s。否则不会执行预处理，
 * 即把#include, #define等这些当作以'#'开头的注释，而忽略掉，最终导致编译错误
 */
#include "ls1b_regdef.h"
#include "ls1b_mipsregs.h"
#include "ls1b_asm.h"
#include "ls1b_stackframe.h"
#include "ls1b_cacheops.h"


    .section ".text", "ax"
    .set noreorder


/*
 * 禁止中断
 * void irq_disable_all(void)
*/
    .globl irq_disable_all
irq_disable_all:
    mfc0    t0, CP0_STATUS
    and     t0, 0xfffffffe
    mtc0    t0, CP0_STATUS
    jr      ra
    nop



/*
 * 使能中断
 * void irq_enable_all(void)
 */
    .globl irq_enable_all
irq_enable_all:
    mfc0    t0, CP0_STATUS
    or      t0, 0x7c01
    mtc0    t0, CP0_STATUS
    jr      ra
    nop

    

/*
* General   exception vector for all other CPUs.
*
* Be careful when changing this, it has to be at most 128 bytes
* to fit into space reserved for the exception handler.
* 
* 整个异常向量的入口处理函数
* void general_exception(void)
*/
    .globl general_exception
general_exception:
    mfc0    k1, CP0_CAUSE
    andi    k1, k1, 0x7c
    # 注意，这里exception_handlers不是函数，而是地址
    # 即exception_handlers+k1 = exception_handlers+ExcCode*4
    # exception_handlers[]中保存了各个异常处理函数的首地址，每个占4字节
    # 这里看上去很巧妙，实际上个人认为很不好理解，很容易混淆
    PTR_L   k0, exception_handlers(k1)
    jr  k0


    .globl handle_int
handle_int:
    SAVE_ALL

    jal     plat_irq_dispatch
    nop
    
    RESTORE_ALL_AND_RET




    .globl handle_reserved
handle_reserved:
    .set    mips3
    eret
    .set    mips0



    .set reorder



