/*******************************************************************************
	
	Modify History:
	xxxx.xx.xx
	
*******************************************************************************/

#ifndef _RESOLVER_AD2S_H
#define _RESOLVER_AD2S_H

#include "board.h"
#include "std_defs.h"

u8 resolver_ad2s_init(u8 option);
void resolver_ad2s_wr_cfg(u8 addr, u8 dat);
u8 resolver_ad2s_rd_cfg(u8 addr);
u32 resolver_ad2s_read(u8 option);
u16 resolver_ad2s_read_pos(u8 option);

#endif /* define _RESOLVER_AD2S_H */

