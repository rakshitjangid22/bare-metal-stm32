#ifndef INC_SYSTEM_H
#define INC_SYSTEM_H

#include "common-defines.h"

#define TICK_FREQ   (1000)
#define AHB_FREQ    (84000000)

void system_setup(void);
uint64_t system_get_ticks(void);

#endif  // INC_SYSTEM_H