#include "rtc.h"

///////////////////////////////////////////////////////////////////////////////
u16 volatile g_ms_uw=0;
u16 g_min=0;
u16 g_hour=0;
u16 g_day=0;

///////////////////////////////////////////////////////////////////////////////
void rtc_setting(void)
{
	RTC_KSCCFG     =  0x0003;      // Module Enable
	_nop_(); _nop_();
	SCU_RTCCLKCON  =  0x0005; 
	
	// FOSC=8MHz => 31.250kHz
	while(!(RTC_CON & 0x8000));    // Wait until access is possible
	RTC_CON        =  0x0002;
	
	// Overflow period on T14,CNT0-CNT3:  1.0240 [ms] 1.0486 [s] 1.1185 [min] 1.1930 [h] 1.6968 [months]
	
	RTC_T14REL     =  0xFFE0;
	RTC_T14        =  RTC_T14REL;
	RTC_RTCL       =  0x0000;
	RTC_RELL       =  0x0000;
	RTC_RTCH       =  0x0000;
	RTC_RELH       =  0x0000;
	
	RTC_IC         =  0x005C;     
	RTC_ISNC       =  0x0001;
	
	RTC_CON_RUN    =  1;
}

///////////////////////////////////////////////////////////////////////////////
// Period: 1.024ms reserved for future use
void isr_rtc(void) interrupt RTCINT
{
	if(RTC_ISNC_T14IR)
	{
		RTC_ISNC_T14IR = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////

