////////////////////////////////////////////////////////////////////////////////
/**************************************************
*    File Name    : ctrl.c
*    Copyright    : QingChen
*    Module Name  : Electric Power Steering
     *
*    CPU          : XC2000
*    RTOS         : No RTOS
*
*    Create Date  : 2019.11.14
*    Author/Cororation: Perfersor YLY
*    Abstract Description:
*
*--------------- Revision History --------------
*    No Version Date      Revisied By  Item    Description
*    1  V0.0    19.11.26  WGY                  eBooster
**************************************************/

/**************************************************
*    Debug switch Section
**************************************************/
//#define PRESURE_PID_CTRL
/**************************************************
*    Include File Section
**************************************************/
#include "aut_drv.h"

/**************************************************
*    MacroDefine Section
**************************************************/

/**************************************************
*    StructDefine Section
**************************************************/

/**************************************************
*    Prototype Declare Section
**************************************************/

/**************************************************
*    Global Variable Declare Section
**************************************************/

/**************************************************
*    File Static Variable Define Section
**************************************************/

/**************************************************
*    Function Define Section
**************************************************/

// WGY Debug Data
u8 AUT_DRV_BUFF[8];

///////////////////////////////////////////////////////////////////////////////
//Auto Drive Command from CAN bus
void do_aut_drv_response(u8 *buf)
{
	u16 *pt;

	// Compatible with Motorola Order of BYTEs.
	AUT_DRV_BUFF[0]=buf[0];
	AUT_DRV_BUFF[1]=buf[1];
	AUT_DRV_BUFF[2]=buf[2];
	AUT_DRV_BUFF[3]=buf[3];
	AUT_DRV_BUFF[4]=buf[4];
	AUT_DRV_BUFF[5]=buf[5];
	AUT_DRV_BUFF[6]=buf[6];
	AUT_DRV_BUFF[7]=buf[7];


	
	pt = (u16*)AUT_DRV_BUFF;
	
	if(pt[2]!=0x00)
		return;
	
	///////////////////////////////////////////////////////
}
