/*
 * eTechRacing.c
 *
 *  Created on: Feb 15, 2024
 *      Author: Bernat
 */
#include "main.h"
#include "stdlib.h"

void delay_us (uint16_t us, TIM_HandleTypeDef timer)
{
	__HAL_TIM_SET_COUNTER(&timer,0);  					// set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&timer) < us);  		// wait for the counter to reach the us input in the parameter
}
