/*
 * LTC6811-1_eTechRacing.h
 *
 *  Created on: Jan 29, 2024
 *      Author: Bernat
 */

#ifndef SRC_LTC6811_1_ETECHRACING_H_
#define SRC_LTC6811_1_ETECHRACING_H_


#include "main.h"

#define CSPORT GPIOB
#define CSPIN GPIO_PIN_6
#define VCELLMAX 4000
#define VCELLMIN 3000
#define REFON 0x1 							//Reference remains powered up until watchdog timeout
#define DTEN 0x1 							//Enable the discharge timer for discharge switches
#define ADCOPT 0x1							//27k,7k,422 and 26 ADC frequencies
#define DCCx_INIT 0x000 					//Initially, deactivate DCC
#define DCTO 0xC							//Discharge Time Out Value to 60 min (0xC) --> SI DTEN = 1, DCTO serà el temps que tardarà el xip en adormir-se
											// Here avoided divions like 0.1 that is floating point for the compiler use directly integer (is faster though)
#define OVERVOLTAGE_TH (uint16_t)(((VCELLMAX * 10) / (16)) / 10)
#define UNDERVOLTAGE_TH (uint16_t)((((VCELLMIN * 10) / (16)) / 10) - 1)

#define ICn 1	  							// The number of ICs being written
#define CELL_CHANNELS 12					// The number of cells read by each slave
#define AUX_CHANNELS 6						// Number of GPIOS read back
#define MD_1  0b10							// ADC Conversion Mode
#define DCP_1  0							// Controls if Discharge is permitted during conversion: 0 = Not Permitted; 1 = Permitted
#define CH_1  0x00							// Sets which Cell channels are converted



void outputCS(uint8_t input);
void wakeup_idle(SPI_HandleTypeDef spi_channel);
uint16_t pec15_calc(uint8_t len, uint8_t *data);
void wakeup_sleep(SPI_HandleTypeDef spi_channel,TIM_HandleTypeDef timer, int total_ic, int WAKEUP_DELAY);
int **set_cfgr(int total_ic, int n);
void ltc6811_wrcfg(uint8_t total_ic, uint8_t **config[ICn][6],SPI_HandleTypeDef spi_channel);
void ltc6811_adcv(uint8_t MD, uint8_t DCP, uint8_t CH,SPI_HandleTypeDef spi_channel);
uint16_t ltc6811_rdcv(uint8_t reg,uint8_t total_ic,uint16_t cell_codes[][CELL_CHANNELS],SPI_HandleTypeDef spi_channel);
void ltc6811_rdcv_reg(uint8_t reg, uint8_t total_ic, uint8_t *data,SPI_HandleTypeDef spi_channel);
void spi_transmit_recieve(SPI_HandleTypeDef spi_channel, uint8_t cmd_input, uint8_t *data, uint8_t REG_LEN_TOTAL_IC);
void ltc6811_adax(uint8_t MD,uint8_t CHG,SPI_HandleTypeDef spi_channel);
int8_t ltc6811_rdaux(uint8_t reg,uint8_t total_ic,uint16_t aux_codes[][AUX_CHANNELS], SPI_HandleTypeDef spi_channel);
void ltc6811_rdaux_reg(uint8_t reg, uint8_t total_ic, uint8_t *data, SPI_HandleTypeDef spi_channel);

#endif /* SRC_LTC6811_1_ETECHRACING_H_ */
