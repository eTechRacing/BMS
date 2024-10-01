/*
 * LTC6811-1_eTechRacing1.c
 *
 *  Created on: Jan 26, 2024
 *      Author: Bernat
 */
#include "LTC6811-1_eTechRacing.h"
#include "eTechRacing.h"
#include "BMS.h"
#include "main.h"
#include "stdlib.h"
#include <pgmspace.h>
#include "Linduino.h"

//uint16_t DCC[TOTAL_IC*12];									// Declaration of the DCC array with a TOTAL_IC length
uint8_t TOTAL_IC = 3;
uint16_t V_BALANCING = 36000;
uint16_t V_MIN = 35000;
uint16_t DCC[180];
uint16_t EVAL[180];
uint16_t crc15Table[] = {0x0,  0xc599, 0xceab, 0xb32, 0xd8cf, 0x1d56, 0x1664, 0xd3fd, 0xf407, 0x319e, 0x3aac,  //!<precomputed CRC15 Table
                               0xff35, 0x2cc8, 0xe951, 0xe263, 0x27fa, 0xad97, 0x680e, 0x633c, 0xa6a5, 0x7558, 0xb0c1,
                               0xbbf3, 0x7e6a, 0x5990, 0x9c09, 0x973b, 0x52a2, 0x815f, 0x44c6, 0x4ff4, 0x8a6d, 0x5b2e,
                               0x9eb7, 0x9585, 0x501c, 0x83e1, 0x4678, 0x4d4a, 0x88d3, 0xaf29, 0x6ab0, 0x6182, 0xa41b,
                               0x77e6, 0xb27f, 0xb94d, 0x7cd4, 0xf6b9, 0x3320, 0x3812, 0xfd8b, 0x2e76, 0xebef, 0xe0dd,
                               0x2544, 0x2be,  0xc727, 0xcc15, 0x98c, 0xda71, 0x1fe8, 0x14da, 0xd143, 0xf3c5, 0x365c,
                               0x3d6e, 0xf8f7, 0x2b0a, 0xee93, 0xe5a1, 0x2038, 0x7c2, 0xc25b, 0xc969, 0xcf0, 0xdf0d,
                               0x1a94, 0x11a6, 0xd43f, 0x5e52, 0x9bcb, 0x90f9, 0x5560, 0x869d, 0x4304, 0x4836, 0x8daf,
                               0xaa55, 0x6fcc, 0x64fe, 0xa167, 0x729a, 0xb703, 0xbc31, 0x79a8, 0xa8eb, 0x6d72, 0x6640,
                               0xa3d9, 0x7024, 0xb5bd, 0xbe8f, 0x7b16, 0x5cec, 0x9975, 0x9247, 0x57de, 0x8423, 0x41ba,
                               0x4a88, 0x8f11, 0x57c, 0xc0e5, 0xcbd7, 0xe4e, 0xddb3, 0x182a, 0x1318, 0xd681, 0xf17b,
                               0x34e2, 0x3fd0, 0xfa49, 0x29b4, 0xec2d, 0xe71f, 0x2286, 0xa213, 0x678a, 0x6cb8, 0xa921,
                               0x7adc, 0xbf45, 0xb477, 0x71ee, 0x5614, 0x938d, 0x98bf, 0x5d26, 0x8edb, 0x4b42, 0x4070,
                               0x85e9, 0xf84,  0xca1d, 0xc12f, 0x4b6, 0xd74b, 0x12d2, 0x19e0, 0xdc79, 0xfb83, 0x3e1a, 0x3528,
                               0xf0b1, 0x234c, 0xe6d5, 0xede7, 0x287e, 0xf93d, 0x3ca4, 0x3796, 0xf20f, 0x21f2, 0xe46b, 0xef59,
                               0x2ac0, 0xd3a,  0xc8a3, 0xc391, 0x608, 0xd5f5, 0x106c, 0x1b5e, 0xdec7, 0x54aa, 0x9133, 0x9a01,
                               0x5f98, 0x8c65, 0x49fc, 0x42ce, 0x8757, 0xa0ad, 0x6534, 0x6e06, 0xab9f, 0x7862, 0xbdfb, 0xb6c9,
                               0x7350, 0x51d6, 0x944f, 0x9f7d, 0x5ae4, 0x8919, 0x4c80, 0x47b2, 0x822b, 0xa5d1, 0x6048, 0x6b7a,
                               0xaee3, 0x7d1e, 0xb887, 0xb3b5, 0x762c, 0xfc41, 0x39d8, 0x32ea, 0xf773, 0x248e, 0xe117, 0xea25,
                               0x2fbc, 0x846,  0xcddf, 0xc6ed, 0x374, 0xd089, 0x1510, 0x1e22, 0xdbbb, 0xaf8, 0xcf61, 0xc453,
                               0x1ca,  0xd237, 0x17ae, 0x1c9c, 0xd905, 0xfeff, 0x3b66, 0x3054, 0xf5cd, 0x2630, 0xe3a9, 0xe89b,
                               0x2d02, 0xa76f, 0x62f6, 0x69c4, 0xac5d, 0x7fa0, 0xba39, 0xb10b, 0x7492, 0x5368, 0x96f1, 0x9dc3,
                               0x585a, 0x8ba7, 0x4e3e, 0x450c, 0x8095};


