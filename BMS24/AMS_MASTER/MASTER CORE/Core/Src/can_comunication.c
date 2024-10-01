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

#include "LTC6811-1_eTechRacing.h"
#include "eTechRacing.h"
#include "main.h"
#include "stdlib.h"
#include "can_comunication.h"
#include "errors.h"
#include <pgmspace.h>
#include "Linduino.h"
#include "BMS.h"
uint32_t TxMailbox;
/*
 * 	Function: filtercanconfig
 * 	Purpose: Define the configurations needed to recieve data.
 * 	Inputs: Tha canfil that has to be configured
 */

void CAN1_FILTER_INIT(CAN_FilterTypeDef *canfil){
	   canfil->FilterBank = 0;								// This refers to which filter is being configured. On this case is the filter number 0
	   canfil->FilterMode = CAN_FILTERMODE_IDMASK;			// FilterMode: How are we filtering the incoming messages. Only the messages that coincide with the mask and the filter are accepted
	   canfil->FilterFIFOAssignment = CAN_FILTER_FIFO0;		// Defines at which FIFO is this filter being configured to.
	   canfil->FilterIdHigh = 0x0000;						// MSB: Most Significant Bit. When it's in 0, accepts all the messages
	   canfil->FilterIdLow = 0x0000;						// LSB: Least Significant Bit. When it's in 0, accepts all the messages
	   canfil->FilterMaskIdHigh = 0x0000;					// Most Significant Bit of the mask. When it's in 0, accepts all the messages
	   canfil->FilterMaskIdLow = 0x0000;					// Least Significant Bit of the mask. When it's in 0, accepts all the messages
	   canfil->FilterScale = CAN_FILTERSCALE_32BIT;			// Defines the Filter Scale. (use the 32 bits)
	   canfil->FilterActivation = ENABLE;					// This activates the filter as it is enable
	   canfil->SlaveStartFilterBank = 14;					// Indicates the first filter slave number. In this case it is the principal filter.
}

void CAN2_FILTER_INIT(CAN_FilterTypeDef *canfil){
	   canfil->FilterBank = 14;								// This refers to which filter is being configured. On this case is the filter number 0
	   canfil->FilterMode = CAN_FILTERMODE_IDMASK;			// FilterMode: How are we filtering the incoming messages. Only the messages that coincide with the mask and the filter are accepted
	   canfil->FilterFIFOAssignment = CAN_FILTER_FIFO1;		// Defines at which FIFO is this filter being configured to.
	   canfil->FilterIdHigh = 0x0000;						// MSB: Most Significant Bit. When it's in 0, accepts all the messages
	   canfil->FilterIdLow = 0x0000;						// LSB: Least Significant Bit. When it's in 0, accepts all the messages
	   canfil->FilterMaskIdHigh = 0x0000;					// Most Significant Bit of the mask. When it's in 0, accepts all the messages
	   canfil->FilterMaskIdLow = 0x0000;					// Least Significant Bit of the mask. When it's in 0, accepts all the messages
	   canfil->FilterScale = CAN_FILTERSCALE_32BIT;			// Defines the Filter Scale. (use the 32 bits)
	   canfil->FilterActivation = ENABLE;					// This activates the filter as it is enable
	   canfil->SlaveStartFilterBank = 14;					// Indicates the first filter slave number. In this case it is the principal filter.
}

void CAN1_CONFIG_INIT(CAN_FilterTypeDef canfil, CAN_HandleTypeDef hcan){
    if(HAL_CAN_ConfigFilter(&hcan, &canfil) != HAL_OK) Error_Handler();
    if(HAL_CAN_Start(&hcan) != HAL_OK)Error_Handler();
    if(HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)Error_Handler();
}

void CAN2_CONFIG_INIT(CAN_FilterTypeDef canfil, CAN_HandleTypeDef hcan){
    if(HAL_CAN_ConfigFilter(&hcan, &canfil) != HAL_OK) Error_Handler();
    if(HAL_CAN_Start(&hcan) != HAL_OK)Error_Handler();
    if(HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)Error_Handler();
}

