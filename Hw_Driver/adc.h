#ifndef _ADC_H
#define _ADC_H

#include "std_defs.h"
#include "hw.h"

void ad_setting(void);
void ad_scan(void);

extern u16 g_ad0[];
extern u16 g_ad1[];

u16 get_ad_value(u8 chan_no);

#endif /* define _ADC_H */
