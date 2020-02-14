#ifndef _MCU1_IMP_H
#define _MCU1_IMP_H

#include "std_defs.h"
#include "recfmt.h"

extern MCUMSG2 mcumsg2;

#define DUMMY_BYTE 0x00
#define VSPEED_LIMITL  FKMH(10.0)
#define VSPEED_LIMITH  FKMH(40.0)

void do_mcu2_msg(void);
void mcu1_imp_init(void);
void do_mcu1(void);
u8 calc_chksum(u8 *buf,u8 buflen);

#endif /* define _MCU1_IMP_H */
