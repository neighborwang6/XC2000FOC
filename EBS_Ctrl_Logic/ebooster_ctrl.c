/**************************************************
*    File Name    : ebooster_ctrl.c
*    Copyright    : QingChen
*    Module Name  : eBooster
     *
*    CPU          : XC2000
*    RTOS         : No RTOS
*
*    Create Date  : 2019.11.26/22:10
*    Author/Cororation: WGY/QingChen Techology
*    Abstract Description: The main ctrl logic of the ebooster.
*
*--------------- Revision History --------------
*    No Version Date      Revisied By  Item    Description
*    1  V0.0    19.11.26  WGY                  
**************************************************/

/**************************************************
*    Debug switch Section
**************************************************/

/**************************************************
*    Include File Section
**************************************************/
#include "ebooster_ctrl.h"
#include "std_defs.h"
#include "Filters.h"
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

//--------Sensor Val--------
extern u16 g_pres_sense_val;  //The Pressure of the brake fluid in the hydraculic cylinder.
extern u32 g_disp_sense_val;	//The relative diaplacement of the iBooster.

float DisplacementSenseMM=0;
u16 PressureAVE=0;
u16 ControlModePre=1,ControlMode=0;
float RodMotor=0,RodActual=0,RodActual0=0.2,PercentError=0;
float DisplacementSenseMM_Pre=0,RodActual_Pre=0;
u16 DataTimer20ms=0,TimerState0=0,TimerState1=0,TimerState2=0;

float Eerror=0,sum_error=0,Derror=0,e_error_0=50,Eerror_pre=0;
float ControlP=2;
float ControlI=0.04;
float ControlD=0;
float test=0,test1=0,test2=0;
u16 state=0,state_pre=0;
u16 Drv_state=1;
u16 DisplacementInit=1998;
u16 PressureInit=130;  //
u16 stop_state=0,state_time=0,star_state=0;
u16 Timer1s=0,LimitState=0;
float  BrkStarVal=0.15;
u16 pressure_state=0,pressure_steady=0;
u16 Timer500ms=0,TimerCycle=0;
float TryRod0=0;

//----------------Output Val
u16 Voltage=0;
u16 MotorState=0;  //The direction of the moto rotation.

/**************************************************
*    File Static Variable Define Section
**************************************************/

/**************************************************
*    Function Define Section
**************************************************/

void DoEboosterCtrl(void)
{
	//------------------------------------------------Data Cal------------------------------------------//
	if(g_pres_sense_val>PressureInit)
	{
		PressureAVE=(g_pres_sense_val-PressureInit);
	}
	else
	{
		PressureAVE=0;
	}
	DisplacementSenseMM=(g_disp_sense_val*1.0-DisplacementInit*1.0);
	
    //pressure_ctrl//////////////////////////////////////////////////////////////////////////////////////////////////////

    if(ControlModePre==1)
    {
			if(PressureAVE>=0)
        {
					RodMotor=(PressureAVE*1.0)/20.0;//4.25;//num_better pressure sensor30.762
					if(RodMotor>=15)
					{
						RodMotor=15;
					}
        }
        else
        {
            RodMotor=0;
        }
        RodActual=RodMotor+DisplacementSenseMM*0.00122;
        if(RodActual<0)
        {
            RodActual=0;
        }

        //------------------------------------------------control mode------------------------------------------//
        if(((RodActual)>=(RodActual0)))//&&(DisplacementSenseMM>=0.1)
        {
            ControlMode=1;  //
        }
        else
        {
            ControlMode=0;  //
        }
       
        //------------------------------------------------control mode end------------------------------------------//
        if((ControlMode==1))
        {
					Eerror=DisplacementSenseMM; //Eerror: MAX_VAL == 14
					
					sum_error+=(Eerror);  //PID - I

					Derror=Eerror-Eerror_pre; //PID - D
					
					if((sum_error>=(700.0/ControlI)))
					{
						sum_error=700.0/ControlI;
					}
					if(sum_error<=-(300.0/ControlI))
					{
						sum_error=-300.0/ControlI;
					
					}
					Eerror_pre=Eerror;
					
					//--------PID CTRL--------
					if((Eerror>4.2)||(Eerror<-4.2))//PID stage 1
					{
						//ControlP = 25.0;
						//ControlI = 0.0;
						
						if((ControlP*Eerror+ControlI*sum_error+ControlD*Derror)>=0)
						{
							Voltage=(ControlP*Eerror+ControlI*sum_error+ControlD*Derror);
							MotorState=0;
						}
						else
						{
							Voltage=-(ControlP*Eerror+ControlI*sum_error+ControlD*Derror);
							MotorState=1;
							if(Voltage>=300)
				{
					Voltage =300; //Limit to 3V.1200 => 12V 
				}
						}
					}
					else if((Eerror>4.2)||(Eerror<-4.2))//PID stage 2
					{
						//ControlP = 5.0;
						//ControlI = 0.002;
						if((ControlP*Eerror+ControlI*sum_error+ControlD*Derror)>=0)
							{
								Voltage=(ControlP*Eerror+ControlI*sum_error+ControlD*Derror);
								MotorState=0;
							}
							else
							{
								Voltage=-(ControlP*Eerror+ControlI*sum_error+ControlD*Derror);
								MotorState=1;
								if(Voltage>=300)
				{
					Voltage =300; //Limit to 3V.1200 => 12V 
				}
							}
					}
				}
				if((PressureAVE>300)&&(LimitState==0)&&(DisplacementSenseMM>=50))  //7.5Mpa
				{
					
						Voltage=70;//22.22
						LimitState=1;
				}
				if(DisplacementSenseMM<=-150)
					{
						LimitState=0;
					}
				if(Voltage>=700)
				{
					Voltage =700; //Limit to 3V.1200 => 12V 
				}
        /* Drive the motor to initial position*/
        if(ControlMode==0)
        {
					/*Detect motor position*/
					Voltage=0;
					sum_error=0;
					RodActual=0;
					RodMotor=0;
					test=0;
					Eerror=0;
					Drv_state=1;
					state=5;
        }
    }
		else
		{
			Voltage=0;
      sum_error=0;
      RodActual=0;
      RodMotor=0;
      test=0;
      Eerror=0;
      Drv_state=1;
      state=5;
		}
    //////////////////////////////////////////////////////////////////////



}
