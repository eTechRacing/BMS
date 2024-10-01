/**
  ******************************************************************************
  * @file           : LTC6811.h
  * @brief          : Header for LTC6811.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
	Author: Miguel �ngel Alegre
	Mail: alegrelopez.miguel@gmail.com
	Tel: 681057057
	Date: 16/12/2018
	Driverless UPC - 2019
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LTC6811_2_H
#define LTC6811_2_H


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
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */
/*COMMANDS CODE BEGIN Private Defines*/
#define CC_WRCFGA 0x001 //Write Configuration Register A command code
#define CC_WECFGB 0x024 //Write Configuration Register B command code
#define CC_RDCFGA 0x002 //Read Configuration Register A command code
#define CC_RDCFGB 0x026 //Read Configuration Register B command code
#define CC_RDCVA 0x004 //Read Cell Voltage Register Group A command code
#define CC_RDCVB 0x006 //Read Cell Voltage Register Group B command code
#define CC_RDCVC 0x008 //Read Cell Voltage Register Group C command code
#define CC_RDCVD 0x00A //Read Cell Voltage Register Group D command code
#define CC_RDCVE 0x009 //Read Cell Voltage Register Group E command code
#define CC_RDCVF 0x00B //Read Cell Voltage Register Group F command code
#define CC_RDAUXA 0x00C //Read Auxiliary Register Group A command code
#define CC_RDAUXB 0x00E //Read Auxiliary Register Group B command code
#define CC_RDAUXC 0x00D //Read Auxiliary Register Group C command code
#define CC_RDAUXD 0x00F //Read Auxiliary Register Group D command code
#define CC_RDSTATA 0x010 //Read Status Register Group A command code
#define CC_RDSTATB 0x012 //Read Status Register Group B command code
#define CC_WRSCTRL 0x014 //Write S Control Register Group command code
#define CC_WRPWM 0x020 //Write PWM Register Group command code
#define CC_WRPSB 0x01C //Write PWM/S Control Register Group B command code
#define CC_RDSCTRL 0x016 //Read S Control Register Group command code
#define CC_RDPWM 0x022 //Read PWM Register Group command code
#define CC_RDPSB 0x01E //Read PWM/S Control Register Group B command code
#define CC_STSCTRL 0x019 //Start S Control Pulsing and Poll Status command code
#define CC_CLRSCTRL 0x018 //Clear S Control Register Group command code

/*Different Configurations for ADC Mode*/
/*It depends on CFRG0[0] (Configuration Register Group ADCOPT Bit) to choose between both values*/
#define CC_MD_422_1k ((0x0)<<7)&(0x180) //422Hz (ADCOPT = 0), 1kHz (ADCOPT = 1)
#define CC_MD_27k_14k ((0x1)<<7)&(0x180) //27kHz (ADCOPT = 0), 14kHz (ADCOPT = 1)
#define CC_MD_7K_3k ((0x2)<<7)&(0x180) //7kHz (ADCOPT = 0), 3kHz (ADCOPT = 1)
#define CC_MD_26_2k ((0x3)<<7)&(0x180) //26Hz (ADCOPT = 0), 2kHz (ADCOPT = 1)

/*Discharge Permited Bit*/
#define DCP_NOTPERMITED ((0x0)<<4)&(0x010) //Discharge not permited
#define DCP_PERMITED ((0x1)<<4)&(0x010) //Discharge permited

/*Cell selection for ADC Conversion*/
#define CH_ALL (0x0)&0x007 //Select all cells for ADC conversion
#define CH_1and7 (0x1)&0x007 //Select cells 1 and 7 for ADC conversion
#define CH_2and8 (0x2)&0x007 //Select cells 2 and 8 for ADC conversion
#define CH_3and9 (0x3)&0x007 //Select cells 3 and 9 for ADC conversion
#define CH_4and10 (0x4)&0x007 //Select cells 4 and 10 for ADC conversion
#define CH_5and11 (0x5)&0x007 //Select cells 5 and 11 for ADC conversion
#define CH_6and12 (0x6)&0x007 //Select cells 6 and 12 for ADC conversion

/*Pull-Up/Pull-down Current for Open Wire conversions*/
#define PUP_DOWN ((0x0)<<6)&0x040 //Pull down current
#define PUP_UP ((0x1)<<6)&0x040 //Pull up current

