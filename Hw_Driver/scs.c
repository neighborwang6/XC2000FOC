//****************************************************************************
// @Module        Project Settings
// @Filename      SCS.C
// @Project       xc2kTest.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC2365A-104F80
//
// @Compiler      Keil
//
// @Codegenerator 2.0
//
// @Description   This file contains the SCS driver.
//                      
//                Note:
//                This file should not be changed by 
//                the user.
//
//----------------------------------------------------------------------------
// @Date          2010-4-18 21:53:02
//
//****************************************************************************

// USER CODE BEGIN (SCS_General,1)

// USER CODE END



//****************************************************************************
// @Project Includes
//****************************************************************************

#include "scs.h"
#include  "xc2365a.h"
// USER CODE BEGIN (SCS_General,2)
// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (SCS_General,3)

// USER CODE END


// Function-like macros ******************************************************

// PLLSTAT_COND_3: For polling a PLLSTAT condition up to 3 times; an "if"
// construct needs at least 12 instruction cycles before 3rd check. "
// Depending on the compiler (optimization) and the program location, the
// effective number of instruction cycles may be higher.
#define PLLSTAT_COND_3(Condition) (((Condition) && (Condition)) && (Condition))

// GET_CYCLES: Converts Time[us] and MaxSysFreq[Hz] into cycles for timer,
// rounding up; for compile-time use only.
#define GET_CYCLES(Time, MaxSysFreq) \
        ((unsigned int)(((((long)(Time))*((MaxSysFreq)/1000))+15999)/16000))

// Constants that may be overwritten by the user *****************************

// If not user defined:
// Number of attempts for high precision osc. start
#ifndef SCS_ATTEMPTS_OSC_HP
#define SCS_ATTEMPTS_OSC_HP 10
#endif

// If not user defined:
// Timeout in [us] for PLLV after high precision osc. start
#ifndef SCS_TIME_OSC_HP_PLLV
#define SCS_TIME_OSC_HP_PLLV 5000
#endif

// If not user defined:
// Delay time/timeout in [us] for 1024 cycles after high precision osc. start
#ifndef SCS_TIME_OSC_HP_1024
#define SCS_TIME_OSC_HP_1024 1000
#endif

// Constants that cannot be overwritten by the user **************************

// Maximum system frequency values in [Hz]
#define SCS_F_INT_5MHZ 5200000
#define SCS_F_INT_10MHZ 10400000

// Other times in [us]
#define SCS_TIME_VCO_BAND_SWITCH 150
#define SCS_TIME_VCO_FINDIS_TO_BASE 50
#define SCS_TIME_VCO_K2 5
#define SCS_TIME_VCO_LOCK 200

// Check clock parameters ****************************************************

// Check fXTAL
#if SCS_F_XTAL < 4000000 || SCS_F_XTAL > 25000000
#   error  "SCS_F_XTAL out of range"
#endif

// Check fPLL target frequency
#if SCS_F_PLL_TARGET < 1000000 || SCS_F_PLL_TARGET > 80000000
#   error  "SCS_F_PLL_TARGET out of range"
#endif

// check fP
#if SCS_F_P < 4000000 || SCS_F_P > 16000000
#   error  "SCS_F_P out of range"
#endif

// check fVCO
#if SCS_F_VCO < 10000000 || SCS_F_VCO > 160000000
#   error  "SCS_F_VCO out of range"
#endif

// Check P divider
#if SCS_P < 1 || SCS_P > 16
#   error  "SCS_P out of range"
#endif

// Check N divider
#if SCS_N < 16 || SCS_N > 40
#   error  "SCS_N out of range"
#endif

// Check K2_5MHZ divider
#if SCS_K2_5MHZ < 1 || SCS_K2_5MHZ > 512
#   error  "SCS_K2_5MHZ out of range"
#endif

// Check K2 divider and additional K2 divider steps if defined

#if SCS_K2 < 1 || SCS_K2 > 512
#   error  "SCS_K2 out of range"
#endif

#ifdef SCS_K2_1
#  if SCS_K2_1 < 1 || SCS_K2_1 > 512
#     error  "SCS_K2_1 out of range"
#  endif
#endif

#ifdef SCS_K2_2
#  if SCS_K2_2 < 1 || SCS_K2_2 > 512
#     error  "SCS_K2_2 out of range"
#  endif
#endif

#ifdef SCS_K2_3
#  if SCS_K2_3 < 1 || SCS_K2_3 > 512
#     error  "SCS_K2_3 out of range"
#  endif
#endif

#ifdef SCS_K2_4
#  if SCS_K2_4 < 1 || SCS_K2_4 > 512
#     error  "SCS_K2_4 out of range"
#  endif
#endif



//****************************************************************************
// @Typedefs
//****************************************************************************



//****************************************************************************
// @Imported Global Variables
//****************************************************************************



//****************************************************************************
// @Global Variables
//****************************************************************************



//****************************************************************************
// @Privare Variables
//****************************************************************************

// Saved value of CCU60_KSCFG
static unsigned int Ccu60KscfgSave;

// Saved value of CCU60 MCFG
static unsigned int Ccu60McfgSave;

// Saved value of CCU60 TCTR0
static unsigned int Ccu60Tctr0Save;

// Saved value of CCU60 TCTR2
static unsigned int Ccu60Tctr2Save;

// Saved value of CCU60 PISELH
static unsigned int Ccu60PiselhSave;



//****************************************************************************
// @External Prototypes
//****************************************************************************



//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

static SCS_ErrorType SCS_RampUpPllInNormalMode(void);


//****************************************************************************
// @Local Functions
//****************************************************************************

//****************************************************************************;
// @Function      inline void SCS_EnableOscHiPrecCrystal(void)
//
//-----------------------------------------------------------------------------
// @Description   This expert level function configures the high precision
//                oscillator for crystal mode.
//                
//                Notes:
//                - The user is responsible for disabling the register
//                  protection.
//
//-----------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_EnableOscHiPrecCrystal,1)

// USER CODE END

_inline void SCS_EnableOscHiPrecCrystal(void)
{
   // clear MODE for external crystal/clock, clear SHBY to disable shaper bypass,
   // clear rh and w bits, leave other bits unchanged
   SCU_HPOSCCON &=
    ((0U    <<0U) | // PLLV for PLL Valid Status Bit (rh)
                    // 0: OSC_HP frequency is not usable
                    // 1: OSC_HP frequency is usable
     (0U    <<1U) | // OSCWDTRST Oscillator Watchdog Reset
                    // 0 : Osc. watchdog is not reset and remains active
                    // 1 : Osc. watchdog is reset and restarted
     (0U    <<2U) | // MODE Oscillator Mode
                    // 0 : External crystal/clock, no osc. power-saving mode
                    // 1 : OSC_HP disabled, no osc. power-saving mode
                    // 2 : External clock, osc. power-saving mode
                    // 3 : OSC_HP disabled, osc. power-saving mode
     (0U    <<4U) | // GAINSEL Oscillator Gain Selection (rh)
                    // 0: gain control from 4 MHz to 8 MHz
                    // 1: gain control from 4 MHz to 16 MHz
                    // 2: gain control from 4 MHz to 20 MHz
                    // 3: gain control from 4 MHz to 25 MHz
     (0U    <<6U) | // X1D XTAL1 Data Value (rh)
                    // inverted level of pin XTAL1 if X1DEN is set
     (1U    <<7U) | // X1DEN XTAL1 Data Enable
                    // 0 : 0 Bit X1D is not updated
                    // 1 : 1 Bit X1D can be updated
     (0U    <<8U) | // SHBY Shaper Bypass
                    // 0 : 0 Shaper is not bypassed
                    // 1 : 1 Shaper is bypassed
     (1U    <<9U) | // EMCLKEN OSCWDT Emergency System Clock Source Select Enable
                    // 0 : MCM controlled by SYSCON0.CLKSEL in OSCWDT emergency case
                    // 1 : MCM controlled by SYSCON0.EMCLKSEL in OSCWDT emergency case
     (1U    <<10U)| // EMFINDISEN Emergency Input Clock Disconnect Enable
                    // 0 : PLLSTAT.FINDIS not updated in OSCWDT emergency case
                    // 1 : PLLSTAT.FINDIS is set in OSCWDT emergency case
     (0U    <<11U)| // OSC2L1 OSCWDT Reached Status (rh)
                    // 0: OSCWDT did not detect frequency below limit
                    // 1: OSCWDT detected frequency below limit
     (0U    <<12U));// OSC2L0 OSCWDT Left Status (rh)
                    // 0: OSCWDT did not detect frequency above limit
                    // 1: OSCWDT detected frequency above limit

} // end of function SCS_EnableOscHiPrecCrystal

