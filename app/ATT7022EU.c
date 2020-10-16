#include "ATT7022EU.h"
#include "MR25H256.h"
#include "config.h"
#include "../lib/ls1b_public.h"
#include "MY_define.h" 
#include "../example/led.h"

ls1b_spi_info_t spi0_info_ATT7022EU={0};


//功率系数K=2.592*10^10/(HFconst*EC*2^23)=0.0161581830549312=1/K
//K=61.8882 HFconst=59.7589
//EC=3200;
#define  ATT7022EU_K  61.8882   //功率常数K值
//#define  ATT7022EU_K  6.18882   //功率常数K值

#define  U_REF 18   //220  较表单相电压参考值(V) 

unsigned short DGT_Data_Ptr = 0;
int IValue=0;


void adjust_touch_init(void)
{
	gpio_init(ADJUST_PIN, gpio_mode_input);
	led_init(ADJUST_START_LED);	
	led_init(CLEAR_START_LED);
	return ;
}

volatile BOOL read_adjust_flag(void)
{
	
	return (!gpio_get(ADJUST_PIN));
}

volatile BOOL read_clear_flag(void)
{
	
	return (!gpio_get(CLEAR_PIN));
}

void ATT7022EU_SPI_PIN_Init(void)
{  
	  //Pin Initialize 
    adjust_touch_init();	
    pin_set_purpose(SPI0_CLK_Pin,PIN_PURPOSE_OTHER);//clk
    pin_set_purpose(SPI0_MISO_Pin,PIN_PURPOSE_OTHER);//miso
    pin_set_purpose(SPI0_MOSI_Pin,PIN_PURPOSE_OTHER);//mosi
    pin_set_purpose(SPI0_CS2_Pin,PIN_PURPOSE_OTHER);//cs

}
/*******************************************************************************
* 函数名称: ATT7022EU_SPI_MODE_Init0()
* 功能描述: SPI0初始化--作为主设备实现
* 输入参数: void
* 返回参数: 无
********************************************************************************/
void ATT7022EU_SPI_MODE_Init0(void)
{  
  
    //SPI0 Initialize
    spi0_info_ATT7022EU.SPIx			= LS1B_SPI_0;
    spi0_info_ATT7022EU.cs			= LS1B_SPI_CS_2;
    spi0_info_ATT7022EU.max_speed_hz	= 125000;
    spi0_info_ATT7022EU.cpol			= SPI_CPOL_0;
    spi0_info_ATT7022EU.cpha			= SPI_CPHA_1;

    spi_init(&spi0_info_ATT7022EU);
}


void ATT7022EU_Write_Reg(unsigned char RegAddr,unsigned int Value)
{
	ATT7022EU_SPI_MODE_Init0();
	SPI0_CS2_L;
	delay_us(10);
    spi_txrx_byte(&spi0_info_ATT7022EU,0x80|RegAddr);
	delay_us(10);
	spi_txrx_byte(&spi0_info_ATT7022EU,(Value&0x00ff0000)>>16);
	spi_txrx_byte(&spi0_info_ATT7022EU,(Value&0x0000ff00)>>8);
	spi_txrx_byte(&spi0_info_ATT7022EU,Value&0xff);
	SPI0_CS2_H;

}

void ATT7022EU_Reset(void)
{
	ATT7022EU_SPI_MODE_Init0();
	SPI0_CS2_L;
    spi_txrx_byte(&spi0_info_ATT7022EU,0xD3);
	spi_txrx_byte(&spi0_info_ATT7022EU,0);
	spi_txrx_byte(&spi0_info_ATT7022EU,0);
	spi_txrx_byte(&spi0_info_ATT7022EU,0);
	SPI0_CS2_H;
}
unsigned int ATT7022EU_Read_Reg(unsigned char RegAddr)
{
	unsigned int Value=0;
	unsigned char ret=0;
	ATT7022EU_SPI_MODE_Init0();
	delay_us(10);
	SPI0_CS2_L;
	delay_us(10);
	ret=spi_txrx_byte(&spi0_info_ATT7022EU,0x00+RegAddr);
	delay_us(10);	
	ret=spi_txrx_byte(&spi0_info_ATT7022EU,0xFF);
	Value=ret;
	ret=spi_txrx_byte(&spi0_info_ATT7022EU,0xFF);
	Value=(Value<<8)+ret;
	ret=spi_txrx_byte(&spi0_info_ATT7022EU,0xFF);
	Value=(Value<<8)+ret;	
	SPI0_CS2_H;
	return Value;
}

