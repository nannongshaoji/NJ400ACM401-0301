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
	unsigned int   AVoltage_Check;		//A���ѹУ׼����
	unsigned int   BVoltage_Check;		//B���ѹУ׼����
	unsigned int   CVoltage_Check;		//C���ѹУ׼����
	unsigned int   ACurrent_Check;		//A�����У׼����
	unsigned int   BCurrent_Check;		//B�����У׼����
	unsigned int   CCurrent_Check;		//C�����У׼����
	unsigned int   APower_Check;		//A�๦��У׼����
	unsigned int   BPower_Check;		//B�๦��У׼����
	unsigned int   CPower_Check;		//C�๦��У׼����	
	///////////////45018///////////////////////////////////////
	unsigned short	RunMode;				//����ģʽ
	unsigned short	RS485Addr;			//RS485��վ��ַ
	unsigned short	RS485Baudrate;	//RS485������
	unsigned short	CurrentRatio;		//�������������
	unsigned short	VoltRatio;			//��ѹ���������
	unsigned short	FilterLevel;		//�˲��ȼ�
	unsigned short  CGQGroupNo;			//���������
	unsigned short  CGQIndexNo;			//���������
	unsigned short	DLSleepTime;	  //����ϱ����	��	
	unsigned short	NC1[9];				//����
	unsigned short	RS485_data;			//RS485�ӿڲ�������
	unsigned short	Zigbee_data;		//zigbee�ӿڲ�������
	unsigned short	DLCheckMode;	  //�������ģʽ
	///////////////45039///////////////////////////////////////
	unsigned short  Zigbee_EN;			//Zigbeeʹ��
	unsigned int  Zigbee_ID;				//Zigbee�����
	unsigned short  Zigbee_SC;			//Zigbee�ŵ���
	unsigned short	Zigbee_EE;			//Zigbee����ʹ��
	unsigned int	Zigbee_KY;			//������Կ 10����
	unsigned int	Zigbee_NY;			//������Կ 10����
	unsigned short	Zigbee_AO;      //AO����
	unsigned short	Zigbee_ZS;      //ZS����
	unsigned short Buf[50]; //����
}__attribute__ ((packed)) _RTU_DL_Config;

typedef struct {
	unsigned short Head ; //�̶�0xAA55
	unsigned short IsOK ;  //����������־  0-��������1-����
	unsigned int FileLen;  //�ļ�����
	unsigned short PageNum;  //FLASH���ҳ��
	unsigned short NC0;
	_RTU_DL_Config Config;
	unsigned short Buf[50]; //����
}__attribute__ ((packed)) _RTU_Config;

typedef struct {
	unsigned short  Version;			//�汾
	unsigned short	ManufactureCode;	//���Ҵ���
	unsigned short	RunMode;				//����ģʽ
	unsigned short	RS485Addr;			//RS485��վ��ַ
	unsigned short	RS485Baudrate;	//RS485������ 0-9600��1-19200��2-38400
	unsigned short	CurrentRatio;		//�������������
	unsigned short	VoltRatio;			//��ѹ���������
	unsigned short	FilterLevel;		//�˲��ȼ� 0-�ޣ�1-1�� ~4
	unsigned short  CGQGroupNo;			//���������
	unsigned short  CGQIndexNo;			//���������
	unsigned short	DLSleepTime;	  //����ϱ����	��		
	unsigned short	NC1[9];					//����
	unsigned short	RS485_data;			//RS485�ӿڲ�������
	unsigned short	Zigbee_data;		//zigbee�ӿڲ�������
	unsigned short	DLCheckMode;	  //�������ģʽ
	unsigned short  Zigbee_EN;			//Zigbeeʹ��
	unsigned int  Zigbee_ID;			//Zigbee�����
	unsigned short  Zigbee_SC;			//Zigbee�ŵ���
	unsigned short	Zigbee_EE;			//Zigbee����ʹ��
	unsigned int	Zigbee_KY;			//������Կ 10����
	unsigned int	Zigbee_NY;			//������Կ 10����
	unsigned short	Zigbee_AO;      //AO����
	unsigned short	Zigbee_ZS;      //ZS����
	unsigned short	NC2[66];			//����
}__attribute__ ((packed)) _DL_Config_1;