//****************************************************************************;
// @Function      inline void SCS_RestartVcoLockDetect(void)
//
//-----------------------------------------------------------------------------
// @Description   This expert level function restarts the VCO lock detection.
//                
//                Notes:
//                - The user is responsible for disabling the register
//                  protection.
//
//-----------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_RestartVcoLockDetect,1)

// USER CODE END

_inline void SCS_RestartVcoLockDetect(void)
{
   // restart VCO lock detection
   SCU_PLLCON1_RESLD =
    1U;             // RESLD: Restart VCO Lock Detection (w)
                    // Setting this bit will reset bit PLLSTAT.VCOLOCK and
                    // restart the VCO lock detection

} // end of function SCS_RestartVcoLockDetect

//****************************************************************************;
// @Function      inline void SCS_DisconnectVcoInput(void)
//
//-----------------------------------------------------------------------------
// @Description   This expert level function disconnects VCO from its clock
//                input.
//                
//                Notes:
//                - The user is responsible for disabling the register
//                  protection.
//
//-----------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_DisconnectVcoInput,1)

// USER CODE END

_inline void SCS_DisconnectVcoInput(void)
{
   // disconnect VCO from its clock input
   SCU_STATCLR1 =
     (0U    <<0U) | // Clear PLLSTAT.VCOL0 (w)
     (0U    <<1U) | // Clear PLLSTAT.VCOL1 (w)
     (0U    <<2U) | // Clear HPOSCCON.OSC2L1 (w)
     (0U    <<3U) | // Clear HPOSCCON.OSC2L2 (w)
     (1U    <<4U) | // Set PLLSTAT.FINDIS (w)
     (0U    <<5U) ; // Clear PLLSTAT.FINDIS (w)

} // end of function SCS_DisconnectVcoInput

//****************************************************************************;
// @Function      inline void SCS_ConnectVcoInput(void)
//
//-----------------------------------------------------------------------------
// @Description   This expert level function connects VCO from its clock
//                input.
//                
//                Notes:
//                - The user is responsible for disabling the register
//                  protection.
//
//-----------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_ConnectVcoInput,1)

// USER CODE END

_inline void SCS_ConnectVcoInput(void)
{
   // connect VCO from its clock input
   SCU_STATCLR1 =
     (0U    <<0U) | // Clear PLLSTAT.VCOL0 (w)
     (0U    <<1U) | // Clear PLLSTAT.VCOL1 (w)
     (0U    <<2U) | // Clear HPOSCCON.OSC2L1 (w)
     (0U    <<3U) | // Clear HPOSCCON.OSC2L2 (w)
     (0U    <<4U) | // Set PLLSTAT.FINDIS (w)
     (1U    <<5U) ; // Clear PLLSTAT.FINDIS (w)

} // end of function SCS_ConnectVcoInput

//****************************************************************************;
// @Function      inline void SCS_SelectVcoBand(unsigned int VcoBand)
//
//-----------------------------------------------------------------------------
// @Description   This expert level function sets the required VCO band.
//                
//                Notes:
//                - The user is responsible for disabling the register
//                  protection.
//
//-----------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------
// @Parameters    VcoBand: 0 or 1
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_SelectVcoBand,1)

// USER CODE END

_inline void SCS_SelectVcoBand(unsigned int VcoBand)
{
   // select VCO band
   SCU_PLLCON0_VCOSEL = VcoBand;

} // end of function SCS_SelectVcoBand

//****************************************************************************;
// @Function      inline void SCS_StartTimer(unsigned int Cycles)
//
//-----------------------------------------------------------------------------
// @Description   This expert level function starts timer T13 for counting
//                a certain number of clocks.
//                
//                Notes:
//                - It is assumed that the timer is already initialized by 
//                  SCS_InitTimer.
//                - Cycles < 3 are set to 3 for hardware reasons.
//
//-----------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------
// @Parameters    Cycles: Number of T13 clocks to count, 0...65535
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_StartTimer,1)

// USER CODE END

_inline void SCS_StartTimer(unsigned int Cycles)
{
   // stop T13 for security reasons, clear T13
   CCU60_TCTR4 =
     (0U    <<0U) | // T12RR Timer 12 Run Reset (w)
     (0U    <<1U) | // T12RS Timer 12 Run Set (w)
     (0U    <<2U) | // T12RES Timer 12 Reset (w)
     (0U    <<3U) | // DTRES Dead-Time Counter Reset (w))
     (0U    <<5U) | // T12CNT Timer T12 Count Event if enabled (PISELH) (w)
     (0U    <<6U) | // T12STR Timer 12 Shadow Transfer Request (w)
     (0U    <<7U) | // T12STD Timer 12 Shadow Transfer Disable (w)
     (1U    <<8U) | // T13RR Timer 13 Run Reset (w)
     (0U    <<9U) | // T13RS Timer 13 Run Set (w)
     (1U    <<10U)| // T13RES Timer 13 Reset (w)
     (0U    <<13U)| // T13CNT Timer T13 Count Event if enabled (PISELH) (w)
     (0U    <<14U)| // T13STR Timer 13 Shadow Transfer Request (w)
     (0U    <<15U); // T13STD Timer 13 Shadow Transfer Disable (w)

   // limit T13 period
   if (Cycles < 3U)
   {
     Cycles = 3U;
   }

   //set T13 period
   CCU60_T13PR = Cycles - 2U;

   // enable T13 shadow transfer for period setting
   CCU60_TCTR4 =
     (0U    <<0U) | // T12RR Timer 12 Run Reset (w)
     (0U    <<1U) | // T12RS Timer 12 Run Set (w)
     (0U    <<2U) | // T12RES Timer 12 Reset (w)
     (0U    <<3U) | // DTRES Dead-Time Counter Reset (w))
     (0U    <<5U) | // T12CNT Timer T12 Count Event if enabled (PISELH) (w)
     (0U    <<6U) | // T12STR Timer 12 Shadow Transfer Request (w)
     (0U    <<7U) | // T12STD Timer 12 Shadow Transfer Disable (w)
     (0U    <<8U) | // T13RR Timer 13 Run Reset (w)
     (0U    <<9U) | // T13RS Timer 13 Run Set (w)
     (0U    <<10U)| // T13RES Timer 13 Reset (w)
     (0U    <<13U)| // T13CNT Timer T13 Count Event if enabled (PISELH) (w)
     (1U    <<14U)| // T13STR Timer 13 Shadow Transfer Request (w)
     (0U    <<15U); // T13STD Timer 13 Shadow Transfer Disable (w)

   // start T13
   CCU60_TCTR4 =
     (0U    <<0U) | // T12RR Timer 12 Run Reset (w)
     (0U    <<1U) | // T12RS Timer 12 Run Set (w)
     (0U    <<2U) | // T12RES Timer 12 Reset (w)
     (0U    <<3U) | // DTRES Dead-Time Counter Reset (w))
     (0U    <<5U) | // T12CNT Timer T12 Count Event if enabled (PISELH) (w)
     (0U    <<6U) | // T12STR Timer 12 Shadow Transfer Request (w)
     (0U    <<7U) | // T12STD Timer 12 Shadow Transfer Disable (w)
     (0U    <<8U) | // T13RR Timer 13 Run Reset (w)
     (1U    <<9U) | // T13RS Timer 13 Run Set (w)
     (0U    <<10U)| // T13RES Timer 13 Reset (w)
     (0U    <<13U)| // T13CNT Timer T13 Count Event if enabled (PISELH) (w)
     (0U    <<14U)| // T13STR Timer 13 Shadow Transfer Request (w)
     (0U    <<15U); // T13STD Timer 13 Shadow Transfer Disable (w)

} // end of function SCS_StartTimer

