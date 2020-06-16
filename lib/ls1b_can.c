#include "ls1b_public.h"
#include "ls1b_regs.h"
#include "ls1b_clock.h"
#include "ls1b_can.h"
#include "ls1b_delay.h"
#include "ls1b_pin.h"
#include "sja1000.h"
#include "../app/candrv.h"
#include "../app/system_init.h"
extern uint8 MODULE_ADDR;	


/********************** debug ********************/
void dumpcanregs(volatile unsigned char* base)
{
	int i;
	myprintf("====dump addr: 0x%x regs\n",base);
		
	for(i = 0;i < 30;i++)
	{	
		myprintf("==addr:0x%x  val: 0x%x\n",base+i,*(base+i));
	}
	
	
}

/*********************** init *********************/

/* set can-bus timer reg 6,7 */
void bustimer_init(volatile unsigned char* base)
{
	*(base+6) = 0x03;
	*(base+7) = 0x1c;
	//*(base+6) = 0x47;
	//*(base+7) = 0x4e;
}

void desc_init(volatile unsigned char* base)
{
//filer
	*(base+16) = 0x0;
	*(base+17) = 0x0;
	*(base+18) = 0x0;
	*(base+19) = 0x0;
//filer mask	
	*(base+20) = 0xff;
	*(base+21) = 0xff;
	*(base+22) = 0xff;
	*(base+23) = 0xff;
}

void set_mode(volatile unsigned char* base)
{
	
//	*(base+0) = 0x0;
	*(base+0) = 0x04;
	
//	*(base+4) = 0xff;
	
}

/* init can-descriptor and mode */
void canConfig(volatile unsigned char* base)
{
	int i;
	unsigned char status;
	if(base==(volatile unsigned char*)LS1B_REG_BASE_CAN0)
	{
		pin_set_purpose(LS1B_CAN0_RX_GPIO38,PIN_PURPOSE_OTHER);
		pin_set_purpose(LS1B_CAN0_TX_GPIO39,PIN_PURPOSE_OTHER);
	}
	else if(base==(volatile unsigned char*)LS1B_REG_BASE_CAN1)
	{
		pin_set_purpose(LS1B_CAN1_RX_GPIO40,PIN_PURPOSE_OTHER);
		pin_set_purpose(LS1B_CAN1_TX_GPIO41,PIN_PURPOSE_OTHER);
	}
	*(base+REG_IER) = IRQ_OFF;
	*(base+REG_MOD) = MOD_RM;
	status = *(base+REG_MOD) ;
	for (i=0;i<100;i++)
	{
		if (status & MOD_RM)
		{
			break;
		}
		*(base+REG_MOD) = MOD_RM;
		delay_us(10);
		status =  *(base+REG_MOD);
	}
	*(base+REG_CMR) = 0x80;
	*(base+REG_CDR) = 0xc0;
	*(base+REG_RXERR) = 0;
	*(base+REG_TXERR) = 0;
	//can0_write_reg(REG_ACCC0, (dev->can.addr & 0x7F));
	*(base+REG_ACCC0) =  (MODULE_ADDR& 0x7F);
	*(base+REG_ACCC1) = 0x00;
	*(base+REG_ACCC2) = 0x7F;
	*(base+REG_ACCC3) = 0x00;
	*(base+REG_ACCM0) = 0x80;	
	*(base+REG_ACCM1) = 0xff;
	*(base+REG_ACCM2) = 0x80;
	*(base+REG_ACCM3) = 0xff;	
	//bustimer_init(base);
	*(base+REG_BTR0) = 0x03;
	*(base+REG_BTR1) = 0x1c;//2f
	*(base+REG_IER) =  IRQ_ALL & ~IRQ_BEI;
	*(base+REG_OCR) = 0x5a;	
	*(base+REG_MOD) = MOD_STM;
//externed mode
//	*(base+1) = 0x81;
	//*(base+1) = 0x8c;
	while (*(base+REG_SR)  & 0x80);
//	desc_init(base);
	//set_mode(base);
	
}

void  can0_reset(void)
{
	unsigned char status = can0_read_reg((volatile unsigned char* )REG_MOD);
	int i;
	
	/* disable interrupts */
	//can0_write_reg(REG_IER, IRQ_OFF);

	for (i = 0; i < 10; i++) {
		/* check reset bit */
		if (status & MOD_RM) {
		//	can0_write_reg((volatile unsigned char* )REG_MOD, MOD_STM);
			return;
		}
		can0_write_reg((volatile unsigned char* )REG_MOD, MOD_RM);	/* reset chip */
		delay_ms(1);
		status = can0_read_reg((volatile unsigned char* )REG_MOD);
	}
	return;
}


int canNormalRun(volatile unsigned char* base)
{
	unsigned char mod;
	mod =*(base+REG_MOD) ;

	if (mod & 0x01)
	{
		*(base+REG_MOD)=(mod & 0xfe);
	}
	mod =*(base+REG_MOD) ;
	if (mod & 0x01)	return (0x30);

	return (0);
}

INT32U CANInit (volatile unsigned char* base)
{
	INT32U K=0;
	canConfig(base);
	canNormalRun(base);
	while((CANGSR_Bits_BS(base)!=0)&&K<1000)
	{
		K++;
		NOP;NOP;NOP;
	}
		if(K>=1000)
	{
		S_BITCLR((uint8 *) &CAN_STATE.STATE,0);//BITCLR(&(CAN_STATE.STATE),0);
		S_BITSET((uint8 *) &CAN_STATE.STATE,7);//BITSET(&(CAN_STATE.STATE),7);
		return 0x22;
	}
	
	S_BITSET((uint8 *) &CAN_STATE.STATE,0);//BITSET(&CAN_STATE.STATE,0);
	S_BITCLR((uint8*) &CAN_STATE.STATE,7);//BITCLR(&CAN_STATE.STATE,7);
	return 0;

}


