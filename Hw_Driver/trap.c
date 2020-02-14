//////////////////////////////////////////////////////////
#include "xc2365a.h"

/*********************************************************
 * Non-Maskable Interrupt
*********************************************************/

static void NMI_trap (void) interrupt 0x02  
{
	while (1);
}


/*********************************************************
 * Stack Overflow Interrupt
**********************************************************/
static void STKOF_trap (void) interrupt 0x04  
{
	while (1);
}


/*********************************************************
 * Stack Underflow Interrupt
*********************************************************/
static void STKUF_trap (void) interrupt 0x06  
{
	while (1);
}


/*********************************************************
 * Class B Hardware Trap:
 *   ->  Undefined Opcode
 *   ->  Protected Instruction Fault
 *   ->  Illegal Word Operand Access
 *   ->  Illegal Instruction Access
 *   ->  Illegal External Bus Access
*********************************************************/
static void Class_B_trap (void) interrupt 0x0A  
{
	if(TFR_UNDOPC)    // Undefined Opcode
	{
		TFR_UNDOPC = 0;

		while (1);
	}

	if(TFR_ACER)     // Programm Memory Interface Access Error
	{
		TFR_ACER = 0;
		while (1);
	}

	if(TFR_PRTFLT)
	{
		TFR_PRTFLT = 0;
		while (1);
	}

	if(TFR_ILLOPA)
	{
		TFR_ILLOPA = 0;
		while (1);
	}
}
