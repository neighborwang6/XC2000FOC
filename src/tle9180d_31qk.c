////////////////////////////////////////////////////////////////////////////////
/**************************************************
*    File Name    : tle9180d-31qk
*    Copyright    : QingChen
*    Module Name  : 
     *
*    CPU          : XC2000
*    RTOS         : No RTOS
*
*    Create Date  : 
*    Author/Cororation: Perfersor YLY
*    Abstract Description:
*
*--------------- Revision History --------------
*    No Version Date      Revisied By  Item    Description
*    1  V0.0    19.11.26  WGY                  eBooster
**************************************************/


/**************************************************
*    Debug switch Section
**************************************************/
//#define xxxx
/**************************************************
*    Include File Section
**************************************************/
#include "tle9180d_31qk.h"
#include "rtc.h"

/**************************************************
*    MacroDefine Section
**************************************************/

#define TLE9180D_INH
#define LE9180D_SOFF
#define LE9180D_ENA

#define Conf_Sig    00H
#define Conf_Gen_1  01H
#define Conf_Gen_2  02H
#define Conf_Gen_3  03H
#define Conf_wwd    04H
#define Tl_vs       05H
#define Tl_vdh      06H
#define Tl_cbvcc    07H
#define Fm_1        08H
#define Fm_2        09H
#define Fm_3        0AH
#define Fm_4        0BH
#define Fm_5        0CH
#define Dt_hs       0DH
#define Dt_ls       0EH
#define Ft_1        0FH
#define Ft_2        10H
#define Ft_3        11H
#define Ft_4        12H
#define Fm_6        13H


#define Op_gain_1_addr        20H
#define Op_gain_2_addr        21H
#define Op_gain_3_addr        22H
#define Op_0cl_addr           23H
#define op_con_addr           24H
#define Sc_ls_1_addr          25H
#define Sc_ls_2_addr          26H
#define Sc_ls_3_addr          27H
#define Sc_hs_1_addr          28H
#define Sc_hs_2_addr          29H
#define Sc_hs_3_addr          2AH
#define Li_ctr_addr           2BH
#define Misc_ctr_addr         2CH
#define art_tlp_addr          2DH
#define art_tla_addr          2EH
#define art_fi_addr           2FH
#define art_acc_addr          30H
#define art_entry_addr        31H
#define nop_addr              32H
#define Drev_mark_addr        33H
#define Ds_mark_addr          34H
#define Sel_st_1_addr         35H
#define Sel_st_2_addr         36H
#define En_st_addr            37H
#define Om_over_addr          40H
#define Err_over_addr         41H
#define Ser_addr              42H
#define Err_i_1_addr          43H
#define Err_i_2_addr          44H
#define Err_e_addr            45H
#define Err_sd_addr           46H
#define Err_scd_addr          47H
#define Err_indiag_addr       48H
#define Err_osf_addr          49H
#define Err_spiconf_addr      4AH
#define Err_op_12_addr        4BH
#define Err_op_3_addr         4CH
#define Err_outp_addr         4DH
#define dsm_ls1_addr          4EH
#define dsm_ls2_addr          4FH
#define dsm_ls3_addr          50H
#define dsm_hs1_addr          51H
#define dsm_hs2_addr          52H
#define dsm_hs3_addr          53H
#define Rdm_ls1_addr          54H
#define Rdm_ls2_addr          55H
#define Rdm_ls3_addr          56H
#define Rdm_hs1_addr          57H
#define Rdm_hs2_addr          58H
#define Rdm_hs3_addr          59H
#define temp_ls1_addr         5AH
#define temp_ls2_addr         5BH
#define temp_ls3_addr         5CH  
#define temp_hs1_addr         5DH
#define temp_hs2_addr         5EH
#define temp_hs3_addr         5FH
#define wwlc_addr             60H
#define res_cc1_addr          61H
#define res_cc2_addr          62H
#define res_cc3_addr          63H
#define res_vcc_addr          64H
#define res_cb_addr           65H
#define res_vs_addr           66H
#define res_vdh_addr          67H

//00 80 03 1C 56 07 a0 95 30 70 10 20 60 0e 0e 85 50 0e 02 00

u8 TLE9180D_Conf_Reg[20]={0x00, 0x80, 0x03, 0x1C, 0x56, 0x07, 0xa0, 0x95, 0x30, 0x70, 
                          0x10, 0x20, 0x60, 0x0e, 0x0e, 0x85, 0x50, 0x0e, 0x02, 0x00};

/**************************************************
*    StructDefine Section
**************************************************/

/**************************************************
*    Prototype Declare Section
**************************************************/

/**************************************************
*    Global Variable Declare Section
**************************************************/

/**************************************************
*    File Static Variable Define Section
**************************************************/

/**************************************************
*    Function Define Section
**************************************************/

void tle9180d_31qk_init(void)
{
	  volatile unsigned int uwTemp;

  //   -----------------------------------------------------------------------
  //   Initialization of USIC Peripherals:
  //   -----------------------------------------------------------------------

  ///  - USIC0 module clock = 80.00 MHz

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Kernel State Configuration:
  ///  -----------------------------------------------------------------------

  U0C0_KSCFG     =  0x0003;      // load U0C0 kernel state configuration 
                                 // register

  uwTemp         =  U0C0_KSCFG;  // dummy read to avoid pipeline effects	
	
	//->>>>  Channel config
	
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

  ///  - Shift Clock output configuration (SCLKCFG) = 1, Clock polarity =1

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
	
	//U0C0_0IC       =  0x005E;    //??
	
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
  P2_IOCR04      =  0x0020; // load port register for data input (Pull-up device connected)

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

}

/////////////////////////////////////////////////////////
void spi_tle9180d_31qk_putc(u16 ch)
{
	U0C0_PSCR |= 0x2000;
	U0C0_TBUF00=ch;
}

/////////////////////////////////////////////////////////
u16 spi_tle9180d_31qk_trans(u16 ch)
{
	WAIT_EVENT_READY((U0C0_PSR & 0x2000),2);

	U0C0_PSCR     |= 0x2000;
	U0C0_TBUF00    = ch;

	//2ms: enough to transmit a char
	WAIT_EVENT_READY(((U0C0_PSR & 0x8000) || (U0C0_PSR & 0x4000)),2);
	U0C0_PSCR     |= 0xC000;
	return(U0C0_RBUF);
}
/////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 crc3(u16 Data)
{
	u8 i;
	u8 checksum=0;
	//The CRC generator polynomial is x^3+x^1+1. 0b1011
	u32 Polynomial=0xB0000000;
	u32 RealPolynomial=0x30000000;
	u32 Gk=(u32)Data;
	
	for(i=0;i<28;i++)
	{
		if((Gk&0x80000000)==1)
		{
			Gk^=RealPolynomial;
			Gk<<=1;
		}
			else
			{
				Gk<<=1;
			}
	}
	
	checksum = (Gk&0x70000000)>>;

	return 0;
	
}

//void TLE9180D_Communicate(u8 C,u8 ADDR,u8 Data)
//{
//	//MOSI: C+ADDR[6:0]+DATA[7:0]+Resverd[4:0]+CRC[3:0]
//	
//	//MISO: SPI status[5:0]+ADDR[6:0]+DATA[7:0]+R+CRC[3:0]
//	
//}