/*
 * can_comunication.c
 *
 *  Created on: Feb 10, 2024
 *      Author: Carmen Unió Cruz
 *
 *  This Source file contains functions used for the CAN Communication in the
 *  code for the Master BMS.
 *
 *  The function that start with "message_cantx" are the ones in charge of taking the various
 *  signals and variables and preparing the whole message and header for the CAN communication.
 *
 *  The functions that start with "message_canrx" are the ones in charge of taking the data recieved
 *   and adapting it to the thing needed
 */

	#include "main.h"
	#include "cmsis_os.h"
	#include "can_comunication.h"
	#include "errors.h"
	#include "defined_variables.h"
	#include "LTC6811-1_eTechRacing.h"
	#include "eTechRacing.h"

	// Filters for communication

	void filtercanconfig(CAN_HandleTypeDef *hcan){
		canfil.FilterBank = 0;								// This refers to which filter is being configured. On this case is the filter number 0
		canfil.FilterMode = CAN_FILTERMODE_IDMASK;			// FilterMode: How are we filtering the incoming messages. Only the messages that coincide with the mask and the filter are accepted
		canfil.FilterFIFOAssignment = CAN_RX_FIFO0;			// Defines at which FIFO is this filter being configured to. In this case is the FIFO 0.
		canfil.FilterIdHigh = 0x0000;						// MSB: Most Significant Bit. When it's in 0, accepts all the messages
		canfil.FilterIdLow = 0x0000;						// LSB: Least Significant Bit. When it's in 0, accepts all the messages
		canfil.FilterMaskIdHigh = 0x0000;					// Most Significant Bit of the mask. When it's in 0, accepts all the messages
		canfil.FilterMaskIdLow = 0x0000;					// Least Significant Bit of the mask. When it's in 0, accepts all the messages
		canfil.FilterScale = CAN_FILTERSCALE_32BIT;			// Defines the Filter Scale. It will use the 32 bits of the mask and the identifier
		canfil.FilterActivation = ENABLE;					// This activates the filter as it is enable
		canfil.SlaveStartFilterBank = 14;					// Indicates the first filter slave number. In this case it is the principal filter.
	}

	// TX CAN Functions

	void message_cantx_AIR_State(CAN_HandleTypeDef *hcan, char AIRs_State){

		TxHeader.DLC = 1; 									//Number of bites to be transmitted max- 8. DLC: Data Length Code
		TxHeader.IDE = CAN_ID_STD;							//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
		TxHeader.RTR = CAN_RTR_DATA;						//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
		TxHeader.StdId = 0x91;								//Standard identifier ID
		TxHeader.TransmitGlobalTime = DISABLE;				//A temporal mark in the CAN message is not added
		TxData[0] = AIRs_State;								//Sent data. The TxData is the buffer where the data is saved
		}

	void message_cantx_Temperature_State(CAN_HandleTypeDef *hcan, uint16_t Lowest_CellTemperature, uint16_t Highest_CellTemperature, uint16_t Average_CellTemperature){

		TxHeader.DLC = 6; 									//Number of bites to be transmitted max- 8. DLC: Data Length Code
		TxHeader.IDE = CAN_ID_STD;							//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
		TxHeader.RTR = CAN_RTR_DATA;						//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
		TxHeader.StdId = 0x93;								//Standard identifier ID
		TxHeader.TransmitGlobalTime = DISABLE;				//A temporal mark in the CAN message is not added
		TxData[0] = Lowest_CellTemperature;
		TxData[1] = Lowest_CellTemperature >> 8;
		TxData[2] = Highest_CellTemperature;
		TxData[3] = Highest_CellTemperature >> 8;
		TxData[4] = Average_CellTemperature;
		TxData[5] = Average_CellTemperature >> 8;
		}

	void message_cantx_Charger_Parameters(CAN_HandleTypeDef *hcan, uint16_t Charger_MaxVoltage, uint8_t Charger_MaxCurrent){

		TxHeader.DLC = 3; 									//Number of bites to be transmitted max- 8. DLC: Data Length Code
		TxHeader.IDE = CAN_ID_STD;							//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
		TxHeader.RTR = CAN_RTR_DATA;						//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
		TxHeader.StdId = 0x9D;								//Standard identifier ID
		TxHeader.TransmitGlobalTime = DISABLE;				//A temporal mark in the CAN message is not added
		TxData[0] = Charger_MaxVoltage;						//Sent data. The TxData is the buffer where the data is saved
		TxData[1] = Charger_MaxVoltage>>8;
		TxData[2] = Charger_MaxCurrent;
		}

	void message_cantx_Keep_Alive(CAN_HandleTypeDef *hcan, unsigned char Keep_Alive){

		TxHeader.DLC = 1; 									//Number of bites to be transmitted max- 8. DLC: Data Length Code
		TxHeader.IDE = CAN_ID_STD;							//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
		TxHeader.RTR = CAN_RTR_DATA;						//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
		TxHeader.StdId = 0xCA;								//Standard identifier ID
		TxHeader.TransmitGlobalTime = DISABLE;				//A temporal mark in the CAN message is not added
		TxData[0] = Keep_Alive;								//Sent data. The TxData is the buffer where the data is saved
		}

	void message_cantx_Shutdown(CAN_HandleTypeDef *hcan, unsigned char Shutdown_PackageIntck, unsigned char Shutdown_BMS, unsigned char Shutdown_IMD){

		TxHeader.DLC = 1; 									//Number of bites to be transmitted max- 8. DLC: Data Length Code
		TxHeader.IDE = CAN_ID_STD;							//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
		TxHeader.RTR = CAN_RTR_DATA;						//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
		TxHeader.StdId = 0x9D;								//Standard identifier ID
		TxHeader.TransmitGlobalTime = DISABLE;				//A temporal mark in the CAN message is not added
		TxData[0] = (Shutdown_IMD << 2) | (Shutdown_BMS << 1) | Shutdown_PackageIntck; 	//Sent data. The TxData is the buffer where the data is saved
		}			// Lectura de pinaje | Shutdown_BMS = BMS_OK | Lectura de pinaje

	void message_cantx_SoC_SoH(CAN_HandleTypeDef *hcan, uint16_t SoC_Avg, uint16_t SoC_High, uint16_t SoC_Low, uint16_t SoH){

		TxHeader.DLC = 8; 									//Number of bites to be transmitted max- 8. DLC: Data Length Code
		TxHeader.IDE = CAN_ID_STD;							//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
		TxHeader.RTR = CAN_RTR_DATA;						//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
		TxHeader.StdId = 0x95;								//Standard identifier ID
		TxHeader.TransmitGlobalTime = DISABLE;				//A temporal mark in the CAN message is not added
		TxData[0] = SoC_Avg;								//Sent data. The TxData is the buffer where the data is saved
		TxData[1] = SoC_Avg >> 8;							//Sent data. The TxData is the buffer where the data is saved
		TxData[2] = SoC_High;								//Sent data. The TxData is the buffer where the data is saved
		TxData[3] = SoC_High >> 8;							//Sent data. The TxData is the buffer where the data is saved
		TxData[4] = SoC_Low;								//Sent data. The TxData is the buffer where the data is saved
		TxData[5] = SoC_Low >> 8;							//Sent data. The TxData is the buffer where the data is saved
		TxData[6] = SoH;									//Sent data. The TxData is the buffer where the data is saved
		TxData[7] = SoH >> 8;
		}

	// This Function called "combined bits" takes different bits and puts them all together in a variable.
	uint8_t combined_bits(unsigned char bit1, unsigned char bit2, unsigned char bit3, unsigned char bit4,
						   unsigned char bit5, unsigned char bit6, unsigned char bit7, unsigned char bit8){

		uint8_t byte = (uint8_t)(bit8 << 7) | (uint8_t)(bit7 << 6) | (uint8_t)(bit6 << 5) | (uint8_t)(bit5 << 4) |
		                (uint8_t)(bit4 << 3) | (uint8_t)(bit3 << 2) | (uint8_t)(bit2 << 1) | (uint8_t)bit1;

		// Bit 1 is the Least Significant Bit and it goes up until Bit 8 which has the Most Significant Bit
		// Byte -> [Bit 8  Bit 7  Bit 6  Bit 5  Bit 4  Bit 3  Bit 2  Bit 1]

		return byte;
	}

	void message_cantx_StateAndFailReport(CAN_HandleTypeDef *hcan, unsigned char BMSerror_UnderTemperature, unsigned char BMSerror_OverTemperature,
											unsigned char BMSerror_OverVoltage, unsigned char BMSerror_UnderVoltage,
											unsigned char BMSerror_OverCurrent, unsigned char State_PrechargeRelay,
											unsigned char State_Air_Plus, unsigned char State_Air_Minus, unsigned char State_Balancing,
											unsigned char State_AccumulatorFans, unsigned char State_DivideCurrent,
											unsigned char BMSerror_SlaveDisconnection, unsigned char BMSerror_VoltageDisconnection,
											unsigned char BMSerror_NTCDisconnection){

		TxHeader.DLC = 8; 									//Number of bites to be transmitted max- 8. DLC: Data Length Code
		TxHeader.IDE = CAN_ID_STD;							//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
		TxHeader.RTR = CAN_RTR_DATA;						//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
		TxHeader.StdId = 0x94;								//Standard identifier ID
		TxHeader.TransmitGlobalTime = DISABLE;				//A temporal mark in the CAN message is not added
		TxData[0] = combined_bits(BMSerror_UnderTemperature, BMSerror_OverTemperature, BMSerror_OverVoltage, BMSerror_UnderVoltage, BMSerror_OverCurrent, State_PrechargeRelay, State_Air_Plus, State_Air_Minus);								//Sent data. The TxData is the buffer where the data is saved
		TxData[1] = combined_bits(State_Balancing, State_AccumulatorFans, State_DivideCurrent, BMSerror_SlaveDisconnection, BMSerror_VoltageDisconnection, BMSerror_NTCDisconnection, 0, 0);							//Sent data. The TxData is the buffer where the data is saved
		}

	void message_cantx_Voltage_State(CAN_HandleTypeDef *hcan, uint16_t Lowest_CellVoltage, uint16_t Highest_CellVoltage, uint16_t Average_CellVoltage){

		TxHeader.DLC = 6; 									//Number of bites to be transmitted max- 8. DLC: Data Length Code
		TxHeader.IDE = CAN_ID_STD;							//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
		TxHeader.RTR = CAN_RTR_DATA;						//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
		TxHeader.StdId = 0x92;								//Standard identifier ID
		TxHeader.TransmitGlobalTime = DISABLE;				//A temporal mark in the CAN message is not added
		TxData[0] = Lowest_CellVoltage;						//Sent data. The TxData is the buffer where the data is saved
		TxData[1] = Lowest_CellVoltage >> 8;
		TxData[2] = Highest_CellVoltage;
		TxData[3] = Highest_CellVoltage >> 8;
		TxData[4] = Average_CellVoltage;
		TxData[5] = Average_CellVoltage >> 8;
		}

	// RX Can Functions

	void message_canrx_AIRs_Request(CAN_HandleTypeDef *hcan, uint8_t RxData,
								int AIRPLUS_PIN, int AIRPLUS_PORT, int AIRMINUS_PIN,
								int AIRMINUS_PORT, int PRECHARGE_PORT, int PRECHARGE_PIN){
		if(RxData == 6){
			HAL_GPIO_WritePin(AIRPLUS_PORT, AIRPLUS_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AIRMINUS_PORT, AIRMINUS_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PRECHARGE_PORT, PRECHARGE_PIN, GPIO_PIN_SET);
		}
		if(RxData == 3){
			HAL_GPIO_WritePin(AIRPLUS_PORT, AIRPLUS_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AIRMINUS_PORT, AIRMINUS_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PRECHARGE_PORT, PRECHARGE_PIN, GPIO_PIN_RESET);
		}
		else{
			HAL_GPIO_WritePin(AIRPLUS_PORT, AIRPLUS_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AIRMINUS_PORT, AIRMINUS_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PRECHARGE_PORT, PRECHARGE_PIN, GPIO_PIN_RESET);
		}
	}

	void message_canrx_Current_Sensor(CAN_HandleTypeDef *hcan, uint8_t RxData){

	}
	// Extra functions

	// This Function called "uncombined bytes" takes a byte of data and separes each bit in different variables
	uint8_t uncombined_bytes(uint8_t Byte, unsigned char *byte_array){

			byte_array[0] = (Byte >> 7) & 0x01;
		    byte_array[1] = (Byte >> 6) & 0x01;
		    byte_array[2] = (Byte >> 5) & 0x01;
		    byte_array[3] = (Byte >> 4) & 0x01;
		    byte_array[4] = (Byte >> 3) & 0x01;
		    byte_array[5] = (Byte >> 2) & 0x01;
		    byte_array[6] = (Byte >> 1) & 0x01;
		    byte_array[7] = Byte & 0x01;

		    return *byte_array;

		}

	uint8_t data_to_byte(uint16_t Data,unsigned cha){

		}
