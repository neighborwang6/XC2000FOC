#ifndef _RECFMT_H
#define _RECFMT_H

#include "std_defs.h"

#define MAGIC_MCU1 ((u8)0xa5)
#define MAGIC_MCU2 ((u8)0xb5)

#define MAGIC_MCU1_DAT ((u8)0xc5)
#define MAGIC_MCU2_DAT ((u8)0xd5)

#define MCUMSG_LEN 25
// #pragma pack(1)
// MCUMSG_LEN Bytes
typedef struct tagMCUMSG1 {
	u8 magic_src;
	u8 magic_tgt;
	u8 sn;
	u8 busy;
	u16 vfl;
	u16 vfr;
	u16 vrl;
	u16 vrr;
	u16 vref;
	u8 sol_status;
	u8 task_status;
	u8 misc_status;
	u8 vbat;
	u8 ee_flag;
	u8 ee_option;
	u8 ee_dat[9];
	u8 chksum;
}MCUMSG1;

// MCUMSG_LEN Bytes
typedef struct tagMCUMSG2 {
	u8 magic_src;
	u8 magic_tgt;
	u8 sn;
	u8 busy;
	u8 task_cmd;
	u8 task_dat[4];
	u8 sol_cmd;
	u8 mot_cmd;
	u8 misc_cmd;
	u8 misc_status;
	u8 ee_cmd;
	u8 ee_option;
	u8 ee_dat[9];
	u8 chksum;
}MCUMSG2;

typedef struct tagMCUMSGDAT {
	u8 magic_src;
	u8 magic_tgt;
	u8 sn;
	u8 option;
	u8 dat[MCUMSG_LEN-5];
	u8 chksum;
}MCUMSGDAT;

// #pragma pack()

enum {
	TASK_CMD_NONE         = 0xa0,
	TASK_CMD_DIAGIN       = 0xa1,
	TASK_CMD_DIAGOUT      = 0xa2,
	TASK_CMD_TASK3        = 0xa3,
	TASK_CMD_TASK4        = 0xa4,
	TASK_CMD_ACT_NONE     = 0x40,
	TASK_CMD_ACT_DCTRL    = 0x41,   // DelayedActuatorControl
	TASK_CMD_ACT_PCTRL    = 0x42,   // PeriodicActuatorControl
	TASK_CMD_ACT_TEST     = 0x43,   // ActuatorTest
	TASK_CMD_DYN_TEST     = 0x44,   // DynamicTest
	TASK_CMD_WSENSOR_TEST = 0x45,   // Wheel Speed Sensors Test
	TASK_CMD_EMSTOP       = 0x46    // Emergency Stop
};

enum {
	EE_CMD_INVALID= 0x00,
	EE_CMD_NONE   = 0xa0, // default to EE_CMD_RD_DTC
	EE_CMD_WRITE  = 0xa1,
	EE_CMD_READ   = 0xa2,
	EE_CMD_INIT   = 0xa3,
	EE_CMD_RD_DTC = 0xa4,
	EE_CMD_WR_DTC = 0xa5
};

enum {
	MSGDAT_NONE     = 0x50,
	MSGDAT_EEPROM   = 0x51,
	MSGDAT_ECU_ID   = 0x52,
	MSGDAT_SYS_ID   = 0x53,
	MSGDAT_CAR_ID   = 0x54,
	MSGDAT_HW_VER   = 0x55,
	MSGDAT_SW_VER   = 0x56,
	MSGDAT_FLASH    = 0x57
};

enum {
	SN_MSG_NORMAL   = 0x50,
	SN_MSG_DAT      = 0x60
};

enum {
	MSTAT_BRAKE    = 0x01,
	MSTAT_ABS      = 0x02,	   // ABS On/OFF
	MSTAT_LAMP     = 0x04,
	MSTAT_SYSFAIL  = 0x08,
	MSTAT_SW4W     = 0x10,
	MSTAT_SWCLOCK  = 0x20,
	MSTAT_SOLRELAY = 0x40,
	MSTAT_MOTRELAY = 0x80
};

enum {
	MCMD_MOTOR     = 0x01,
	MCMD_LAMP      = 0x02
};

enum {
	MSTAT2_RLDRV   = 0x01,
	MSTAT2_LAMP    = 0x02,
	MSTAT2_SMODE   = 0x04,   // Super Mode
	MSTAT2_SYS_OK  = 0x08
};

enum {
	MBUSY2_NONE           = 0x00,
	MBUSY2_TASK_ACT       = 0x40,   // bit7-6: task group (Aactuator: 01), bit5-4: task status, bit3-bit0: task id
	MBUSY2_TASK_ACT_DCTRL = 0x41,   // DelayedActuatorControl
	MBUSY2_TASK_ACT_PCTRL = 0x42,   // PeriodicActuatorControl
	MBUSY2_TASK_ACT_TEST  = 0x43,   // ActuatorTest
	MBUSY2_TASK_DYN_TEST  = 0x44    // DynamicTest
};

enum {
	TASK_STAT_NONE      = 0x00,
	TASK_STAT_STARTED   = 0x01,
	TASK_STAT_COMPLETE  = 0x02,
	TASK_STAT_RUNNING   = 0x04,
	TASK_STAT_STOPPED   = 0x08,
	TASK_STAT_FAIL      = 0x10
};

#endif /* define _RECFMT_H */
