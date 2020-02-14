////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
*
*  
*      
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

#ifndef _CTRL_IF_H
#define _CTRL_IF_H

#include "std_defs.h"

////////////////////////////////////////////////////////////////////////////////
// Brake request from Master ECU (e.g. VCU, ESC, etc.): 0~1000 => 0~100%
extern u16 g_brake_pedal_pos_can_uw;

////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
The following functions are implemented in low level hardware driver
and called by user application.
*******************************************************************************/
// Return the pressure of master cylinder, 0.01MPa/bit
u16 get_mc_pressure(void);

/*******************************************************************************/
// Return the 1st brake pedal displacement
// 0~1000 => 0~100%
u16 get_disp_sensor0(void);

/*******************************************************************************/
// Set Motor offset
// offset: 0.1 degree/bit
u8 set_motor_offset(s16 offset);

/*******************************************************************************/
// Set Motor rotational direction
// dir: 0: clock wise, 1: counter-clockwise
u8 set_motor_dir(u8 dir);// (reserved)

/*******************************************************************************/
// Set Motor control voltage if PMSM algorithm implemented by the low level driver
// volt: 0.1V/bit
u8 set_motor_volt(u16 volt);// (reserved)

/*******************************************************************************/
// The motor driver (PMSM or BLDC) should be implemented in the low level code.
// All the phases PWM duty can be controlled by this function.
// phase_idx: U, V, W phase
// duty: 0.1%/bit, max: 1000 => 100%

u8 set_pwm_duty(u8 phase_idx, u16 duty);// (reserved)

/*******************************************************************************/
// To enable the motor driver
u8 set_motor_ctrl_enable(u8 enable);

/*******************************************************************************/
// Return the IGN switch output voltage, 0.1V/bit
u16 get_ign_volt(void);

/*******************************************************************************/
// Retuen the brake pedal light switch, 1:Press the pedal ; 0:Do not press the pedal.
u16 get_brk_pedal_state(void);
	
/*******************************************************************************/
// Return the brake pedal switch output voltage, 0.1V/bit
u16 get_brk_pedal_volt(void);

/*******************************************************************************/
// Return the current measurement, 0.01A/bit
// i_idx: current index. Line current: 0, Phase current U/V/W: 1,2,3
u16 get_current(u8 i_idx);

/*******************************************************************************/
// Return the speed of the motor, 0.1 r/min / bit
s16 get_motor_speed(void);


u16 get_motor_angle_e(void);
u16 get_motor_angle_m(void);
s32 get_motor_total_angle(void);
	
/*******************************************************************************/
// Return the PCB temperature, 0.1 Degree C / bit
u16 get_temp_sensor_pcb(void);

/*******************************************************************************/
// Return the MOSFET area temperature, 0.1 Degree C / bit
u16 get_temp_sensor_mosfet(void);

/*******************************************************************************/
// Return the battery voltage, 0.1V/bit
u16 get_bat_volt(void);

/*******************************************************************************
Function: Send message by CAN
id: 11
bit or 29 bit CAN ID
buf: bytes to be sent (max 8bytes)
*******************************************************************************/
u8 can_txx(u32 id,u8 *buf); // (reserved)

/*******************************************************************************
Function: CAN setting
baudrate: default 500,000Hz
option: reserved
*******************************************************************************/
u8 can_settingx(u32 baudrate, u8 option); // (reserved)

/*******************************************************************************
The following functions are implemented in user application
and called by low level hardware driver.
*******************************************************************************/

/*******************************************************************************
Function: Called if CAN message received
Note: Implmented by user application, called by low level hardware
id: CAN ID of current received message
cfg: low nibble: DLC of CAN message
buf: where message data can be found
*******************************************************************************/
u8 can_do_obj(u32 id,u8 cfg,u8 far *buf);

/*******************************************************************************
Function: Called every 1ms
Note: Implmented by user application, called by low level hardware
Place critical codes in this function only.
*******************************************************************************/
u8 isr_1ms_hook_user(void);

/*******************************************************************************
Function: Called every 5ms
Note: Implmented by user application, called by low level hardware
*******************************************************************************/
u8 isr_5ms_hook_user(void);

/*******************************************************************************
Function: Called every 10ms
Note: Implmented by user application, called by low level hardware
*******************************************************************************/
u8 isr_10ms_hook_user(void);

/*******************************************************************************
Function: Called every 10ms/1ms
Note: send can message to CAN bus for debugging
*******************************************************************************/
void transmit_can_data_10ms(void);
void transmit_can_data_1ms(void);

////////////////////////////////////////////////////////////////////////////////
#endif /* define _CTRL_IF_H */
////////////////////////////////////////////////////////////////////////////////
