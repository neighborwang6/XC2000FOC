#include "std_defs.h"
#include "xc2365a.h"
#include <intrins.h>
#include <absacc.h>

/////////////////////////////////////////////////
void spi_setting(void);
/////////////////////////////////////////////////
void spi_tle_setting(void);
void spi_tle_putc(u16 ch);
u16 spi_tle_getc(void);
u16 spi_tle_trans(u16 ch);

/////////////////////////////////////////////////
void spi_eeprom_setting(void);
void spi_eeprom_putc(u16 ch);
u16 spi_eeprom_getc(void);
u16 spi_eeprom_trans(u16 ch);
/////////////////////////////////////////////////
void spi_mcu2_setting(void);
void spi_mcu2_putc(u16 ch);
u16 spi_mcu2_getc(void);
u16 spi_mcu2_trans(u16 ch);
