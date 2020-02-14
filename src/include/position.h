////////////////////////////////////////////////////////////////////////////////
// this file define the struct about the eQEP,to calculate the rotor positon and PMSM speed.
#ifndef POSITION_H_
#define POSITION_H_

#include "dmctype.h"

//
// Included Files
//
#include "IQmathLib.h"

////////////////////////////////////////////////////////////////////////////////
// Defines for the default initializer for the POSSPEED Object.
//

#define POSSPEED_DEFAULTS {0,0,0,0,0,3,1024,0,0,0,0,0,0,\
        (void (*)(long))EQep1_Init,\
        (void (*)(long))Posspeed_Calc }
# define    UTMR_PRD          150000                         // Unit Timer for 10KHz at 150 MHz SYSCLKOUT ,speed calculate every 1ms,150000/150000000=1ms
# define    _UPPS              5                            // CAP TIMER,// 1/32 for unit position  , UPEVNT occurs when every 2^k QCLKs  32
# define    _CCPS              7                            // 1/128 for CAP clock   QCTMR prescale   1/(150MHz/128)
# define    Mech_scaler       _IQ(0.0002441406)             // 1/4096
# define    Speed_M_scaler    _IQ16(3051.757813)            // 2^UPPS/(4096*3)/(2^CCPS/150000000)
//
#define     CircleDiv6        60  //682                            //   4096/6
// Typedef for the structure of the POSSPEED Object

#define MOT_ANGLE_DEFAULTS {0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0}

////////////////////////////////////////////////////////////////////////////////
//
typedef struct
{
    _iq theta_elec;     	// Output: Motor Electrical angle (Q24)
    _iq theta_mech;     	// Output: Motor Mechanical Angle (Q24)
    int Direction;      	// Output: Motor rotation direction (Q0)
    int QEP_cnt_idx; 	 	// Variable: Encoder counter index (Q0)
    int theta_raw;     		// Variable: Raw angle from Timer 2 (Q0)

    int pole_pairs;     	// Parameter: Number of pole pairs (Q0)

    int offset_angle;       // Parameter: Raw angular offset between encoder and phase a (Q0)
    int index_sync_flag; 	// Output: Index sync status (Q0)

    _iq   oldpos;  			// Q24 Input: Electrical angle (pu)
    _iq   newp;             // Q24
    _iq   delta_pos;        // Q24  Output :  when calculate speed by T method(high speed), the decrease/increase position between former pos. and latter pos.

    int32 SpeedRpm_T;       // Q16 T method to calculate speed
    int32 SpeedRpm_M;       // Q16 M method to calculate speed
    void (*init)();     	// Pointer to the init function
    void (*calc)();    		// Pointer to the calc function
}POSSPEED;

////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    float angle_sense;
    float angle_senseo;
	float angle_elec;     	// Output: Motor Electrical angle (Q24)
    float angle_mech;     	// Output: Motor Mechanical Angle (Q24)
    float angle_eleco;  			// Q24
    float angle_mecho;             // Q24

    _iq angle_eiq;
    _iq angle_miq;

    int direction;      	// Output: Motor rotation direction (Q0)
//    float angle_raw;     		// Variable: Raw angle (Q0)
    _iq turns_cnt;

    int pole_pairs;     	// Parameter: Number of pole pairs (Q0)

    int offset_angle;       // Parameter: Raw angular offset between encoder and phase a (Q0)

	_iq adc_g;
	_iq adc_y;
	_iq elec_sine;
	_iq elec_cosine;


    float delta_pos;        // Q24  Output :  when calculate speed by T method(high speed), the decrease/increase position between former pos. and latter pos.
    _iq delta_rawpos;
    float delta_possum;

    _iq SpeedRpm_T;       // Q16 T method to calculate speed
    _iq SpeedRpm_M;       // Q16 M method to calculate speed
//    void (*init)();     	// Pointer to the init function
//    void (*calc)();    		// Pointer to the calc function
}MOT_ANGLE;

////////////////////////////////////////////////////////////////////////////////
// Typedef for the POSSPEED_handle
//
typedef POSSPEED *POSSPEED_handle;
typedef MOT_ANGLE *MOT_ANGLE_handle;
// Function Prototypes
//
void EQep1_Init(void);
void Posspeed_Calc(POSSPEED_handle);

void Motangle_Calc(MOT_ANGLE_handle);
void Turnangle_Calc(MOT_ANGLE_handle);
void RotationSpeedCalc(MOT_ANGLE *p);

extern MOT_ANGLE motorangle;

#endif /* POSITION_H_ */
