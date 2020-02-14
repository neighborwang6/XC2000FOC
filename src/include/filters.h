
/**********************************************************************************
    Author: DAE, Tsinghua Univ.
    Date: 2016.12.30
    Version:
        1.0
**********************************************************************************/
#ifndef _FILTER_H
#define _FILTER_H

#include "std_defs.h"

u16 filter_currentA(u16 currentA_adc);
u16 filter_currentB(u16 currentB_adc);
u16 filter_currentC(u16 currentC_adc);
u16 filter_currentM(u16 currentM_adc);
u16 filter_currentD(u16 currentD_calc);
u16 filter_currentQ(u16 currentQ_calc);

#endif /* define _FILTER_H */
