/****************************************Copyright (c)**************************************************
**                               南大傲拓科技江苏股份有限公司
**                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/
#include "../lib/ls1b_delay.h"
#include "../lib/ls1b_public.h"
#include "system_init.h"
#include "timer.h"


/*************************************************************************************
*Function name	:
*Description	:
*
*Parameters		:
*				:
*Returned		:
*************************************************************************************/
void NA_GPIO_Init(void)
{
    //LED Pin Initialize
    gpio_init(RUN_LED_PIN, gpio_mode_output);
    gpio_init(ACT_LED_PIN, gpio_mode_output);
    gpio_init(FAULT_LED_PIN, gpio_mode_output);
    RUN_LED_OFF;
    ACT_LED_OFF;
    FAULT_LED_OFF;
    //Module Address Pin Initialize
    gpio_init(CANADDR_ID0_PIN, gpio_mode_input);
	gpio_init(CANADDR_ID1_PIN, gpio_mode_input);
    gpio_init(CANADDR_ID2_PIN, gpio_mode_input);	
    gpio_init(CANADDR_ID3_PIN, gpio_mode_input);
	gpio_init(CANADDR_ID4_PIN, gpio_mode_input); 
	gpio_init(CANADDR_ID5_PIN, gpio_mode_input);
	gpio_init(CANADDR_ID6_PIN, gpio_mode_input);
}
/*************************************************************************************
*Function name	:
*Description	:
*
*Parameters		:
*				:
*Returned		:
*************************************************************************************/
void NA_Modnle_Init(void)
{
    uint16 temp_addr1,temp_addr2,i;
    for(i=0;i<5;i++)
    {
        temp_addr1=(uint8)(((gpio_get(CANADDR_ID2_PIN)<<2)|(gpio_get(CANADDR_ID6_PIN)<<1)|gpio_get(CANADDR_ID5_PIN))*15 \
        +((gpio_get(CANADDR_ID0_PIN)<<3)|(gpio_get(CANADDR_ID1_PIN)<<2)|(gpio_get(CANADDR_ID3_PIN)<<1)|gpio_get(CANADDR_ID4_PIN)));
        delay_ms(500);
        temp_addr2=(uint8)(((gpio_get(CANADDR_ID2_PIN)<<2)|(gpio_get(CANADDR_ID6_PIN)<<1)|gpio_get(CANADDR_ID5_PIN))*15 \
        +((gpio_get(CANADDR_ID0_PIN)<<3)|(gpio_get(CANADDR_ID1_PIN)<<2)|(gpio_get(CANADDR_ID3_PIN)<<1)|gpio_get(CANADDR_ID4_PIN)));
        if (temp_addr1 == temp_addr2) 
        {
           MODULE_ADDR = temp_addr1;
           myprintf("MODULE_ADDR = %d\n",MODULE_ADDR);
           break;
        }
        else 
        {
           MODULE_ADDR = 0;
           myprintf("MODULE_ADDR ERROR\n");
        }
    }
    MODULE_TYPE = NJ400ACM0301;
    MODULE_STATE = 0x40;
    CLK_SOFT_INIT();
}
/*************************************************************************************
*Function name	:
*Description	:
*
*Parameters		:
*				:
*Returned		:
*************************************************************************************/
uint32 S_BITTST(uint8 *P,uint8 NO)
{
	uint8 *BIT_BYTE_P;
	uint8 BYTE_POSI;
	uint8 BIT_POSI;
	uint8 BIT_MAP[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
	
	BIT_BYTE_P=P;
	BYTE_POSI=NO/8;
	BIT_POSI=NO % 8;
	if ((*(BIT_BYTE_P+BYTE_POSI) & BIT_MAP[BIT_POSI])!=0)  return 0xFF;
	else return(0);
}
/*************************************************************************************
*Function name	:
*Description	:
*
*Parameters		:
*				:
*Returned		:
*************************************************************************************/
uint32 S_BITTST32(uint32 *P,uint8 NO)
{
	uint32 BIT_MAP[32]={0x00000001,0x00000002,0x00000004,0x00000008,0x00000010,0x00000020,0x00000040,0x00000080,
						0x00000100,0x00000200,0x00000400,0x00000800,0x00001000,0x00002000,0x00004000,0x00008000,
						0x00010000,0x00020000,0x00040000,0x00080000,0x00100000,0x00200000,0x00400000,0x00800000,
						0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,0x40000000,0x80000000};
	
	if ((*P & BIT_MAP[NO])!=0)  return 0xFF;
	else return(0);
}
/*************************************************************************************
*Function name	:S_BITCLR
*Description	:bit clear
*
*Parameters		:P-->Parameter
*				:NO-->num
*Returned		:None
*************************************************************************************/
void S_BITCLR(uint8 *P,uint8 NO)
{
	uint32 BIT_MAP[8]={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F};
	
	*P&=BIT_MAP[NO];
}
/*************************************************************************************
*Function name	:S_BITSET
*Description	:bit set
*
*Parameters		:P-->Parameter
*				:NO-->num
*Returned		:None
*************************************************************************************/
void S_BITSET(uint8 *P,uint8 NO)
{
	uint32 BIT_MAP[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
	
	*P|=BIT_MAP[NO];
}
/********************************************* End of File *********************************************/
