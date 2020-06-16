// �����ʱԴ�ļ�


#include "ls1b_clock.h"
#include "ls1b_public.h"
#include "ls1b_delay.h"

/*
 * ��ʱָ��ʱ�䣬��λms
 * @j ��ʱʱ�䣬��λms
 */
void delay_ms(unsigned int n)
{ 
    for ( ; n > 0; n--)
    {
        delay_us(1000);
    }   
}


/*
 * ��ʱָ��ʱ�䣬��λus
 * @n ��ʱʱ�䣬��λus
 */
void delay_us(unsigned int n)
{
    unsigned int count_max,i;  
    if(n < 10)          count_max = 51 * n;//1us
    else if(n < 20)     count_max = 69 * n;//10us
    else if(n < 50)     count_max = 70 * n;//20us
    else                count_max = 71 * n;//50us\100us\200us\500us
    // ��ʱ
    for (i = 0; i < count_max; i++)
    {
        __asm__ ("nop");        // ע�⣬���������������࣬����ᱻ�Ż���           
    }
}


/*
 * ��ʱָ��ʱ�䣬��λs
 * @i ��ʱʱ�䣬��λs
 */
void delay_s(unsigned int n)
{
    for ( ; n > 0; n--)
    {
        delay_ms(1000);
    }

}



