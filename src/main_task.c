////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
* Reserved timer
*  
*      
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

#include "main_task.h"

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void main_task_init(void)
{
	GPT12E_T2IC=0x40|(1<<2)|(0);  // Lowest priority except CPU idle
	GPT12E_T2CON=0x0002;          // 1.6us/tick@20MHz,0.8us@40Mhz,0.4us@80MHz
	
	main_task_stop();	
	main_task_setperiod(10);       // 10ms
	main_task_start();
}

////////////////////////////////////////////////////////////////////////////////
// Timer2 period: 10ms
////////////////////////////////////////////////////////////////////////////////
void isr_main_timer(void) interrupt T2INT
{
	main_task_stop();
	main_task_setperiod(10);       // 10ms
	main_task_start();
	
	// Function removed
	// do_ctrl();
	// g_overflow_10ms_ub=1;
}
////////////////////////////////////////////////////////////////////////////////
