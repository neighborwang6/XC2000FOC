////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
*
*  
*      
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

#ifndef _XC2000_HW_H
#define _XC2000_HW_H

#include "std_defs.h"
#include "xc2365a.h"
#include <intrins.h>
#include <absacc.h>

#include "int_vec.h"

/////////////////////////////////////////////////////////
extern u8 volatile g_overflow_5ms_ub;
extern u8 volatile g_overflow_10ms_ub;

/////////////////////////////////////////////////////////
#define FSYS_HZ 80000000UL
#define FSYS_MHZ 80UL

#define ABS_CYC 10

#define F_VOLT(x) ((u16)((x)*(1023/5.0)))
/////////////////////////////////////////////////////////
enum {  
	AD_VO1          = 0x00,
	AD_SH1_MON      = 0x02,
	AD_AIN3         = 0x03,  //PWM_EX1
	AD_AIN4         = 0x04,  //PWM_EX0
	AD_BAT12V_MON   = 0x05,
	AD_VCC5_SW_MON  = 0x08,
	AD_TEMP_SS1     = 0x09,
	AD_BAT12V_MONY  = 0x0A,
	AD_VCC5B_MON    = 0x0B,
	AD_SH2_MON      = 0x0D,
	AD_SH3_MON      = 0x0F,
	
	AD_VO2          = 0x10,
	//AD_VRO          = 0x12,
	AD_BAT12V_SW    = 0x14,
	AD_ERR2         = 0x15,
	AD_SCDL         = 0x16,

	AD_PRS0         = 0x03,
	//AD_SIN          = 0x10,
	//AD_COS          = 0x14,
	AD_BRK0         = 0x04,
};

/////////////////////////////////////////////////////////
u16 hw_init(void);

void delay_ms(u16 tm);
void delay_msx(u16 tm);
void delay_us(u16 tm);
void delay_ns(u8 tm);

void gpio_setting(void);
void ad_setting(void);
void pll_setting(void);
void capcom_setting(void);

void mcu_unlock(void);
void mcu_lock(void);

/////////////////////////////////////////////////////////
#define BAT24VBY_CTRL P4_OUT_P6
/////////////////////////////////////////////////////////
#define GET_CC20_PIN() (P2_IN_P7)
#define GET_CC21_PIN() (P2_IN_P8)
#define GET_CC22_PIN() (P2_IN_P9)
#define GET_CC23_PIN() (P2_IN_P10)
#define GET_CC26_PIN() (P4_IN_P2)
#define GET_CC27_PIN() (P4_IN_P3)

// SAE J1708/RS485
#define KLINE_RXD P3_IN_P11
#define KLINE_TXD P3_OUT_P10

/////////////////////////////////////////////////////////
// SPI Interface (<=> MCU2)
#define SET_MCU1_MOSI(x) P2_OUT_P3=(x)
#define GET_MCU1_MISO()  (P2_OUT_P4)
#define SET_MCU1_SCK(x)  P2_OUT_P5=(x)
#define SET_MCU1_SS(x)   P2_OUT_P6=(x)

// TLE7189F: Motor Controller
#define GET_TLE_SCDL()   get_ad_value(AD_SCDL)
#define GET_TLE_ERR1()   (P7_IN_P1)
#define GET_TLE_ERR2()   (P15_IN_P5)
#define SET_TLE_ENA(x)   P0_OUT_P7=(x)
#define GET_TLE_ENA()    (P0_IN_P7)

#define SET_VCC5B_EN(x)  P7_OUT_P2=(x)
#define SET_VCC5_HOLD(x) P7_OUT_P0=(x)
#define SET_VCC5_SW(x)   P7_OUT_P4=(x)
#define SET_VCC5_SS(x)   SET_VCC5_SW(x)
#define SET_ISN1_OFFSET(x)   P6_OUT_P0=(x)
#define SET_ISP1_OFFSET(x)   P6_OUT_P1=(x)

