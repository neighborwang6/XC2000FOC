///////////////////////////////////////////////////////////////////////
#include "adc.h"

///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// ADC0
u16 g_ad0[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const u8 ad0_chx[16]={0,2,3,4,5,8,9,10,11,13,15,255};

// ADC1
u16 g_ad1[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const u8 ad1_chx[16]={0,2,4,5,6,255};

///////////////////////////////////////////////////////////////////////
void ad_setting(void)
{
	ADC0_KSCFG     =  0x0003;      //ADC0 kernel configuration : fADC=80MHz
	_nop_();  _nop_();
	
	// conversion time (CTC)    = 03.09 us
	// Analog clock is 1/12th of module clock and digital clock is 1/1 times 
	// of module clock
	
	// Permanent arbitration mode
	ADC0_GLOBCTR   =  0x000B;
	
	// Arbitration Slot 0 is enabled
	// wait-for-start mode
	ADC0_ASENR     =  0x0001;
	ADC0_RSPR0     =  0x0000;
	
	// Configuration of Channel Control Registers:
	// the result register0 is selected
	// the reference voltage selected is Standard Voltage (Varef)
	// the input class selected is Input Class 0

	ADC0_CHCTR0    =  0x0004;
	ADC0_CHCTR1    =  0x0004;
	ADC0_CHCTR2    =  0x0004;
	ADC0_CHCTR3    =  0x0004;
	ADC0_CHCTR4    =  0x0004;
	ADC0_CHCTR5    =  0x0004;
	ADC0_CHCTR6    =  0x0004;
	ADC0_CHCTR10   =  0x0004;
	ADC0_CHCTR11   =  0x0004;
	ADC0_CHCTR12   =  0x0004;
	
	// 10 bit resolution selected
	ADC0_INPCR0    =  0x0000;
	ADC0_INPCR1    =  0x0000;
	
	// Configuration of Result Control Registers:
	// the data reduction filter is disabled
	// the event interrupt is disabled
	// the wait-for-read mode is enabled
	// the FIFO functionality is disabled
	ADC0_RCR0      =  0x0040;
	ADC0_RCR1      =  0x0000;
	ADC0_RCR2      =  0x0000;
	ADC0_RCR3      =  0x0000;
	ADC0_RCR4      =  0x0000;
	ADC0_RCR5      =  0x0000;
	ADC0_RCR6      =  0x0000;
	ADC0_RCR7      =  0x0000;
	
	// Configuration of Channel Interrupt Node Pointer Register:
	ADC0_CHINPR0   =  0x0000;
	ADC0_CHINPR4   =  0x0000;
	ADC0_CHINPR8   =  0x0000;
	ADC0_CHINPR12  =  0x0000;
	
	// Configuration of Event Interrupt Node Pointer Register
	ADC0_EVINPR0   =  0x0000;
	ADC0_EVINPR8   =  0x0000;
	ADC0_EVINPR12  =  0x0000;
	
	ADC0_QMR0      =  0x0001;
	ADC0_QMR2      =  0x0000;
	
	// Configuration of Conversion Request Mode Registers:Parallel Source 
	// the gating line is permanently Disabled
	// the external trigger is disabled
	// the source interrupt is disabled
	// the autoscan functionality is disabled
	ADC0_CRMR1     =  0x0000;      // conversion request mode register 1
	
	ADC0_SYNCTR   |=  0x0010;
	P5_DIDIS       =  0x1C7F;      // Digital input disable register
	ADC0_GLOBCTR  |=  0x0300;      // turn on Analog part

	////////////////////////////////////////////////////////
	// ADC0_KSCFG = 0x0003;
	_nop_(); _nop_();
	
	ADC1_GLOBCTR   =  0x000B;
	ADC1_ASENR     =  0x0001;
	ADC1_RSPR0     =  0x0000;
	
	ADC1_CHCTR0    =  0x0004;
	ADC1_CHCTR1    =  0x0004;
	ADC1_CHCTR2    =  0x0004;
	ADC1_CHCTR3    =  0x0004;
	ADC1_CHCTR4    =  0x0004;
	ADC1_CHCTR5    =  0x0004;
	ADC1_CHCTR6    =  0x0004;
	
	ADC1_INPCR0    =  0x0000;
	ADC1_INPCR1    =  0x0000;
	
	ADC1_RCR0      =  0x0040;
	ADC1_RCR1      =  0x0000;
	ADC1_RCR2      =  0x0000;
	ADC1_RCR3      =  0x0000;
	ADC1_RCR4      =  0x0000;
	ADC1_RCR5      =  0x0000;
	ADC1_RCR6      =  0x0000;
	ADC1_RCR7      =  0x0000;
	
	ADC1_CHINPR0   =  0x0000;
	ADC1_CHINPR4   =  0x0000;
	
	ADC1_EVINPR0   =  0x0000;
	ADC1_EVINPR8   =  0x0000;
	ADC1_EVINPR12  =  0x0000;
	
	ADC1_QMR0      =  0x0001;
	ADC1_QMR2      =  0x0000;
	ADC1_CRMR1     =  0x0000;
	
	ADC1_SYNCTR   |=  0x0010;
	P15_DIDIS      =  0x007F;
	ADC1_GLOBCTR  |=  0x0300;

	////////////////////////////////////////////////////////
	ADC0_QINR0=0;
	ADC1_QINR0=0;
	////////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////////
void ad_scan(void)
{
	static u8 ad0_idx=0;
	static u8 ad1_idx=0;

	u8 chnum;
	u16 val;

	// ADC0 Data Valid
	if(!(ADC0_GLOBSTR & 0x0001) && (ADC0_RCR0 & 0x1000))
	{
	    chnum=(u8)(ADC0_RESRV0 >> 12);
		val=(ADC0_RESR0 & 0x0fff) >> 2;
		g_ad0[chnum]=val;

	    if(ad0_chx[++ad0_idx]==(u8)255)
	       ad0_idx=0;
		ADC0_QINR0=ad0_chx[ad0_idx];
	}

	// ADC1 Data Valid
	if(!(ADC1_GLOBSTR & 0x0001) && (ADC1_RCR0 & 0x1000))
	{
		chnum=(u8)(ADC1_RESRV0 >> 12);
		val=(ADC1_RESR0 & 0x0fff) >> 2;
		g_ad1[chnum]=val;

	    if(ad1_chx[++ad1_idx]==(u8)255)
	       ad1_idx=0;
		ADC1_QINR0=ad1_chx[ad1_idx];
	}
}
/////////////////////////////////////////////////////////
u16 get_ad_value(u8 chan_no)
{
   u16 g_ad_tmp=0;

   if((chan_no&0xf0)==0x10) // ADC1
   {
      g_ad_tmp=(u16)g_ad1[chan_no&0x0f];
   }
   else           // ADC0
   {
      g_ad_tmp=(u16)g_ad0[chan_no];
   }

   return g_ad_tmp;
}
/////////////////////////////////////////////////////////
