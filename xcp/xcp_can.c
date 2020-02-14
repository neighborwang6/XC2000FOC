#include "xcp_can.h"
#include "hican.h"
#include "xcpbasic.h"

void XcpCanInit(void)
{
}

void XcpCanBackground(void)
{
}

void XcpCanSend(u8 len, u8 *msg)
{
	if(len>8)
		return;
	
    can_sload(2,msg);
    can_stx(2);	
	
	XcpSendCallBack();
}

void ApplXcpInterruptDisable(void)
{
}

void ApplXcpInterruptEnable(void)
{
}
