#ifndef LS1B_CAN_H
#define LS1B_CAN_H
#include "../app/candrv.h"
#include "../app/config.h"

// CAN引脚定义
#define LS1B_CAN0_RX_GPIO38                   (38)        
#define LS1B_CAN0_TX_GPIO39                   (39)       
#define LS1B_CAN1_RX_GPIO40                   (40)      
#define LS1B_CAN1_TX_GPIO41                   (41)        

#define CANGSR_Bits_BS(CanBase)	        (*(CanBase+2)&0x80) 
#define CANGSR_Bits_TBS(CanBase)	    (*(CanBase+2)&0x04) 
#define can0_read_reg(reg)		        (*(reg+0xbfe50000))
#define can1_read_reg(reg)		        (*(reg+0xbfe54000))
#define can0_write_reg(reg, val)        (*(reg+0xbfe50000)=val)
#define can1_write_reg(reg, val)        (*(reg+0xbfe54000)=val)


// ...还有一些gpio可以复用为gpio的，有需要可以自己添加
void dumpcanregs(volatile unsigned char* base);
void bustimer_init(volatile unsigned char* base);
void desc_init(volatile unsigned char* base);
void set_mode(volatile unsigned char* base);
void  can0_reset(void);
int canNormalRun(volatile unsigned char* base);
void can_init(volatile unsigned char* base);
INT32U CANInit(volatile unsigned char* base);
void send_frame(volatile unsigned char* base);
void receive_frame(volatile volatile unsigned char* base);
void receive_irq_enable(volatile unsigned char* base);
extern void CAN_Receive(volatile unsigned char* base, CanRxMsg *RxMessage);
extern void CAN_Transmit(volatile unsigned char* base, CanTxMsg* TxMessage);

#endif
