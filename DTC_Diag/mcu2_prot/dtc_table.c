#include "std_defs.h"
#include "dtc_man.h"

const GDTC xdtc[]={
	{0x55,0xaa33},
	{ DTC_ECU_HW     , 0x5000 }, // 0x80 + 0x01,
	{ DTC_ECU_SW     , 0x5010 }, // 0x80 + 0x02,
	{0,0}
};
