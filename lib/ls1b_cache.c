/*************************************************************************
 *
 * ���ٻ������Դ�ļ��ļ�
 *
 *************************************************************************/


#include <strings.h>
#include "ls1b_mipsregs.h"
#include "ls1b_cacheops.h"
#include "ls1b_cache.h"


#define INDEX_BASE			        0x80000000


// icache��dcache����Ϣ
struct cache_desc icache_desc;
struct cache_desc dcache_desc;


#define cache_op(op,addr)						\
	__asm__ __volatile__(						\
	"	.set	push					\n"	\
	"	.set	noreorder				\n"	\
	"	.set	mips3\n\t				\n"	\
	"	cache	%0, %1					\n"	\
	"	.set	pop					\n"	\
	:								\
	: "i" (op), "R" (*(unsigned char *)(addr)))


#define cache32_unroll32(base,op)					\
	__asm__ __volatile__(						\
	"	.set push					\n"	\
	"	.set noreorder					\n"	\
	"	.set mips3					\n"	\
	"	cache %1, 0x000(%0); cache %1, 0x020(%0)	\n"	\
	"	cache %1, 0x040(%0); cache %1, 0x060(%0)	\n"	\
	"	cache %1, 0x080(%0); cache %1, 0x0a0(%0)	\n"	\
	"	cache %1, 0x0c0(%0); cache %1, 0x0e0(%0)	\n"	\
	"	cache %1, 0x100(%0); cache %1, 0x120(%0)	\n"	\
	"	cache %1, 0x140(%0); cache %1, 0x160(%0)	\n"	\
	"	cache %1, 0x180(%0); cache %1, 0x1a0(%0)	\n"	\
	"	cache %1, 0x1c0(%0); cache %1, 0x1e0(%0)	\n"	\
	"	cache %1, 0x200(%0); cache %1, 0x220(%0)	\n"	\
	"	cache %1, 0x240(%0); cache %1, 0x260(%0)	\n"	\
	"	cache %1, 0x280(%0); cache %1, 0x2a0(%0)	\n"	\
	"	cache %1, 0x2c0(%0); cache %1, 0x2e0(%0)	\n"	\
	"	cache %1, 0x300(%0); cache %1, 0x320(%0)	\n"	\
	"	cache %1, 0x340(%0); cache %1, 0x360(%0)	\n"	\
	"	cache %1, 0x380(%0); cache %1, 0x3a0(%0)	\n"	\
	"	cache %1, 0x3c0(%0); cache %1, 0x3e0(%0)	\n"	\
	"	.set pop					\n"	\
		:							\
		: "r" (base),						\
		  "i" (op));



extern void disable_cpu_cache(void);
extern void enable_cpu_cache(void);



// ̽��cache���ͺʹ�С
void cache_probe(void)
{
    unsigned long config1 = 0;      // Э������0�ļĴ���config1
    unsigned int lsize = 0;         // �д�С

    // ��ȡ�Ĵ���config1
    config1 = read_c0_config1();

    // icache�����Ϣ
    lsize = (config1 >> 19) & 7;    // �Ĵ���config1��IL
    if (0 == lsize)                 // �ж��Ƿ��и��ٻ���
    {
        // û�и��ٻ��棬�򷵻�
        icache_desc.linesz = 0;
        icache_desc.cachesize = 0;
        return ;
    }
    icache_desc.linesz  = 2 << lsize;                       // �д�С=2*2^L�ֽ�
    icache_desc.sets    = 64 << ((config1 >> 22) & 7);      // ��ֱ������λ����=64*2^S
    icache_desc.ways    = 1 + ((config1 >> 16) & 7);        // ������·��=1+A
    icache_desc.cachesize = icache_desc.sets * icache_desc.ways * icache_desc.linesz;  // cache��С
    icache_desc.waysize = icache_desc.cachesize / icache_desc.ways;
    icache_desc.waybit  = ffs(icache_desc.waysize);

    // dcache�����Ϣ
    lsize = (config1 >> 10) & 7;    // �Ĵ���config1��DL
    if (0 == lsize)                 // �ж��Ƿ��и��ٻ���
    {
        // û�и��ٻ��棬�򷵻�
        dcache_desc.linesz = 0;
        dcache_desc.cachesize = 0;
        return ;
    }
    dcache_desc.linesz  = 2 << lsize;                       // �д�С=2*2^L�ֽ�
    dcache_desc.sets    = 64 << ((config1 >> 13) & 7);      // ��ֱ������λ����=64*2^S
    dcache_desc.ways    = 1 + ((config1 >> 7) & 7);         // ������·��=1+A
    dcache_desc.cachesize = dcache_desc.sets * dcache_desc.ways * dcache_desc.linesz;  // cache��С
    dcache_desc.waysize = dcache_desc.cachesize / dcache_desc.ways;
    dcache_desc.waybit  = ffs(dcache_desc.waysize);

    return ;
}


