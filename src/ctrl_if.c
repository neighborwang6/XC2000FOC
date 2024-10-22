////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
*
*  
*      
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

#include "ctrl_if.h"
#include "board.h"
#include "motor_if.h"
#include "svpwm.h"

#define GET_BRK_PEDAL_STATE() (P2_IN_P10)

#define TLE9180D_31QK
//#define TLE7189F

////////////////////////////////////////////////////////////////////////////////
// Brake command from CAN bus
u16 g_brake_pedal_pos_can_uw=0;

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 get_brake_pedal_pos0(void)
{
	return get_ad_value(AD_BRK0);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 get_brake_pedal_pos1(void)
{
	return get_ad_value(AD_BRK0);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 get_brake_pedal_pos_can(void)
{
	return g_brake_pedal_pos_can_uw;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 get_mc_pressure0(void)
{
	return get_ad_value(AD_PRS0);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 get_mc_pressure1(void)
{
	return get_ad_value(AD_PRS0);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
s16 get_steer_torque0(void)
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
s16 get_steer_torque1(void)
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 get_motor_angle_m(void)
{
	return g_angle_sensor;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 get_motor_angle_e(void)
{
	return g_angle_eiq;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
s32 get_motor_total_angle(void)
{
	return ((g_total_angle_sl*90)>>12);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
s16 get_motor_speed(void)
{
	return g_mot_speed_sw;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 set_motor_speed(s16 speed) // reserved
{
	UNUSED_PARA(speed);

	return 1;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 set_motor_torque(s16 torq) // reserved
{
	UNUSED_PARA(torq);

	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// volt: 0~12V: => 0~120 (0.1V/bit)
////////////////////////////////////////////////////////////////////////////////
u8 set_motor_volt(u16 volt)
{
	g_duty_uw=(u16)((10000UL*volt)/Udc);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 set_motor_dir(u8 dir)
{
	if(dir>1)
		return 1;

	g_dir_ub=dir;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// 0: disable, 1: enable level1, 2: enable_level2
////////////////////////////////////////////////////////////////////////////////
u8 set_motor_ctrl_enable(u8 enable)
{
	if(enable==1)
	{
		SET_TLE_ENA(1);
		SET_EN_UVW(1);
		set_phase_on(0,0);
		set_phase_on(1,0);
		set_phase_on(2,0);

		g_time_inh=(u16)0xFFFF;
	}
	else if(enable==2)
	{
		SET_TLE_ENA(1);
		SET_EN_UVW(1);
		set_phase_on(0,1);
		set_phase_on(1,1);
		set_phase_on(2,1);

		g_time_inh=0;
	}
	else if(enable==0)
	{
		SET_TLE_ENA(0);
		SET_EN_UVW(0);

		set_phase_on(0,0);
		set_phase_on(1,0);
		set_phase_on(2,0);
		pwm_setduty(0,0);
		pwm_setduty(1,0);
		pwm_setduty(2,0);

		g_time_inh=(u16)0xFFFF;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 save_parameters(u8 *para_buf, u8 len)
{
	UNUSED_PARA(para_buf);
	UNUSED_PARA(len);

	return 1;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 read_parameters(u8 *para_buf, u8 len)
{
	UNUSED_PARA(para_buf);
	UNUSED_PARA(len);

	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// 0.1degree/bit
////////////////////////////////////////////////////////////////////////////////
u8 set_motor_offset(s16 offset)
{
	if(g_motor_type_ub==MOTOR_TYPE_BLDC)
		g_pos_offset_bldc_uw=(u16)offset;
	else if(g_motor_type_ub==MOTOR_TYPE_PMSM)
		g_pos_offset_pmsm_uw=(u16)offset;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 get_bat_volt(void)
{
	u16 l_tmpu16;

	l_tmpu16=get_ad_value(AD_BAT12V_MON);
	// Resistor divider : 1M/330K
	// 1023 => 5V, Output adjusted to 0.1V/bit
	l_tmpu16=(l_tmpu16*25)>>7; //l_tmpu16*4*5*10/1024;

	return l_tmpu16;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 get_mc_pressure(void)
{
	u16 l_tmpu16;

	// VRO remap to MC pressure input.
	l_tmpu16=get_ad_value(AD_AIN3);

	return l_tmpu16;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 get_disp_sensor0(void)
{
	u16 l_tmpu16;

	l_tmpu16=get_ad_value(AD_BRK0);

	return l_tmpu16;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 get_ign_volt(void)
{
	// IGN: digital input in DIAS EPS ECU
	if(GET_IGN())
		return get_bat_volt();
else
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Brake pedal light switch 1:Press the pedal ; 0:Do not press the pedal.
////////////////////////////////////////////////////////////////////////////////
u16 get_brk_pedal_state(void)
{
	if(GET_BRK_PEDAL_STATE())
	{
		return 1;
	}
	else
	{
		return 0;
		
	}
}

////////////////////////////////////////////////////////////////////////////////
// No brake pedal switch available to eBooster
////////////////////////////////////////////////////////////////////////////////
u16 get_brk_pedal_volt(void)
{
	return (u16)get_ad_value(AD_VO2);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 get_current(u8 i_idx)
{
	u16 l_tmpu16=0;

	switch(i_idx)
	{
	case 0:
		l_tmpu16=get_ad_value(AD_VO1);
		break;

	case 1:
	case 2:
	case 3:
		l_tmpu16=(u16)0xFFFF;
		break;
	}

	return l_tmpu16;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 get_temp_sensor_pcb(void)
{
	return get_ad_value(AD_TEMP_SS1);
}

////////////////////////////////////////////////////////////////////////////////
// Not mount: unavailable
////////////////////////////////////////////////////////////////////////////////
u16 get_temp_sensor_mosfet(void)
{
	return (u16)0xFFFF;
}

////////////////////////////////////////////////////////////////////////////////