/* Function: Balancing
 * Purpose: Balancing is a function that evaluates the conditions for the cells balancing and returns the order to the IC.
 * The algorithm works the following way: it evaluates individually each cell and if its voltage is over a limit (V_MIN) and the voltage is over an upper limit (V_BALANCING)
 * or the difference between the cell voltage is bigger than a difference (DIFF_MAX) with the minimum cell voltage (VCELL_MIN).
 * Set the Discharge of the cells: 1 -> Turn ON Shorting Switch for Cell x; 0 -> Turn OFF Shorting Switch for Cell x (Default);
 * It is an array of 12 bits for each slave
 * Inputs: Minimum voltage for start balancing, less charged cell, cell voltages array, limit voltage to balance, maximum difference between the value and the minimum voltage, and the number of ICs
*/
void Balancing(uint16_t *VOLTAGES, uint16_t VCELL_MIN)		// Array of the read voltages
		{
			//uint16_t VCELL_MIN = 27000;					// Variable of the less charged cell
			for(int j = 0; j < TOTAL_IC; j++)				// Loops for each voltage in the evaluated IC
			{
				uint16_t BALANCING1 = 0b00000000;			// Variable 0 or 1 that sets if the evaluated cell should (1) or not (0) start balancing, has a 16 bits dimension
				uint16_t BALANCING2 = 0b00000000;			// Variable 0 or 1 that sets if the evaluated cell should (1) or not (0) start balancing, has a 16 bits dimension

				for(int i = 0; i < 12; i++)					// A for function that evaluates each cell of the IC
				{
				    if(i < 8){
				        if(VOLTAGES[i+j*12]>V_MIN && (VOLTAGES[i+j*12] > V_BALANCING || (VOLTAGES[i+j*12]-VCELL_MIN)>DIFF_MAX))	//If the evaluated voltage is over V_MIN && (the evaluated voltage is over V_BALANCING || the difference between the evaluated voltage and VCELL_MIN is bigger than DIFF_MAX
					    {
						BALANCING1 = (BALANCING1|(1<<i));	//Set a 1 in the position of the evaluated cell in the BALANCING variable
					    }
				    }
				    else if(i>=8 && i<=12){
				        if(VOLTAGES[i+j*12]>V_MIN && (VOLTAGES[i+j*12] > V_BALANCING || (VOLTAGES[i+j*12]-VCELL_MIN)>DIFF_MAX))	//If the evaluated voltage is over V_MIN && (the evaluated voltage is over V_BALANCING || the difference between the evaluated voltage and VCELL_MIN is bigger than DIFF_MAX
					    {
						BALANCING2 = (BALANCING2|(1<<(i-8)));//Set a 1 in the position of the evaluated cell in the BALANCING variable
					    }
				    }
				}
				EVAL[j] = (BALANCING2<<8) + (BALANCING1);	 //Set the DCC array in the evaluated IC position to the value of the BALANCING variable
				DCC[j] = (BALANCING2<<8) + (BALANCING1);
				}
		}

/* Function: MUX_WSITCH
 * Purpose: To switch to the next mux pin changing the GPIOx value
 * Inputs: GPIOx
*/
//MUX CH = 0 -> GPIO 5 = 0; GPIO 4 = 0; GPIO 3 = 0; GPIO 2 = 0; GPIO 1 = 0;
//MUX CH = 1 -> GPIO 5 = 0; GPIO 4 = 0; GPIO 3 = 1; GPIO 2 = 0; GPIO 1 = 0;
//MUX CH = 2 -> GPIO 5 = 0; GPIO 4 = 1; GPIO 3 = 0; GPIO 2 = 0; GPIO 1 = 0;
//MUX CH = 3 -> GPIO 5 = 0; GPIO 4 = 1; GPIO 3 = 1; GPIO 2 = 0; GPIO 1 = 0;
//MUX CH = 4 -> GPIO 5 = 1; GPIO 4 = 0; GPIO 3 = 0; GPIO 2 = 0; GPIO 1 = 0;
//MUX CH = 5 -> GPIO 5 = 1; GPIO 4 = 0; GPIO 3 = 1; GPIO 2 = 0; GPIO 1 = 0;
//MUX CH = 6 -> GPIO 5 = 1; GPIO 4 = 1; GPIO 3 = 0; GPIO 2 = 0; GPIO 1 = 0;
//MUX CH = 7 -> GPIO 5 = 1; GPIO 4 = 1; GPIO 3 = 1; GPIO 2 = 0; GPIO 1 = 0;

