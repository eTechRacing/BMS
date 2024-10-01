/**
  ******************************************************************************
  * @file           : Definitions.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
	Author: Bernat Carnota
	Mail: bernatcarnota@gmail.com
	Tel: 611459840
	Date: 2024
  ******************************************************************************

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
#include "stm32f7xx_hal.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the
  *        HAL drivers code
  */

/* USER CODE BEGIN Private defines */
/*COMMANDS CODE BEGIN Private Defines*/
#define VCELLMAX 4.2						//Definition of the maximum voltage
#define VCELLMIN 3.2						//Definition of the minimum voltage

#define OVERVOLTAGE_TH (uint16_t)((VCELLMAX)/16/0.1)
#define UNDERVOLTAGE_TH (uint16_t)((((VCELLMIN)/0.1)/16)-1)

/*Definition of the Slave Balancing arrays*/
uint32_t SlBl1 [0,0,0,0,0,0,0,0,0,0,0,0];	//Definition of the balancing array for the Slave 1
uint32_t SlBl2 [0,0,0,0,0,0,0,0,0,0,0,0];	//Definition of the balancing array for the Slave 2
uint32_t SlBl3 [0,0,0,0,0,0,0,0,0,0,0,0];	//Definition of the balancing array for the Slave 3
uint32_t SlBl4 [0,0,0,0,0,0,0,0,0,0,0,0];	//Definition of the balancing array for the Slave 4
uint32_t SlBl5 [0,0,0,0,0,0,0,0,0,0,0,0];	//Definition of the balancing array for the Slave 5
uint32_t SlBl6 [0,0,0,0,0,0,0,0,0,0,0,0];	//Definition of the balancing array for the Slave 6
uint32_t SlBl7 [0,0,0,0,0,0,0,0,0,0,0,0];	//Definition of the balancing array for the Slave 7
uint32_t SlBl8 [0,0,0,0,0,0,0,0,0,0,0,0];	//Definition of the balancing array for the Slave 8
uint32_t SlBl9 [0,0,0,0,0,0,0,0,0,0,0,0];	//Definition of the balancing array for the Slave 9
uint32_t SlBl10 [0,0,0,0,0,0,0,0,0,0,0,0];	//Definition of the balancing array for the Slave 10
uint32_t SlBl11 [0,0,0,0,0,0,0,0,0,0,0,0];	//Definition of the balancing array for the Slave 11
uint32_t SlBl12 [0,0,0,0,0,0,0,0,0,0,0,0];	//Definition of the balancing array for the Slave 12

/*Definition of the Slave Balancing bits*/
#define SLBLBI1 0b000000000000
#define SLBLBI2 0b000000000000
#define SLBLBI3 0b000000000000
#define SLBLBI4 0b000000000000
#define SLBLBI5 0b000000000000
#define SLBLBI6 0b000000000000
#define SLBLBI7 0b000000000000
#define SLBLBI8 0b000000000000
#define SLBLBI9 0b000000000000
#define SLBLBI10 0b000000000000
#define SLBLBI11 0b000000000000
#define SLBLBI12 0b000000000000


uint32_t BalancingArray [SlBlBI1,SlBlBI2,SlBlBI3,SlBlBI4,SlBlBI5,SlBlBI6,SlBlBI7,SlBlBI8,SlBlBI9,SlBlBI10,SlBlBI11,SlBlBI12];


uint32_t OVERVOLTFLAG []
