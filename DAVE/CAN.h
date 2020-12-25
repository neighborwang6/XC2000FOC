//****************************************************************************
// @Module        MultiCAN Module (CAN)
// @Filename      CAN.h
// @Project       MotoCtrl.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC2365B-40F80
//
// @Compiler      TASKING VX-toolset
//
// @Codegenerator 2.1
//
// @Description   This file contains all function prototypes and macros for 
//                the CAN module.
//
//----------------------------------------------------------------------------
// @Date          2020/12/24 14:40:44
//
//****************************************************************************

// USER CODE BEGIN (CAN_Header,1)

// USER CODE END



#ifndef _CAN_H_
#define _CAN_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (CAN_Header,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

#define SetListCommand(ValueH, ValueL) CAN_PANCTRH = ValueH; CAN_PANCTRL = ValueL; while(CAN_PANCTRL & CAN_PANCTR_BUSY);

// USER CODE BEGIN (CAN_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

 // Panel Busy Flag
 #define CAN_PANCTR_BUSY       0x0100

 // Message Object Status Register
 #define MOSTAT_RXPND          0x0001
 #define MOSTAT_TXPND          0x0002
 #define MOSTAT_NEWDAT         0x0008
 #define MOSTAT_MSGLST         0x0010
 #define MOSTAT_RST_MNR        0x0019
 #define MOSTAT_RST_NT         0x000A



 // Structure for a single MultiCAN object
 // A total of 64 such object structures exists

 struct stCanObj
 {
   uword  uwMOFCRL;    // Function Control Register Low
   uword  uwMOFCRH;    // Function Control Register High
   uword  uwMOFGPRL;   // FIFO/Gateway Pointer Register Low
   uword  uwMOFGPRH;   // FIFO/Gateway Pointer Register High
   uword  uwMOIPRL;    // Interrupt Pointer Register Low
   uword  uwMOIPRH;    // Interrupt Pointer Register High
   uword  uwMOAMRL;    // Acceptance Mask Register Low
   uword  uwMOAMRH;    // Acceptance Mask Register High
   ubyte  ubData[8];   // Message Data 0..7
   uword  uwMOARL;     // Arbitration Register Low
   uword  uwMOARH;     // Arbitration Register High
   uword  uwMOCTRL;    // Control Register Low
   uword  uwMOCTRH;    // Control Register High
 };

 #define CAN_HWOBJ ((struct stCanObj volatile far*) 0x201000)

// USER CODE BEGIN (CAN_Header,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

 ///  -------------------------------------------------------------------------
 ///  @Definition of a structure for the CAN data
 ///  -------------------------------------------------------------------------

 // The following data type serves as a software message object. Each access to
 // a hardware message object has to be made by forward a pointer to a software
 // message object (MCAN_SWObj). The data type has the following fields:
 //
 // ubMOCfg:
 // this byte contains the 'Data Lenght Code(DLC)', 
 // 'Identifier Extension bit(IDE) - if 0 standard frame, else 1 extended frame', 
 // 'Message Direction(DIR) - if 0 receive Object, else 1 transmit Object'
 // 'Acceptance Mask bit(MIDE) - if 0 receives both, else 1 receives matching IDE'
 //
 //
 //         7     6     5     4     3     2     1     0
 //      |-----------------------------------------------|
 //      |     | MIDE |IDE | DIR |          DLC          |
 //      |-----------------------------------------------|
 //
 // ulID:
 // this field is four bytes long and contains either the 11-bit identifier
 // or the 29-bit identifier
 //
 // ulMask:
 // this field is four bytes long and contains either the 11-bit mask
 // or the 29-bit mask
 //
 // ubData[8]:
 // 8 bytes containing the data of a frame
 //
 // uwCounter:
 // this field is two bytes long and contains the counter value
 //

 typedef struct
 {
     ubyte ubMOCfg;    // message object configuration
     ulong ulID;       // standard (11-bit)/extended (29-bit) identifier
     ulong ulMask;     // standard (11-bit)/extended (29-bit) mask
     ubyte ubData[8];  // 8-bit data bytes
     uword uwCounter;  // frame counter(MOIPRnH[15-0])
 }stCAN_SWObj;


// USER CODE BEGIN (CAN_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (CAN_Header,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (CAN_Header,7)

// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

void CAN_vInit(void);



// USER CODE BEGIN (CAN_Header,8)

// USER CODE END


//****************************************************************************
// @Interrupt Vectors
//****************************************************************************


// USER CODE BEGIN (CAN_Header,9)

// USER CODE END


#endif  // ifndef _CAN_H_
