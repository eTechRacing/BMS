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

SPI_HandleTypeDef hspi1;

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
static void MX_SPI1_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_UART4_Init(void);
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
  MX_SPI1_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_UART4_Init();
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
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);					//The high state of the chip select pin is set
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
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
	char temp_buf[12];
	char uint8_t CFGR0[8] = {1,1,1,1,1,1,1,1};
	char uint8_t CFGR1[8] = {1,1,1,1,1,1,1,1};
	char uint8_t CFGR2[8] = {1,1,1,1,1,1,1,1};
	char uint8_t CFGR3[8] = {1,1,1,1,1,1,1,1};
	char uint8_t CFGR4[8] = {1,1,1,1,1,1,1,1};
	char uint8_t CFGR5[8] = {1,1,1,1,1,1,1,1};

	/////////////////////////////////////////
	//S'han de pasar les llistes a binari/hexadecimal
	/////////////////////////////////////////

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);					//The Low state of the chip select pin is set
	HAL_SPI_Transmit(&hspi1, 												//Set the SPI channel that is used
			(uint8_t *)&CFGR0, 												//Pass the data that has to be sent in an 8 bit unsigned integer pointer
			1, 																//How many bytes will be sent
			HAL_MAX_DELAY);													//The time it will take
	HAL_SPI_Transmit(&hspi1,(uint8_t *)&CFGR1,1,HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1,(uint8_t *)&CFGR2,1,HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1,(uint8_t *)&CFGR3,1,HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1,(uint8_t *)&CFGR4,1,HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1,(uint8_t *)&CFGR5,1,HAL_MAX_DELAY);

	/////////////////////////////////////////
	//S'han de definir el valor de les variables
	/////////////////////////////////////////
	void initCellsVoltageMeasurement(SPI_HandleTypeDef spi_channel,uint16_t dischargePermitted,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
		sendGenCommand(spi_channel,(CC_ADCV)|(cellsVoltageMeasurementMode)|(dischargePermitted)|(CH_ALL),CS_Port,CS_Pin);
	}

uint8_t	readCellsVoltages(SPI_HandleTypeDef spi_channel,uint32_t *cellsVoltages, uint32_t *overAndUnderVoltages,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
		uint8_t CVRegAdata[6];
		uint8_t CVRegBdata[6];
		uint8_t CVRegCdata[6];
		uint8_t CVRegDdata[6];
		uint8_t statusRegBdata[6];
		uint8_t CRC_count=0;

		while(sendReadCommand(spi_channel,CC_RDCVA,CVRegAdata,6,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
		if(CRC_count == CRC_LIMIT){
			return 0;
		}
		CRC_count = 0;
		while(sendReadCommand(spi_channel,CC_RDCVB,CVRegBdata,6,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
		if(CRC_count == CRC_LIMIT){
			return 0;
		}
		CRC_count = 0;
		while(sendReadCommand(spi_channel,CC_RDCVC,CVRegCdata,6,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
		if(CRC_count == CRC_LIMIT){
			return 0;
		}
		CRC_count = 0;
		while(sendReadCommand(spi_channel,CC_RDCVD,CVRegDdata,6,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
		if(CRC_count == CRC_LIMIT){
			return 0;
		}
		CRC_count = 0;
		while(	sendReadCommand(spi_channel,CC_RDSTATB,statusRegBdata,6,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
		if(CRC_count == CRC_LIMIT){
			return 0;
		}


		for(int i=0;i<3;i++){
			cellsVoltages[i] = ((CVRegAdata[2*i+1] << 8) | CVRegAdata[2*i])*100;
			cellsVoltages[3+i] = ((CVRegBdata[2*i+1] << 8) | CVRegBdata[2*i])*100;
			cellsVoltages[6+i] = ((CVRegCdata[2*i+1] << 8) | CVRegCdata[2*i])*100;
			cellsVoltages[9+i] = ((CVRegDdata[2*i+1] << 8) | CVRegDdata[2*i])*100;
		}

		*overAndUnderVoltages = (statusRegBdata[4] << 16) | (statusRegBdata[3] << 8) | statusRegBdata[2];
		return 1;
	}

    osDelay(1);
  }
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
