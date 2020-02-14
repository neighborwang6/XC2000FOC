#include "ccu2_ccu6.h"
#include "pwm.h"
#include "sent_decode.h"

///////////////////////////////////////////////////////////////////////////////
// CC20,CC21,CC23,CC22,CC26,CC27: CC23 <=> CC22 (swap)
// 0~10000: => 0~100%
u16 g_pwm_ex_duty_uw[6]={0,0,0,0,0,0};

// 0.2us/bit
u16 g_pwm_ex_period_uw[6]={20000,20000,40000,25000,0,0};
const u16 G_PWM_EX_PERIOD_STD[6]={20000,20000,40000,25000,0,0};

// Time of previous edge
u16 g_pwm_ex_timestamp0[6]={0,0,0,0,0,0};
// Time of previous rising edge
u16 g_pwm_ex_timestamp0_r[6]={0,0,0,0,0,0};

// Time of current edge
u16 g_pwm_ex_timestamp[6]={0,0,0,0,0,0};

u16 g_disp_value_sent=0; //The Relative Displacement of the initial position of the pedal.(Origional:CC2_CC22(PWM_EX3))
///////////////////////////////////////////////////////////////////////////////
// WGY comment out the function void cc2_setting(void) in 2019/11/26
/*void cc2_setting(void)
{
	// Staggered mode, FSYS = 80MHz: T7: 50ns/tick, T8: 0.2us/tick
	CC2_KSCCFG     =  0x0003;
	_nop_(); _nop_(); 
	_nop_(); _nop_();
	
	CC2_T78CON     =  0x0401;
	CC2_M5         =  0xB333; // CC20~CC23: CC23 assigned to T8
	CC2_M6         =  0x3300; // CC26~CC27

	// Wheel Speed Input
	CC2_CC20IC     =  0x40|(8<<2)|(0);
	CC2_CC21IC     =  0x40|(8<<2)|(1);
	CC2_CC22IC     =  0x40|(8<<2)|(2);
	CC2_CC23IC     =  0x40|(8<<2)|(3);
	
	CC2_CC26IC     =  0x40|(9<<2)|(0);
	CC2_CC27IC     =  0x40|(9<<2)|(1);
	
	CC2_IOC        =  0x0004; // Non-Staggered Mode

	CC2_T78CON_T7R    = 1;
	CC2_T78CON_T8R    = 1;
}*/
void cc2_setting(void)
{
	volatile unsigned int uwTemp;

  ///  -----------------------------------------------------------------------
  ///  Configuration of  Kernel state configuration Control:
  ///  -----------------------------------------------------------------------
  ///  - the Peripheral is supplied with clock signal for CAPCOM2 unit
  ///  - the CC2 module clock = 80.00 MHz

  CC2_KSCCFG     =  0x0003;      // load CAPCOM2 Kernel state configuration 
                                 // register

  uwTemp         =  CC2_KSCCFG;  // dummy read to avoid pipeline effects

  ///  -----------------------------------------------------------------------
  ///  Configuration of CAPCOM2 Control:
  ///  -----------------------------------------------------------------------
  ///  - staggered mode is disabled

  CC2_IOC        =  0x0004;      // load CAPCOM2 I/O control register

  ///  -----------------------------------------------------------------------
  ///  Configuration of CAPCOM2 Timer 7:
  ///  -----------------------------------------------------------------------
  ///  - timer 7 works in timer mode
  ///  - prescaler factor is 2
  ///  - timer 7 run bit is set

  ///  -----------------------------------------------------------------------
  ///  Configuration of CAPCOM2 Timer 8:
  ///  -----------------------------------------------------------------------
  ///  - timer 8 works in timer mode
  ///  - prescaler factor is 1
  ///  - timer 8 run bit is reset


  CC2_T78CON     =  0x0001;      // load CAPCOM2 timer 7 and timer 8 control 
                                 // register

  CC2_T7         =  0x0001;      // load CAPCOM2 timer 7 register

  CC2_T7REL      =  0x0001;      // load CAPCOM2 timer 7 reload register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Timer Port Pins:
  ///  -----------------------------------------------------------------------


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Timer Interrupts:
  ///  -----------------------------------------------------------------------


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 16:
  ///  -----------------------------------------------------------------------
  ///  - channel 16 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 17:
  ///  -----------------------------------------------------------------------
  ///  - channel 17 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 18:
  ///  -----------------------------------------------------------------------
  ///  - channel 18 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 19:
  ///  -----------------------------------------------------------------------
  ///  - channel 19 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 20:
  ///  -----------------------------------------------------------------------
  ///  - channel 20 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 21:
  ///  -----------------------------------------------------------------------
  ///  - channel 21 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 22:
  ///  -----------------------------------------------------------------------
  ///  - capture on negative transition (falling edge) at pin CC22IO (P2.9)
  ///  - CC22 allocated to CAPCOM2 timer 7
  ///  - single event mode is disabled


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 23:
  ///  -----------------------------------------------------------------------
  ///  - channel 23 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 24:
  ///  -----------------------------------------------------------------------
  ///  - channel 24 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 25:
  ///  -----------------------------------------------------------------------
  ///  - channel 25 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 26:
  ///  -----------------------------------------------------------------------
  ///  - channel 26 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 27:
  ///  -----------------------------------------------------------------------
  ///  - channel 27 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 28:
  ///  -----------------------------------------------------------------------
  ///  - channel 28 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 29:
  ///  -----------------------------------------------------------------------
  ///  - channel 29 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 30:
  ///  -----------------------------------------------------------------------
  ///  - channel 30 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 31:
  ///  -----------------------------------------------------------------------
  ///  - channel 31 is disabled

  CC2_M5         =  0x0200;      // load CAPCOM2 mode register 5

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel Port Pins:
  ///  -----------------------------------------------------------------------
  ///  - P2.9 is used for CAPCOM2 input(CC2_22)


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channels Interrupts:
  ///  -----------------------------------------------------------------------
  ///  CC22 service request node configuration:
  ///  - CC22 interrupt priority level (ILVL) = 13
  ///  - CC22 interrupt group level (GLVL) = 0
  ///  - CC22 group priority extension (GPX) = 0

  CC2_CC22IC     =  0x0074;     
  CC2_T78CON_T7R    = 1;    // set CAPCOM2 timer 7 run bit

}


