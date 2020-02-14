////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
*
*  
*      
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

#include "motor_if.h"
#include "motor_bldc.h"
#include "motor_pmsm.h"
#include "resolver_ad2s.h"

////////////////////////////////////////////////////////////////////////////////
//
u8 g_motor_type_ub=MOTOR_TYPE_PMSM;

////////////////////////////////////////////////////////////////////////////////
// Variables for Motor Control
u32 g_pos=0;
u32 g_posx_uw=0;
u16 g_slot_idx=0;
u16 g_time_run=0;
u8 g_dir_ub=0;
u8 g_timer_250us=0;

u16 g_slot_idx_ss=0;
////////////////////////////////////////////////////////////////////////////////
#define DUTY_LIMIT 300
#define STALL_SPEED_LIMIT 100
#define STALL_TIME_LIMIT 1000

u16 g_duty_max=DUTY_LIMIT;
u16 g_stall_speed=STALL_SPEED_LIMIT;
u16 g_stall_time=STALL_TIME_LIMIT;

u16 g_duty_uw=0;
u16 g_step_mode_ub=1;

u16 g_pos_offset_pmsm_uw=F_ANGLE(65);
u16 g_pos_offset_bldc_uw=F_ANGLE(60);

u16 g_turns=0;
u16 g_turns_cmd=0;
u16 g_slot_bits=0;
u16 g_slot_bits0=0;

u16 g_time_inh=0xFFFF;
u16 g_timeout=0;
s16 g_mot_speed_sw=0; //RPM
u16 g_timer_calc_speed_ub=0;
extern void calc_motor_pos(void);
#define FDEGREE(x) (4096UL*(x)/360)

#ifdef WIN32
extern u16 pc_sim_get_resolver_pos(u16 para);

#endif
///////////////////////////////////////////////////////////////////////////////
//

