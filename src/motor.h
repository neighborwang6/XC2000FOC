////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
*
*  
*	   
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

#ifndef _MOTOR_H
#define _MOTOR_H

#include "std_defs.h"

#include "motor_bldc.h"
#include "motor_pmsm.h"
#include "pwm.h"

void calc_mot_speed(void);

#define update_pwm1_duty(x) pwm_setduty(0,(x))
#define update_pwm2_duty(x) pwm_setduty(1,(x))
#define update_pwm3_duty(x) pwm_setduty(2,(x))

#endif /* define _MOTOR_H */
////////////////////////////////////////////////////////////////////////////////
