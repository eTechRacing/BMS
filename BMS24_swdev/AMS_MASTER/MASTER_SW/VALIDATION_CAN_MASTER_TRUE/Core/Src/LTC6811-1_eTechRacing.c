/*
 * LTC6811-1_eTechRacing1.c
 *
 *  Created on: Jan 26, 2024
 *      Author: Bernat
 */
#include "LTC6811-1_eTechRacing.h"
#include "eTechRacing.h"
#include "main.h"
#include "stdlib.h"
#include <pgmspace.h>
#include "Linduino.h"

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

uint8_t GPIOx = 0b00001011; //Suma 001 al final per anar variant -> La configuració dels GPIOs poden ser valors entre 0,1,2,3,4,5,6,7
// 0 -> 0b000 000 11 | 1 -> 0b000 001 11 | 2 -> 0b000 010 11 | 3 -> 0b000 011 11 | 4 -> 0b000 100 11 | 5 -> 0b000 101 11 | 6 -> 0b000 110 11 | 7 -> 0b000 111 11 |
// 0 -> 0b000 CBA 11
//  CFGR0 	| GPIO5	| GPIO4	| GPIO3	| GPIO2	| GPIO1	| REFON	| DTEN	| ADCOPT| // | GPIO1 -> 1 | GPIO2 -> 1 | GPIO3 -> A | GPIO4 -> B | GPIO5 -> C

uint16_t DCC[2] = {0x0FFF,0x0FFF}; 			//0b00001111 11111111

void outputCS(uint8_t input){
	if(input == 1){
		HAL_GPIO_WritePin(CSPORT, CSPIN, GPIO_PIN_SET);					//The High state of the chip select pin is set
	}
	else{
		HAL_GPIO_WritePin(CSPORT, CSPIN, GPIO_PIN_RESET);				//The Low state of the chip select pin is set
	}
}

void wakeup_idle(SPI_HandleTypeDef spi_channel)
{
	uint8_t idle_message = 0xFF;
	outputCS(0);					//The Low state of the chip select pin is set
	HAL_SPI_Transmit(&spi_channel, (uint8_t*)&idle_message, 1, HAL_MAX_DELAY);
	outputCS(1);					//The Low state of the chip select pin is set
}

/*
Calculates  and returns the CRC15
*/
uint16_t pec15_calc(uint8_t len, //Number of bytes that will be used to calculate a PEC
                    uint8_t *data //Array of data that will be used to calculate  a PEC
                   )
{
  uint16_t remainder,addr;

  remainder = 16;//initialize the PEC
  for (uint8_t i = 0; i<len; i++) // loops for each byte in data array
  {
    addr = ((remainder>>7)^data[i])&0xff;//calculate PEC table address
    remainder = (remainder<<8)^pgm_read_word_near(crc15Table+addr);
  }
  return(remainder*2);//The CRC15 has a 0 in the LSB so the remainder must be multiplied by 2
}

/*
 Generic wakeup command to wake the ltc6811 from sleep
*/
void wakeup_sleep(SPI_HandleTypeDef spi_channel, TIM_HandleTypeDef timer, int total_ic, int WAKEUP_DELAY)
{
	uint8_t idle_message = 0xFF;
	for(int i = 0; i<total_ic; i++)
	{
		outputCS(0);				//The Low state of the chip select pin is set
		HAL_SPI_Transmit(&spi_channel, (uint8_t*)&idle_message, 1, HAL_MAX_DELAY);
		outputCS(1);				//The High state of the chip select pin is set
	}
	delay_us(WAKEUP_DELAY, timer);
}



