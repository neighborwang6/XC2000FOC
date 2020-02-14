/*----------------------------------------------------------------------------
| Project: XCP_Delivery
|
|  Description:   Implementation of the XCP Protocol Layer
|                 XCP V1.0 slave device driver
|                 Basic Version
|
|-----------------------------------------------------------------------------
| Copyright (c) by Vector Informatik GmbH.  All rights reserved.
----------------------------------------------------------------------------*/

#if !defined(__XCP_CFG_H__)
#define __XCP_CFG_H__

#define XCP_TRANSPORT_LAYER_TYPE_CAN
#define XCP_ENABLE_SEED_KEY
#define XCP_ENABLE_CALIBRATION_PAGE

/* define here configuration parameters for customizing XcpBasic driver */
//#define XCP_ENABLE_VX1000
//#define XCP_ENABLE_VX1000_TRICORE


/*----------------------------------------------------------------------------*/
/* Project specific includes */

#include "board.h"

/*----------------------------------------------------------------------------*/
/* Test */

/* Use XCP_PRINT to generate diagnostic messages */
//#define XCP_ENABLE_TESTMODE
#ifdef XCP_ENABLE_TESTMODE
  #define ApplXcpPrint XcpPrintf
  #define XCP_ASSERT(x) if (!(x)) XcpPrintf("Assertion failed in line %u",__LINE__);
#else
  #define XCP_ASSERT(x)
#endif


/*----------------------------------------------------------------------------*/
/* Data types */

  /* 8-Bit qualifier */
typedef unsigned char  vuint8;
typedef signed char    vsint8;   

/* 16-Bit qualifier */
typedef unsigned short vuint16;
typedef signed short   vsint16;

/* 32-Bit qualifier */
typedef unsigned long  vuint32; 
typedef signed long    vsint32;


/*----------------------------------------------------------------------------*/

/* XCP slave device identification (optional) */
#define kXcpStationIdLength 7       /* Slave device identification length */
#define kXcpStationIdString "XCPDemo"

#if defined ( kXcpStationIdLength )
  extern vuint8 kXcpStationId[];
#endif


/*----------------------------------------------------------------------------*/
/* XCP protocol parameters */

/* Copy routine */
#define xcpMemCpy memcpy

/* Byte order */
#define C_CPUTYPE_LITTLEENDIAN /* Intel */

/* XCP message length */
#define kXcpMaxCTO     8      /* Maximum CTO Message Lenght */
#define kXcpMaxDTO     8      /* Maximum DTO Message Lenght */

#define XCP_DISABLE_PARAMETER_CHECK

/* Enable COMM_MODE_INFO */
#define XCP_ENABLE_COMM_MODE_INFO

/* Enable GET_SEED and UNLOCK */
//#define XCP_ENABLE_SEED_KEY

/* Enable xcpUserService() to handle user defined commands */
//#define XCP_ENABLE_USER_COMMAND

/* Enable transmission of event messages */
#define XCP_ENABLE_SEND_EVENT

/* Enable xcpPrintf() and xcpPutChar() via SERV_TEXT events */
#if defined(XCP_CC_AUTOSAR) || defined(XCP_CC_TC_OVRAM)
  #include <stdio.h>
  #include <stdarg.h>
  #define XCP_ENABLE_SERV_TEXT
  #define XCP_ENABLE_SERV_TEXT_PUTCHAR
  #define XCP_ENABLE_SERV_TEXT_PRINT
  #define XCP_ENABLE_SERV_TEXT_PRINTF
#else

#endif


/* Disable FLASH programming */
/* Implement the flash programming feature in the ECU */
/* Not available in xcpBasic ! */
#define XCP_DISABLE_PROGRAM

/* Activate the flash programming kernel download support */
#define XCP_ENABLE_BOOTLOADER_DOWNLOAD


/*----------------------------------------------------------------------------*/
/* Additional user call backs */

#if defined(XCP_CC_AUTOSAR) || defined(XCP_CC_TC_OVRAM)
  #define ApplXcpInit ApplXcpInit
  extern void ApplXcpInit( void );
#endif



/*----------------------------------------------------------------------------*/
/* XCP Calibration Parameters */

#define XCP_ENABLE_CALIBRATION

#define XCP_ENABLE_SHORT_DOWNLOAD
#define XCP_ENABLE_SHORT_UPLOAD

/* Enable block transfer */ 
#define XCP_ENABLE_BLOCK_UPLOAD
#define XCP_ENABLE_BLOCK_DOWNLOAD

/* Enable memory checksum */
/* The checksum will be calculated in XcpBackground() */
/* This may be implementation specific */
#define XCP_ENABLE_CHECKSUM
#define kXcpChecksumMethod XCP_CHECKSUM_TYPE_ADD14
                   
/* Enable Calibration Page switching and dynamic calibration overlay RAM allocation */
#if defined(XCP_CC_AUTOSAR) || defined(XCP_CC_TC_OVRAM)
  #define XCP_ENABLE_CALIBRATION_MEM_ACCESS_BY_APPL
  #define XCP_ENABLE_CALIBRATION_PAGE
  #define XCP_ENABLE_PAGE_COPY
#endif
#define XCP_DISABLE_SEGMENT_INFO
#define XCP_DISABLE_PAGE_INFO 
#define XCP_DISABLE_PAGE_FREEZE


/*----------------------------------------------------------------------------*/
/* XCP Data Stimulation Parameters */

/* Synchronous Data Stimulation (STIM) */ 
//#define XCP_ENABLE_STIM
//#define XCP_DISABLE_STIM
//#define kXcpStimOdtCount 16

/*----------------------------------------------------------------------------*/
/* XCP Data Acquisition Parameters */
                                  
/* Enable data acquisition */
#define XCP_ENABLE_DAQ                   
//#define XCP_ENABLE_DAQ_HDR_ODT_DAQ

#define XCP_ENABLE_DAQ_PRESCALER
#define XCP_ENABLE_DAQ_OVERRUN_INDICATION
//#define XCP_ENABLE_DAQ_RESUME
#define XCP_ENABLE_DAQ_PROCESSOR_INFO
#define XCP_ENABLE_DAQ_RESOLUTION_INFO

#define XCP_ENABLE_WRITE_DAQ_MULTIPLE

/* Memory space reserved for DAQ */
/* This memory is also shared with the VX, so the size was set to fullfill VX max requirement */
#define kXcpDaqMemSize (1024*2)
#define XCP_ENABLE_SEND_QUEUE
#define XCP_DISABLE_SEND_BUFFER

/* DAQ Timestamp */ 
#define XCP_ENABLE_DAQ_TIMESTAMP
#ifdef XCP_ENABLE_DAQ_TIMESTAMP 
  #define kXcpDaqTimestampSize 2 
  #define kXcpDaqTimestampUnit DAQ_TIMESTAMP_UNIT_1NS
  #define kXcpDaqTimestampTicksPerUnit 15360  // 133MHz/4 -> 30ns * 512 = 15360 (Triboard TC1767) Error 0%
#endif


/* Enable Plug&Play for the event definitions */
//#define XCP_ENABLE_DAQ_EVENT_INFO

#endif /* __XCP_CFG_H__ */
