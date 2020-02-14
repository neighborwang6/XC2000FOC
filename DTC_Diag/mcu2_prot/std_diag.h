#ifndef _STD_DIAG_H
#define _STD_DIAG_H

/*********************************************************************/
void timer_isr_hook(void);
void rec_isr_hook(char ch);

/*********************************************************************/
enum {
	YLY_FUNCTION            =0xa5,
	YLY_GET_VERSION         =0x11,
	YLY_SET_BAUDRATE        =0x22,
	YLY_SET_MODE            =0x33,
	YLY_RESET               =0x44
};
/*********************************************************************/
#define THB1 ((u8)0x68)
#define THB2 ((u8)0x6a)
#define THB3 ((u8)0xf1)
#define EHB1 ((u8)0x48)
#define EHB2 ((u8)0x6b)
#define EHB3 ((u8)0x28)

#define ENT_ADDR   ((u8)0x30)
#define ENT_SYNC   ((u8)0x55) 
#define ENT_K1     ((u8)0x08)
#define ENT_K2     ((u8)0x08)

enum {
   CMD_T_LINK_KEEP         = 0xf1,
   CMD_R_DTC_INFO          = 0x03,
   CMD_T_DTC_INFO          = 0x43,
      DTC_C1101              = 0x5101,    // Battery Voltage High
      DTC_C1102              = 0x5102,    // Battery Voltage Low
      DTC_C1200              = 0x5200,    // Speed Sensor Error: LF
      DTC_C1203              = 0x5203,    // Speed Sensor Error: LR
      DTC_C1206              = 0x5206,    // Speed Sensor Error: RF
      DTC_C1209              = 0x5209,    // Speed Sensor Error: RR
      DTC_C1275              = 0x5275,    // G Sensor Error
      DTC_C1604              = 0x5604,    // ECU Hardware Error
      DTC_C1606              = 0x5606,    // ECU Software Error
      DTC_C2101              = 0x6101,    // Motor/Relay Error
      DTC_C2112              = 0x6112,    // Solenoid/Relay Error
      DTC_C2113              = 0x6113,    // HCU Error
      DTC_C1101_IDX              = 0x01,
      DTC_C1102_IDX              = 0x02,
      DTC_C1200_IDX              = 0x03,
      DTC_C1203_IDX              = 0x04,
      DTC_C1206_IDX              = 0x05,
      DTC_C1209_IDX              = 0x06,
      DTC_C1275_IDX              = 0x07,
      DTC_C1604_IDX              = 0x08,
      DTC_C1606_IDX              = 0x09,
      DTC_C2101_IDX              = 0x0a,
      DTC_C2112_IDX              = 0x0b,
      DTC_C2113_IDX              = 0x0c,
   CMD_R_CLEAR_DTC         = 0x14,
   CMD_T_CLEAR_DTC         = 0x54,
   CMD_T_GENERIC_RESPONSE  = 0x7f,
      GR_OK                  = 0x00,
      GR_REJECT              = 0x10,
      GR_NOT_SUPPORT         = 0x11,
      GR_BUSY_REPEAT         = 0x21,
      GR_NOT_COMPLETE        = 0x23,
   CMD_R_DATA_STREAM       = 0xb1,
   CMD_T_DATA_STREAM       = 0xf1,
      DS_LINK_KEEP           = 0x01,
      DS_WHEEL_SPEED         = 0x0e,
      DS_WARNING_LAMP        = 0x1e,
      DS_BRAKE_LAMP          = 0x1f,
      DS_HL_SPEED_SWITCH     = 0x20,
      DS_MOTOR_RELAY         = 0x21,
      DS_SOLENOID_RELAY      = 0x22,
      DS_MOTOR               = 0x23,
      DS_G_SENSOR            = 0x24,
      DS_DRIVE_MODE_SWITCH   = 0x25,
      DS_VALVE               = 0x26,
   CMD_R_COMPONENT_TEST    = 0xbf,
   CMD_T_COMPONENT_TEST    = 0xff,
      CT_MOTOR               = 0x20,
      CT_WARNING_LAMP        = 0x21,
      CT_FL_IN_VALVE         = 0x22,
      CT_FL_OUT_VALVE        = 0x23,
      CT_FR_IN_VALVE         = 0x24,
      CT_FR_OUT_VALVE        = 0x25,
      CT_RL_IN_VALVE         = 0x26,
      CT_RL_OUT_VALVE        = 0x27,
      CT_RR_IN_VALVE         = 0x28,
      CT_RR_OUT_VALVE        = 0x29,
      CT_ALL_VALVE           = 0x30,     // Control 8 Valves
	  CT_ALL_MISC            = 0x31,     // BIT0: MOTOR, BIT1: LAMP
	  CT_EXHAUST             = 0xf0,
   CMD_R_EXHAUST           = 0xbf,       // the same as CMD_R_COMPONENT_TEST
   CMD_T_EXHAUST           = 0xff
};

enum {
   BR_5BPS     = 0,
   BR_10400BPS = 1
};

/*********************************************************************/

#endif /* define _STD_DIAG_H */