uint16_t MUX_SWITCH(uint16_t GPIOx){
	switch (GPIOx){
		  case 0b00000011:
			GPIOx = 0b00000111;
			break;
		  case 0b00000111:
			GPIOx = 0b00001011;
			break;
		  case 0b00001011:
			GPIOx = 0b00001111;
			break;
		  case 0b00001111:
			GPIOx = 0b00010011;
			break;
		  case 0b00010011:
			GPIOx = 0b00010111;
			break;
		  case 0b00010111:
			GPIOx = 0b00011011;
			break;
		  case 0b00011011:
			GPIOx = 0b00011111;
			break;
		  case 0b00011111:
			GPIOx = 0b00000011;
			break;
	  }
	  return GPIOx;
}

/*
 * Function: outputCS
 * Purpose: The outputCs is a function that sets the CS pin to high or low depending on the entering value input (1) or (0)
 */
void outputCS(uint8_t CS_INPUT){
	if(CS_INPUT == 1){
		HAL_GPIO_WritePin(CSPORT, CSPIN, GPIO_PIN_SET);			//The High state of the chip select pin is set
	}
	else{
		HAL_GPIO_WritePin(CSPORT, CSPIN, GPIO_PIN_RESET);		//The Low state of the chip select pin is set
	}
}

/*
 * Function: wakeup_idle
 * Purpose: The wakeup_idle function sends a byte thought SPI with no specific purpose, it just wakes up the ICs.
 * Inputs: The spi_channel which wants to be used
 */
void wakeup_idle(){
	for(uint8_t i = 0; i<TOTAL_IC; i++)				// Loops for each IC
		{
	outputCS(0);		// The Low state of the chip select pin is set
	delay_us(20);		// Waits WAKEUP_DELAY to send again
	outputCS(1);		// The High state of the chip select pin is set
	delay_us(20);		// Waits WAKEUP_DELAY to send again
		}
}

/*
 * Function: wakeup_sleep
 * Purpose:Generic wakeup command to wake the ltc6811 from sleep
 * Inputs: The spi_channel where the data should be sent, the timer to have a delay of 1 us, the number of ICs, the delay between sending each message in us
*/
void wakeup_sleep(){
	for(uint8_t i = 0; i<TOTAL_IC; i++)				// Loops for each IC
	{
		outputCS(0);		// The Low state of the chip select pin is set
		delay_us(700);		// Waits WAKEUP_DELAY to send again
		outputCS(1);		// The High state of the chip select pin is set
		delay_us(700);		// Waits WAKEUP_DELAY to send again
	}
}

/*
 * Function: pec15_calc
 * Purpose: The pec15_calc calculates and returns the CRC15
 * Inputs: The length of the data calculated and the data which has to be evaluated to calculate the PEC
 * uint8_t DATA_LENGTH, 		// Number of bytes that will be used to calculate a PEC
   uint8_t *DATA 				// Array of data that will be used to calculate  a PEC

*/
uint16_t pec15_calc(uint8_t DATA_LENGTH, uint8_t *DATA){
  uint16_t remainder;
  uint16_t addr;
  remainder = 16;								// Initialize the PEC
  for (uint8_t i = 0; i<DATA_LENGTH; i++) 		// Loops for each byte in data array
  {
    addr = ((remainder>>7)^DATA[i])&0xff;		// Calculate PEC table address
    remainder = (remainder<<8)^pgm_read_word_near(crc15Table+addr);
  }
  return(remainder*2);							// The CRC15 has a 0 in the LSB so the remainder must be multiplied by 2
}


