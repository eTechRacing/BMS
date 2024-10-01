/*
 * errors.h
 *
 *  Created on: Feb 21, 2024
 *      Author: Usuario
 */

#ifndef INC_ERRORS_H_
#define INC_ERRORS_H_

void flagmatrix_BMSerror_OverVoltage(uint16_t **VOLTAGE_ARRAY, int TOTAL_IC, int TOTAL_VOLTAGES, uint16_t **FLAGMATRIX_OV, int VOLTMAX,	unsigned char OK_BMS, int TIM_ERROR_VOLTAGE, unsigned char OV);
void detection_BMSerror_UnderVoltage(uint16_t **VOLTAGE_ARRAY, int TOTAL_IC, int TOTAL_VOLTAGES,uint16_t **FLAGMATRIX_UV, int VOLTMIN,unsigned char OK_BMS, int TIM_ERROR_VOLTAGE, unsigned char UV);
void detection_BMSerror_OverTemperature(uint16_t **TEMPERATURE_ARRAY, int TOTAL_IC, int TOTAL_AUX, uint16_t **FLAGMATRIX_OT, int TEMPHIGH, unsigned char OK_BMS, int TIM_ERROR_TEMP, unsigned char OT);
void detection_BMSerror_UnderTemperature(uint16_t **TEMPERATURE_ARRAY, int TOTAL_IC, int TOTAL_AUX, uint16_t **FLAGMATRIX_UT, int TEMPLOW, unsigned char OK_BMS,int TIM_ERROR_TEMP, unsigned char UT);
void detection_BMSerror_OverCurrent(uint16_t Current, int MAXCURRENT, int TIM_ERROR_TEMP);
void detection_BMSerror_SlaveDisconnection(void);


#endif /* INC_ERRORS_H_ */
