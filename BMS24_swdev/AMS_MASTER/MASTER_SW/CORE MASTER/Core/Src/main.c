/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
ADC_HandleTypeDef hadc1;

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

SPI_HandleTypeDef hspi4;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart4;

/* Definitions for Lectures_Task */
osThreadId_t Lectures_TaskHandle;
const osThreadAttr_t Lectures_Task_attributes = {
  .name = "Lectures_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SOC_Task */
osThreadId_t SOC_TaskHandle;
const osThreadAttr_t SOC_Task_attributes = {
  .name = "SOC_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Calc_Volt_Task */
osThreadId_t Calc_Volt_TaskHandle;
const osThreadAttr_t Calc_Volt_Task_attributes = {
  .name = "Calc_Volt_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Calc_Temp_Task */
osThreadId_t Calc_Temp_TaskHandle;
const osThreadAttr_t Calc_Temp_Task_attributes = {
  .name = "Calc_Temp_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for CANTX_Lectures_ */
osThreadId_t CANTX_Lectures_Handle;
const osThreadAttr_t CANTX_Lectures__attributes = {
  .name = "CANTX_Lectures_",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for CANRX_Current_T */
osThreadId_t CANRX_Current_THandle;
const osThreadAttr_t CANRX_Current_T_attributes = {
  .name = "CANRX_Current_T",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for CANRX_AIR_Task */
osThreadId_t CANRX_AIR_TaskHandle;
const osThreadAttr_t CANRX_AIR_Task_attributes = {
  .name = "CANRX_AIR_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Reading_Task */
osThreadId_t Reading_TaskHandle;
const osThreadAttr_t Reading_Task_attributes = {
  .name = "Reading_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for CANTX_BMSOK_Tas */
osThreadId_t CANTX_BMSOK_TasHandle;
const osThreadAttr_t CANTX_BMSOK_Tas_attributes = {
  .name = "CANTX_BMSOK_Tas",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for CANTX_VolTemTas */
osThreadId_t CANTX_VolTemTasHandle;
const osThreadAttr_t CANTX_VolTemTas_attributes = {
  .name = "CANTX_VolTemTas",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Error_Volt_Task */
osThreadId_t Error_Volt_TaskHandle;
const osThreadAttr_t Error_Volt_Task_attributes = {
  .name = "Error_Volt_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Error_Temp_Task */
osThreadId_t Error_Temp_TaskHandle;
const osThreadAttr_t Error_Temp_Task_attributes = {
  .name = "Error_Temp_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Error_Curr_Task */
osThreadId_t Error_Curr_TaskHandle;
const osThreadAttr_t Error_Curr_Task_attributes = {
  .name = "Error_Curr_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Error_Slav_Task */
osThreadId_t Error_Slav_TaskHandle;
const osThreadAttr_t Error_Slav_Task_attributes = {
  .name = "Error_Slav_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_UART4_Init(void);
static void MX_SPI4_Init(void);
static void MX_CAN1_Init(void);
static void MX_CAN2_Init(void);
void Lectures_VoltTemp(void *argument);
void SOC_CALC_Task(void *argument);
void Volt_Task(void *argument);
void Temp_Task(void *argument);
void CANTX_Task(void *argument);
void CANRXCurrentTask(void *argument);
void AIR_Task(void *argument);
void ReadingTask(void *argument);
void BMSOK_Task(void *argument);
void CANTX_Volt_Temp(void *argument);
void Error_Volt(void *argument);
void Error_Temp(void *argument);
void Error_Curr(void *argument);
void Error_Slaves(void *argument);

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
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_UART4_Init();
  MX_SPI4_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Lectures_Task */
  Lectures_TaskHandle = osThreadNew(Lectures_VoltTemp, NULL, &Lectures_Task_attributes);

  /* creation of SOC_Task */
  SOC_TaskHandle = osThreadNew(SOC_CALC_Task, NULL, &SOC_Task_attributes);

  /* creation of Calc_Volt_Task */
  Calc_Volt_TaskHandle = osThreadNew(Volt_Task, NULL, &Calc_Volt_Task_attributes);

  /* creation of Calc_Temp_Task */
  Calc_Temp_TaskHandle = osThreadNew(Temp_Task, NULL, &Calc_Temp_Task_attributes);

  /* creation of CANTX_Lectures_ */
  CANTX_Lectures_Handle = osThreadNew(CANTX_Task, NULL, &CANTX_Lectures__attributes);

  /* creation of CANRX_Current_T */
  CANRX_Current_THandle = osThreadNew(CANRXCurrentTask, NULL, &CANRX_Current_T_attributes);

  /* creation of CANRX_AIR_Task */
  CANRX_AIR_TaskHandle = osThreadNew(AIR_Task, NULL, &CANRX_AIR_Task_attributes);

  /* creation of Reading_Task */
  Reading_TaskHandle = osThreadNew(ReadingTask, NULL, &Reading_Task_attributes);

  /* creation of CANTX_BMSOK_Tas */
  CANTX_BMSOK_TasHandle = osThreadNew(BMSOK_Task, NULL, &CANTX_BMSOK_Tas_attributes);

  /* creation of CANTX_VolTemTas */
  CANTX_VolTemTasHandle = osThreadNew(CANTX_Volt_Temp, NULL, &CANTX_VolTemTas_attributes);

  /* creation of Error_Volt_Task */
  Error_Volt_TaskHandle = osThreadNew(Error_Volt, NULL, &Error_Volt_Task_attributes);

  /* creation of Error_Temp_Task */
  Error_Temp_TaskHandle = osThreadNew(Error_Temp, NULL, &Error_Temp_Task_attributes);

  /* creation of Error_Curr_Task */
  Error_Curr_TaskHandle = osThreadNew(Error_Curr, NULL, &Error_Curr_Task_attributes);

  /* creation of Error_Slav_Task */
  Error_Slav_TaskHandle = osThreadNew(Error_Slaves, NULL, &Error_Slav_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 10;
  RCC_OscInitStruct.PLL.PLLN = 216;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_VBAT;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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
  hcan1.Init.Prescaler = 16;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_3TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_3TQ;
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
  hcan2.Init.Prescaler = 16;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_3TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_1TQ;
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
  * @brief SPI4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI4_Init(void)
{

  /* USER CODE BEGIN SPI4_Init 0 */

  /* USER CODE END SPI4_Init 0 */

  /* USER CODE BEGIN SPI4_Init 1 */

  /* USER CODE END SPI4_Init 1 */
  /* SPI4 parameter configuration*/
  hspi4.Instance = SPI4;
  hspi4.Init.Mode = SPI_MODE_MASTER;
  hspi4.Init.Direction = SPI_DIRECTION_2LINES;
  hspi4.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi4.Init.NSS = SPI_NSS_SOFT;
  hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi4.Init.CRCPolynomial = 7;
  hspi4.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi4.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI4_Init 2 */

  /* USER CODE END SPI4_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LTC6820_CS_GPIO_Port, LTC6820_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LTC6820_CS_Pin */
  GPIO_InitStruct.Pin = LTC6820_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LTC6820_CS_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_Lectures_VoltTemp */
/**
  * @brief  Function implementing the Lectures_Task thread.
  * @param  argument: Not used
  * @retval None
  */

/* USER CODE END Header_Lectures_VoltTemp */
void Lectures_VoltTemp(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
	  /////////////////////////////////////////////////////WRCFG//////////////////////////////////////////////////////////////////////
	  /*
	  uint8_t config[][6] is a two dimensional array of the configuration data that will be written, the array should contain the 6 bytes for each

	   IC in the daisy chain. The lowest IC in the daisy chain should be the first 6 byte block in the array. The array should
	   have the following format:
	   |  config[0][0]| config[0][1] |  config[0][2]|  config[0][3]|  config[0][4]|  config[0][5]| config[1][0] |  config[1][1]|  config[1][2]|  .....    |
	   |--------------|--------------|--------------|--------------|--------------|--------------|--------------|--------------|--------------|-----------|
	   |IC1 CFGR0     |IC1 CFGR1     |IC1 CFGR2     |IC1 CFGR3     |IC1 CFGR4     |IC1 CFGR5     |IC2 CFGR0     |IC2 CFGR1     | IC2 CFGR2    |  .....    |

	   The function will calculate the needed PEC codes for the write data
	   and then transmit data to the ICs on a daisy chain.
	   */
	char uint8_t config[][6] = [[11111111,11111111,11111111,11111111,11111111,11111111], //IC1 //CFGR: Configuration Register
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC2
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC3
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC4
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC5
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC6
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC7
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC8
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC9
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC10
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC11
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC12
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC13
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC14
								[11111111,11111111,11111111,11111111,11111111,11111111]];//IC15
	//						 	  CFGR0		CFGR1	 CFGR2	  CFGR3    CFGR4    CFGR5

	//Register	| Bit 7	| Bit 6	| Bit 5	| Bit 4	| Bit 3	| Bit 2	| Bit 1	| Bit 0	| //DTEN -> Discharge Timer Enable (Read only): 1= Enables the Discharge Timer for Discharge Switches 0=Disables Discharge Timer
	//CFGR0 	| GPIO5	| GPIO4	| GPIO3	| GPIO2	| GPIO1	| REFON	| DTEN	| ADCOPT=0| //GPIO -> General Purpose Input/Output
	//CFGR1 	| VUV[7]| VUV[6]| VUV[5]| VUV[4]| VUV[3]| VUV[2]| VUV[1]| VUV[0]| //REFON -> References powered up: 1=References Remain Powered Up Until Watchdog Timeout 0=References Shut Down After Conversions (Default)
	//CFGR2 	| VOV[3]| VOV[2]| VOV[1]| VOV[0]|VUV[11]|VUV[10]| VUV[9]| VUV[8]| //VUV -> Undervoltage Comparison Voltage: Comparison Voltage = (VUV+1)*16*100uV; Defalut VUV = 0x000
	//CFGR3 	|VOV[11]|VOV[10]| VOV[9]| VOV[8]| VOV[7]| VOV[6]| VOV[5]| VOV[4]| //VOV -> Overvoltage Comparison Voltage: Comparison Voltage = VOV*16*100uV; Defalut VOV = 0x000
	//CFGR4 	| DCC8	| DCC7	| DCC6	| DCC5	| DCC4	| DCC3	| DCC2	| DCC1	| //DCC -> Discharge Cell X: X->[1,12] 1=Turn ON Shorting Switch for Cell x 0=Turn OFF Shorting Switch for Cell x (Default)
	//CFGR5 	|DCTO[3]|DCTO[2]|DCTO[1]|DCTO[0]| DCC12	| DCC11	| DCC10	| DCC9	| //DCTO -> Discharge Time Out Value
																				  //ADCOPT -> ADC Mode Option Bite: 0=Selects Modes 27kHz, 7kHz, 422Hz or 26Hz with MD[1:0] Bits in ADC Conversion Commands (Default) 1=Selects Modes 14kHz, 3kHz, 1kHz or 2kHz with MD[1:0] Bits in ADC Conversion Commands

	/*INIZIALIZATION PARAMETERS*/
	#define VCELLMAX 4000
	#define VCELLMIN 3000
	#define REFON 0x1 //Reference remains powered up until watchdog timeout
	#define DTEN 0x1 //Enable the discharge timer for discharge switches
	#define ADCOPT 0x0 //27k,7k,422 and 26 ADC frequencies
	#define DCCx_INIT 0x000 //Initially, deactivate DCC
	#define DCTO 0xC//Discharge Time Out Value to 60 min (0xC) --> SI DTEN = 1, DCTO serà el temps que tardarà el xip en adormir-se

	// Here avoided divions like 0.1 that is floating point for the compiler use directly integer (is faster though)
	#define OVERVOLTAGE_TH (uint16_t)(((VCELLMAX * 10) / (16)) / 10)
	#define UNDERVOLTAGE_TH (uint16_t)((((VCELLMIN * 10) / (16)) / 10) - 1)

	uint8_t GPIOx = 0b00001011; //Suma 001 al final per anar variant -> La configuració dels GPIOs poden ser valors entre 0,1,2,3,4,5,6,7
	// 0 -> 0b000 000 11 | 1 -> 0b000 001 11 | 2 -> 0b000 010 11 | 3 -> 0b000 011 11 | 4 -> 0b000 100 11 | 5 -> 0b000 101 11 | 6 -> 0b000 110 11 | 7 -> 0b000 111 11 |
	// 0 -> 0b000 CBA 11
	//  CFGR0 	| GPIO5	| GPIO4	| GPIO3	| GPIO2	| GPIO1	| REFON	| DTEN	| ADCOPT| // | GPIO1 -> 1 | GPIO2 -> 1 | GPIO3 -> A | GPIO4 -> B | GPIO5 -> C


	uint16_t DCC[] = [0x0FFF]; //0b00001111 11111111
	uint8_t nIC = 15;								// The number of ICs being written

	void set_cfgr(uint8_t nIC){
		uint8_t config[][6];
		for(int i= 0; i < nIC; i++){
			config[i][0] = (uint8_t)((GPIOx<<3)&0xF8)|((REFON<<2)&0x04)|((DTEN<<1)&0x02)|(ADCOPT&0x01);
		    //0b01011000 & 0b11111000 | 0b00000100 & 0b0100 | 0b00000010 & 0b0010 | 0b00000000 & 0b00000001
		    //	     0b0101100		  |     0b00000100	    |	 0b00000010	      | 		  0b00000000
		    //		CFGR0 -> 0b0101110
			config[i][1] = (uint8_t)(UNDERVOLTAGE_TH&(0x00FF));
			config[i][2] = (uint8_t)(((OVERVOLTAGE_TH&(0x000F))<<4) | ((UNDERVOLTAGE_TH&(0x0F00))>>8));
			config[i][3] = (uint8_t)((OVERVOLTAGE_TH&(0x0FF0))>>4);
			config[i][4] = (uint8_t)(DCC[i]&0x00FF);
		    //0b00001111 11111111 & 0b00000000 11111111
		    //		0b00000000 11111111
		    // 		CFGR4 -> 0b11111111
			config[i][5] = (uint8_t)(((DCTO)<<4) | (((DCC[i]&0x0F00))>>8));
		    // 0b11000000 	 | (0b00001111 11111111 & 0b00001111 00000000)>>8
		    // 0b11000000 	 | (0b00001111 00000000)>>8
		    // 0b11000000 	 | 0b00000000 00001111
		    // 		CFGR5 =	0b11001111
		}
	}


	/////////////////////////////////////////
	//S'han de pasar les llistes a binari/hexadecimal
	/////////////////////////////////////////


	void ltc6811_wrcfg(uint8_t nIC, uint8_t config[][6]);



	/////////////////////////////////////////////////////ADCV//////////////////////////////////////////////////////////////////////

	/*Starts ADC conversions of the ltc6811 Cpin inputs.
	  The type of ADC conversion executed can be changed by setting the following parameters:
	 |Variable|Function                                      |
	 |--------|----------------------------------------------|
	 | MD     | Determines the filter corner of the ADC      |
	 | CH     | Determines which cell channels are converted |
	 | DCP    | Determines if Discharge is Permitted       	 |

	 Broadcast Command Code:
	-------------

	|CMD[0:1] |  15   |  14   |  13   |  12   |  11   |  10   |   9   |   8   |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
	|-----------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|
	|ADCV:      |   0   |   0   |   0   |   0   |   0   |   0   |   1   | MD[1] | MD[2] |   1   |   1   |  DCP  |   0   | CH[2] | CH[1] | CH[0] |

	*/
	uint8_t MD = 0b10;								// ADC Conversion Mode
	uint8_t DCP = 0;								// Controls if Discharge is permitted during conversion: 0 = Not Permitted; 1 = Permitted
	uint8_t CH = 0b000;								// Sets which Cell channels are converted

	void ltc6811_adcv(uint8_t MD, uint8_t DCP, uint8_t CH);



	/////////////////////////////////////////////////////RDCVX//////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////
	//Falta per definir la variable CELL_CHANNELS
	/////////////////////////////////////////

	/*The function is used to read the cell codes of the ltc6811.
	 This function will send the requested read commands parse the data
	 and store the cell voltages in cell_codes variable.

	 uint8_t reg; This controls which cell voltage register is read back.
	          0: Read back all Cell registers
	          1: Read back cell group A
	          2: Read back cell group B
	          3: Read back cell group C
	          4: Read back cell group D

	  uint8_t total_ic; This is the number of ICs in the daisy chain(-1 only)

	  uint16_t cell_codes[]; An array of the parsed cell codes from lowest to highest. The cell codes will
	  be stored in the cell_codes[] array in the following format:
	  |  cell_codes[0][0]| cell_codes[0][1] |  cell_codes[0][2]|    .....     |  cell_codes[0][11]|  cell_codes[1][0] | cell_codes[1][1]|  .....   |
	  |------------------|------------------|------------------|--------------|-------------------|-------------------|-----------------|----------|
	  |IC1 Cell 1        |IC1 Cell 2        |IC1 Cell 3        |    .....     |  IC1 Cell 12      |IC2 Cell 1         |IC2 Cell 2       | .....    |

	  @return int8_t, PEC Status.

	    0: No PEC error detected

	    -1: PEC error detected, retry read
	*/
	uint8_t reg = 0; 								//Controls which cell voltage register is read back: 0 -> All Cell registers; 1 -> A; 2 -> B; 3 -> C; 4 -> D
	uint8_t total_ic = nIC-1;						//Number of ICs in the daisy chain(-1 only)
	uint16_t cell_codes[][CELL_CHANNELS] = [[1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111], //IC1
											[1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111], //IC2
											[1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111], //IC3
											[1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111], //IC4
											[1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111], //IC5
											[1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111], //IC6
											[1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111], //IC7
											[1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111], //IC8
											[1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111], //IC9
											[1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111], //IC10
											[1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111], //IC11
											[1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111], //IC12
											[1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111], //IC13
											[1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111], //IC14
											[1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111,1111111111111111]] //IC15
	//										  	Volt1				Volt2			Volt3	 		Volt4    			Volt5     		Volt6   		Volt7    		Volt8    			Volt9    		Volt10   		Volt11    		Volt12

	uint8_t ltc6811_rdcv(uint8_t reg, uint8_t total_ic, uint16_t cell_codes[][CELL_CHANNELS]);



	/////////////////////////////////////////////////////WRCFG//////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////
	//S'ha de fer una funció que canvï la configuració dels GPIOS i PWM del array Config[][6]
	/////////////////////////////////////////

	  /*
	  uint8_t config[][6] is a two dimensional array of the configuration data that will be written, the array should contain the 6 bytes for each

	   IC in the daisy chain. The lowest IC in the daisy chain should be the first 6 byte block in the array. The array should
	   have the following format:
	   |  config[0][0]| config[0][1] |  config[0][2]|  config[0][3]|  config[0][4]|  config[0][5]| config[1][0] |  config[1][1]|  config[1][2]|  .....    |
	   |--------------|--------------|--------------|--------------|--------------|--------------|--------------|--------------|--------------|-----------|
	   |IC1 CFGR0     |IC1 CFGR1     |IC1 CFGR2     |IC1 CFGR3     |IC1 CFGR4     |IC1 CFGR5     |IC2 CFGR0     |IC2 CFGR1     | IC2 CFGR2    |  .....    |

	   The function will calculate the needed PEC codes for the write data
	   and then transmit data to the ICs on a daisy chain.
	   */
	char uint8_t config[][6] = [[11111111,11111111,11111111,11111111,11111111,11111111], //IC1 //CFGR: Configuration Register
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC2
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC3
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC4
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC5
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC6
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC7
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC8
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC9
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC10
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC11
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC12
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC13
								[11111111,11111111,11111111,11111111,11111111,11111111], //IC14
								[11111111,11111111,11111111,11111111,11111111,11111111]];//IC15
	//						 	  CFGR0		CFGR1	 CFGR2	  CFGR3    CFGR4    CFGR5

	//Register	| Bit 7	| Bit 6	| Bit 5	| Bit 4	| Bit 3	| Bit 2	| Bit 1	| Bit 0	| //DTEN -> Discharge Timer Enable (Read only): 1= Enables the Discharge Timer for Discharge Switches 0=Disables Discharge Timer
	//CFGR0 	| GPIO5	| GPIO4	| GPIO3	| GPIO2	| GPIO1	| REFON	| DTEN	| ADCOPT| //GPIO -> General Purpose Input/Output
	//CFGR1 	| VUV[7]| VUV[6]| VUV[5]| VUV[4]| VUV[3]| VUV[2]| VUV[1]| VUV[0]| //REFON -> References powered up: 1=References Remain Powered Up Until Watchdog Timeout 0=References Shut Down After Conversions (Default)
	//CFGR2 	| VOV[3]| VOV[2]| VOV[1]| VOV[0]|VUV[11]|VUV[10]| VUV[9]| VUV[8]| //VUV -> Undervoltage Comparison Voltage: Comparison Voltage = (VUV+1)*16*100uV; Defalut VUV = 0x000
	//CFGR3 	|VOV[11]|VOV[10]| VOV[9]| VOV[8]| VOV[7]| VOV[6]| VOV[5]| VOV[4]| //VOV -> Overvoltage Comparison Voltage: Comparison Voltage = VOV*16*100uV; Defalut VOV = 0x000
	//CFGR4 	| DCC8	| DCC7	| DCC6	| DCC5	| DCC4	| DCC3	| DCC2	| DCC1	| //DCC -> Discharge Cell X: X->[1,12] 1=Turn ON Shorting Switch for Cell x 0=Turn OFF Shorting Switch for Cell x (Default)
	//CFGR5 	|DCTO[3]|DCTO[2]|DCTO[1]|DCTO[0]| DCC12	| DCC11	| DCC10	| DCC9	| //DCTO -> Discharge Time Out Value
																				  //ADCOPT -> ADC Mode Option Bite: 0=Selects Modes 27kHz, 7kHz, 422Hz or 26Hz with MD[1:0] Bits in ADC Conversion Commands (Default) 1=Selects Modes 14kHz, 3kHz, 1kHz or 2kHz with MD[1:0] Bits in ADC Conversion Commands

	/////////////////////////////////////////
	//S'han de pasar les llistes a binari/hexadecimal
	/////////////////////////////////////////

	uint8_t nIC = 15;								// The number of ICs being written
	void ltc6811_wrcfg(uint8_t nIC, uint8_t config[][6]);



	/////////////////////////////////////////////////////ADAX//////////////////////////////////////////////////////////////////////
	 /*
	The type of ADC conversion executed can be changed by passing the following variables to the conversion functions:
	 |Variable|Function                                      |
	 |--------|----------------------------------------------|
	 | MD     | Determines the filter corner of the ADC      |
	 | CHG    | Determines which GPIO channels are converted |
	 | ST     | Determines which Self Test is executed       |

	Starts an ADC conversions of the ltc6811 GPIO inputs.
	Command Code:
	-------------

	|CMD[0:1] |  15   |  14   |  13   |  12   |  11   |  10   |   9   |   8   |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
	|-----------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|
	|ADAX:      |   0   |   0   |   0   |   0   |   0   |   1   |   0   | MD[1] | MD[2] |   1   |   1   |  0    |   0   | CHG[2]| CHG[1]| CHG[0]|
	*/
	uint8_t MD = 0b10;								// ADC Conversion Mode
	uint8_t CHG = 001;								//Which GPIOS are converted: 000 -> GPIO1-5; 001 -> GPIO1; 010 -> GPIO2; 011 -> GPIO3; 100 -> GPIO4; 101 -> GPIO5
	void ltc6811_adax(uint8_t MD,uint8_t CHG);

	/////////////////////////////////////////////////////RDAUXA//////////////////////////////////////////////////////////////////////
	/*!  Reads and parses the ltc6811 auxiliary registers.

	 The function is used to read the  parsed GPIO codes of the ltc6811. This function will send the requested
	 read commands parse the data and store the gpio voltages in aux_codes variable


	 uint16_t aux_codes[][6]; A two dimensional array of the gpio voltage codes. The GPIO codes will
	 be stored in the aux_codes[][6] array in the following format:
	 |  aux_codes[0][0]| aux_codes[0][1] |  aux_codes[0][2]|  aux_codes[0][3]|  aux_codes[0][4]|  aux_codes[0][5]| aux_codes[1][0] |aux_codes[1][1]|  .....    |
	 |-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|---------------|-----------|
	 |IC1 GPIO1        |IC1 GPIO2        |IC1 GPIO3        |IC1 GPIO4        |IC1 GPIO5        |IC1 Vref2        |IC2 GPIO1        |IC2 GPIO2      |  .....    |

	@return  int8_t, PEC Status
	  0: No PEC error detected
	 -1: PEC error detected, retry read

	*/
	uint8_t reg = 0; 								//Controls which cell voltage register is read back: 0 -> All Cell registers; 1 -> A; 2 -> B; 3 -> C; 4 -> D
	uint8_t nIC = 15;								// The number of ICs being written
	AUX_CHANNELS = 6;								//Number of GPIOS read back
   uint16_t aux_codes[][AUX_CHANNELS] = [[11111111,11111111,11111111,11111111,11111111,11111111], // IC1 //!< A two dimensional array of the parsed gpio voltage codes
										  [11111111,11111111,11111111,11111111,11111111,11111111], // IC2
										  [11111111,11111111,11111111,11111111,11111111,11111111], // IC3
										  [11111111,11111111,11111111,11111111,11111111,11111111], // IC4
										  [11111111,11111111,11111111,11111111,11111111,11111111], // IC5
										  [11111111,11111111,11111111,11111111,11111111,11111111], // IC6
										  [11111111,11111111,11111111,11111111,11111111,11111111], // IC7
										  [11111111,11111111,11111111,11111111,11111111,11111111], // IC8
										  [11111111,11111111,11111111,11111111,11111111,11111111], // IC9
										  [11111111,11111111,11111111,11111111,11111111,11111111], // IC10
										  [11111111,11111111,11111111,11111111,11111111,11111111], // IC11
										  [11111111,11111111,11111111,11111111,11111111,11111111], // IC12
										  [11111111,11111111,11111111,11111111,11111111,11111111], // IC13
										  [11111111,11111111,11111111,11111111,11111111,11111111], // IC14
										  [11111111,11111111,11111111,11111111,11111111,11111111]];// IC15
	//										GPIO1	 GPIO2    GPIO3     GPIO4    GPIO5   Vref

	int8_t ltc6811_rdaux(uint8_t reg, uint8_t nIC, uint16_t aux_codes[][AUX_CHANNELS]);



  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_SOC_CALC_Task */
/**
* @brief Function implementing the SOC_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SOC_CALC_Task */
void SOC_CALC_Task(void *argument)
{
  /* USER CODE BEGIN SOC_CALC_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END SOC_CALC_Task */
}

/* USER CODE BEGIN Header_Volt_Task */
/**
* @brief Function implementing the Calc_Volt_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Volt_Task */
void Volt_Task(void *argument)
{
  /* USER CODE BEGIN Volt_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Volt_Task */
}

/* USER CODE BEGIN Header_Temp_Task */
/**
* @brief Function implementing the Calc_Temp_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Temp_Task */
void Temp_Task(void *argument)
{
  /* USER CODE BEGIN Temp_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Temp_Task */
}

/* USER CODE BEGIN Header_CANTX_Task */
/**
* @brief Function implementing the CANTX_Lectures_ thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CANTX_Task */
void CANTX_Task(void *argument)
{
  /* USER CODE BEGIN CANTX_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END CANTX_Task */
}

/* USER CODE BEGIN Header_CANRXCurrentTask */
/**
* @brief Function implementing the CANRX_Current_T thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CANRXCurrentTask */
void CANRXCurrentTask(void *argument)
{
  /* USER CODE BEGIN CANRXCurrentTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END CANRXCurrentTask */
}

/* USER CODE BEGIN Header_AIR_Task */
/**
* @brief Function implementing the CANRX_AIR_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AIR_Task */
void AIR_Task(void *argument)
{
  /* USER CODE BEGIN AIR_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END AIR_Task */
}

/* USER CODE BEGIN Header_ReadingTask */
/**
* @brief Function implementing the Reading_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ReadingTask */
void ReadingTask(void *argument)
{
  /* USER CODE BEGIN ReadingTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ReadingTask */
}

/* USER CODE BEGIN Header_BMSOK_Task */
/**
* @brief Function implementing the CANTX_BMSOK_Tas thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_BMSOK_Task */
void BMSOK_Task(void *argument)
{
  /* USER CODE BEGIN BMSOK_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END BMSOK_Task */
}

/* USER CODE BEGIN Header_CANTX_Volt_Temp */
/**
* @brief Function implementing the CANTX_VolTemTas thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CANTX_Volt_Temp */
void CANTX_Volt_Temp(void *argument)
{
  /* USER CODE BEGIN CANTX_Volt_Temp */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END CANTX_Volt_Temp */
}

/* USER CODE BEGIN Header_Error_Volt */
/**
* @brief Function implementing the Error_Volt_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Error_Volt */
void Error_Volt(void *argument)
{
  /* USER CODE BEGIN Error_Volt */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Error_Volt */
}

/* USER CODE BEGIN Header_Error_Temp */
/**
* @brief Function implementing the Error_Temp_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Error_Temp */
void Error_Temp(void *argument)
{
  /* USER CODE BEGIN Error_Temp */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Error_Temp */
}

/* USER CODE BEGIN Header_Error_Curr */
/**
* @brief Function implementing the Error_Curr_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Error_Curr */
void Error_Curr(void *argument)
{
  /* USER CODE BEGIN Error_Curr */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Error_Curr */
}

/* USER CODE BEGIN Header_Error_Slaves */
/**
* @brief Function implementing the Error_Slav_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Error_Slaves */
void Error_Slaves(void *argument)
{
  /* USER CODE BEGIN Error_Slaves */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Error_Slaves */
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
