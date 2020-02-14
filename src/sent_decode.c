/**************************************************
*    File Name    : sent_decode.c
*    Copyright    : QingChen
*    Module Name  : Electric Power Steering
     *
*    CPU          : XC2000
*    RTOS         : No RTOS
*
*    Create Date  : 2019.11.26
*    Author/Cororation: WGY/QingChen Techology
*    Abstract Description: Get Sent Data From disp Sensor of BOSCH iBooster.
*
*--------------- Revision History --------------
*    No Version Date      Revisied By  Item    Description
*    1  V0.0    19.11.14  WGY                  
**************************************************/


/**************************************************
*    Debug switch Section
**************************************************/

/**************************************************
*    Include File Section
**************************************************/
#include "std_defs.h"
#include "sent_decode.h"
/**************************************************
*    MacroDefine Section
**************************************************/
#define TIMER_ATOM_US (6.67e-3) // Timer Resolution in microsecond
//#define TIMER_TICKS_PER_US ((u16)(1.0/(TIMER_ATOM_US)))
#define TIMER_TICKS_PER_US (40)

#define F_US(x) ((x)*(TIMER_TICKS_PER_US)*1UL)
#define IDLE_TM F_US(500UL)
#define F_TICKS(x) (x)
#define F_TICKS_TM(x) ((x)*g_tick_period)
#define SYN_TICKS F_TICKS(56)
#define SYN_TICKS_TM F_TICKS_TM(56)

#define TM_EQUAL(t1,t2,tolerance) ((t1)<(t2)+(tolerance) && (t1)+(tolerance)>(t2))
#define TM_GREAT_THAN(t1,t2) (t1>t2)
#define TM_LESS_THAN(t1,t2) (t1<t2)
/**************************************************
*    StructDefine Section
**************************************************/

/**************************************************
*    Prototype Declare Section
**************************************************/
enum {
    FPHASE_NONE      = 0,
    FPHASE_SYN_OK    = 1,
    FPHASE_STATUS_OK = 2,
    FPHASE_DATA_OK   = 3
};

/**************************************************
*    Global Variable Declare Section
**************************************************/
u8 sent_data[7];
u8 sent_data_idx=0;
u32 timer_val0=0; // Timer valve at previous falling edge
u32 timer_val=0;  // Timer value at current falling edge
u8 frame_phase=FPHASE_NONE;  //Default mode
u32 g_tick_period=F_US(3); // XXus/bit

/**************************************************
*    File Static Variable Define Section
**************************************************/

/**************************************************
*    Function Define Section
**************************************************/

u8 sent_decode(u32 timer, u8 option)
{
    u8 ret_code=1;
    u8 tmpu8;
    u32 pulsex;

    if(option==1)
    {
        frame_phase=FPHASE_NONE;
        sent_data_idx=0;

        return 1;
    }

    pulsex=timer;

    if(pulsex>IDLE_TM)
    {
        frame_phase=FPHASE_NONE;
        sent_data_idx=0;

        return 1;
    }

    switch(frame_phase)
    {
    case FPHASE_NONE:
        if(TM_EQUAL(pulsex,SYN_TICKS_TM,SYN_TICKS_TM/5)) // SYN nibble detected
        {
            g_tick_period=pulsex/SYN_TICKS;
            frame_phase=FPHASE_SYN_OK;
            sent_data_idx=0;
        }
        break;
    case FPHASE_SYN_OK:
        if(TM_GREAT_THAN(pulsex,F_TICKS_TM(27.9)) || TM_LESS_THAN(pulsex,F_TICKS_TM(11.5))) // Invalid STATUS nibble detected
        {
            frame_phase=FPHASE_NONE;
            sent_data_idx=0;
            break;
        }
        pulsex+=(g_tick_period/4);  // Round to: a.75 => a+1
        tmpu8=(u8)(pulsex/g_tick_period);
        if(tmpu8>27)
            tmpu8=27;
        if(tmpu8<12)
            tmpu8=12;

        sent_data_idx=0;
        sent_data[sent_data_idx]=tmpu8-12;  //Status & Communication
        sent_data_idx++;
        frame_phase=FPHASE_STATUS_OK;
        break;

    case FPHASE_STATUS_OK:
        if(TM_GREAT_THAN(pulsex,F_TICKS_TM(27.9)) || TM_LESS_THAN(pulsex,F_TICKS_TM(11.5))) // Invalid Data & CRC nibble detected
        {
            frame_phase=FPHASE_NONE;
            sent_data_idx=0;
            break;
        }

        // Timing trouble
        // ...
        pulsex+=(g_tick_period/4); // Round to: a.75 => a+1
        tmpu8=(u8)(pulsex/g_tick_period);
        if(tmpu8>27)
            tmpu8=27;
        if(tmpu8<12)
            tmpu8=12;
        sent_data[sent_data_idx]=tmpu8-12;

        sent_data_idx++;
        if(sent_data_idx>=7)
        {
            frame_phase=FPHASE_DATA_OK;
            ret_code=0;
            break;
        }

        break;

    case FPHASE_DATA_OK:
        sent_data_idx=0;
        frame_phase=FPHASE_NONE;
        ret_code=2;
        break;

    default:
        sent_data_idx=0;
        frame_phase=FPHASE_NONE;
        break;
    }

    return ret_code;
}
