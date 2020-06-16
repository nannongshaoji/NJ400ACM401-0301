// 系统滴答定时器相关接口


#ifndef __OPENLOONGSON_SYS_TICK_H
#define __OPENLOONGSON_SYS_TICK_H



// 滴答定时器初始化
void sys_tick_init(void);

// 时钟中断处理函数
void sys_tick_handler(void);

// 获取tick值
unsigned long sys_tick_get(void);



#endif

