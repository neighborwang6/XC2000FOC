///////////////////////////////////////////////////////////////////////////////////////
/*

*/
///////////////////////////////////////////////////////////////////////////////////////
#include "stdlib.h"
#include "string.h"
#include "dtc_man_mcu2.h"

extern SDTC *dtc_pt;
extern const GDTC xdtc[];

///////////////////////////////////////////////////////////////////////////////////////
// Retrieve a DTC info
u8 dtc_infox(u8 idh,u8 idl,u8 *buf)
{
	UNUSED_PARA(idh);
	UNUSED_PARA(idl);
	UNUSED_PARA(buf);
	
	return 0;
}
