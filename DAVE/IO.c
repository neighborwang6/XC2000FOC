//****************************************************************************
// @Module        Parallel Ports
// @Filename      IO.c
// @Project       MotoCtrl.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC2365B-40F80
//
// @Compiler      TASKING VX-toolset
//
// @Codegenerator 2.1
//
// @Description   This file contains functions that use the IO module.
//
//----------------------------------------------------------------------------
// @Date          2020/12/24 14:40:42
//
//****************************************************************************

// USER CODE BEGIN (IO_General,1)

// USER CODE END



//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.h"

// USER CODE BEGIN (IO_General,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (IO_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (IO_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (IO_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (IO_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (IO_General,7)

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (IO_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (IO_General,9)

// USER CODE END


//****************************************************************************
// @Function      void IO_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the IO function 
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

// USER CODE BEGIN (IO_Function,1)

// USER CODE END

void IO_vInit(void)
{
  // USER CODE BEGIN (IO_Function,2)

  // USER CODE END


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P0:
  ///  -----------------------------------------------------------------------
  ///  P0.0 is used as alternate output for the CCU61 output (CC60)
  ///  - push/pull output is selected
  ///  P0.1 is used as alternate output for the CCU61 output (CC61)
  ///  - push/pull output is selected
  ///  P0.2 is used as alternate output for the  CCU61 output (CC62)
  ///  - push/pull output is selected
  ///  P0.3 is used as alternate output for the CCU61 output (COUT60)
  ///  - push/pull output is selected
  ///  P0.4 is used as alternate output for the CCU61 output (COUT61)
  ///  - push/pull output is selected
  ///  P0.5 is used as alternate output for the  CCU61 output (COUT62)
  ///  - push/pull output is selected

  ///  P0.0 - P0.3 output driver characteristic: strong driver
  ///  P0.4 - P0.7 output driver characteristic: strong driver

  ///  P0.0 - P0.3 output edge characteristic: sharp edge mode
  ///  P0.4 - P0.7 output edge characteristic: sharp edge mode


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P1:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P1 is used


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P2:
  ///  -----------------------------------------------------------------------
  ///  P2.0 is used as alternate input for the CAN0 Receive input(RXDC0C)
  ///  P2.1 is used as alternate output for the CAN0 Transmit output(TXDC0C)
  ///  - push/pull output is selected
  ///  P2.3 is used as alternate output for the USIC0 Channel0 Shift Data 
  ///  Output(DOUT)
  ///  - push/pull output is selected
  ///  P2.4 is used as alternate input for the USIC0 Channel0 Shift Data 
  ///  input(DX0F)
  ///  P2.5 is used as alternate output for the USIC0 Channel0 Shift Clock 
  ///  Output(SCLKOUT)
  ///  - push/pull output is selected
  ///  P2.6 is used as alternate output for the USIC0 Channel0 Shift Control 
  ///  output0(SELO0)
  ///  - push/pull output is selected

  ///  P2.0 - P2.3 output driver characteristic: strong driver
  ///  P2.4 - P2.7 output driver characteristic: strong driver
  ///  P2.8 - P2.13 output driver characteristic: strong driver

  ///  P2.0 - P2.3 output edge characteristic: sharp edge mode
  ///  P2.4 - P2.7 output edge characteristic: sharp edge mode
  ///  P2.8 - P2.13 output edge characteristic: sharp edge mode


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P4:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P4 is used


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P5:
  ///  -----------------------------------------------------------------------
  ///  P5.8 is used as alternate input for the  CCU61 Ext.Run Select(T12HRC)


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P6:
  ///  -----------------------------------------------------------------------
  ///  P6.0 is used as alternate output for the CAN2 Transmit output(TXDC2D)
  ///  - push/pull output is selected
  ///  P6.1 is used as alternate input for the CAN2 Receive input(RXDC2E)

  ///  P6.0 - P6.2 output driver characteristic: strong driver

  ///  P6.0 - P6.2 output edge characteristic: sharp edge mode


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P7:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P7 is used


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P10:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P10 is used


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P15:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P15 is used
  ///  - Port15 Data register P15(Read only)



  // USER CODE BEGIN (IO_Function,3)

  // USER CODE END

} //  End of function IO_vInit




// USER CODE BEGIN (IO_General,10)

// USER CODE END

