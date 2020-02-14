////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
*
*  
*      
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

#include "eeprom.h"
#include "hw.h"

////////////////////////////////////////////////////////////////////////////////
//

// Chip: AT25010, etc.
// #define ADDR_LEN 8
// Chip: M95640, etc.
#define ADDR_LEN 16

extern u8 eeprom_wr_hook(u16 addr,u8 len,u8 *buf);
extern u16 spi_eeprom_trans(u16 ch);

#ifdef EE_HW_SPI
#define EE_HW_SPI_L
#endif

#define CMD_WREN   0x06
#define CMD_WRDI   0x04
#define CMD_RDSR   0x05
#define CMD_WRSR   0x01
#define CMD_READ   0x03
#define CMD_WRITE  0x02

void cmd_wren(void);
void cmd_wrdi(void);
u16 byte_shift(u16 dat,u8 bit_num);
u8 cmd_rdsr(void);
void cmd_wrsr(u8 ch);

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 eeprom_init(void)
{
	u8 buf[2];
	
	// call gpio_setting first to init the corresponding PINs
	
	SET_EESS(1); delay_us(1);
	SET_EESCK(0);delay_us(1);
	SET_EEWP(0); delay_us(1);
	SET_EESDI(1);delay_us(1);
	
	SET_EEWP(1);
	cmd_wren();
	if(!(cmd_rdsr()&0x02))
	{
		SET_EEWP(0);
		return 1;
	}
	cmd_wrdi();
	if(cmd_rdsr()&0x02)
	{
		SET_EEWP(0);
		return 2;
	}
	eeprom_rd(0,2,buf);
	if(buf[0]!=0x55 || buf[1]!=0xaa)
	{
		buf[0]=0x55;buf[1]=0xaa;
		if(eeprom_wr(0,2,buf))
			return 3;
		eeprom_rd(0,2,buf);
		if(buf[0]!=0x55 || buf[1]!=0xaa)
		{
			return 4;
		}
	}
	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u16 byte_shift(u16 dat,u8 bit_num)
{
#ifndef EE_HW_SPI_L
	
	u8 idx;
	u16 tmp,tmp1;
	
	if(bit_num!=8 && bit_num!=16)
		return 0;
	
	tmp=dat;
	tmp1=0;
	if(bit_num==8)
		tmp<<=8;
		
	
	for(idx=0;idx<bit_num;idx++)                  
	{                                       
		SET_EESCK(0);                 //   .....  (bit begin to shift)
		SET_EESDI((tmp&0x8000)?1:0);  //   |
		tmp<<=1;                      //   |
		delay_us(1);                  //   |
		SET_EESCK(1);                 //   |___   (bit ready and latch)
		tmp1<<=1;                     //       |
		tmp1|=GET_EESDO();            //       | 
		delay_us(1);                  //   ....|
	}
	
	return tmp1;
	
#else
	
	return spi_eeprom_trans(ch);
	
#endif
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void cmd_wren(void)
{
	SET_EESS(0); delay_us(1);
	byte_shift(CMD_WREN,8);
	SET_EESCK(0);delay_us(1);
	SET_EESS(1); delay_us(1);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void cmd_wrdi(void)
{
	SET_EESS(0); delay_us(1);
	byte_shift(CMD_WRDI,8);
	SET_EESCK(0);delay_us(1);
	SET_EESS(1); delay_us(1);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 cmd_rdsr(void)
{
	u8 tmp;
	SET_EESS(0); delay_us(1);
	byte_shift(CMD_RDSR,8);
	tmp=(u8)byte_shift(0,8);
	SET_EESCK(0);delay_us(1);
	SET_EESS(1); delay_us(1);
	
	return tmp;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void cmd_wrsr(u8 ch)
{
	SET_EESS(0); delay_us(1);
	byte_shift(CMD_WRSR,8);
	byte_shift(ch,8);
	SET_EESCK(0);delay_us(1);
	SET_EESS(1); delay_us(1);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 eeprom_wr(u16 addr,u8 len,u8 *buf)
{
	u8 idx;
	
	if(len>8) // 8 bytes per page
		return 1;
	
	SET_EEWP(1); delay_us(1);
	cmd_wren();
	
	SET_EESS(0); delay_us(1);
	byte_shift(CMD_WRITE,8);
	byte_shift(addr,16);
	for(idx=0;idx<len;idx++)
		byte_shift(buf[idx],8);
	SET_EESCK(0);delay_us(1);
	SET_EESS(1); delay_us(1);
	
	idx=0;
	while(cmd_rdsr()&1)
	{
		delay_msx(1);
		idx++;
		if(idx>=100)
			break;
	}
	
	cmd_wrdi();
	SET_EEWP(0); delay_us(1);
	
	eeprom_wr_hook(addr,len,buf);
	
	return (idx>=100)?1:0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
u8 eeprom_rd(u16 addr,u8 len,u8 *buf)
{
	u8 idx;
	SET_EEWP(1); delay_us(1);
	SET_EESS(0); delay_us(1);
	byte_shift(CMD_READ,8);
	byte_shift(addr,16);
	for(idx=0;idx<len;idx++)
		buf[idx]=(u8)byte_shift(0,8);
	SET_EESCK(0);delay_us(1);
	SET_EESS(1); delay_us(1);
	SET_EEWP(0); delay_us(1);
	
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