typedef union 	{      	
				_DL_Config_1  Config;
			 	unsigned short	buf[sizeof(_DL_Config_1)/2];
}_DL_Config;

typedef struct {
	///////////////40000///////////////////////////////////////
	WORD	RTU_Year_Month;			//ֻ������ǰʱ�䣬���£����ֽ�Ϊ��
	WORD	RTU_Day_Hour;				//ֻ������ǰʱ�䣬��ʱ�����ֽ�Ϊ��
	WORD	RTU_Minute_Second;	//ֻ������ǰʱ�䣬���룬���ֽ�Ϊ��
	WORD	GT_ID;							//��ͼID��
	WORD	GT_Point;						//��ͼ����
	WORD	GT_State;						//��ͼ״̬
	WORD	GT_DeadPoint;				//��ͼ������
	WORD	GT_Time;						//�������(��λ0.01��)
	WORD	GT_CurPoint;				//��ǰ�ɼ���
	WORD	NC1[7];							//����
	///////////////40016///////////////////////////////////////
	short	ACurrent_Valid;		//ֻ����A�������Чֵ0.01A
	short	BCurrent_Valid;		//ֻ����B�������Чֵ0.01A
	short	CCurrent_Valid;		//ֻ����C�������Чֵ0.01A
	WORD	AVoltage_Valid;		//ֻ����A���ѹ��Чֵ0.1V
	WORD	BVoltage_Valid;		//ֻ����B���ѹ��Чֵ0.1V
	WORD	CVoltage_Valid;		//ֻ����C���ѹ��Чֵ0.1V
	short	AActivePower_Valid;		//ֻ����A���й�������Чֵ0.01A
	short	BActivePower_Valid;		//ֻ����B���й�������Чֵ0.01A
	short	CActivePower_Valid;		//ֻ����C���й�������Чֵ0.01A
	short	AReactivePower_Valid;		//ֻ����A���޹�������Чֵ0.1V
	short	BReactivePower_Valid;		//ֻ����B���޹�������Чֵ0.1V
	short	CReactivePower_Valid;		//ֻ����C���޹�������Чֵ0.1V
	short	ActivePower;			//ֻ�����й�����(0.01kW)
	short	ReactivePower;		//ֻ�����޹�����(0.01var)
	short	ApparentPower;		//ֻ�������ڹ���(0.01VA)	
	WORD	PowerNetFreq;			//ֻ��������Ƶ��0.01Hz
	short	PowerFactor;			//ֻ��������������-1.00~1.00��
//	WORD	ABC_State;				//����״̬
	unsigned int	SumActivePower;		//ֻ�������й�����
	unsigned int	SumReactivePower;	//ֻ�������޹�����
	INT16S AngleA   ; //A���  X10��
	INT16S AngleB   ; //B���  X10��
	INT16S AngleC   ; //C���  X10��
	INT16U AngleUAB   ; //UAB��  X10��
	INT16U AngleUAC   ; //UAC��  X10��
	INT16U AngleUBC   ; //UBC��  X10��
	WORD	CurrentRMS;				//����
	///////////////40039///////////////////////////////////////
	WORD	NC2[61];			//����
	///////////////40100///////////////////////////////////////
	short	GT_DL_Value[300];	//λ�Ƶ��Ӧ����ֵ250�� 0.01A
	short	GT_GL_Value[300];	//λ�Ƶ��Ӧ�й�����ֵ250�� 0.01KW
	WORD	NC3[50];			//����
}__attribute__ ((packed)) _DL_Data_1;

typedef union 	{      	
				_DL_Data_1  Data;
			 	unsigned short	buf[sizeof(_DL_Data_1)/2];
}__attribute__ ((packed)) _DL_Data;

