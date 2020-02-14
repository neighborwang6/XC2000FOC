////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
*
*  
*      
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

#ifndef _DIAG_HW_H
#define _DIAG_HW_H

#include "std_defs.h"

extern u8 sys_health[];

// System error if set to non-Zero.
extern u16 g_sys_dtc_uw;

void diag_hw_init(void);
void diag_in_hook(void);
void diag_out_hook(void);

u16 spi_mcu2_trans(u16 ch);
void spi_mcu2_putc(u16 ch);

void diag_timer_10ms_hook(void);
void diag_timer_50ms_hook(void);
void diag_main_loop_hook(void);

enum {
	HEALTH_IDX_SPI    = 0,
	HEALTH_IDX_DIAG   = 1,	   // MCU2 DTC status
	HEALTH_IDX_SENSOR = 2,
	HEALTH_IDX_MAX    = 2
};

#endif /* define _DIAG_HW_H */
////////////////////////////////////////////////////////////////////////////////

