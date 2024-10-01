/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LTC6811-1_eTechRacing.h"
#include "eTechRacing.h"
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "can_comunication.h"
#include "BMS.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim13;
TIM_HandleTypeDef htim14;

/* USER CODE BEGIN PV */
//ERRORS---------------------------------------------------------------------------------
uint8_t BMS_OK = 1;
//uint8_t PEC_SLAVES_Temp[8*TOTAL_IC];
//uint8_t PEC_SLAVES_Volt[4*TOTAL_IC];
uint8_t PEC_SLAVES_Temp[120];
uint8_t PEC_SLAVES_Volt[60];


//Informació rebuda i calculada------------------------------------------------------------------
//uint16_t voltatjes[12*TOTAL_IC];
//uint16_t temperatures[8*TOTAL_IC];
//uint16_t temperatures_conv[8*TOTAL_IC];
uint16_t voltatjes[180];
uint16_t temperatures[120];
uint16_t temperatures_conv[120];
int32_t Current;
uint8_t curr_arr[4];
uint16_t min_volt;
uint16_t max_volt;
uint32_t sum_volt;
uint16_t min_temp;
uint16_t max_temp;
float mean_temp;
float mean_volt;

//FLAGS EVALUATIONS------------------------------------------------------------------------------------------
//uint32_t ov_flag_arr[12*TOTAL_IC];
//uint32_t uv_flag_arr[12*TOTAL_IC];
//uint32_t ot_flag_arr[8*TOTAL_IC];
//uint32_t sd_flag_arr_volt[4*TOTAL_IC];
//uint32_t sd_flag_arr_temp[8*TOTAL_IC];
uint32_t ov_flag_arr[180];
uint32_t uv_flag_arr[180];
uint32_t ot_flag_arr[120];
uint32_t sd_flag_arr_volt[60];
uint32_t sd_flag_arr_temp[120];
uint32_t oc_flag_arr[1];
uint32_t etas_flag[1];
uint32_t current_flag[1];
uint32_t current_sync_flag[1];
uint32_t can_balancing_flag[1];

uint8_t current_error[1] = {1};
uint8_t KEEP_ALIVE = 0;
uint8_t BMS_MONITORING = 0;
uint8_t IMD_MONITORING = 0;
uint8_t SDC_MONITORING = 0;
uint8_t AMS_LED = 1;
uint8_t IMD_LED = 1;



//FLAGS------------------------------------------------------------------------------------------
uint8_t OV_FLAG = 1;
uint8_t UV_FLAG = 1;
uint8_t OT_FLAG = 1;
uint8_t UT_FLAG = 0;				//Not evaluated
uint8_t OC_FLAG = 1;
uint8_t SD_FLAG = 1;
uint8_t CE_FLAG = 1;
uint8_t ETAS_SYNC_FLAG = 0;
uint8_t CURR_SYNC_FLAG = 1;
uint8_t CAN_BALANCING_FLAG = 0;

uint8_t Airs_State[4] = {0,0,0,0}; //0 airs state,1 air+ state,  2 air- state, 3precharge state

uint8_t msg = 0;

//Configuració-----------------------------------------------------------------------------------
uint8_t GPIOx = 0b00000011;
uint8_t CH = 0b00000000;
uint8_t BLC = 0;							// 0 if not balancing; 1 if balancing
uint8_t AIRs_request = 0;
uint8_t CURR_STATE = 1;

//CAN--------------------------------------------------------------------------------------------
CAN_RxHeaderTypeDef RxHeader;
CAN_TxHeaderTypeDef TxHeader;
CAN_FilterTypeDef canfil;
CAN_FilterTypeDef canfil2;
uint8_t RxData[30];
uint8_t TxData[30];

//INTERRUPCIO TIMER TEMPREATURES I VOLTATJES
uint16_t COBID_TEMP = 0x11A;
uint16_t COBID_VOLT = 0xEA;
uint8_t IC_TEMP = 0;
uint8_t IC_VOLT = 0;
uint8_t RX_VOLT_TEMP = 0;


