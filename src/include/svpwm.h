////////////////////////////////////////////////////////////////////////////////
#ifndef SVPWM_H_
#define SVPWM_H_

#include "std_defs.h"
////////////////////////////////////////////////////////////////////////////////
//
typedef struct{
	u16  As;          // A phase
	u16  Bs;         // B phase
	u16  Cs;         // B phase
	s32  Alpha;      // alpha axis
	s32  Beta;       // beta axis
	s16  Ds;         // d axis
	s16  Qs;         // q axis
	u16  Angle;	  // Input: rotating angle (pu)   Q24
	s32  Sine;       //
	s32  Cosine;     //
	s16  Ttmp;       //
	s16  Tx;         // first  vector(main vector) conducting time
	s16  Ty;         // second vector(assistant vector) conducting time
	s16  Tz;         // zero vector conducting time
	u16  T0;         // zero vector conducting time
	u16  T1;         // first  vector(main vector) conducting time
	u16  T2;         // second vector(assistant vector) conducting time
	u16  Ta;		  // Output: reference phase-a switching function
	u16  Tb;		  // Output: reference phase-b switching function
	u16  Tc;		  // Output: reference phase-c switching function
	s32  tmp1;	      // Variable: temp variable to decide sector
	s32  tmp2;		  // Variable: temp variable
	s32  tmp3;		  // Variable: temp variable

	u16 VecSector;	  // Space vector sector
	u16 N;           // N=A+2B+4C ,to decide the sector
}V_CLARK_PARK;        // transformation  clark park

////////////////////////////////////////////////////////////////////////////////
//
typedef struct{
	u16  As;         // A phase
	u16  Bs;         // B phase
	u16  Cs;         // B phase
	u16  Alpha;      // alpha axis
	u16  Beta;       // beta axis
	s16  Ds;         // d axis
	s16  Qs;         // q axis
	u16  Angle;		 // Input: rotating angle (pu)
	s32  Sine;       //
	s32  Cosine;     //
}I_CLARK_PARK;

////////////////////////////////////////////////////////////////////////////////
# define ONEbySQRT3   0.57735026918963    /* 1/sqrt(3) */
# define ONEbyTHREE   0.3333333333        /* 1/3     */
# define SQRTTHREE    1.732050805
// # define Ts           0.0001              // control period
// # define Udc          12                  // bus voltage
# define PI           3.141592653

// 0.1us/bit
#define Ts  1000
#define Udc 12000
			   
////////////////////////////////////////////////////////////////////////////////
//
void Clark(I_CLARK_PARK *v);
void Park(I_CLARK_PARK *v);
void IPark(V_CLARK_PARK *v);
void Current_Convert_Voltage(V_CLARK_PARK *v,I_CLARK_PARK *I);
void motor_sector_cal(V_CLARK_PARK *v);
void motor_sector_switch_cal(V_CLARK_PARK *v);

////////////////////////////////////////////////////////////////////////////////
// For motor with resolver
#define POLE_PAIR_NUM (1)

// For motor with MR angle sensor
// #define POLE_PAIR_NUM (4)

#define F_ANGLE(x) ((x)*10)
#define ANGLE_PER_POLE_PAIR (F_ANGLE(360)/POLE_PAIR_NUM)

#endif /* SVPWM_H_ */
////////////////////////////////////////////////////////////////////////////////
