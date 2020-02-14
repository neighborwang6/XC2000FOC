#ifndef _EEPROM_H
#define _EEPROM_H

#include "std_defs.h"

u8 eeprom_init(void);
u8 eeprom_wr(u16 addr,u8 len,u8 *buf);
u8 eeprom_rd(u16 addr,u8 len,u8 *buf);

extern void delay_ms(u16 tm);
extern void delay_us(u16 tm);

#endif /* define _EEPROM_H */