//****************************************************************************;
// @Function      inline void SCS_DelayByTimer(unsigned int Cycles)
//
//-----------------------------------------------------------------------------
// @Description   This expert level function starts timer T13 and waits until
//                the specified number of clocks is counted.
//                
//                Notes:
//                - It is assumed that the timer is already initialized by 
//                  SCS_InitTimer.
//                - Cycles < 3 are set to 3 for hardware reasons.
//
//-----------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------
// @Parameters    Cycles: Number of T13 clocks to count, 0...65535
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_DelayByTimer,1)

// USER CODE END

_inline void SCS_DelayByTimer(unsigned int Cycles)
{
   // start delay timer T13
   SCS_StartTimer(Cycles);

   // wait until delay time clocks are counted
   do
   {
   }
   while(CCU60_TCTR0_T13R);

} // end of function SCS_DelayByTimer


//****************************************************************************;
// @Function      SCS_ErrorType SCS_RampUpPllInNormalMode (void)
//
//-----------------------------------------------------------------------------
// @Description   This private function performs a ramp-up of the PLL in 
//                Normal Operation Mode.
//
//                Notes: 
//                - The user is responsible for disabling the register
//                  protection.
//-----------------------------------------------------------------------------
// @Returnvalue   Error code
//
//-----------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_RampUpPllInNormalMode,1)

// USER CODE END


static SCS_ErrorType SCS_RampUpPllInNormalMode(void)
{
   SCS_ErrorType Error;

   #ifdef SCS_K2_1
     // set additional K2 divider step SCS_K2_1 if needed
     Error = SCS_ApplyNewK2Div(((unsigned int)SCS_K2_1) - 1U);
     if(Error)
     {
       return Error;
     }

     #ifdef SCS_K2_2
       // wait delay time for K2 with new system clock
       SCS_DelayByTimer(GET_CYCLES(SCS_TIME_VCO_K2, SCS_F_VCO/SCS_K2_1));

       // set additional K2 divider step SCS_K2_2 if needed
       Error = SCS_ApplyNewK2Div(((unsigned int)SCS_K2_2) - 1U);
       if(Error)
       {
         return Error;
       }

       #ifdef SCS_K2_3
         // wait delay time for K2 switch with new system clock
         SCS_DelayByTimer(GET_CYCLES(SCS_TIME_VCO_K2, SCS_F_VCO/SCS_K2_2));

         // set additional K2 divider step SCS_K2_3 if needed
         Error = SCS_ApplyNewK2Div((((unsigned int)SCS_K2_3)) - 1U);
         if(Error)
         {
           return Error;
         }
         // wait delay time for K2_3 with new system clock
         SCS_DelayByTimer(GET_CYCLES(SCS_TIME_VCO_K2, SCS_F_VCO/SCS_K2_3));
       #else
         // no SCS_K2_3: wait delay time for K2_2 with new system clock
         SCS_DelayByTimer(GET_CYCLES(SCS_TIME_VCO_K2, SCS_F_VCO/SCS_K2_2));
       #endif  // SCS_K2_3

     #else
       // no SCS_K2_2: wait delay time for K2_1 with new system clock
       SCS_DelayByTimer(GET_CYCLES(SCS_TIME_VCO_K2, SCS_F_VCO/SCS_K2_1));
     #endif  // SCS_K2_2

   #endif  // SCS_K2_1

   // set final K2 divider SCS_K2
   Error = SCS_ApplyNewK2Div((((unsigned int)SCS_K2)) - 1U);
   if(Error)
   {
     return Error;
   }

   // no error
   return SCS_STATE_NO_ERROR;

}  // end of function SCS_RampUpPllInNormalMode


//****************************************************************************;
// @Function      SCS_ErrorType SCS_GoFromBaseToNormalMode (void)
//
//-----------------------------------------------------------------------------
// @Description   This use case function performs a transition from Base Mode 
//                to Normal Operation Mode.
//
//                Notes: 
//                - The user is responsible for disabling the register
//                  protection.
//-----------------------------------------------------------------------------
// @Returnvalue   Error code
//
//-----------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_GoFromBaseToNormalMode,1)

// USER CODE END


