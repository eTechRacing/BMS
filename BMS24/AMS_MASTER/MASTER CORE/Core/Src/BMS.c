/*
 * BMS.c
 *
 *  Created on: May 24, 2024
 *      Author: usuari
 */
#include "main.h"
#include "stdlib.h"
#include "BMS.h"

//Varibales

//	ERROR_BMS = 0;
//	OPEN_AIRS = 1;
//	PRECHARGE = 2;
//	CL_AIRS = 3;
//	CH_PRECHARGE = 4;
//	CL_CHARGER = 5;

uint8_t StateMachineSteps(uint8_t run_cur_state, uint8_t AIRs_request, uint8_t BMS_OK,uint8_t ETAS_SYNC_FLAG){
	switch(run_cur_state){
		  case 1:		//OK
			  if(AIRs_request == 6){
				  run_cur_state = 2;
			  }
			  if(AIRs_request == 8){
				  run_cur_state = 4;
			  }
			  if(AIRs_request == 9){
				  run_cur_state = 5;
			  }
			  if(BMS_OK == 0){
				  run_cur_state = 0;
			  }
			  break;

		  case 2:		//PRECHARGE
			  if(AIRs_request == 0){
				  run_cur_state = 1;
			  }
			  if(AIRs_request == 3){
				  run_cur_state = 3;
			  }
			  if(AIRs_request == 8){
				  run_cur_state = 4;
			  }
			  if(ETAS_SYNC_FLAG == 1){
				  run_cur_state = 1;
			  }
			  if(BMS_OK == 0){
				  run_cur_state = 0;
			  }
			  break;

		  case 3:	//CLOSED_AIRS
			  if(AIRs_request == 0){
				  run_cur_state = 1;
			  }
			  if(ETAS_SYNC_FLAG == 1){
				  run_cur_state = 1;
			  }
			  if(BMS_OK == 0){
				  run_cur_state = 0;
			  }
			  break;

		  case 4:	//CHARGER_PRECHARGE
			  if(AIRs_request == 9){
				  run_cur_state = 5;
			  }
			  if(AIRs_request == 0){
				  run_cur_state = 1;
			  }
			  if(ETAS_SYNC_FLAG == 1){
				  run_cur_state = 1;
			  }
			  if(BMS_OK == 0){
				  run_cur_state = 0;
			  }
			  break;

		  case 5:		//CLOSED_CHARGER
			  if(AIRs_request == 0){
				  run_cur_state = 1;
			  }
			  if(ETAS_SYNC_FLAG == 1){
				  run_cur_state = 1;
			  }
			  if(BMS_OK == 0){
				  run_cur_state = 0;
			  }
			  break;
		  }
	return run_cur_state;
}

//airs_state = 0 -> OPEN_AIRS
//airs_state = 3 -> CLOSED_AIRS
//airs_state = 6 -> PRECHARGE
//airs_state = 8 -> PRECHARGE_CHARGER
//airs_state = 9 -> CLOSED_CHARGER

uint8_t StateMachineActuation(uint8_t run_cur_state,uint8_t *airs_state, uint8_t CAN_BALANCING){
	uint8_t BLC = 0;
	switch(run_cur_state){
		  case 0:			//Obrir rele BMS
			HAL_GPIO_WritePin(BMS_RELAY_PORT, BMS_RELAY_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AIRPLUS_PORT, AIRPLUS_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AIRMINUS_PORT, AIRMINUS_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PRECHARGE_PORT, PRECHARGE_PIN, GPIO_PIN_RESET);
			BLC = 0;
			airs_state[0] = 0;
			airs_state[1] = 0;
			airs_state[2] = 0;
			airs_state[3] = 0;
			break;
		  case 1:		//OPEN_AIRS: Obrir tots els relés
			HAL_GPIO_WritePin(BMS_RELAY_PORT, BMS_RELAY_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AIRPLUS_PORT, AIRPLUS_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AIRMINUS_PORT, AIRMINUS_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PRECHARGE_PORT, PRECHARGE_PIN, GPIO_PIN_RESET);
			BLC = 0;
			airs_state[0] = 0;
			airs_state[1] = 0;
			airs_state[2] = 0;
			airs_state[3] = 0;
			  break;
		  case 2:		//PRECHARGE: Tancar relé de precharge i AIRminus
			HAL_GPIO_WritePin(BMS_RELAY_PORT, BMS_RELAY_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AIRPLUS_PORT, AIRPLUS_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AIRMINUS_PORT, AIRMINUS_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PRECHARGE_PORT, PRECHARGE_PIN, GPIO_PIN_SET);
			BLC = 0;
			airs_state[0] = 6;
			airs_state[1] = 0;
			airs_state[2] = 1;
			airs_state[3] = 1;
			  break;
		  case 3:			//CLOSED_AIRS: Obrir relé de precharge i tancar AIRs
			HAL_GPIO_WritePin(BMS_RELAY_PORT, BMS_RELAY_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AIRPLUS_PORT, AIRPLUS_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AIRMINUS_PORT, AIRMINUS_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PRECHARGE_PORT, PRECHARGE_PIN, GPIO_PIN_RESET);
			BLC = 0;
			airs_state[0] = 3;
			airs_state[1] = 1;
			airs_state[2] = 1;
			airs_state[3] = 0;
			  break;
		  case 4:	//CHARGER_PRECHARGE: Tancar relé de precharge i AIRminus
			HAL_GPIO_WritePin(BMS_RELAY_PORT, BMS_RELAY_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AIRPLUS_PORT, AIRPLUS_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AIRMINUS_PORT, AIRMINUS_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PRECHARGE_PORT, PRECHARGE_PIN, GPIO_PIN_SET);
			BLC = 0;
			airs_state[0] = 6;
			airs_state[1] = 0;
			airs_state[2] = 1;
			airs_state[3] = 1;
			  break;
		  case 5:		//CLOSED_CHARGER: Obrir relé de precharge i tancar AIRs
			HAL_GPIO_WritePin(BMS_RELAY_PORT, BMS_RELAY_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AIRPLUS_PORT, AIRPLUS_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AIRMINUS_PORT, AIRMINUS_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PRECHARGE_PORT, PRECHARGE_PIN, GPIO_PIN_RESET);
			BLC = 0;
			airs_state[0] = 3;
			airs_state[1] = 1;
			airs_state[2] = 1;
			airs_state[3] = 0;
			  break;
		  }
	if(CAN_BALANCING == 1){
		BLC = 1;
	}
	return BLC;
}
