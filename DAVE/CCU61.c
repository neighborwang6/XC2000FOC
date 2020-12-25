//****************************************************************************
// @Module        Capture / Compare Unit 61 (CCU61)
// @Filename      CCU61.c
// @Project       MotoCtrl.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC2365B-40F80
//
// @Compiler      TASKING VX-toolset
//
// @Codegenerator 2.1
//
// @Description   This file contains functions that use the CCU61 module.
//
//----------------------------------------------------------------------------
// @Date          2020/12/24 14:40:43
//
//****************************************************************************

// USER CODE BEGIN (CCU61_General,1)

// USER CODE END



//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.h"

// USER CODE BEGIN (CCU61_General,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (CCU61_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (CCU61_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (CCU61_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (CCU61_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (CCU61_General,7)

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (CCU61_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (CCU61_General,9)

// USER CODE END


//****************************************************************************
// @Function      void CCU61_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the CCU61 function 
//                library. It is assumed that the SFRs used by this library 
//                are in reset state. 
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    None
//
//----------------------------------------------------------------------------
// @Date          2020/12/24
//
//****************************************************************************

// USER CODE BEGIN (Init,1)

// USER CODE END

void CCU61_vInit(void)
{
  volatile unsigned int uwTemp;
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  Configuration of KERNEL REGISTERS :
  ///  -----------------------------------------------------------------------
  ///  - CCU61 Module is enabled.
  ///  - The CCU61 module clock = 80.000 MHz. 
  ///  - T12 is enabled.
  ///  - T13 is enabled.
  ///  - MCM is enabled.

  CCU61_KSCFG    =  0x0003;      // Kernel State Configuration Register

  uwTemp         =  CCU61_KSCFG;  // dummy read to avoid pipeline effects

  ///  -----------------------------------------------------------------------
  ///  Configuration of CCU61 Timer 12:
  ///  -----------------------------------------------------------------------
  ///  - Timer 12 Input clock factor (T12CLK) is 0
  ///  - prescaler factor is 0
  ///  - Timer 12 run bit is reset
  ///  - Single shot mode is disabled
  ///  - Timer 12 works in center aligned mode
  ///  - Interrupt on period match is disabled
  ///  - Interrupt on one match is disabled
  ///  - No External run selection is selected.
  ///  - Timer mode is selected.
  ///  - 

  CCU61_T12PR    =  0x0001;      // load CCU61 T12 period register

  ///  -----------------------------------------------------------------------
  ///  Configuration of T13HR Signal:
  ///  -----------------------------------------------------------------------
  ///  - Signal T13HRD is selected as Input
  CCU61_PISELH   =  0x0003;      // Load CCU61 Port Input Selection register


  ///  -----------------------------------------------------------------------
  ///  Configuration of CCU61 Timer 13:
  ///  -----------------------------------------------------------------------
  ///  - Timer 13 Input Clock factor (T13CLK) is 0
  ///  - prescaler factor is 0
  ///  - Timer 13 run bit is reset
  ///  - Trigger control is disabled
  ///  - No External run selection is selected.
  ///  - Timer mode is selected.
  ///  - 
  ///  - Single shot mode is disabled
  ///  - Interrupt on period match is disabled
  ///  - Interrupt on compare match is disabled

  CCU61_T13PR    =  0x0001;      // load CCU61 T13 period register

  CCU61_TCTR0    =  0x0080;      // load CCU61 timer control register 0
  CCU61_TCTR2    =  0x0020;      // load CCU61 timer control register 2

  ///  -----------------------------------------------------------------------
  ///  Configuration of Multi Channel Mode:
  ///  -----------------------------------------------------------------------
  ///  - Multi channel mode is disabled


  ///  -----------------------------------------------------------------------
  ///  Configuration of CCU61 Channel 0:
  ///  -----------------------------------------------------------------------
  ///  - Compare mode 1 is selected
  ///  - T12 modulation for output CC60 is enabled
  ///  - T13 modulation for output CC60 is disabled
  ///  - The trap functionality of the pin CC60 is disabled
  ///  - The compare output CC60 drives passive level while CC60ST is '1'
  ///  - The passive level of the output CC60 is '0'
  ///  - T12 modulation for output COUT60 is enabled
  ///  - T13 modulation for output COUT60 is disabled
  ///  - The trap functionality of the pin COUT60 is disabled
  ///  - The compare output COUT60 drives passive level while CC60ST is '0'
  ///  - The passive level of the output COUT60 is '0'
  ///  - Dead time generation is disabled

  ///  - Generation interrupt on flag ICC60R is disabled
  ///  - Generation interrupt on flag ICC60F is disabled

  CCU61_CC60SR   =  0x0000;      // Load CCU61 capture/compare shadow 
                                 // register for channel 0

  ///  -----------------------------------------------------------------------
  ///  Configuration of CCU61 Channel 1:
  ///  -----------------------------------------------------------------------
  ///  - Compare mode 1 is selected
  ///  - T12 modulation for output CC61 is enabled
  ///  - T13 modulation for output CC61 is disabled
  ///  - The trap functionality of the pin CC61 is disabled
  ///  - The compare output CC61 drives passive level while CC61ST is '0'
  ///  - The passive level of the output CC61 is '1'
  ///  - T12 modulation for output COUT61 is disabled
  ///  - T13 modulation for output COUT61 is disabled
  ///  - The trap functionality of the pin COUT61 is disabled
  ///  - The compare output COUT61 drives passive level while CC61ST is '0'
  ///  - The passive level of the output COUT61 is '0'
  ///  - Dead time generation is disabled

  ///  - Generation interrupt on flag ICC61R is disabled
  ///  - Generation interrupt on flag ICC61F is disabled

  CCU61_CC61SR   =  0x0000;      // Load CCU61 capture/compare shadow 
                                 // register for channel 1

  ///  -----------------------------------------------------------------------
  ///  Configuration of CCU61 Channel 2:
  ///  -----------------------------------------------------------------------
  ///  - Compare mode 1 is selected
  ///  - T12 modulation for output CC62 is disabled
  ///  - T13 modulation for output CC62 is disabled
  ///  - The trap functionality of the pin CC62 is disabled
  ///  - The compare output CC62 drives passive level while CC62ST is '0'
  ///  - The passive level of the output CC62 is '0'
  ///  - T12 modulation for output COUT62 is disabled
  ///  - T13 modulation for output COUT62 is disabled
  ///  - The trap functionality of the pin COUT62 is disabled
  ///  - The compare output COUT62 drives passive level while CC62ST is '0'
  ///  - The passive level of the output COUT62 is '0'
  ///  - Dead time generation is disabled

  ///  - Generation interrupt on flag ICC62R is disabled
  ///  - Generation interrupt on flag ICC62F is disabled

  CCU61_CC62SR   =  0x0000;      // Load CCU61 capture/compare shadow 
                                 // register for channel 2

  ///  -----------------------------------------------------------------------
  ///  Configuration of CCU61 Channel 3:
  ///  -----------------------------------------------------------------------
  ///  - T13 output is not inverted


  CCU61_CC63SR   =  0x0000;      // load CCU61 capture/compare shadow 
                                 // register for channel 3


  CCU61_T12DTC   =  0x0001;      // load CCU61 dead time control register for 
                                 // timer T12

  CCU61_T12MSEL  =  0x0111;      // load CCU61 T12 campture/compare mode 
                                 // select register

  CCU61_CMPSTAT  =  0x0100;      // load CCU61 compare status register

  CCU61_PSLR     =  0x0004;      // load CCU61 passive state level register

  CCU61_MODCTR   =  0x0007;      // load CCU61 modulation control register

  ///  -----------------------------------------------------------------------
  ///  Configuration of CCU61 trap control:
  ///  -----------------------------------------------------------------------
  ///  - Trap can only be generated by SW by setting the bit TRPF
  ///  - The trap state is left when a zero-match of T12 (while counting up) 
  ///    is detected (synchronization to T12)
  ///  - Bit TRPF is automatically cleared by HW (according to TRPPEN, TRPM0 
  ///    and TRPM1)
  ///  - Trap interrupt is disabled


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CCU61 Channels Interrupts:
  ///  -----------------------------------------------------------------------

  ///  -----------------------------------------------------------------------
  ///  Configuration of T12, T13 ---- CCU61_TCTR4 Register:
  ///  -----------------------------------------------------------------------
  ///  - Enable shadow transfer of T12 and T13
  ///  - Timer 12 run bit is reset
  ///  - Timer 13 run bit is reset
  CCU61_TCTR4    =  0x4040;      // load CCU61 timer control register 4

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CCU61 Channel Port Pins:
  ///  -----------------------------------------------------------------------
  ///  - P0.0 is used for CCU61 output (CC60)
  ///  - P0.1 is used for CCU61 output (CC61)
  ///  - P0.2 is used for  CCU61 output (CC62)
  ///  - P0.3 is used for CCU61 output (COUT60)
  ///  - P0.4 is used for CCU61 output (COUT61)
  ///  - P0.5 is used for  CCU61 output (COUT62)
  ///  - P5.8 is used for  CCU61 Ext.Run Select(T12HRC)

  P0_IOCR00 = 0x00B0;    //set direction register
  P0_IOCR01 = 0x00B0;    //set direction register
  P0_IOCR02 = 0x00B0;    //set direction register
  P0_IOCR03 = 0x00B0;    //set direction register
  P0_IOCR04 = 0x00B0;    //set direction register
  P0_IOCR05 = 0x00B0;    //set direction register

  CCU61_PISELL   =  0xBF00;      // Load CCU61 Port Input Selection register


  // USER CODE BEGIN (NodeI3,3)

  // USER CODE END

} //  End of function CCU61_vInit




// USER CODE BEGIN (CCU61_General,10)

// USER CODE END

