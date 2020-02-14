/*
 * PID_st.h
 *
 *  Created on: 2017Äê9ÔÂ17ÈÕ
 *      Author: dwl
 */
#include "std_defs.h"
#ifndef _PID_H
#define _PID_H
//---------------

typedef struct{
               f32 kp;            // Q24
               f32 ki;            // Q24
               f32 kd;            // Q24
	             f32 kp_stg2;       // Q24
               f32 ki_stg2;       // Q24
               f32 kd_stg2;       // Q24
               f32 delta_kp;      // Q24
               f32 delta_ki;      // Q24
               f32 delta_kd;      // Q24
               s32 actual_value;  // actual value of the control variable    Q16
               s32 desire_value;  // desired value of the control variable   Q16
               s32 lowscale;      // a minimum sacle factor the control variable, if actual value < actual value*lowscale,dont use integral   Q24
               s32 highscale;     // a maximun sacle factor the control variable, if actual value > actual value*highscale,dont use integral  Q24
               s32 max;           // output max value   Q24
               s32 min;           // output min value   Q24
               s32 ki_index;      // Q24
               s32 kd_index;      // Q24
               s32 kp_index;      // Q24
               s32 sum_err;
               s32 pre_err;
               s32 err;
               f32 output;        // Q24
               }PID_st;
//------------

//-------function declaration
s16 PID_realize(PID_st *p);
void PID_Init(PID_st *p);
void PID_Config(PID_st *p, s32 kp,s32 ki,s32 kd,s32 lowscale,s32 highscale,s32 min ,s32 max);

//-------------------------

#endif /* PID_H_ */