/*
// * Function: ltc6811_wrcfg
// * Purpose: This command will write the configuration registers of the ltc6811-1s
// * connected in a daisy chain stack. The configuration is written in descending
// * order so the last device's configuration is written first.
// * Inputs: The number of ICs, the configuration from the function set_cfgr, the SPI channel used
*/
void ltc6811_wrcfg(SPI_HandleTypeDef spi_channel, uint8_t GPIOx, uint16_t *VOLTAGES, uint8_t BLC, uint16_t VCELL_MIN){
  const uint8_t BYTES_IN_REG = 6;
  const uint8_t CMD_LEN = 4+(8*TOTAL_IC);
  uint8_t cmd[CMD_LEN];			// CMD array
  uint16_t cfg_pec;				// PEC
  uint8_t cmd_index; 			// Command counter
  uint8_t config[TOTAL_IC][6];


  if(BLC == 0){
	  for(int i = 0; i < TOTAL_IC*12; i++){
		  DCC[i] = 0;
	  }
  }
  if(BLC == 1){
	  Balancing(VOLTAGES, VCELL_MIN);
  }

  for(int i= 0; i < TOTAL_IC; i++)
  		{
	  	  	config[i][0] = (uint8_t)((GPIOx<<3)&0xF8)|((REFON<<2)&0x04)|((DTEN<<1)&0x02)|(ADCOPT&0x01);
  	    /*				0b01011000 & 0b11111000 | 0b00000100 & 0b0100 | 0b00000010 & 0b0010 | 0b00000001 & 0b00000001
  		    	  		   0b0101 1000		  |     0b0000 0100	    |	 0b0000 0010	      | 		  0b0000 0001
  		    		CFGR0 -> 0b0101 1111*/
  			config[i][1] = (uint8_t)(UNDERVOLTAGE_TH&(0x00FF));
  			config[i][2] = (uint8_t)(((OVERVOLTAGE_TH&(0x000F))<<4) | ((UNDERVOLTAGE_TH&(0x0F00))>>8));
  			config[i][3] = (uint8_t)((OVERVOLTAGE_TH&(0x0FF0))>>4);
  			config[i][4] = (uint8_t)(DCC[i]&0x00FF); //DCC[TOTAL_IC-i-1]
  		    				/*0b00001111 11111111 & 0b00000000 11111111
  		    							0b00000000 11111111
  		     							CFGR4 -> 0b11111111*/
  			config[i][5] = (uint8_t)(((DCTO)<<4) | (((DCC[i]&0x0F00))>>8));
  		    /* 0b11000000 	 | (0b00001111 11111111 & 0b00001111 00000000)>>8
  		       0b11000000 	 | (0b00001111 00000000)>>8
  		       0b11000000 	 | 0b00000000 00001111
  		     		CFGR5 =	0b11001111*/
  		}

  cmd[0] = 0x00;		// Command format 0
  cmd[1] = 0x01;		// Command format 1
  cmd[2] = 0x3d;		// Calculated PEC of Command format 0
  cmd[3] = 0x6e;		// Calculated PEC of Command format 1


  cmd_index = 4;
  for (uint8_t current_ic = TOTAL_IC; current_ic > 0; current_ic--)    	// executes for each ltc6811 in daisy chain, this loops starts with
  {
    // the last IC on the stack. The first configuration written is
    // received by the last IC in the daisy chain

    for (uint8_t current_byte = 0; current_byte < BYTES_IN_REG; current_byte++) // executes for each of the 6 bytes in the CFGR register
    {
      // current_byte is the byte counter

      cmd[cmd_index] = config[current_ic-1][current_byte];           	 //adding the config data to the array to be sent
      cmd_index = cmd_index + 1;
    }

    cfg_pec = (uint16_t)pec15_calc(BYTES_IN_REG, &config[current_ic-1][0]);   // calculating the PEC for each ICs configuration register data
    cmd[cmd_index] = (uint8_t)(cfg_pec >> 8);
    cmd[cmd_index + 1] = (uint8_t)cfg_pec;
    cmd_index = cmd_index + 2;
  }


  wakeup_idle ();               // This will guarantee that the ltc6811 isoSPI port is awake.This command can be removed.

  outputCS(0);								// Set the CS to low to send data
  HAL_SPI_Transmit(&spi_channel, (uint8_t*)cmd, CMD_LEN, HAL_MAX_DELAY); // Sends the CMD trough SPI
  outputCS(1);								// Set the CS to high
}


