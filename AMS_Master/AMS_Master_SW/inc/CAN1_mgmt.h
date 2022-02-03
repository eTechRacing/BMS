#ifndef _CAN1_
#define	_CAN1_

#include <xc.h>
#include <stdint.h>
#include "main.h"

void external_CAN_management ( void );
void __attribute__ ( (interrupt, no_auto_psv) ) _C1Interrupt( void );

#endif

