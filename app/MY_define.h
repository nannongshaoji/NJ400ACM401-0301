#ifndef _Define_H_
#define _Define_H_

#include<string.h>
#include"config.h"

extern unsigned short CalcCrcFast(unsigned char*puchMsg,unsigned short usDataLen);
	
#define USART1_RXD_DMA_CHAN  DMA1_Channel3
#define USART1_RXD_BUF_SIZE 300
#define ConfigSize  100
#define DataSize  700
#define FlashConfigSize  700
#define POWER_NWDATA_Addr  0x10
#define Main_Procedure_Buf_Addr 0x30
#define PACKAGEHEAD 0xAABB   //0xAA55
#define WORD unsigned short
#define DWORD unsigned int

typedef struct {
	unsigned int   AVoltage_Check;		//A相电压校准参数
	unsigned int   BVoltage_Check;		//B相电压校准参数
	unsigned int   CVoltage_Check;		//C相电压校准参数
	unsigned int   ACurrent_Check;		//A相电流校准参数
	unsigned int   BCurrent_Check;		//B相电流校准参数
	unsigned int   CCurrent_Check;		//C相电流校准参数
	unsigned int   APower_Check;		//A相功率校准参数
	unsigned int   BPower_Check;		//B相功率校准参数
	unsigned int   CPower_Check;		//C相功率校准参数	
	///////////////45018///////////////////////////////////////
	unsigned short	RunMode;				//运行模式
	unsigned short	RS485Addr;			//RS485从站地址
	unsigned short	RS485Baudrate;	//RS485波特率
	unsigned short	CurrentRatio;		//电流互感器变比
	unsigned short	VoltRatio;			//电压互感器变比
	unsigned short	FilterLevel;		//滤波等级
	unsigned short  CGQGroupNo;			//传感器组号
	unsigned short  CGQIndexNo;			//传感器编号
	unsigned short	DLSleepTime;	  //电参上报间隔	秒	
	unsigned short	NC1[9];				//保留
	unsigned short	RS485_data;			//RS485接口测试数据
	unsigned short	Zigbee_data;		//zigbee接口测试数据
	unsigned short	DLCheckMode;	  //进入测试模式
	///////////////45039///////////////////////////////////////
	unsigned short  Zigbee_EN;			//Zigbee使能
	unsigned int  Zigbee_ID;				//Zigbee网络号
	unsigned short  Zigbee_SC;			//Zigbee信道号
	unsigned short	Zigbee_EE;			//Zigbee加密使能
	unsigned int	Zigbee_KY;			//连接密钥 10进制
	unsigned int	Zigbee_NY;			//网络密钥 10进制
	unsigned short	Zigbee_AO;      //AO设置
	unsigned short	Zigbee_ZS;      //ZS设置
	unsigned short Buf[50]; //保留
}__attribute__ ((packed)) _RTU_DL_Config;

typedef struct {
	unsigned short Head ; //固定0xAA55
	unsigned short IsOK ;  //程序完整标志  0-不完整，1-完整
	unsigned int FileLen;  //文件长度
	unsigned short PageNum;  //FLASH存放页数
	unsigned short NC0;
	_RTU_DL_Config Config;
	unsigned short Buf[50]; //保留
}__attribute__ ((packed)) _RTU_Config;

typedef struct {
	unsigned short  Version;			//版本
	unsigned short	ManufactureCode;	//厂家代码
	unsigned short	RunMode;				//运行模式
	unsigned short	RS485Addr;			//RS485从站地址
	unsigned short	RS485Baudrate;	//RS485波特率 0-9600，1-19200，2-38400
	unsigned short	CurrentRatio;		//电流互感器变比
	unsigned short	VoltRatio;			//电压互感器变比
	unsigned short	FilterLevel;		//滤波等级 0-无，1-1级 ~4
	unsigned short  CGQGroupNo;			//传感器组号
	unsigned short  CGQIndexNo;			//传感器编号
	unsigned short	DLSleepTime;	  //电参上报间隔	秒		
	unsigned short	NC1[9];					//保留
	unsigned short	RS485_data;			//RS485接口测试数据
	unsigned short	Zigbee_data;		//zigbee接口测试数据
	unsigned short	DLCheckMode;	  //进入测试模式
	unsigned short  Zigbee_EN;			//Zigbee使能
	unsigned int  Zigbee_ID;			//Zigbee网络号
	unsigned short  Zigbee_SC;			//Zigbee信道号
	unsigned short	Zigbee_EE;			//Zigbee加密使能
	unsigned int	Zigbee_KY;			//连接密钥 10进制
	unsigned int	Zigbee_NY;			//网络密钥 10进制
	unsigned short	Zigbee_AO;      //AO设置
	unsigned short	Zigbee_ZS;      //ZS设置
	unsigned short	NC2[66];			//保留
}__attribute__ ((packed)) _DL_Config_1;

