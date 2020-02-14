#include "ypara.h"
#include "prot_imp.h"

const YPARA g_ypara={
	HEAD_MAGIC,
	"L330",
	"ABS",
	"\x006\x042\x028",
	"\x001\x000\x000\x000",
	"\x002\x065\x080\x007\x013",
	"1122334455",
	0x9f,         // Baud Rate  : 0xae: 9600, 0x9f: 10400
	ECU_ADDR_CONST,
	TST_ADDR_CONST,
	FKMH_R(40),
	1,            // No limit to V Speed and ABS On/Off
	// Scaling Table
	"\x003\x091\x06B\x003\x09A\x002\x003\x097\x066\x003\x092\x045\x003\x094\x065\x003\x095\x068\x003\x09D\x001\x0FF",
	// Data Table
	   "E02"
	   "ABS"
	   "BBN"
	   "0708"
	   "YF_83",
	TAIL_MAGIC
};
