// 软件延时源文件


#include "ls1b_clock.h"
#include "ls1b_public.h"
#include "ls1b_delay.h"

/*
 * 延时指定时间，单位ms
 * @j 延时时间，单位ms
 */
void delay_ms(unsigned int n)
{ 
    for ( ; n > 0; n--)
    {
        delay_us(1000);
    }   
}


/*
 * 延时指定时间，单位us
 * @n 延时时间，单位us
 */
void delay_us(unsigned int n)
{
    unsigned int count_max,i;  
    if(n < 10)          count_max = 51 * n;//1us
    else if(n < 20)     count_max = 69 * n;//10us
    else if(n < 50)     count_max = 70 * n;//20us
    else                count_max = 71 * n;//50us\100us\200us\500us
    // 延时
    for (i = 0; i < count_max; i++)
    {
        __asm__ ("nop");        // 注意，这里必须用内联汇编，否则会被优化掉           
    }
}


/*
 * 延时指定时间，单位s
 * @i 延时时间，单位s
 */
void delay_s(unsigned int n)
{
    for ( ; n > 0; n--)
    {
        delay_ms(1000);
    }

}



