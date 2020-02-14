////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
*
*  
*      
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

#include "xc2k_can.h"
#include "int_vec.h"

#ifdef WIN32
#include "SMaster_Sim_funcs.h"
#endif

////////////////////////////////////////////////////////////////////////////////

/*****************************************************************
fcan=Fsys/(1024-CAN0_STEP):

CAN_STEP   fcan
1022         40 MHz
1023         80 MHz (selected in this file)

////////////////////////////////////////////////////////////////////////////////
tq = (BRP+1) / fcan
TSync = 1 tq
TSeg1 = (TSEG1 + 1) tq (min. 3 tq)
TSeg2 = (TSEG2 + 1) tq (min. 2 tq)
bit time = TSync + TSeg1 + TSeg2 (min. 8 tq)

fcan=40MHz
baudrate TSEG1   TSEG2  BRP
1MHz      6        1    7
500KHz    6        1    15
250KHz    6        1    31

*****************************************************************/
// fcan=40Mhz @ Fsys=80MHz
#ifndef FSYS_MHZ
   #error "FSYS_MHZ not defined"
#endif

#define CAN_STEP 1023

// 1MHz: CAN Node 0
#define CAN0_TSEG1   6
#define CAN0_TSEG2   1
#define CAN0_SJW     1
#define CAN0_BRP	 7
// 500KHz: CAN Node 1
#define CAN1_TSEG1   6
#define CAN1_TSEG2   1
#define CAN1_SJW     1
#define CAN1_BRP	 15

// 250KHz: CAN Node 1
// #define CAN1_TSEG1   6
// #define CAN1_TSEG2   1
// #define CAN1_SJW     1
// #define CAN1_BRP	 31

u8 g_can_status=0; // bit0: bus off(1); bit1: tx ok, bit2: rx ok


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void can_setting(void)
{
	int idx;
	volatile u16 tmpu16;
	
	MCAN_KSCCFG  =  0x0003;	
	tmpu16=MCAN_KSCCFG;
    _nop_(); _nop_();

    CAN_FDRL     =  0x4000|(CAN_STEP);    // Set fcan
	tmpu16=CAN_FDRL;

    while(CAN_PANCTRL & CAN_PANCTR_BUSY); // Wait util Panel Ready
	
	// Configuration of CAN Node 0
    CAN_NCR0     =  0x004F;               // Configuration Mode
    CAN_NIPR0    =  0x0021;               // Select Interrupt Node Pointer

	  P2_IOCR01    =  0x0090;
	  P2_IOCR00    =  0x0000;               // P2.0-RXDC0C P2.1-TXDC0 O1
    CAN_NPCR0    =  0x0002;
	
    CAN_NBTR0L   =  (CAN0_TSEG2<<12)|(CAN0_TSEG1<<8)|(CAN0_SJW<<6)|(CAN0_BRP); // Baudrate Setting
	
    CAN_NECNT0H  = 0x0060;   // Configuration of the Error Counter
    CAN_NECNT0L  = 0x0000;       
    CAN_NFCR0H   = 0x0000;   // Configuration of the Frame Counter
    CAN_NFCR0L   = 0x0000;

	// Configuration of the CAN Message Object List Structure
	// Allocate 32-MOs for list 1 (CAN0)
	
	for(idx=0;idx<32;idx++)
	{
		SetListCommand((0x0100|idx),0x0002);
	}
	// Allocate 16-MOs for list 2 (CAN1)
	
	CAN_0IC        =  0x007C;     
	CAN_1IC        =  0x007A;     
	CAN_2IC        =  0x0076;     

	CAN_NCR0 &= ~((u16)0x0041); // reset INIT and CCE
}

////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
cfg:  Bit  7  6  5  4  3  2  1   0
           |        |     |  |___|___ CAN Node (0,1,2,3)
           |        |     |
           |        |     |__________ Ext ID Flag
           |________|________________ DLC

