////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
*
*  
*      
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
#ifndef _MOTOR_IF_H
#define _MOTOR_IF_H

#include "std_defs.h"
#include "pwm.h"

////////////////////////////////////////////////////////////////////////////////

enum {
	MOTOR_TYPE_BLDC = 1,
	MOTOR_TYPE_PMSM = 2,

	MOTOR_TYPE_NONE = 0
};

// #define MOTOR_MODE_BLDC
#define MOTOR_MODE_PMSM

#define F_MOT_VOLT(x) ((x)*1000)

////////////////////////////////////////////////////////////////////////////////
// Under organizing...
extern u16 g_slot_idx;
extern u8 g_dir_ub;
extern s16 g_mot_speed_sw;

// Motor rotator angle: 0-4095
extern u16 g_resolver_pos_uw; 
extern u16 g_resolver_pos0_uw; 
extern s32 g_total_angle_sl;
extern s32 g_total_angle0_sl;

// Offset of Motor rotator angle
extern u16 g_mot_angle_offset_bldc_uw;
extern u16 g_mot_angle_offset_pmsm_uw;

extern u8 g_motor_type_ub;

extern u16 g_angle_eiq;
extern u16 g_angle_sensor;
extern s32 g_total_angle_sl;
extern u16 g_duty_uw;
extern u16 g_time_inh;

extern u16 g_pos_offset_bldc_uw;
extern u16 g_pos_offset_pmsm_uw;

////////////////////////////////////////////////////////////////////////////////
// Read the phase current
s16 get_phase_current(u8 phase_no);

// Read the motor temperature
u16 get_motor_temperature(void);

void do_motor_bldc_debug(u8 *buf);

s16 vqs_cmd_filter(s16 vqs);

void calc_mot_total_angle(void);

#endif /* _MOTOR_IF_H */
////////////////////////////////////////////////////////////////////////////////