void ATT7022EU_Read_CG_Data(void)
{
	float f_value;
	IValue=ATT7022EU_Read_Reg(0x0d); 
	DL_Data.Data.AVoltage_Valid=IValue*100/8/1024;	
	IValue=ATT7022EU_Read_Reg(0x0e);
	DL_Data.Data.BVoltage_Valid=IValue*100/8/1024;
	IValue=ATT7022EU_Read_Reg(0x0f);
	DL_Data.Data.CVoltage_Valid=IValue*100/8/1024;
	IValue=ATT7022EU_Read_Reg(0x10);
	DL_Data.Data.ACurrent_Valid=IValue*100/8/8*DL_Config.Config.CurrentRatio/128;	
	IValue=ATT7022EU_Read_Reg(0x11);
	DL_Data.Data.BCurrent_Valid=IValue*100/8/8*DL_Config.Config.CurrentRatio/128;	
	IValue=ATT7022EU_Read_Reg(0x12);
	DL_Data.Data.CCurrent_Valid=IValue*100/8/8*DL_Config.Config.CurrentRatio/128;	
	IValue=ATT7022EU_Read_Reg(0x01);
	if (IValue > 8388608) IValue = IValue - 16777216;
	f_value = ((float)IValue)*10/ATT7022EU_K;        
	DL_Data.Data.AActivePower_Valid=f_value*DL_Config.Config.CurrentRatio;
	IValue=ATT7022EU_Read_Reg(0x02);
	if (IValue > 8388608) IValue = IValue - 16777216;
	f_value = ((float)IValue)*10/ATT7022EU_K;        
	DL_Data.Data.BActivePower_Valid=f_value*DL_Config.Config.CurrentRatio;
	IValue=ATT7022EU_Read_Reg(0x03);
	if (IValue > 8388608) IValue = IValue - 16777216;
	f_value = ((float)IValue)*10/ATT7022EU_K;        
	DL_Data.Data.CActivePower_Valid=f_value*DL_Config.Config.CurrentRatio;
	
	IValue=ATT7022EU_Read_Reg(0x05);
	if (IValue > 8388608) IValue = IValue - 16777216;
	f_value = ((float)IValue)*10/ATT7022EU_K;        
	f_value=f_value*DL_Config.Config.CurrentRatio;			
	DL_Data.Data.AReactivePower_Valid=(short)f_value;
	IValue=ATT7022EU_Read_Reg(0x06);
	if (IValue > 8388608) IValue = IValue - 16777216;
	f_value = ((float)IValue)*10/ATT7022EU_K;  
	f_value = f_value*DL_Config.Config.CurrentRatio;      
	DL_Data.Data.BReactivePower_Valid=(short)f_value;
	IValue=ATT7022EU_Read_Reg(0x07);
	if (IValue > 8388608) IValue = IValue - 16777216;
	f_value = ((float)IValue)*10/ATT7022EU_K;   
	f_value = f_value*DL_Config.Config.CurrentRatio;
	DL_Data.Data.CReactivePower_Valid=(short)f_value;
	
	IValue=ATT7022EU_Read_Reg(0x04);
	if (IValue > 8388608) IValue = IValue - 16777216;
	f_value = ((float)IValue)*2*10/ATT7022EU_K;        
	DL_Data.Data.ActivePower=f_value*DL_Config.Config.CurrentRatio;
	IValue=ATT7022EU_Read_Reg(0x08);
	if (IValue > 8388608) IValue = IValue - 16777216;
	f_value = ((float)IValue)*2*10/ATT7022EU_K; 
	f_value = f_value*DL_Config.Config.CurrentRatio;			
	DL_Data.Data.ReactivePower=(short)f_value;
	IValue=ATT7022EU_Read_Reg(0x0C);
	if (IValue > 8388608) IValue = IValue - 16777216;
	f_value = ((float)IValue)*2*10/ATT7022EU_K;        
	DL_Data.Data.ApparentPower=f_value*DL_Config.Config.CurrentRatio;
	
	IValue=ATT7022EU_Read_Reg(0x17);
	if (IValue > 8388608) IValue = IValue - 16777216;
	f_value = ((float)IValue) /8388608;          
	DL_Data.Data.PowerFactor=f_value*100;

	IValue=ATT7022EU_Read_Reg(0x1C);
	DL_Data.Data.PowerNetFreq=IValue*100/8/1024;
#if 0	
	IValue=ATT7022EU_Read_Reg(0x2C);
	myprintf("ATT7022EU_Read_Reg(0x2C)=0x%x \n",IValue);
	if((IValue&0x1f)>0)	DL_Data.Data.ABC_State=1;
	else DL_Data.Data.ABC_State=0;
#endif

	IValue=ATT7022EU_Read_Reg(0x18);
//	myprintf("ATT7022EU_Read_Reg(0x18)=0x%x \n",IValue);
	if (IValue >= 1048576) IValue = IValue - 16777216;
	DL_Data.Data.AngleA=(float)IValue*1800/1048576;
	
	IValue=ATT7022EU_Read_Reg(0x19);
//	myprintf("ATT7022EU_Read_Reg(0x19)=0x%x \n",IValue);
	if (IValue >= 1048576) IValue = IValue - 16777216;
	DL_Data.Data.AngleB=(float)IValue*1800/1048576;	

	IValue=ATT7022EU_Read_Reg(0x1A);
//	myprintf("ATT7022EU_Read_Reg(0x1A)=0x%x \n",IValue);
	if (IValue >= 1048576) IValue = IValue - 16777216;
	DL_Data.Data.AngleC=(float)IValue*1800/1048576;
	
	IValue=ATT7022EU_Read_Reg(0x26);
//	myprintf("ATT7022EU_Read_Reg(0x18)=0x%x \n",IValue);
//	if (IValue >= 1048576) IValue = IValue - 16777216;
	DL_Data.Data.AngleUAB=(float)IValue*1800/1048576;
	
	IValue=ATT7022EU_Read_Reg(0x27);
//	myprintf("ATT7022EU_Read_Reg(0x19)=0x%x \n",IValue);
//	if (IValue >= 1048576) IValue = IValue - 16777216;
	DL_Data.Data.AngleUAC=(float)IValue*1800/1048576;	

	IValue=ATT7022EU_Read_Reg(0x28);
//	myprintf("ATT7022EU_Read_Reg(0x1A)=0x%x \n",IValue);
//	if (IValue >= 1048576) IValue = IValue - 16777216;
	DL_Data.Data.AngleUBC=(float)IValue*1800/1048576;
	
	IValue=ATT7022EU_Read_Reg(0x1D);
	if((IValue&0x88)!=0)
	{
		if((IValue&0x08)!=0)
		{//有功电能溢出
			SumActivePower_reg=SumActivePower_reg+524288*DL_Config.Config.CurrentRatio;//0x1000000/32;
			IValue=ATT7022EU_Read_Reg(0x21);
			DL_Data.Data.SumActivePower=SumActivePower_reg+IValue*DL_Config.Config.CurrentRatio/32;  //单位0.01KWh
		}
		else if((IValue&0x80)!=0)
		{//无功电能溢出
			SumReactivePower_reg=SumReactivePower_reg+524288*DL_Config.Config.CurrentRatio;//0x1000000/32;
			IValue=ATT7022EU_Read_Reg(0x25);
			DL_Data.Data.SumReactivePower=SumReactivePower_reg+IValue*DL_Config.Config.CurrentRatio/32;  //单位0.01KWh
		}
	}
	else
	{
		IValue=ATT7022EU_Read_Reg(0x21);
		DL_Data.Data.SumActivePower=SumActivePower_reg+IValue*DL_Config.Config.CurrentRatio/32;  //单位0.01KWh
		IValue=ATT7022EU_Read_Reg(0x25);
		DL_Data.Data.SumReactivePower=SumReactivePower_reg+IValue*DL_Config.Config.CurrentRatio/32; //单位0.01KWh
	}
}

