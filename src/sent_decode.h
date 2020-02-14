#ifndef _SENT_DECODE_H
#define _SENT_DECODE_H

#include "std_defs.h"

extern u8 sent_data[];

u8 calc_16_array(u8 dat_len, u8 *buf);
u8 sent_decode(u32 timer, u8 option);

#endif /* define _SENT_DECODE */
