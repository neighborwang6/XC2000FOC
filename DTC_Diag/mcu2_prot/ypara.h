#ifndef _YPARA_H
#define _YPARA_H

#include "std_defs.h"

#define CAR_TYPE_EJ02

#define HEAD_MAGIC 0x55aa44bbUL
#define TAIL_MAGIC 0xaa55bb44UL

// #pragma pack(1)
typedef struct tagYPARA {
	u32 head_magic;
	char car_id[8];
	char sys_id[8];
	char hw_ver[8];
	char sw_ver[8];
	char mf_id[8];
	u8 sn[12];
	u8 br;         // Baud Rate  : 0xae: 9600, 0x9f: 10400
	u8 tgt_addr;
	u8 src_addr;
	u8 vspd_limit;
	u8 limit_en;
	u8 scale_table[24];
	u8 data_table[40];
	u32 tail_magic;
}YPARA;
// #pragma pack()

extern const YPARA g_ypara;

#define ECU_ADDR g_ypara.tgt_addr
#define TST_ADDR g_ypara.src_addr

#endif /* define _YPARA_H */

