////////////////////////////////////////////////////////////////////////////////
/**************************************************
*    File Name    : ctrl.c
*    Copyright    : QingChen
*    Module Name  : Electric Power Steering
     *
*    CPU          : XC2000
*    RTOS         : No RTOS
*
*    Create Date  : 2019.11.14
*    Author/Cororation: Perfersor YLY
*    Abstract Description:
*
*--------------- Revision History --------------
*    No Version Date      Revisied By  Item    Description
*    1  V0.0    19.11.26  WGY                  eBooster
**************************************************/

/**************************************************
*    Debug switch Section
**************************************************/
//#define PRESURE_PID_CTRL
/**************************************************
*    Include File Section
**************************************************/
#include "ctrl.h"
#include "ctrl_if.h"
#include "motor_if.h"
#include "motor_pmsm.h"
#include "hw.h"
#include "adc.h"
#include "can_if.h"

#include "Filters_EbsSensors.h"
#include "ebooster_ctrl.h"
#include "MotorDrive.h"
#include "pid.h"

/**************************************************
*    MacroDefine Section
**************************************************/

/**************************************************
*    StructDefine Section
**************************************************/

/**************************************************
*    Prototype Declare Section
**************************************************/
enum
{
	MOTOR_FORWORD = 0,
	MOTOR_BACKWARD = 1,
	MOTOR_HOLD = 2

};
/**************************************************
*    Global Variable Declare Section
**************************************************/
u16 TryVqs=0;
u16 test_dir=0;
u16 motor_offset=630;
const u16 C_VAL_DISP_INITIAL = 1900;
u16 g_mc_pressure;  //The Hydraulic master cylinder's preasure - RT 5ms per scan.
extern u16 g_disp_value_sent; //Relative Displacement - RT (Origional:CC2_CC22(PWM_EX3)) Relstive Displacement define in ccu2_ccu6.c


PID_st g_pid_pres_ctrl;  //PID controller required parameter structure.
////////////////////////////////////////////////////////////////////////////////
// Variables that sent to CAN
extern u16 g_pres_sense_val;  //The Hydraulic master cylinder's preasure value after filtrate.
extern u32 g_disp_sense_val;  //The Relative Displacement value after filtrate.

extern float DisplacementSenseMM;
extern u16 Voltagecal,Voltage;
extern u16 PressureAVE;
extern float Eerror;
extern float sum_error;
extern float Derror;
extern float e_error_0;
extern float Eerror_pre;
extern u16 MotorState;  //The direction of the moto rotation.
extern u16 TryValue1,TryValue2,TryValue3,TryValue4;

/**************************************************
*    File Static Variable Define Section
**************************************************/

/**************************************************
*    Function Define Section
**************************************************/

