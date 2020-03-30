/****************************************Copyright (c)**************************************************
**                               南大傲拓科技江苏股份有限公司
**                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/
#ifndef _COM_CPU_H_
#define _COM_CPU_H_
#include "config.h"

#define HCM_CH_NUM              8  //HCM通道数
#define MAX_NUM_HZ              220

#define HCM_MODE_NO_USE         0 // 无
#define HCM_MODE_0              1 // 0-测频模式
#define HCM_MODE_1              2 // 1-事件计数模式
#define HCM_MODE_2              3 // 2-归零计数模式
#define HCM_MODE_3              4 // 3-环形计数模式
#define HCM_MODE_4              5 // 4-单相加减计数模式
#define HCM_MODE_5              6 // 5-双相加减计数模式
#define HCM_MODE_6              7 // 6-双相方向计数模式
#define HCM_MODE_7              8 // 7-双相AB计数模式

#define HCM_FILTER_LEVEL_NO_USE 0 //滤波等级无
#define HCM_FILTER_LEVEL_LOW    1 //滤波等级低
#define HCM_FILTER_LEVEL_MIDDLE 2 //滤波等级中
#define HCM_FILTER_LEVEL_HIGH   3 //滤波等级高

#define AOSET_CODE_RESET        0X01
#define AOSET_CODE_START        0X02
#define AOSET_CODE_STOP         0X03
#define AOSET_CODE_TIME_CYCLE   0X04 //设置检测周期/模式3阈值
#define AOSET_CODE_BIJIAO       0X05 //设置比较值
#define AOSET_CODE_ZHIHOU       0X06 //设置滞后值
//#define AOSET_CODE_MODE3_YUZHI  0X06 //设置模式3阈值

#define STATE_RESET             0X01
#define STATE_START             0X02
#define STATE_STOP              0X04
#define STATE_SET_S_VALUE       0X08
//#define STATE_SET_ZHIHOU 0X10
//#define STATE_SET_MODE3_YUZHI 0X20

typedef union
{
    uint16 Word;
    struct
    {															
        uint16 mode :4; //计数模式
        uint16 filter0 :2; //I_AUX引脚滤波等级
        uint16 filter1 :2; //SIN/AIN引脚滤波等级
        uint16 filter2 :2; //BIN引脚滤波等级
        uint16 x1_x4_bit :1; //X1/X4倍率位
        uint16 rc :3; //预留
        uint16 int_en:1; //中断(事件)使能位
        uint16 cp_en:1; //比较使能位
    }Bits;
}s_config0;

extern s_config0 config0[HCM_CH_NUM];

typedef union
{
	uint16 Word;
	struct
	{						
		uint16 start_stop_bit :1; //启停标志位 0-stop 1-start
		uint16 i_aux:1; //aux状态位
		uint16 counter_low :1; //计数值低
		uint16 counter_equ :1; //计数值等
		uint16 counter_high :1; //计数值高
		uint16 cap_low :1; //捕获值低
		uint16 cap_equ :1; //捕获值等
		uint16 cap_high:1; //捕获值高
		uint16 event_chan :1; //事件位?
		uint16 rc :7;		//保留
//			uint16 event_chan1 :1; //通道1事件位
//			uint16 event_chan2 :1; //通道2事件位
//			uint16 event_chan3 :1; //通道3事件位
//			uint16 event_chan4 :1; //通道4事件位
//			uint16 event_chan5 :1; //通道5事件位
//			uint16 event_chan6 :1; //通道6事件位
//			uint16 event_chan7 :1; //通道7事件位			
	}Bits;
}s_status0;
extern s_status0 status0[HCM_CH_NUM];

typedef struct
{
	uint32 hcm_s_value0; //输入参数值
	uint32 hcm_s_value1;//比较值，输入参数 用于获得比较状态
	uint16 hcm_s_value2;//校准系数(测频模式)/滞后值(双向模式)
	
	uint32 hcm_hst_value;//输出结果当前计数值
	uint32 hcm_hst_c_value;//输出结果捕获值
}s_hcm_data;

extern s_hcm_data hcm_data[HCM_CH_NUM];
extern uint32 measure_10hz_data[HCM_CH_NUM][MAX_NUM_HZ];
extern uint8 measure_10hz_index[HCM_CH_NUM];
extern uint8 aoset_state_byte[HCM_CH_NUM];//启动停止复位控制字节 bit0-复位 bit1-启动 bit2-停止 bit3设置输入参数值

extern void process_mode0_data(uint8 channo,uint8 step,uint8 indexnow);
extern void hcm_read_mode0_data(void);
extern void hcm_process(void);
#endif
/********************************************* End of File *********************************************/
