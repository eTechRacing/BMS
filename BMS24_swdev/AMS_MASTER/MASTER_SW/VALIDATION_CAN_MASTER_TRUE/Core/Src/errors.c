/*
 * errors.c
 *
 *  Created on: Feb 13, 2024
 *      Author: Carmen Unió Cruz
 *
 *
 *
 */
	#include "main.h"
	#include "cmsis_os.h"
	#include "can_comunication.h"
	#include "errors.h"
	#include "defined_variables.h"
	#include "LTC6811-1_eTechRacing.h"
	#include "eTechRacing.h"


	void flagmatrix_BMSerror_OverVoltage(uint16_t **VOLTAGE_ARRAY,				// Array that contains all of the Voltages. VOLTAGE_ARRAY[TOTAL_IC][TOTAL_VOLTAGES]
											int TOTAL_IC,						// Number of Slaves connected to the Master BMS.
											int TOTAL_VOLTAGES,					// Number of Cells per each Slave
											uint16_t **FLAGMATRIX_OV, 			// Matrix that contains all of the flags of the errors
											int VOLTMAX,						// Value of the Maximum Voltage
											unsigned char OK_BMS,				// The State of the BMS
											int TIM_ERROR_VOLTAGE,
											unsigned char OV){

		for(int i = 0; i <= TOTAL_IC; i ++){									// Analizes each slave
			for(int j = 0; j <= TOTAL_VOLTAGES; j ++){							// Analizes each cell in the slave
				if (VOLTAGE_ARRAY[i][j] >= VOLTMAX){							// Compares the cell voltage with the Voltage Max
					if ((HAL_GetTick() - FLAGMATRIX_OV[i][j]) >= TIM_ERROR_VOLTAGE)	// If thats true, calculates the difference between the time the error started and the actual
						OK_BMS = 0;												// If the difference is bigger is bigger or equal to 500ms -> BMS ERROR
					}
				else {															// If the voltage is lower than the Voltage Max
					FLAGMATRIX_OV[i][j] = HAL_GetTick() ;						// Actualizes the time
				}
			}
		}
	}

	void detection_BMSerror_UnderVoltage(uint16_t **VOLTAGE_ARRAY,				// Array that contains all of the Voltages. VOLTAGE_ARRAY[TOTAL_IC][TOTAL_VOLTAGES]
											int TOTAL_IC,						// Number of Slaves connected to the Master BMS.
											int TOTAL_VOLTAGES,					// Number of Cells per each Slave
											uint16_t **FLAGMATRIX_UV,			// Matrix that contains all of the flags of the errors
											int VOLTMIN,						// Value of the Maximum Voltage
											unsigned char OK_BMS,				// The State of the BMS
											int TIM_ERROR_VOLTAGE,
											unsigned char UV){
		for(int i = 0; i <= TOTAL_IC; i ++){									// Analizes each slave
			for(int j = 0; j <= TOTAL_VOLTAGES; j ++){							// Analizes each cell in the slave
				if ((VOLTAGE_ARRAY[i][j] <= VOLTMIN) && (j != (5||11) || (j != (4||10) && i != (1||4||7||10||13)))){		//If the Cell Voltage is lower than the Voltage Minimum and is not one of the cells that read 0
					if ((HAL_GetTick() - FLAGMATRIX_UV[i][j]) >= TIM_ERROR_VOLTAGE)	// If thats true, calculates the difference between the time the error started and the actual
						OK_BMS = 0;												// If the difference is bigger is bigger or equal to 500ms -> BMS ERROR
					}
				else {															// If the voltage is bigger than the Voltage Min
					FLAGMATRIX_UV[i][j] = HAL_GetTick() ;						// Actualizes the time
				}
			}
		}
	}


	void detection_BMSerror_OverTemperature(uint16_t **TEMPERATURE_ARRAY,
												int TOTAL_IC,
												int TOTAL_AUX,
												uint16_t **FLAGMATRIX_OT,
												int TEMPHIGH,
												unsigned char OK_BMS,				// The State of the BMS
												int TIM_ERROR_TEMP,
												unsigned char OT){
			for(int i = 0; i <= TOTAL_IC; i ++){
				for(int j = 0; j <= TOTAL_AUX; j ++){
					if ((TEMPERATURE_ARRAY[i][j] > TEMPHIGH) && (TEMPERATURE_ARRAY[i][j] != 0) ){
						if ((HAL_GetTick() - FLAGMATRIX_OT[i][j]) > TIM_ERROR_TEMP)
							OK_BMS = 0;
						}
					else {
						FLAGMATRIX_OT[i][j] = HAL_GetTick() ;
					}
				}
			}
		}

	void detection_BMSerror_UnderTemperature(uint16_t **TEMPERATURE_ARRAY,
												int TOTAL_IC,
												int TOTAL_AUX,
												uint16_t **FLAGMATRIX_UT,
												int TEMPLOW,
												unsigned char OK_BMS,				// The State of the BMS
												int TIM_ERROR_TEMP,
												unsigned char UT){				// Time of the Error Temperature

			for(int i = 0; i <= TOTAL_IC; i ++){
				for(int j = 0; j <= TOTAL_AUX; j ++){
					if ((TEMPERATURE_ARRAY[i][j] < TEMPLOW) && (TEMPERATURE_ARRAY[i][j] != 0) ){
						if ((HAL_GetTick() - FLAGMATRIX_UT[i][j]) > TIM_ERROR_TEMP)
								OK_BMS = 0;
						}
					else {
						FLAGMATRIX_UT[i][j] = HAL_GetTick() ;
					}
				}
			}
		}

	void detection_BMSerror_OverCurrent(uint16_t Current,
											int MAXCURRENT,
											// The State of the BMS
											int TIM_ERROR_TEMP){				// Time of the Error Temperature
		uint16_t FlagTime;
		if (Current >= MAXCURRENT){
			if (HAL_GetTick() - FlagTime > TIM_ERROR_TEMP){
				OK_BMS = 0;
			} else {

				FlagTime = HAL_GetTick();
			}
		}
	}


	void detection_BMSerror_SlaveDisconnection(void){;

				}

