/**********************************************************************************
    Author: DAE, Tsinghua Univ.
    Date: 2016.12.30
    Version:
        1.0
**********************************************************************************/
#ifndef _FILTER_EBSSENSORS_H
#define _FILTER_EBSSENSORS_H
#include "std_defs.h"

u16 filter_pres_sense(u16 pres_sense_get);  //filt the pressure
s32 filter_disp_sense(s32 disp_sense_get);  //filt the displacement

#endif /* define _FILTER_H */