void print_power_data (void)
{

#if 0

	myprintf("ID:0x%x \n",ATT7022EU_Read_Reg(0x5d));	
	ATT7022EU_Write_Reg(0xc6,0x5a);	
	myprintf("0x00 reg:0x%04x \n",ATT7022EU_Read_Reg(0));
	myprintf("0x01 reg:0x%04x \n",ATT7022EU_Read_Reg(1));	
	myprintf("AVoltage_Check:0x%x \n",ATT7022EU_Read_Reg(0x17));
	ATT7022EU_Write_Reg(0x17,0x123456);
	myprintf("AVoltage_Check:0x%x \n",ATT7022EU_Read_Reg(0x17));	
	myprintf("RTU_Config.Config.AVoltage_Check:0x%x \n",RTU_Config.Config.AVoltage_Check);
	ATT7022EU_Write_Reg(0x17,RTU_Config.Config.AVoltage_Check);
	myprintf("AVoltage_Check:0x%x \n",ATT7022EU_Read_Reg(0x17));
	ATT7022EU_Write_Reg(0xc6,0xaa);

#endif
	
	myprintf("ACurrent_Valid:%d\n",DL_Data.Data.ACurrent_Valid);
	myprintf("BCurrent_Valid:%d\n",DL_Data.Data.BCurrent_Valid);
	myprintf("CCurrent_Valid:%d\n",DL_Data.Data.CCurrent_Valid);
	myprintf("AVoltage_Valid:%d\n",DL_Data.Data.AVoltage_Valid);
	myprintf("BVoltage_Valid:%d\n",DL_Data.Data.BVoltage_Valid);
	myprintf("CVoltage_Valid:%d\n",DL_Data.Data.CVoltage_Valid);
	myprintf("AActivePower_Valid:%d\n",DL_Data.Data.AActivePower_Valid);
	myprintf("BActivePower_Valid:%d\n",DL_Data.Data.BActivePower_Valid);
	myprintf("CActivePower_Valid:%d\n",DL_Data.Data.CActivePower_Valid);
	myprintf("AReactivePower_Valid:%d\n",DL_Data.Data.AReactivePower_Valid);
	myprintf("BReactivePower_Valid:%d\n",DL_Data.Data.BReactivePower_Valid);
	myprintf("CReactivePower_Valid:%d\n",DL_Data.Data.CReactivePower_Valid);
	myprintf("ActivePower:%d\n",DL_Data.Data.ActivePower);
	myprintf("ReactivePower:%d\n",DL_Data.Data.ReactivePower);
	myprintf("ApparentPower:%d\n",DL_Data.Data.ApparentPower);
	myprintf("PowerNetFreq:%d\n",DL_Data.Data.PowerNetFreq);
	myprintf("PowerFactor:%d\n",DL_Data.Data.PowerFactor);
	myprintf("SumActivePower:%d\n",DL_Data.Data.SumActivePower);
	myprintf("SumReactivePower:%d\n",DL_Data.Data.SumReactivePower);
	myprintf("AngleA:%d\n",DL_Data.Data.AngleA);
	myprintf("AngleB:%d\n",DL_Data.Data.AngleB);
	myprintf("AngleC:%d\n\n\n",DL_Data.Data.AngleC);
	myprintf("AngleUAB:%d\n",DL_Data.Data.AngleUAB);
	myprintf("AngleUAC:%d\n",DL_Data.Data.AngleUAC);
	myprintf("AngleUBC:%d\n\n\n",DL_Data.Data.AngleUBC);

}




