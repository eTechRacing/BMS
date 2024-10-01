/*
 * defined_variables.h
 *
 *  Created on: Feb 17, 2024
 *      Author: Carmen Unió Cruz
 *     	Phone: +34 602 611 082
 *      Email: cuniocruz@gmail.com
 */

#ifndef INC_DEFINED_VARIABLES_H_
#define INC_DEFINED_VARIABLES_H_


	#include "main.h"
	#include "cmsis_os.h"
	#include "can_comunication.h"
	#include "errors.h"
	#include "defined_variables.h"
	#include "LTC6811-1_eTechRacing.h"
	#include "eTechRacing.h"

//--------------------------------------------------------------------------------------------------------
//				PINS
//		We define the specific pins for the Master BMS and their different uses
//--------------------------------------------------------------------------------------------------------

	#define BMS_ERROR 					//0 -> No hay Error ; 1 -> Hay error.
	#define BMS_ERROR_Port
	#define BMS_ERROR_Pin

	#define IMD_Monitoring
	#define IMD_Monitoring_Port
	#define IMD_Monitoring_Pin

	#define BMS_Monitoring
	#define BMS_Monitoring_Port
	#define BMS_Monitoring_Pin

	#define SDC_END_Monitoring
	#define SDC_END_Monitoring_Port
	#define SDC_END_Monitoring_Pin

	#define AIRPlus_Control
	#define AIRPlus_Control_Port
	#define AIRPlus_Control_Pin

	#define AIRMinus_Control
	#define AIRMinus_Control_Port
	#define AIRMinus_Control_Pin

	#define Precharge_Control
	#define Precharge_Control_Port
	#define Precharge_Control_Pin

	//#define OPEN_AIRS AIRs_State = 0
	//#define CLOSED_AIRS AIRs_State = 3
	//#define PRECHARGE AIRs_State = 6
	//#define PRECHARGE_CHARGER AIRs_State = 9
	//char AIRs_State;

// CAN VARIABLES -----------------------------------------------------------------------------------------

	CAN_RxHeaderTypeDef 	RxHeader; 					//CAN Bus Receive Header. This variable holds all the data needed for the CAN comunication.
	CAN_TxHeaderTypeDef 	TxHeader; 					//CAN Bus Transmit Header
	uint8_t 				TxData[8];  				//CAN Bus Transmit Buffer
	uint8_t 				RxData[8];  				//CAN Bus Receive Buffer
	CAN_FilterTypeDef 		canfil; 					//CAN Bus Filter
	uint16_t 				TxMailbox; 					//CAN Bus Mail box variable.

// GLOBAL VARIABLES --------------------------------------------------------------------------------------
	unsigned char BMS_OK; 								// BMS_OK defines the State of the BMS | 0 -> BMS Error ; 1 -> OK

//ACCUMULATOR VARIABLES ----------------------------------------------------------------------------------

	//Temperature
	uint16_t Lowest_CellTemperature;					//Lowest Cell Temperature
	uint16_t Highest_CellTemperature;					//Highest Cell Temperature
	uint16_t Average_CellTemperature;					//Average Cell Temperature

	//Charger
	uint16_t Charger_MaxVoltage;						//
	uint8_t Charger_MaxCurrent;

	// Keep Alive
	unsigned char Keep_Alive;

	// Shutdown
	unsigned char Shutdown_PackageIntck;
	unsigned char Shutdown_BMS;
	unsigned char Shutdown_IMD;

	// State of Charge & State of Health
	uint16_t SoC_Avg;
	uint16_t SoC_High;
	uint16_t SoC_Low;
	uint16_t SoH;

	// Errors
	unsigned char BMSerror_UnderTemperature;
	uint16_t FlagMatrix_UnderTemperature[ICn][CELL_CHANNELS];
	#define TEMPMAX 45
	#define TEMPMIN 30
	unsigned char BMSerror_OverTemperature;
	uint16_t FlagMatrix_OverTemperature[ICn][CELL_CHANNELS];
	#define TIMERRORTEMP 10000
	unsigned char BMSerror_OverVoltage;
	uint16_t FlagMatrix_OverVoltage[ICn][CELL_CHANNELS];
	unsigned char BMSerror_UnderVoltage;
	uint16_t FlagMatrix_UnderVoltage[ICn][CELL_CHANNELS];
	unsigned char BMSerror_OverCurrent;
	#define TIMERRORVOLT 500
	#define TIMERRORCURR 500
	#define OVERCURRENT 10000
	unsigned char State_PrechargeRelay;
	unsigned char State_Air_Plus;
	unsigned char State_Air_Minus;
	unsigned char State_Balancing;
	unsigned char State_AccumulatorFans;
	unsigned char State_DivideCurrent;
	unsigned char BMSerror_SlaveDisconnection;
	unsigned char BMSerror_VoltageDisconnection;
	#define TIMERRORDISC 500
	uint16_t FlagMatrix_VoltageDisconnection[ICn][CELL_CHANNELS];
	unsigned char BMSerror_NTCDisconnection;
	#define TEMPn  8
	uint16_t FlagMatrix_NTCDisconnection[ICn][TEMPn];





	// Voltage
	uint16_t Lowest_CellVoltage;
	uint16_t Highest_CellVoltage;
	uint16_t Average_CellVoltage;


#endif /* INC_DEFINED_VARIABLES_H_ */
