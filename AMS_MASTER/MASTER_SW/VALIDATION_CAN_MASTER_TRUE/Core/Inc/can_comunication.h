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


void filtercanconfig(CAN_HandleTypeDef *hcan, CAN_FilterTypeDef canfil, int CAN_FILTERMODE, int CAN_RX_FIFO, int CAN_FILTERSCALE);
void message_cantx_AIR_State(CAN_HandleTypeDef *hcan, int CAN_ID, int CAN_REMOTE_DATA, char AIRs_State, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader);
void message_cantx_Temperature_State(int CAN_ID, int CAN_REMOTE_DATA, uint16_t Lowest_CellTemperature, uint16_t Highest_CellTemperature, uint16_t Average_CellTemperature, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader);
void message_cantx_Charger_Parameters(int CAN_ID, int CAN_REMOTE_DATA, uint16_t Charger_MaxVoltage, uint8_t Charger_MaxCurrent, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader);
void message_cantx_Keep_Alive(int CAN_ID, int CAN_REMOTE_DATA, unsigned char Keep_Alive, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader);
void message_cantx_Shutdown(int CAN_ID, int CAN_REMOTE_DATA, unsigned char Shutdown_PackageIntck, unsigned char Shutdown_BMS, unsigned char Shutdown_IMD, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader);
void message_cantx_SoC_SoH(int CAN_ID, int CAN_REMOTE_DATA, CAN_HandleTypeDef *hcan, uint16_t SoC_Avg, uint16_t SoC_High, uint16_t SoC_Low, uint16_t SoH, uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader);
uint8_t combined_bits(unsigned char bit1, unsigned char bit2, unsigned char bit3, unsigned char bit4, unsigned char bit5, unsigned char bit6, unsigned char bit7, unsigned char bit8);
void message_cantx_Voltage_State(int CAN_ID, int CAN_REMOTE_DATA, CAN_HandleTypeDef *hcan, uint16_t Lowest_CellVoltage, uint16_t Highest_CellVoltage, uint16_t Average_CellVoltage);
void message_cantx_Voltage_State(int CAN_ID, int CAN_REMOTE_DATA, CAN_HandleTypeDef *hcan, uint16_t Lowest_CellVoltage, uint16_t Highest_CellVoltage, uint16_t Average_CellVoltage,uint8_t *TxData, CAN_TxHeaderTypeDef TxHeader);
void message_canrx_AIRs_Request(CAN_HandleTypeDef *hcan, uint8_t RxData, int AIRPLUS_PIN, int AIRPLUS_PORT, int AIRMINUS_PIN, int AIRMINUS_PORT, int PRECHARGE_PORT, int PRECHARGE_PIN);
void message_canrx_Current_Sensor(uint8_t RxData[5], unsigned char *Current_Error, uint32_t *Current_Value);
unsigned char uncombined_bytes(uint8_t Byte, unsigned char *byte_array);
void message_canrx_Syncronism(uint8_t RxData[1], unsigned char *Keep_Alive);


#endif /* INC_CAN_COMUNICATION_H_ */
