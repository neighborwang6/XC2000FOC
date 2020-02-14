
/**********************************************************************************
    Author: DAE, Tsinghua Univ.
    Date: 2016.12.30
    Version:
        1.0
**********************************************************************************/
#include "Filters_EbsSensors.h"

u32 g_sum_pres_sense=0;
u16 g_pres_sense_val=0;  //1V == 5Mpa

u32 g_sum_disp_sense=0;
u32 g_disp_sense_val=0;

//filt the press
u16 filter_pres_sense(u16 pres_sense_get)
{
	u32 sum_temp;
	
	g_sum_pres_sense+=pres_sense_get;
	g_sum_pres_sense-=g_pres_sense_val;
	sum_temp=g_sum_pres_sense;
	sum_temp=(sum_temp*160)>>8;
	g_pres_sense_val=(u16)(sum_temp);
	
	return g_pres_sense_val;
}

//filt the disp
s32 filter_disp_sense(s32 disp_sense_get)
{
	s32 sum_temp;

	g_sum_disp_sense+=disp_sense_get;
	g_sum_disp_sense-=g_disp_sense_val;
	sum_temp=g_sum_disp_sense;
	sum_temp=(sum_temp*160)>>8;
	g_disp_sense_val=sum_temp;

	return g_disp_sense_val;
}


////////////////////////////////////////////////////////////////////////////////
