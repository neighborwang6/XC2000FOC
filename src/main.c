////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
*
*  
*      
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

#include "std_defs.h" 
#include "board.h"
#include "main_task.h"
#include "diag_hw.h"
#include "hican.h"

#include "resolver_ad2s.h"
#include "pwm.h"

#include "ctrl.h"
#include "ctrl_if.h"

#ifdef WIN32
#include "pc_hw.h"
#endif

////////////////////////////////////////////////////////////////////////////////
// Global functions
void app_init(void);
void app_10ms_task(void);
void app_5ms_task(void);
void app_mainloop_task(void);

u16 TryValue1=0,TryValue2=0,TryValue3=0,TryValue4=0,TryValue5=0,TryValue6=0,TryValue7=0,TryValue9=0,TryValue10=0,TryValue11=0,TryValue12=0;
s16 TryValue8=0;
s32 TryValue13;
extern u16 g_ctrl_test_timer_uw;
extern u16 test_dir;
extern u16 motor_offset;
extern u16 TryVqs;
extern u16 g_disp_value_sent; //Relative Displacement - RT (Origional:CC2_CC22(PWM_EX3)) Relstive Displacement define in ccu2_ccu6.c
U16 DEBUG_VAL=0;
////////////////////////////////////////////////////////////////////////////////
// Main entry
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	app_init();

    while (1)
	{   
		//////////////////////////////////////////////////////
		// 5ms task
		if (g_overflow_5ms_ub == 1)
		{
			g_overflow_5ms_ub=0;
			app_5ms_task();
			
			TryValue1=get_disp_sensor0();  //PWM_EX0
			TryValue2=get_mc_pressure();  //PWM_EX1
			TryValue3=get_brk_pedal_state();//PWM_EX2
			TryValue4=g_disp_value_sent;  //PWM_EX3
			//TryValue3=get_brk_pedal_volt();
			//TryValue4=get_bat_volt();
			//TryValue5=get_ign_volt();
			TryValue6=get_motor_angle_m();
			//TryValue7=get_motor_angle_e();
			//TryValue8=get_motor_speed();
			//TryValue9=get_temp_sensor_pcb();
			//TryValue10=get_temp_sensor_mosfet();
			//TryValue9=g_ctrl_test_timer_uw;
			//TryValue10=test_dir;
			//TryValue11=motor_offset;
			//TryValue12=TryVqs;
			TryValue13 = get_motor_total_angle();
			
		}

		//////////////////////////////////////////////////////
		// 10ms task
		if (g_overflow_10ms_ub == 1)
        {  
      g_overflow_10ms_ub = 0;
			app_10ms_task();
        }

		//////////////////////////////////////////////////////
		// Mainloop task
		app_mainloop_task();
		
		//////////////////////////////////////////////////////
    } // while(1)...
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void app_init(void)
{
#ifdef WIN32
	pc_hw_init();
#else	
	hw_init();
#endif
	
	diag_hw_init();

	//////////////////////////////////////////////////////
	// Special instruction for XC2000 MCU
    _einit_(); 
	
	//////////////////////////////////////////////////////
	// FIXED: yly
	keep_alive(1); //Power Hold.
	
	//////////////////////////////////////////////////////
	// Wait the power stable
	SET_BAT12V_SW(1); delay_ms(10); //Open the power of XUAN BIAN RESOVER.
	SET_AD2S_RESET(1); delay_ms(10);
	
	// Motor control OFF
	set_motor_ctrl_enable(0);
	
	// VCC5 for Torque sensor ON.(The connector VCC5)
	SET_VCC5_SS(1);
	
	//TLE9180D-31QK: -INH
	SET_SCDL_DIS(0);
	
	//////////////////////////////////////////////////////
	// EEPROM Init
	g_sys_dtc_uw |= eeprom_init();
	
	//////////////////////////////////////////////////////
	// Init the resolver decoder
	//resolver_ad2s_init(0);
	
	// PWM Init
	//pwm_init();
	
	//////////////////////////////////////////////////////
	// Motor control OFF (But enable)
	//set_motor_ctrl_enable(1);
	
	//////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void app_10ms_task(void)
{
	diag_timer_10ms_hook();
	// Reset watchdog timer
	_srvwdt_();
	
	/////////////////////////////////////////////////
	isr_10ms_hook_user();
	
	do_boff_recovery();
	DEBUG_VAL = GET_IGN();
	/////////////////////////////////////////////////
	// IGN Off?
	if(!GET_IGN())
	{
		// Save something to EEPROM...
		// power_down_hook_user();
		SET_BAT12V_SW(0);
		
		keep_alive(0);  
	}
	/////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void app_5ms_task(void)
{
	isr_5ms_hook_user();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void app_mainloop_task(void)
{
#ifdef WIN32
	delay_ms_pc(1);
	do_vcan_msg();
#endif
	//////////////////////////////////////////////////////
	//
	diag_main_loop_hook(); 
}
////////////////////////////////////////////////////////////////////////////////