SCS_ErrorType SCS_GoFromBaseToNormalMode(void)
{
   SCS_ErrorType Error;

   // enable high precision oscillator for crystal
   SCS_EnableOscHiPrecCrystal();

   // check high precision oscillator frequency
   Error = SCS_CheckFreqOscHiPrec();
   if(Error)
   {
     return Error;
   }

   // set K1 divider = 1 for VCO bypass frequency of 5 MHz
   Error = SCS_ApplyNewK1Div(1U - 1U);
   if(Error)
   {
     return Error;
   }

   // enable VCO bypass, new fSYS = 5 MHz
   Error = SCS_EnableVcoBypass();
   if(Error)
   {
     return Error;
   }

   if(!SCU_PLLCON0_VCOSEL)
   {  // current VCO band = 0:

      // select VCO band 1
      SCS_SelectVcoBand(1U);

      // wait band switch delay time with 5 MHz int. osc.
      SCS_DelayByTimer(GET_CYCLES(SCS_TIME_VCO_BAND_SWITCH, SCS_F_INT_5MHZ));
   }
   else
   {  // current VCO band = 1:

      // set K2 divider = 8
      Error = SCS_ApplyNewK2Div(8U - 1U);
      if(Error)
      {
        return Error;
      }
   }

   // set final P divider
   Error = SCS_ApplyNewPDiv((((unsigned int)SCS_P)) - 1U);
   if(Error)
   {
     return Error;
   }

   // set final N divider
   Error = SCS_ApplyNewNDiv((((unsigned int)SCS_N)) - 1U);
   if(Error)
   {
     return Error;
   }

   // disconnect VCO from clock input (enable free-running oscillator)
   SCS_DisconnectVcoInput();

   // wait until base frequency is reached with 10 MHz int. osc.
   SCS_DelayByTimer(GET_CYCLES(SCS_TIME_VCO_FINDIS_TO_BASE, SCS_F_INT_10MHZ));

   // disable VCO bypass, new fSYS ~ (50 MHz / 8) ~ 6.3 MHz
   Error = SCS_DisableVcoBypass();
   if(Error)
   {
     return Error;
   }

   // select high precision oscillator as VCO source
   Error = SCS_SelectVcoSrcOscHiPrec();
   if(Error)
   {
     return Error;
   }

   // set K2 for fSYS = 5 MHz with final settings, new fSYS ~ (50 MHz / K2)
   // for fVCO = 160 MHz: K2 = 32, new fSYS ~ 1.6 MHz
   Error = SCS_ApplyNewK2Div(((unsigned int)SCS_K2_5MHZ) - 1U);
   if(Error)
   {
     return Error;
   }

   // connect VCO to clock input, new fSYS ~ 5MHz
   SCS_ConnectVcoInput();

   // restart VCO lock detection
   SCS_RestartVcoLockDetect();

   // start timer for VCO lock with maximum ~ 5 MHz
   SCS_StartTimer(GET_CYCLES(SCS_TIME_VCO_LOCK, SCS_F_INT_5MHZ));

   // wait until lock occurs or timeout is over
   do
   {
   }
   while((!SCU_PLLSTAT_VCOLOCK) && CCU60_TCTR0_T13R);
   if(!SCU_PLLSTAT_VCOLOCK)
   {
     // no VCO lock
     return SCS_STATE_TO_VCO_LOCK;
   }

   // enable VCOLCK emergency
   Error = SCS_EnableVcoLockEmerg();
   if(Error)
   {
     return Error;
   }

   // ramp up PLL for fSys = final value in normal operation mode
   Error = SCS_RampUpPllInNormalMode();
   if(Error)
   {
     return Error;
   }

   // no error
   return SCS_STATE_NO_ERROR;

}  // end of function SCS_GoFromBaseToNormalMode


//****************************************************************************;
// @Function      void SCS_InitTimer (void)
//-----------------------------------------------------------------------------
// @Description   This function configures CCU6 timer T13 as one-shot timer with
//                a resolution of 16 CCU6 clocks = 16 system clocks.
//
//                Notes:
//                - The function will enable CCU6 and will overwrite previous
//                  settings for T13.
//                - The function will save the previous CCU6 register contents;
//                  they may be restored via SCS_RestoreTimer.
//                - The user must call this function before any other SCS driver
//                  function. (except SCS_SelectBandgapHiPrec if needed).
//                - The user is responsible for disabling the register protection.
//
//-----------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_InitTimer,1)

// USER CODE END

void SCS_InitTimer(void)
{
   volatile unsigned int Work;

   // save CCU60_KSCFG
   Ccu60KscfgSave = CCU60_KSCFG;

   // enable CC60 module
   CCU60_KSCFG |=
     (1U    <<0U) | // MODEN Module Enable
     (1U    <<1U) | // BPMODEN Bit Protection for MODEN, set to 1 for change (w)
     (0U    <<4U) | // NOMCFG Normal Operation Mode Configuration
                    // kernel mode applied in normal operation mode
     (0U    <<7U) | // BPNOM Bit Protection for NOMCFG, set to 1 for change (w)
     (0U    <<8U) | // SUMCFG Suspend Mode Configuration
                    // Kernel mode applied in suspend mode
     (0U    <<11U)| // BPSUM Bit Protection for SUMCFG, set to 1 for change (w)
     (0U    <<12U)| // COMCFG Clock Off Mode Configuration
                    // kernel mode applied in clock off mode
     (0U    <<15U); // BPCOM Bit Protection for COMCFG, set to 1 for change (w)

   // read SFR back to avoid pipeline effects
   Work = CCU60_KSCFG;

   // save CCU60_MCFG
   Ccu60McfgSave = CCU60_MCFG;

   // enable T13 functionality, leave other bits unchanged
   CCU60_MCFG |=
     (0U    <<0U) | // T12 T12 Available
     (1U    <<1U) | // T13 T13 Available
     (0U    <<2U);  // MCM Multi-Channel Mode Available
 
   // stop T13, clear T13
   // (CCU60_TCTR4 needs not be saved)
   CCU60_TCTR4 =
     (0U    <<0U) | // T12RR Timer 12 Run Reset (w)
     (0U    <<1U) | // T12RS Timer 12 Run Set (w)
     (0U    <<2U) | // T12RES Timer 12 Reset (w)
     (0U    <<3U) | // DTRES Dead-Time Counter Reset (w)
     (0U    <<5U) | // T12CNT Timer T12 Count Event if enabled (PISELH) (w)
     (0U    <<6U) | // T12STR Timer 12 Shadow Transfer Request (w)
     (0U    <<7U) | // T12STD Timer 12 Shadow Transfer Disable (w)
     (1U    <<8U) | // T13RR Timer 13 Run Reset (w)
     (0U    <<9U) | // T13RS Timer 13 Run Set (w)
     (1U    <<10U)| // T13RES Timer 13 Reset (w)
     (0U    <<13U)| // T13CNT Timer T13 Count Event if enabled (PISELH) (w)
     (0U    <<14U)| // T13STR Timer 13 Shadow Transfer Request (w)
     (0U    <<15U); // T13STD Timer 13 Shadow Transfer Disable (w)

   // save CCU60_TCTR0
   Ccu60Tctr0Save = CCU60_TCTR0;
 
   // set T13 period, for fCC6/16, clear T13PRE to disable additional prescaler,
   // leave bits T12CLK, T12PRE, CTM unchanged
   CCU60_TCTR0 = (CCU60_TCTR0 & ((7U <<0U)|(1U <<3U)|(1U <<7U))) |
     ((0U    <<0U) | // T12CLK Timer T12 Input Clock Select
                    // 0: f = fCC6
                    // 1: f = fCC6 / 2
                    // 2: f = fCC6 / 4
                    // 3: f = fCC6 / 8
                    // 4: f = fCC6 / 16
                    // 5: f = fCC6 / 32
                    // 6: f = fCC6 / 64
                    // 7: f = fCC6 / 128
     (0U    <<3U) | // T12PRE Timer T12 Prescaler Bit
                    // 0: additional prescaler disabled
                    // 1: additional prescaler enabled
     (0U    <<4U) | // T12R Timer T12 Run Bit (rh)
                    // 0: Timer is stopped
                    // 1: Timer is running
     (0U    <<5U) | // STE12 Timer T12 Shadow Transfer Enable (rh)
                    // 0: Shadow register transfer is disabled
                    // 1: Shadow register transfer is enabled
     (0U    <<6U) | // CDIR Count Direction of Timer T12 (rh)
                    // 0: T12 counts up
                    // 1: T12 counts down
     (0U    <<7U) | // CTM T12 Operating Mode
                    // 0: Edge-aligned Mode
                    // 1: Center-aligned Mode
     (4U    <<8U) | // T13CLK Timer T13 Input Clock Select
                    // 0: f = fCC6
                    // 1: f = fCC6 / 2
                    // 2: f = fCC6 / 4
                    // 3: f = fCC6 / 8
                    // 4: f = fCC6 / 16
                    // 5: f = fCC6 / 32
                    // 6: f = fCC6 / 64
                    // 7: f = fCC6 / 128
     (0U    <<11U)| // T13PRE Timer T13 Prescaler Bit
                    // 0: additional prescaler disabled
                    // 1: additional prescaler enabled
     (0U    <<12U)| // T13R Timer T13 Run Bit (rh)
                    // 0: Timer is stopped
                    // 1: Timer is running
     (0U    <<13U));// STE13 Timer T13 Shadow Transfer Enable (rh)
                    // 0: Shadow register transfer is disabled
                    // 1: Shadow register transfer is enabled

   // save CCU60_TCTR2
   Ccu60Tctr2Save = CCU60_TCTR2;

   // enable T13 single-shot, clear other T13 bits,
   // leave bits T12SSC and T12RSEL unchanged
   CCU60_TCTR2 = (CCU60_TCTR2 & ((1U <<0U)|(3U <<8U))) |
   ((0U    <<0U) | // T12SSC T12 Single Shot Control
    (1U    <<1U) | // T13SSC T13 Single Shot Control
    (0U    <<2U) | // T13TEC T13 Trigger Event Control to start T13
                   // 0: No action
                   // 1: T13R set on T12 compare event on channel 0
                   // 2: T13R set on T12 compare event on channel 1
                   // 3: T13R set on T12 compare event on channel 2
                   // 4: T13R set on any T12 compare event (ch. 0, 1, 2)
                   // 5: T13R set on period-match of T12
                   // 6: T13R set on zero-match of T12 (while counting up)
                   // 7: Any edge of inputs CCPOSx
    (0U    <<5U) | // T13TED T13 Trigger Event Direction for T13TEC trigger
                   // 0: Reserved, no action
                   // 1: While T12 is counting up
                   // 2: While T12 is counting down
                   // 3: Independent on the count direction of T12
    (0U    <<8U) | // T12RSEL T12 External Run Selection
                   // 0: External setting of T12R disabled
                   // 1: T12R set on rising edge of T12HR
                   // 2: T12R set on falling edge of T12HR
                   // 3: T12R set on any edge of T12HR
    (0U    <<10U));// T13RSEL T13 External Run Selection
                   // 0: External setting of T13R is disabled
                   // 1: T13R set on rising edge of T13HR
                   // 2: T13R set on falling edge of T13HR
                   // 3: T13R set on any edge of T13HR

   // save CCU60_PISELH
   Ccu60PiselhSave = CCU60_PISELH;

   // enable T13HRA input (rising edge), leave T12 bits unchanged
    CCU60_PISELH = (CCU60_PISELH & (3U <<2U)) |
    ((0U    <<0U) | // IST13HR T13 Input Select for T13HR
                    // 0: T13HRA, 1: T13HRB, 2: T13HRC, 3: T13HRD
     (0U    <<2U) | // ISCNT12 T12 Input Select for Counting Input
                    // 0: T12 prescaler, 1: TCTR4.CNT12 written with 1,
                    // 2: Rising edge on T12HR signal, 3: Falling edge on T12HR signal
     (0U    <<4U)); // ISCNT13 T13 Input Select for Counting Input
                    // 0: T13 prescaler, 1: TCTR4.CNT13 written with 1,
                    // 2: Rising edge on T13HR signal, 3: Falling edge on T13HR signal

} // end of function SCS_InitTimer