typedef struct {
	///////////////30000///////////////////////////////////////
	short	ACurrent_Valid;		//ֻ����A�������Чֵ0.01A
	short	BCurrent_Valid;		//ֻ����B�������Чֵ0.01A
	short	CCurrent_Valid;		//ֻ����C�������Чֵ0.01A
	WORD	AVoltage_Valid;		//ֻ����A���ѹ��Чֵ0.01V
	WORD	BVoltage_Valid;		//ֻ����B���ѹ��Чֵ0.01V
	WORD	CVoltage_Valid;		//ֻ����C���ѹ��Чֵ0.01V
	short	ActivePower;			//ֻ�����й�����(0.01kW)
	short	ReactivePower;		//ֻ�����޹�����(0.01var)
	short	ApparentPower;		//ֻ�������ڹ���(0.01VA)
	WORD	PowerNetFreq;			//ֻ��������Ƶ��0.01Hz
	short	PowerFactor;			//ֻ��������������-1.00~1.00��
//	WORD	ABC_State;				//����״̬
	WORD	SumActivePower_high;		//ֻ�������й�����
	WORD	SumActivePower_low;		//ֻ�������й�����
	WORD	SumReactivePower_high;	//ֻ�������޹�����
	WORD	SumReactivePower_low;	//ֻ�������޹�����
	///////////////30016///////////////////////////////////////
	WORD	NC2[34];			//����
	///////////////30100///////////////////////////////////////
	short	GT_DL_Value[300];	//λ�Ƶ��Ӧ����ֵ250�� 0.01A
	///////////////30900///////////////////////////////////////
	short	GT_GL_Value[300];	//λ�Ƶ��Ӧ�й�����ֵ250�� 0.01KW
	WORD	NC3[10];			//����
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
	unsigned char mode;		//ģʽ��0-ʵ�����ߣ�0x10-��������
	unsigned char SynchroStartFlag; //ͬ������ʱ��ʼ
	unsigned char DLmesure_flag; 		//����ͼ�ɼ�������
	unsigned char DLStart_mesure_flag; //��ʼ�ɼ�����ͼ���ݱ��
	unsigned char Dead_point;   //������
	unsigned char Send_flag;		//���ͱ�ʶ
	unsigned char ReSend_flag;	//�ط���ʶ
	unsigned char NC;
	unsigned short SynchroTime;  	//ͬ������ʱʱ��
	unsigned short IntervalTime;  //�������ʱ��
	unsigned short GTDownFlag;   	//��ͼ��������
	unsigned short GT_Time;				//��ͼ������� 
	unsigned short GT_Time_Over_Count;   //��ͼ������ʱ
	unsigned short Recv_Time_Over_Count; //���ݽ��ճ�ʱ
}_DL_Pagram;


extern _RTU_Config RTU_Config; 	//��FLASH�������ݿ�
extern _DL_Config DL_Config; 		//�ڴ������������ò�������
extern  _DL_Data DL_Data; 				//���ʵʱ����
extern _DL_Pagram DL_Pagram; 		//��βɼ����̱���
extern _AK_DL_Data AK_DL_Data;

extern _CLOCK SysClock; 		//��ǰRTCʱ��
extern _CLOCK SysBCDClock; 	//��ǰRTCʱ��BCD��

extern short	GT_DL_buf[500];	//λ�Ƶ��Ӧ����ֵ250�� 0.01A
extern short	GT_GL_buf[500];	//λ�Ƶ��Ӧ�й�����ֵ250�� 0.01KW

extern unsigned short f50ms_count;		//zigbee���ڽ��տ��м�����
extern unsigned char SleepTimerFlag; 	//��ζ�ʱ��������ϴ����

extern unsigned short fxms_flag; //��βɼ����ʱ����
extern unsigned short f1s_event; //ˢ��RTCʱ���¼����
extern unsigned short Diagnosis_event; //���ʱ�����¼�
extern unsigned char DC_Check_flag; //���У׼������ 1-���У׼��2-ʵ��ֵУ׼��3-Ĭ��ֵУ׼
extern unsigned short WDG_10ms_count; //���Ź�ι�������ʱ��

extern  unsigned int	SumActivePower_reg;		//���й�����
extern  unsigned int	SumReactivePower_reg;	//���޹�����

extern unsigned char Clear_SumPower_Flag; //���������־
extern unsigned char WDG_Disable_Flag; //���Ź�ʹ�ܱ�־
#endif