//uint8_t pwm_reg[][];
//
//void set_pwm(SPI_HandleTypeDef spi_channel)
//{
//
//}


	int **set_cfgr(int total_ic, int n){
		int **config_set;
		config_set = malloc(sizeof(int *) * total_ic);
		for(int i = 0; i < total_ic; i++)
		{
			config_set[i] = malloc(sizeof(int) * n);
		}

	for(int i= 0; i < total_ic; i++)
	{	config_set[i][0] = (uint8_t)((GPIOx<<3)&0xF8)|((REFON<<2)&0x04)|((DTEN<<1)&0x02)|(ADCOPT&0x01);
	    //				0b01011000 & 0b11111000 | 0b00000100 & 0b0100 | 0b00000010 & 0b0010 | 0b00000001 & 0b00000001
		    //	  		   0b0101 1000		  |     0b0000 0100	    |	 0b0000 0010	      | 		  0b0000 0001
		    //		CFGR0 -> 0b0101 1111
	config_set[i][1] = (uint8_t)(UNDERVOLTAGE_TH&(0x00FF));
	config_set[i][2] = (uint8_t)(((OVERVOLTAGE_TH&(0x000F))<<4) | ((UNDERVOLTAGE_TH&(0x0F00))>>8));
	config_set[i][3] = (uint8_t)((OVERVOLTAGE_TH&(0x0FF0))>>4);
	config_set[i][4] = (uint8_t)(DCC[i]&0x00FF); //DCC[total_ic-i-1]
		    //0b00001111 11111111 & 0b00000000 11111111
		    //		0b00000000 11111111
		    // 		CFGR4 -> 0b11111111
	config_set[i][5] = (uint8_t)(((DCTO)<<4) | (((DCC[i]&0x0F00))>>8));
		    // 0b11000000 	 | (0b00001111 11111111 & 0b00001111 00000000)>>8
		    // 0b11000000 	 | (0b00001111 00000000)>>8
		    // 0b11000000 	 | 0b00000000 00001111
		    // 		CFGR5 =	0b11001111
		}
	return config_set;
	}


/*
 This command will write the configuration registers of the ltc6811-1s
 connected in a daisy chain stack. The configuration is written in descending
 order so the last device's configuration is written first.
*/
void ltc6811_wrcfg(uint8_t total_ic, //The number of ICs being written to
                   uint8_t **config[ICn][6] //A two dimensional array of the configuration data that will be written
                  ,SPI_HandleTypeDef spi_channel){
  const uint8_t BYTES_IN_REG = 6;
  const uint8_t CMD_LEN = 4+(8*total_ic);
  uint8_t *cmd;
  uint16_t cfg_pec;
  uint8_t cmd_index; //command counter

  cmd = (uint8_t *)malloc(CMD_LEN*sizeof(uint8_t));


  cmd[0] = 0x00;
  cmd[1] = 0x01;
  cmd[2] = 0x3d;
  cmd[3] = 0x6e;


  cmd_index = 4;
  for (uint8_t current_ic = total_ic; current_ic > 0; current_ic--)       // executes for each ltc6811 in daisy chain, this loops starts with
  {
    // the last IC on the stack. The first configuration written is
    // received by the last IC in the daisy chain

    for (uint8_t current_byte = 0; current_byte < BYTES_IN_REG; current_byte++) // executes for each of the 6 bytes in the CFGR register
    {
      // current_byte is the byte counter

      cmd[cmd_index] = &config[current_ic-1][current_byte];            //adding the config data to the array to be sent
      cmd_index = cmd_index + 1;
    }

    cfg_pec = (uint16_t)pec15_calc(BYTES_IN_REG, &config[current_ic-1][0]);   // calculating the PEC for each ICs configuration register data
    cmd[cmd_index] = (uint8_t)(cfg_pec >> 8);
    cmd[cmd_index + 1] = (uint8_t)cfg_pec;
    cmd_index = cmd_index + 2;
  }


  wakeup_idle (spi_channel);                                 //This will guarantee that the ltc6811 isoSPI port is awake.This command can be removed.

  outputCS(0);
  HAL_Delay(10);
  outputCS(1);
  HAL_Delay(10);
  outputCS(0);
  HAL_SPI_Transmit(&spi_channel, (uint8_t*)&cmd, CMD_LEN, HAL_MAX_DELAY);
  outputCS(1);
  free(cmd);
}

	/*Starts ADC conversions of the ltc6811 Cpin inputs.
	  The type of ADC conversion executed can be changed by setting the following parameters:
	 |Variable|Function                                      |
	 |--------|----------------------------------------------|
	 | MD     | Determines the filter corner of the ADC      |
	 | CH     | Determines which cell channels are converted |
	 | DCP    | Determines if Discharge is Permitted       	 |

	 Broadcast Command Code:
	-------------

	|CMD[0:1] |  15   |  14   |  13   |  12   |  11   |  10   |   9   |   8   |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
	|-----------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|
	|ADCV:      |   0   |   0   |   0   |   0   |   0   |   0   |   1   | MD[1] | MD[2] |   1   |   1   |  DCP  |   0   | CH[2] | CH[1] | CH[0] |

	*/