/*Self Test Mode selection*/
#define ST_1 ((0x1)<<5)&0x060 //Self test mode 1. It depends on ADC conversion frequency
#define ST_2 ((0x2)<<5)&0x060 //Self test mode 2. It depends on ADC conversion frequency
#define ST_1_RESULT_NORMALMODE 0x9555

/*GPIO Selection for ADC conversion*/
#define CHG_ALL (0x0)&0x007  //Select all GPIOS with 2nd reference
#define CHG_GPIO1 (0x1)&0x007 //Select GPIO1
#define CHG_GPIO2 (0x2)&0x007 //Select GPIO2
#define CHG_GPIO3 (0x3)&0x007 //Select GPIO3
#define CHG_GPIO4 (0x4)&0x007 //Select GPIO4
#define CHG_GPIO5 (0x5)&0x007 //Select GPIO5
#define CHG_2ndRef (0x6)&0x007 //2nd Reference

/*Status group selection for Data Acquisition system diagnostics*/
#define CHST_ALL (0x0)&0x007  //Select Sum of Cells, Internal Die Temperature, Power Supply Analog and Power Supply Digital
#define CHST_SC (0x1)&0x007 //Select Sum of Cells
#define CHST_ITMP (0x2)&0x007 //Select Internal Die Temperature
#define CHST_VA (0x3)&0x007 //Select Analog Power Supply
#define CHST_VD (0x4)&0x007 //Select Digital Power Supply

#define CC_ADCV 0x260 //Start Cell Voltage ADC Conversion and Poll Status Command Code BASE! it must be completed with MD, DCP and CH.
//Ex: Starting CELL Voltage ADC Conversion of All cells, at Normal Mode (7 kHz) and with Discharge Disabled is CC_ADCV|CC_MD_7k_3k|CH_ALL|DCP_NOTPERMITED

#define CC_ADOW 0x228 //Start Open Wire ADC Conversion and Poll Status Command Code BASE! It must be completed with MD, PUP, DCP and CH.
//Ex: Starting Open Wire ADC Conversion of All cells, at Normal Mode (7 kHz), Pull Down current and with Discharge Disabled is CC_ADOW|CC_MD_7k_3k|CH_ALL|DCP_NOTPERMITED|PUP_DOWN

#define CC_CVST 0x207 //Start Self Test Cell Voltage conversion and Poll Status Command Code BASE! It must be completed with MD and ST.
//Ex: Starting Self Test Cell Voltage Conversion at Normal Mode (7 kHz) and Self Test 1 is CC_CVST|CC_MD_7k_3k|ST_1

#define CC_ADOL 0x201 //Start Overlap Measurement of Cell 7 Voltage Command Code BASE! It must be completed with MD and DCP.
//Ex: Starting Overlap Measurement of Cell 7 Voltage at Fast Mode (27 kHz) and Discharge Permited it CC_ADOL|CC_MD_27k_14k|DCP_PERMITED

#define CC_ADAX 0x460 //Start GPIOs ADC Conversion and Poll Status Command Code BASE! It must be completed with MD and CHG.
//Ex: Starting Measurement of GPIO 3 at Filtered Mode (26 Hz) is CC_ADAX|CC_MD_26_2k|CHG_GPIO3

#define CC_ADAXD 0x400 //Start GPIOs ADC Conversion With Digital Redundancy and Poll Status Command Code BASE! It must be completed with MD and CHG.
//Ex: Starting Measurement of All GPIOS with Digital Redundancy at Filtered Mode (26 Hz) is CC_ADAXD|CC_MD_26_2k|CHG_ALL

#define CC_AXST 0x407 //Start Self Test GPIOs Conversion and Poll Status Command Code BASE! It must be completed with MD and ST.
//Ex: Starting self Test GPIOs conversion at Normal Mode (7 kHz) and Self Test 2 is CC_AXST|CC_MD_7k_3k|ST_2

#define CC_ADSTAT 0x468 //Start Status Group ADC Conversion and Poll Status Command Code BASE! It must be completed with MD and CHST.
//EX: Starting Sum of Cells Status Measurement at Normal Mode is CC_ADSTAT|CHST_SC|CC_MD_7k_3k

#define CC_ADSTATD 0x408 //Start Status Group ADC Conversion  With Digital Redundancy and Poll Status Command Code BASE! It must be completed with MD and CHST.
//EX: Starting Sum of Cells Status Measurement at Normal Mode is CC_ADSTAT|CHST_SC|CC_MD_7k_3k

