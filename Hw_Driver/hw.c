#include "hw.h"
#include "ssc.h"
#include "rtc.h"
#include "timer.h"
#include "scs.h"
#include "xc2k_can.h"
#include "adc.h"
#include "ccu2_ccu6.h"

/////////////////////////////////////////////////////////

u8 volatile g_overflow_5ms_ub=0;
u8 volatile g_overflow_10ms_ub=0;

void mcu_unlock(void)
{
    SCU_SLC = 0xAAAA;                   // command 0
    SCU_SLC = 0x5554;                   // command 1
    SCU_SLC = 0x96FF;                   // command 2
    SCU_SLC = 0x0000;                   // command 3
}
void mcu_lock(void)
{
    SCU_SLC = 0xAAAA;                   // command 0
    SCU_SLC = 0x5554;                   // command 1
    SCU_SLC = 0x96FF;                   // command 2
    SCU_SLC = 0x1800;                   // command 3;
    SCU_SLC = 0x8EFF;                   // command 4
}
/////////////////////////////////////////////////////////
u16 hw_init(void)
{
	u16 ret=0;

	PSW_IEN=0;
	mcu_unlock();
	
	SCS_InitTimer();
	SCS_GoFromBaseToNormalMode();
	SCS_RestoreTimer();

	gpio_setting();
	pll_setting();
	ad_setting();
	spi_setting();
	capcom_setting();
	rtc_setting();
	timer_setting();
	can_setting();
	ad_setting();  //Why we need init the AD module twice?
	cc2_setting();
	cc6_setting();
	
	mcu_lock();
	PSW_IEN=1;
	
	CAPCOM_ON();

	return ret;
}

/////////////////////////////////////////////////////////
void gpio_setting(void)
{
	P7_IOCR02      =  0x0080;  // VCC5B_ENAX
	P7_IOCR00      =  0x0080;  // VCC5_HOLDX
	P7_IOCR03      =  0x0080;  // AD2S_RESET
	P7_IOCR01      =  0x0002;  // ERR1 (pull up)
	P7_IOCR04      =  0x0080;  // U1_EN
	P6_IOCR00      =  0x0000;  // ISN1X1
	P6_IOCR01      =  0x0000;  // ISP1X1
	P6_IOCR02      =  0x0080;  // AD2S_DIR
	P2_IOCR12      =  0x0080;  // AD2S_WR
	P2_IOCR11      =  0x0080;  // AD2S_RD
	
	P2_IOCR01      =  0x0090;  // TXCAN0
	P2_IOCR00      =  0x0000;  // RXCAN0
	P2_IOCR02      =  0x0080;  // SCDL_DIS
	P4_IOCR00      =  0x0080;  // AD2S_A0A1
	
	P0_IOCR00      =  0x00B0;  // IH1
	P0_IOCR01      =  0x00B0;  // IH2
	P0_IOCR02      =  0x00B0;  // IH3
	P0_IOCR03      =  0x00B0;  // IL1
	P0_IOCR04      =  0x00B0;  // IL2
	P0_IOCR05      =  0x00B0;  // IL3

	P2_IOCR07      =  0x0000;  // CC2_CC20
	P2_IOCR08      =  0x0000;  // CC2_CC21
	P2_IOCR09      =  0x0000;  // CC2_CC22
	//P2_IOCR10      =  0x0000;  // CC2_CC23
  P2_IOCR10      =  0x0010; //  ///  P2.10 is used as general input - pull-down device is assigned

	P4_IOCR02      =  0x0000;  // CC2_CC26
	P4_IOCR03      =  0x0000;  // CC2_CC27

	P2_IOCR13      =  0x0080;  // AD2S_CS
	
	P10_IOCR04     =  0x0080;  // Q1_G_CTRLX

	P10_IOCR05     =  0x0080;  // EE_SCK
	P10_IOCR08     =  0x0080;  // EE_CS
	P10_IOCR14     =  0x0000;  // EE_SDO
	P10_IOCR15     =  0x0080;  // EE_SDI

	P10_IOCR07     =  0x0080;  // EN_UVWX
	P10_IOCR06     =  0x0000;  // DIS_UVW_X (Monitor the control of MCU2)
	P0_IOCR06      =  0x0080;  // AD2S_SAMPLE
	P0_IOCR07      =  0x0080;  // ENA
	
	P5_DIDIS       =  0xFFFF;
	P15_DIDIS      =  0xCF;  // P15.5: ERR2 (digital input enable)
	
	P2_IOCR03      =  0x0080;  // Debug only	
}

/////////////////////////////////////////////////////////
void pll_setting(void)
{
	SCU_EXTCON     =  0;  // External Clock Control Register
}

/////////////////////////////////////////////////////////
void capcom_setting(void)
{
}

/////////////////////////////////////////////////////////
// Use instrument to get an acceptable 1us tick
void delay_us(u16 tm)
{
	u16 idx,i;
	for(idx=0;idx<tm;idx++)
		for(i=0;i<37;i++);   // 80MHz,Large Memory Mode, XC2000 core
}

/////////////////////////////////////////////////////////
// Not Accurate Delay
void delay_ns(u8 tm)
{
	u16 idx,i;
	for(idx=0;idx<tm;idx++)
		for(i=0;i<2;i++);   // 80MHz,Large Memory Mode, XC2000 core
}
/////////////////////////////////////////////////////////
// Not Accurate Delay
void delay_msx(u16 tm)
{
   u16 idx;

   for(idx=0;idx<tm;idx++)
      delay_us(1000);
}

/////////////////////////////////////////////////////////
void delay_ms(u16 tm)
{
   WAIT_EVENT_READY((0),tm);
/*
   u16 tm0,tm1;

   tm0=g_ms_uw;
   while(1)
   {
      tm1=g_ms_uw-tm0;
      _srvwdt_(); 
      if(tm1>=tm)
	     break;
   }
*/
}
/////////////////////////////////////////////////////////
void keep_alive(u8 on_off)
{
	if(on_off)
	{
		SET_VCC5_HOLD(0);
	}
	else
	{
		SET_VCC5_HOLD(1);
	}
}
/////////////////////////////////////////////////////////