//****************************************************************************;
// @Function      void SCS_RestoreTimer (void)
//-----------------------------------------------------------------------------
// @Description   This expert level function restores the old CCU6 register
//                values.
//
//                Notes:
//                - It is assumed that the CCU6 registers have been saved by
//                  execution of SCS_InitTimer.
//                - The function will stop and clear T13.
//                - The user may call this function after the last SCS driver
//                  function.
//                - The user is responsible for disabling the register protection.
//
//-----------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_RestoreTimer,1)

// USER CODE END

void SCS_RestoreTimer(void)
{
   // stop T13, clear T13
   CCU60_TCTR4 =
     (0U    <<0U) | // T12RR Timer 12 Run Reset (w)
     (0U    <<1U) | // T12RS Timer 12 Run Set (w)
     (0U    <<2U) | // T12RES Timer 12 Reset (w)
     (0U    <<3U) | // DTRES Dead-Time Counter Reset (w)
     (0U    <<5U) | // T12CNT Timer T12 Count Event if enabled (PISELH) (w)
     (0U    <<6U) | // T12STR Timer 12 Shadow Transfer Request (w)
     (0U    <<7U) | // T12STD Timer 12 Shadow Transfer Disable (w)
     (1U    <<8U) | // T13RR Timer 13 Run Reset (w)
     (0U    <<9U) | // T13RS Timer 13 Run Set (w)
     (1U    <<10U)| // T13RES Timer 13 Reset (w)
     (0U    <<13U)| // T13CNT Timer T13 Count Event if enabled (PISELH) (w)
     (0U    <<14U)| // T13STR Timer 13 Shadow Transfer Request (w)
     (0U    <<15U); // T13STD Timer 13 Shadow Transfer Disable (w)

  // restore CCU60_MCFG
  CCU60_MCFG = Ccu60McfgSave;

  // restore CCU60_TCTR0
  CCU60_TCTR0 = Ccu60Tctr0Save;

  // restore CCU60_TCTR2
  CCU60_TCTR2 = Ccu60Tctr2Save;

  // restore CCU60_PISELH
  CCU60_PISELH = Ccu60PiselhSave;

  // restore CCU60_KSCFG, enable change
  CCU60_KSCFG = Ccu60KscfgSave |
    ((0U    <<0U) | // MODEN Module Enable
     (1U    <<1U) | // BPMODEN Bit Protection for MODEN, set to 1 for change (w)
     (0U    <<4U) | // NOMCFG Normal Operation Mode Configuration
                    // kernel mode applied in normal operation mode
     (0U    <<7U) | // BPNOM Bit Protection for NOMCFG, set to 1 for change (w)
     (0U    <<8U) | // SUMCFG Suspend Mode Configuration
                    // Kernel mode applied in suspend mode
     (0U    <<11U)| // BPSUM Bit Protection for SUMCFG, set to 1 for change (w)
     (0U    <<12U)| // COMCFG Clock Off Mode Configuration
                    // kernel mode applied in clock off mode
     (0U    <<15U));// BPCOM Bit Protection for COMCFG, set to 1 for change (w)

} // end of function SCS_RestoreTimer

//****************************************************************************;
// @Function      SCS_ErrorType SCS_CheckFreqOscHiPrec (void)
//
//-----------------------------------------------------------------------------
// @Description   This expert level function checks the high precision
//                oscillator for valid output frequency.
//
//-----------------------------------------------------------------------------
// @Returnvalue   Error code
//
//-----------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_CheckFreqOscHiPrec,1)

// USER CODE END


