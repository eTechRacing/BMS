/*
 * LTC6811-1_eTechRacing.h
 *
 *  Created on: Jan 29, 2024
 *      Author: Bernat
 */

#ifndef SRC_LTC6811_1_ETECHRACING_H_
#define SRC_LTC6811_1_ETECHRACING_H_

#include "main.h"

//GENERAL
extern uint8_t TOTAL_IC;							//Number of slaves

//BALANCING CONDITIONS-----------------------------------------------------------------------------------------
#define V_MIN 35000							// Constant of the minimum voltage for start balancing, first condition for balancing. Two conditions at least should be happening for balancing (Balancing -> Stop chargin
#define V_BALANCING 37000					// Constant of the limit voltage to start balancing, it has no other conditions for balancing
#define DIFF_MAX 200						// Constant of the Maximum difference between the evaluated voltage and the minimum voltage)

//LTC6811 WRCFG-------------------------------------------------------------------------------------------------
#define REFON 0x1 							//Reference remains powered up until watchdog timeout
#define DTEN 0x0 							//Enable the discharge timer for discharge switches
#define ADCOPT 0x0							//27k,7k,422 and 26 ADC frequencies
#define DCTO 0x0							//Discharge Time Out Value to 60 min (0xC) --> SI DTEN = 1, DCTO serà el temps que tardarà el xip en adormir-se

//ERROR CONDITIONS----------------------------------------------------------------------------------------------
#define VCELLMAX 42000
#define VCELLMIN 28000
#define MAXCURRENT 200000 //200 ampers
#define HIGHCURRENT 135000 //135 ampers
#define OVERVOLTAGE_TH (uint16_t)(((VCELLMAX * 10) / (16)) / 10)
#define UNDERVOLTAGE_TH (uint16_t)((((VCELLMIN * 10) / (16)) / 10) - 1)
#define ERROR_VOLT_TIME 500
#define ERROR_TEMP_TIME 1000
#define ERROR_CURRENT_TIME 500
#define SLAVE_DISC_TIME 500
#define ETAS_SYNC_MAX_TIME 200
#define CURR_SYNC_MAX_TIME 500
#define CAN_BALANCING_SYNC_TIME 500
#define MAX_TEMP 600

//LTC6811 ADAX---------------------------------------------------------------------------------------
#define CHG  001							// Which GPIOS are converted: 000 -> GPIO1-5; 001 -> GPIO1; 010 -> GPIO2; 011 -> GPIO3; 100 -> GPIO4; 101 -> GPIO5

//LTC6811 RDCV_REG----------------------------------------------------------------------------------------
#define CELL_CHANNELS 12					// The number of cells read by each slave

//LTC6811 RDAUX_REG--------------------------------------------------------------------------------------
#define AUX_CHANNELS 6						// Number of GPIOS read back

//LTC6811 ADCV-----------------------------------------------------------------------------------------
#define MD  0b00000000						// ADC Conversion Mode
#define DCP  0								// Controls if Discharge is permitted during conversion: 0 = Not Permitted; 1 = Permitted

//FUNCTIONS
void Balancing(uint16_t *VOLTAGES, uint16_t VCELL_MIN);
uint16_t MUX_SWITCH(uint16_t GPIOx);
void outputCS(uint8_t CS_INPUT);
void wakeup_idle();
void wakeup_sleep();
uint16_t pec15_calc(uint8_t DATA_LENGTH, uint8_t *DATA);
void ltc6811_wrcfg(SPI_HandleTypeDef spi_channel, uint8_t GPIOx, uint16_t *VOLTAGES, uint8_t BLC, uint16_t VCELL_MIN);
void ltc6811_adcv(SPI_HandleTypeDef spi_channel, uint8_t CH);
void ltc6811_rdcv_reg(SPI_HandleTypeDef spi_channel, uint8_t reg, uint8_t *data);
void ltc6811_rav(SPI_HandleTypeDef spi_channel, uint8_t *PEC_SLAVES, uint16_t *voltatjes, uint8_t GPIOx, uint8_t CH, uint8_t BLC, uint16_t VCELL_MIN);
void ltc6811_adax(SPI_HandleTypeDef spi_channel);
void ltc6811_rdaux_reg(SPI_HandleTypeDef spi_channel, uint8_t reg, uint8_t *data);
void ltc6811_rat(SPI_HandleTypeDef spi_channel, uint8_t *PEC_SLAVES, uint16_t *voltatjes, uint16_t *temperatures, uint8_t BLC, uint16_t VCELL_MIN);

#endif /* SRC_LTC6811_1_ETECHRACING_H_ */
