////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
*
*  
*      
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

#include "motor_if.h"
#include "motor_pmsm.h"
#include "resolver_ad2s.h"

#include "filters.h"

///////////////////////////////////////////////////////////////////////////////
//
void calc_motor_pos(void);
///////////////////////////////////////////////////////////////////////////////
extern u16 g_duty_uw;
extern u16 g_pos_offset_pmsm_uw;

extern u16 g_time_inh;
///////////////////////////////////////////////////////////////////////////////
//	
I_CLARK_PARK I={0,0,0,0,0,0,0,0,0,0};
V_CLARK_PARK V={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

s16 Vqs_cmd=0;

u16 g_angle_sensor=0;
u16 g_angle_eiq=0;
s16 g_angle_offset=0;

///////////////////////////////////////////////////////////////////////////////
// Called very 0.1ms
//
///////////////////////////////////////////////////////////////////////////////
void do_motor_pmsm_ctrl(void)
{
	s16 l_vqs;
	
	if(g_time_inh)
	{
		if(g_time_inh!=(u16)0xFFFF)
			g_time_inh--;
		
		return;
	}
	
	// Calculate the Mechanical and Electrical angle
	calc_motor_pos();
	
	// Calculate the angle sum in the rotational direction
	// calc_motor_angle(&g_motor_angle);

	I.Angle=g_angle_eiq;
	
	Park(&I);
	V.Angle = I.Angle;
	V.Sine  = I.Sine;
	V.Cosine= I.Cosine;
	
	if(g_duty_uw>1000)  // Limit to 100% of Udc (Const nominal value: 12V)
		g_duty_uw=1000;

	// 0.001V/bit
	l_vqs=(Udc/1000)*g_duty_uw; //Should be: 'Udc*g_duty_uw/1000';
	Vqs_cmd=vqs_cmd_filter(l_vqs);
	
	// Debug only: More than 8V is prohibited to protect the hardware before PMSM control verified.
	if(Vqs_cmd>F_MOT_VOLT(8))
		Vqs_cmd=F_MOT_VOLT(8);

	// Set Vd to ZERO
	V.Ds=0;
	// Set Vq according to direction: 0.001V/bit
	V.Qs=(g_dir_ub==0)?(Vqs_cmd):(-Vqs_cmd);

	// Inverse Park Transform
	IPark(&V);
	// Sector Selection
	motor_sector_cal(&V);
	// SVPWM
	motor_sector_switch_cal(&V);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void calc_motor_pos(void)
{
	u32 tmpu32;
	u16 l_pole_pair_idx;
	s16 tmps16;
	
	// 1. Calculate the rotator angle (Mechanical angle): 0.1degree/bit (scale factor: 10)
	tmpu32=g_resolver_pos_uw;  // Measurement from resolver: 12bit
	tmpu32*=F_ANGLE(360);
	tmpu32>>=12;
	g_angle_sensor=(u16)tmpu32;
	
	// 2. Set offset
	g_angle_offset=(s16)g_pos_offset_pmsm_uw;

	// 3. Adjust to Electrical angle
	// Find the sector number
	l_pole_pair_idx=(u16)(tmpu32/ANGLE_PER_POLE_PAIR);
	// Span to 360 degree
	tmps16 = (g_angle_sensor - l_pole_pair_idx*ANGLE_PER_POLE_PAIR)*POLE_PAIR_NUM;
	// Add offset
	tmps16 += g_angle_offset;
	
	// Reasonable check
	if (tmps16 >= F_ANGLE(360))
		g_angle_eiq = tmps16 - F_ANGLE(360);
	else if (tmps16 < 0)
		g_angle_eiq = tmps16 + F_ANGLE(360);
	else
		g_angle_eiq=tmps16;

	//////////////////////////////////////////////////////
	// Debug only
	// g_angle_eiq=1500;
}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
s32 g_sum_vqs_sl=0;

s16 vqs_cmd_filter(s16 vqs)
{
	s16 l_vqs;
	
	l_vqs=vqs;
	
#if 0	
	s32 sum_temp;

	g_sum_vqs_sl+=vqs;
	g_sum_vqs_sl-=Vqs_cmd;
	sum_temp=g_sum_vqs_sl;
	sum_temp=(sum_temp*1)>>8;
	if(vqs<Vqs_cmd) // Decrease torque, do it right away
		l_vqs=vqs;
	else
		l_vqs=(s16)sum_temp;
#endif
	
	return l_vqs;
}
////////////////////////////////////////////////////////////////////////////////
