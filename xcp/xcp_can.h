#ifndef _XCP_CAN_H
#define _XCP_CAN_H

#include "std_defs.h"

#define kCanXcpMaxCTO 8
#define kCanXcpMaxDTO 8

void XcpCanInit(void);
void XcpCanBackground(void);
void XcpCanSend(u8 len, u8 *msg);

#endif /* define _XCP_CAN_H */