SCS_ErrorType SCS_CheckFreqOscHiPrec(void)
{
   unsigned int Attempts;

   // initialize attempts
   Attempts = (unsigned int)SCS_ATTEMPTS_OSC_HP;

   do
   {
     // start timer
     SCS_StartTimer(GET_CYCLES(SCS_TIME_OSC_HP_PLLV, SCS_F_INT_10MHZ));

     // wait until oscillator is usable or time is over
     do
     {
     }
     while((!SCU_HPOSCCON_PLLV) && CCU60_TCTR0_T13R);

     if(!SCU_HPOSCCON_PLLV)
     {
       // timeout error
       return SCS_STATE_TO_OSC_HP_PLLV;
     }

     // clear status bit for high precision osc. frequency below limit
     SCU_STATCLR1 =
       (0U    <<0U) | // Clear PLLSTAT.VCOL0 (w)
       (0U    <<1U) | // Clear PLLSTAT.VCOL1 (w)
       (1U    <<2U) | // Clear HPOSCCON.OSC2L1 (w)
       (0U    <<3U) | // Clear HPOSCCON.OSC2L0 (w)
       (0U    <<4U) | // Set PLLSTAT.FINDIS (w)
       (0U    <<5U) ; // Clear PLLSTAT.FINDIS (w)


     // start timer, timeout for fSYS = 10 MHz
     SCS_StartTimer(GET_CYCLES(SCS_TIME_OSC_HP_1024, SCS_F_INT_10MHZ));

     // wait until frequency error occurs, oscillator is stable or time is over
     do
     {
     }
     while((!SCU_HPOSCCON_OSC2L1) && (!SCU_PLLSTAT_OSCLOCK) && CCU60_TCTR0_T13R);
     if(!SCU_HPOSCCON_OSC2L1 && SCU_PLLSTAT_OSCLOCK)
     {
       // no frequency error, oscillator is stable
       return SCS_STATE_NO_ERROR;
     }
     // decrement attempts
     Attempts--;
   }
   while(Attempts != 0U);

   // error: too many attempts
   return SCS_STATE_OFW_ATTEMPTS_OSC_HP;

}  // end of function SCS_CheckFreqOscHiPrec

//****************************************************************************;
// @Function      SCS_ErrorType SCS_ApplyNewPDiv (unsigned int PDiv)
//
//-----------------------------------------------------------------------------
// @Description   This expert level function applies a new P divider value to
//                the PLL frequency.
//                Notes:
//                - If the divider is already set to the specified value, the
//                  function will return immediately without error.
//                - The user is responsible for disabling the register protection.
//
//-----------------------------------------------------------------------------
// @Returnvalue   Error code
//
//-----------------------------------------------------------------------------
// @Parameters    PDiv: P divider value
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_ApplyNewPDiv,1)

// USER CODE END


SCS_ErrorType SCS_ApplyNewPDiv(unsigned int PDiv)
{
   if(PDiv == SCU_PLLCON1_PDIV)
   {
     // new value = old value: do nothing, return without error
     return SCS_STATE_NO_ERROR;
   }

   // clear status bit for high precision osc. frequency below limit
   SCU_STATCLR1 =
     (1U    <<0U) | // Clear PLLSTAT.VCOL0 (w)
     (1U    <<1U) | // Clear PLLSTAT.VCOL1 (w)
     (0U    <<2U) | // Clear HPOSCCON.OSC2L1 (w)
     (0U    <<3U) | // Clear HPOSCCON.OSC2L0 (w)
     (0U    <<4U) | // Set PLLSTAT.FINDIS (w)
     (0U    <<5U) ; // Clear PLLSTAT.FINDIS (w)

   // set P divider value. clear P acknowledge
   // leave bits PLLPWD, OSCSEL, AOSCSEL, EMCLKEN, EMFINDISEN unchanged
   SCU_PLLCON1 = 
     (SCU_PLLCON1 & ((1U <<0U)|(1U <<1U)|(1U <<3U)|(1U <<5U)|(1U <<6U))) | 
    ((0U    <<0U) | // PLLPWD PLL Power Saving Mode
                    // 0: Normal behavior
                    // 1: PLL block is put into power saving mode
     (0U    <<1U) | // OSCSEL Oscillator Input Selection
                    // 0: PLL external oscillator
                    // 1: PLL internal oscillator
     (0U    <<2U) | // RESLD Restart VCO Lock Detection (w)
                    // Resets PLLSTAT.VCOLOCK and restarts VCO lock detection
     (0U    <<3U) | // AOSCSEL Asynchronous Oscillator Input Selection
                    // 0: Configuration is controlled via bit OSCSEL
                    // 1: PLL internal clock is selected asynchronously
     (0U    <<5U) | // EMCLKEN VCOLCK Emergency System Clock Source Select Enable
                    // 0: MCM controlled by SYSCON0.CLKSEL in VCOLCK emergency case
                    // 1: MCM controlled by SYSCON0.EMCLKSEL in VCOLCK emergency case
     (0U    <<6U) | // EMFINDISEN Emergency Input Clock Disconnect Enable
                    // 0: PLLSTAT.FINDIS not updated in VCOLCK emergency case
                    // 1: PLLSTAT.FINDIS is set in VCOLCK emergency case
     (PDiv  <<8U) | // PDIV P-Divider Value
                    // 0..15: P-Divider = PDIV + 1
     (0U    <<15U));// PACK P-Divider Ready Acknowledge
                    // Provides acknowledge to PRDY

   // wait until P acknowledge = 0 with timeout
   if(PLLSTAT_COND_3(SCU_PLLSTAT_PRDY))
   {
     return SCS_STATE_TO_PDIV;
   }

   // set P acknowledge
   // SCU_PLLCON1_PACK = 1U; ; not available depending on SFR file
   SCU_PLLCON1 |= (1U <<15U);

   // wait until P acknowledge = 1 with timeout
   if(PLLSTAT_COND_3(!SCU_PLLSTAT_PRDY))
   {
     return SCS_STATE_TO_PDIV;
   }

   // no error
   return SCS_STATE_NO_ERROR;

}  // end of function SCS_ApplyNewPDiv

//****************************************************************************;
// @Function      SCS_ErrorType SCS_ApplyNewNDiv (unsigned int NDiv)
//
//-----------------------------------------------------------------------------
// @Description   This expert level function applies a new N divider value to
//                the PLL frequency.
//                Notes:
//                - If the divider is already set to the specified value, the
//                  function will return immediately without error.
//                - The user is responsible for disabling the register protection.
//
//-----------------------------------------------------------------------------
// @Returnvalue   Error code
//
//-----------------------------------------------------------------------------
// @Parameters    NDiv: N divider value
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_ApplyNewNDiv,1)

// USER CODE END


