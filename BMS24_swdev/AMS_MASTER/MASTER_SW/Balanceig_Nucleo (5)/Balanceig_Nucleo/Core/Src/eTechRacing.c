/*
 * eTechRacing.c
 *
 *  Created on: Feb 15, 2024
 *      Author: Bernat
 */
//Includes
#include "main.h"
#include "stdlib.h"
#include "eTechRacing.h"
#include "LTC6811-1_eTechRacing.h"
//Functions

/* Function: delay_us
 * Purpose: Generate a delay in microseconds
 * Inputs: The amount of microseconds of the loop, the timer which has to be used
 */
void delay_us (uint16_t us, TIM_HandleTypeDef timer)
{
	__HAL_TIM_SET_COUNTER(&timer,0);  					// Set the counter of the timer to 0
	while (__HAL_TIM_GET_COUNTER(&timer) < us);  		// Wait for the counter to reach the us input in the parameter
}

int _write(int file, char *ptr, int len)
{
  int i=0;
  for(i=0 ; i<len ; i++)
    ITM_SendChar((*ptr++));
  return len;
}

void spi_transmit_recieve(SPI_HandleTypeDef spi_channel, uint8_t cmd_input, uint8_t *data, uint8_t REG_LEN_TOTAL_IC)
{
	uint8_t MSG = 0XFF;
	HAL_SPI_Transmit(&spi_channel, cmd_input, 4, HAL_MAX_DELAY);
	for(int i = 0; i < REG_LEN_TOTAL_IC; i++)
	{
		HAL_SPI_TransmitReceive(&spi_channel, MSG, data[i], 1, HAL_MAX_DELAY);
	}
}