void ltc6811_wrpwm(SPI_HandleTypeDef spi_channel){
  const uint8_t BYTES_IN_REG = 6;
  const uint8_t CMD_LEN = 4+(8*TOTAL_IC);
  uint8_t cmd[CMD_LEN];			// CMD array
  uint16_t cfg_pec;				// PEC
  uint8_t cmd_index; 			// Command counter
  uint8_t config[TOTAL_IC][6];
  uint16_t cmd_pec;
  uint8_t cmd_1[2];

// Patrón de PWM para balanceo
    uint8_t PWM_ON = 0b0111;
    uint8_t PWM_OFF = 0b0000;

    // Inicializar el array config basado en EVAL
    for (int i = 0; i < TOTAL_IC; i++) {
        for (int j = 0; j < 6; j++) {
            config[i][j] = 0;
        }

        for (int j = 0; j < 12; j++) {
            uint8_t bit_value = (EVAL[i] >> j) & 1;
            uint8_t pwm_value = bit_value ? PWM_ON : PWM_OFF;

            if (j < 8) {
                config[i][j / 2] |= (pwm_value << ((j % 2) * 4));
            } else {
                config[i][4 + (j - 8) / 2] |= (pwm_value << (((j - 8) % 2) * 4));
            }
        }
    }

  cmd_1[0] = 0x00;
  cmd_1[1] = 0x20;
  cmd[0] = 0x00;
  cmd[1] = 0x20;
  cmd_pec = pec15_calc(2, cmd_1);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec);


  cmd_index = 4;
  for (uint8_t current_ic = TOTAL_IC; current_ic > 0; current_ic--)    	// executes for each ltc6811 in daisy chain, this loops starts with
  {
    for (uint8_t current_byte = 0; current_byte < BYTES_IN_REG; current_byte++) // executes for each of the 6 bytes in the CFGR register
    {
      cmd[cmd_index] = config[current_ic-1][current_byte];           	 //adding the config data to the array to be sent
      cmd_index = cmd_index + 1;
    }
    cfg_pec = (uint16_t)pec15_calc(BYTES_IN_REG, &config[current_ic-1][0]);   // calculating the PEC for each ICs configuration register data
    cmd[cmd_index] = (uint8_t)(cfg_pec >> 8);
    cmd[cmd_index + 1] = (uint8_t)cfg_pec;
    cmd_index = cmd_index + 2;
  }


  wakeup_idle ();               // This will guarantee that the ltc6811 isoSPI port is awake.This command can be removed.
  outputCS(0);								// Set the CS to low to send data
  HAL_SPI_Transmit(&spi_channel, (uint8_t*)cmd, CMD_LEN, HAL_MAX_DELAY); // Sends the CMD trough SPI
  outputCS(1);								// Set the CS to high
}


/* Function: ltc6811_adcv
 * Purpose: Starts ADC conversions of the ltc6811 pin inputs.
  The type of ADC conversion executed can be changed by setting the following parameters:
 |Variable|Function                                      |
 |--------|----------------------------------------------|
 | MD     | Determines the filter corner of the ADC      | It depends on the clock used:
															 If ADCOPT(CFGR0[0]) = 0: 00->442Hz; 01->27kHz; 10->7kHz; 11->26Hz
															 If ADCOPT(CFGR0[1]) = 1: 00->1kHz; 01->14kHz; 10->3kHz; 11->2kHz
 | CH     | Determines which cell channels are converted | If CH=000->All cells; CH=001->1 & 7; CH=010->2 & 8; CH=011->3 & 9; CH=100->4 & 10; CH=101->5 & 11; CH=110->6 & 12;
 | DCP    | Determines if Discharge is Permitted       	 | DCP=0->Not Permitted; DCP=1->Permitted

 Broadcast Command Code:
-------------

|CMD[0:1] |  15   |  14   |  13   |  12   |  11   |  10   |   9   |   8   |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
|-----------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|
|ADCV:      |   0   |   0   |   0   |   0   |   0   |   0   |   1   | MD[1] | MD[2] |   1   |   1   |  DCP  |   0   | CH[2] | CH[1] | CH[0] |
* Inputs: The Conversion Mode (MD), discharge permitted (DCP), the cell selection for ADC conversion (CH) and the SPI channel used
*/
void ltc6811_adcv(SPI_HandleTypeDef spi_channel, uint8_t CH){
	  uint8_t cmd[4];
	  uint16_t cmd_pec;
	  uint8_t md_bits;

		  md_bits = (MD & 0x02) >> 1;
		  cmd[0] = md_bits + 0x02;
		  md_bits = (MD & 0x01) << 7;
		  cmd[1] =  md_bits + 0x60 + (DCP<<4) + CH;
		  cmd_pec = pec15_calc(2, cmd);
		  cmd[2] = (uint8_t)(cmd_pec >> 8);
		  cmd[3] = (uint8_t)(cmd_pec);

		  wakeup_idle (); //This will guarantee that the ltc6811 isoSPI port is awake. This command can be removed.

		  outputCS(0);															//The Low state of the chip select pin is set
		  HAL_SPI_Transmit(&spi_channel, (uint8_t *)cmd, 4, HAL_MAX_DELAY);
		  outputCS(0);
															//The High state of the chip select pin is set
	}