no: 0-15 supported           
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
void can_tx(u8 no,u32 id,u8 cfg,u8 *buf)
{
	u16 tmpu16;
	
	tmpu16=(cfg&0x03)+1;
	tmpu16<<=8;           // Adjust
	
	// Set CAN Node
	while(CAN_PANCTRL & CAN_PANCTR_BUSY);
	SetListCommand((tmpu16|no),0x0002);
	
	CAN_HWOBJ[no].MOCTRH=  0x0e28;  // NEWDAT,MSGVAL,TXEN0,TXEN1,DIR
	CAN_HWOBJ[no].MOCTRL=  0x0000;
	
	// Set ID
	tmpu16=0x4000;           // Priority Class: List order
	if(cfg&0x04)             // Extended ID
	{
		tmpu16|=0x2000;
		tmpu16|=(u16)(id>>16);
		CAN_HWOBJ[no].MOARH=tmpu16;
		CAN_HWOBJ[no].MOARL=(u16)id;
	}
	else
	{
		tmpu16|=(id<<2);
		CAN_HWOBJ[no].MOARH=tmpu16;
		CAN_HWOBJ[no].MOARL=0x0000;
	}
	
	// Set Init Data
	fmemcpy((u8 *)CAN_HWOBJ[no].DATA,buf,8);
	
	// Mask
    CAN_HWOBJ[no].MOAMRH = 0x3fff;
    CAN_HWOBJ[no].MOAMRL = 0xffff;
	
	// Pending Bit
    CAN_HWOBJ[no].MOIPRH = 0x0000;
	if(no<16)
		CAN_HWOBJ[no].MOIPRL = (((u16)no)<<8);
	else if(no<32)
		CAN_HWOBJ[no].MOIPRL = (((u16)no)<<8)|0x11;
	
	tmpu16=cfg&0xf0;
	tmpu16<<=4;           // Set DLC
	// tmpu16|=0x0080;       // Single Transmit Trial
	
    CAN_HWOBJ[no].MOFCRH = tmpu16;
    CAN_HWOBJ[no].MOFCRL = 0x0000;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void can_rx(u8 no,u32 id,u32 mask,u8 cfg)
{
	u16 tmpu16;
	
	tmpu16=(cfg&0x03)+1;
	tmpu16<<=8;           // Adjust
	
	// Set CAN Node
	//while(CAN_PANCTRL & CAN_PANCTR_BUSY);
	//SetListCommand((tmpu16|no),0x0002);
	
	CAN_HWOBJ[no].MOCTRH=  0x00A0;  // NEWDAT,RXEN
	CAN_HWOBJ[no].MOCTRL=  0x0000;
	

	
	// Set ID
	tmpu16=0x4000;           // Priority Class: List order
	if(cfg&0x04)             // Extended ID
	{
		tmpu16|=0x2000;
		tmpu16|=(u16)(id>>16);
		CAN_HWOBJ[no].MOARH=tmpu16;
		CAN_HWOBJ[no].MOARL=(u16)id;
		
	}
	else
	{
		tmpu16|=(id<<2);
		CAN_HWOBJ[no].MOARH=tmpu16;
		CAN_HWOBJ[no].MOARL=0x0000;
		
	}
	
	// Mask
	if(cfg&0x04)             // Extended ID
	{
		CAN_HWOBJ[no].MOAMRH = ((u16)(mask>>16)) & 0x3FFF;  // load MO[no] acceptance mask register high
		CAN_HWOBJ[no].MOAMRL = (u16)mask;  // load MO[no] acceptance mask register low  
		
	}
	else
	{
	    CAN_HWOBJ[no].MOAMRH = ((u16)(mask<<2)) & 0x3FFF;
    	CAN_HWOBJ[no].MOAMRL = 0xFFFF;
	}
	
	// Pending Bit
    CAN_HWOBJ[no].MOIPRH = 0x0000;
	if(no<16)
		CAN_HWOBJ[no].MOIPRL = (((u16)no)<<8);
	else if(no<32)
		CAN_HWOBJ[no].MOIPRL = (((u16)no)<<8)|0x11;

	if(cfg&0x04)
	{
		// Function Control
    CAN_HWOBJ[no].MOFCRH = 0x0801;
    CAN_HWOBJ[no].MOFCRL = 0x0000;

	}
	else
	{
		// Function Control
    CAN_HWOBJ[no].MOFCRH = 0x0001;
    CAN_HWOBJ[no].MOFCRL = 0x0000;
	
	}
	
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void can_load(u8 no,u32 id,u8 cfg,u8 *buf)
{
	u16 tmpu16;
	
	CAN_HWOBJ[no].MOCTRL = 0x0020;       // reset MSGVAL
	CAN_HWOBJ[no].MOCTRH = 0x0808;       // set NEWDAT
	fmemcpy((u8 *)CAN_HWOBJ[no].DATA,buf,8);
	CAN_HWOBJ[no].MOCTRH = 0x0020;       // set MSGVAL
	CAN_HWOBJ[no].MOCTRL = 0x0040;       // reset RTSEL
	
	// Set ID
	tmpu16=0x4000;           // Priority Class: List order
	if(cfg&0x04)             // Extended ID
	{
		tmpu16|=0x2000;
		tmpu16|=(u16)(id>>16);
		CAN_HWOBJ[no].MOARH=tmpu16;
		CAN_HWOBJ[no].MOARL=(u16)id;
	}
	else
	{
		tmpu16|=(id<<2);
		CAN_HWOBJ[no].MOARH=tmpu16;
		CAN_HWOBJ[no].MOARL=0x0000;
	}

	tmpu16=cfg&0xf0;
	tmpu16<<=4;           // Set DLC
	// tmpu16|=0x0080;       // Single Transmit Trial
	
    CAN_HWOBJ[no].MOFCRH = tmpu16;
    CAN_HWOBJ[no].MOFCRL = 0x0000;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void can_stx(u8 no)
{
#ifndef WIN32
	// CAN Bus off
	if(CAN_NSR0 & 0x80)
		return;
	// Node Initialization
	if(CAN_NCR0 & 0x01)
		return;

	CAN_HWOBJ[no].MOCTRH = 0x0720;  // set TXRQ,TXEN0,TXEN1,MSGVAL
#else

	u16 id;
	u8 dlc;

	sbus_putc('[');
	sbus_putc('=');
	sbus_putc(']');
	dlc=(CAN_HWOBJ[no].MOFCRH>>8)&0x0f;
	sbus_putc(dlc);
	id=(CAN_HWOBJ[no].MOARH>>2)&0x7ff;

	sbus_putc((u8)id);
	sbus_putc((u8)(id>>8));
	sbus_putc(0);
	sbus_putc(0);

	for(dlc=0;dlc<8;dlc++)
	{
		sbus_putc(CAN_HWOBJ[no].DATA[dlc]);
	}
#endif

}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 can_stx_ex(u8 no)
{
	if(can_busy(no))
	{
		can_abort(no);
		return 1;
	}
	can_stx(no);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void can_abort(u8 no)
{
	CAN_HWOBJ[no].MOCTRL = 0x0720;  // reset TXRQ,TXEN0,TXEN1,MSGVAL
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 can_busy(u8 no)
{
	return (CAN_HWOBJ[no].MOCTRL & 0x0100)?1:0; // MOCTRL => MOSTATL
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void can_sload(u8 no,u8 *buf)
{
	CAN_HWOBJ[no].MOCTRH = 0x0008;       // set NEWDAT
	fmemcpy((u8 *)CAN_HWOBJ[no].DATA,buf,8);
	CAN_HWOBJ[no].MOCTRH = 0x0020;       // set MSGVAL
	CAN_HWOBJ[no].MOCTRL = 0x0040;       // reset RTSEL
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void isr_can_txrx0(void) interrupt CAN_SRN0INT
{
	u16 nodeinfo;
	u16 objinfo;
	u8 objno;
	u32 id;

	nodeinfo = CAN_NSR0;
	
	if (nodeinfo & 0x0008)               // TXOK => reset 
	{
		CAN_NSR0 = 0x0337;
		g_can_status|=0x02;
	}
	
	if (nodeinfo & 0x0010)               // if RXOK => deal => reset
	{
		CAN_NSR0 = 0x032F;
		g_can_status|=0x04;
	}
	
	CAN_MSIMASKH = 0xf000;          // set message index mask register
	CAN_MSIMASKL = 0x000f;
	//CAN_MSIMASKH = 0x8000;
	//CAN_MSIMASKL = 0x0000;
	
	while (CAN_MSID0 != 0x0020)
	{
		objno=(u8)CAN_MSID0;
		objinfo = CAN_HWOBJ[objno].MOCTRL;
		
		if(objinfo & MOSTAT_RXPND)          // Receive interrupt
		{				
			if(objinfo & MOSTAT_NEWDAT)       // if NEWDAT is set
			{
				id=CAN_HWOBJ[objno].MOARH & 0x1FFF;
				id<<=16;
				id|=(CAN_HWOBJ[objno].MOARL);
				if((CAN_HWOBJ[objno].MOARH & 0x2000)==0)  //Notation! ((CAN_HWOBJ[objno].MOARH & 0x20000000)==0) before.
					id>>=18;
				do_objx(objno, id,
					(u8 *)CAN_HWOBJ[objno].DATA,
					(u8)((CAN_HWOBJ[objno].MOFCRH>>8)&0x0f));
			}
			CAN_HWOBJ[objno].MOCTRL = MOSTAT_RST_MNR;            // reset MSGLST + NEWDAT + RXPND
		} 
		if(objno<16)
			CAN_MSPND0L = ~(((u16)1)<<objno);
		else if(objno<32)
			CAN_MSPND0H = ~(((u16)1)<<(objno-16));
	}
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void isr_can_status(void) interrupt CAN_SRN1INT
{
	u16 nodeinfo;
	
	nodeinfo = CAN_NSR0;
	
	if (!(nodeinfo & 0x0020))               // if ALERT
		return;
		
	
	if (nodeinfo & 0x0080)           // if BOFF
	{
		// CAN_NCR0 &= ~((u16)0x0041);  // reset INIT and CCE
		g_can_status|=1;
	}
	
	// if (nodeinfo & 0x0040);          // if EWRN
	
	if (nodeinfo & 0x0100)           // if LLE
	{
		// A list length error has been detected during message
		// acceptance filtering. The number of elements in the list
		// that belongs to this CAN node differs from the list SIZE 
		// given in the list termination pointer
		
		CAN_NSR0      = 0x023F;        // reset LLE
	}
	
	if (nodeinfo & 0x0200)           // if LOE
	{
		// A list object error has been detected during message
		// acceptance filtering. A message object with wrong LIST 
		// index entry in the Message Object Control Register has 
		// been detected
		
		CAN_NSR0      = 0x013F;        // reset LOE
	}
	
	if (CAN_NCR0 & 0x0001)           // if INIT
	{
	}
	
	CAN_NSR0      = 0x031F;          // reset ALERT
	
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void isr_can_error(void) interrupt CAN_SRN2INT
{
	
	u16 nodeinfo;
	
	nodeinfo = CAN_NSR0;
	
	if (!(nodeinfo & 0x0007))               // if LEC
		return;
		
	switch (nodeinfo & 0x0007)       // LEC0 (Last Error Code0)
	{
	case 1: // Stuff Error
		// More than 5 equal bits in a sequence have occurred
		// in a part of a received message where this is not
		// allowed.
		
		break;
		
	case 2: // Form Error
		// A fixed format part of a received frame has the
		// wrong format.
		
		break;
		
	case 3: // Ack Error
		// The message this CAN controller transmitted was
		// not acknowledged by another node.
		
		break;
		
	case 4: // Bit1 Error
		// During the transmission of a message (with the
		// exeption of the arbitration field), the device
		// wanted to send a recessive level ("1"), but the
		// monitored bus value was dominant.
		
		break;
		
	case 5: // Bit0 Error
		// During the transmission of a message (or acknow-
		// ledge bit, active error flag, or overload flag),
		// the device wanted to send a dominant level ("0"),
		// but the monitored bus value was recessive.
		
		// During busoff recovery this status is set each time 
		// a sequence of 11 recessive bits has been monitored.
		// The CPU may use this code as indication that the 
		// bus is not continuously disturbed).
		
		if (nodeinfo & 0x0080)  // if Busoff status
		{
			
		}
		else
		{
		}
		break;
		
	case 6: // CRC Error
		// The CRC check sum was incorrect in the message
		// received.
		
		break;
		
	default:
		break;
	}
	CAN_NSR0      = 0x0338;          // reset LEC
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void set_can_on(void)
{
	CAN_NCR0 &= ~((u16)0x0041);  // reset INIT and CCE
	g_can_status &= (u8)0xFE;    // clear Bus OFF status
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 get_can_bus_off(void)
{
	// CAN Bus off
	// if(CAN_NSR0 & 0x80)
	if(g_can_status & 1)
		return 1;
	else
		return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 can_txx(u32 id,u8 *buf)
{
	UNUSED_PARA(id);
	UNUSED_PARA(buf);

	return 1;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 can_settingx(u32 baudrate, u8 option)
{
	UNUSED_PARA(baudrate);
	UNUSED_PARA(option);

	return 1;
}
////////////////////////////////////////////////////////////////////////////////