////////////////////////////////////////////////////////////////////////////////
// 5ms task
////////////////////////////////////////////////////////////////////////////////
u16 g_ctrl_test_timer_uw=0;

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 isr_5ms_hook_user(void)
{
	#ifdef PRESURE_PID_CTRL
	// Sensor measurement...
	g_mc_pressure=get_mc_pressure();  //Gei the Mc Preseaure

	// Signal filtering...
	filter_pres_sense(g_mc_pressure);
	filter_disp_sense(g_disp_value_sent);
	
	// Control logic...
	//DoEboosterCtrl();
	
	// Control output...
	MotorDrive();
	#else
	switch(g_ctrl_test_timer_uw)
	{
	case 0:
		break;

	case 100:
		//motor_offset=630;
		set_motor_ctrl_enable(2);
		set_motor_volt(TryVqs); // 1V
		set_motor_dir(test_dir);//0;
		set_motor_offset(motor_offset);//630
		break;
		
	case 500:
		set_motor_dir(1);
		set_motor_offset(550);//550
		break;
		
	case 900:
		set_motor_ctrl_enable(0);
		break;
	}

	#endif
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 isr_10ms_hook_user(void)
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 isr_1ms_hook_user(void)
{
	// Test only: FIXME: yly
	static u8 tmx=0;

	transmit_can_data_1ms();

	if((++tmx)>=10)
	{
		tmx=0;
		transmit_can_data_10ms();
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 can_do_obj(u32 id,u8 cfg,u8 far *buf)
{
	UNUSED_PARA(cfg);
	
	// Test only: Command message from master controller, e.g. VCU, ESC, etc.)
	if(id!=0x888)
		return 1;

	g_brake_pedal_pos_can_uw=buf[1]; // 8-bit
	g_brake_pedal_pos_can_uw<<=2;    // adjust to 10-bit

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void transmit_can_data_10ms(void)
{
    u8 bufx[8];
	
	////////////////////////////////////////////////////////////////////////////
	//bufx[0]=(u8)(get_ad_value(AD_VCC5B_MON)>>2);
	//bufx[1]=(u8)(get_ad_value(AD_BAT12V_MONY)>>2);
	//bufx[2]=(u8)(get_ad_value(AD_TEMP_SS1)>>2);
	//bufx[3]=(u8)(get_ad_value(AD_VCC5_SW_MON)>>2);
	//bufx[4]=(u8)(get_ad_value(AD_BAT12V_MON)>>2);
	//bufx[5]=(u8)(get_ad_value(AD_AIN4)>>2);
	//bufx[6]=(u8)(get_ad_value(AD_AIN3)>>2);
	//bufx[7]=(u8)(get_ad_value(AD_VO1)>>2);

	//bufx[0]=((u8)(((u16)(DisplacementSenseMM))>>8));
	//bufx[1]=((u8)(((u16)(DisplacementSenseMM))&0xFF));
	//bufx[2]=(u8)(Voltage>>8);
	//bufx[3]=(u8)(Voltage&0xff);
	//bufx[4]=(u8)(PressureAVE>>8);
	//bufx[5]=(u8)(PressureAVE&0xff);
	//bufx[6]=((u8)(((u16)(Eerror))>>8));
	//bufx[7]=((u8)(((u16)(Eerror))&0xFF));
	
	
	//can_sload(7,bufx);  //0x105
  //can_stx(7);
		
	////////////////////////////////////////////////////////////////////////////
	//bufx[0]=(u8)(MotorState);
	//bufx[1]=(u8)(1);
	//bufx[2]=(u8)(2);
	//bufx[3]=(u8)(3);
	//bufx[4]=(u8)(4);
	//bufx[5]=(u8)(5);
	//bufx[6]=(u8)(6);
	//bufx[7]=(u8)(7);

	//can_sload(8,bufx);  //0x106
  //can_stx(8);
	
	////////////////////////////////////////////////////////////////////////////
	//bufx[0]=(u8)(g_angle_eiq);
	//bufx[1]=(u8)(g_angle_eiq>>8);
	//bufx[2]=(u8)(g_total_angle_sl);
	//bufx[3]=(u8)(g_total_angle_sl>>8);
	//bufx[4]=(u8)(g_mot_speed_sw);
	//bufx[5]=(u8)(g_mot_speed_sw>>8);
	//bufx[6]=(u8)(g_dir_ub);
	//bufx[7]=(u8)(g_resolver_pos_uw/683);
	
    //can_sload(9,bufx);
    //can_stx(9);
			////////////////////////////////////////////////////////////////////////////
	bufx[0]=(u8)(1);
	bufx[1]=(u8)(2);
	bufx[2]=(u8)(3);
	bufx[3]=(u8)(4);
	bufx[4]=(u8)(5);
	bufx[5]=(u8)(6);
	bufx[6]=(u8)(7);
	bufx[7]=(u8)(8);
	
  can_sload(16,bufx);
  can_stx(16);
	
	bufx[0]=(u8)(1);
	bufx[1]=(u8)(2);
	bufx[2]=(u8)(3);
	bufx[3]=(u8)(4);
	bufx[4]=(u8)(5);
	bufx[5]=(u8)(6);
	bufx[6]=(u8)(7);
	bufx[7]=(u8)(8);
	
  can_sload(17,bufx);
  can_stx(17);
	
	bufx[0]=(u8)(1);
	bufx[1]=(u8)(2);
	bufx[2]=(u8)(3);
	bufx[3]=(u8)(4);
	bufx[4]=(u8)(5);
	bufx[5]=(u8)(6);
	bufx[6]=(u8)(7);
	bufx[7]=(u8)(8);
	
  can_sload(18,bufx);
  can_stx(18);
	
	bufx[0]=(u8)(1);
	bufx[1]=(u8)(2);
	bufx[2]=(u8)(3);
	bufx[3]=(u8)(4);
	bufx[4]=(u8)(5);
	bufx[5]=(u8)(6);
	bufx[6]=(u8)(7);
	bufx[7]=(u8)(8);
	
  can_sload(19,bufx);
  can_stx(19);
	////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void transmit_can_data_1ms(void)
{
	/*
	u8 bufx[8];
	
	////////////////////////////////////////////////////////////////////////////
	bufx[0]=(u8)(g_resolver_pos_uw);
	bufx[1]=(u8)(g_resolver_pos_uw>>8);
	bufx[2]=(u8)(g_duty_A/10);
	bufx[3]=(u8)(g_duty_B/10);
	bufx[4]=(u8)(g_duty_C/10);
	bufx[5]=(u8)(g_angle_eiq);
	bufx[6]=(u8)(g_angle_eiq>>8);
	bufx[7]=0;
	
    can_sload(4,bufx);
    can_stx(4);

	////////////////////////////////////////////////////////////////////////////
	// bufx[0]=(u8)(g_pwm_ex_duty_uw[0]);
	// bufx[1]=(u8)(g_pwm_ex_duty_uw[0]>>8);
	// bufx[2]=(u8)(g_pwm_ex_duty_uw[1]);
	// bufx[3]=(u8)(g_pwm_ex_duty_uw[1]>>8);
	// bufx[4]=(u8)(g_pwm_ex_duty_uw[2]);
	// bufx[5]=(u8)(g_pwm_ex_duty_uw[2]>>8);
	// bufx[6]=(u8)(g_pwm_ex_duty_uw[3]);
	// bufx[7]=(u8)(g_pwm_ex_duty_uw[3]>>8);

	bufx[0]=(u8)(Vqs_cmd/100);
	bufx[1]=(u8)(g_angle_offset/10);
	bufx[2]=(u8)(V.Alpha/10); // 0.01V/bit
	bufx[3]=(u8)(V.Beta/10);  // 0.01V/bit
	bufx[4]=(u8)(V.VecSector);
	bufx[5]=(u8)(V.T0/10);
	bufx[6]=(u8)(V.T1/10);
	bufx[7]=(u8)(V.T2/10);

    can_sload(5,bufx);
    can_stx(5);

	////////////////////////////////////////////////////////////////////////////
	bufx[0]=(u8)(get_ad_value(AD_SCDL)>>2);
	bufx[1]=(u8)(GET_TLE_ERR2());
	bufx[1]<<=1;
	bufx[1]|=(GET_TLE_ERR1());
	bufx[2]=(u8)(get_ad_value(AD_BAT12V_SW)>>2);
	// bufx[3]=(u8)(get_ad_value(AD_VRO)>>2);
	// bufx[4]=(u8)(get_ad_value(AD_VO2)>>2);
	bufx[3]=(u8)(get_ad_value(AD_VO2));
	bufx[4]=(u8)(get_ad_value(AD_VO2)>>8);
	bufx[5]=(u8)(get_ad_value(AD_SH1_MON)>>2);
	bufx[6]=(u8)(get_ad_value(AD_SH2_MON)>>2);
	bufx[7]=(u8)(get_ad_value(AD_SH3_MON)>>2);
	
    can_sload(6,bufx);
    can_stx(6);
		*/
	////////////////////////////////////////////////////////////////////////////
}

