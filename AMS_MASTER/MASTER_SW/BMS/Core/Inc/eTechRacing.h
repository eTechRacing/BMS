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
void delay_us (uint16_t us, TIM_HandleTypeDef timer);
int _write(int file, char *ptr, int len);
void spi_transmit_recieve(SPI_HandleTypeDef spi_channel, uint8_t cmd_input, uint8_t *data, uint8_t REG_LEN_TOTAL_IC);

#endif /* INC_ETECHRACING_H_ */
