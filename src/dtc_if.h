#ifndef _DTC_IF_H
#define _DTC_IF_H

#include "std_defs.h"

extern u8 gee_dirty2;
u8 dtc_add(u8 id,u8 fm,u8 reserved);
u8 dtc_del(u8 id,u8 flag);

extern void dtc_clr_init(void);
extern u8 dtc_man_init(void);
// id: must be continuous
enum {
	DTC_ECU_HW     = 0x80 + 0x01,
	DTC_ECU_SW     = 0x80 + 0x02,
	DTC_BAT_LOW    = 0x80 + 0x03,
	DTC_BAT_HIGH   = 0x80 + 0x04,
	DTC_BRAKE      = 0x80 + 0x05,
	DTC_SOL_RELAY  = 0x80 + 0x06,
	DTC_MOT_RELAY  = 0x80 + 0x07,
	DTC_ABS_LAMP   = 0x80 + 0x08,   // FIXME : reserved
	DTC_GSENSOR    = 0x80 + 0x09,
	DTC_SWITCH_4W  = 0x80 + 0x0a,   // 4W Switch
	DTC_SWITCH_CL  = 0x80 + 0x0b,   // Central Differential Lock
	DTC_SWITCH_RL  = 0x80 + 0x0c,   // Rear Axial Differential Lock
	DTC_SOL_FLIN   = 0x80 + 0x0d,
	DTC_SOL_FLOUT  = 0x80 + 0x0e,
	DTC_SOL_FRIN   = 0x80 + 0x0f,
	DTC_SOL_FROUT  = 0x80 + 0x10,
	DTC_SOL_RLIN   = 0x80 + 0x11,
	DTC_SOL_RLOUT  = 0x80 + 0x12,
	DTC_SOL_RRIN   = 0x80 + 0x13,
	DTC_SOL_RROUT  = 0x80 + 0x14,
	DTC_HCU_FL     = 0x80 + 0x15,
	DTC_HCU_FR     = 0x80 + 0x16,
	DTC_HCU_RL     = 0x80 + 0x17,
	DTC_HCU_RR     = 0x80 + 0x18,
	DTC_SENSOR_FL0 = 0x80 + 0x19,  // Short/Open
	DTC_SENSOR_FR0 = 0x80 + 0x1a,
	DTC_SENSOR_RL0 = 0x80 + 0x1b,
	DTC_SENSOR_RR0 = 0x80 + 0x1c,
	DTC_SENSOR_FL1 = 0x80 + 0x1d,  // Invalid/No signal
	DTC_SENSOR_FR1 = 0x80 + 0x1e,
	DTC_SENSOR_RL1 = 0x80 + 0x1f,
	DTC_SENSOR_RR1 = 0x80 + 0x20,
	DTC_SENSOR_FL2 = 0x80 + 0x21,  // Performance
	DTC_SENSOR_FR2 = 0x80 + 0x22,
	DTC_SENSOR_RL2 = 0x80 + 0x23,
	DTC_SENSOR_RR2 = 0x80 + 0x24,
	DTC_EEPROM     = 0x80 + 0x25   // FIXME: Record in memory, but show a DTC_ECU_HW to tester
};

#endif /* define _DTC_IF_H */