SCS_ErrorType SCS_ApplyNewNDiv(unsigned int NDiv)
{
   if(NDiv == SCU_PLLCON0_NDIV)
   {
     // new value = old value: do nothing, return without error
     return SCS_STATE_NO_ERROR;
   }

   // clear VCO Lock Detection Lost and VCO Lock Detection Reached
   SCU_STATCLR1 =
     (1U    <<0U) | // Clear PLLSTAT.VCOL0 (w)
     (1U    <<1U) | // Clear PLLSTAT.VCOL1 (w)
     (0U    <<2U) | // Clear HPOSCCON.OSC2L1 (w)
     (0U    <<3U) | // Clear HPOSCCON.OSC2L0 (w)
     (0U    <<4U) | // Set PLLSTAT.FINDIS (w)
     (0U    <<5U) ; // Clear PLLSTAT.FINDIS (w)

   // set N divider value. clear N acknowledge
   // leave bits VCOBY, VCOPWD, VCOSEL unchanged
   SCU_PLLCON0 = (SCU_PLLCON0 & ((1U <<0U)|(1U <<1U)|(1U <<2U))) |
    ((0U    <<0U) | // VCOBY Bypass
                    // 0: Normal operation, VCO is not bypassed
                    // 1: Prescaler Mode; VCO is bypassed
     (0U    <<1U) | // VCOPWD VCO Power Saving Mode
                    // 0: Normal behavior
                    // 1: VCO is put into power saving mode
     (0U    <<2U) | // VCOSEL VCO Range Select
                    // 0: 48...112 MHz
                    // 1: 96...160 MHz
     (0U    <<4U) | // REGENCLR PLL Power Regulator Enable Clear (w)
                    // 0: Configuration for PLL power regulator  unchanged
                    // 1: PLL is not powered (no operation possible)
     (0U    <<5U) | // REGENSET PLL Power Regulator Enable Set (w)
                    // 0: Configuration for PLL power regulator unchanged
                    // 1: PLL is powered (operation possible)
     (NDiv  <<8U) | // NDIV N-Divider Value
                    // 0..63: N-Divider = NDIV + 1
     (0U    <<15U));// NACK N-Divider Ready Acknowledge
                    // Provides acknowledge to NRDY

   // wait until N acknowledge = 0 with timeout
   if(PLLSTAT_COND_3(SCU_PLLSTAT_NRDY))
   {
     return SCS_STATE_TO_NDIV;
   }

   // set N acknowledge
   // SCU_PLLCON0_NACK = 1U; not available depending on SFR file
   SCU_PLLCON0 |= (1U <<15U);

   // wait until N acknowledge = 1 with timeout
   if(PLLSTAT_COND_3(!SCU_PLLSTAT_NRDY))
   {
     return SCS_STATE_TO_NDIV;
   }

   // no error
   return SCS_STATE_NO_ERROR;

}  // end of function SCS_ApplyNewNDiv

//****************************************************************************;
// @Function      SCS_ErrorType SCS_ApplyNewK1Div (unsigned int K1Div)
//
//-----------------------------------------------------------------------------
// @Description   This expert level function applies a new K1 divider value to
//                the PLL frequency.
//                Notes:
//                - If the divider is already set to the specified value, the
//                  function will return immediately without error.
//                - The user is responsible for disabling the register protection.
//
//-----------------------------------------------------------------------------
// @Returnvalue   Error code
//
//-----------------------------------------------------------------------------
// @Parameters    K1Div: K1 divider value
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_ApplyNewK1Div,1)

// USER CODE END


SCS_ErrorType SCS_ApplyNewK1Div(unsigned int K1Div)
{
   if(K1Div == SCU_PLLCON2_K1DIV)
   {
     // new value = old value: do nothing, return without error
     return SCS_STATE_NO_ERROR;
   }

   // set K1 divider value, clear K1 acknowledge 
   SCU_PLLCON2 = 
     (K1Div <<0U) | // K1DIV K1-Divider Value
                    // 0..511: K1-Divider = K1DIV + 1
     (0U    <<15U); // K1ACK K1-Divider Ready Acknowledge
                    // Provides acknowledge to K1RDY

   // wait until K1 acknowledge = 0 with timeout
   if(PLLSTAT_COND_3(SCU_PLLSTAT_K1RDY))
   {
     return SCS_STATE_TO_K1DIV;
   }

   // set K1 acknowledge
   SCU_PLLCON2_K1ACK = 1U;

   // wait until K1 acknowledge = 1 with timeout
   if(PLLSTAT_COND_3(!SCU_PLLSTAT_K1RDY))
   {
     return SCS_STATE_TO_K1DIV;
   }

   // no error
   return SCS_STATE_NO_ERROR;

}  // end of function SCS_ApplyNewK1Div

//****************************************************************************;
// @Function      SCS_ErrorType SCS_ApplyNewK2Div (unsigned int K2Div)
//
//-----------------------------------------------------------------------------
// @Description   This expert level function applies a new K2 divider value to
//                the PLL frequency.
//                Notes:
//                - If the divider is already set to the specified value, the
//                  function will return immediately without error.
//                - The user is responsible for disabling the register protection.
//
//-----------------------------------------------------------------------------
// @Returnvalue   Error code
//
//-----------------------------------------------------------------------------
// @Parameters    K2Div: K2 divider value
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_ApplyNewK2Div,1)

// USER CODE END


SCS_ErrorType SCS_ApplyNewK2Div(unsigned int K2Div)
{
   if(K2Div == SCU_PLLCON3_K2DIV)
   {
     // new value = old value: do nothing, return without error
     return SCS_STATE_NO_ERROR;
   }

   // set K2 divider value, clear K2 acknowledge 
   SCU_PLLCON3 = 
     (K2Div <<0U) | // K2DIV K2-Divider Value
                    // 0..511: K2-Divider = K2DIV + 1
     (0U    <<15U); // K2ACK K2-Divider Ready Acknowledge
                    // Provides acknowledge to K2RDY

   // wait until K2 acknowledge = 0 with timeout
   if(PLLSTAT_COND_3(SCU_PLLSTAT_K2RDY))
   {
     return SCS_STATE_TO_K2DIV;
   }

   // set K2 acknowledge
   // SCU_PLLCON3_K2ACK = 1U; not available depending on SFR file
   SCU_PLLCON3 |= (1U <<15U);

   // wait until K2 acknowledge = 1 with timeout
   if(PLLSTAT_COND_3(!SCU_PLLSTAT_K2RDY))
   {
     return SCS_STATE_TO_K2DIV;
   }

   // no error
   return SCS_STATE_NO_ERROR;

}  // end of function SCS_ApplyNewK2Div

//****************************************************************************;
// @Function      SCS_ErrorType SCS_SelectVcoSrcOscHiPrec (void)
//
//-----------------------------------------------------------------------------
// @Description   This expert level function selects the high precision 
//                oscillator as the source for the VCO.
//                Notes:
//                - The user is responsible for disabling the register protection.
//
//-----------------------------------------------------------------------------
// @Returnvalue   Error code
//
//-----------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_SelectVcoSrcOscHiPrec,1)

// USER CODE END


SCS_ErrorType SCS_SelectVcoSrcOscHiPrec(void)

{
   // clear OSCSEL. clear AOSCSEL, leave other bits unchanged
   SCU_PLLCON1 &=
     (1U    <<0U) | // PLLPWD PLL Power Saving Mode
                    // 0: Normal behavior
                    // 1: PLL block is put into power saving mode
     (0U    <<1U) | // OSCSEL Oscillator Input Selection
                    // 0: PLL external oscillator
                    // 1: PLL internal oscillator
     (1U    <<2U) | // RESLD Restart VCO Lock Detection (w)
                    // Resets PLLSTAT.VCOLOCK and restarts VCO lock detection
     (0U    <<3U) | // AOSCSEL Asynchronous Oscillator Input Selection
                    // 0: Configuration is controlled via bit OSCSEL
                    // 1: PLL internal clock is selected asynchronously
     (1U    <<5U) | // EMCLKEN VCOLCK Emergency System Clock Source Select Enable
                    // 0: MCM controlled by SYSCON0.CLKSEL in VCOLCK emergency case
                    // 1: MCM controlled by SYSCON0.EMCLKSEL in VCOLCK emergency case
     (1U    <<6U) | // EMFINDISEN Emergency Input Clock Disconnect Enable
                    // 0: PLLSTAT.FINDIS not updated in VCOLCK emergency case
                    // 1: PLLSTAT.FINDIS is set in VCOLCK emergency case
     (0xFU  <<8U) | // PDIV P-Divider Value
                    // 0..15: P-Divider = PDIV + 1
     (1U    <<15U); // PACK P-Divider Ready Acknowledge
                    // Provides acknowledge to PRDY

   // wait until high precision oscillator is selected (OSCSELST = 0), with timeout
   if(PLLSTAT_COND_3(SCU_PLLSTAT_OSCSELST))
   {
     return SCS_STATE_TO_OSC_HP_SEL;
   }

   // no error
   return SCS_STATE_NO_ERROR;

}  // end of function SCS_SelectVcoSrcOscHiPrec

