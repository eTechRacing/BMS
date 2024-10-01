/*
 * eTechRacing.h
 *
 *  Created on: Feb 15, 2024
 *      Author: Bernat
 */

#ifndef INC_ETECHRACING_H_
#define INC_ETECHRACING_H_
//Includes
#include "main.h"

//Functions
void delay_us (long int us);
uint16_t Min_Volt(uint16_t *voltatjes);
uint16_t Max_Volt(uint16_t *voltatjes);
float Mean_Volt(uint16_t *voltatjes);
uint32_t Sum_Volt(uint16_t *voltatjes);
uint16_t Min_Temp(uint16_t *temperatures);
uint16_t Max_Temp(uint16_t *temperatures);
float Mean_Temp(uint16_t *temperatures);
void Conv_Temp(uint16_t *temperatures, uint16_t *temperatures_conv);
int32_t Curr_EVAL(uint8_t *current_arr);

#endif /* INC_ETECHRACING_H_ */