void message_cantx_TEMPERATURES_IT(uint16_t COBID, uint8_t IC, CAN_HandleTypeDef hcan2, uint16_t *temperatures, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader){
	TxHeader.DLC = 8;										//Number of bites to be transmitted max- 8. DLC: Data Length Code
	TxHeader.IDE = 0;										//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
	TxHeader.RTR = 0;										//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
	TxHeader.StdId = COBID;									//Standard identifier ID
	TxHeader.TransmitGlobalTime = DISABLE;					//A temporal mark in the CAN message is not added
	TxData[0] = temperatures[0+IC*8] >> 8;
	TxData[1] = temperatures[0+IC*8];
	TxData[2] = temperatures[1+IC*8] >> 8;
	TxData[3] = temperatures[1+IC*8];
	TxData[4] = temperatures[2+IC*8] >> 8;
	TxData[5] = temperatures[2+IC*8];
	TxData[6] = temperatures[3+IC*8] >> 8;
	TxData[7] = temperatures[3+IC*8];
	if (HAL_CAN_AddTxMessage(&hcan2, &TxHeader, TxData, &TxMailbox) != HAL_OK);
	COBID = COBID + 1;
	TxHeader.DLC = 8;										//Number of bites to be transmitted max- 8. DLC: Data Length Code
	TxHeader.IDE = 0;										//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
	TxHeader.RTR = 0;										//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
	TxHeader.StdId = COBID;									//Standard identifier ID
	TxHeader.TransmitGlobalTime = DISABLE;					//A temporal mark in the CAN message is not added
	TxData[0] = temperatures[4+IC*8] >> 8;
	TxData[1] = temperatures[4+IC*8];
	TxData[2] = temperatures[5+IC*8] >> 8;
	TxData[3] = temperatures[5+IC*8];
	TxData[4] = temperatures[6+IC*8] >> 8;
	TxData[5] = temperatures[6+IC*8];
	TxData[6] = temperatures[7+IC*8] >> 8;
	TxData[7] = temperatures[7+IC*8];
	if (HAL_CAN_AddTxMessage(&hcan2, &TxHeader, TxData, &TxMailbox) != HAL_OK);
}