/***********************send frame*********************/
void send_frame(volatile unsigned char* base)
{
//tx frame & id
	int volatile i;
	for(i=20000;i!=0;i--){
		if(*(base+2)&0x04) break;
	}
	if(i==0){

		myprintf("***11111****erro*****\n");
		return;
	}
	*(base+16) = 0x88;
//	delay(300);
//	printf("*************************base+16_0x84=%x\n",*(base+16));
	//*(base+17) = 0x12;
	//*(base+18) = 0x34;
	//*(base+19) = 0x45;
	//*(base+20) = 0x56;
	*(base+17) = 0x01;

//	delay(300);
//	printf("*************************base+17_0x1=%x\n",*(base+17));
	*(base+18) = 0x02;
	*(base+19) = 0x03;
	*(base+20) = 0x04;
//tx data
    static unsigned char j=0;

	*(base+21) = (0x01+j);
	*(base+22) = (0x02+j);
	*(base+23) = (0x03+j);
	*(base+24) =(0x04+j);
	*(base+25) = (0x05+j);
	*(base+26) = (0x06+j);
	*(base+27) = (0x07+j);
	*(base+28) = (0x08+j);
	j++;
	*(base+1) = 0x81;	
	delay_us(10);	
	for(i=20;i!=0;i--){
		if(*(base+2)&0x08) break;
	}
	if(i==0){

		myprintf("**2222222****erro*****\n");
		return;
	}
}


/***********************send frame*********************/
void CAN_Transmit(volatile unsigned char* base, CanTxMsg* TxMessage)
{
//tx frame & id
	int volatile i;
	for(i=20000;i!=0;i--){
		if(*(base+2)&0x04) break;
	}
	if(0==i){

		myprintf("***11111****erro*****\n");
		return;
	}
	if( CAN_RTR_DATA == TxMessage->RTR){
			
		*(base+16) = 0x80|TxMessage->DLC ;
		}

	*(base+17) =(unsigned char)( (TxMessage->ExtId>>24)&0x000000FF);
	*(base+18) =(unsigned char)( (TxMessage->ExtId>>16)&0x000000FF);
	*(base+19) =(unsigned char) ( (TxMessage->ExtId>>8)&0x000000FF);
	*(base+20) = (unsigned char) ( TxMessage->ExtId&0x000000FF);
//tx data

	*(base+21) = TxMessage->Data[0];
	*(base+22) = TxMessage->Data[1];
	*(base+23) = TxMessage->Data[2];
	*(base+24) = TxMessage->Data[3];
	*(base+25) = TxMessage->Data[4];
	*(base+26) = TxMessage->Data[5];
	*(base+27) = TxMessage->Data[6];
	*(base+28) = TxMessage->Data[7];
	*(base+1) = 0x81;	
	delay_us(10);	
	for(i=20000;i!=0;i--){
		if(*(base+2)&0x08) break;
	}
	if(i==0){

		myprintf("**2222222****erro*****\n");
		return;
	}
}


/***********************receive frame*********************/
void receive_frame(volatile unsigned char* base)
{
//poll rx intr
	myprintf("====pre to receive...\n");
	
	myprintf("===desc addr: %x val: %02x\n",base+2,*(base+2));	
//	dumpcanregs(base);
	
	//while(!(*(base+3) & 0x1));
//	while(!((*(base+2)) & 0x01));	

	myprintf("===desc addr: %x val: %02x\n",base+2,*(base+2));	
	myprintf("===rx a frame!\n");	
	myprintf("===desc addr: %x val: %02x\n",base+16,*(base+16));	
	myprintf("===desc addr: %x val: %02x\n",base+17,*(base+17));	
	myprintf("===desc addr: %x val: %02x\n",base+18,*(base+18));	
	myprintf("===desc addr: %x val: %02x\n",base+19,*(base+19));	
	myprintf("===desc addr: %x val: %02x\n",base+20,*(base+20));	
	myprintf("===desc addr: %x val: %02x\n",base+21,*(base+21));	
	myprintf("===desc addr: %x val: %02x\n",base+22,*(base+22));	
	myprintf("===desc addr: %x val: %02x\n",base+23,*(base+23));	
	myprintf("===desc addr: %x val: %02x\n",base+24,*(base+24));	
	myprintf("===desc addr: %x val: %02x\n",base+25,*(base+25));	
	myprintf("===desc addr: %x val: %02x\n",base+26,*(base+26));	
	myprintf("===desc addr: %x val: %02x\n",base+27,*(base+27));	
	myprintf("===desc addr: %x val: %02x\n",base+28,*(base+28));

	*(base+1) = 0x8c;
}

void CAN_Receive(volatile unsigned char* base, CanRxMsg *RxMessage)
{
	RxMessage->DLC=*(base+16)&0x0f;
	RxMessage->ExtId=(*(base+17)<<24)|(*(base+18)<<16)|(*(base+19)<<8)|(*(base+20));
	RxMessage->Data[0]=*(base+21);
	RxMessage->Data[1]=*(base+22);
	RxMessage->Data[2]=*(base+23);
	RxMessage->Data[3]=*(base+24);
	RxMessage->Data[4]=*(base+25);
	RxMessage->Data[5]=*(base+26);	
	RxMessage->Data[6]=*(base+27);
	RxMessage->Data[7]=*(base+28);	
	*(base+1) = 0x8c;

}
void receive_irq_enable(volatile unsigned char* base)
{
	myprintf("\n[%s]\n",__FUNCTION__);
	*(base+4) =0x01;
	myprintf("===desc addr: %x val: %02x\n",base+4,*(base+4));
}