void ltc6811_adcv(uint8_t MD, uint8_t DCP, uint8_t CH,SPI_HandleTypeDef spi_channel){
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


	  wakeup_idle (spi_channel); //This will guarantee that the ltc6811 isoSPI port is awake. This command can be removed.
	  outputCS(0);															//The Low state of the chip select pin is set
	  HAL_SPI_Transmit(&spi_channel, (uint8_t*)&cmd, 4, HAL_MAX_DELAY);
	  outputCS(1);															//The High state of the chip select pin is set
	}

	  	/*The function is used to read the cell codes of the ltc6811.
	  	 This function will send the requested read commands parse the data
	  	 and store the cell voltages in cell_codes variable.

	  	 uint8_t reg; This controls which cell voltage register is read back.
	  	          0: Read back all Cell registers
	  	          1: Read back cell group A
	  	          2: Read back cell group B
	  	          3: Read back cell group C
	  	          4: Read back cell group D

	  	  uint8_t total_ic; This is the number of ICs in the daisy chain(-1 only)

	  	  uint16_t cell_codes[]; An array of the parsed cell codes from lowest to highest. The cell codes will
	  	  be stored in the cell_codes[] array in the following format:
	  	  |  cell_codes[0][0]| cell_codes[0][1] |  cell_codes[0][2]|    .....     |  cell_codes[0][11]|  cell_codes[1][0] | cell_codes[1][1]|  .....   |
	  	  |------------------|------------------|------------------|--------------|-------------------|-------------------|-----------------|----------|
	  	  |IC1 Cell 1        |IC1 Cell 2        |IC1 Cell 3        |    .....     |  IC1 Cell 12      |IC2 Cell 1         |IC2 Cell 2       | .....    |

	  	  @return int8_t, PEC Status.

	  	    0: No PEC error detected

	  	    -1: PEC error detected, retry read
	  	*/

		// Reads and parses the ltc6811 cell voltage registers.
	uint16_t ltc6811_rdcv(uint8_t reg,uint8_t total_ic,uint16_t cell_codes[][CELL_CHANNELS], SPI_HandleTypeDef spi_channel){
	  	  const uint8_t NUM_RX_BYT = 8;
	  	  const uint8_t BYT_IN_REG = 6;
	  	  const uint8_t CELL_IN_REG = 3;
	  	  const uint8_t NUM_CV_REG = 4;


	  	  uint8_t *cell_data;
	  	  uint8_t pec_error = 0;
	  	  uint16_t parsed_cell;
	  	  uint16_t received_pec;
	  	  uint16_t data_pec;
	  	  uint8_t data_counter=0; //data counter
	  	  cell_data = (uint8_t *) malloc((NUM_RX_BYT*total_ic)*sizeof(uint8_t));

	  	  if (reg == 0)
	  	  {

	  	    for (uint8_t cell_reg = 1; cell_reg<NUM_CV_REG+1; cell_reg++)                   //executes once for each of the ltc6811 cell voltage registers
	  	    {
	  	      data_counter = 0;
	  	      ltc6811_rdcv_reg(cell_reg, total_ic, cell_data,spi_channel);                //Reads a single Cell voltage register

	  	      for (uint8_t current_ic = 0 ; current_ic < total_ic; current_ic++)      // executes for every ltc6811 in the daisy chain
	  	      {
	  	        // current_ic is used as the IC counter

	  	        for (uint8_t current_cell = 0; current_cell<CELL_IN_REG; current_cell++)  // This loop parses the read back data into cell voltages, it
	  	        {
	  	          // loops once for each of the 3 cell voltage codes in the register

	  	          parsed_cell = cell_data[data_counter] + (cell_data[data_counter + 1] << 8);//Each cell code is received as two bytes and is combined to
	  	          //Array a bytes
	  	          // create the parsed cell voltage code

	  	          cell_codes[current_ic][current_cell  + ((cell_reg - 1) * CELL_IN_REG)] = parsed_cell;
	  	          //cell_codes[		0  ][		0		+    ((1	- 1 ) * 3	)] = cell_codes[0][0]
	  	          //cell_codes[		0  ][		1		+    ((1	- 1 ) * 3	)] = cell_codes[0][1]
	  	          //cell_codes[		0  ][		2		+    ((1	- 1 ) * 3	)] = cell_codes[0][2]
	  	          //cell_codes[		1  ][		0		+    ((1	- 1 ) * 3	)] = cell_codes[1][0]
	  	          //cell_codes[		1  ][		1		+    ((1	- 1 ) * 3	)] = cell_codes[1][1]
	  	          //cell_codes[		1  ][		2		+    ((1	- 1 ) * 3	)] = cell_codes[1][2]
	  	          //|
	  	          //|Per les 15 slaves
	  	          //|
	  	          //V
	  	          //cell_codes[		0  ][		0		+    ((2	- 1 ) * 3	)] = cell_codes[0][3]
	  	          //cell_codes[		0  ][		1		+    ((2	- 1 ) * 3	)] = cell_codes[0][4]
	  	          //cell_codes[		0  ][		2		+    ((2	- 1 ) * 3	)] = cell_codes[0][5]
	  	          //cell_codes[		1  ][		0		+    ((2	- 1 ) * 3	)] = cell_codes[1][3]
	  	          //cell_codes[		1  ][		1		+    ((2	- 1 ) * 3	)] = cell_codes[1][4]
	  	          //cell_codes[		1  ][		2		+    ((2	- 1 ) * 3	)] = cell_codes[1][5]
	  	          //|
	  	          //|Per les 15 slaves
	  	          //|
	  	          //V
	  	          //cell_codes[		0  ][		0		+    ((3	- 1 ) * 3	)] = cell_codes[0][6]
	  	          //cell_codes[		0  ][		1		+    ((3	- 1 ) * 3	)] = cell_codes[0][7]
	  	          //cell_codes[		0  ][		2		+    ((3	- 1 ) * 3	)] = cell_codes[0][8]
	  	          //cell_codes[		1  ][		0		+    ((3	- 1 ) * 3	)] = cell_codes[1][6]
	  	          //cell_codes[		1  ][		1		+    ((3	- 1 ) * 3	)] = cell_codes[1][7]
	  	          //cell_codes[		1  ][		2		+    ((3	- 1 ) * 3	)] = cell_codes[1][8]
	  	          //|
	  	          //|Per les 15 slaves
	  	          //|
	  	          //V
	  	          //cell_codes[		0  ][		0		+    ((4	- 1 ) * 3	)] = cell_codes[0][9]
	  	          //cell_codes[		0  ][		1		+    ((4	- 1 ) * 3	)] = cell_codes[0][10]
	  	          //cell_codes[		0  ][		2		+    ((4	- 1 ) * 3	)] = cell_codes[0][11]
	  	          //cell_codes[		1  ][		0		+    ((4	- 1 ) * 3	)] = cell_codes[1][9]
	  	          //cell_codes[		1  ][		1		+    ((4	- 1 ) * 3	)] = cell_codes[1][10]
	  	          //cell_codes[		1  ][		2		+    ((4	- 1 ) * 3	)] = cell_codes[1][11]
	  	          data_counter = data_counter + 2;                       //Because cell voltage codes are two bytes the data counter
	  	          //must increment by two for each parsed cell code
	  	        }

	  	        received_pec = (cell_data[data_counter] << 8) + cell_data[data_counter+1]; //The received PEC for the current_ic is transmitted as the 7th and 8th
	  	        //after the 6 cell voltage data bytes
	  	        data_pec = pec15_calc(BYT_IN_REG, &cell_data[current_ic * NUM_RX_BYT]);
	  	        if (received_pec != data_pec)
	  	        {
	  	          pec_error = -1;                             //The pec_error variable is simply set negative if any PEC errors
	  	          //are detected in the serial data
	  	        }
	  	        data_counter=data_counter+2;                        //Because the transmitted PEC code is 2 bytes long the data_counter
	  	        //must be incremented by 2 bytes to point to the next ICs cell voltage data
	  	      }
	  	    }
	  	  }

	  	  else
	  	  {

	  	    ltc6811_rdcv_reg(reg, total_ic,cell_data,spi_channel);
	  	    for (uint8_t current_ic = 0 ; current_ic < total_ic; current_ic++)        // executes for every ltc6811 in the daisy chain
	  	    {
	  	      // current_ic is used as the IC counter

	  	      for (uint8_t current_cell = 0; current_cell < CELL_IN_REG; current_cell++)  // This loop parses the read back data into cell voltages, it
	  	      {
	  	        // loops once for each of the 3 cell voltage codes in the register

	  	        parsed_cell = cell_data[data_counter] + (cell_data[data_counter+1]<<8); //Each cell code is received as two bytes and is combined to
	  	        // create the parsed cell voltage code

	  	        cell_codes[current_ic][current_cell + ((reg - 1) * CELL_IN_REG)] = 0x0000FFFF & parsed_cell;
	  	        data_counter= data_counter + 2;                       //Because cell voltage codes are two bytes the data counter
	  	        //must increment by two for each parsed cell code
	  	      }

	  	      received_pec = (cell_data[data_counter] << 8 )+ cell_data[data_counter + 1]; //The received PEC for the current_ic is transmitted as the 7th and 8th
	  	      //after the 6 cell voltage data bytes
	  	      data_pec = pec15_calc(BYT_IN_REG, &cell_data[current_ic * NUM_RX_BYT]);
	  	      if (received_pec != data_pec)
	  	      {
	  	        pec_error = -1;                             //The pec_error variable is simply set negative if any PEC errors
	  	        //are detected in the serial data
	  	      }
	  	      data_counter= data_counter + 2;                       //Because the transmitted PEC code is 2 bytes long the data_counter
	  	      //must be incremented by 2 bytes to point to the next ICs cell voltage data
	  	    }
	  	  }


	  	  free(cell_data);
	  	  return(pec_error);
	  	}


	  	//Read the raw data from the ltc6811 cell voltage register
	  	void ltc6811_rdcv_reg(uint8_t reg, //Determines which cell voltage register is read back
	  	                      uint8_t total_ic, //the number of ICs in the
	  	                      uint8_t *data //An array of the unparsed cell codes
							  ,SPI_HandleTypeDef spi_channel)
	  	{
	  	  const uint8_t REG_LEN = 8; //number of bytes in each ICs register + 2 bytes for the PEC
	  	  uint8_t REG_LEN_TOTAL_IC = (REG_LEN*total_ic);
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

	  	  wakeup_idle (spi_channel); //This will guarantee that the ltc6811 isoSPI port is awake. This command can be removed.

	  	  outputCS(0);
	  	  HAL_Delay(10);
	  	  outputCS(1);
	  	  HAL_Delay(10);
	  	  outputCS(0);					//The Low state of the chip select pin is set
	  	  //HAL_SPI_TransmitReceive(&spi_channel, cmd, data, REG_LEN_TOTAL_IC,HAL_MAX_DELAY);
	  	  spi_transmit_recieve(spi_channel, cmd, data, REG_LEN_TOTAL_IC);
	  	  outputCS(1);					//The Low state of the chip select pin is set
	  	  //Fer una funció que envia el cmd i un bucle for que rep byte per byte al informació fent servir transmitrecieve enviant 0XFF i posant el recieve a la posició de data que t'interessa
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

	  	 /*
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
	  		  	*/

	  		  	//Start a GPIO and Vref2 Conversion
void ltc6811_adax(uint8_t MD,uint8_t CHG,SPI_HandleTypeDef spi_channel){
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

	  		    wakeup_idle (spi_channel); //This will guarantee that the ltc6811 isoSPI port is awake. This command can be removed.
	  		    outputCS(0);					//The Low state of the chip select pin is set
	  		    HAL_SPI_Transmit(&spi_channel, cmd, 4, HAL_MAX_DELAY);
	  		    outputCS(0);					//The Low state of the chip select pin is set

	  		  }
/*
 The function is used
 to read the  parsed GPIO codes of the ltc6811. This function will send the requested
 read commands parse the data and store the gpio voltages in aux_codes variable
*/
int8_t ltc6811_rdaux(uint8_t reg, //Determines which GPIO voltage register is read back.
                     uint8_t total_ic,//the number of ICs in the system
                     uint16_t aux_codes[][AUX_CHANNELS],//A two dimensional array of the gpio voltage codes.
					 SPI_HandleTypeDef spi_channel)
{
  const uint8_t NUM_RX_BYT = 8;
  const uint8_t BYT_IN_REG = 6;
  const uint8_t GPIO_IN_REG = 3;
  const uint8_t NUM_GPIO_REG = 2;
  uint8_t *data;
  uint8_t data_counter = 0;
  int8_t pec_error = 0;
  uint16_t parsed_aux;
  uint16_t received_pec;
  uint16_t data_pec;
  data = (uint8_t *) malloc((NUM_RX_BYT*total_ic)*sizeof(uint8_t));

  if (reg == 0)
  {

    for (uint8_t gpio_reg = 1; gpio_reg<NUM_GPIO_REG+1; gpio_reg++)                 //executes once for each of the ltc6811 aux voltage registers
    {
      data_counter = 0;
      ltc6811_rdaux_reg(gpio_reg, total_ic,data, spi_channel);                 //Reads the raw auxiliary register data into the data[] array

      for (uint8_t current_ic = 0 ; current_ic < total_ic; current_ic++)      // executes for every ltc6811 in the daisy chain
      {
        // current_ic is used as the IC counter


        for (uint8_t current_gpio = 0; current_gpio< GPIO_IN_REG; current_gpio++) // This loop parses the read back data into GPIO voltages, it
        {
          // loops once for each of the 3 gpio voltage codes in the register

          parsed_aux = data[data_counter] + (data[data_counter+1]<<8);              //Each gpio codes is received as two bytes and is combined to
          // create the parsed gpio voltage code

          aux_codes[current_ic][current_gpio +((gpio_reg-1)*GPIO_IN_REG)] = parsed_aux;
          data_counter=data_counter+2;                        //Because gpio voltage codes are two bytes the data counter
          //must increment by two for each parsed gpio voltage code

        }

        received_pec = (data[data_counter]<<8)+ data[data_counter+1];          //The received PEC for the current_ic is transmitted as the 7th and 8th
        //after the 6 gpio voltage data bytes
        data_pec = pec15_calc(BYT_IN_REG, &data[current_ic*NUM_RX_BYT]);
        if (received_pec != data_pec)
        {
          pec_error = -1;                             //The pec_error variable is simply set negative if any PEC errors
          //are detected in the received serial data
        }

        data_counter=data_counter+2;                        //Because the transmitted PEC code is 2 bytes long the data_counter
        //must be incremented by 2 bytes to point to the next ICs gpio voltage data
      }


    }

  }
  else
  {

    ltc6811_rdaux_reg(reg, total_ic, data, spi_channel);
    for (int current_ic = 0 ; current_ic < total_ic; current_ic++)            // executes for every ltc6811 in the daisy chain
    {
      // current_ic is used as an IC counter


      for (int current_gpio = 0; current_gpio<GPIO_IN_REG; current_gpio++)    // This loop parses the read back data. Loops
      {
        // once for each aux voltage in the register

        parsed_aux = (data[data_counter] + (data[data_counter+1]<<8));        //Each gpio codes is received as two bytes and is combined to
        // create the parsed gpio voltage code
        aux_codes[current_ic][current_gpio +((reg-1)*GPIO_IN_REG)] = parsed_aux;
        //aux_codes[0][ 0 + ((1 - 1) * 3)] = aux_codes[0][0] = parsed_aux
        //aux_codes[0][ 1 + ((1 - 1) * 3)] = aux_codes[0][1] = parsed_aux
        //aux_codes[0][ 2 + ((1 - 1) * 3)] = aux_codes[0][2] = parsed_aux
        data_counter=data_counter+2;                      //Because gpio voltage codes are two bytes the data counter
        //must increment by two for each parsed gpio voltage code
      }

      received_pec = (data[data_counter]<<8) + data[data_counter+1];         //The received PEC for the current_ic is transmitted as the 7th and 8th
      //after the 6 gpio voltage data bytes
      data_pec = pec15_calc(BYT_IN_REG, &data[current_ic*NUM_RX_BYT]);
      if (received_pec != data_pec)
      {
        pec_error = -1;                               //The pec_error variable is simply set negative if any PEC errors
        //are detected in the received serial data
      }

      data_counter=data_counter+2;                        //Because the transmitted PEC code is 2 bytes long the data_counter
      //must be incremented by 2 bytes to point to the next ICs gpio voltage data
    }
  }
  free(data);
  return (pec_error);
}


/*
 The function reads a single GPIO voltage register and stores thre read data
 in the *data point as a byte array. This function is rarely used outside of
 the ltc6811_rdaux() command.
 */
void ltc6811_rdaux_reg(uint8_t reg, //Determines which GPIO voltage register is read back
                       uint8_t total_ic, //The number of ICs in the system
                       uint8_t *data, //Array of the unparsed auxiliary codes
					   SPI_HandleTypeDef spi_channel)
{
  const uint8_t REG_LEN = 8; // number of bytes in the register + 2 bytes for the PEC
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

	  wakeup_idle (spi_channel); //This will guarantee that the ltc6811 isoSPI port is awake. This command can be removed.

	  outputCS(0);					//The Low state of the chip select pin is set
	  HAL_SPI_TransmitReceive(&spi_channel, cmd, data, (REG_LEN*total_ic),HAL_MAX_DELAY);
	  outputCS(1);					//The Low state of the chip select pin is set

}
