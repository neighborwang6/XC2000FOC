////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
*
*  
*      
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

#include "board.h"
#include "pwm.h"

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 pwm_init(void)
{
	set_phase_on(0,0);
	set_phase_on(1,0);
	set_phase_on(2,0);
	
	pwm_setduty(0,0);
	pwm_setduty(1,0);
	pwm_setduty(2,0);

	return 0;
}
////////////////////////////////////////////////////////////////////////////////
// ch_num: 0/1/2: Phase A/B/C, duty: 0~1000 => 0~100%
////////////////////////////////////////////////////////////////////////////////
u8 pwm_setduty(u8 ch_num, u16 duty)
{
	// Parameters check
	if(ch_num>3)
		return 1;

	if(duty>1000)
		return 2;
	
	////////////////////////////////////////////////////////
	switch(ch_num)
	{
		// Center-aligned/Edge-aligned Mode: -duty*4 / -duty*8
		//      |<----- Update the duty ---------->|<--- Shadow transfer enable --->|
		case 0: CCU61_CC60SR = MOT_PWM_PERIOD-duty*4; CCU61_TCTR4|=0x40; break;
		case 1: CCU61_CC61SR = MOT_PWM_PERIOD-duty*4; CCU61_TCTR4|=0x40; break;
		case 2: CCU61_CC62SR = MOT_PWM_PERIOD-duty*4; CCU61_TCTR4|=0x40; break;
			
		default: break;
	}
	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Set Phase A/B/C: On/Off
////////////////////////////////////////////////////////////////////////////////
u8 set_phase_on(u8 phase_no,u8 on_off)
{
	// Set the IHx/ILx Pin of TLE7189F as INPUT to shut down the phase.
	// Restore the previous setting to turn the phase ON.
	switch(phase_no)
	{
		//              |<------ Set port dir: input ---->|       |<----- Set port dir: output ------>|
		case 0:
			if(!on_off){P0_IOCR00=0x0000; P0_IOCR03=0x0000;} else {P0_IOCR00=0x00B0; P0_IOCR03=0x00B0;}; break;
		case 1:
			if(!on_off){P0_IOCR01=0x0000; P0_IOCR04=0x0000;} else {P0_IOCR01=0x00B0; P0_IOCR04=0x00B0;}; break;
		case 2:
			if(!on_off){P0_IOCR02=0x0000; P0_IOCR05=0x0000;} else {P0_IOCR02=0x00B0; P0_IOCR05=0x00B0;}; break;
		default:
			break;
	}
	
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
