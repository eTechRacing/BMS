#ifndef _CAN2_
#define	_CAN2_

#include <xc.h>
#include <stdint.h>
#include "main.h"

void __attribute__ ( (interrupt, no_auto_psv) ) _C2Interrupt( void );

#endif

