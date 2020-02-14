#include "MotorDrive.h"
#include "std_defs.h"
#include "ctrl_if.h"
#include "MotorCtrlSet.h"
u16 protect_pre=0;
u16 Control_mode=0;
u16 TestDUTY=0;

extern u16 MotorModeSet;
extern u16 state,Voltage;
extern float sum_error;

extern u16 DisplacementInit,DisplacementSenseMM;
extern u16 ControlModePre;
extern u16 MotorState;
extern u16 PressureAVE,ControlMode;

void MotorDrive(void)
{
	if(ControlModePre==1)
	{
		if (ControlMode==1)
			{
				protect_pre=0;
						
						if(MotorState==0)  //&&(TargetPressure<=(PressureAVE*0.9))
						{
							Control_mode=1;  //ForWard
						}
						else if(MotorState==1)
						{
							Control_mode=2;  //BackWard
						}
				
            switch (Control_mode)
            {
            case 1:
              MotorModeSet=100;  //Forward
						  MotorCtrlSet();
						  set_motor_volt(Voltage);
                break;
            case 2:
                MotorModeSet=500;
						    MotorCtrlSet();
						set_motor_volt(Voltage);
                break;
            case 3:
                MotorModeSet=900;
								set_motor_volt(0);
						    MotorCtrlSet();
                break;
					}
        }
      else
        {
            sum_error=0;
            Control_mode=5;
            protect_pre++;
						if(protect_pre>=600)
            {
                Control_mode=8;
                protect_pre=1080;
            }
							
					switch (Control_mode)
            {
            case 5:
              MotorModeSet=900;
						  MotorCtrlSet();
						  set_motor_volt(0);//2V
                break;
            case 8: 
                MotorModeSet=900;
						    MotorCtrlSet();
								set_motor_volt(0);
                break;
            }
    }
		}
}
