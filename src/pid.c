/**************************************************
*    File Name    : PID.c
*    Copyright    : QingChen
*    Module Name  : Electric Power Steering
     *
*    CPU          : XC2000
*    RTOS         : No RTOS
*
*    Create Date  : 2019.11.14
*    Author/Cororation: WGY/QingChen Techology
*    Abstract Description:
*
*--------------- Revision History --------------
*    No Version Date      Revisied By  Item    Description
*    1  V0.0    19.11.14  WGY                  EPS
**************************************************/

/**************************************************
*    Debug switch Section
**************************************************/
//#define TORQUE_OPEN_LOOP_CTRL
#define TORQUE_PID_CTRL
/**************************************************
*    Include File Section
**************************************************/
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

/**************************************************
*    Global Variable Declare Section
**************************************************/

/**************************************************
*    File Static Variable Define Section
**************************************************/

/**************************************************
*    Function Define Section
**************************************************/

//-----------------------------------
//---------PID_st realize function
s16 PID_realize(PID_st *p)
{
	f32 opt_p,opt_i,opt_d;
	/* err calculate */
	p->err = (p->desire_value) - (p->actual_value);
	p->sum_err += p->err;
	//Proportion
	opt_p = (p->kp)*(p->err);
	//Integral
	opt_i = (p->ki)*(p->sum_err);
	//Differential
	opt_d = (p->ki)*((p->err) - (p->pre_err));

	p->pre_err = p->err;  //Update the history error.
	
	p->output = opt_p + opt_d;
	return (s16)(p->output);

}

void PID_Init(PID_st *p)
{
	p->kp = 0;            // Q24
	p->ki = 0;            // Q24
	p->kd = 0;            // Q24
	p->delta_kp = 0;      // Q24
	p->delta_ki = 0;      // Q24
	p->delta_kd = 0;      // Q24
	p->actual_value = 0;  // actual value of the control variable    Q16
	p->desire_value = 0;  // desired value of the control variable   Q16
	p->lowscale = 0;      // a minimum sacle factor the control variable, if actual value < actual value*lowscale,dont use integral   Q24
	p->highscale = 0;     // a maximun sacle factor the control variable, if actual value > actual value*highscale,dont use integral  Q24
	p->max = 0;           // output max value   Q24
	p->min = 0;           // output min value   Q24
	p->ki_index = 0;      // Q24
	p->kd_index = 0;      // Q24
	p->kp_index = 0;      // Q24
	p->sum_err = 0;
	p->pre_err = 0;
	p->err = 0;
	p->output = 0;

}

	//g_pid_pres_ctrl.kp = 0;
	//g_pid_pres_ctrl.ki = 0;
	//g_pid_pres_ctrl.kd = 0;
	//g_pid_pres_ctrl.desire_value = C_VAL_DISP_INITIAL;
	//g_pid_pres_ctrl.actual_value = g_disp_value0;
	//TryVqs = PID_realize(&g_pid_pres_ctrl);