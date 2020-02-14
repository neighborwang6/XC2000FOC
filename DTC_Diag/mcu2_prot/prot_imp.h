#ifndef _PROT_IMP_
#define _PROT_IMP_

#include "std_defs.h"

#define KB1  ((u8)0x6b)       
#define KB2  ((u8)0x8f)

#define ECU_ADDR_CONST ((u8)0x28)
#define TST_ADDR_CONST ((u8)0xf1)

extern u8 comp_on_tout[];

#define FKMH_R(x) (x)
#define FVOLT(x) ((u8)(5.0*(x)/255.0))

enum {
   DIAG_MODE_NONE  = 0,
   DIAG_MODE_STD   = 1,
   DIAG_MODE_EOL   = 2,
   DIAG_MODE_SYS   = 3,
   DIAG_MODE_CAR   = 4
};

u8 do_tester_msg(u8 *inbuf,u8 *outbuf,u8 msglen,u8 option);
u8 get_msg_len(u8 *buf,u8 len);
u8 get_sid_offset(u8 *buf,u8 len);
u8 calc_chksum(u8 *buf,u8 buflen);

void do_component_test(u8 sid,u8 para0,u8 para1,u8 option);

#endif /* define _PROT_IMP_ */
