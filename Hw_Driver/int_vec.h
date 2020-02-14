#ifndef _INT_VEC_H
#define _INT_VEC_H

#ifndef WIN32

#define CC2_CC16INT    0x10
#define CC2_CC17INT    0x11
#define CC2_CC18INT    0x12
#define CC2_CC19INT    0x13
#define CC2_CC20INT    0x14
#define CC2_CC21INT    0x15
#define CC2_CC22INT    0x16
#define CC2_CC23INT    0x17
#define CC2_CC24INT    0x18
#define CC2_CC25INT    0x19
#define CC2_CC26INT    0x1A
#define CC2_CC27INT    0x1B
#define U0C0_0INT      0x50
#define U0C0_1INT      0x51
#define RTCINT         0x6E
#define T2INT          0x20
#define T3INT          0x21
#define U1C1_0INT      0x59
#define U1C1_1INT      0x5a

#define CAN_SRN0INT    0x40
#define CAN_SRN1INT    0x41
#define CAN_SRN2INT    0x42
#else
#define interrupt

#define CC2_CC16INT    // 0x10
#define CC2_CC17INT    // 0x11
#define CC2_CC18INT    // 0x12
#define CC2_CC19INT    // 0x13
#define CC2_CC20INT    // 0x14
#define CC2_CC21INT    // 0x15
#define CC2_CC22INT    // 0x16
#define CC2_CC23INT    // 0x17
#define CC2_CC24INT    // 0x18
#define CC2_CC25INT    // 0x19
#define CC2_CC26INT    // 0x1A
#define CC2_CC27INT    // 0x1B
#define U0C0_0INT      // 0x50
#define U0C0_1INT      // 0x51
#define RTCINT         // 0x6E
#define T2INT          // 0x20
#define T3INT          // 0x21
#define U1C1_0INT      // 0x59
#define U1C1_1INT      // 0x5a

#define CAN_SRN0INT    // 0x40
#define CAN_SRN1INT    // 0x41
#define CAN_SRN2INT    // 0x42

#endif


#endif /* define _INT_VEC_H */
