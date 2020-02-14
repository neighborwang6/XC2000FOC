#include "stdlib.h"
#include "string.h"
#include "dtc_man.h"
#include "recfmt.h"

///////////////////////////////////////////////////////////////////////////////////////
// EEPROM About
// Mirror of EEPROM
u8 gee_buf[EE_SIZE];

// Mirror of EEPROM DTC erea
u8 *gdtc_buf=NULL; // DTC num: 12, 4bytes per DTC
// From MCUMSG2
u8 gee_cmd=EE_CMD_NONE;
u8 gee_cmd_bak=EE_CMD_NONE;
u8 gee_dat[32];  // First byte: address info
// EEPROM operation result
u8 gee_stat=0;
// Mark EEPROM chagnes
u16 gee_dirty=0;  // every bit => one DTC memory position
u16 gee_dirty1=0; // Calibration Parameter, 8bytes/bit (LSB->MSB), use u32 for more Parameter
u8 gee_dirty2=0;  // bit0: Used for dtc_byte && dtc_byte_bak
u8 gdtc_num=0;    // all DTCs
u8 gdtc_num_active=0;   // DTCs that active (present now)

// Point to gdtc_buf
SDTC *dtc_pt;

u8 *gpara_buf=NULL;

const char EE_VER[]="Y001";

extern u8 dtc_byte[];
extern u8 dtc_byte_bak[];
extern u8 calc_chksum(u8 *buf,u8 len);

///////////////////////////////////////////////////////////////////////////////////////
//
u8 dtc_man_init(void)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Add a DTC
u8 dtc_add(u8 id,u8 fm,u8 env)
{
	UNUSED_PARA(id);
	UNUSED_PARA(fm);
	UNUSED_PARA(env);
	
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////
// Del a DTC
// flag: 0 delete, 1 make no present
u8 dtc_del(u8 id,u8 flag)
{
	UNUSED_PARA(id);
	UNUSED_PARA(flag);
	
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////
//
u8 dtc_del_all(void)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// List all active/de-active DTC 
// type: 0x60 active,0x40 de-active
// return DTC number requested
u8 dtc_list(u8 type,u8 *buf)
{
	UNUSED_PARA(type);
	UNUSED_PARA(buf);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Retrieve a DTC info
u8 dtc_info(u8 id,u8 *buf)
{
	UNUSED_PARA(id);
	UNUSED_PARA(buf);
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Return ignition number before
u8 ign_add(void)
{
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////
// DTC manager routine
u8 dtc_man_nsr(void)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// update the image of EEPROM
u8 eeprom_wr_hook(u16 addr,u8 len,u8 *buf)
{
	memcpy(gee_buf+addr,buf,len);
	return 0;
}