unsigned int ATT7022EU_Read_Data(void)
{
	ATT7022EU_Read_CG_Data();
	return 0;
}

void spi_DL_Check(void)
{
	ATT7022EU_Write_Reg(0x17,RTU_Config.Config.AVoltage_Check);
	ATT7022EU_Write_Reg(0x18,RTU_Config.Config.BVoltage_Check);
	ATT7022EU_Write_Reg(0x19,RTU_Config.Config.CVoltage_Check);

	ATT7022EU_Write_Reg(0x1a,RTU_Config.Config.ACurrent_Check);
	ATT7022EU_Write_Reg(0x1b,RTU_Config.Config.BCurrent_Check);
	ATT7022EU_Write_Reg(0x1c,RTU_Config.Config.CCurrent_Check);
	
	ATT7022EU_Write_Reg(0x04,RTU_Config.Config.APower_Check);
	ATT7022EU_Write_Reg(0x05,RTU_Config.Config.BPower_Check);
	ATT7022EU_Write_Reg(0x06,RTU_Config.Config.CPower_Check);
	ATT7022EU_Write_Reg(0x07,RTU_Config.Config.APower_Check);
	ATT7022EU_Write_Reg(0x08,RTU_Config.Config.BPower_Check);
	ATT7022EU_Write_Reg(0x09,RTU_Config.Config.CPower_Check);
	ATT7022EU_Write_Reg(0x0A,RTU_Config.Config.APower_Check);
	ATT7022EU_Write_Reg(0x0B,RTU_Config.Config.BPower_Check);
	ATT7022EU_Write_Reg(0x0C,RTU_Config.Config.CPower_Check);
}

