#ifndef _CC2_CCU6_H
#define _CC2_CCU6_H

#include "hw.h"

#define VALUE_100P   0
#define VALUE_0P    (155*4+1)

#define VALUE_SW_ON	 0xffff
#define VALUE_SW_OFF 0x0000

void cc2_setting(void);
void cc6_setting(void);

#endif /* define _CC2_CCU6_H */
