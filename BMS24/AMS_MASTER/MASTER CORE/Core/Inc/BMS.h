/*
 * BMS.h
 *
 *  Created on: May 24, 2024
 *      Author: usuari
 */

#ifndef INC_BMS_H_
#define INC_BMS_H_

#define BMS_RELAY_PORT GPIOE
#define BMS_RELAY_PIN GPIO_PIN_3

#define AIRPLUS_PORT GPIOA
#define AIRPLUS_PIN GPIO_PIN_5

#define AIRMINUS_PORT GPIOA
#define AIRMINUS_PIN GPIO_PIN_7

#define PRECHARGE_PORT GPIOA
#define PRECHARGE_PIN GPIO_PIN_6

#define BMS_MONITORING_PORT GPIOE
#define BMS_MONITORING_PIN GPIO_PIN_2

#define IMD_MONITORING_PORT GPIOE
#define IMD_MONITORING_PIN GPIO_PIN_6

#define SDC_MONITORING_PORT GPIOC
#define SDC_MONITORING_PIN GPIO_PIN_15

#define CSPORT GPIOB
#define CSPIN GPIO_PIN_6

//Funcions
uint8_t StateMachineSteps(uint8_t run_cur_state, uint8_t AIRs_request, uint8_t BMS_OK,uint8_t ETAS_SYNC_FLAG);
uint8_t StateMachineActuation(uint8_t run_cur_state,uint8_t *airs_state, uint8_t CAN_BALANCING);



#endif /* INC_BMS_H_ */
