#ifndef _TASK_MAN_H
#define _TASK_MAN_H

#include "std_defs.h"
#include "diag_hw.h"

enum {
	ACTNUM_NONE         =0x00,
	ACTNUM_ABSLAMP      =0x02,
	ACTNUM_EBDLAMP      =0x0e,
	ACTNUM_MOTRLY       =0x22,
	ACTNUM_VINFL        =0x30,
	ACTNUM_VOUTFL       =0x32,
	ACTNUM_VINFR        =0x34,
	ACTNUM_VOUTFR       =0x36,
	ACTNUM_VINRR        =0x38,
	ACTNUM_VOUTRR       =0x3a,
	ACTNUM_VINRL        =0x3c,
	ACTNUM_VOUTRL       =0x3e,
	ACTNUM_OVHEAT       =0x5e,
	ACTNUM_SOLRLY_DEACT =0x64,
	ACTNUM_SPDLMT       =0x68
};

void do_mcu2_task(void);
void do_mcu2_task_init(void);
void do_mcu2_task_10ms_hook(void);

////////////////////////////////////////////////
extern u8  gmisc_task;
extern u8  gmisc_task_stat;
extern u8  gmisc_task_option[];
extern u16 gmisc_task_tout;
extern u16 gmisc_task_wspd_data[];

u8 do_mcu2_taskx(void);
void do_mcu2_taskx_init(void);
void do_mcu2_taskx_10ms_hook(void);
////////////////////////////////////////////////

#endif /* define _TASK_MAN_H */

