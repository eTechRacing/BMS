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

void CAN1_FILTER_INIT(FDCAN_FilterTypeDef *canfil){
	   canfil->IdType = FDCAN_STANDARD_ID;
	   canfil->FilterIndex = 0;
	   canfil->FilterType = FDCAN_FILTER_RANGE;
	   canfil->FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	   canfil->FilterID1 = 0x0000;
	   canfil->FilterID2 = 0x0000;
	   canfil->RxBufferIndex = 0;
}

void CAN2_FILTER_INIT(FDCAN_FilterTypeDef *canfil){
	   canfil->IdType = FDCAN_STANDARD_ID;
	   canfil->FilterIndex = 0;
	   canfil->FilterType = FDCAN_FILTER_RANGE;
	   canfil->FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
	   canfil->FilterID1 = 0x0000;
	   canfil->FilterID2 = 0x0000;
	   canfil->RxBufferIndex = 0;
}

void CAN1_CONFIG_INIT(FDCAN_FilterTypeDef canfil, FDCAN_HandleTypeDef hcan){
    if(HAL_FDCAN_ConfigFilter(&hcan, &canfil) != HAL_OK) Error_Handler();
    if(HAL_FDCAN_Start(&hcan) != HAL_OK)Error_Handler();
    if(HAL_FDCAN_ActivateNotification(&hcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)Error_Handler();
}

void CAN2_CONFIG_INIT(FDCAN_FilterTypeDef canfil, FDCAN_HandleTypeDef hcan){
    if(HAL_FDCAN_ConfigFilter(&hcan, &canfil) != HAL_OK) Error_Handler();
    if(HAL_FDCAN_Start(&hcan) != HAL_OK)Error_Handler();
    if(HAL_FDCAN_ActivateNotification(&hcan, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0) != HAL_OK)Error_Handler();
}

void message_cantx_TEMPERATURES_IT(uint16_t COBID, uint8_t IC, FDCAN_HandleTypeDef hcan2, uint16_t *temperatures, uint8_t *TxData, FDCAN_TxHeaderTypeDef TxHeader){
	TxHeader.Identifier = COBID;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;

	TxData[0] = temperatures[0+IC*8] >> 8;
	TxData[1] = temperatures[0+IC*8];
	TxData[2] = temperatures[1+IC*8] >> 8;
	TxData[3] = temperatures[1+IC*8];
	TxData[4] = temperatures[2+IC*8] >> 8;
	TxData[5] = temperatures[2+IC*8];
	TxData[6] = temperatures[3+IC*8] >> 8;
	TxData[7] = temperatures[3+IC*8];
	if (HAL_FDCAN_AddMessageToTxFifoQ(&hcan2, &TxHeader, TxData) != HAL_OK);
	COBID = COBID + 1;
	TxHeader.Identifier = COBID;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;
	TxData[0] = temperatures[4+IC*8] >> 8;
	TxData[1] = temperatures[4+IC*8];
	TxData[2] = temperatures[5+IC*8] >> 8;
	TxData[3] = temperatures[5+IC*8];
	TxData[4] = temperatures[6+IC*8] >> 8;
	TxData[5] = temperatures[6+IC*8];
	TxData[6] = temperatures[7+IC*8] >> 8;
	TxData[7] = temperatures[7+IC*8];
	if (HAL_FDCAN_AddMessageToTxFifoQ(&hcan2, &TxHeader, TxData) != HAL_OK);
}

void message_cantx_VOLTATJES_IT(uint16_t COBID, uint8_t IC, FDCAN_HandleTypeDef hcan2, uint16_t *voltatjes, uint8_t *TxData, FDCAN_TxHeaderTypeDef TxHeader){
	TxHeader.Identifier = COBID;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;
	TxData[0] = voltatjes[0+IC*12] >> 8;
	TxData[1] = voltatjes[0+IC*12];
	TxData[2] = voltatjes[1+IC*12] >> 8;
	TxData[3] = voltatjes[1+IC*12];
	TxData[4] = voltatjes[2+IC*12] >> 8;
	TxData[5] = voltatjes[2+IC*12];
	TxData[6] = voltatjes[3+IC*12] >> 8;
	TxData[7] = voltatjes[3+IC*12];
	if (HAL_FDCAN_AddMessageToTxFifoQ(&hcan2, &TxHeader, TxData) != HAL_OK);
	COBID = COBID + 1;
	if((IC == 0) || (IC == 2) || (IC == 3) || (IC == 5) || (IC == 6) || (IC == 8) || (IC == 9) || (IC == 11) || (IC == 12) || (IC == 14)){
	TxHeader.Identifier = COBID;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;
	TxData[0] = voltatjes[4+IC*12] >> 8;
	TxData[1] = voltatjes[4+IC*12];
	TxData[2] = voltatjes[6+IC*12] >> 8;
	TxData[3] = voltatjes[6+IC*12];
	TxData[4] = voltatjes[7+IC*12] >> 8;
	TxData[5] = voltatjes[7+IC*12];
	TxData[6] = voltatjes[8+IC*12] >> 8;
	TxData[7] = voltatjes[8+IC*12];
	if (HAL_FDCAN_AddMessageToTxFifoQ(&hcan2, &TxHeader, TxData) != HAL_OK);
	COBID = COBID + 1;
	TxHeader.Identifier = COBID;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_4;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;

	TxData[0] = voltatjes[9+IC*12] >> 8;
	TxData[1] = voltatjes[9+IC*12];
	TxData[2] = voltatjes[10+IC*12] >> 8;
	TxData[3] = voltatjes[10+IC*12];
	if (HAL_FDCAN_AddMessageToTxFifoQ(&hcan2, &TxHeader, TxData) != HAL_OK);
	}else{
	TxHeader.Identifier = COBID;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;
	TxData[0] = voltatjes[6+IC*12] >> 8;
	TxData[1] = voltatjes[6+IC*12];
	TxData[2] = voltatjes[7+IC*12] >> 8;
	TxData[3] = voltatjes[7+IC*12];
	TxData[4] = voltatjes[8+IC*12] >> 8;
	TxData[5] = voltatjes[8+IC*12];
	TxData[6] = voltatjes[9+IC*12] >> 8;
	TxData[7] = voltatjes[9+IC*12];
	if (HAL_FDCAN_AddMessageToTxFifoQ(&hcan2, &TxHeader, TxData) != HAL_OK);
	}
}

	void message_cantx_SF(FDCAN_HandleTypeDef hcan, uint8_t UT_FLAG, uint8_t OT_FLAG, uint8_t OV_FLAG, uint8_t UV_FLAG, uint8_t OC_FLAG, uint8_t PRECHARGE_RELAY, uint8_t AIRPLUS, uint8_t AIRMINUS, uint8_t ACCU_INTERLOCK,uint8_t BMS_OK, uint8_t SLAVE_DISC, uint8_t CURRENT_DISC, uint8_t SHOULD_CHARGE, uint8_t IS_CHARGING, uint8_t *TxData, FDCAN_TxHeaderTypeDef TxHeader){
		uint8_t missatge1 = combined_bits(UT_FLAG, OT_FLAG, OV_FLAG, UV_FLAG, OC_FLAG, PRECHARGE_RELAY, AIRPLUS, AIRMINUS);
		uint8_t missatge2 = combined_bits(ACCU_INTERLOCK, BMS_OK, SLAVE_DISC, CURRENT_DISC, SHOULD_CHARGE, IS_CHARGING, 0, 0);
		TxHeader.Identifier = SF;
		TxHeader.IdType = FDCAN_STANDARD_ID;
		TxHeader.TxFrameType = FDCAN_DATA_FRAME;
		TxHeader.DataLength = FDCAN_DLC_BYTES_2;
		TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
		TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
		TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
		TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
		TxHeader.MessageMarker = 0;
		TxData[0] = missatge1;
		TxData[1] = missatge2;
		if (HAL_FDCAN_AddMessageToTxFifoQ(&hcan, &TxHeader, TxData) != HAL_OK);
	}

	void message_cantx_AIR_State(FDCAN_HandleTypeDef hcan1, uint8_t *AIRs_State, uint8_t *TxData, FDCAN_TxHeaderTypeDef TxHeader){
		TxHeader.Identifier = 0x91;
		TxHeader.IdType = FDCAN_STANDARD_ID;
		TxHeader.TxFrameType = FDCAN_DATA_FRAME;
		TxHeader.DataLength = FDCAN_DLC_BYTES_1;
		TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
		TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
		TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
		TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
		TxHeader.MessageMarker = 0;
		TxData[0] = AIRs_State[0];
		if (HAL_FDCAN_AddMessageToTxFifoQ(&hcan1, &TxHeader, TxData) != HAL_OK);//Sent data. The TxData is the buffer where the data is saved
		}

	/*
	 * 	Function: message_cantx_Temperature_State
	 * 	Purpose: Take the inputs and variables needed and send it via CAN.
	 * 	Inputs: Lowest, Highest and Average Temperatures
	 */
	void message_cantx_Temperature_State(FDCAN_HandleTypeDef hcan1, uint16_t Lowest_CellTemperature, uint16_t Highest_CellTemperature, uint16_t Average_CellTemperature, uint8_t *TxData, FDCAN_TxHeaderTypeDef TxHeader){
		TxHeader.Identifier = 0x93;
		TxHeader.IdType = FDCAN_STANDARD_ID;
		TxHeader.TxFrameType = FDCAN_DATA_FRAME;
		TxHeader.DataLength = FDCAN_DLC_BYTES_6;
		TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
		TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
		TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
		TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
		TxHeader.MessageMarker = 0;
		TxData[0] = Lowest_CellTemperature >> 8;
		TxData[1] = Lowest_CellTemperature;
		TxData[2] = Highest_CellTemperature >> 8;
		TxData[3] = Highest_CellTemperature;
		TxData[4] = Average_CellTemperature >> 8;
		TxData[5] = Average_CellTemperature;
		if (HAL_FDCAN_AddMessageToTxFifoQ(&hcan1, &TxHeader, TxData) != HAL_OK);
		}
	/*
	 * 	Function: message_cantx_Voltage_State
	 * 	Purpose: Take the inputs and variables needed and send it via CAN.
	 * 	Inputs: Lowest, Lowest and Average Cell Voltage
	 */

	void message_cantx_Voltage_State(FDCAN_HandleTypeDef hcan1,uint16_t Lowest_CellVoltage, uint16_t Highest_CellVoltage, uint32_t Sum_Of_All_Voltages,uint8_t *TxData, FDCAN_TxHeaderTypeDef TxHeader){
		TxHeader.Identifier = 0x92;
		TxHeader.IdType = FDCAN_STANDARD_ID;
		TxHeader.TxFrameType = FDCAN_DATA_FRAME;
		TxHeader.DataLength = FDCAN_DLC_BYTES_7;
		TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
		TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
		TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
		TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
		TxHeader.MessageMarker = 0;
		TxData[0] = Lowest_CellVoltage >> 8;				//Sent data. The TxData is the buffer where the data is saved
		TxData[1] = Lowest_CellVoltage;
		TxData[2] = Highest_CellVoltage >> 8;
		TxData[3] = Highest_CellVoltage;
		TxData[4] = Sum_Of_All_Voltages >> 16;
		TxData[5] = Sum_Of_All_Voltages>> 8;
		TxData[6] = Sum_Of_All_Voltages;
		if (HAL_FDCAN_AddMessageToTxFifoQ(&hcan1, &TxHeader, TxData) != HAL_OK);
						//Sent data. The TxData is the buffer where the data is saved
		}

	/*
	 * 	Function: message_cantx_Keep_Alive
	 * 	Purpose: Take the inputs and variables needed and send it via CAN.
	 * 	Inputs: Keep Alive
	 */

	void message_cantx_Keep_Alive(FDCAN_HandleTypeDef hcan1, uint8_t Keep_Alive, uint8_t *TxData, FDCAN_TxHeaderTypeDef TxHeader){
		TxHeader.Identifier = 0xCA;
		TxHeader.IdType = FDCAN_STANDARD_ID;
		TxHeader.TxFrameType = FDCAN_DATA_FRAME;
		TxHeader.DataLength = FDCAN_DLC_BYTES_1;
		TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
		TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
		TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
		TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
		TxHeader.MessageMarker = 0;
		TxData[0] = Keep_Alive;
		if (HAL_FDCAN_AddMessageToTxFifoQ(&hcan1, &TxHeader, TxData) != HAL_OK); //Sent data. The TxData is the buffer where the data is saved
		}

	/*
	 * 	Function: message_cantx_Shutdown
	 * 	Purpose: Take the inputs and variables needed and send it via CAN.
	 * 	Inputs: Package Interlock, BMS (BMS_OK) and IMD -> Bits
	 */

	void message_cantx_Shutdown(FDCAN_HandleTypeDef hcan1, unsigned char Shutdown_PackageIntck, unsigned char Shutdown_BMS, unsigned char Shutdown_IMD, uint8_t *TxData, FDCAN_TxHeaderTypeDef TxHeader){
		TxHeader.Identifier = 0x82;
		TxHeader.IdType = FDCAN_STANDARD_ID;
		TxHeader.TxFrameType = FDCAN_DATA_FRAME;
		TxHeader.DataLength = FDCAN_DLC_BYTES_1;
		TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
		TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
		TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
		TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
		TxHeader.MessageMarker = 0;
		TxData[0] = (Shutdown_IMD << 2) | (Shutdown_BMS << 1) | Shutdown_PackageIntck; 	//Sent data. The TxData is the buffer where the data is saved
					// Lectura de pinaje | Shutdown_BMS = BMS_OK | Lectura de pinaje
		if (HAL_FDCAN_AddMessageToTxFifoQ(&hcan1, &TxHeader, TxData) != HAL_OK); //Sent data. The TxData is the buffer where the data is saved
	}

	uint8_t combined_bits(unsigned char bit1, unsigned char bit2, unsigned char bit3, unsigned char bit4,
						   unsigned char bit5, unsigned char bit6, unsigned char bit7, unsigned char bit8){

		uint8_t byte = (uint8_t)(bit8 << 7) | (uint8_t)(bit7 << 6) | (uint8_t)(bit6 << 5) | (uint8_t)(bit5 << 4) |
		                (uint8_t)(bit4 << 3) | (uint8_t)(bit3 << 2) | (uint8_t)(bit2 << 1) | (uint8_t)bit1;

		// Bit 1 is the Least Significant Bit and it goes up until Bit 8 which has the Most Significant Bit
		// Byte -> [Bit 8  Bit 7  Bit 6  Bit 5  Bit 4  Bit 3  Bit 2  Bit 1]

		return byte;
	}

