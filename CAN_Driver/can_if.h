////////////////////////////////////////////////////////////////////////////////
#ifndef _CAN_IF_H
#define _CAN_IF_H

#include "std_defs.h"
////////////////////////////////////////////////////////////////////////////////

extern u8 g_can_status;	  // bit0: bus off(1); bit1: tx ok, bit2: rx ok

////////////////////////////////////////////////////////////////////////////////
void can_tx(u8 no,u32 id,u8 cfg,u8 *buf);
void can_rx(u8 no,u32 id,u32 mask,u8 cfg);
void can_stx(u8 no);
void can_sload(u8 no,u8 *buf);
void can_do_objx_hook_user(u8 no,u32 id,u8 far * buf,u8 len);

void can_load(u8 no,u32 id,u8 cfg,u8 *buf);
u8   can_stx_ex(u8 no);
void set_can_on(void);
u8 get_can_bus_off(void);

////////////////////////////////////////////////////////////////////////////////

#endif /* define _CAN_IF_H */
