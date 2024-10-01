/*
 * can_comunication.h
 *
 *  Created on: Feb 10, 2024
 *      Author: Carmen Unió Cruz
 *
 *  This Header file contains the defined functions used for the
 *  Can Communication in the code for the Master BMS.
 */

#ifndef INC_CAN_COMUNICATION_H_
#define INC_CAN_COMUNICATION_H_

#include "main.h"

//COBIDS
#define DISPLAY_SYNC 0xE0
#define ETAS_SYNC 0x80
#define AIRS_REQUEST 0x90
#define SF 0xE1
#define CURRENT 0x3C2
#define CAN_FORCED_BALANCING 0x140
#define CAN_BMS_SELECTION 0x69

//INIT---------------------------------------------------------------------------------------------------
void CAN1_FILTER_INIT(CAN_FilterTypeDef *canfil);
void CAN2_FILTER_INIT(CAN_FilterTypeDef *canfil);
void CAN1_CONFIG_INIT(CAN_FilterTypeDef canfil, CAN_HandleTypeDef hcan);
void CAN2_CONFIG_INIT(CAN_FilterTypeDef canfil, CAN_HandleTypeDef hcan);
//TX-----------------------------------------------------------------------------------------------------
void message_cantx_TEMPERATURES_IT(uint16_t COBID, uint8_t IC, CAN_HandleTypeDef hcan2, uint16_t *temperatures, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader);
void message_cantx_VOLTATJES_IT(uint16_t COBID, uint8_t IC, CAN_HandleTypeDef hcan2, uint16_t *voltatjes, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader);
void message_cantx_SF(CAN_HandleTypeDef hcan, uint8_t UT_FLAG, uint8_t OT_FLAG, uint8_t OV_FLAG, uint8_t UV_FLAG, uint8_t OC_FLAG, uint8_t PRECHARGE_RELAY, uint8_t AIRPLUS, uint8_t AIRMINUS, uint8_t ACCU_INTERLOCK,uint8_t BMS_OK, uint8_t SLAVE_DISC, uint8_t CURRENT_DISC, uint8_t SHOULD_CHARGE, uint8_t IS_CHARGING, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader);
void message_cantx_AIR_State(CAN_HandleTypeDef hcan1, uint8_t *AIRs_State, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader);
void message_cantx_Temperature_State(CAN_HandleTypeDef hcan1, uint16_t Lowest_CellTemperature, uint16_t Highest_CellTemperature, uint16_t Average_CellTemperature, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader);
void message_cantx_Keep_Alive(CAN_HandleTypeDef hcan1, uint8_t Keep_Alive, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader);
void message_cantx_Shutdown(CAN_HandleTypeDef hcan1, unsigned char Shutdown_PackageIntck, unsigned char Shutdown_BMS, unsigned char Shutdown_IMD, unsigned char AMS_LED, unsigned char IMD_LED, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader);
void message_cantx_Voltage_State(CAN_HandleTypeDef hcan1,uint16_t Lowest_CellVoltage, uint16_t Highest_CellVoltage, uint32_t Sum_Of_All_Voltages,uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader);
//CONVERSIONS--------------------------------------------------------------------------------------------
uint8_t combined_bits(unsigned char bit1, unsigned char bit2, unsigned char bit3, unsigned char bit4, unsigned char bit5, unsigned char bit6, unsigned char bit7, unsigned char bit8);

#endif /* INC_CAN_COMUNICATION_H_ */