/* Function: ltc6811_rdcv_reg
 * Purpose: The function reads a single cell voltage register and stores the read data
 in the *data point as a byte array. This function is rarely used outside of
 the ltc6811_rdcv() command.

 @param[in] uint8_t REG_VOLT; This controls which cell voltage register is read back.
  1: Read back cell group A
  2: Read back cell group B
  3: Read back cell group C
  4: Read back cell group D

	Command Code:
	-------------
	|CMD[0:1] |  15   |  14   |  13   |  12   |  11   |  10   |   9   |   8   |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
	|-----------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|
	|RDCVA:     |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   1   |   0   |   0   |
	|RDCVB:     |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   1   |   1   |   0   |
	|RDCVC:     |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   1   |   0   |   0   |   0   |
	|RDCVD:     |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   1   |   0   |   1   |   0   |
	//uint8_t REG_VOLT, //Determines which cell voltage register is read back
	 * uint8_t *data //An array of the unparsed cell codes


 * Inputs: The register read, the number of ICs, the voltage of the cells array and the SPI channel used
*/
void ltc6811_rdcv_reg(SPI_HandleTypeDef spi_channel,uint8_t reg, uint8_t *data){
		  const uint8_t REG_LEN = 8; //number of bytes in each ICs register + 2 bytes for the PEC
		  uint8_t REG_LEN_TOTAL_IC = (4+REG_LEN*(TOTAL_IC));
		  uint8_t cmd[4];
		  uint16_t cmd_pec;

		  if (reg == 1)     //1: RDCVA
		  {
			cmd[1] = 0x04;
			cmd[0] = 0x00;
		  }
		  else if (reg == 2) //2: RDCVB
		  {
			cmd[1] = 0x06;
			cmd[0] = 0x00;
		  }
		  else if (reg == 3) //3: RDCVC
		  {
			cmd[1] = 0x08;
			cmd[0] = 0x00;
		  }
		  else if (reg == 4) //4: RDCVD
		  {
			cmd[1] = 0x0A;
			cmd[0] = 0x00;
		  }
		  else if (reg == 5) //4: RDCVE
		  {
			cmd[1] = 0x09;
			cmd[0] = 0x00;
		  }
		  else if (reg == 6) //4: RDCVF
		  {
			cmd[1] = 0x0B;
			cmd[0] = 0x00;
		  }


		  cmd_pec = pec15_calc(2, cmd);
		  cmd[2] = (uint8_t)(cmd_pec >> 8);
		  cmd[3] = (uint8_t)(cmd_pec);

		  wakeup_idle (); //This will guarantee that the ltc6811 isoSPI port is awake. This command can be removed.

		  outputCS(0);								// Set the CS to low to send data
		  HAL_SPI_TransmitReceive(&spi_channel, (uint8_t *)cmd, (uint8_t *)data, REG_LEN_TOTAL_IC, HAL_MAX_DELAY);
		  outputCS(1);					//The Low state of the chip select pin is set
}
/* Function: ltc6811_rav
 * Purpose: The function is used to read the cell codes of the ltc6811.
 This function will send the requested read commands parse the data
 and store the cell voltages in cell_codes variable.

 uint8_t REG_VOLT; This controls which cell voltage register is read back.
		  0: Read back all Cell registers
		  1: Read back cell group A
		  2: Read back cell group B
		  3: Read back cell group C
		  4: Read back cell group D

  uint8_t TOTAL_IC; This is the number of ICs in the daisy chain(-1 only)

  uint16_t cell_codes[]; An array of the parsed cell codes from lowest to highest. The cell codes will
  be stored in the cell_codes[] array in the following format:
  |  cell_codes[0][0]| cell_codes[0][1] |  cell_codes[0][2]|    .....     |  cell_codes[0][11]|  cell_codes[1][0] | cell_codes[1][1]|  .....   |
  |------------------|------------------|------------------|--------------|-------------------|-------------------|-----------------|----------|
  |IC1 Cell 1        |IC1 Cell 2        |IC1 Cell 3        |    .....     |  IC1 Cell 12      |IC2 Cell 1         |IC2 Cell 2       | .....    |

  @return int8_t, PEC Status.

	0: No PEC error detected

	-1: PEC error detected, retry read
* Inputs: The voltage of the cells array and the SPI channel used
*/
void ltc6811_rav(SPI_HandleTypeDef spi_channel, uint8_t *PEC_SLAVES, uint16_t *voltatjes, uint8_t GPIOx, uint8_t CH, uint8_t BLC, uint16_t VCELL_MIN){
	uint8_t recieve[4+TOTAL_IC*8];
	uint8_t pec_array[6];
	uint8_t data_pec;
	uint8_t received_pec;

	 ltc6811_wrcfg(spi_channel, GPIOx, voltatjes, BLC, VCELL_MIN);
	 ltc6811_adcv(spi_channel, CH);
	 HAL_Delay(13);

	 for(int reg = 0; reg<4; reg++){
		  ltc6811_rdcv_reg(spi_channel, reg+1, recieve);
		  uint8_t sum = 0;
		  uint8_t sum2 = 4;
		  uint8_t ic = 0;
		  while(sum<6*TOTAL_IC){

			  received_pec = (recieve[sum2+6] << 8) + recieve[sum2+7]; //The received PEC for the current_ic is transmitted as the 7th and 8th

			  pec_array[0] = recieve[sum2];
			  pec_array[1] = recieve[sum2+1];
			  pec_array[2] = recieve[sum2+2];
			  pec_array[3] = recieve[sum2+3];
			  pec_array[4] = recieve[sum2+4];
			  pec_array[5] = recieve[sum2+5];

			  data_pec = pec15_calc(6, pec_array);
	  	        if (received_pec != data_pec)
	  	        {
	  	        	PEC_SLAVES[ic * 4 + reg] = 1;
	  	        }
	  	        else{
	  	        	voltatjes[12*ic+(reg*3)+2] = (recieve[sum2+5]<<8)+recieve[sum2+4];
					voltatjes[12*ic+(reg*3)+1] = (recieve[sum2+3]<<8)+recieve[sum2+2];
					voltatjes[12*ic+(reg*3)] = (recieve[sum2+1]<<8)+recieve[sum2];
					PEC_SLAVES[ic * 4 + reg] = 0;
	  	        }
	  	      sum = sum+6;
	  	      sum2= sum2+8;
	  	      ic++;
		  }

	 }
}
/* Function: ltc6811_adax
* Purpose: Start a GPIO and Vref2 Conversion
	The type of ADC conversion executed can be changed by passing the following variables to the conversion functions:
	 |Variable|Function                                      |
	 |--------|----------------------------------------------|
	 | MD     | Determines the filter corner of the ADC      |
	 | CHG    | Determines which GPIO channels are converted |
	 | ST     | Determines which Self Test is executed       |

	Starts an ADC conversions of the ltc6811 GPIO inputs.
	Command Code:
	-------------

	|CMD[0:1] |  15   |  14   |  13   |  12   |  11   |  10   |   9   |   8   |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
	|-----------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|
	|ADAX:      |   0   |   0   |   0   |   0   |   0   |   1   |   0   | MD[1] | MD[2] |   1   |   1   |  0    |   0   | CHG[2]| CHG[1]| CHG[0]|
* Inputs: The Conversion Mode (MD), GPIO Selection for ADC and the SPI channel which has to be used
*/
void ltc6811_adax(SPI_HandleTypeDef spi_channel){
		uint8_t cmd[4];
		uint16_t cmd_pec;
		uint8_t md_bits;

		md_bits = (MD & 0x02) >> 1;
		cmd[0] = md_bits + 0x04;
		md_bits = (MD & 0x01) << 7;
		cmd[1] = md_bits + 0x60 + CHG ;

		cmd_pec = pec15_calc(2, cmd);
		cmd[2] = (uint8_t)(cmd_pec >> 8);
		cmd[3] = (uint8_t)(cmd_pec);

		wakeup_idle (); //This will guarantee that the ltc6811 isoSPI port is awake. This command can be removed.

		outputCS(0);					//The Low state of the chip select pin is set
		HAL_SPI_Transmit(&spi_channel, cmd, 4, HAL_MAX_DELAY);
		outputCS(1);					//The Low state of the chip select pin is set
	  }

