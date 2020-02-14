#include "ssc.h"
#include "hw.h"
#include "mcu1_imp.h"

extern volatile u16 g_ms_uw;

void spi_setting(void)
{
	// Single MCU
	spi_mcu2_setting();
#ifdef EE_HW_SPI
	spi_eeprom_setting();
#endif
}

/////////////////////////////////////////////////////////
/*

*/
/////////////////////////////////////////////////////////
void spi_mcu2_setting(void)
{
// Single MCU
#if 0
	////////////////////////////////////
	U0C0_KSCFG = 0x0003;
	_nop_(); _nop_();
	
	U0C0_CCR       =  0x0000;
	
	// DX0F selected
	U0C0_DX0CR     =  0x0013;      // IN: Data
	
	U0C0_INPRL     =  0x0000;      // Interrupt Node Pointer register 
	U0C0_INPRH     =  0x0000;
	
	U0C0_SCTRL     =  0x0103;      // shift control register
	U0C0_SCTRH     =  0x0707;
	
	U0C0_TCSRL     =  0x0500;      // transmit control/status register
	U0C0_TCSRH     =  0x0000;
	
	U0C0_PSR      |=  0x2000;      // protocol status register
	
	U0C0_0IC       =  0x005E;
	
	P2_IOCR03      =  0x0090;  // MOSI
	P2_IOCR04      =  0x0000;  // MISO
	P2_IOCR05      =  0x0090;  // SCK
	P2_IOCR06      =  0x0090;  // SS
	
	U0C0_CCR       =  0xC001;

#endif
}

// Single MCU
#if 0
void isr_spi_mcu2_rx(void) interrupt U0C0_0INT
{
	// SLVSS can be used to ensure data integration
	u8 ch;
	if (U0C0_PSR & 0xc000)          //Receive Interrupt flag
	{
		U0C0_PSCR|= 0xc000;         //clear PSR_RIF
		ch=(u8)U0C0_RBUF;
		do_spi_char(ch);
	}
}
#endif

/////////////////////////////////////////////////////////
u16 spi_mcu2_trans(u16 ch)
{
	UNUSED_PARA(ch);
	
	return 0;
}

/////////////////////////////////////////////////////////
void spi_mcu2_putc(u16 ch)
{
	U0C0_PSCR |= 0x2000;
	U0C0_TBUF00=ch;
}

/////////////////////////////////////////////////////////
/*

*/
/////////////////////////////////////////////////////////
// Not needed if IO Sim
void spi_eeprom_setting(void)
{
	U0C1_KSCFG = 0x0003;
	_nop_();_nop_();
	U0C1_CCR   = 0x0000;

	// Generate baudrate (4MHz)
	// 80MHz*(0x39a/1024)/(8+1)/2=4MHz
	U0C1_FDRL      =  0x439a;
	U0C1_BRGH      =  0x8008;
	U0C1_BRGL      =  0x0000;
	
	//DX0C is selected
	U0C1_DX0CR     =  0x0012;

	// Disable Interrupt	
	U0C1_INPRL     =  0x0000;
	U0C1_INPRH     =  0x0000;

	// Shift Control
	U0C1_SCTRL     =  0x0103;
	// Frame & Word Length
	U0C1_SCTRH     =  0x0707;

	// Transmission Control	
	U0C1_TCSRL     =  0x0500;
	U0C1_TCSRH     =  0x0000;
	
	// Protocol Control (Delay Control etc., fppp selected as fctqin)
	U0C1_PCRL      =  0x0f17;
	U0C1_PCRH      =  0x0000;

	// For the First Transmission
	U0C1_PSR      |=  0x2000;
	
	// Pin Control	
	P10_IOCR14 = 0x0020; // EE_SDO
	P10_IOCR15 = 0x00A0; // EE_SDI
	P10_IOCR05 = 0x0090; // EE_SCK
	P10_IOCR08 = 0x00A0; // EE_CS

	// SPI in GPIO mode: see gpio_setting
	
	// Function as SSC/SPI	
	U0C1_CCR   = 0x0001;    
}

/////////////////////////////////////////////////////////
u16 spi_eeprom_trans(u16 ch)
{
	WAIT_EVENT_READY((U0C1_PSR & 0x2000),2);

	U0C1_PSCR     |= 0x2000;
	U0C1_TBUF00    = ch;

	//2ms: enough to transmit a char
	WAIT_EVENT_READY(((U0C1_PSR & 0x8000) || (U0C1_PSR & 0x4000)),2);
	U0C1_PSCR     |= 0xC000;
	return(U0C1_RBUF);
}
/////////////////////////////////////////////////////////