#define CC_STATST 0x40F //Start Self Test Status Group Conversion and Poll Status Command Code BASE! It must be completed with MD and ST
#define CC_ADCVAX 0x46F //Start Combined Cell Voltage and GPIO1,GPIO2 Conversion and Poll Status Command Code BASE! It must be completed with MD and DCP.
#define CC_ADCVSC 0x467 //Start Combined Cell Voltage and SC Conversion and Poll Status Command Code BASE! It must be completed with MD and DCP.
#define CC_CLRCLL 0x711 //Clear Cell Voltage Register Groups Command Code.
#define CC_CLRAUX 0x712 //Clear Auxiliary Register Groups Command Code.
#define CC_CLRSTAT 0x713 //Clear Status Register Groups Command Code.
#define CC_PLADC 0x714 //Poll ADC Conversion Status Command Code.
#define CC_DIAGN 0x715 //Diagnose MUX and Poll Status Command Code.
#define CC_WRCOMM 0x721 //Write COMM Register Group Command Code.
#define CC_RDCOMM 0x722 //Read COMM Register Group Command Code.
#define CC_STCOMM 0x723 //Start I2C/SPI Communication Command Code.




/*INIZIALIZATION PARAMETERS*/
#define GPIOx 0x1F //Pull-down off for GPIOs
#define REFON 0x1 //Reference remains powered up until watchog timeout
#define DTEN 0x1 //Enable the discharge timer for discharge switches
#define ADCOPT 0x0 //27k,7k,422 and 26 ADC frequencies
#define DCCx_INIT 0x000 //Initially, deactivate DCC
#define DCTO 0xC//Discharge Time Out Value to 60 min (0xC) --> SI DTEN = 1, DCTO serà el temps que tardarà el xip en adormir-se

/*Other Parameters*/
#define POLL_ADC_TIMES 10
#define MAX_TIME_POLLING_ADCV 5000
#define MAX_TIME_POLLING_GPIO 5000
#define MAX_TIME_POLLING_STATUS 5000
#define MAX_TIME_POLLING_OVERLAP 1200
#define MAX_TIME_POLLING_DIAG 12000
#define MAX_REFUP_TIME 50000

#define OVERVOLTAGE_TH (uint16_t)((VCELLMAX)/16/0.1)
#define UNDERVOLTAGE_TH (uint16_t)((((VCELLMIN)/0.1)/16)-1)

#define CRC_LIMIT 4

/*COMMANDS CODE END Private Defines*/


