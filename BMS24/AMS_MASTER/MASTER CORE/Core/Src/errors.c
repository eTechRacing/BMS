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
#include "errors.h"
#include "stdlib.h"
#include "LTC6811-1_eTechRacing.h"
#include "eTechRacing.h"


	uint8_t OV_EVAL(uint16_t *VOLTATGES, uint32_t *FLAG_ARRAY){
		uint8_t OV_FLAG = 0;
		for(int i = 0; i < TOTAL_IC; i++){
			for(int j = 0; j < 12; j++){
				 if((VOLTATGES[12*i+j] >= VCELLMAX)){
					  if ((HAL_GetTick() - FLAG_ARRAY[12*i+j]) >= ERROR_VOLT_TIME){
						  OV_FLAG = 1;
						  return OV_FLAG;
					  }
				  }
				  else{
					  FLAG_ARRAY[12*i+j] = HAL_GetTick();
					  OV_FLAG = 0;
				  }
			}
		  }
		return OV_FLAG;
	}

	 uint8_t UV_EVAL(uint16_t *VOLTATGES, uint32_t *FLAG_ARRAY){
		uint8_t UV_FLAG = 0;
		for(int i = 0; i < TOTAL_IC; i++){
			for(int j = 0; j < 12; j++){
				if((((j!=5) && (j!=11)) && ((i==0) ||(i==2) ||(i==3) ||(i==5) ||(i==6) ||(i==8) ||(i==9) ||(i==11) ||(i==12) ||(i==14))) || (((j!=4) && (j!=5) && (j!=10) && (j!=11)) && ((i==1) ||(i==4) ||(i==7) ||(i==10) ||(i==13)))){
				 if(VOLTATGES[12*i+j] >= VCELLMIN){
					 FLAG_ARRAY[12*i+j] = HAL_GetTick();
				 		UV_FLAG = 0;

					 }
				 else{
					 if ((HAL_GetTick() - FLAG_ARRAY[12*i+j]) >= ERROR_VOLT_TIME){
				 		UV_FLAG = 1;
				 		return UV_FLAG;
					 }
				 }
				}

				else{
					FLAG_ARRAY[12*i+j] = HAL_GetTick();
				}
			}
		  }
		return UV_FLAG;
	}



	uint8_t OT_EVAL(uint16_t *TEMPERATURES, uint32_t *FLAG_ARRAY){
			for(int i = 0; i <= 16; i ++){

					if (TEMPERATURES[i] <= 600){
						FLAG_ARRAY[i] = HAL_GetTick();
					}
					else {
						if ((HAL_GetTick() - FLAG_ARRAY[i]) >= 1000){
							return 1;
						}
					}
			}
			return 0;
		}

	uint8_t ETAS_SYNC_EVAL(uint32_t *etas_flag_arr){

		if((HAL_GetTick() - etas_flag_arr[0]) >= ETAS_SYNC_MAX_TIME){
			return 1;
		}
		return 0;
	}

	uint8_t CURR_SYNC_EVAL(uint32_t *curr_flag_arr){

		if((HAL_GetTick() - curr_flag_arr[0]) >= CURR_SYNC_MAX_TIME){
			return 1;
		}
		return 0;
	}

	uint8_t BALANCING_SYNC_EVAL(uint32_t *balancing_flag_arr){

		if((HAL_GetTick() - balancing_flag_arr[0]) >= CAN_BALANCING_SYNC_TIME){
			return 0;
		}
		return 1;
	}

	//Condició1: Si pasa de 136A durant 500ms -> Salta error
	//Condició2: Si en algun punt passa de 200A
	uint8_t detection_BMSerror_OverCurrent(int32_t Current, uint32_t *FLAG_ARRAY){				// Time of the Error Temperature
		if (Current >= MAXCURRENT){
			return 1;
		}
		if((Current >= HIGHCURRENT)){
			if (HAL_GetTick() - FLAG_ARRAY[0] > ERROR_CURRENT_TIME){
						return 1;
			}
		}
		 else {
			FLAG_ARRAY[0] = HAL_GetTick();
		}
		return 0;
	}

	uint8_t slave_disc(uint8_t *PEC_SLAVES_Volt,uint8_t *PEC_SLAVES_Temp, uint32_t *FLAG_ARRAY_Volt,uint32_t *FLAG_ARRAY_Temp){
		uint8_t flag = 0;
		for(int i = 0; i < 8*TOTAL_IC; i++){
			if(PEC_SLAVES_Temp[i] == 1){
				  if((HAL_GetTick() - FLAG_ARRAY_Temp[i]) >= SLAVE_DISC_TIME){
					  flag=1;
				  }
			}
				  else
					  FLAG_ARRAY_Temp[i] = HAL_GetTick();


		  }
		for(int i = 0; i < 4*TOTAL_IC; i++){
					if(PEC_SLAVES_Volt[i] == 1){
						  if((HAL_GetTick() - FLAG_ARRAY_Volt[i]) >= SLAVE_DISC_TIME){
							  flag = 1;
						  }
					}
					else
						FLAG_ARRAY_Volt[i] = HAL_GetTick();
				  }
		return flag;
	}

	uint8_t current_error_eval(uint8_t *current_error, uint32_t *FLAG_ARRAY){
			if(current_error[0] == 1){
				  if((HAL_GetTick() - FLAG_ARRAY[0]) >= SLAVE_DISC_TIME){
					  return 1;
				  }
			}
				  else
				  {
					  FLAG_ARRAY[0] = HAL_GetTick();
					  return 0;
				  }
			return 0;
	}
