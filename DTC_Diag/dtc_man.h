#ifndef _DTC_MAN_H
#define _DTC_MAN_H

#include "std_defs.h"
#include "eeprom.h"
#include "ypara.h"

extern u8 gee_dat[];
extern u8 gee_cmd;
extern u8 gee_cmd_bak;
extern u8 gdtc_num;
extern u8 gdtc_num_active;
extern u8 *gdtc_buf;
extern u8 *gpara_buf;

u8 dtc_man_init(void);
u8 dtc_add(u8 id,u8 fm,u8 env);
u8 dtc_del(u8 id,u8 flag);
u8 dtc_info(u8 id,u8 *buf);
u8 dtc_list(u8 type,u8 *buf);
u8 dtc_man_nsr(void);
u8 ign_add(void);
u8 dtc_del_all(void);

extern void dtc_clr_init(void);

// #pragma pack(1)

////////////////////////////////////////////
typedef struct tagSDTC {
	u8 id;
	u8 fm;   // Fail mode (present or no Bit[7-5], error type Bit[4-0], SEE FIAT 9141 Doc)
	u8 env;
	u8 cnt;  // Fault number
}SDTC;

// #pragma pack()

// id: must be continuous
enum {
	DTC_ECU_HW     = 0x80 + 0x01,
	DTC_ECU_SW     = 0x80 + 0x02,
	DTC_BAT_LOW    = 0x80 + 0x03,
	DTC_BAT_HIGH   = 0x80 + 0x04,
	DTC_EEPROM     = 0x80 + 0x05   // FIXME: Record in memory, but show a DTC_ECU_HW to tester
};
// Infomation Header  (Fix to 8 Bytes)
// 0x55+0xaa+rsv0+rsv1+EEPROM_VERSION (4 bytes)
#define EE_HEAD_ADDR 0x00
// Parameter
#define EE_PARA_ADDR 0x08
// Ignition number (2 bytes)
#define IGN_ADDR 0x08
// DTC address in EEPROM
#define DTC_ADDR     0x10
#define DTC_ADDR_END 0x40

// User Space
#define USER_ADDR     0x40
#define USER_ADDR_END 0x50

// User Space 0: dtc_byte [0x40,0x47]
#define USER_ADDR0    0x40
// User Space 1: WriteDataByLocalIdentifier Erea
#define USER_ADDR1_FILLIN    0x48
#define USER_ADDR1_EOL       0x4a
#define USER_ADDR1_SN        0x4c

// Calibration Parameter address in EEPROM
#define CAL_ADDR     0x50
#define CAL_ADDR_END 0x80
#define CAL_SIZE     (CAL_ADDR_END-CAL_ADDR)

// AT25010/020/040: 128/256/512 Bytes
#define EE_SIZE  0x80

#define DTCNUM_MAX 12

/////////////////////////////////////////////////////////////////////////////
typedef struct tagGDTC {
	u8 id;      // Internal ID
	u16 xid;    // External ID (standard, eg. C1101,C2402,...)
	u8 failtype;// Fail Type
}GDTC;
					
/////////////////////////////////////////////////////////////////////////////

#endif /* define _DTC_MAN_H */


