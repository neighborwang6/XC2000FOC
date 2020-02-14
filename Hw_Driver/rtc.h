#ifndef _RTC_H
#define _RTC_H

#include "std_defs.h"
#include "hw.h"

extern u16 volatile g_ms_uw;
extern u16 g_min;
extern u16 g_hour;
extern u16 g_day;

void rtc_setting(void);

#endif /* define _RTC_H */