/*
 * Function: ltc6811_rdaux_reg
 * Purspose:The function reads a single GPIO voltage register and stores thre read data
 in the *data point as a byte array. This function is rarely used outside of
 the ltc6811_rdaux() command.

Command Code:
-------------

|CMD[0:1]     |  15   |  14   |  13   |  12   |  11   |  10   |   9   |   8   |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
|---------------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|
|RDAUXA:      |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   1   |   1   |   0   |   0   |
|RDAUXB:      |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |   1   |   1   |   1   |   0   |
* Inputs: The register read, the number of ICs, the voltage of the cells array and the SPI channel used
*/
void ltc6811_rdaux_reg(SPI_HandleTypeDef spi_channel, uint8_t reg, uint8_t *data //Array of the unparsed auxiliary codes
					   ){
  // SPI_HandleTypeDef hspi1;
  const uint8_t REG_LEN = 8; // number of bytes in the register + 2 bytes for the PEC
  uint8_t REG_LEN_TOTAL_IC = (4+(REG_LEN*TOTAL_IC));
  uint8_t cmd[4];
  uint16_t cmd_pec;


  if (reg == 1)     //Read back auxiliary group A
  {
    cmd[1] = 0x0C;
    cmd[0] = 0x00;
  }
  else if (reg == 2)  //Read back auxiliary group B
  {
    cmd[1] = 0x0e;
    cmd[0] = 0x00;
  }
  else if (reg == 3)  //Read back auxiliary group B
  {
    cmd[1] = 0x0D;
    cmd[0] = 0x00;
  }
  else if (reg == 4)  //Read back auxiliary group B
  {
    cmd[1] = 0x0F;
    cmd[0] = 0x00;
  }
  else          //Read back auxiliary group A
  {
    cmd[1] = 0x0C;
    cmd[0] = 0x00;
  }

  cmd_pec = pec15_calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec);

	  wakeup_idle (); //This will guarantee that the ltc6811 isoSPI port is awake. This command can be removed.

	  outputCS(0);								// Set the CS to low to send data
	  HAL_SPI_TransmitReceive(&spi_channel, (uint8_t *)cmd, (uint8_t *)data, REG_LEN_TOTAL_IC,HAL_MAX_DELAY); // Sends the CMD trough SPI
	  outputCS(1);								// Set the  CS to high
}

