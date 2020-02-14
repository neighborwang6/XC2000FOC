////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
* High Layer CAN Driver Implementation
*  
*      
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

#include "hican.h"
#include "xcpbasic.h"
#include "motor_if.h"
#include "ctrl_if.h"
#include "aut_drv.h"

#ifdef WIN32
#include "pc_hw.h"
#endif

#define STANDARD_ID
#define EXTENDEDIDS
////////////////////////////////////////////////////////////////////////////////
// CAN IDs: Diagnose
u16 g_diag_tester_id = 0x7C0;
u16 g_diag_resp_id   = 0x7C1;

// CAN IDs: XCP
u16 g_cto_id         = 0x60A;
u16 g_dto_id         = 0x60B;

u16 wgy = 0;

////////////////////////////////////////////////////////////////////////////////
// Init CAN application layer
////////////////////////////////////////////////////////////////////////////////
u8 hican_init(void)
{
	#ifdef STANDARD_ID
	
	// Note: Message Box: 0~15 => CAN node 0, 16~31 => CAN node 0
	
	// Message Box 4~9: Transmit (Standard IDs)
    can_load(4,0x304,0x80,(u8 *)"--------");
    can_load(5,0x103,0x80,(u8 *)"--------");
    can_load(6,0x104,0x80,(u8 *)"--------");
    can_load(7,0x105,0x80,(u8 *)"--------");
    can_load(8,0x106,0x80,(u8 *)"--------");
    can_load(9,0x107,0x80,(u8 *)"--------");
	
	// Message Box 12: Accept all IDs
	//can_rx(12,0,0|0x800,0x80);
	
	// Message Box 0/1: Accept the IDs specified
	can_rx(0,g_cto_id,        0x7FF|0x800,0x80);
	can_rx(1,g_diag_tester_id,0x7FF|0x800,0x80);
	// Message Box 2/3: Will send the IDs specified
	//can_load(2,g_dto_id,      0x80,(u8 *)"--------");
	//can_load(3,g_diag_resp_id,0x80,(u8 *)"--------");
	#endif

  #ifdef EXTENDEDIDS
	// Message Box 16~19: Transmit (Extended IDs)
		can_load(16,0x18F0010B,0x84,(u8 *)"--------"); // Node 0, 29bit-id
    can_load(17,0x18FF020B,0x84,(u8 *)"--------"); // Node 0, 29bit-id
    can_load(18,0x18FF030B,0x84,(u8 *)"--------"); // Node 0, 29bit-id
    can_load(19,0x18FF040B,0x84,(u8 *)"--------"); // Node 0, 29bit-id
		
	// Message Box 28~31: Accept the IDs specified
	  can_rx(31,0x089F6265,0x1FFFFFFF|0x20000000,0x84); // Node 0
    can_rx(30,0x1CF00401,0x1FFFFFFF|0x20000000,0x84); // Node 0
    can_rx(29,0x0CF00402,0x1FFFFFFF|0x20000000,0x84); // Node 0 
    can_rx(28,0x0CF00403,0x1FFFFFFF|0x20000000,0x84); // Node 0
	//can_rx(31,0x110,0x7FF|0x800,0x80); // Node 0
  //can_rx(30,0x110,0x7FF|0x800,0x80); // Node 0
  //can_rx(29,0x110,0x7FF|0x800,0x80); // Node 0
  //can_rx(28,0x110,0x7FF|0x800,0x80); // Node 0
	
	// Message Box 27: Accept all IDs
	//can_rx(31,0x0CF00400,0x20000000,0x84);
	
	//can_rx(31,0x0CF00400,0x1FFFFFFF|0x20000000,0x85); // Node 1
  //can_rx(30,0x0CF00401,0x1FFFFFFF|0x20000000,0x85); // Node 1
  //can_rx(29,0x0CF00402,0x1FFFFFFF|0x20000000,0x85); // Node 1
  //can_rx(28,0x0CF00403,0x1FFFFFFF|0x20000000,0x85); // Node 1
#endif

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// 10ms task
////////////////////////////////////////////////////////////////////////////////
void hican_10ms_hook(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// 1ms task
////////////////////////////////////////////////////////////////////////////////
void hican_1ms_hook(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// CAN bus off reccovery: reserved
////////////////////////////////////////////////////////////////////////////////
// 10ms task
u8 do_boff_recovery(void)
{
	static u8 l_timer_boff=0;

	if(get_can_bus_off())
	{
		l_timer_boff++;
		// Node Initialization retry: 500ms after Bus off
		if(l_timer_boff>=50) // 500ms
		{
			set_can_on();
			l_timer_boff=0;
		}
	}
	else
		l_timer_boff=0;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Callback function: called when CAN message received
////////////////////////////////////////////////////////////////////////////////
void can_do_objx_hook_hican(u8 no,u32 id,u8 far * buf,u8 len)
{
	u8 bufx[32];
	u8 idx;
	
	UNUSED_PARA(no);
	UNUSED_PARA(buf);
	UNUSED_PARA(len);
	
	for(idx=0;idx<len;idx++)
	{
		bufx[idx]=buf[idx];
	}
	
	if(id==g_diag_tester_id)
	{
		// Parsing the Tester command: ISO 15765
		// do_diag_cmd(buf,len);
		return;
	}
	if(id==g_cto_id)
	{
		// Parsing the XCP CTO command
		XcpCommand((u32 *)bufx);

#ifdef WIN32
		show_msglist("XCP:[%02X %02X %02X %02X %02X %02X %02X %02X]",
			bufx[0],bufx[1],bufx[2],bufx[3],
			bufx[4],bufx[5],bufx[6],bufx[7]
			);
#endif
		return;
	}
	//Auto Drive Commend
	if(id == 0x089F6265)
	{
		do_aut_drv_response(buf);
	}
	if(id == 0x0CF00401)
	{
		wgy = 1;
	}
	if(id == 0x0CF00402)
	{
		wgy = 1;
	}
	if(id == 0x0CF00403)
	{
		wgy = 1;
	}
	if(id==0x401)
	{
		do_motor_bldc_debug(buf);
	}

	can_do_obj(id,len,buf);
}
////////////////////////////////////////////////////////////////////////////////
