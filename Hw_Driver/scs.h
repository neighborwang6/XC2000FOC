//****************************************************************************
// @Module        Project Settings
// @Filename      SCS.H
// @Project       xc2kTest.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC2365A-104F80
//
// @Compiler      Keil
//
// @Codegenerator 2.0
//
// @Description   This file is the header for the SCS driver
//             
//                Note:
//                This file should not be changed by 
//                the user
//
//----------------------------------------------------------------------------
// @Date          2010-4-18 21:53:02
//
//****************************************************************************

// USER CODE BEGIN (SCS_Header,1)

// USER CODE END



#ifndef _SCS_H_
#define _SCS_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (SCS_Header,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (SCS_Header,3)

// USER CODE END

#define CCU60_TCTR0_T13R (CCU60_TCTR0 & (1U <<12U))
#define SCU_PLLCON0_NDIV ((SCU_PLLCON0 >> 8U) & 0x3FU)
#define SCU_PLLCON1_PDIV ((SCU_PLLCON1 >> 8U) & 0xFU)
#define SCU_PLLCON2_K1DIV (SCU_PLLCON2 & 0x3FFU)
#define SCU_PLLCON3_K2DIV (SCU_PLLCON3 & 0x3FFU)
#define SCU_PLLSTAT_VCOBYST (SCU_PLLSTAT & (1U <<0U))
#define SCU_PLLSTAT_OSCSELST (SCU_PLLSTAT & (1U <<2U))
#define SCU_PLLSTAT_VCOLOCK (SCU_PLLSTAT & (1U <<3U))
#define SCU_PLLSTAT_PRDY (SCU_PLLSTAT & (1U <<5U))
#define SCU_PLLSTAT_NRDY (SCU_PLLSTAT & (1U <<6U))
#define SCU_PLLSTAT_K1RDY (SCU_PLLSTAT & (1U <<7U))
#define SCU_PLLSTAT_K2RDY (SCU_PLLSTAT & (1U <<8U))
#define SCU_PLLSTAT_OSCLOCK (SCU_PLLSTAT & (1U <<15U))


//****************************************************************************
// @Defines
//****************************************************************************

// Notes: All frequencies are specified in [Hz].

// Crystal frequency fCRYST at XTAL1/XTAL2, given by hardware
// Range: 4000000...25000000
#define SCS_F_XTAL 8000000
// Target value of PLL frequency fPLL for normal operation mode
// Range: 10000000...80000000
#define SCS_F_PLL_TARGET 80000000
// Maximum PLL frequency step
#define SCS_F_MAXSTEP 20000000
// If not user defined:
// Target VCO frequency fVCO for normal operation mode
#ifndef SCS_F_VCO_TARGET
#define SCS_F_VCO_TARGET 160000000
#endif
// If not user defined:
// Target frequency after P divider fP for normal operation mode
#ifndef SCS_F_P_TARGET
#define SCS_F_P_TARGET 4000000
#endif
// If not user defined:
// Calculate P divider for normal operation mode, round up
#ifndef SCS_P
#define SCS_P ((SCS_F_XTAL + SCS_F_P_TARGET - 1)/SCS_F_P_TARGET)
#endif
// Calculate frequency after P divider fP for normal operation mode, round down
#define SCS_F_P (SCS_F_XTAL/SCS_P)
// If not user defined:
// Calculate final K2 divider for normal operation mode, round down to avoid SCS_F_VCO > SCS_F_VCO_TARGET
#ifndef SCS_K2
#define SCS_K2 (SCS_F_VCO_TARGET/SCS_F_PLL_TARGET)
#endif
// If not user defined:
// Calculate N divider for normal operation mode, round to nearest; avoid SCS_F_VCO > SCS_F_VCO_TARGET
#ifndef SCS_N
#define SCS_N_TEMP ((SCS_F_PLL_TARGET * SCS_K2 + SCS_F_P/2)/SCS_F_P)
#  if SCS_F_P * SCS_N_TEMP > SCS_F_VCO_TARGET
#     define SCS_N (SCS_N_TEMP - 1)
#  else
#     define SCS_N SCS_N_TEMP
#  endif
#endif
// Calculate actual VCO frequency fVCO for normal operation operation
#define SCS_F_VCO (SCS_F_P * SCS_N)
// Calculate K2 for 5 MHz, round to nearest
#define SCS_K2_5MHZ ((SCS_F_VCO + 2500000)/5000000)
// Cactual actual PLL frequency fPLL_5MHZ for normal operation mode
#define SCS_F_PLL_5MHZ (SCS_F_VCO/SCS_K2_5MHZ)
// Calculate actual PLL frequency fPLL for normal operation mode
#define SCS_F_PLL (SCS_F_VCO/SCS_K2)

// If not user defined and if necessary:
// Calculate additional K2 step for transition from free-running to normal operation mode, round up
#ifndef SCS_K2_1
#  if SCS_F_PLL - SCS_F_PLL_5MHZ > SCS_F_MAXSTEP
#     define SCS_K2_1 ((SCS_F_VCO + SCS_F_PLL_5MHZ + SCS_F_MAXSTEP - 1)/(SCS_F_PLL_5MHZ + SCS_F_MAXSTEP))
#  endif
#endif