uint8_t BMS_SELECTION = 0;
uint8_t val = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_CAN1_Init(void);
static void MX_TIM14_Init(void);
static void MX_CAN2_Init(void);
static void MX_TIM13_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_CAN1_Init();
  MX_TIM14_Init();
  MX_CAN2_Init();
  MX_TIM13_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //INICIALITZACIÓ DE CAN-------------------------------------------------------------------------

  CAN1_FILTER_INIT(&canfil);											//CAN 1 Filter
  CAN2_FILTER_INIT(&canfil2);											//CAN 2 Filter

  CAN1_CONFIG_INIT(canfil, hcan1);										//CAN 1 Configuration - Initialization
  CAN2_CONFIG_INIT(canfil2, hcan2);										//CAN 2 Configuration - Initialization

  if(HAL_TIM_Base_Start_IT(&htim14) != HAL_OK) Error_Handler();			//TIMER 14 Initialization
  if(HAL_TIM_Base_Start_IT(&htim13) != HAL_OK) Error_Handler();			//TIMER 13 Initialization

  HAL_GPIO_WritePin(BMS_RELAY_PORT, BMS_RELAY_PIN, GPIO_PIN_RESET);		//Close BMS relay

  outputCS(1);															//Set CS high
  while (1)
  {
//Voltage & Temperature lectures--------------------------------------------------------------------
	  ltc6811_rav(hspi1, PEC_SLAVES_Volt, voltatjes, GPIOx, CH, BLC, min_volt);	//Voltage lectures

//Voltage & Temperature lectures--------------------------------------------------------------------
	   ltc6811_rat(hspi1, PEC_SLAVES_Temp, voltatjes, temperatures, BLC, min_volt); //Temperature lectures
	   Conv_Temp(temperatures, temperatures_conv);									//Temperature lectures conversion
	   switch(BMS_SELECTION){
	   case 0:
		   //Calculation of min, max, avg volt, avg temp & current-----------------------------------------------------------
		   	   min_volt = Min_Volt(voltatjes);									//Minimum voltage calculation
		   	   max_volt = Max_Volt(voltatjes);									//Maximum voltage calculation
		   	   sum_volt = Sum_Volt(voltatjes);									//Sum of all voltages
		   	   mean_volt = Mean_Volt(voltatjes);								//Mean voltage calculation
		   	   min_temp = Min_Temp(temperatures_conv);							//Minimum temperature calculation
		   	   max_temp = Max_Temp(temperatures_conv);							//Maximum temperature calculation
		   	   mean_temp = Mean_Temp(temperatures_conv);						//Mean temperature calculation

		   //Errors evaluation---------------------------------------------------------------------------------------
		   	   OV_FLAG = OV_EVAL(voltatjes, ov_flag_arr);						//Over voltage error evaluation
		   	   UV_FLAG = UV_EVAL(voltatjes, uv_flag_arr);						//Under voltage error evaluation
		   	   OT_FLAG = OT_EVAL(temperatures_conv, ot_flag_arr);				//Over temperature error evaluation
		   	   OC_FLAG = detection_BMSerror_OverCurrent(Current, oc_flag_arr);	//Over current error evaluation
		   	   ETAS_SYNC_FLAG = ETAS_SYNC_EVAL(etas_flag);						//Evaluate if the ETAS is alive
		   	   CURR_SYNC_FLAG = CURR_SYNC_EVAL(current_sync_flag);				//Evaluate if the current sensor is alive
		   	   CAN_BALANCING_FLAG = BALANCING_SYNC_EVAL(can_balancing_flag);	//Evaluate the forced balancing sent by CANoe
		   	   SD_FLAG = slave_disc(PEC_SLAVES_Volt, PEC_SLAVES_Temp, sd_flag_arr_volt, sd_flag_arr_temp); //Evaluate the slave disconnection
		   	   CE_FLAG = current_error_eval(current_error, current_flag);		//Evaluate if there is a current error for more than 500ms
		   	   if(HAL_GetTick()>4500){
		   	   if(((OV_FLAG == 1) || (UV_FLAG == 1) || (OT_FLAG == 1) || (OC_FLAG == 1) ||(SD_FLAG == 1)||(CE_FLAG == 1)||(CURR_SYNC_FLAG == 1)) ) BMS_OK = 0;	}//If there is any error, BMS_OK = 0
			   BMS_MONITORING = HAL_GPIO_ReadPin(BMS_MONITORING_PORT, BMS_MONITORING_PIN)^1 ;	//Evaluate the BMS_Monitoring pin
			   IMD_MONITORING = HAL_GPIO_ReadPin(IMD_MONITORING_PORT, IMD_MONITORING_PIN)^1;	//Evaluate the IMD_Monitoring pin
			   SDC_MONITORING = HAL_GPIO_ReadPin(SDC_MONITORING_PORT, SDC_MONITORING_PIN)^1;	//Evaluate the SDC_Monitoring pin
			   AMS_LED = BMS_OK;
			   if(BMS_MONITORING==1 && IMD_MONITORING == 0){
				   IMD_LED = 0;
			   }
			   break;
	   case 1:
		   //Calculation of min, max, avg volt, avg temp & current-----------------------------------------------------------
		   	   min_volt = 36000;
		   	   max_volt = 39500;
		   	   sum_volt = 5880000;
		   	   mean_volt = 37500;
		   	   min_temp = 252;
		   	   max_temp = 263;
		   	   mean_temp = 256;

		   //Errors evaluation---------------------------------------------------------------------------------------
		   	   OV_FLAG = 0;
		   	   UV_FLAG = 0;
		   	   OT_FLAG = 0;
		   	   OC_FLAG = 0;
		   	   ETAS_SYNC_FLAG = ETAS_SYNC_EVAL(etas_flag);						//Evaluate if the ETAS is alive
		   	   CURR_SYNC_FLAG = CURR_SYNC_EVAL(current_sync_flag);				//Evaluate if the current sensor is alive
		   	   CAN_BALANCING_FLAG = BALANCING_SYNC_EVAL(can_balancing_flag);	//Evaluate the forced balancing sent by CANoe
		   	   SD_FLAG = slave_disc(PEC_SLAVES_Volt, PEC_SLAVES_Temp, sd_flag_arr_volt, sd_flag_arr_temp); //Evaluate the slave disconnection
		   	   CE_FLAG = current_error_eval(current_error, current_flag);		//Evaluate if there is a current error for more than 500ms
		   	 BMS_MONITORING = HAL_GPIO_ReadPin(BMS_MONITORING_PORT, BMS_MONITORING_PIN)^1 ;	//Evaluate the BMS_Monitoring pin
		   				   IMD_MONITORING = HAL_GPIO_ReadPin(IMD_MONITORING_PORT, IMD_MONITORING_PIN)^1;	//Evaluate the IMD_Monitoring pin
		   				   SDC_MONITORING = HAL_GPIO_ReadPin(SDC_MONITORING_PORT, SDC_MONITORING_PIN)^1;	//Evaluate the SDC_Monitoring pin
			   BMS_OK = 1;
			   AMS_LED = BMS_OK;
			   if(BMS_MONITORING==1 && IMD_MONITORING == 0){
				   IMD_LED = 0;
			   }
		   break;
	   }
//State Machine Evaluation------------------------------------------------------------------------------
	   CURR_STATE = StateMachineSteps(CURR_STATE, AIRs_request, BMS_OK, ETAS_SYNC_FLAG);	//Evaluate which is the current state of the charging process
	   BLC = StateMachineActuation(CURR_STATE, Airs_State, CAN_BALANCING_FLAG);				//Actuates according to the current state of the charging process controlling the airs and balancing
   /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 108;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 12;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_6TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief CAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */

  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 12;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_2TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_6TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */

  /* USER CODE END CAN2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM13 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM13_Init(void)
{

  /* USER CODE BEGIN TIM13_Init 0 */

  /* USER CODE END TIM13_Init 0 */

  /* USER CODE BEGIN TIM13_Init 1 */

  /* USER CODE END TIM13_Init 1 */
  htim13.Instance = TIM13;
  htim13.Init.Prescaler = 10-1;
  htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim13.Init.Period = 10800;
  htim13.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim13.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim13) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM13_Init 2 */

  /* USER CODE END TIM13_Init 2 */

}

