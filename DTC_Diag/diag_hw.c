////////////////////////////////////////////////////////////////////////////////
//
#include "diag_hw.h"
#include "mcu1_imp.h"
#include "mcu2_imp.h"
#include "dtc_man.h"
#include "hican.h"

////////////////////////////////////////////////////////////////////////////////
//

void ad_scan(void);
u8 can_test_init(void);

extern void mcu1_imp_init(void);

extern u16 diag_timeout; 
extern u16 volatile g_ms_uw;

u8 sys_health[HEALTH_IDX_MAX+1];

// Specify if the ECU in diag mode (Normal mode initially)
u8 g_diag_stat=0;

////////////////////////////////////////////////////////////////////////////////
//
void diag_hw_init(void)
{
   mcu1_imp_init();

   dtc_man_init();
   can_test_init();

   hican_init();

   sys_health[HEALTH_IDX_SPI]=200;
   sys_health[HEALTH_IDX_DIAG]=0;
}

////////////////////////////////////////////////////////////////////////////////
//
void do_spi_char(u8 ch)
{
	ch=ch;
}

////////////////////////////////////////////////////////////////////////////////
//
void diag_main_loop_hook(void)
{
	///////////////////////////////////////////////////////
	// Post Prossess the SPI Message
	do_mcu1_msg();
	do_mcu2_msg();
	///////////////////////////////////////////////////////
	// Deal with the DTC change
	dtc_man_nsr();

	///////////////////////////////////////////////////////
	ad_scan();            // FIXED: yly : why put it here :(

	///////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
// Enter diagnostic mode
void diag_in_hook(void)
{
   // diag_in_hook_user();
}

////////////////////////////////////////////////////////////////////////////////
// Leave diagnostic mode
void diag_out_hook(void)
{
}
