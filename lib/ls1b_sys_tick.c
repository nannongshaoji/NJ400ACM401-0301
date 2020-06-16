// ϵͳ�δ�ʱ����ؽӿ�

#include "ls1b_mipsregs.h"
#include "../app/timer.h"
//#define CPU_HZ			                (240 * 1000000)
//#define CPU_HZ			                (235102040)
//#define CPU_HZ			                (235110000)
#define CPU_HZ			                (210000000)
#define TICK_PER_SECOND                 (2000)


static volatile unsigned long system_tick=0;


// �δ�ʱ����ʼ��Ҫע�Ȿ��û���������ļ�����IM7
void sys_tick_init(void)
{
	write_c0_compare(CPU_HZ/2/TICK_PER_SECOND);
	write_c0_count(0);
	unsigned int c0_status = 0;
	c0_status = read_c0_status();
	c0_status |= CAUSEF_IP7;
	write_c0_status(c0_status);

}


void sys_tick_increase(void)
{
    ++system_tick;
}


// �δ�ʱ���жϴ�����
void sys_tick_handler(void)
{
    unsigned int count;
    count = read_c0_compare();
    write_c0_compare(count);
    write_c0_count(0);	
    IRQ_Time_1();


   // sys_tick_increase();

    return ;
}


// ��ȡtickֵ
unsigned long sys_tick_get(void)
{
    return system_tick;
}