/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 10-1;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 10000;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BMS_RELAY_GPIO_Port, BMS_RELAY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, USER_LED_Pin|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pins : BMS_MONITORING_Pin IMD_MONITORING_Pin */
  GPIO_InitStruct.Pin = BMS_MONITORING_Pin|IMD_MONITORING_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : BMS_RELAY_Pin */
  GPIO_InitStruct.Pin = BMS_RELAY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BMS_RELAY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SDC_Pin */
  GPIO_InitStruct.Pin = SDC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SDC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USER_LED_Pin PA3 PA5 PA6
                           PA7 */
  GPIO_InitStruct.Pin = USER_LED_Pin|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
 {
   if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK){
   }
   if(RxHeader.StdId == ETAS_SYNC){					//If the received message through CAN 1 has the ETAS_SYNC COBID
	   msg = 1;									//Start the CAN transmitting function
	   etas_flag[0] = HAL_GetTick();			//Actualize the etas_flag with the current tick
   }
   if(RxHeader.StdId == AIRS_REQUEST){			//If the received message through CAN 1 has the AIRS_REQUEST COBID
	   AIRs_request = RxData[0];				//Save the RxData which contains the charging state number
   }

   if(RxHeader.StdId == CURRENT){				//If the received message through CAN 1 has the CURRENT COBID
	   Current = Curr_EVAL(RxData);				//Extract the current value sent by CAN 1
	   current_sync_flag[0]=HAL_GetTick();		//Actualize the current_sync_flag with the current tick
	   current_error[0] = RxData[4];			//Saves the error data byte of the current value
   }


 }
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
 {
   if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, RxData) != HAL_OK){
   }
   if(RxHeader.StdId == DISPLAY_SYNC){			//If the received message through CAN 2 has the DISPLAY_SYNC COBID
	   RX_VOLT_TEMP = 1;						//Set the RX_VOLT_TEMP to 1 to start the Voltages and Temperatures transmission through CAN 2
   }
   if(RxHeader.StdId == CAN_FORCED_BALANCING){	//If the received message through CAN 2 has the CAN_FORCED_BALANCING COBID
	   can_balancing_flag[0] = HAL_GetTick();	//Actualize the can_balancing_flag with the current tick
   }
   if(RxHeader.StdId == CAN_BMS_SELECTION){	//If the received message through CAN 2 has the CAN_FORCED_BALANCING COBID
	   BMS_SELECTION = RxData[0] & 0x01;	//Actualize the can_balancing_flag with the current tick
	   TOTAL_IC = RxData[0]>>1;
	   val = RxData[0];
   }
   if(RxHeader.StdId == CAN_BMS_SELECTION){	//If the received message through CAN 2 has the CAN_FORCED_BALANCING COBID

      }
 }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 {
   if (htim->Instance == TIM14)					//When the TIMER 14 has a pulse, the function moves 1 iteration forward, up to 5 times
   {
  	 switch(msg){								//Depending on the msg state
   case 1:
  	 message_cantx_Voltage_State(hcan1, min_volt, max_volt, sum_volt, TxData, TxHeader); 		//Send the min, max and mean voltage values through CAN 1
  	 msg=2;																						//Set the msg state to 2
  	 break;
   case 2:
  	 message_cantx_Temperature_State(hcan1, min_temp, max_temp, mean_temp, TxData, TxHeader);	//Send the min, max and mean temperature values through CAN 1
  	 msg=3;																						//Set the msg state to 3
  	 break;
   case 3:
  	 message_cantx_AIR_State(hcan1,Airs_State, TxData, TxHeader);								//Send the Airs_State array through CAN 1, contains the current state of each AIR
  	 msg=4;																						//Set the msg state to 4
  	 break;
   case 4:
  	 message_cantx_Keep_Alive(hcan1, KEEP_ALIVE, TxData, TxHeader);								//Send the Keep_Alive of the master
  	 KEEP_ALIVE++;																				//Sum 1 to the Keep_alive value
  	 msg=5;																						//Set the msg state to 5
  	 break;
   case 5:
  	 message_cantx_Shutdown(hcan1, SDC_MONITORING, BMS_MONITORING, IMD_MONITORING, AMS_LED, IMD_LED,TxData, TxHeader);	//Send the SDC, BMS, IMD monitoring through CAN 1
  	 msg =0;																							//Set the msg state to 0
  	 break;
   }
}
   if (htim->Instance == TIM13){					//When the TIMER 14 has a pulse, the function moves 1 iteration forward, up to 5 times
	if((RX_VOLT_TEMP<=2*TOTAL_IC) && (RX_VOLT_TEMP > 0)){						//Enters the function if RX_VOLT_TEMP is between 1 and 2*TOTAL_IC, so that the odd ones will be the TEMP Rx and the pair ones will be the VOLT Rx
	   if((RX_VOLT_TEMP == 1) || (RX_VOLT_TEMP == 3) || (RX_VOLT_TEMP == 5) || (RX_VOLT_TEMP == 7) || (RX_VOLT_TEMP == 9) || (RX_VOLT_TEMP == 11) || (RX_VOLT_TEMP == 13) || (RX_VOLT_TEMP == 15) || (RX_VOLT_TEMP == 17) || (RX_VOLT_TEMP == 19) || (RX_VOLT_TEMP == 21) || (RX_VOLT_TEMP == 23) || (RX_VOLT_TEMP == 25) || (RX_VOLT_TEMP == 27) || (RX_VOLT_TEMP == 29)){
		   message_cantx_TEMPERATURES_IT(COBID_TEMP, IC_TEMP, hcan2, temperatures_conv, TxData, TxHeader); //Send all voltages of a slave through CAN 2, the slave iterates by summing 1 to the IC
		   IC_TEMP = IC_TEMP+1;													//Sum 1 to the IC
		   if(IC_TEMP<TOTAL_IC) COBID_TEMP += 2;
		   else IC_TEMP = 0, COBID_TEMP = 0x11A;
	   }

	   else{
		   message_cantx_VOLTATJES_IT(COBID_VOLT, IC_VOLT, hcan2, voltatjes, TxData, TxHeader);
		   uint8_t increments[15] = {3,2,3,3,2,3,3,2,3,3,2,3,3,2,3};
		   IC_VOLT = IC_VOLT+1;
		   if (IC_VOLT < TOTAL_IC) COBID_VOLT += increments[IC_VOLT-1];
		   else IC_VOLT = 0, COBID_VOLT = 0xEA;
	   }
	   RX_VOLT_TEMP += 1;

	   message_cantx_SF(hcan2, UT_FLAG, OT_FLAG, OV_FLAG, UV_FLAG, OC_FLAG, Airs_State[3], Airs_State[1], Airs_State[2], SDC_MONITORING, BMS_OK, SD_FLAG, CE_FLAG, UV_FLAG, BLC, TxData, TxHeader);
	}
	else RX_VOLT_TEMP = 0;
   }

}
/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