///////////////////////////////////////////////////////////////////////////////
// Command from CAN bus
void do_motor_bldc_debug(u8 *buf)
{
	u16 l_buf[4];
	u16 *pt;
	u16 l_duty_uw;

	// Backup Duty
	l_duty_uw=g_duty_uw;
	
	// Compatible with Motorola Order of BYTEs.
	l_buf[1]=buf[0];
	l_buf[1]<<=8;
	l_buf[1]|=buf[1];
	l_buf[0]=buf[2];
	l_buf[0]<<=8;
	l_buf[0]|=buf[3];
	l_buf[3]=buf[4];
	l_buf[3]<<=8;
	l_buf[3]|=buf[5];
	l_buf[2]=buf[6];
	l_buf[2]<<=8;
	l_buf[2]|=buf[7];
	
	pt=(u16 *)l_buf;
	
	if(pt[2]!=0x55aa)
		return;
	
	switch(pt[1])
	{
	case 0x5501:  // Set Offset
		g_pos_offset_bldc_uw=pt[0];
		g_pos_offset_pmsm_uw=pt[0];
		break;
		
	case 0x5502:  // Set Duty
		l_duty_uw=pt[0];
		break;
		
	case 0x5503:  // Set Dir
		g_dir_ub=pt[0]?1:0;
		break;
		
	case 0x5504:  // Set Step Mode (Dir also valid)
		g_step_mode_ub=1;
		g_dir_ub=pt[3]?1:0;
		g_time_inh=0;
		g_turns=pt[0]*4;
		g_turns_cmd=g_turns;
		break;
		
	case 0x5505:  // Set Continuous Mode (Dir also valid)
		g_time_inh=0;
		g_step_mode_ub=0;
		g_dir_ub=pt[3]?1:0;
	
		set_phase_on(0,1);
		set_phase_on(1,1);
		set_phase_on(2,1);
		break;
		
	case 0x5506:  // Set Continuous Mode (Dir and Duty also valid)
		g_time_inh=0;
		g_step_mode_ub=0;
		g_dir_ub=pt[3]?1:0;
		l_duty_uw=pt[0];
		break;
		
	case 0x5507:  // Set Step Mode (Dir and Duty also valid)
		g_step_mode_ub=1;
		g_dir_ub=(pt[3] & 0x01)?1:0;
		pt[3]>>=1;
		l_duty_uw=pt[3];
		
		g_time_inh=0;
		g_turns=pt[0]*4;
		g_turns_cmd=g_turns;
		break;

	case 0x5508:  // Change motor type (control mode)
		g_motor_type_ub=(u8)pt[0];
		if(g_motor_type_ub!=MOTOR_TYPE_BLDC)
			g_motor_type_ub=MOTOR_TYPE_PMSM;

		break;
		
	case 0x5570:  // Emergency Stop
		g_step_mode_ub=1;
		g_time_inh=(u16)0xFFFF;
		g_time_run=0;
		
		set_phase_on(1,0);
		set_phase_on(2,0);
		set_phase_on(3,0);
		pwm_setduty(0,0);
		pwm_setduty(1,0);
		pwm_setduty(2,0);
		
		break;
		
	case 0x557F:  // Keep Link
		g_timeout=20000; // 2s
		break;
		
	default:
		break;
	}

	///////////////////////////////////////////////////////
	// Update Duty
	if(l_duty_uw!=g_duty_uw)
	{
		if(pt[3]<=950)  // Test only: FIXME
			g_duty_uw=l_duty_uw;
	}
	///////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////////////////
// Called very 0.1ms

void do_motor_bldc_ctrl(void)
{
	//calc_motor_pos();
	
	u16 gate_bits=0;
	u16 slot_idx=0;
	u16 slot_idx_bak=0;
	u16 event_flag=0;
	u16 tmpu16;
	
	////////////////////////////////////////////////////////////////////////
	//
	const u8 SLOT_IDX_F[6]={0,1,2,3,4,5};
	// const u8 SLOT_IDX_B[6]={2,1,0,5,4,3};
	const u8 SLOT_IDX_B[6]={3,4,5,0,1,2};
	////////////////////////////////////////////////////////////////////////
	//
	if(g_time_run<10000)
		g_time_run++;
	
	/**************************************************************************************
	if(g_duty_uw>g_duty_max && ((g_mot_speed_sw)?g_mot_speed_sw:(-g_mot_speed_sw))<=g_stall_speed 
		&& g_time_run>=g_stall_time) // 250ms
		g_duty_uw=200; // Max: ?% when stall
	
	**************************************************************************************/
	
	if(g_timeout)
	{
		/*********************************
		g_timeout--;
		if(g_timeout==0)
		{
			g_time_inh=(u16)0xFFFF;
			set_phase_on(1,0);
			set_phase_on(2,0);
			set_phase_on(3,0);
			g_step_mode_ub=1;
			g_time_run=0;
		}
		*********************************/
	}

	////////////////////////////////////////////////////////////////////////
	if(g_time_inh)
	{
		if(g_time_inh!=(u16)0xFFFF)
			g_time_inh--;
		
		goto func_exit;
	}
	
	// Calculate Slot Index (6-Steps/Slots for BLDC)
	tmpu16 = g_resolver_pos_uw;
	tmpu16 += g_pos_offset_bldc_uw;
	tmpu16 &= 0x0FFF;
	
	g_slot_idx_ss=(u8)(tmpu16/683);
	g_slot_idx_ss++;
	g_slot_idx_ss=(g_slot_idx_ss%6);
	
	if(g_dir_ub==0) // Forward
		g_slot_idx=SLOT_IDX_F[g_slot_idx_ss];
	else            // Backward
		g_slot_idx=SLOT_IDX_B[g_slot_idx_ss];
	
	gate_bits=0;
	
	// A: Phase 0, B: Phase 1, C: Phase 2
	switch(g_slot_idx)
	{
	case 0: // B+C-
		set_phase_on(0,0);
		pwm_setduty(2,0);
		pwm_setduty(1,g_duty_uw);
		set_phase_on(1,1);
		set_phase_on(2,1);
		
		gate_bits = gate_bits | 0x0010;
		gate_bits = gate_bits | 0x0008;
		
		g_slot_bits|=0x01;
		break;
		
	case 1: // B+A-
		set_phase_on(2,0);
		pwm_setduty(0,0);
		pwm_setduty(1,g_duty_uw);
		set_phase_on(1,1);
		set_phase_on(0,1);
		
		gate_bits = gate_bits | 0x0008;
		gate_bits = gate_bits | 0x0001;
		
		g_slot_bits|=0x02;
		break;
		
	case 2: // C+A-
		set_phase_on(1,0);
		pwm_setduty(0,0);
		pwm_setduty(2,g_duty_uw);
		set_phase_on(0,1);
		set_phase_on(2,1);
		
		gate_bits = gate_bits | 0x0001;
		gate_bits = gate_bits | 0x0020;
		
		g_slot_bits|=0x04;
		break;
		
	case 3: // C+B-
		set_phase_on(0,0);
		pwm_setduty(1,0);
		pwm_setduty(2,g_duty_uw);
		set_phase_on(1,1);
		set_phase_on(2,1);
		
		gate_bits = gate_bits | 0x0020;
		gate_bits = gate_bits | 0x0004;
		
		g_slot_bits|=0x08;
		break;
		
	case 4: // A+B-
		set_phase_on(2,0);
		pwm_setduty(1,0);
		pwm_setduty(0,g_duty_uw);
		set_phase_on(1,1);
		set_phase_on(0,1);
		
		gate_bits = gate_bits | 0x0004;
		gate_bits = gate_bits | 0x0002;
		
		g_slot_bits|=0x10;
		break;
		
	case 5: // A+C-
		set_phase_on(1,0);
		pwm_setduty(2,0);
		pwm_setduty(0,g_duty_uw);
		set_phase_on(0,1);
		set_phase_on(2,1);
		
		gate_bits = gate_bits | 0x0002;
		gate_bits = gate_bits | 0x0010;
		
		g_slot_bits|=0x20;
		break;
		
	default:
		break;
	}
	
	// Legacy: the turns of rotation might not accurate at high speed.
	// Use more appropriate calculation method, if resolver presents.
	if(g_slot_bits==0x3F && g_slot_idx==0)
	{
		g_slot_bits=0;
		
		if(g_turns)
		{
			g_turns--;
			if(g_turns==0)
			{
				if(g_step_mode_ub==1)
				{
					/************************************/
					g_time_inh=(u16)0xFFFF;
					pwm_setduty(0,0);
					pwm_setduty(1,0);
					pwm_setduty(2,0);
					
					g_time_run=0;
					/************************************/
				}
			}
		}
	}
	g_slot_bits0=g_slot_bits;
	
func_exit:
	return;
}

////////////////////////////////////////////////////////////////////////////////
//
void calc_mot_total_angle(void)
{
	s32 tmps32_a;
	s32 tmps32_b;
	
	////////////////////////////////////////////////////////
	if(g_resolver_pos_uw>=g_resolver_pos0_uw) // Forward
	{
		/**********************************************************************
		 The number of 1000: large jump indicates the overflow of resolver output.
		 If motor speed is very fast, large jump can also be observed in a calculation period (1ms).
		 e.g. 4 pair-pole@6000RPM => 100 revolutions/second => (100*4096*4/1000)/ms => 409/ms
		 So, 1000 chosed: 1000>409
		**********************************************************************/
		if(g_resolver_pos_uw>=g_resolver_pos0_uw+1000) // Large jump => overflow
		{
			tmps32_a=(s32)(4096-g_resolver_pos_uw); // 4096: Resolver 12bit
			tmps32_b=(s32)g_resolver_pos0_uw;
			tmps32_a=-(tmps32_a+tmps32_b);
		}
		else
		{
			tmps32_a=(s32)g_resolver_pos_uw;
			tmps32_b=(s32)g_resolver_pos0_uw;
			tmps32_a-=tmps32_b;
		}
	}
	else
	{
		if(g_resolver_pos_uw+1000<=g_resolver_pos0_uw) // Large jump => overflow
		{
			tmps32_a=(s32)(4096-g_resolver_pos0_uw); // 4096: Resolver 12bit
			tmps32_b=(s32)g_resolver_pos_uw;
			tmps32_a+=tmps32_b;
		}
		else
		{
			tmps32_a=(s32)g_resolver_pos_uw;
			tmps32_b=(s32)g_resolver_pos0_uw;
			tmps32_a-=tmps32_b;
		}
	}
		
	g_total_angle_sl+=tmps32_a;
	g_resolver_pos0_uw=g_resolver_pos_uw;
	////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
//
void calc_mot_speed(void)
{
	static u16 tmx_calc_speed_cnt=0;
	s32 tmps32;
	
	////////////////////////////////////////////////////////////////////////
	//
	tmx_calc_speed_cnt++;
	if(tmx_calc_speed_cnt>=200) // 20ms
	{
		tmx_calc_speed_cnt=0;
		g_timer_calc_speed_ub=1;
	}
	else
	{
		return;
	}
	
	////////////////////////////////////////////////////////////////////////
	// Calculation rotational speed
	if(g_timer_calc_speed_ub)  // 20ms
	{
		g_timer_calc_speed_ub=0;
		
		tmps32=(s32)(g_total_angle_sl-g_total_angle0_sl);
		g_total_angle0_sl=g_total_angle_sl;
		
		g_mot_speed_sw=(s16)((tmps32*375)>>11); // (tmps32*50*60)/(4096*4);
		// FIXME: Max RPM
		if(g_mot_speed_sw>5000)
			g_mot_speed_sw=5000;
		else if(g_mot_speed_sw<-5000)
			g_mot_speed_sw=-5000;
	}
}

////////////////////////////////////////////////////////////////////////////////
// 0.1ms (100us) timer callback function
//
void isr_100us_timer_hook_user(void)
{
	g_resolver_pos_uw=resolver_ad2s_read_pos(0);

#ifdef WIN32
	g_resolver_pos_uw=pc_sim_get_resolver_pos(0);
#endif
	
	if(g_motor_type_ub==MOTOR_TYPE_PMSM)
		do_motor_pmsm_ctrl();

	if(g_motor_type_ub==MOTOR_TYPE_BLDC)
		do_motor_bldc_ctrl();
	calc_motor_pos();
	calc_mot_speed();
}
////////////////////////////////////////////////////////////////////////////////