///////////////////////////////////////////////////////////////////////////////
// CCU6: fcc6=fsys=80MHz 
void cc6_setting(void)
{
	CCU61_KSCFG    =  0x0003;      // Kernel State Configuration Register
	_nop_(); _nop_(); 
	_nop_(); _nop_();
	
	CCU61_PISELH   =  0x0003;
	
	CCU61_T12PR    =  MOT_PWM_PERIOD-1;  // MOT_PWM_PERIOD=8000-1 or 4000-1: 10KHz PWM (Edge/Center-aligned mode)
	CCU61_T13PR    =  MOT_PWM_PERIOD-1;
	
	CCU61_TCTR0    =  0x0080; // Center-aligned Mode
	CCU61_TCTR2    =  0x0020; // T12 is counting up.
	
	CCU61_CC60SR   =  0; // 0% duty
	CCU61_CC61SR   =  0; // 0% duty
	CCU61_CC62SR   =  0; // 0% duty
	
	CCU61_T12DTC   =  0x0750; // Dead-time Control: 0x50: 80ticks@80MHz: 1us
	CCU61_T12MSEL  =  0x0111; // Compare mode
	CCU61_CMPSTAT  =  0x2A00; // Passive state: 
	CCU61_MODCTR   =  0x003F;
	CCU61_PSLR     =  0x0015; // Passive Level: 101010
	
	CCU61_TCTR4    =  0x4242;
	
	// Done in gpio_setting() 
	// P0_IOCR00      =  0x00B0;  // IH1
	// P0_IOCR01      =  0x00B0;  // IH2
	// P0_IOCR02      =  0x00B0;  // IH3
	// P0_IOCR03      =  0x00B0;  // IL1
	// P0_IOCR04      =  0x00B0;  // IL2
	// P0_IOCR05      =  0x00B0;  // IL3
	
	CCU61_PISELL   =  0xFF00;
}

///////////////////////////////////////////////////////////////////////////////
// 
#define DO_CC_PARSE(tm,index,pin_state)                                            \
{                                                                                  \
	u16 tmpu16;                                                                    \
	u32 tmpu32;                                                                    \
	                                                                               \
	g_pwm_ex_timestamp[index]=tm;                                                  \
	if(pin_state==0)                                                               \
	{                                                                              \
		tmpu16=g_pwm_ex_timestamp[index]-g_pwm_ex_timestamp0[index];               \
		if(tmpu16<g_pwm_ex_period_uw[index])                                       \
		{                                                                          \
			tmpu32=tmpu16;                                                         \
			tmpu32*=10000;                                                         \
			if(g_pwm_ex_period_uw[index])                                          \
			{                                                                      \
				tmpu32/=g_pwm_ex_period_uw[index];                                 \
				g_pwm_ex_duty_uw[index]=(u16)tmpu32;                               \
			}                                                                      \
		}                                                                          \
	}                                                                              \
	else                                                                           \
	{                                                                              \
		tmpu16=tm-g_pwm_ex_timestamp0_r[index];                                    \
		if(tmpu16<G_PWM_EX_PERIOD_STD[index]+(G_PWM_EX_PERIOD_STD[index]>>2)       \
			&& tmpu16>G_PWM_EX_PERIOD_STD[index]-(G_PWM_EX_PERIOD_STD[index]>>2))  \
		{                                                                          \
			g_pwm_ex_period_uw[index]=tmpu16;                                      \
		}			                                                               \
		g_pwm_ex_timestamp0_r[index]=tm;                                           \
	}                                                                              \
	g_pwm_ex_timestamp0[index]=g_pwm_ex_timestamp[index];	                       \
}


void isr_cc20(void) interrupt CC2_CC20INT
{
	DO_CC_PARSE(CC2_CC20,0,GET_CC20_PIN());
}

/////////////////////////////////////////////////
void isr_cc21(void) interrupt CC2_CC21INT
{
	DO_CC_PARSE(CC2_CC21,1,GET_CC21_PIN());
}

/////////////////////////////////////////////////
u16 timer7_val = 0;
u32 tmpu32;
extern u16 g_disp_value_sent;

void isr_cc22(void) interrupt CC2_CC22INT
{
	 // USER CODE BEGIN (CC22,2)
	if(P2_IN_P9 == 0)
	{
		timer7_val = CC2_CC22;
		CC2_T7 = 0;
		if(sent_decode(timer7_val,0)==0)
		{
			tmpu32=sent_data[1];
			tmpu32<<=4;
			tmpu32|=sent_data[2];
			tmpu32<<=4;
			tmpu32|=sent_data[3];
			g_disp_value_sent = (u16)tmpu32;
	}
	}

  // USER CODE END
	//DO_CC_PARSE(CC2_CC22,2,GET_CC22_PIN());
}

/////////////////////////////////////////////////
void isr_cc23(void) interrupt CC2_CC23INT
{
	DO_CC_PARSE(CC2_CC23,3,GET_CC23_PIN());
}

/////////////////////////////////////////////////
void isr_cc26(void) interrupt CC2_CC26INT
{
}

/////////////////////////////////////////////////
void isr_cc27(void) interrupt CC2_CC27INT
{
}

///////////////////////////////////////////////////////////////////////////////
