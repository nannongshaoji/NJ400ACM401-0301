/****************************************Copyright (c)**************************************************
**                               南大傲拓科技江苏股份有限公司
**                                        研发部
**                                         
**                                 http://www.nandaauto.com
**
********************************************************************************************************/
#include "../lib/ls1b_public.h"
#include "com_cpu.h"
#include "CPLD.h"

s_config0 config0[HCM_CH_NUM] = {{0},{0}};
s_hcm_data hcm_data[HCM_CH_NUM] = {{0},{0},{0},{0},{0}};
s_status0 status0[HCM_CH_NUM] = {{0},{0}};

uint8 aoset_state_byte[HCM_CH_NUM] = {0};
uint32 measure_10hz_data[HCM_CH_NUM][MAX_NUM_HZ] = {{0},{0},{0},{0},{0},{0},{0},{0}};
uint8 measure_10hz_index[HCM_CH_NUM]  ={0};
uint8 process_mode0_data_flag[HCM_CH_NUM] = {0};

/*************************************************************************************
*Function name	:
*Description	:
*
*Parameters 	:
*
*Returned		:
*************************************************************************************/
void process_mode0_data(uint8 channo,uint8 step,uint8 indexnow)
{
  uint32 data;
//	if(processflag==1)
	if(indexnow>=step)
	{
		data= (measure_10hz_data[channo][indexnow]-measure_10hz_data[channo][indexnow-step]);
	}
	else
	{
		data= (measure_10hz_data[channo][indexnow]-measure_10hz_data[channo][MAX_NUM_HZ-step+indexnow]);	
	}
	hcm_data[channo].hcm_hst_value=data*1000/step;//放大10倍，精确到0.1HZ
//	if(hcm_data[channo].hcm_hst_value<9600)
//	{
//			hcm_data[channo].hcm_hst_value=hcm_data[channo].hcm_hst_value;
//	}
}
/*************************************************************************************
*Function name	:
*Description	:
*
*Parameters 	:
*
*Returned		:
*************************************************************************************/
void hcm_read_mode0_data(void)
{
    uint8 i;
    uint8 timestep;
	for(i=0;i<HCM_CH_NUM;i++)
	{
        if(config0[i].Bits.mode!=HCM_MODE_0)
            continue;
        if(status0[i].Bits.start_stop_bit==1)
        {	
            if(config0[i].Bits.mode==HCM_MODE_0) //测频模式
            {
                //measure_10hz_data[i][measure_10hz_index[i]]=CPLD_Read(i*4+2);
                //measure_10hz_index[i]++;
                //if(measure_10hz_index[i]>=MAX_NUM_HZ)
                //measure_10hz_index[i]=0;

                if(hcm_data[i].hcm_s_value0<=2000) //控制最大200个数，即2秒钟作差计算频率
                    timestep=hcm_data[i].hcm_s_value0/10;
                else
                    timestep=200;
                
                if(measure_10hz_index[i]>timestep)
                    process_mode0_data_flag[i]=1;
            
                if(process_mode0_data_flag[i]==1)
                {
                    if(measure_10hz_index[i]==0)
                        process_mode0_data(i,timestep,MAX_NUM_HZ-1);
                    else						
                        process_mode0_data(i,timestep,measure_10hz_index[i]-1);
                }
            }
        }
        else
        {
            measure_10hz_index[i]=0;
            process_mode0_data_flag[i]=0;
//				hcm_data[i].hcm_hst_value=0;
            continue;
        }
	}
}
/*************************************************************************************
*Function name	:
*Description	:
*
*Parameters 	:
*
*Returned		:
*************************************************************************************/
void hcm_process(void)
{
	uint8 i;
	uint16 u16state;
	int32 temp1,temp2;
    
    for(i=0;i<HCM_CH_NUM;i++)
    {
        if(config0[i].Bits.mode==0) //如果配置成无,不做处理
            continue;
        
        if(AO_FLAG&(1<<i))
        {
            if(aoset_state_byte[i]&STATE_RESET)
            {
                CPLD_Write(4*i,HST_Rst|(config0[i].Bits.mode-1));
                hcm_data[i].hcm_hst_value=0;
                hcm_data[i].hcm_hst_c_value=0;
                //status0[i].Bits.start_stop_bit=0;
                aoset_state_byte[i] &= ~STATE_RESET;
            }
            if(aoset_state_byte[i]&STATE_START)
            {
                if(config0[i].Bits.mode==8)
                CPLD_Write(4*i,HST_EN|HST_Start|(config0[i].Bits.x1_x4_bit<<4)|(config0[i].Bits.mode-1));
                else						
                CPLD_Write(4*i,HST_EN|HST_Start|(config0[i].Bits.mode-1));
                aoset_state_byte[i] &= ~STATE_START;	
                //status0[i].Bits.start_stop_bit=1;
            }
            if(aoset_state_byte[i]&STATE_STOP)
            {
                CPLD_Write(4*i,HST_EN|HST_Stop|(config0[i].Bits.mode-1));
                aoset_state_byte[i] &= ~STATE_STOP;
                //status0[i].Bits.start_stop_bit=0;
            }
            if(aoset_state_byte[i]&STATE_SET_S_VALUE)
            {
                CPLD_Write(4*i+1,hcm_data[i].hcm_s_value0);
                aoset_state_byte[i] &= ~STATE_SET_S_VALUE;
            }
            AO_FLAG &= ~(1<<i);
        }
        
        u16state = CPLD_Read(i*4);//读HST_Ctrl
        if(u16state&(1<<6))		//启停状态位
        {
            status0[i].Bits.start_stop_bit=1;
        }
        else
        {
            status0[i].Bits.start_stop_bit=0;
        }
        
        if(config0[i].Bits.mode>HCM_MODE_0)// 
        {
            hcm_data[i].hcm_hst_value=CPLD_Read(i*4+2);//读取HST_Value
            hcm_data[i].hcm_hst_c_value=CPLD_Read(i*4+3);//读取HST_C_Value
            
            if(config0[i].Bits.int_en==1) //中断使能后
            {	
                if(u16state&(1<<8)) //Int_flag
                {
                    CPLD_Write(4*i,u16state|0x100); //清Int_flag
                    status0[i].Bits.event_chan =1;			//置事件位
                }
            }

            if(u16state&(1<<9)) //I_AUX
            {
                status0[i].Bits.i_aux=1; 
            }
            else
            {
                status0[i].Bits.i_aux=0;
            }
            
        }
        
        if(config0[i].Bits.cp_en==1) //比较使能后增加比较状态位
        {
            if(config0[i].Bits.mode<=HCM_MODE_3)//1,2,3,4模式无符号数比较
            {
                if(hcm_data[i].hcm_hst_value>hcm_data[i].hcm_s_value1)
                {
                    status0[i].Bits.counter_high=1;
                    status0[i].Bits.counter_equ=0;
                    status0[i].Bits.counter_low=0;
                }
                else if(hcm_data[i].hcm_hst_value<hcm_data[i].hcm_s_value1)
                {
                    status0[i].Bits.counter_high=0;
                    status0[i].Bits.counter_equ=0;
                    status0[i].Bits.counter_low=1;
                }
                else
                {
                    status0[i].Bits.counter_high=0;
                    status0[i].Bits.counter_equ=1;
                    status0[i].Bits.counter_low=0;
                }
            }
            else //5,6,7,8模式有符号数比较
            {
                temp1=(int32)(hcm_data[i].hcm_hst_value);
                temp2=(int32)(hcm_data[i].hcm_s_value1);
                if(temp1>temp2)
                {
                    status0[i].Bits.counter_high=1;
                    status0[i].Bits.counter_equ=0;
                    status0[i].Bits.counter_low=0;
                }
                else if(temp1<temp2)
                {
                    status0[i].Bits.counter_high=0;
                    status0[i].Bits.counter_equ=0;
                    status0[i].Bits.counter_low=1;
                }
                else
                {
                    status0[i].Bits.counter_high=0;
                    status0[i].Bits.counter_equ=1;
                    status0[i].Bits.counter_low=0;
                }
            }						
            //模式1、3、5、6、7增加捕获值与比较值状态位
            if((config0[i].Bits.mode==HCM_MODE_1)||(config0[i].Bits.mode==HCM_MODE_3)||(config0[i].Bits.mode==HCM_MODE_5)||(config0[i].Bits.mode==HCM_MODE_6)||(config0[i].Bits.mode==HCM_MODE_7))
            {
                if(config0[i].Bits.mode<=HCM_MODE_3)//1,2,3,4模式无符号数比较
                {
                    if(hcm_data[i].hcm_hst_c_value>hcm_data[i].hcm_s_value1)
                    {
                        status0[i].Bits.cap_high=1;
                        status0[i].Bits.cap_equ=0;
                        status0[i].Bits.cap_low=0;
                    }
                    else if(hcm_data[i].hcm_hst_c_value<hcm_data[i].hcm_s_value1)
                    {
                        status0[i].Bits.cap_high=0;
                        status0[i].Bits.cap_equ=0;
                        status0[i].Bits.cap_low=1;
                    }
                    else
                    {
                        status0[i].Bits.cap_high=0;
                        status0[i].Bits.cap_equ=1;
                        status0[i].Bits.cap_low=0;
                    }
                }
                else//5,6,7,8模式有符号数比较
                {
                    temp1=(int32)(hcm_data[i].hcm_hst_c_value);
                    temp2=(int32)(hcm_data[i].hcm_s_value1);
                    if(temp1>temp2)
                    {
                        status0[i].Bits.cap_high=1;
                        status0[i].Bits.cap_equ=0;
                        status0[i].Bits.cap_low=0;
                    }
                    else if(temp1<temp2)
                    {
                        status0[i].Bits.cap_high=0;
                        status0[i].Bits.cap_equ=0;
                        status0[i].Bits.cap_low=1;
                    }
                    else
                    {
                        status0[i].Bits.cap_high=0;
                        status0[i].Bits.cap_equ=1;
                        status0[i].Bits.cap_low=0;
                    }
                }
            }
        }			
//		if(start_stop_res_byte[i]&AOSET_START)				
//		else if(start_stop_res_byte[i]&AOSET_STOP)
//			status0[i].Bits.start_stop_bit=0;
//									
    }			
}

/********************************************* End of File *********************************************/