void ATT7022EU_init(void)
{
	ATT7022EU_Write_Reg(0xC9,0x5A);//打开校准参数写使能
	ATT7022EU_Write_Reg(0xC3,0x00);//清校表数据
	ATT7022EU_Write_Reg(0x01,0xb9fe);//使能打开电压采集
	ATT7022EU_Write_Reg(0x03,0xf80c);//读后不清零
	ATT7022EU_Write_Reg(0x31,0x3437);
	ATT7022EU_Write_Reg(0x02,0x0000);//电流/电压增益为1倍
	ATT7022EU_Write_Reg(0x1e,59);
	ATT7022EU_Write_Reg(0x37,0x00);
	ATT7022EU_Write_Reg(0x1d,0x0160);
	ATT7022EU_Write_Reg(0x36,0x30);
	spi_DL_Check();
}

void ATT7022EU_Thread(void)
{
	float f_value=0;
	float Ugain=0;
	unsigned int IValue=0;
	static int adjust_times=0;
	static int clear_times=0;

	if(fxms_flag==1)
	{
		fxms_flag=0;

		if(read_adjust_flag())
			adjust_times+=1;
		
		if((adjust_times>=3)&&(DC_Check_flag==0))
		{		
			led_on(ADJUST_START_LED);
			if(!read_adjust_flag())
			{
				DC_Check_flag=1;
				adjust_times=0;
				led_off(ADJUST_START_LED);
			}					
		}


		if(read_clear_flag())   
			clear_times+=1;

		if((clear_times>=3)&&(Clear_SumPower_Flag==0))
		{		
			led_on(CLEAR_START_LED);
			if(!read_clear_flag())
			{
				Clear_SumPower_Flag=1;
				clear_times=0;
				led_off(CLEAR_START_LED);
			}					
		}
		if(Clear_SumPower_Flag==1)
		{
			Clear_SumPower_Flag=0;
			DL_Data.Data.SumActivePower=0;
			DL_Data.Data.SumReactivePower=0;
			SumActivePower_reg=0;
			SumReactivePower_reg=0;			
			DL_Save_Data();			
			DL_Read_Data();
		}
		
		ATT7022EU_Read_Data();
		if(DC_Check_flag==1)
		{//解除电参校准
			//DC_Check_flag=0;
			DC_Check_flag=3;
			
			
			ATT7022EU_Write_Reg(0x17,0);
			ATT7022EU_Write_Reg(0x18,0);
			ATT7022EU_Write_Reg(0x19,0);
			ATT7022EU_Write_Reg(0x1a,0);
			ATT7022EU_Write_Reg(0x1b,0);
			ATT7022EU_Write_Reg(0x1c,0);			
			ATT7022EU_Write_Reg(0x04,0);
			ATT7022EU_Write_Reg(0x05,0);
			ATT7022EU_Write_Reg(0x06,0);
			ATT7022EU_Write_Reg(0x07,0);
			ATT7022EU_Write_Reg(0x08,0);
			ATT7022EU_Write_Reg(0x09,0);
			ATT7022EU_Write_Reg(0x0A,0);
			ATT7022EU_Write_Reg(0x0B,0);
			ATT7022EU_Write_Reg(0x0C,0);
	 
		}
		else if(DC_Check_flag==2)
		{//电参校准
			DC_Check_flag=0;

			if((DL_Data.Data.AVoltage_Valid>0)&&(DL_Data.Data.BVoltage_Valid>0)&&(DL_Data.Data.CVoltage_Valid>0))
			{
				f_value=((float)DL_Data.Data.AVoltage_Valid)/100;
				Ugain=U_REF/f_value-1;
				if(Ugain>=0) Ugain=Ugain*32768;
				else Ugain=Ugain*32768+65536; 	
				RTU_Config.Config.AVoltage_Check=Ugain;
				f_value=((float)DL_Data.Data.BVoltage_Valid)/100;
				Ugain=U_REF/f_value-1;
				if(Ugain>=0) Ugain=Ugain*32768;
				else Ugain=Ugain*32768+65536; 	
				RTU_Config.Config.BVoltage_Check=Ugain;
				f_value=((float)DL_Data.Data.CVoltage_Valid)/100;
				Ugain=U_REF/f_value-1;
				if(Ugain>=0) Ugain=Ugain*32768;
				else Ugain=Ugain*32768+65536; 	
				RTU_Config.Config.CVoltage_Check=Ugain;

#if 1				
				myprintf("******************************************** \n");
				myprintf("DL_Data.Data.ACurrent_Valid:0x%x \n",DL_Data.Data.ACurrent_Valid);
				myprintf("DL_Data.Data.BCurrent_Valid:0x%x \n",DL_Data.Data.BCurrent_Valid);
				myprintf("DL_Data.Data.CCurrent_Valid:0x%x \n",DL_Data.Data.CCurrent_Valid);
				myprintf("DL_Config.Config.CurrentRatio:0x%x \n",DL_Config.Config.CurrentRatio);
				myprintf("******************************************** \n");

#endif				
				if((DL_Data.Data.ACurrent_Valid>0)&&(DL_Data.Data.BCurrent_Valid>0)&&(DL_Data.Data.CCurrent_Valid>0)&&(DL_Config.Config.CurrentRatio>0))
				{
					f_value=((float)DL_Data.Data.ACurrent_Valid)/100/DL_Config.Config.CurrentRatio;
					Ugain=1/f_value-1;
					if(Ugain>=0) Ugain=Ugain*32768;
					else Ugain=Ugain*32768+65536; 						
					RTU_Config.Config.ACurrent_Check=Ugain;
					f_value=((float)DL_Data.Data.BCurrent_Valid)/100/DL_Config.Config.CurrentRatio;
					Ugain=1/f_value-1;
					if(Ugain>=0) Ugain=Ugain*32768;
					else Ugain=Ugain*32768+65536;  	
					RTU_Config.Config.BCurrent_Check=Ugain;
					f_value=((float)DL_Data.Data.CCurrent_Valid)/100/DL_Config.Config.CurrentRatio;
					Ugain=1/f_value-1;
					if(Ugain>=0) Ugain=Ugain*32768;
					else Ugain=Ugain*32768+65536;  
					RTU_Config.Config.CCurrent_Check=Ugain;

					IValue=ATT7022EU_Read_Reg(0x01);
					if(IValue > 8388608) IValue = IValue - 16777216;
					f_value = ((float)IValue)/ATT7022EU_K;	
					f_value=(f_value-U_REF)/U_REF;
					Ugain=(0-f_value)/(1+f_value);
					if(Ugain>=0) Ugain=Ugain*32768;
					else Ugain=Ugain*32768+65536; 
					RTU_Config.Config.APower_Check=Ugain;	

					IValue=ATT7022EU_Read_Reg(0x02);
					if(IValue > 8388608) IValue = IValue - 16777216;
					f_value = ((float)IValue)/ATT7022EU_K;	
					f_value=(f_value-U_REF)/U_REF;
					Ugain=(0-f_value)/(1+f_value);
					if(Ugain>=0) Ugain=Ugain*32768;
					else Ugain=Ugain*32768+65536; 
					RTU_Config.Config.BPower_Check=Ugain;	

					IValue=ATT7022EU_Read_Reg(0x03);
					if(IValue > 8388608) IValue = IValue - 16777216;
					f_value = ((float)IValue)/ATT7022EU_K;	
					f_value=(f_value-U_REF)/U_REF;
					Ugain=(0-f_value)/(1+f_value);
					if(Ugain>=0) Ugain=Ugain*32768;
					else Ugain=Ugain*32768+65536; 
					RTU_Config.Config.CPower_Check=Ugain;		
					spi_DL_Check();
					myprintf("RTU_Config.Head=0x%h\n",RTU_Config.Head);
					MRAM_WRITE((unsigned char *)(&RTU_Config.Head), Main_Procedure_Buf_Addr, sizeof(_RTU_Config));
				}
			}
		}
		else if(DC_Check_flag==3)
		{//默认值电参校准
			DC_Check_flag=2;
			return ;
			
			RTU_Config.Config.AVoltage_Check=0x4F41;
			RTU_Config.Config.BVoltage_Check=0x4F41;
			RTU_Config.Config.CVoltage_Check=0x4F41;
			RTU_Config.Config.ACurrent_Check=0x8B60;
			RTU_Config.Config.BCurrent_Check=0x8B60;
			RTU_Config.Config.CCurrent_Check=0x8B60;
			RTU_Config.Config.APower_Check=0x0E72;
			RTU_Config.Config.BPower_Check=0x0E72;
			RTU_Config.Config.CPower_Check=0x0E72;
			
			ATT7022EU_Write_Reg(0x17,RTU_Config.Config.AVoltage_Check);
			ATT7022EU_Write_Reg(0x18,RTU_Config.Config.BVoltage_Check);
			ATT7022EU_Write_Reg(0x19,RTU_Config.Config.CVoltage_Check);

			ATT7022EU_Write_Reg(0x1a,RTU_Config.Config.ACurrent_Check);
			ATT7022EU_Write_Reg(0x1b,RTU_Config.Config.BCurrent_Check);
			ATT7022EU_Write_Reg(0x1c,RTU_Config.Config.CCurrent_Check);
			
			ATT7022EU_Write_Reg(0x04,RTU_Config.Config.APower_Check);
			ATT7022EU_Write_Reg(0x05,RTU_Config.Config.BPower_Check);
			ATT7022EU_Write_Reg(0x06,RTU_Config.Config.CPower_Check);
			ATT7022EU_Write_Reg(0x07,RTU_Config.Config.APower_Check);
			ATT7022EU_Write_Reg(0x08,RTU_Config.Config.BPower_Check);
			ATT7022EU_Write_Reg(0x09,RTU_Config.Config.CPower_Check);
			ATT7022EU_Write_Reg(0x0A,RTU_Config.Config.APower_Check);
			ATT7022EU_Write_Reg(0x0B,RTU_Config.Config.BPower_Check);
			ATT7022EU_Write_Reg(0x0C,RTU_Config.Config.CPower_Check);
		
			MRAM_WRITE((unsigned char *)(&RTU_Config.Head), Main_Procedure_Buf_Addr, sizeof(_RTU_Config));
		}
	}
}