// ��ʼ�����ٻ���
void cache_init(void)
{
    cache_probe();

    return ;
}



// ��дdcache��ĳ��ָ������
// @start ����д����ʼ��ַ
// @end ����д�Ľ�����ַ
void dcache_writeback_invalidate_range(unsigned long start, unsigned long end)
{
    unsigned long dcache_size = 0;          // dcache��С
    unsigned long line_size = 0;            // dcache�д�С
    unsigned long addr = 0;
    unsigned long aend = 0;

    // �жϴ�С�Ƿ񳬹�dcache��С
    dcache_size = dcache_desc.cachesize;    // ��ȡdcache��С
    if (end - start > dcache_size)
    {
        // ��д����dcache
        dcache_writeback_invalidate_all();
        return ;
    }

    // ��дָ������
    line_size = dcache_desc.linesz;         // ��ȡdcache�д�С
    addr = start & ~(line_size - 1);        // һ�λ�дһ�У����Ե�λ�������м���
    aend = (end - 1) & ~(line_size - 1);
    while (1)
    {
        cache_op(Hit_Writeback_Inv_D, addr);
        if (addr == aend)
            break;
        addr += line_size;
    }

    return ;
}


// ����icache��ĳ��ָ������
// @start �����ϵ���ʼ��ַ
// @end �����ϵĽ�����ַ
void icache_invalidate_range(unsigned long start, unsigned long end)
{
    unsigned long icache_size = 0;          // icache��С
    unsigned long line_size = 0;            // icache�д�С
    unsigned long addr = 0;
    unsigned long aend = 0;

    // �жϴ�С�Ƿ񳬹�icache��С
    icache_size = icache_desc.cachesize;    // ��ȡicache��С
    if (end - start > icache_size)
    {
        // ��������icache
        icache_invalidate_all();
        return ;
    }

    // ����ָ������
    line_size = icache_desc.linesz;         // ��ȡicache�д�С
    addr = start & ~(line_size - 1);        // һ������һ�У����Ե�λ�������м���
    aend = (end - 1) & ~(line_size - 1);
    while (1)
    {
        cache_op(Hit_Invalidate_I, addr);
        if (addr == aend)
            break;
        addr += line_size;
    }

    return ;
}


// ��д����dcache
void dcache_writeback_invalidate_all(void)
{
    unsigned long start = INDEX_BASE;
    unsigned long end   = start + dcache_desc.waysize;
    unsigned long ws_inc= 1UL << dcache_desc.waybit;
    unsigned long ws_end= dcache_desc.ways << dcache_desc.waybit;
    unsigned long ws, addr;
    unsigned char lsize = dcache_desc.linesz;

    for (ws = 0; ws < ws_end; ws += ws_inc)
        for (addr = start; addr < end; addr += lsize * 32)
            cache32_unroll32(addr | ws, Index_Writeback_Inv_D);
    
    return ;
}


// ��������icache
void icache_invalidate_all(void)
{
    unsigned long start = INDEX_BASE;
    unsigned long end   = start + icache_desc.waysize;
    unsigned long ws_inc= 1UL << icache_desc.waybit;
    unsigned long ws_end= icache_desc.ways << icache_desc.waybit;
    unsigned long ws, addr;
    unsigned char lsize = icache_desc.linesz;

    for (ws = 0; ws < ws_end; ws += ws_inc)
        for (addr = start; addr < end; addr += lsize * 32)
            cache32_unroll32(addr | ws, Index_Invalidate_I);
    
    return ;
}