void message_cantx_VOLTATJES_IT(uint16_t COBID, uint8_t IC, CAN_HandleTypeDef hcan2, uint16_t *voltatjes, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader){
	TxHeader.DLC = 8;										//Number of bites to be transmitted max- 8. DLC: Data Length Code
	TxHeader.IDE = 0;										//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
	TxHeader.RTR = 0;										//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
	TxHeader.StdId = COBID;									//Standard identifier ID
	TxHeader.TransmitGlobalTime = DISABLE;					//A temporal mark in the CAN message is not added
	TxData[0] = voltatjes[0+IC*12] >> 8;
	TxData[1] = voltatjes[0+IC*12];
	TxData[2] = voltatjes[1+IC*12] >> 8;
	TxData[3] = voltatjes[1+IC*12];
	TxData[4] = voltatjes[2+IC*12] >> 8;
	TxData[5] = voltatjes[2+IC*12];
	TxData[6] = voltatjes[3+IC*12] >> 8;
	TxData[7] = voltatjes[3+IC*12];
	if (HAL_CAN_AddTxMessage(&hcan2, &TxHeader, TxData, &TxMailbox) != HAL_OK);
	COBID = COBID + 1;
	if((IC == 0) || (IC == 2) || (IC == 3) || (IC == 5) || (IC == 6) || (IC == 8) || (IC == 9) || (IC == 11) || (IC == 12) || (IC == 14)){
	TxHeader.DLC = 8;										//Number of bites to be transmitted max- 8. DLC: Data Length Code
	TxHeader.IDE = 0;										//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
	TxHeader.RTR = 0;										//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
	TxHeader.StdId = COBID;									//Standard identifier ID
	TxHeader.TransmitGlobalTime = DISABLE;					//A temporal mark in the CAN message is not added
	TxData[0] = voltatjes[4+IC*12] >> 8;
	TxData[1] = voltatjes[4+IC*12];
	TxData[2] = voltatjes[6+IC*12] >> 8;
	TxData[3] = voltatjes[6+IC*12];
	TxData[4] = voltatjes[7+IC*12] >> 8;
	TxData[5] = voltatjes[7+IC*12];
	TxData[6] = voltatjes[8+IC*12] >> 8;
	TxData[7] = voltatjes[8+IC*12];
	if (HAL_CAN_AddTxMessage(&hcan2, &TxHeader, TxData, &TxMailbox) != HAL_OK);
	COBID = COBID + 1;
	TxHeader.DLC = 4;									//Number of bites to be transmitted max- 8. DLC: Data Length Code
	TxHeader.IDE = 0;									//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
	TxHeader.RTR = 0;									//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
	TxHeader.StdId = COBID;								//Standard identifier ID
	TxHeader.TransmitGlobalTime = DISABLE;				//A temporal mark in the CAN message is not added
	TxData[0] = voltatjes[9+IC*12] >> 8;
	TxData[1] = voltatjes[9+IC*12];
	TxData[2] = voltatjes[10+IC*12] >> 8;
	TxData[3] = voltatjes[10+IC*12];
	if (HAL_CAN_AddTxMessage(&hcan2, &TxHeader, TxData, &TxMailbox) != HAL_OK);
	}else{
	TxHeader.DLC = 8;									//Number of bites to be transmitted max- 8. DLC: Data Length Code
	TxHeader.IDE = 0;									//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
	TxHeader.RTR = 0;									//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
	TxHeader.StdId = COBID;								//Standard identifier ID
	TxHeader.TransmitGlobalTime = DISABLE;				//A temporal mark in the CAN message is not added
	TxData[0] = voltatjes[6+IC*12] >> 8;
	TxData[1] = voltatjes[6+IC*12];
	TxData[2] = voltatjes[7+IC*12] >> 8;
	TxData[3] = voltatjes[7+IC*12];
	TxData[4] = voltatjes[8+IC*12] >> 8;
	TxData[5] = voltatjes[8+IC*12];
	TxData[6] = voltatjes[9+IC*12] >> 8;
	TxData[7] = voltatjes[9+IC*12];
	if (HAL_CAN_AddTxMessage(&hcan2, &TxHeader, TxData, &TxMailbox) != HAL_OK);
	}
}

	void message_cantx_SF(CAN_HandleTypeDef hcan, uint8_t UT_FLAG, uint8_t OT_FLAG, uint8_t OV_FLAG, uint8_t UV_FLAG, uint8_t OC_FLAG, uint8_t PRECHARGE_RELAY, uint8_t AIRPLUS, uint8_t AIRMINUS, uint8_t ACCU_INTERLOCK,uint8_t BMS_OK, uint8_t SLAVE_DISC, uint8_t CURRENT_DISC, uint8_t SHOULD_CHARGE, uint8_t IS_CHARGING, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader){
		uint8_t missatge1 = combined_bits(UT_FLAG, OT_FLAG, OV_FLAG, UV_FLAG, OC_FLAG, PRECHARGE_RELAY, AIRPLUS, AIRMINUS);
		uint8_t missatge2 = combined_bits(ACCU_INTERLOCK, BMS_OK, SLAVE_DISC, CURRENT_DISC, SHOULD_CHARGE, IS_CHARGING, 0, 0);
		TxHeader.DLC = 2;									//Number of bites to be transmitted max- 8. DLC: Data Length Code
		TxHeader.IDE = 0;									//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
		TxHeader.RTR = 0;									//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
		TxHeader.StdId = SF;								//Standard identifier ID
		TxHeader.TransmitGlobalTime = DISABLE;				//A temporal mark in the CAN message is not added

		TxData[0] = missatge1;
		TxData[1] = missatge2;
		if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK);
	}

	void message_cantx_AIR_State(CAN_HandleTypeDef hcan1, uint8_t *AIRs_State, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader){
		TxHeader.DLC = 1; 									//Number of bites to be transmitted max- 8. DLC: Data Length Code
		TxHeader.IDE = 0;									//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
		TxHeader.RTR = 0;									//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
		TxHeader.StdId = 0x91;								//Standard identifier ID
		TxHeader.TransmitGlobalTime = DISABLE;				//A temporal mark in the CAN message is not added
		TxData[0] = AIRs_State[0];
		if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK);//Sent data. The TxData is the buffer where the data is saved
		}

	/*
	 * 	Function: message_cantx_Temperature_State
	 * 	Purpose: Take the inputs and variables needed and send it via CAN.
	 * 	Inputs: Lowest, Highest and Average Temperatures
	 */
	void message_cantx_Temperature_State(CAN_HandleTypeDef hcan1, uint16_t Lowest_CellTemperature, uint16_t Highest_CellTemperature, uint16_t Average_CellTemperature, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader){
		TxHeader.DLC = 6; 									//Number of bites to be transmitted max- 8. DLC: Data Length Code
		TxHeader.IDE = 0;									//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
		TxHeader.RTR = 0;									//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
		TxHeader.StdId = 0x93;								//Standard identifier ID
		TxHeader.TransmitGlobalTime = DISABLE;				//A temporal mark in the CAN message is not added
		TxData[0] = Lowest_CellTemperature >> 8;
		TxData[1] = Lowest_CellTemperature;
		TxData[2] = Highest_CellTemperature >> 8;
		TxData[3] = Highest_CellTemperature;
		TxData[4] = Average_CellTemperature >> 8;
		TxData[5] = Average_CellTemperature;
		if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK);
		}
	/*
	 * 	Function: message_cantx_Voltage_State
	 * 	Purpose: Take the inputs and variables needed and send it via CAN.
	 * 	Inputs: Lowest, Lowest and Average Cell Voltage
	 */

	void message_cantx_Voltage_State(CAN_HandleTypeDef hcan1,uint16_t Lowest_CellVoltage, uint16_t Highest_CellVoltage, uint32_t Sum_Of_All_Voltages,uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader){
		TxHeader.DLC = 7; 									//Number of bites to be transmitted max- 8. DLC: Data Length Code
		TxHeader.IDE = 0;									//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
		TxHeader.RTR = 0;									//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
		TxHeader.StdId = 0x92;								//Standard identifier ID
		TxHeader.TransmitGlobalTime = DISABLE;				//A temporal mark in the CAN message is not added
		TxData[0] = Lowest_CellVoltage >> 8;				//Sent data. The TxData is the buffer where the data is saved
		TxData[1] = Lowest_CellVoltage;
		TxData[2] = Highest_CellVoltage >> 8;
		TxData[3] = Highest_CellVoltage;
		TxData[4] = Sum_Of_All_Voltages >> 16;
		TxData[5] = Sum_Of_All_Voltages>> 8;
		TxData[6] = Sum_Of_All_Voltages;
		if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK);
						//Sent data. The TxData is the buffer where the data is saved
		}

	/*
	 * 	Function: message_cantx_Keep_Alive
	 * 	Purpose: Take the inputs and variables needed and send it via CAN.
	 * 	Inputs: Keep Alive
	 */

	void message_cantx_Keep_Alive(CAN_HandleTypeDef hcan1, uint8_t Keep_Alive, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader){
		TxHeader.DLC = 1; 									//Number of bites to be transmitted max- 8. DLC: Data Length Code
		TxHeader.IDE = 0;									//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
		TxHeader.RTR = 0;									//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
		TxHeader.StdId = 0xCA;								//Standard identifier ID
		TxHeader.TransmitGlobalTime = DISABLE;				//A temporal mark in the CAN message is not added
		TxData[0] = Keep_Alive;
		if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK); //Sent data. The TxData is the buffer where the data is saved
		}

	/*
	 * 	Function: message_cantx_Shutdown
	 * 	Purpose: Take the inputs and variables needed and send it via CAN.
	 * 	Inputs: Package Interlock, BMS (BMS_OK) and IMD -> Bits
	 */

	void message_cantx_Shutdown(CAN_HandleTypeDef hcan1, unsigned char Shutdown_PackageIntck, unsigned char Shutdown_BMS, unsigned char Shutdown_IMD, unsigned char AMS_LED, unsigned char IMD_LED, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader){
		TxHeader.DLC = 1; 									//Number of bites to be transmitted max- 8. DLC: Data Length Code
		TxHeader.IDE = 0;									//IDE: Identifier Extension. ID_STD: Standard Identifier. Dominant(0) = 11 bit ID, Recessive(1) = 29 bit ID
		TxHeader.RTR = 0;									//RTR: Remote Transmission Request, Dominant(0) = Data frame, Recessive (1) = Remote Frame. Type of trace
		TxHeader.StdId = 0x82;								//Standard identifier ID
		TxHeader.TransmitGlobalTime = DISABLE;				//A temporal mark in the CAN message is not added
		TxData[0] = (IMD_LED << 4 )|(AMS_LED << 3)|(Shutdown_IMD << 2) | (Shutdown_BMS << 1) | Shutdown_PackageIntck; 	//Sent data. The TxData is the buffer where the data is saved
					// Lectura de pinaje | Shutdown_BMS = BMS_OK | Lectura de pinaje
		if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK); //Sent data. The TxData is the buffer where the data is saved
	}

	uint8_t combined_bits(unsigned char bit1, unsigned char bit2, unsigned char bit3, unsigned char bit4,
						   unsigned char bit5, unsigned char bit6, unsigned char bit7, unsigned char bit8){

		uint8_t byte = (uint8_t)(bit8 << 7) | (uint8_t)(bit7 << 6) | (uint8_t)(bit6 << 5) | (uint8_t)(bit5 << 4) |
		                (uint8_t)(bit4 << 3) | (uint8_t)(bit3 << 2) | (uint8_t)(bit2 << 1) | (uint8_t)bit1;

		// Bit 1 is the Least Significant Bit and it goes up until Bit 8 which has the Most Significant Bit
		// Byte -> [Bit 8  Bit 7  Bit 6  Bit 5  Bit 4  Bit 3  Bit 2  Bit 1]

		return byte;
	}

