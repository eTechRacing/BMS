#ifndef _CALCULATIONS_
#define _CALCULATIONS_

#include <xc.h>
#include <stdint.h>
#include "constants.h"

int max_cell_voltage(int voltage[]);
int min_cell_voltage(int voltage[]);
long total_voltage(int voltage[]);
int SoC_from_OCV(int OCV);
int max_cell_temperature(int temperature[120]);
int min_cell_temperature(int temperature[120]);
int avg_cell_temperature(int temperature[120]);
int max_onboard_temperature(int temperature[120]);
int min_onboard_temperature(int temperature[120]);
int avg_onboard_temperature(int temperature[120]);


#endif