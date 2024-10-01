/*
 * errors.h
 *
 *  Created on: Feb 21, 2024
 *      Author: Usuario
 */

#ifndef INC_ERRORS_H_
#define INC_ERRORS_H_

uint8_t OV_EVAL(uint16_t *VOLTATGES, uint32_t *FLAG_ARRAY);
uint8_t UV_EVAL(uint16_t *VOLTATGES, uint32_t *FLAG_ARRAY);
uint8_t OT_EVAL(uint16_t *TEMPERATURES, uint32_t *FLAG_ARRAY);
uint8_t UT_EVAL(uint16_t *TEMPERATURES, uint32_t *FLAG_ARRAY);
uint8_t ETAS_SYNC_EVAL(uint32_t *etas_flag_arr);
uint8_t CURR_SYNC_EVAL(uint32_t *curr_flag_arr);
uint8_t BALANCING_SYNC_EVAL(uint32_t *balancing_flag_arr);
uint8_t detection_BMSerror_OverCurrent(int32_t Current, uint32_t *FALG_ARRAY);
uint8_t slave_disc(uint8_t *PEC_SLAVES_Volt,uint8_t *PEC_SLAVES_Temp, uint32_t *FLAG_ARRAY_Volt,uint32_t *FLAG_ARRAY_Temp);
uint8_t current_error_eval(uint8_t *current_error,uint32_t *FLAG_ARRAY_Temp);

#endif /* INC_ERRORS_H_ */
