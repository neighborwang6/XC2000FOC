//****************************************************************************
// @Module        Universal Serial Interface Channel (USIC) U0C0
// @Filename      U0C0.c
// @Project       MotoCtrl.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC2365B-40F80
//
// @Compiler      TASKING VX-toolset
//
// @Codegenerator 2.1
//
// @Description   This file contains functions that use the U0C0 module.
//
//----------------------------------------------------------------------------
// @Date          2020/12/24 14:40:43
//
//****************************************************************************

// USER CODE BEGIN (U0C0_General,1)

// USER CODE END



//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.h"

// USER CODE BEGIN (U0C0_General,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (U0C0_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (U0C0_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (U0C0_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (U0C0_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (U0C0_General,7)

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (U0C0_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (U0C0_General,9)

// USER CODE END


//****************************************************************************
// @Function      void U0C0_SSC_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the U0C0 function
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

void U0C0_SSC_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Channel Control Register (Mode & Interrupts):
  ///  -----------------------------------------------------------------------
  /// - The USIC channel is disabled
  /// - The parity generation is disabled

  U0C0_CCR       =  0x0000;      // load U0C0 channel control register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Fractional Divider:
  ///  -----------------------------------------------------------------------
  ///  - The Fractional divider is selected
  ///  - The step value STEP = 640

  U0C0_FDRL      =  0x8280;      // load U0C0 fractional divider register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Baudrate Generator:
  ///  -----------------------------------------------------------------------
  ///  - The selected BaudRate is 100.000 kbaud
  ///  - The PreDivider for CTQ, PCTQ = 0
  ///  - The Denominator for CTQ, DCTQ = 0
  ///  - The Divider factor PDIV = 249

  U0C0_BRGL      =  0x0000;      // load U0C0 load baud rate generator 
                                 // register L

  ///  - Shift Clock output configuration (SCLKCFG) = 2, Clock phase =1

  U0C0_BRGH      =  0x80F9;      // load U0C0 load baud rate generator 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Input Control Register:
  ///  -----------------------------------------------------------------------
  ///  - The data input DX0F is selected

  U0C0_DX0CR     =  0x0015;      // load U0C0 input control register(Data)

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Interrupt Node Pointer Register:
  ///  -----------------------------------------------------------------------
  ///  - RINP pointer points to Interrupt node U0C0_0IC

  U0C0_INPRL     =  0x0000;      // load U0C0 Interrupt Node Pointer register 
                                 // L
  U0C0_INPRH     =  0x0000;      // load U0C0 Interrupt Node Pointer register 
                                 // H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Shift Control:
  ///  -----------------------------------------------------------------------
  ///  - Transmit/Receive MSB first is selected

  U0C0_SCTRL     =  0x0103;      // load U0C0 shift control register L
  U0C0_SCTRH     =  0x0717;      // load U0C0 shift control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Transmit Control/Status Register:
  ///  -----------------------------------------------------------------------

  U0C0_TCSRL     =  0x0500;      // load U0C0 transmit control/status 
                                 // register L
  U0C0_TCSRH     =  0x0000;      // load U0C0 transmit control/status 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Protocol Control Register:
  ///  -----------------------------------------------------------------------

  U0C0_PCRL      =  0x0F47;      // load U0C0 protocol control register L
  U0C0_PCRH      =  0x0001;      // load U0C0 protocol control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Protocol Status Register:
  ///  -----------------------------------------------------------------------
  ///  - TBIF is set to simplify polling

  U0C0_PSR      |=  0x2000;      // load U0C0 protocol status register


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U0C0 Interrupts:
  ///  -----------------------------------------------------------------------
  ///  U0C00IC service request node configuration:
  ///  - U0C00IC interrupt priority level (ILVL) = 12
  ///  - U0C00IC interrupt group level (GLVL) = 1
  ///  - U0C00IC group priority extension (GPX) = 0

  U0C0_0IC       =  0x0071;     


  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Channel Control Register (Mode & Interrupts):
  ///  -----------------------------------------------------------------------
  ///  - SSC (SPI) Protocol is selected 
  ///  - The parity generation is disabled
  ///  - Receive interrupt is enabled

  U0C0_CCR       =  0x4001;      // load U0C0 channel control register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U0C0 Port Pins:
  ///  -----------------------------------------------------------------------
  P2_IOCR04      =  0x0020;      // load port register for data input

  ///  - P2.3 is used for USIC0 Channel0 Shift Data Output(DOUT)
  ///  - P2.5 is used for USIC0 Channel0 Shift Clock Output(SCLKOUT)
  ///  - P2.6 is used for USIC0 Channel0 Shift Control output0(SELO0)

  P2_IOCR03 = 0x0090;    //set direction register
  P2_IOCR05 = 0x0090;    //set direction register
  P2_IOCR06 = 0x0090;    //set direction register


  ///  -----------------------------------------------------------------------
  ///  Configuration of U0C0 FIFO:
  ///  -----------------------------------------------------------------------
  ///  -----------------------------------------------------------------------
  ///  Configuration of U0C0 Transmitter Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Transmit FIFO buffer contains 2 entries
  ///  - Transmit FIFO buffer starts at Data Pointer 0
  ///  - Limit for transmit FIFO interrupt generation is 0
  ///  - Limit for transmit FIFO interrupt generation is 0
  ///  - Limit for transmit FIFO interrupt generation is 0

  U0C0_TBCTRL    =  0x0000;      // load U0C0 transmitter buffer control 
                                 // register L
  U0C0_TBCTRH    =  0x0100;      // load U0C0 transmitter buffer control 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of U0C0 Receiver Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Receive FIFO buffer is disabled

  // USER CODE BEGIN (U0C0_Function,3)

  // USER CODE END



} //  End of function U0C0_SSC_vInit


//****************************************************************************
// @Function      void U0C0_SSC_vi0IC(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the Service 
//                Request Node U0C0_0IC.
//                Please note that you have to add application specific code 
//                to this function.
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

// USER CODE BEGIN (SSC0IC,1)

// USER CODE END

_interrupt(U0C0_0INT)  void U0C0_SSC_vi0IC(void)
{

  // USER CODE BEGIN (SSC0IC,2)

  // USER CODE END

  if (U0C0_PSR & 0x4000)          //Receive Interrupt flag
  {

  // USER CODE BEGIN (SSC0IC,4)

  // USER CODE END

  U0C0_PSCR   |= 0x4000;         //clear PSR_RIF
  }

  // USER CODE BEGIN (SSC0IC,11)

  // USER CODE END

} //  End of function U0C0_SSC_vi0IC




// USER CODE BEGIN (U0C0_General,10)

// USER CODE END

