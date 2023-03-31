#ifndef _STATE_
#define _STATE_

#include <xc.h>
#include <stdint.h>


int fans_control(int fans_temperature, int max_cell_temperature, int Fans1_Control, int Fans2_Control);
int overvoltage_check(int max_cell_voltage, int max_OK_voltage);
int pwm_fans_control(int current_limitation, int max_cell_temperature, int Fans1_Control, int Fans2_Control, int Fans3_Control, int Fans4_Control, int Fans5_Control);

void state_and_control(void);

#endif