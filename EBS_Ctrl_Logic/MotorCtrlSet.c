#include "MotorCtrlSet.h"
#include "std_defs.h"
#include "ctrl_if.h"

u16 Forward=0,Backward=1;
u16 MotorOffset=0;
u16  MotorModeSet=0;
void MotorCtrlSet(void)
{
	switch(MotorModeSet)
	{
	case 0:
		break;

	case 100:
		MotorOffset=650;//820;//820
		set_motor_ctrl_enable(2);
		set_motor_dir(Forward);
		set_motor_offset(MotorOffset);//630
		break;
		
	case 500:
		MotorOffset=630;//375;//375
	  set_motor_ctrl_enable(2);
		set_motor_dir(Backward);
		set_motor_offset(MotorOffset);//550
		break;
		
	case 900:
		set_motor_ctrl_enable(0);
		break;
	}
}