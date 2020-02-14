#ifndef _MCU2_IMP_H
#define _MCU2_IMP_H

#include "std_defs.h"
#include "recfmt.h"

extern MCUMSG1  mcumsg1;
extern MCUMSG2  mcumsg2;
extern u8 sys_health[];

void mcu2_imp_init(void);
void do_mcu2(void);
void do_mcu1_msg(void);
void submit_task(u8 task_id,u8 *para,u8 len);

#endif /* define _MCU2_IMP_H */