#define SET_AD2S_RESET(x)  P7_OUT_P3=(x)
#define SET_AD2S_DIR(x)  P6_OUT_P2=(x)
#define SET_AD2S_CS(x)   P2_OUT_P13=(x)
#define SET_AD2S_WR(x)   P2_OUT_P12=(x)
#define SET_AD2S_RD(x)   P2_OUT_P11=(x)
#define SET_AD2S_A0A1(x) P4_OUT_P0=(x)
#define SET_AD2S_SAMPLE(x) P0_OUT_P6=(x)

#define SET_AD2S_DATA(x) P1_OUT=(x)
#define GET_AD2S_DATA()  (P1_IN)

// 0: Input, 1: Output

enum {
	DIR_IN=0, 
	DIR_OUT=1
};
#define SET_AD2S_DPORT_DIR(x) \
{                       \
	if(x)               \
	{                   \
		P1_IOCR00=0x80; \
		P1_IOCR01=0x80; \
		P1_IOCR02=0x80; \
		P1_IOCR03=0x80; \
		P1_IOCR04=0x80; \
		P1_IOCR05=0x80; \
		P1_IOCR06=0x80; \
		P1_IOCR07=0x80; \
	}                   \
	else                \
	{                   \
		P1_IOCR00=0x00; \
		P1_IOCR01=0x00; \
		P1_IOCR02=0x00; \
		P1_IOCR03=0x00; \
		P1_IOCR04=0x00; \
		P1_IOCR05=0x00; \
		P1_IOCR06=0x00; \
		P1_IOCR07=0x00; \
	}                   \
}

// MCU2 Disable the UVW switch, MCU1 read back the control
#define GET_DIS_UVW() (P10_IN_P6)
#define SET_EN_UVW(x) P10_OUT_P7=(x)

// Set to '1': Diable Motor controller
#define SET_SCDL_DIS(x) P2_OUT_P2=(x)
#define SET_BAT12V_SW(x) P10_OUT_P4=(x)

/////////////////////////////////////////////////////////
#define GET_IGN() (P4_IN_P1)
/////////////////////////////////////////////////////////
// EEPROM Interface
// #define EE_HW_SPI

#ifdef EE_HW_SPI
 #define SET_EESCK(x)
 #define SET_EESDI(x)
 #define GET_EESDO()
#else
 #define SET_EESCK(x) P10_OUT_P5=(x)
 #define SET_EESDI(x) P10_OUT_P15=(x)
 #define GET_EESDO()  (P10_IN_P14)
#endif

#define SET_EESS(x)  P10_OUT_P8=(x)
#define SET_EEWP(x)

/////////////////////////////////////////////////////////
// Inherit
#define clrbit(var,bitn) (var &=(unsigned short)(~(0x01<<(bitn))))
#define setbit(var,bitn) (var |=(0x01<<(bitn)))
#define INTR_ENABLE()  {PSW_IEN=1;}
#define INTR_DISABLE() {PSW_IEN=0;}
#define CAPCOM_OFF()    {CC2_CC16IC_IE=0;CC2_CC17IC_IE=0;CC2_CC18IC_IE=0;CC2_CC19IC_IE=0;}     
#define CAPCOM_ON()     {CC2_CC16IC_IE=1;CC2_CC17IC_IE=1;CC2_CC18IC_IE=1;CC2_CC19IC_IE=1;}    
#define NOP4()    {_nop_();_nop_();_nop_();_nop_();}

#define WAIT_EVENT_READY(event,tm)   \
   {                                 \
       u16 tm0,tm1;                  \
                                     \
       tm0=tm1=g_ms_uw;              \
       while(!(event))               \
       {                             \
           tm1=g_ms_uw;              \
           tm1-=tm0;                 \
           if(tm1>tm)                \
           {                         \
               break;                \
           }                         \
		   _srvwdt_();               \
       }                             \
   }

#define RELAY_ON()
#define RELAY_OFF()
#define GET_RELAY() 0
#define GET_RELAY_STATUS() 0

void keep_alive(u8 on_off);
   
/////////////////////////////////////////////////////////
#endif /* define _XC2000_HW_H */