/*
	void detection_BMSerror_VoltageDisconnection(uint16_t **VOLTAGE_ARRAY,		//Voltage matrix
														int TOTAL_IC,
														int TOTAL_VOLTAGES,
														uint16_t **FLAGMATRIX_VD,
														unsigned char OK_BMS){

		for(int i = 0; i <= TOTAL_IC; i ++){
			for(int j = 0; j <= TOTAL_VOLTAGES; j ++){
				if ((VOLTAGE_ARRAY[i][j] == 0) && (j != (5||11) || (j != (4||10) && i != (1||4||7||10||13)))){
					if ((HAL_GetTick() - FLAGMATRIX_VD[i][j]) > 500)
						OK_BMS = 0;
					}
					else {
						FLAGMATRIX_VD[i][j] = HAL_GetTick() ;
						}
					}
				}
			}

	void detection_BMSerror_NTCDisconnection(uint16_t **TEMPERATURE_ARRAY,		//Voltage matrix
													int TOTAL_IC,
													int TOTAL_TEMPERATURES,
													uint16_t **FLAGMATRIX_NTCD,
													unsigned char OK_BMS){

		for(int i = 0; i <= TOTAL_IC; i ++){
			for(int j = 0; j <= TOTAL_TEMPERATURES; j ++){
				if (TEMPERATURE_ARRAY[i][j] == 0){
					if ((HAL_GetTick() - FLAGMATRIX_NTCD[i][j]) > 500)
						OK_BMS = 0;
					}
					else {
						FLAGMATRIX_NTCD[i][j] = HAL_GetTick() ;
						}
					}
				}
			}


*/