// If not user defined:
// Calculate additional K2 step for transition from free-running to normal operation mode, round up
#ifndef SCS_K2_2
#  ifdef SCS_K2_1
#    if SCS_F_PLL - SCS_F_VCO/SCS_K2_1 > SCS_F_MAXSTEP
#       define SCS_K2_2_TEMP ((SCS_F_VCO + SCS_F_VCO/SCS_K2_1 + SCS_F_MAXSTEP - 1)/(SCS_F_VCO/SCS_K2_1 + SCS_F_MAXSTEP))
#       if  SCS_K2_2_TEMP < SCS_K2_1
#           define SCS_K2_2 SCS_K2_2_TEMP
#       endif
#    endif
#  endif
#endif

// If not user defined:
// Calculate additional K2 step for transition from free-running to normal operation mode, round up
# ifndef SCS_K2_3
#  ifdef SCS_K2_2
#    if SCS_F_PLL - SCS_F_VCO/SCS_K2_2 > SCS_F_MAXSTEP
#       define SCS_K2_3_TEMP ((SCS_F_VCO + SCS_F_VCO/SCS_K2_2 + SCS_F_MAXSTEP - 1)/(SCS_F_VCO/SCS_K2_2 + SCS_F_MAXSTEP))
#       if  SCS_K2_3_TEMP < SCS_K2_2
#           define SCS_K2_3 SCS_K2_3_TEMP
#       endif
#    endif
#  endif
# endif


//****************************************************************************
// @Typedefs
//****************************************************************************

// Type for SCS error

typedef enum
{
  SCS_STATE_NO_ERROR  = 0,       // no error
  SCS_STATE_TO_OSC_HP_PLLV,      // timeout for high precision osc. PLLV
  SCS_STATE_OFW_ATTEMPTS_OSC_HP, // overflow of attempts for high precision osc.
  SCS_STATE_TO_OSC_HP_SEL,       // timeout for high precision osc. selection
  SCS_STATE_TO_K1DIV,            // timeout for K1 divider setting
  SCS_STATE_TO_K2DIV,            // timeout for K2 divider setting
  SCS_STATE_TO_PDIV,             // timeout for P divider setting
  SCS_STATE_TO_NDIV,             // timeout for N divider setting
  SCS_STATE_TO_VCOBYST,          // timeout for VCO bypass switching
  SCS_STATE_TO_VCO_LOCK,         // timeout for VCO lock
  SCS_STATE_VCO_UNLOCKED        // VCO is unlocked
} SCS_ErrorType;

// Type for EXTCLK MUX setting

typedef enum
{
  SCS_EXTCLK_F_SYS    = 0, // system frequency
  SCS_EXTCLK_F_OUT    = 1, // programmable output frequency fOUT
  SCS_EXTCLK_F_PLL    = 2, // PLL output frequency fPLL
  SCS_EXTCLK_F_OSC    = 3, // high precision oscillator frequency fOSC
  SCS_EXTCLK_F_WU     = 4, // wakeup oscillator frequency fWU
  SCS_EXTCLK_F_CLKIN1 = 5, // direct input frequency fCLKIN1
  SCS_EXTCLK_F_RTC    = 8  // real-time frequency fRTC
} SCS_Extclk_Type;


//****************************************************************************
// @Imported Global Variables
//****************************************************************************


//****************************************************************************
// @Global Variables
//****************************************************************************


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

// Use case functions
SCS_ErrorType SCS_GoFromBaseToNormalMode(void);
void SCS_InitTimer(void);
void SCS_RestoreTimer(void);

// Expert level functions
_inline void SCS_EnableOscHiPrecCrystal(void);
SCS_ErrorType SCS_CheckFreqOscHiPrec(void);
SCS_ErrorType SCS_ApplyNewPDiv(unsigned int PDiv);
SCS_ErrorType SCS_ApplyNewNDiv(unsigned int NDiv);
SCS_ErrorType SCS_ApplyNewK1Div(unsigned int K1Div);
SCS_ErrorType SCS_ApplyNewK2Div(unsigned int K2Div);
_inline void SCS_RestartVcoLockDetect(void);
_inline void SCS_SelectVcoBand(unsigned int VcoBand);
SCS_ErrorType SCS_SelectVcoSrcOscHiPrec(void);
_inline void SCS_DisconnectVcoInput(void);
_inline void SCS_ConnectVcoInput(void);
SCS_ErrorType SCS_EnableVcoLockEmerg(void);
SCS_ErrorType SCS_DisableVcoBypass(void);
SCS_ErrorType SCS_EnableVcoBypass(void);
_inline void SCS_StartTimer(unsigned int Cycles);
_inline void SCS_DelayByTimer(unsigned int Cycles);


#endif  // ifndef _SCS_H_
