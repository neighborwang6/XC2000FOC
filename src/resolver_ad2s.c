////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
*
*  
*      
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

#include "resolver_ad2s.h"

////////////////////////////////////////////////////////////////////////////////
// Current rotator position
u16 g_resolver_pos_uw=0;
// Previous rotator position
u16 g_resolver_pos0_uw=0;
// Rotator angle sum (Electric Angle)
s32 g_total_angle_sl=0;
s32 g_total_angle0_sl=0;

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 resolver_ad2s_init(u8 option)
{
	UNUSED_PARA(option);
	
	resolver_ad2s_wr_cfg(0x91,40);
	resolver_ad2s_rd_cfg(0x91);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void resolver_ad2s_wr_cfg(u8 addr, u8 dat)
{
	SET_AD2S_A0A1(1);   delay_ns(1);
	SET_AD2S_CS(0);     delay_ns(1);
	SET_AD2S_WR(0);     delay_ns(1);
	SET_AD2S_DPORT_DIR(DIR_OUT);
	SET_AD2S_DATA(addr); delay_ns(1);
	SET_AD2S_WR(1);     delay_ns(1);
	SET_AD2S_WR(0);     delay_ns(1);
	SET_AD2S_DATA(dat); delay_ns(1);
	SET_AD2S_WR(1);     delay_ns(1);
	SET_AD2S_CS(1);     delay_ns(1);
	
	SET_AD2S_DPORT_DIR(DIR_IN); 
	delay_ns(1);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 resolver_ad2s_rd_cfg(u8 addr)
{
	u16 res=0;
	
	SET_AD2S_A0A1(1);   delay_ns(1);
	SET_AD2S_CS(0);     delay_ns(1);
	SET_AD2S_WR(0);     delay_ns(1);
	SET_AD2S_DPORT_DIR(DIR_OUT);
	SET_AD2S_DATA(addr); delay_ns(1);
	SET_AD2S_WR(1);      delay_ns(1);
	SET_AD2S_DPORT_DIR(DIR_IN);
	SET_AD2S_RD(0);     delay_ns(1);
	res=GET_AD2S_DATA();
	SET_AD2S_RD(1);     delay_ns(1);
	SET_AD2S_CS(1);     delay_ns(1);
	
	return (u8)res;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u32 resolver_ad2s_read(u8 option)
{
	u8  val;
	u32 res=0;

	UNUSED_PARA(option);
	
	SET_AD2S_SAMPLE(1);
	SET_AD2S_SAMPLE(0); delay_ns(1);
	SET_AD2S_SAMPLE(1); delay_ns(1);
	
	SET_AD2S_A0A1(0);   delay_ns(1);
	SET_AD2S_CS(0);     delay_ns(1);

	SET_AD2S_RD(0);     delay_ns(1);
	val=(u8)GET_AD2S_DATA();
	res=val;
	SET_AD2S_RD(1);     delay_ns(1);

	SET_AD2S_RD(0);     delay_ns(1);
	res=GET_AD2S_DATA();
	res<<=8; res|=val;
	SET_AD2S_RD(1);     delay_ns(1);

	SET_AD2S_RD(0);     delay_ns(1);
	res=GET_AD2S_DATA();
	res<<=8; res|=val;
	SET_AD2S_RD(1);     delay_ns(1);

	SET_AD2S_CS(1);     delay_ns(1);
	
	return res;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 resolver_ad2s_read_pos(u8 option)
{
	u16 res=0;
	
	UNUSED_PARA(option);

	// Sample Data
	SET_AD2S_SAMPLE(1);
	SET_AD2S_SAMPLE(0);  delay_ns(1);
	SET_AD2S_SAMPLE(1);  delay_ns(1);

	// Chip select
	SET_AD2S_A0A1(1);    delay_ns(1);
	SET_AD2S_CS(0);      delay_ns(1);
	
	// Read D15 to D8 of Position
	SET_AD2S_WR(0);      delay_ns(1);
	SET_AD2S_DPORT_DIR(DIR_OUT);
	SET_AD2S_DATA(0x80); delay_ns(1);
	SET_AD2S_WR(1);      delay_ns(1);
	SET_AD2S_DPORT_DIR(DIR_IN);
	SET_AD2S_RD(0);      delay_ns(1);
	res=GET_AD2S_DATA();
	SET_AD2S_RD(1);      delay_ns(1);

	// Read D7 to D0 of Position
	SET_AD2S_WR(0);      delay_ns(1);
	SET_AD2S_DPORT_DIR(DIR_OUT);
	SET_AD2S_DATA(0x81); delay_ns(1);
	SET_AD2S_WR(1);      delay_ns(1);
	SET_AD2S_DPORT_DIR(DIR_IN);
	SET_AD2S_RD(0);      delay_ns(1);
	res<<=8;
	res|=GET_AD2S_DATA();
	SET_AD2S_RD(1);      delay_ns(1);

	// Chip deselect
	SET_AD2S_CS(1);      delay_ns(1);
	
	// Encoder Resolution: 16bit (default),absolute resolution: 12bit 
	res>>=4;
	return res;
	
}
////////////////////////////////////////////////////////////////////////////////
