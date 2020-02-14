////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
*
*  
*      
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

#include "filters.h"

u16 sum_currentA=0;
u16 phase_currentA=0;
u16 sum_currentB=0;
u16 phase_currentB=0;
u16 sum_currentC=0;
u16 phase_currentC=0;
u16 sum_currentM=0;
u16 phase_currentM=0;
u16 sum_currentD=0;
u16 currentD=0;
u16 sum_currentQ=0;
u16 currentQ=0;

s32 sum_motor_m_angle=0;
s32 motor_m_angle=0;

s32 sum_s_sense=0;
s32 s_sense=0;

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
s32 filter_motor_m_angle(s32 motor_m_angle_get)
{
	s32 sum_temp;

	sum_motor_m_angle+=motor_m_angle_get;
	sum_motor_m_angle-=motor_m_angle;
	sum_temp=sum_motor_m_angle;
	sum_temp=(sum_temp*160)>>8;
	motor_m_angle=sum_temp;

	return motor_m_angle;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
s32 filter_s_sense(s32 s_sense_get)
{
	s32 sum_temp;

	sum_s_sense+=s_sense_get;
	sum_s_sense-=s_sense;
	sum_temp=sum_s_sense;
	sum_temp=(sum_temp*160)>>8;
	s_sense=sum_temp;

	return s_sense;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 filter_currentD(u16 currentD_calc)
{
	s32 sum_temp;

	sum_currentD+=currentD_calc;
	sum_currentD-=currentD;
	sum_temp=sum_currentD;
	sum_temp=(sum_temp*160)>>8;
	currentD=(u16)sum_temp;

	return currentD;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 filter_currentQ(u16 currentQ_calc)
{
	s32 sum_temp;

	sum_currentQ+=currentQ_calc;
	sum_currentQ-=currentQ;
	sum_temp=sum_currentQ;
	sum_temp=(sum_temp*160)>>8;
	currentQ=(u16)sum_temp;

	return currentQ;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 filter_currentA(u16 currentA_adc)
{
	s32 sum_temp;

	sum_currentA+=currentA_adc;
	sum_currentA-=phase_currentA;
	sum_temp=sum_currentA;
	sum_temp=(sum_temp*160)>>8;
	phase_currentA=(u16)sum_temp;

	return phase_currentA;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 filter_currentB(u16 currentB_adc)
{
	s32 sum_temp;

	sum_currentB+=currentB_adc;
	sum_currentB-=phase_currentB;
	sum_temp=sum_currentB;
	sum_temp=(sum_temp*160)>>8;
	phase_currentB=(u16)sum_temp;

	return phase_currentB;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 filter_currentC(u16 currentC_adc)
{
	s32 sum_temp;
	
	sum_currentC+=currentC_adc;
	sum_currentC-=phase_currentC;
	sum_temp=sum_currentC;
	sum_temp=(sum_temp*160)>>8;
	phase_currentC=(u16)sum_temp;

	return phase_currentC;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 filter_currentM(u16 currentM_adc)
{
	s32 sum_temp;

	sum_currentM+=currentM_adc;
	sum_currentM-=phase_currentM;
	sum_temp=sum_currentM;
	sum_temp=(sum_temp*160)>>8;
	phase_currentM=(u16)sum_temp;

	return phase_currentM;
}
////////////////////////////////////////////////////////////////////////////////
