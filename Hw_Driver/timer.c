#include "hw.h"
#include "timer.h"
#include "rtc.h"

#include "hican.h"
#include "motor_if.h"
#include "ctrl_if.h"

void isr_timer_hook_user(void);
void isr_100us_timer_hook_user(void);

///////////////////////////////////////////////////////////////////////////////
// Find main loop timer in module: MAIN_TASK.C
///////////////////////////////////////////////////////////////////////////////
void timer_setting(void)
{
	GPT12E_KSCCFG  =  0x0003;      // Module Enable
	_nop_(); _nop_();
	_nop_(); _nop_();

	GPT12E_T3CON   =  0x0000;      // 0.1us/tick

	// T4: reload to T3, generate 1ms tick
	GPT12E_T4CON   =  0x0027;
	// GPT12E_T4      =  (u16)(-(125*FSYS_MHZ));  // 1ms
	GPT12E_T4      =  (u16)(-(12.5*FSYS_MHZ));  // 0.1ms
	GPT12E_T3      =  GPT12E_T4;
	
	GPT12E_T3IC	      =  0x40|(11<<2)|(1);
	GPT12E_T3CON_T3R  =  1;
}

///////////////////////////////////////////////////////////////////////////////
// Period: 0.1ms
// High load of this function is absolutely prohibited in order to give real-time base

void isr_systick_timer(void) interrupt T3INT
{
	static u8 tmx=0;

	SET_MCU1_MOSI(0); // Debug only: Time consumption test
	if((++tmx)>=10)	     // 0.1ms tick: 1ms task (10ms in PC simulation)
	{
		isr_timer_hook();
		isr_timer_hook_user();  //CAN
		tmx=0;
	}
	SET_MCU1_MOSI(1); // Debug only: Time consumption test
	isr_100us_timer_hook_user();
	SET_MCU1_MOSI(0); // Debug only: Time consumption test
}
///////////////////////////////////////////////////////////////////////////////
// 1ms timer
void isr_timer_hook(void) 
{ 
	static u8 tmx=0;
	static u8 tmx_5ms=0;

    g_ms_uw++;

	if((++tmx)>=10)
	{
		tmx=0;
		g_overflow_10ms_ub=1;
	}

	if((++tmx_5ms)>=5)
	{
		tmx_5ms=0;
		g_overflow_5ms_ub=1;
	}
}

///////////////////////////////////////////////////////////////////////////////
void isr_timer_hook_user(void)
{
	hican_1ms_hook();
	isr_1ms_hook_user();
	calc_mot_total_angle();
}
///////////////////////////////////////////////////////////////////////////////
