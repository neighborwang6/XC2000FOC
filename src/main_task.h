#ifndef _MAIN_TASK_H
#define _MAIN_TASK_H

#include "std_defs.h"
#include "board.h"

void main_task_init(void);

#define main_task_start() GPT12E_T2CON_T2R=1
#define main_task_stop()  GPT12E_T2CON_T2R=0

// Reference: Table 15-7 GPT1 Overall Prescaler Factors for Internal Count Clock 
// [user's manul v1.2 2009-07 xc2300b]
#define main_task_setperiod(x) GPT12E_T2=(u16)(65536UL-((x)*FSYS_MHZ*1000/32))

#endif /* define _MAIN_TASK_H */