//****************************************************************************;
// @Function      SCS_ErrorType SCS_EnableVcoLockEmerg (void)
//
//-----------------------------------------------------------------------------
// @Description   This expert level function enables the VCO loss-of-lock
//                emergency handling.
//
//-----------------------------------------------------------------------------
// @Returnvalue   Error code
//
//-----------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_EnableVcoLockEmerg,1)

// USER CODE END


SCS_ErrorType SCS_EnableVcoLockEmerg(void)

{
   unsigned int Work;

   if(!SCU_PLLSTAT_VCOLOCK)
   {
     // VCO unlocked
     return SCS_STATE_VCO_UNLOCKED;
   }

   // clear VCO Lock Detection Lost and VCO Lock Detection Reached
   SCU_STATCLR1 =
     (1U    <<0U) | // Clear PLLSTAT.VCOL0 (w)
     (1U    <<1U) | // Clear PLLSTAT.VCOL1 (w)
     (0U    <<2U) | // Clear HPOSCCON.OSC2L1 (w)
     (0U    <<3U) | // Clear HPOSCCON.OSC2L0 (w)
     (0U    <<4U) | // Set PLLSTAT.FINDIS (w)
     (0U    <<5U) ; // Clear PLLSTAT.FINDIS (w)

  // disable emergency clock selection
  SCU_PLLCON1_EMCLKEN = 0U;

  // disable emergency clock selection
  SCU_SYSCON0_EMCLKSELEN = 0U;

   // clear loss-of-lock emergency state
   SCU_STATCLR0 =
     (0U    <<12U) | // Clear bit SYSCON0.EMSOSC (w)
     (1U    <<13U) ; // Clear bit SYSCON0.EMSVCO (w)

  // enable emergency input clock disconnection
  SCU_PLLCON1_EMFINDISEN = 1U;

  // clear VCOLCK Trap Request Flag
  SCU_TRAPCLR =
    (0U    <<0U) | // FAT Clear Flash Access Trap Request
    (0U    <<1U) | // ESR0T Clear ESR0 Trap Request
    (0U    <<2U) | // ESR1T Clear ESR1 Trap Request
    (0U    <<3U) | // ESR2T Clear ESR2 Trap Request
    (0U    <<4U) | // OSCWDTT Clear OSCWDT Trap Request
    (0U    <<5U) | // RAT Clear Register Access Trap Request
    (0U    <<6U) | // PET Clear Register Parity Error Trap Request
    (1U    <<7U) ; // VCOLCKT Clear VCOLCK Trap Request

  // enable VCOLCK trap request
  SCU_TRAPDIS &=
   ((1U    <<0U) | // FAT Disable Flash Access Trap Request
    (1U    <<1U) | // ESR0T Disable ESR0 Trap Request
    (1U    <<2U) | // ESR1T Disable ESR1 Trap Request
    (1U    <<3U) | // ESR2T Disable ESR2 Trap Request
    (1U    <<4U) | // OSCWDTT Disable OSCWDT Trap Request
    (1U    <<5U) | // RAT Disable Register Access Trap Request
    (1U    <<6U) | // PET Disable Register Parity Error Trap Request
    (0U    <<7U)); // VCOLCKT Disable VCOLCK Trap Request

  Work = SCU_PLLSTAT;

  if((Work &
   ((0U    <<0U) | // VCOBYST VCO Bypass Status Freerunning/Normal (rh)
    (0U    <<1U) | // PWDSTAT Power-saving Mode Status (rh)
    (0U    <<2U) | // OSCSELST Oscillator Input Selection Status OSC_PLL (rh)
    (0U    <<3U) | // VCOLOCK PLL VCO Lock Status (rh)
    (0U    <<4U) | // BY Bypass Mode Status (rh)
    (0U    <<5U) | // PRDY P-Divider Ready Status (rh)
    (0U    <<6U) | // NRDY N-Divider Ready Status (rh)
    (0U    <<7U) | // K1RDY K1-Divider Ready Status (rh)
    (0U    <<8U) | // K2RDY K1-Divider Ready Status (rh)
    (0U    <<9U) | // FINDIS Input Clock Disconnect Select Status (rh)
    (1U    <<10U)| // VCOL0 VCO Lock Detection Lost Status (rh)
    (1U    <<11U)| // VCOL1 VCO Lock Detection Reached Status (rh)
    (0U    <<12U)))// REGSTAT PLL Power Regulator Status powered (operation possible) (rh)
    || (!(Work &
    (1U    <<3U))))// VCOLOCK PLL VCO Lock Status (rh)

    {
      // VCOL0 = 1 or VCOL1 = 1 or VCOLOCK = 0: VCO unlocked
      return SCS_STATE_VCO_UNLOCKED;
    }

   // no error
   return SCS_STATE_NO_ERROR;

}  // end of function SCS_EnableVcoLockEmerg

//****************************************************************************;
// @Function      SCS_ErrorType SCS_DisableVcoBypass (void)
//
//-----------------------------------------------------------------------------
// @Description   This expert level function disables VCO bypass.
//                Notes:
//                - The user is responsible for disabling the register protection.
//
//-----------------------------------------------------------------------------
// @Returnvalue   Error code
//
//-----------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_DisableVcoBypass,1)

// USER CODE END


SCS_ErrorType SCS_DisableVcoBypass(void)

{
   // disable VCO bypass
   SCU_PLLCON0_VCOBY = 0U;

   // wait until VCO bypass status = 1 with timeout
   if(PLLSTAT_COND_3(!SCU_PLLSTAT_VCOBYST))
   {
     return SCS_STATE_TO_VCOBYST;
   }


   // no error
   return SCS_STATE_NO_ERROR;

}  // end of function SCS_DisableVcoBypass

//****************************************************************************;
// @Function      SCS_ErrorType SCS_EnableVcoBypass (void)
//
//-----------------------------------------------------------------------------
// @Description   This expert level function enables VCO bypass.
//                Notes:
//                - The user is responsible for disabling the register protection.
//
//-----------------------------------------------------------------------------
// @Returnvalue   Error code
//
//-----------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------
// @Date          2010-4-18
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_EnableVcoBypass,1)

// USER CODE END


SCS_ErrorType SCS_EnableVcoBypass(void)

{
   // enable VCO bypass
   SCU_PLLCON0_VCOBY = 1U;

   // wait until VCO bypass status = 0 with timeout
   if(PLLSTAT_COND_3(SCU_PLLSTAT_VCOBYST))
   {
     return SCS_STATE_TO_VCOBYST;
   }


   // no error
   return SCS_STATE_NO_ERROR;

}  // end of function SCS_EnableVcoBypass
