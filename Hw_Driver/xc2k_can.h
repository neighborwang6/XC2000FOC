#ifndef _XC2K_CAN_H
#define _XC2K_CAN_H

#ifdef WIN32
#define fmemcpy memcpy
#endif

#include "std_defs.h"
#include "hw.h"

void can_setting(void);
void can_tx(u8 no,u32 id,u8 cfg,u8 *buf);
void can_rx(u8 no,u32 id,u32 mask,u8 cfg);
void can_load(u8 no,u32 id,u8 cfg,u8 *buf);
void can_stx(u8 no);
void can_sload(u8 no,u8 *buf);
void can_parse_int(void);
void do_objx(u8 no,u32 id,u8 far * buf,u8 len);
void can_abort(u8 no);
u8 can_busy(u8 no);
u8 can_stx_ex(u8 no);
void set_can_on(void);
u8 get_can_bus_off(void);

#define SetListCommand(ValueH, ValueL) CAN_PANCTRH = ValueH; CAN_PANCTRL = ValueL; while(CAN_PANCTRL & CAN_PANCTR_BUSY);

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
   u16  MOFCRL;    // Function Control Register Low
   u16  MOFCRH;    // Function Control Register High
   u16  MOFGPRL;   // FIFO/Gateway Pointer Register Low
   u16  MOFGPRH;   // FIFO/Gateway Pointer Register High
   u16  MOIPRL;    // Interrupt Pointer Register Low
   u16  MOIPRH;    // Interrupt Pointer Register High
   u16  MOAMRL;    // Acceptance Mask Register Low
   u16  MOAMRH;    // Acceptance Mask Register High
   u8   DATA[8];   // Message Data 0..7
   u16  MOARL;     // Arbitration Register Low
   u16  MOARH;     // Arbitration Register High
   u16  MOCTRL;    // Control Register Low
   u16  MOCTRH;    // Control Register High
 };

#ifdef WIN32
 extern u16  cansfr_base[];
 #define CAN_HWOBJ ((struct stCanObj volatile far*) (cansfr_base+0x1000))
#else
 #define CAN_HWOBJ ((struct stCanObj volatile far*) 0x201000)
#endif

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
     u8  mocfg;      // message object configuration
     u32 id;         // standard (11-bit)/extended (29-bit) identifier
     u32 mask;       // standard (11-bit)/extended (29-bit) mask
     u8  data[8];    // 8-bit data bytes
     u16 counter;    // frame counter(MOIPRnH[15-0])
 }stCAN_SWObj;

void CAN_vInit(void);
void CAN_vGetMsgObj(u8 ubObjNr, stCAN_SWObj *pstObj);
void CAN_vTransmit(u8 ubObjNr);
void CAN_vConfigMsgObj(u8 ubObjNr, stCAN_SWObj *pstObj);
void CAN_vLoadData(u8 ubObjNr, u8 *ubpubData);

#define CAN_ubRequestMsgObj(ObjNr) (((CAN_HWOBJ[ObjNr].uwMOCTRL & 0x0100) == 0x0000) \
   ? CAN_HWOBJ[ObjNr].uwMOCTRL = 0x0020, 1 : 0)
#define CAN_ubNewData(ObjNr) (((CAN_HWOBJ[ObjNr].uwMOCTRL & 0x0008) == 0x0008) \
   ? 1 : 0)
#define CAN_ubDelMsgObj(ObjNr) (((CAN_HWOBJ[ObjNr].uwMOCTRL & 0x0108) == 0) \
   ?  CAN_HWOBJ[ObjNr].uwMOCTRL = 0x0020,1 : 0)
#define CAN_vReleaseObj(ObjNr) CAN_HWOBJ[ObjNr].uwMOCTRL = 0x0008;
#define CAN_vSetMSGVAL(ObjNr) CAN_HWOBJ[ObjNr].uwMOCTRH |= 0x0020;

#endif /* define _XC2K_CAN_H */