/*Function prototypes*/
void LTC6811_Wakeup(GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void init_PEC15_Table(void);
uint16_t pec15(uint8_t *data, uint8_t length);
uint16_t pec15_TransmitReceive(uint8_t *data);
void sendGenCommand(SPI_HandleTypeDef spi_channel,uint16_t CMD,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void sendGenCommandIT(SPI_HandleTypeDef spi_channel,uint16_t CMD,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void sendGenAdressCommand(SPI_HandleTypeDef spi_channel,uint16_t CMD, uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void sendGenAdressCommandIT(SPI_HandleTypeDef spi_channel,uint16_t CMD,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void sendWriteCommand(SPI_HandleTypeDef spi_channel,uint16_t CMD,uint8_t *data, uint8_t length,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void sendAdressWriteCommand(SPI_HandleTypeDef spi_channel,uint16_t CMD,uint8_t *data, uint8_t length,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void sendAdressWriteCommandIT(SPI_HandleTypeDef spi_channel,uint16_t CMD,uint8_t *data, uint8_t length,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t sendReadCommand(SPI_HandleTypeDef spi_channel,uint16_t CMD,uint8_t *data, uint8_t length,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t sendAdressReadCommand(SPI_HandleTypeDef spi_channel,uint16_t CMD,uint8_t *data, uint8_t length,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void initConfigARegister(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void initAdressConfigARegister(SPI_HandleTypeDef spi_channel,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void initConfigARegiser_DCTOto0(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void setPWMRegister(SPI_HandleTypeDef spi_channel,uint8_t *PWMvalues,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void setManualBalancing(SPI_HandleTypeDef spi_channel,uint16_t DCCvalues,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void setAdressManualBalancing(SPI_HandleTypeDef spi_channel,uint16_t DCCvalues,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void setCellsVoltageMeasurementMode(uint16_t mode);
void initCellsVoltageMeasurement(SPI_HandleTypeDef spi_channel,uint16_t dischargePermitted,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void initCellsVoltageMeasurementIT(SPI_HandleTypeDef spi_channel,uint16_t dischargePermitted,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void initAdressCellsVoltageMeasurement(SPI_HandleTypeDef spi_channel,uint16_t dischargePermitted,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void initAdressCellsVoltageMeasurementIT(SPI_HandleTypeDef spi_channel,uint16_t dischargePermitted,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void initCellXandYMeasurement(SPI_HandleTypeDef spi_channel,uint16_t dischargePermitted,uint16_t cells,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t readCellsVoltages(SPI_HandleTypeDef spi_channel,uint32_t *cellsVoltages, uint32_t *overAndUnderVoltages,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t readAdressCellsVoltages(SPI_HandleTypeDef spi_channel,uint32_t *cellsVoltages, uint32_t *overAndUnderVoltages,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t readAdressCellsVoltagesIT(SPI_HandleTypeDef spi_channel,uint32_t *cellsVoltages, uint32_t *overAndUnderVoltages,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t sendAdressReadCommandIT(SPI_HandleTypeDef spi_channel,uint16_t CMD,uint8_t *data, uint8_t length,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void initOpenWireDetection(SPI_HandleTypeDef spi_channel,uint16_t dischargePermitted,uint16_t pullUp,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void initOpenWireDetectionAdress(SPI_HandleTypeDef spi_channel,uint16_t dischargePermitted,uint16_t pullUp,uint8_t adress, GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void setGPIOMeasurementMode(uint16_t mode);
void initAllGPIOADC_DigitalRed(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void initAdressAllGPIOADC_DigitalRed(SPI_HandleTypeDef spi_channel,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void initGPIOxADC_DigitalRed(SPI_HandleTypeDef spi_channel,uint16_t gpio,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t readGPIOs(SPI_HandleTypeDef spi_channel,uint32_t *results,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t readGPIOsAdress(SPI_HandleTypeDef spi_channel,uint32_t *results,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void initOL(SPI_HandleTypeDef spi_channel,uint16_t ,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t readOLValues(SPI_HandleTypeDef spi_channel,uint32_t *overlapValues,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t readAdressOLValues(SPI_HandleTypeDef spi_channel,uint32_t *overlapValues,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void setStatusMeasurementMode(uint16_t mode);
void initStatusCheckAll(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void initStatusCheck(SPI_HandleTypeDef spi_channel,uint16_t status,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t readStatusResults(SPI_HandleTypeDef spi_channel,uint32_t *sumOfCells, int32_t *internalDieTemperature, uint32_t *analogVoltage, uint32_t *digitalVoltage,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t readAdressStatusResults(SPI_HandleTypeDef spi_channel,uint32_t *sumOfCells, int32_t *internalDieTemperature, uint32_t *analogVoltage, uint32_t *digitalVoltage,GPIO_TypeDef* CS_Port,uint16_t CS_Pin,uint8_t adress);
void clearCellVoltagesRegisterGroup(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void clearAdressCellVoltagesRegisterGroup(SPI_HandleTypeDef spi_channel,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void clearAuxiliaryRegisterGroup(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void clearStatusRegisterGroup(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void clearSControlRegisterGroup(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void initMUXDiag(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t readMUXState(SPI_HandleTypeDef spi_channel,uint8_t *muxState,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t readAdressMUXState(SPI_HandleTypeDef spi_channel,uint8_t *muxState,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t readRevisionCode(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t sendPollADCStatus(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t sendAdressPollADCStatus(SPI_HandleTypeDef spi_channel,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void enterRefUPstate(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t getRefONbit(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void initCellVoltageSelfTest(SPI_HandleTypeDef spi_channel,uint16_t selfTest,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void initAdressCellVoltageSelfTest(SPI_HandleTypeDef spi_channel,uint16_t selfTest,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void initLTC6811(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t readCellsVoltagesRegisters(SPI_HandleTypeDef spi_channel,uint16_t *cellsVoltagesRegisters,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
uint8_t readAdressCellsVoltagesRegisters(SPI_HandleTypeDef spi_channel,uint16_t *cellsVoltagesRegisters,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin);
void delayUS_ASM(uint32_t delay_us);


/* USER CODE END Private defines */

#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