/*
// * Function: ltc6811_rat
// * Purpose: The function is used to read the  parsed GPIO codes of the ltc6811. This function will send the requested
// read commands parse the data and store the gpio voltages in aux_codes variable
//
// uint16_t aux_codes[][6]; A two dimensional array of the gpio voltage codes. The GPIO codes will
// be stored in the aux_codes[][6] array in the following format:
// |  aux_codes[0][0]| aux_codes[0][1] |  aux_codes[0][2]|  aux_codes[0][3]|  aux_codes[0][4]|  aux_codes[0][5]| aux_codes[1][0] |aux_codes[1][1]|  .....    |
// |-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|---------------|-----------|
// |IC1 GPIO1        |IC1 GPIO2        |IC1 GPIO3        |IC1 GPIO4        |IC1 GPIO5        |IC1 Vref2        |IC2 GPIO1        |IC2 GPIO2      |  .....    |
//
//@return  int8_t, PEC Status
//  0: No PEC error detected
// -1: PEC error detected, retry read
// * Inputs: uint16_t aux_codes[][AUX_CHANNELS]//A two dimensional array of the gpio voltage codes.
*/
void ltc6811_rat(SPI_HandleTypeDef spi_channel, uint8_t *PEC_SLAVES, uint16_t *voltatjes, uint16_t *temperatures, uint8_t BLC, uint16_t VCELL_MIN) {
    uint8_t recieve[4 + TOTAL_IC * 8];
    uint8_t pec_array[6];
    uint16_t data_pec;
    uint16_t received_pec;
    uint8_t GPIOx = 0b00000011;

    for (int gpio = 0; gpio < 8; gpio++) {
        for (int i = 0; i < 5; i++) {
            ltc6811_wrcfg(spi_channel, GPIOx, voltatjes, BLC, VCELL_MIN);
        }

        delay_us(700);

        for (int i = 0; i < 4; i++) {
            ltc6811_adax(spi_channel);
        }

        delay_us(2400);

        uint8_t cont = 4;
        ltc6811_rdaux_reg(spi_channel, 1, recieve);

        for (int ic = 0; ic < TOTAL_IC; ic++) {
            received_pec = (recieve[cont + 6] << 8) + recieve[cont + 7];

            for (int i = 0; i < 6; i++) {
                pec_array[i] = recieve[cont + i];
            }

            data_pec = pec15_calc(6, pec_array);

            if (received_pec != data_pec) {
                PEC_SLAVES[ic * 8 + gpio] = 1;
            } else {
                temperatures[gpio + ic * 8] = (recieve[cont + 1] << 8) + recieve[cont];
                PEC_SLAVES[ic * 8 + gpio] = 0;
            }

            cont += 8;
        }
        GPIOx = MUX_SWITCH(GPIOx);
    }
}