typedef union 	{      	
				_DL_Config_1  Config;
			 	unsigned short	buf[sizeof(_DL_Config_1)/2];
}_DL_Config;

typedef struct {
	///////////////40000///////////////////////////////////////
	WORD	RTU_Year_Month;			//只读，当前时间，年月，高字节为年
	WORD	RTU_Day_Hour;				//只读，当前时间，日时，高字节为日
	WORD	RTU_Minute_Second;	//只读，当前时间，分秒，高字节为分
	WORD	GT_ID;							//功图ID号
	WORD	GT_Point;						//功图点数
	WORD	GT_State;						//功图状态
	WORD	GT_DeadPoint;				//功图下死点
	WORD	GT_Time;						//冲程周期(单位0.01秒)
	WORD	GT_CurPoint;				//当前采集点
	WORD	NC1[7];							//保留
	///////////////40016///////////////////////////////////////
	short	ACurrent_Valid;		//只读，A相电流有效值0.01A
	short	BCurrent_Valid;		//只读，B相电流有效值0.01A
	short	CCurrent_Valid;		//只读，C相电流有效值0.01A
	WORD	AVoltage_Valid;		//只读，A相电压有效值0.1V
	WORD	BVoltage_Valid;		//只读，B相电压有效值0.1V
	WORD	CVoltage_Valid;		//只读，C相电压有效值0.1V
	short	AActivePower_Valid;		//只读，A相有功功率有效值0.01A
	short	BActivePower_Valid;		//只读，B相有功功率有效值0.01A
	short	CActivePower_Valid;		//只读，C相有功功率有效值0.01A
	short	AReactivePower_Valid;		//只读，A相无功功率有效值0.1V
	short	BReactivePower_Valid;		//只读，B相无功功率有效值0.1V
	short	CReactivePower_Valid;		//只读，C相无功功率有效值0.1V
	short	ActivePower;			//只读，有功功率(0.01kW)
	short	ReactivePower;		//只读，无功功率(0.01var)
	short	ApparentPower;		//只读，视在功率(0.01VA)	
	WORD	PowerNetFreq;			//只读，电网频率0.01Hz
	short	PowerFactor;			//只读，功率因数（-1.00~1.00）
//	WORD	ABC_State;				//断网状态
	unsigned int	SumActivePower;		//只读，总有功电能
	unsigned int	SumReactivePower;	//只读，总无功电能
	INT16S AngleA   ; //A相角  X10倍
	INT16S AngleB   ; //B相角  X10倍
	INT16S AngleC   ; //C相角  X10倍
	INT16U AngleUAB   ; //UAB角  X10倍
	INT16U AngleUAC   ; //UAC角  X10倍
	INT16U AngleUBC   ; //UBC角  X10倍
	WORD	CurrentRMS;				//保留
	///////////////40039///////////////////////////////////////
	WORD	NC2[61];			//保留
	///////////////40100///////////////////////////////////////
	short	GT_DL_Value[300];	//位移点对应电流值250点 0.01A
	short	GT_GL_Value[300];	//位移点对应有功功率值250点 0.01KW
	WORD	NC3[50];			//保留
}__attribute__ ((packed)) _DL_Data_1;

typedef union 	{      	
				_DL_Data_1  Data;
			 	unsigned short	buf[sizeof(_DL_Data_1)/2];
}__attribute__ ((packed)) _DL_Data;

