///////////////////////////////////////////////////////////////////////////////////////
/*

*/
///////////////////////////////////////////////////////////////////////////////////////
#include "prot_imp.h"
#include "board.h"
#include "std_diag.h"
#include "mcu2_imp.h"
#include "dtc_man.h"
#include "ypara.h"
#include "dtc_man.h"
#include "dtc_man_mcu2.h"
#include "task_man.h"

#define GET_RLDRV() (1)
#define SET_RLDRV(x)

extern u8 g_com_on;

extern code GDTC xdtc[];
extern SDTC *dtc_pt;
extern u16 xdata vspd[];
extern u8 xdata *gdtc_buf;
extern u8 gdtc_num;
extern u8 stx_option;

extern u8 xdata gee_dat[32];
extern u8 gee_cmd;
extern u8 gee_cmd_bak;
extern u8 gee_stat;

extern MCUMSG1 xdata mcumsg1;

extern u8 gee_buf[];

extern u16 on_toutx;

u8 g_wheelspeed_test_status=0;   // bit0: 0-idle, bit1: 1-result available bit2: 1-test running

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
// Because RAM limit, declare common used temporary variables here.
u8 g_tmp=0;
u8 g_tmp0=0;
u8 g_tmpbuf[8];

u8 prod_mode=0xaa; // 0x05: EOL mode disable, 0x50: FILLIN mode disable=>0x55,
// if 0xaa: no DTC will be stored to EEPROM and ABS function
// disabled.
u8 security_mode=0;

u8 comp_on_tout[10]={0,0,0,0,0,0,0,0,0,0}; // SOL,MOTOR,LAMP

u8 diag_mode=DIAG_MODE_NONE;

u8 gact_task=TASK_CMD_ACT_NONE;
u8 gact_task_stat=TASK_STAT_COMPLETE;  // Important to init this value
u16 gact_task_tout=0;
u8 routine_id=0xff;

u8 g_seed[4];

u8 g_msg_offset=0;
u8 g_msg_len=0;
u8 g_msg_error=0;   // BIT0: invalid message
u8 g_msg_data_len=0;

#define SID   inbuf[offset+0]
#define PARA0 inbuf[offset+1]
#define PARA1 inbuf[offset+2]
#define PARA2 inbuf[offset+3]
#define TGT_ADDR inbuf[1]
#define SRC_ADDR inbuf[2]

u8 do_tester_msg(u8 *inbuf,u8 *outbuf,u8 msglen,u8 option)
{
	UNUSED_PARA(inbuf);
	UNUSED_PARA(outbuf);
	UNUSED_PARA(msglen);
	UNUSED_PARA(option);

	return 0;
}
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
u8 get_msg_len(u8 *buf,u8 len)
{
	UNUSED_PARA(buf);
	UNUSED_PARA(len);

	return 0;
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
u8 get_sid_offset(u8 *buf,u8 len)
{
	UNUSED_PARA(buf);
	UNUSED_PARA(len);

	return 0;
}
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
u8 calc_chksum(u8 *buf,u8 buflen)
{
	u8 idx;
	u8 sum=0;
	for(idx=0;idx<buflen;idx++)
	{
		sum+=buf[idx];
	}   
	
	return sum;
}
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void timer_hook_50ms(void)
{

}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void do_component_test(u8 sid,u8 para0,u8 para1,u8 option)
{
	UNUSED_PARA(sid);
	UNUSED_PARA(para0);
	UNUSED_PARA(para1);
	UNUSED_PARA(option);	
}
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
