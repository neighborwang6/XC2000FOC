////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
*
*  
*	   
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

#include "svpwm.h"
#include "motor.h"

#include "sin_data.h"

////////////////////////////////////////////////////////////////////////////////
#define	Lq           0.0002 //0.002
#define	Ld           0.0002
#define	flux         0.02 //0.2
#define	Rs           0.1  //0.01 not rot//1

////////////////////////////////////////////////////////////////////////////////
// The duty
u16	g_duty0;
u16	g_duty1;
u16	g_duty2;

// The duty applied to Phase A/B/C
u16	g_duty_A;
u16	g_duty_B;
u16	g_duty_C;

////////////////////////////////////////////////////////////////////////////////
//clark	transformation,	containing(K=2/3),ensuring the peak	and	RMS	of Voltage is equal
////////////////////////////////////////////////////////////////////////////////
void Clark(I_CLARK_PARK	*I)
{
	UNUSED_PARA(I);
	
	// I->Alpha	= _IQmpy(((I->As*2)- I->Bs - I->Cs),_IQ(ONEbyTHREE)); // 2/3(ua-1/2ub-1/2uc)
	// I->Beta	= _IQmpy((I->Bs	- I->Cs),_IQ(ONEbySQRT3));			  // sqrt3/3(ub-uc)
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void Park(I_CLARK_PARK *I)
{
	if(I->Angle<=F_ANGLE(90))
	{
		I->Sine	  =	g_sin_value[I->Angle];
		I->Cosine =	g_sin_value[F_ANGLE(90)-I->Angle];
	}
	else if(I->Angle<=F_ANGLE(180))
	{
		I->Sine	  =	 g_sin_value[F_ANGLE(180)-I->Angle];
		I->Cosine =	-g_sin_value[I->Angle-F_ANGLE(90)];
	}
	else if(I->Angle<=F_ANGLE(270))
	{
		I->Sine	  =	-g_sin_value[I->Angle-F_ANGLE(180)];
		I->Cosine =	-g_sin_value[F_ANGLE(270)-I->Angle];
	}
	else
	{
		I->Sine	  =	-g_sin_value[F_ANGLE(360)-I->Angle];
		I->Cosine =	 g_sin_value[I->Angle-F_ANGLE(270)];
	}
}

////////////////////////////////////////////////////////////////////////////////
// https://blog.csdn.net/daidi1989/article/details/89945557
////////////////////////////////////////////////////////////////////////////////
void IPark(V_CLARK_PARK	*v)
{
	v->Alpha = (v->Ds *	v->Cosine) - (v->Qs	* v->Sine);
	v->Alpha/=100000;
	v->Beta	 = (v->Ds *	v->Sine) + (v->Qs *	v->Cosine);	
	v->Beta/=100000;
}

////////////////////////////////////////////////////////////////////////////////
// Reference: "SVPWM Pricipal and Control version5...pdf"
////////////////////////////////////////////////////////////////////////////////
void motor_sector_cal(V_CLARK_PARK *v)
{
	const u8 l_sector_idx[7]={0,2,6,1,4,3,5};
	
	// Determine the sector	of Uref
	v->N=0;
	v->tmp1	= v->Beta;
	v->tmp2	=  (1732 * v->Alpha)/1000 -	v->Beta;	 //	(sqrt(3)v->Alpha-v->Beta)/2	 : 1732=1.732*1000:	sqrt(3)=1.732
	v->tmp3	= -(1732 * v->Alpha)/1000 -	v->Beta;	 //	-(sqrt(3)v->Alpha+v->Beta)/2
	
	if (v->tmp1>0)	 //	A=1	,else A=0
		v->N=v->N+1; 
	if (v->tmp2>0)	 //	B=1	,else B=0
		v->N=v->N+2;
	if (v->tmp3>0)	 //	C=1	,else C=0
		v->N=v->N+4; //	vector parameter N=A+2B+4C
	
	if(v->N>=1 && v->N<=6)
		v->VecSector=l_sector_idx[v->N];
}

////////////////////////////////////////////////////////////////////////////////
// PWM generate	function, according	to current Alpha and Beta aixs voltage
// Reference: Study	on DSP based PMSM Control...caj
////////////////////////////////////////////////////////////////////////////////
void motor_sector_switch_cal(V_CLARK_PARK *v)
{
	// Temp	varable, if	T1 + T2	>Ts, overmodulated
	s16	T1,T2;
	s32	l_tmps32_a;
	s32	l_tmps32_b;
	s32	l_tmps32;
	
	// Time	slot resolution: 0.1us/bit
	v->Ttmp= (1732UL*Ts)/2/Udc;								  // (sqrt3*Ts)/(2*Udc)
	v->Tx  = (s16)(v->Ttmp*v->Beta*2/1000);					// (sqrt3*Ts*v->Beta)/Udc
	
	// v->Ty  =	(1000*v->Beta +	1732*v->Alpha)*v->Ttmp/1000;  // (sqrt3*Ts)/(2*Udc)*(sqrt3*v->Alpha+v->Beta)
	// v->Tz  =	(1000*v->Beta -	1732*v->Alpha)*v->Ttmp/1000;  // (sqrt3*Ts)/(2*Udc)*(-sqrt3*v->Alpha+v->Beta)
	l_tmps32_a = v->Alpha *	v->Ttmp	* 1732 / 1000;
	l_tmps32_b = v->Beta * v->Ttmp;
	v->Ty =	(s16)((l_tmps32_b +	l_tmps32_a)/1000);
	v->Tz =	(s16)((l_tmps32_b -	l_tmps32_a)/1000);
	
	// According to	the	sector,	decide the current conducting time of two vectors
	switch (v->VecSector)
	{
	case 1:	// sector 1
		T1=	-v->Tz;	// vector(A	B C) 1 0 0
		T2=	 v->Tx;	// vector(A	B C) 1 1 0
		break;
	case 2:	// sector 2
		T1=	 v->Tz;	// vector(A	B C) 0 1 0
		T2=	 v->Ty;	// vector(A	B C) 1 1 0
		break;
	case 3:	// sector 3
		T1=	 v->Tx;	// vector(A	B C) 0 1 0
		T2=	-v->Ty;	// vector(A	B C) 0 1 1
		break;
	case 4:	// sector 4
		T1=	-v->Tx;	// vector(A	B C) 0 0 1
		T2=	 v->Tz;	// vector(A	B C) 0 1 1
		break;
	case 5:	// sector 5
		T1=	-v->Ty;	// vector(A	B C) 0 0 1
		T2=	-v->Tz;	// vector(A	B C) 1 0 1
		break;
	case 6:	// sector 6
		T1=	 v->Ty;	// vector(A	B C) 1 0 0
		T2=	-v->Tx;	// vector(A	B C) 1 0 1
		break;
	}
	
	if(T1<0	|| T2<0)
	{
		// Something wrong happends...
	}
	
	// When	T1+T2>Ts, need to be normalized
	if (T1+	T2>	Ts)	// over	modulated
	{
		l_tmps32=Ts; l_tmps32*=T1; l_tmps32/=(T1+T2);
		v->T1= (u16)l_tmps32;
		l_tmps32=Ts; l_tmps32*=T2; l_tmps32/=(T1+T2);
		v->T2= (u16)l_tmps32;
	}
	else
	{
		v->T1=(u16)T1;
		v->T2=(u16)T2;
	}
	////////////////////////////////////////////////////////
	// Conducting phase	switch moment: 1st,	2nd, 3rd
	v->Ta= (Ts-	v->T1- v->T2)>>2;	 //	(Ts-T1-T2)/4
	v->Tb= v->Ta + (v->T1>>1);		 //	(Ts-T1-T2)/4+T1/2
	v->Tc= v->Tb + (v->T2>>1);		 //	(Ts-T1-T2)/4+T1/2+T2/2
	
	////////////////////////////////////////////////////////
	// switching time: 0.1%/bit, factor=10
	g_duty0=(u16)(1000 - (2000UL* v->Ta) / Ts);	//(1-2Ta/Ts)*100
	g_duty1=(u16)(1000 - (2000UL* v->Tb) / Ts);	//(1-2Tb/Ts)*100
	g_duty2=(u16)(1000 - (2000UL* v->Tc) / Ts);	//(1-2Tc/Ts)*100
	
	if(g_duty0>2000)
		g_duty0=g_duty0;
	
	////////////////////////////////////////////////////////
	// Determine the current conducting	time of	two	vectors
	switch (v->VecSector)
	{
	case 1:	// sector 1	  0-A-AB-0-0-AB-A-0
		g_duty_A = g_duty2;
		g_duty_B = g_duty1;
		g_duty_C = g_duty0;
		break;
	case 2:	// sector 2	  0-B-AB-0-0-AB-B-0
		g_duty_A = g_duty2;
		g_duty_B = g_duty0;
		g_duty_C = g_duty1;
		break;
	case 3:	// sector 3	  0-B-BC-0-0-BC-B-0
		g_duty_A = g_duty1;
		g_duty_B = g_duty0;
		g_duty_C = g_duty2;
		break;
	case 4:	// sector 4	  0-C-BC-0-0-BC-C-0
		g_duty_A = g_duty0;
		g_duty_B = g_duty1;
		g_duty_C = g_duty2;
		break;
	case 5:	// sector 5	  0-C-CA-0-0-CA-C-0
		g_duty_A = g_duty0;
		g_duty_B = g_duty2;
		g_duty_C = g_duty1;
		break;
	case 6:	// sector 6	  0-A-AC-0-0-AC-A-0
		g_duty_A = g_duty1;
		g_duty_B = g_duty2;
		g_duty_C = g_duty0;
		break;
		
	default:
		break;
	}
	////////////////////////////////////////////////////////
	update_pwm1_duty(g_duty_A);
	update_pwm2_duty(g_duty_B);
	update_pwm3_duty(g_duty_C);
	////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////