typedef struct {
	///////////////30000///////////////////////////////////////
	short	ACurrent_Valid;		//只读，A相电流有效值0.01A
	short	BCurrent_Valid;		//只读，B相电流有效值0.01A
	short	CCurrent_Valid;		//只读，C相电流有效值0.01A
	WORD	AVoltage_Valid;		//只读，A相电压有效值0.01V
	WORD	BVoltage_Valid;		//只读，B相电压有效值0.01V
	WORD	CVoltage_Valid;		//只读，C相电压有效值0.01V
	short	ActivePower;			//只读，有功功率(0.01kW)
	short	ReactivePower;		//只读，无功功率(0.01var)
	short	ApparentPower;		//只读，视在功率(0.01VA)
	WORD	PowerNetFreq;			//只读，电网频率0.01Hz
	short	PowerFactor;			//只读，功率因数（-1.00~1.00）
//	WORD	ABC_State;				//断网状态
	WORD	SumActivePower_high;		//只读，总有功电能
	WORD	SumActivePower_low;		//只读，总有功电能
	WORD	SumReactivePower_high;	//只读，总无功电能
	WORD	SumReactivePower_low;	//只读，总无功电能
	///////////////30016///////////////////////////////////////
	WORD	NC2[34];			//保留
	///////////////30100///////////////////////////////////////
	short	GT_DL_Value[300];	//位移点对应电流值250点 0.01A
	///////////////30900///////////////////////////////////////
	short	GT_GL_Value[300];	//位移点对应有功功率值250点 0.01KW
	WORD	NC3[10];			//保留
}_AK_DL_Data_1;

typedef union 	{      	
				_AK_DL_Data_1  Data;
			 	unsigned short	buf[sizeof(_AK_DL_Data_1)/2];
}_AK_DL_Data;

typedef struct 
{
	unsigned char	Year;
	unsigned char	Month;
	unsigned char	Day;	
	unsigned char	Hour;	
	unsigned char	Minute;
	unsigned char	Second;
}_CLOCK;

typedef struct 
{
	unsigned char mode;		//模式，0-实际曲线，0x10-仿真曲线
	unsigned char SynchroStartFlag; //同步倒计时开始
	unsigned char DLmesure_flag; 		//电流图采集过程中
	unsigned char DLStart_mesure_flag; //开始采集电流图数据标记
	unsigned char Dead_point;   //下死点
	unsigned char Send_flag;		//发送标识
	unsigned char ReSend_flag;	//重发标识
	unsigned char NC;
	unsigned short SynchroTime;  	//同步倒计时时间
	unsigned short IntervalTime;  //采样间隔时间
	unsigned short GTDownFlag;   	//功图下死点标记
	unsigned short GT_Time;				//功图冲程周期 
	unsigned short GT_Time_Over_Count;   //功图测量超时
	unsigned short Recv_Time_Over_Count; //数据接收超时
}_DL_Pagram;


extern _RTU_Config RTU_Config; 	//存FLASH参数数据块
extern _DL_Config DL_Config; 		//内存变量，电参配置参数区块
extern  _DL_Data DL_Data; 				//电参实时数据
extern _DL_Pagram DL_Pagram; 		//电参采集过程变量
extern _AK_DL_Data AK_DL_Data;

extern _CLOCK SysClock; 		//当前RTC时钟
extern _CLOCK SysBCDClock; 	//当前RTC时钟BCD码

extern short	GT_DL_buf[500];	//位移点对应电流值250点 0.01A
extern short	GT_GL_buf[500];	//位移点对应有功功率值250点 0.01KW

extern unsigned short f50ms_count;		//zigbee串口接收空闲计数器
extern unsigned char SleepTimerFlag; 	//电参定时间隔主动上传标记

extern unsigned short fxms_flag; //电参采集间隔时间标记
extern unsigned short f1s_event; //刷新RTC时钟事件标记
extern unsigned short Diagnosis_event; //诊断时间间隔事件
extern unsigned char DC_Check_flag; //电参校准请求标记 1-解除校准，2-实际值校准，3-默认值校准
extern unsigned short WDG_10ms_count; //看门狗喂狗间隔定时器

extern  unsigned int	SumActivePower_reg;		//总有功电能
extern  unsigned int	SumReactivePower_reg;	//总无功电能

extern unsigned char Clear_SumPower_Flag; //电能清零标志
extern unsigned char WDG_Disable_Flag; //看门狗使能标志
#endif
