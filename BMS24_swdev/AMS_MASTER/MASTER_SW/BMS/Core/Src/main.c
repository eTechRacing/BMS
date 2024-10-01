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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LTC6811-1_eTechRacing.h"
#include "eTechRacing.h"
#include <stdio.h>
#include <stdlib.h>
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

TIM_HandleTypeDef htim1;

/* Definitions for Lectures_Task */
osThreadId_t Lectures_TaskHandle;
const osThreadAttr_t Lectures_Task_attributes = {
  .name = "Lectures_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for SOC_Task */
osThreadId_t SOC_TaskHandle;
const osThreadAttr_t SOC_Task_attributes = {
  .name = "SOC_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CalcVolt_Task */
osThreadId_t CalcVolt_TaskHandle;
const osThreadAttr_t CalcVolt_Task_attributes = {
  .name = "CalcVolt_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CalcTemp_Task */
osThreadId_t CalcTemp_TaskHandle;
const osThreadAttr_t CalcTemp_Task_attributes = {
  .name = "CalcTemp_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ErrorVolt_Task */
osThreadId_t ErrorVolt_TaskHandle;
const osThreadAttr_t ErrorVolt_Task_attributes = {
  .name = "ErrorVolt_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for ErrorTemp_Task */
osThreadId_t ErrorTemp_TaskHandle;
const osThreadAttr_t ErrorTemp_Task_attributes = {
  .name = "ErrorTemp_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for ErrorCurrent_Ta */
osThreadId_t ErrorCurrent_TaHandle;
const osThreadAttr_t ErrorCurrent_Ta_attributes = {
  .name = "ErrorCurrent_Ta",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for ErrorSlaves_Tas */
osThreadId_t ErrorSlaves_TasHandle;
const osThreadAttr_t ErrorSlaves_Tas_attributes = {
  .name = "ErrorSlaves_Tas",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* USER CODE BEGIN PV */
//WRITE FUNCTIONS
uint8_t CHG = 001;								// Which GPIOS are converted: 000 -> GPIO1-5; 001 -> GPIO1; 010 -> GPIO2; 011 -> GPIO3; 100 -> GPIO4; 101 -> GPIO5
uint8_t REG_VOLT = 0; 							// Controls which cell voltage register is read back: 0 -> All Cell registers; 1 -> A; 2 -> B; 3 -> C; 4 -> D
uint8_t REG_TEMP = 1; 							// Controls which cell voltage register is read back: 0 -> All Cell registers; 1 -> A; 2 -> B; 3 -> C; 4 -> D
uint8_t **config;								// A two dimensional array with the cells configurations
uint8_t **PWM;									// A two dimensional array with the PWM configurations
uint8_t PEC_ERROR = 0;							// PEC ERROR, set on 0. If PEC_ERROR == 1 -> BMS_ERROR = 1
uint8_t BYTES_IN_REG = 6;						// Bytes in each register

//TIMER
uint16_t DELAY_SLEEP = 711;						// Delay between messages of the Wakeup_sleep function

//BALANCING
uint16_t VCELL_MIN = 4120;						// Variable
uint16_t V_MIN = 4050;							// Constant
uint16_t V_BALANCING = 4195;					// Constant
uint8_t DIFF_MAX = 2;							// Constant

// READ BACK FUNCTIONS
uint16_t cell_codes[ICn][CELL_CHANNELS];		// A two dimensional array of the read voltages
uint16_t aux_codes[ICn][AUX_CHANNELS];			// A two dimensional array of the parsed gpio voltage codes

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
static void MX_CAN2_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
void Lectures(void *argument);
void SOC(void *argument);
void CalcVolt(void *argument);
void CalcTemp(void *argument);
void ErrorVolt(void *argument);
void ErrorTemp(void *argument);
void ErrorCurrent(void *argument);
void ErrorSlaves(void *argument);

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
	wakeup_sleep(hspi1, htim1, ICn, DELAY_SLEEP);
	PWM = set_pwm(ICn);
	ltc6811_wrpwm(ICn, PWM, hspi1, htim1);
	int *DCC = Balancing(V_MIN, VCELL_MIN, cell_codes, V_BALANCING, DIFF_MAX, ICn);
	config = set_cfgr(ICn, BYTES_IN_REG, DCC);
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
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
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
  Lectures_TaskHandle = osThreadNew(Lectures, NULL, &Lectures_Task_attributes);

  /* creation of SOC_Task */
  SOC_TaskHandle = osThreadNew(SOC, NULL, &SOC_Task_attributes);

  /* creation of CalcVolt_Task */
  CalcVolt_TaskHandle = osThreadNew(CalcVolt, NULL, &CalcVolt_Task_attributes);

  /* creation of CalcTemp_Task */
  CalcTemp_TaskHandle = osThreadNew(CalcTemp, NULL, &CalcTemp_Task_attributes);

  /* creation of ErrorVolt_Task */
  ErrorVolt_TaskHandle = osThreadNew(ErrorVolt, NULL, &ErrorVolt_Task_attributes);

  /* creation of ErrorTemp_Task */
  ErrorTemp_TaskHandle = osThreadNew(ErrorTemp, NULL, &ErrorTemp_Task_attributes);

  /* creation of ErrorCurrent_Ta */
  ErrorCurrent_TaHandle = osThreadNew(ErrorCurrent, NULL, &ErrorCurrent_Ta_attributes);

  /* creation of ErrorSlaves_Tas */
  ErrorSlaves_TasHandle = osThreadNew(ErrorSlaves, NULL, &ErrorSlaves_Tas_attributes);

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
  hcan1.Init.Prescaler = 16;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_1TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
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
  hcan2.Init.TimeSeg1 = CAN_BS1_1TQ;
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
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 160;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_PE2_GPIO_Port, LED_PE2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(AIRPlus_Control_GPIO_Port, AIRPlus_Control_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, AIRMinus_Control_Pin|Precharge_Control_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LTC6820_CS_GPIO_Port, LTC6820_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_PE2_Pin */
  GPIO_InitStruct.Pin = LED_PE2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_PE2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : AIRPlus_Control_Pin */
  GPIO_InitStruct.Pin = AIRPlus_Control_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AIRPlus_Control_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : AIRMinus_Control_Pin Precharge_Control_Pin */
  GPIO_InitStruct.Pin = AIRMinus_Control_Pin|Precharge_Control_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SDC_End_Monitoring_Pin IMD_Monitoring_Pin */
  GPIO_InitStruct.Pin = SDC_End_Monitoring_Pin|IMD_Monitoring_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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

/* USER CODE BEGIN Header_Lectures */
/**
  * @brief  Function implementing the Lectures_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Lectures */
void Lectures(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
	  int *DCC = Balancing(V_MIN, VCELL_MIN, cell_codes, V_BALANCING, DIFF_MAX, ICn);
	  config = set_cfgr(ICn, BYTES_IN_REG, DCC);
	  ltc6811_wrcfg(ICn, config, hspi1, htim1);
	  ltc6811_adcv(MD_1, DCP_1, CH_1,hspi1);
	  PEC_ERROR = ltc6811_rdcv(REG_VOLT, ICn, cell_codes,hspi1, htim1);
	  ltc6811_wrcfg(ICn,config,hspi1, htim1);
	  ltc6811_adax(MD_1,CHG,hspi1);
	  PEC_ERROR = ltc6811_rdaux(REG_TEMP, ICn, aux_codes, hspi1, htim1);
	  osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_SOC */
/**
* @brief Function implementing the SOC_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SOC */
void SOC(void *argument)
{
  /* USER CODE BEGIN SOC */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END SOC */
}

/* USER CODE BEGIN Header_CalcVolt */
/**
* @brief Function implementing the CalcVolt_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CalcVolt */
void CalcVolt(void *argument)
{
  /* USER CODE BEGIN CalcVolt */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END CalcVolt */
}

/* USER CODE BEGIN Header_CalcTemp */
/**
* @brief Function implementing the CalcTemp_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CalcTemp */
void CalcTemp(void *argument)
{
  /* USER CODE BEGIN CalcTemp */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END CalcTemp */
}

/* USER CODE BEGIN Header_ErrorVolt */
/**
* @brief Function implementing the ErrorVolt_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ErrorVolt */
void ErrorVolt(void *argument)
{
  /* USER CODE BEGIN ErrorVolt */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ErrorVolt */
}

/* USER CODE BEGIN Header_ErrorTemp */
/**
* @brief Function implementing the ErrorTemp_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ErrorTemp */
void ErrorTemp(void *argument)
{
  /* USER CODE BEGIN ErrorTemp */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ErrorTemp */
}

/* USER CODE BEGIN Header_ErrorCurrent */
/**
* @brief Function implementing the ErrorCurrent_Ta thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ErrorCurrent */
void ErrorCurrent(void *argument)
{
  /* USER CODE BEGIN ErrorCurrent */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ErrorCurrent */
}

/* USER CODE BEGIN Header_ErrorSlaves */
/**
* @brief Function implementing the ErrorSlaves_Tas thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ErrorSlaves */
void ErrorSlaves(void *argument)
{
  /* USER CODE BEGIN ErrorSlaves */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ErrorSlaves */
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
