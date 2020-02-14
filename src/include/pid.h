/*
 * pid.h
 *
 *  Created on: 2017Äê9ÔÂ17ÈÕ
 *      Author: dwl
 */

#ifndef PID_H_
#define PID_H_
//---------------
typedef struct{
               _iq kp;            // Q24
               _iq ki;            // Q24
               _iq kd;            // Q24
               _iq delta_kp;      // Q24
               _iq delta_ki;      // Q24
               _iq delta_kd;      // Q24
               _iq actual_value;  // actual value of the control variable    Q16
               _iq desire_value;  // desired value of the control variable   Q16
               _iq lowscale;      // a minimum sacle factor the control variable, if actual value < actual value*lowscale,dont use integral   Q24
               _iq highscale;     // a maximun sacle factor the control variable, if actual value > actual value*highscale,dont use integral  Q24
               _iq max;           // output max value   Q24
               _iq min;           // output min value   Q24
               _iq ki_index;      // Q24
               _iq kd_index;      // Q24
               _iq kp_index;      //Q24
               _iq sum_err;
               _iq pre_err;
               _iq err;
               _iq output;        // Q24
               } PID;
//------------

//-------function declaration
_iq PID_realize(PID *p);
void PID_Init(PID *p);
void PID_Config(PID *p, float kp,float ki,float kd,float lowscale,float highscale,float min ,float max);

//-------------------------

#endif /* PID_H_ */
