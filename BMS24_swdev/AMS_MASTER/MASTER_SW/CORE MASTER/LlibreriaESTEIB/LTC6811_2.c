/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "math.h"
#include "stm32f7xx_hal.h"
#include "LTC6811_2.h"
//#include "dwt_stm32_delay.h"
//#include "Estructuras.h"

/* Private variables ---------------------------------------------------------*/
uint16_t pec15Table[256];
uint16_t CRC15_POLY = 0x4599;
uint16_t DCC = DCCx_INIT;
uint16_t cellsVoltageMeasurementMode = CC_MD_7K_3k; //Normal Mode By Default
uint16_t GPIOMeasurementMode = CC_MD_7K_3k; //Normal Mode By Default
uint16_t StatusMeasurementMode = CC_MD_7K_3k; //Normal Mode By Default
//Masks for less than 12 cells, using the original datasheet configuration
extern TIM_HandleTypeDef 				htim2;

const uint8_t cells_5_MASK[12]={1,1,1,0,0,0,1,1,0,0,0,0};
const uint8_t cells_6_MASK[12]={1,1,1,0,0,0,1,1,1,0,0,0};
const uint8_t cells_7_MASK[12]={1,1,1,1,0,0,1,1,1,0,0,0};
const uint8_t cells_8_MASK[12]={1,1,1,1,1,0,1,1,1,1,0,0};
const uint8_t cells_9_MASK[12]={1,1,1,1,1,0,1,1,1,1,0,0};
const uint8_t cells_10_MASK[12]={1,1,1,1,1,0,1,1,1,1,1,0};
const uint8_t cells_11_MASK[12]={1,1,1,1,1,1,1,1,1,1,1,0};
//Global variables
//extern SPI_HandleTypeDef hspi2;								//SPI Handler

/* Private function prototypes -----------------------------------------------*/

void LTC6811_Wakeup(GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
  //Set CS to Low Level
  HAL_GPIO_WritePin(CS_Port, CS_Pin,GPIO_PIN_RESET);
  //Delay 300 us
  delayUS_ASM(300);
  //Set CS to High Level
  HAL_GPIO_WritePin(CS_Port, CS_Pin,GPIO_PIN_SET);
  delayUS_ASM(100);
}
/*This function inits the PEC15 table*/
void init_PEC15_Table(){
	for (int i = 0; i < 256; i++)
	{
		uint16_t remainder;
		remainder = i << 7;
			for (int bit = 8; bit > 0; --bit)
			{
				if (remainder & 0x4000)
				{
					remainder = ((remainder << 1));
					remainder = (remainder ^ CRC15_POLY);
				}
				else
				{
					remainder = ((remainder << 1));
				}
			}
		pec15Table[i] = remainder&0xFFFF;
	}
}

/*This function returns the pec15 of the data byte array of length elements*/
uint16_t pec15(uint8_t *data, uint8_t length){
	uint16_t remainder,address;
	remainder = 16;//PEC seed
	for (int i = 0; i < length; i++)
	{
		address = ((remainder >> 7) ^ data[i]) & 0xff;//calculate PEC table address
		remainder = (remainder << 8 ) ^ pec15Table[address];
	}
	return (remainder*2);//The CRC15 has a 0 in the LSB so the final value must be multiplied by 2
}

/*This function returns the pec15 of the data byte array of length elements*/
uint16_t pec15_TransmitReceive(uint8_t *data){
	uint16_t remainder,address;
	remainder = 16;//PEC seed
	for (int i = 4; i < 10; i++)
	{
		address = ((remainder >> 7) ^ data[i]) & 0xff;//calculate PEC table address
		remainder = (remainder << 8 ) ^ pec15Table[address];
	}
	return (remainder*2);//The CRC15 has a 0 in the LSB so the final value must be multiplied by 2
}

/*This functions send a Generic Command (not read, not write) trough SPI bus in Broadcast Mode*/
void sendGenCommand(SPI_HandleTypeDef spi_channel,uint16_t CMD,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t data2send[4];
	uint16_t aux;
	uint16_t pec;
	//Data conversion of CMD to 4 byte array formed by CMD and CRC

	//CMD Data conversion
	aux = (CMD&(0x0700))>>8;
	data2send[0] = (uint8_t)(aux);

	aux = CMD&(0x00FF);
	data2send[1] = (uint8_t)(aux);

	//CRC Calculation
	pec = pec15(data2send,2);
	
	//CRC Data Conversion
	aux = (pec&(0xFF00))>>8;
	data2send[2] = (uint8_t)(aux);
	//It's important to point out that final 0 has been included in pec15() function
	aux = pec&(0x00FF);
    data2send[3] = (uint8_t)(aux);
		
    //Set CS to Low Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_RESET);
    //Delay 10us
    delayUS_ASM(10);
    //Send command data trough SPI
    HAL_SPI_Transmit(&spi_channel,data2send,4,HAL_TIMEOUT);

    delayUS_ASM(10);
    //Set CS to High Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_SET);
}
void sendGenCommandIT(SPI_HandleTypeDef spi_channel,uint16_t CMD,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t data2send[4];
	uint16_t aux;
	uint16_t pec;
	//Data conversion of CMD to 4 byte array formed by CMD and CRC

	//CMD Data conversion
	aux = (CMD&(0x0700))>>8;
	data2send[0] = (uint8_t)(aux);

	aux = CMD&(0x00FF);
	data2send[1] = (uint8_t)(aux);

	//CRC Calculation
	pec = pec15(data2send,2);

	//CRC Data Conversion
	aux = (pec&(0xFF00))>>8;
	data2send[2] = (uint8_t)(aux);

	//It's important to point out that final 0 has been included in pec15() function
	aux = pec&(0x00FF);
    data2send[3] = (uint8_t)(aux);

    //Set CS to Low Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_RESET);

    //Delay 10us
    delayUS_ASM(10);

    //Send command data trough SPI
    HAL_SPI_Transmit_IT(&spi_channel,data2send,4);

    delayUS_ASM(10);
    //Set CS to High Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_SET);
}
// adress is a 4 bit number a3 a2 a1 a0
void sendGenAdressCommand(SPI_HandleTypeDef spi_channel,uint16_t CMD,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t data2send[4];
	uint16_t aux;
	uint16_t pec;

	//Data conversion of CMD to 4 byte array formed by CMD and CRC

	//CMD Data conversion
	aux = ((CMD&(0x0700))>>8)|((adress|0x10)<<3);
	data2send[0] = (uint8_t)(aux);

	aux = CMD&(0x00FF);
	data2send[1] = (uint8_t)(aux);

	//CRC Calculation
	pec = pec15(data2send,2);

	//CRC Data Conversion
	aux = (pec&(0xFF00))>>8;
	data2send[2] = (uint8_t)(aux);

	//It's important to point out that final 0 has been included in pec15() function
	aux = pec&(0x00FF);
    data2send[3] = (uint8_t)(aux);

    //Set CS to Low Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_RESET);

    //Delay 10us
    delayUS_ASM(10);

    //Send command data trough SPI
    HAL_SPI_Transmit(&spi_channel,data2send,4,HAL_TIMEOUT);

    delayUS_ASM(10);
    //Set CS to High Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_SET);
}
void sendGenAdressCommandIT(SPI_HandleTypeDef spi_channel,uint16_t CMD,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t data2send[4];
	uint16_t aux;
	uint16_t pec;

	//Data conversion of CMD to 4 byte array formed by CMD and CRC

	//CMD Data conversion
	aux = ((CMD&(0x0700))>>8)|((adress|0x10)<<3);
	data2send[0] = (uint8_t)(aux);

	aux = CMD&(0x00FF);
	data2send[1] = (uint8_t)(aux);

	//CRC Calculation
	pec = pec15(data2send,2);

	//CRC Data Conversion
	aux = (pec&(0xFF00))>>8;
	data2send[2] = (uint8_t)(aux);

	//It's important to point out that final 0 has been included in pec15() function
	aux = pec&(0x00FF);
    data2send[3] = (uint8_t)(aux);

    //Set CS to Low Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_RESET);

    //Delay 10us
    delayUS_ASM(10);

    //Send command data trough SPI
    HAL_SPI_Transmit_IT(&spi_channel,data2send,4);

    delayUS_ASM(10);
    //Set CS to High Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_SET);
}
void sendWriteCommand(SPI_HandleTypeDef spi_channel,uint16_t CMD,uint8_t *data, uint8_t length,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t data2send[4+length+2];
	uint16_t aux;
	uint16_t pec;

	//Data conversion of CMD to 4 byte array formed by CMD and CRC

	//CMD Data conversion
	aux = (CMD&(0x0700))>>8;
	data2send[0] = (uint8_t)aux;

	aux = CMD&(0x00FF);
	data2send[1] = (uint8_t)aux;

	//Command CRC Calculation
	pec = pec15(data2send,2);

	//CRC Data Conversion
	aux = (pec&(0xFF00))>>8;
	data2send[2] = (uint8_t)aux;

	//It's important to point out that final 0 has been included in pec15() function
	aux = pec&(0x00FF);
    data2send[3] = (uint8_t)aux;

    for(int i=0;i<length;i++){
    	data2send[4+i] = data[i];
    }

    //Data CRC Calculation
    pec = pec15(data,length);

    //CRC Data Conversion
	aux = (pec&(0xFF00))>>8;
	data2send[4+length] = (uint8_t)aux;

	//It's important to point out that final 0 has been included in pec15() function
	aux = pec&(0x00FF);
	data2send[4+length+1] = (uint8_t)aux;

    //Set CS to Low Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_RESET);

    //Delay 10 us
    delayUS_ASM(10);

    //Send command data trough SPI
    HAL_SPI_Transmit(&spi_channel,data2send,6+length,HAL_TIMEOUT);

    delayUS_ASM(10);

    //Set CS to High Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_SET);
}

// adress is a 4 bit number a3 a2 a1 a0
void sendAdressWriteCommand(SPI_HandleTypeDef spi_channel,uint16_t CMD,uint8_t *data, uint8_t length,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t data2send[4+length+2];
	uint16_t aux;
	uint16_t pec;

	//Data conversion of CMD to 4 byte array formed by CMD and CRC

	//CMD Data conversion
	aux = ((CMD&(0x0700))>>8)|((adress|0x10)<<3);
	data2send[0] = (uint8_t)aux;

	aux = CMD&(0x00FF);
	data2send[1] = (uint8_t)aux;

	//Command CRC Calculation
	pec = pec15(data2send,2);

	//CRC Data Conversion
	aux = (pec&(0xFF00))>>8;
	data2send[2] = (uint8_t)aux;

	//It's important to point out that final 0 has been included in pec15() function
	aux = pec&(0x00FF);
    data2send[3] = (uint8_t)aux;

    for(int i=0;i<length;i++){
    	data2send[4+i] = data[i];
    }

    //Data CRC Calculation
    pec = pec15(data,length);

    //CRC Data Conversion
	aux = (pec&(0xFF00))>>8;
	data2send[4+length] = (uint8_t)aux;

	//It's important to point out that final 0 has been included in pec15() function
	aux = pec&(0x00FF);
	data2send[4+length+1] = (uint8_t)aux;

    //Set CS to Low Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_RESET);

    //Delay 10 us
    delayUS_ASM(10);

    //Send command data trough SPI
    HAL_SPI_Transmit(&spi_channel,data2send,6+length,HAL_TIMEOUT);

    delayUS_ASM(10);

    //Set CS to High Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_SET);
}
void sendAdressWriteCommandIT(SPI_HandleTypeDef spi_channel,uint16_t CMD,uint8_t *data, uint8_t length,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t data2send[4+length+2];
	uint16_t aux;
	uint16_t pec;

	//Data conversion of CMD to 4 byte array formed by CMD and CRC

	//CMD Data conversion
	aux = ((CMD&(0x0700))>>8)|((adress|0x10)<<3);
	data2send[0] = (uint8_t)aux;

	aux = CMD&(0x00FF);
	data2send[1] = (uint8_t)aux;

	//Command CRC Calculation
	pec = pec15(data2send,2);

	//CRC Data Conversion
	aux = (pec&(0xFF00))>>8;
	data2send[2] = (uint8_t)aux;

	//It's important to point out that final 0 has been included in pec15() function
	aux = pec&(0x00FF);
    data2send[3] = (uint8_t)aux;

    for(int i=0;i<length;i++){
    	data2send[4+i] = data[i];
    }

    //Data CRC Calculation
    pec = pec15(data,length);

    //CRC Data Conversion
	aux = (pec&(0xFF00))>>8;
	data2send[4+length] = (uint8_t)aux;

	//It's important to point out that final 0 has been included in pec15() function
	aux = pec&(0x00FF);
	data2send[4+length+1] = (uint8_t)aux;

    //Set CS to Low Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_RESET);

    //Delay 10 us
    delayUS_ASM(10);

    //Send command data trough SPI
    HAL_SPI_Transmit_IT(&spi_channel,data2send,6+length);

    delayUS_ASM(10);

    //Set CS to High Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_SET);
}
//This function returns 1 if CRC calculated is OK, and return 0 if CRC calculated is not ok
uint8_t sendReadCommand(SPI_HandleTypeDef spi_channel,uint16_t CMD,uint8_t *data, uint8_t length,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t data2send[4],auxdata[length+2];
	uint16_t aux;
	uint16_t pec;

	//Data conversion of CMD to 4 byte array formed by CMD and CRC

	//CMD Data conversion
	aux = (CMD&(0x0700))>>8;
	data2send[0] = (uint8_t)aux;

	aux = (CMD&(0x00FF));
	data2send[1] = (uint8_t)aux;

	//CRC Calculation
	pec = pec15(data2send,2); 

	//CRC Data Conversion
	aux = (pec&(0xFF00))>>8;
	data2send[2] = (uint8_t)aux;

	//It's important to point out that final 0 has been included in pec15() function
	aux = pec&(0x00FF);
    data2send[3] = (uint8_t)aux;

    //Set CS to Low Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_RESET);

    //Delay 10 us
    delayUS_ASM(10);

    //Send command data trough SPI
    HAL_SPI_Transmit(&spi_channel,data2send,4,HAL_TIMEOUT);

    //Receive Register data trough SPI
    HAL_SPI_Receive(&spi_channel,auxdata,length+2,HAL_TIMEOUT);
		//HAL_SPI_TransmitReceive(&spi_channel,data2send,auxdata,4+length+2,SPI_TIMEOUT);
    //Delay 10 us
    delayUS_ASM(10);

    //Set CS to High Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_SET);
		
    //Calculate PEC from received data
    pec = pec15(auxdata,length);

    //Compare PEC
    if(((pec&(0xFF00))>>8)!=auxdata[length] || (pec&(0x00FF))!=auxdata[length+1]){
    	return 0;
    }

    for(int i = 0;i<length;i++){
		data[i] = auxdata[i];
    }
    return 1;
}
//This function returns 1 if CRC calculated is OK, and return 0 if CRC calculated is not ok
// adress is a 4 bit number a3 a2 a1 a0
//La lenght és sempre de 6, la màxima. És pot eliminar de la funció ja que no s'utilitza
uint8_t sendAdressReadCommand(SPI_HandleTypeDef spi_channel,uint16_t CMD,uint8_t *data, uint8_t length,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t data2send[4],auxdata[10+2]; //auxdata els primers 4 bytes son basura (tot FF) pq es el que rebo al enviar jo la comanda, els seguents 6 es on hi ha la info. Els dos ultims es el pec dels 6 que m'envia el xip
	uint16_t aux;
	uint16_t pec;
	//Data conversion of CMD to 4 byte array formed by CMD and CRC
	//CMD Data conversion
	aux = ((CMD&(0x0700))>>8)|((adress|0x10)<<3);
	data2send[0] = (uint8_t)aux;
	aux = (CMD&(0x00FF));
	data2send[1] = (uint8_t)aux;
	//CRC Calculation
	pec = pec15(data2send,2);
	//CRC Data Conversion
	aux = (pec&(0xFF00))>>8;
	data2send[2] = (uint8_t)aux;
	//It's important to point out that final 0 has been included in pec15() function
	aux = pec&(0x00FF);
	data2send[3] = (uint8_t)aux;
	//Set CS to Low Level
	HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_RESET);
	//Delay 10 us
	delayUS_ASM(10);
	//Send command data trough SPI and receive the info
	HAL_SPI_TransmitReceive(&spi_channel,data2send,auxdata,4+6+2,HAL_TIMEOUT);
	//Delay 10 us
	delayUS_ASM(10);
	//Set CS to High Level
	HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_SET);
	//Calculate PEC from received data
	pec = pec15_TransmitReceive(auxdata);
	//Compare PEC
	if(((pec&(0xFF00))>>8)!=auxdata[10] || (pec&(0x00FF))!=auxdata[11]){
		return 0;
	}
	for(int i = 4;i<10;i++){
		data[i-4] = auxdata[i];
	}
	return 1;
}
uint8_t sendAdressReadCommandIT(SPI_HandleTypeDef spi_channel,uint16_t CMD,uint8_t *data, uint8_t length,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t data2send[4],auxdata[length+2];
	uint16_t aux;
	uint16_t pec;

	//Data conversion of CMD to 4 byte array formed by CMD and CRC

	//CMD Data conversion
	aux = ((CMD&(0x0700))>>8)|((adress|0x10)<<3);
	data2send[0] = (uint8_t)aux;

	aux = (CMD&(0x00FF));
	data2send[1] = (uint8_t)aux;

	//CRC Calculation
	pec = pec15(data2send,2);

	//CRC Data Conversion
	aux = (pec&(0xFF00))>>8;
	data2send[2] = (uint8_t)aux;

	//It's important to point out that final 0 has been included in pec15() function
	aux = pec&(0x00FF);
    data2send[3] = (uint8_t)aux;

    //Set CS to Low Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_RESET);

    //Delay 10 us
    delayUS_ASM(10);

    //Send command data trough SPI
    HAL_SPI_Transmit_IT(&spi_channel,data2send,4);

    //Receive Register data trough SPI
    HAL_SPI_Receive_IT(&spi_channel,auxdata,length+2);

    //Delay 10 us
    delayUS_ASM(10);

    //Set CS to High Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_SET);

    //Calculate PEC from received data
    pec = pec15(auxdata,length);

    //Compare PEC
    if(((pec&(0xFF00))>>8)!=auxdata[length] || (pec&(0x00FF))!=auxdata[length+1]){
    	return 0;
    }

    for(int i = 0;i<length;i++){
		data[i] = auxdata[i];
    }
    return 1;
}
//This function is to init Configuration Register with default values and DCC disabled
void initConfigARegister(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t data2send[6];
	data2send[0] = (uint8_t)((GPIOx<<3)&0xF8)|((REFON<<2)&0x04)|((DTEN<<1)&0x02)|(ADCOPT&0x01);
	data2send[1] = (uint8_t)(UNDERVOLTAGE_TH&(0x00FF));
	data2send[2] = (uint8_t)(((OVERVOLTAGE_TH&(0x000F))<<4) | ((UNDERVOLTAGE_TH&(0x0F00))>>8));
	data2send[3] = (uint8_t)((OVERVOLTAGE_TH&(0x0FF0))>>4);
	data2send[4] = (uint8_t)(DCC&0x00FF);
	data2send[5] = (uint8_t)(((DCTO)<<4) | (((DCC&0x0F00))>>8));

	sendWriteCommand(spi_channel,CC_WRCFGA,data2send,6,CS_Port,CS_Pin);
}
//This function is to init Configuration Register with default values and DCC disabled
void initAdressConfigARegister(SPI_HandleTypeDef spi_channel,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t data2send[6];
	data2send[0] = (uint8_t)((GPIOx<<3)&0xF8)|((REFON<<2)&0x04)|((DTEN<<1)&0x02)|(ADCOPT&0x01);
	data2send[1] = (uint8_t)(UNDERVOLTAGE_TH&(0x00FF));
	data2send[2] = (uint8_t)(((OVERVOLTAGE_TH&(0x000F))<<4) | ((UNDERVOLTAGE_TH&(0x0F00))>>8));
	data2send[3] = (uint8_t)((OVERVOLTAGE_TH&(0x0FF0))>>4);
	data2send[4] = (uint8_t)(DCC&0x00FF);
	data2send[5] = (uint8_t)(((DCTO)<<4) | (((DCC&0x0F00))>>8));

	sendAdressWriteCommand(spi_channel,CC_WRCFGA,data2send,6,adress,CS_Port,CS_Pin);
}

void initConfigARegiser_DCTOto0(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t data2send[6];
	//This is to ensure that balancing is stopped
	DCC = 0x0000;
	data2send[0] = (uint8_t)((GPIOx<<3)&0xF8)|((REFON<<2)&0x04)|((DTEN<<1)&0x02)|(ADCOPT&0x01);
	data2send[1] = (uint8_t)(UNDERVOLTAGE_TH&(0x00FF));
	data2send[2] = (uint8_t)(((OVERVOLTAGE_TH&(0x000F))<<4) | ((UNDERVOLTAGE_TH&(0x0F00))>>8));
	data2send[3] = (uint8_t)((OVERVOLTAGE_TH&(0x0FF0))>>4);
	data2send[4] = (uint8_t)(DCC&0x00FF);
	data2send[5] = (uint8_t)(((0x00)<<4) | (((DCC&0x0F00))>>8));

	sendWriteCommand(spi_channel,CC_WRCFGA,data2send,6,CS_Port,CS_Pin);
}
//This function inits the LTC6811
void initLTC6811(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	//Init configA register
	initConfigARegister(spi_channel,CS_Port,CS_Pin);

	//Set ADC modes to normal mode
	setCellsVoltageMeasurementMode((uint16_t)CC_MD_7K_3k);
	setGPIOMeasurementMode((uint16_t)CC_MD_7K_3k);
	setStatusMeasurementMode((uint16_t)(CC_MD_7K_3k));
}

//Calling this function sends to command to enter into REFUP state
//Returns '1' if the change has been done
void enterRefUPstate(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	//If REFON bit is '0'
	if(!getRefONbit(spi_channel, CS_Port, CS_Pin)){
		//Set REFON bit to '1'
		initConfigARegister(spi_channel,CS_Port,CS_Pin);
	}
}

//This function returns the REFON bit
uint8_t getRefONbit(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t data2read[6];
	sendReadCommand(spi_channel,CC_RDCFGA,data2read,6,CS_Port,CS_Pin);

	return (((data2read[0])&0x04)>>2);
}

//This function is to set the value of PWM duty cicle when balancing with Discharge Timer time out
void setPWMRegister(SPI_HandleTypeDef spi_channel,uint8_t *PWMvalues,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t data2send[6];

	for(int i=0;i<6;i++){
		data2send[i] = (PWMvalues[2*i]&0x0F)|((PWMvalues[2*i+1]&0x0F)<<4);
	}

	sendWriteCommand(spi_channel,CC_WRPWM,data2send,6,CS_Port,CS_Pin);
}

//This function allows the user to externally control the state of the switches for cells balancing
void setManualBalancing(SPI_HandleTypeDef spi_channel,uint16_t DCCvalues,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
//	uint8_t data2send[6];

	/*data2send[0] = (uint8_t)((GPIOx<<3)&0xF8)|((REFON<<2)&0x04)|((DTEN<<1)&0x02)|(ADCOPT&0x01);
	data2send[1] = (uint8_t)(UNDERVOLTAGE_TH&(0x00FF));
	data2send[2] = (uint8_t)(((OVERVOLTAGE_TH&(0x000F))<<4) | ((UNDERVOLTAGE_TH&(0x0F00))>>8));
	data2send[3] = (uint8_t)((OVERVOLTAGE_TH&(0x0FF0))>>4);
	data2send[4] = (uint8_t)(DCCvalues&0x00FF);
	data2send[5] = (uint8_t)(((DCTO)<<4) | ((DCCvalues&0x0F00)>>8));

	sendWriteCommand(CC_WRCFGA,data2send,6);*/
	DCC = DCCvalues;
	initConfigARegister(spi_channel,CS_Port,CS_Pin);
}
void setAdressManualBalancing(SPI_HandleTypeDef spi_channel,uint16_t DCCvalues,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
//	uint8_t data2send[6];

	/*data2send[0] = (uint8_t)((GPIOx<<3)&0xF8)|((REFON<<2)&0x04)|((DTEN<<1)&0x02)|(ADCOPT&0x01);
	data2send[1] = (uint8_t)(UNDERVOLTAGE_TH&(0x00FF));
	data2send[2] = (uint8_t)(((OVERVOLTAGE_TH&(0x000F))<<4) | ((UNDERVOLTAGE_TH&(0x0F00))>>8));
	data2send[3] = (uint8_t)((OVERVOLTAGE_TH&(0x0FF0))>>4);
	data2send[4] = (uint8_t)(DCCvalues&0x00FF);
	data2send[5] = (uint8_t)(((DCTO)<<4) | ((DCCvalues&0x0F00)>>8));

	sendWriteCommand(CC_WRCFGA,data2send,6);*/
	DCC = DCCvalues;
	initAdressConfigARegister(spi_channel,adress,CS_Port,CS_Pin);
}

//This function allows the user to externally set the mode for ADC cells voltage measurement
void setCellsVoltageMeasurementMode(uint16_t mode){
	if((mode == (CC_MD_422_1k)) || (mode == (CC_MD_27k_14k)) || (mode == (CC_MD_26_2k)) || (mode == (CC_MD_7K_3k))){
		cellsVoltageMeasurementMode = mode;
	}else{
		cellsVoltageMeasurementMode = CC_MD_27k_14k; //Normal Mode by default
	}
}

//This function inits the measurement of all cells
void initCellsVoltageMeasurement(SPI_HandleTypeDef spi_channel,uint16_t dischargePermitted,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	sendGenCommand(spi_channel,(CC_ADCV)|(cellsVoltageMeasurementMode)|(dischargePermitted)|(CH_ALL),CS_Port,CS_Pin);
}
void initCellsVoltageMeasurementIT(SPI_HandleTypeDef spi_channel,uint16_t dischargePermitted,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	sendGenCommandIT(spi_channel,(CC_ADCV)|(cellsVoltageMeasurementMode)|(dischargePermitted)|(CH_ALL),CS_Port,CS_Pin);
}
//This function inits the measurement of all cells
void initAdressCellsVoltageMeasurement(SPI_HandleTypeDef spi_channel,uint16_t dischargePermitted,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	sendGenAdressCommand(spi_channel,(CC_ADCV)|(cellsVoltageMeasurementMode)|(dischargePermitted)|(CH_ALL),adress,CS_Port,CS_Pin);
}
void initAdressCellsVoltageMeasurementIT(SPI_HandleTypeDef spi_channel,uint16_t dischargePermitted,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	sendGenAdressCommandIT(spi_channel,(CC_ADCV)|(cellsVoltageMeasurementMode)|(dischargePermitted)|(CH_ALL),adress,CS_Port,CS_Pin);
}
//This function inits the measurement of two specific cells
void initCellXandYMeasurement(SPI_HandleTypeDef spi_channel,uint16_t dischargePermitted,uint16_t cells,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	if((cells == (CH_1and7)) || (cells == (CH_2and8)) || (cells == (CH_3and9)) || (cells == (CH_4and10)) || (cells == (CH_5and11)) || (cells == (CH_6and12))){
		sendGenCommand(spi_channel,(CC_ADCV)|(cellsVoltageMeasurementMode)|(dischargePermitted)|(cells),CS_Port,CS_Pin);
	}
}

//This function returns the value for cells voltage measurements, and also overvoltages and undervoltages flags
//Returned value is in uV
//Over and UnderVoltages flags order is -> C12OV C12UV C11OV C11UV .... C1OV C1UV
//This function return 1 if all data have been received correctly. If not, then 0 is returned.
uint8_t readCellsVoltages(SPI_HandleTypeDef spi_channel,uint32_t *cellsVoltages, uint32_t *overAndUnderVoltages,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
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

uint8_t readAdressCellsVoltages(SPI_HandleTypeDef spi_channel,uint32_t *cellsVoltages, uint32_t *overAndUnderVoltages,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t CVRegAdata[6];
	uint8_t CVRegBdata[6];
	uint8_t CVRegCdata[6];
	uint8_t CVRegDdata[6];
	uint8_t statusRegBdata[6];
	uint8_t CRC_count=0;
	uint8_t adressa=adress;
	uint8_t test=0;
	while(sendAdressReadCommand(spi_channel,CC_RDCVA,CVRegAdata,6,adressa,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		test=2;
		return test;
	}
	CRC_count = 0;
	while(sendAdressReadCommand(spi_channel,CC_RDCVB,CVRegBdata,6,adressa,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		test=3;
		return test;
	}
	CRC_count = 0;
	while(sendAdressReadCommand(spi_channel,CC_RDCVC,CVRegCdata,6,adressa,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		test=7;
		return test;
	}
	CRC_count = 0;
	while(sendAdressReadCommand(spi_channel,CC_RDCVD,CVRegDdata,6,adressa,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		test=8;
		return test;
	}
	CRC_count = 0;
	while(	sendAdressReadCommand(spi_channel,CC_RDSTATB,statusRegBdata,6,adressa,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		test=9;
		return test;
	}


	for(int i=0;i<3;i++){
		cellsVoltages[i] = ((CVRegAdata[2*i+1] << 8) | CVRegAdata[2*i])*100;//Values in uV
		cellsVoltages[3+i] = ((CVRegBdata[2*i+1] << 8) | CVRegBdata[2*i])*100;
		cellsVoltages[6+i] = ((CVRegCdata[2*i+1] << 8) | CVRegCdata[2*i])*100;
		cellsVoltages[9+i] = ((CVRegDdata[2*i+1] << 8) | CVRegDdata[2*i])*100;
	}

	*overAndUnderVoltages = (statusRegBdata[4] << 16) | (statusRegBdata[3] << 8) | statusRegBdata[2];
	return 1;
}
uint8_t readAdressCellsVoltagesIT(SPI_HandleTypeDef spi_channel,uint32_t *cellsVoltages, uint32_t *overAndUnderVoltages,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t CVRegAdata[6];
	uint8_t CVRegBdata[6];
	uint8_t CVRegCdata[6];
	uint8_t CVRegDdata[6];
	uint8_t statusRegBdata[6];
	uint8_t CRC_count=0;
	uint8_t adressa=adress;
	while(sendAdressReadCommandIT(spi_channel,CC_RDCVA,CVRegAdata,6,adressa,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}
	CRC_count = 0;
	while(sendAdressReadCommandIT(spi_channel,CC_RDCVB,CVRegBdata,6,adressa,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}
	CRC_count = 0;
	while(sendAdressReadCommandIT(spi_channel,CC_RDCVC,CVRegCdata,6,adressa,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}
	CRC_count = 0;
	while(sendAdressReadCommandIT(spi_channel,CC_RDCVD,CVRegDdata,6,adressa,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}
	CRC_count = 0;
	while(	sendAdressReadCommandIT(spi_channel,CC_RDSTATB,statusRegBdata,6,adressa,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
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
//This function returns the value for cells voltage measurements registers
//This function return 1 if all data have been received correctly. If not, then 0 is returned.
uint8_t readCellsVoltagesRegisters(SPI_HandleTypeDef spi_channel,uint16_t *cellsVoltagesRegisters,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t CVRegAdata[6];
	uint8_t CVRegBdata[6];
	uint8_t CVRegCdata[6];
	uint8_t CVRegDdata[6];
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

	for(int i=0;i<3;i++){
		cellsVoltagesRegisters[i] = ((CVRegAdata[2*i+1] << 8) | CVRegAdata[2*i]);
		cellsVoltagesRegisters[3+i] = ((CVRegBdata[2*i+1] << 8) | CVRegBdata[2*i]);
		cellsVoltagesRegisters[6+i] = ((CVRegCdata[2*i+1] << 8) | CVRegCdata[2*i]);
		cellsVoltagesRegisters[9+i] = ((CVRegDdata[2*i+1] << 8) | CVRegDdata[2*i]);
	}

	return 1;
}
//This function returns the value for cells voltage measurements registers
//This function return 1 if all data have been received correctly. If not, then 0 is returned.
uint8_t readAdressCellsVoltagesRegisters(SPI_HandleTypeDef spi_channel,uint16_t *cellsVoltagesRegisters,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t CVRegAdata[6];
	uint8_t CVRegBdata[6];
	uint8_t CVRegCdata[6];
	uint8_t CVRegDdata[6];
	uint8_t CRC_count=0;

	while(sendAdressReadCommand(spi_channel,CC_RDCVA,CVRegAdata,6,adress,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}
	CRC_count = 0;
	while(sendAdressReadCommand(spi_channel,CC_RDCVB,CVRegBdata,6,adress,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}
	CRC_count = 0;
	while(sendAdressReadCommand(spi_channel,CC_RDCVC,CVRegCdata,6,adress,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}
	CRC_count = 0;
	while(sendAdressReadCommand(spi_channel,CC_RDCVD,CVRegDdata,6,adress,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}
	CRC_count = 0;

	for(int i=0;i<3;i++){
		cellsVoltagesRegisters[i] = ((CVRegAdata[2*i+1] << 8) | CVRegAdata[2*i]);
		cellsVoltagesRegisters[3+i] = ((CVRegBdata[2*i+1] << 8) | CVRegBdata[2*i]);
		cellsVoltagesRegisters[6+i] = ((CVRegCdata[2*i+1] << 8) | CVRegCdata[2*i]);
		cellsVoltagesRegisters[9+i] = ((CVRegDdata[2*i+1] << 8) | CVRegDdata[2*i]);
	}

	return 1;
}
//This functions inits the openWireDetection in pullUp or pullDown mode
void initOpenWireDetection(SPI_HandleTypeDef spi_channel,uint16_t dischargePermitted,uint16_t pullUp,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	sendGenCommand(spi_channel,(CC_ADOW)|(cellsVoltageMeasurementMode)|(pullUp)|(dischargePermitted)|(CH_ALL),CS_Port,CS_Pin);
}
void initOpenWireDetectionAdress(SPI_HandleTypeDef spi_channel,uint16_t dischargePermitted,uint16_t pullUp,uint8_t adress, GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	sendGenAdressCommand(spi_channel,(CC_ADOW)|(cellsVoltageMeasurementMode)|(pullUp)|(dischargePermitted)|(CH_ALL),adress,CS_Port,CS_Pin);
}

//This function allows the user to externally set the mode for GPIO ADC measurements
void setGPIOMeasurementMode(uint16_t mode){
	if((mode == (CC_MD_422_1k)) || (mode == (CC_MD_27k_14k)) || (mode == (CC_MD_26_2k)) || (mode == (CC_MD_7K_3k))){
		GPIOMeasurementMode = mode;
	}else{
		GPIOMeasurementMode = CC_MD_7K_3k; //Normal Mode by default
	}
}

//Start GPIOs ADC Conversion with digital redundancy
void initAllGPIOADC_DigitalRed(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	sendGenCommand(spi_channel,(CC_ADAXD)|(GPIOMeasurementMode)|(CHG_ALL),CS_Port,CS_Pin);
}
//Start GPIOs ADC Conversion with digital redundancy
void initAdressAllGPIOADC_DigitalRed(SPI_HandleTypeDef spi_channel,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	sendGenAdressCommand(spi_channel,(CC_ADAXD)|(GPIOMeasurementMode)|(CHG_ALL),adress,CS_Port,CS_Pin);
}

//Start GPIOx ADC Conversion with digital redundancy
void initGPIOxADC_DigitalRed(SPI_HandleTypeDef spi_channel,uint16_t gpio,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	if((gpio == (CHG_GPIO1)) || (gpio == (CHG_GPIO2)) || (gpio == (CHG_GPIO3)) || (gpio == (CHG_GPIO4)) || (gpio == (CHG_GPIO5)) || (gpio == (CHG_2ndRef))){
		sendGenCommand(spi_channel,(CC_ADAXD)|(GPIOMeasurementMode)|(gpio),CS_Port,CS_Pin);
	}
}

//This function returns the value of GPIO readings results register in uV
//This function returns 0 if CRC fault detected, and returns 1 if CRC ok.
uint8_t readGPIOs(SPI_HandleTypeDef spi_channel,uint32_t *results,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t AuxRegAdata[6];
	uint8_t AuxRegBdata[6];

	uint8_t CRC_count=0;

	while(sendReadCommand(spi_channel,CC_RDAUXA,AuxRegAdata,6,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}
	CRC_count = 0;
	while(sendReadCommand(spi_channel,CC_RDAUXB,AuxRegBdata,6,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}

	for(int i=0;i<3;i++){
		results[i] = ((AuxRegAdata[2*i+1] << 8) | AuxRegAdata[2*i])*100;
		results[3+i] = ((AuxRegBdata[2*i+1] << 8) | AuxRegBdata[2*i])*100;
	}
	return 1;
}
uint8_t readGPIOsAdress(SPI_HandleTypeDef spi_channel,uint32_t *results,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t AuxRegAdata[6];
	uint8_t AuxRegBdata[6];

	uint8_t CRC_count=0;

	while(sendAdressReadCommand(spi_channel,CC_RDAUXA,AuxRegAdata,6,adress,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}
	CRC_count = 0;
	while(sendAdressReadCommand(spi_channel,CC_RDAUXB,AuxRegBdata,6,adress,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}

	for(int i=0;i<3;i++){
		results[i] = ((AuxRegAdata[2*i+1] << 8) | AuxRegAdata[2*i])*100;
		results[3+i] = ((AuxRegBdata[2*i+1] << 8) | AuxRegBdata[2*i])*100;
	}
	return 1;
}

//This functions start Overlap Measurement of 7 cell voltage
void initOL(SPI_HandleTypeDef spi_channel,uint16_t dischargePermitted,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	sendGenCommand(spi_channel,CC_ADOL|cellsVoltageMeasurementMode|dischargePermitted,CS_Port,CS_Pin);
}

//This function reads value of 7 cell voltage measurement and 8 cell voltage measurement in uV
//This function is for Overlap Measurement of 7 cell utility
//First element is ADC1 measurement, Second Elements is ADC2 measurement
uint8_t readOLValues(SPI_HandleTypeDef spi_channel,uint32_t *overlapValues,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t CVRegCdata[6];
	uint8_t CRC_count = 0;

	CRC_count = 0;
	while(sendReadCommand(spi_channel,CC_RDCVC,CVRegCdata,6,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}

	overlapValues[0] = (CVRegCdata[3]<<8 | CVRegCdata[2])*100;
	overlapValues[1] = (CVRegCdata[1]<<8 | CVRegCdata[0])*100;

	return 1;
}
uint8_t readAdressOLValues(SPI_HandleTypeDef spi_channel,uint32_t *overlapValues,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t CVRegCdata[6];
	uint8_t CRC_count = 0;

	CRC_count = 0;
	while(sendAdressReadCommand(spi_channel,CC_RDCVC,CVRegCdata,6,adress,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}

	overlapValues[0] = (CVRegCdata[3]<<8 | CVRegCdata[2])*100;
	overlapValues[1] = (CVRegCdata[1]<<8 | CVRegCdata[0])*100;

	return 1;
}


//This function allows the user to externally set the mode for status ADC measurements
void setStatusMeasurementMode(uint16_t mode){
	if((mode == (CC_MD_422_1k)) || (mode == (CC_MD_27k_14k)) || (mode == (CC_MD_26_2k)) || (mode == (CC_MD_7K_3k))){
		StatusMeasurementMode = mode;
	}else{
		StatusMeasurementMode = CC_MD_7K_3k; //Normal Mode by default
	}
}

//This function starts all status Group ADC conversion with digital redundancy
void initStatusCheckAll(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	sendGenCommand(spi_channel,(CC_ADSTATD)|(StatusMeasurementMode)|(CHST_ALL),CS_Port,CS_Pin);
}

//This function starts specific status Group ADC conversion with digital redundancy
void initStatusCheck(SPI_HandleTypeDef spi_channel,uint16_t status,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	if((status == (CHST_ITMP)) || (status == (CHST_SC)) || (status == (CHST_VA)) || (status == (CHST_VD))){
		sendGenCommand(spi_channel,CC_ADSTATD|StatusMeasurementMode|status,CS_Port,CS_Pin);
	}
}

//This functions read results from status measurements in microvolts for SC, VA y VB, and in mDegs for IDT
uint8_t readStatusResults(SPI_HandleTypeDef spi_channel,uint32_t *sumOfCells, int32_t *internalDieTemperature, uint32_t *analogVoltage, uint32_t *digitalVoltage,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t statusRegisterGA[6];
	uint8_t statusRegisterGB[6];
	uint8_t CRC_count = 0;

	CRC_count = 0;
	while(sendReadCommand(spi_channel,CC_RDSTATA,statusRegisterGA,6,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}
	CRC_count = 0;
	while(sendReadCommand(spi_channel,CC_RDSTATB,statusRegisterGB,6,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}

	*sumOfCells = (statusRegisterGA[1] << 8 | statusRegisterGA[0])*100*20;
	*internalDieTemperature = (int32_t)(statusRegisterGA[3] << 8 | statusRegisterGA[2])*1000/75-273000;
	*analogVoltage = (statusRegisterGA[5] << 8 | statusRegisterGA[4])*100;
	*digitalVoltage = (statusRegisterGB[1] << 8 | statusRegisterGB[0])*100;

	return 1;
}

//This functions read results from status measurements in microvolts for SC, VA y VB, and in mDegs for IDT
uint8_t readAdressStatusResults(SPI_HandleTypeDef spi_channel,uint32_t *sumOfCells, int32_t *internalDieTemperature, uint32_t *analogVoltage, uint32_t *digitalVoltage,GPIO_TypeDef* CS_Port,uint16_t CS_Pin,uint8_t adress){
	uint8_t statusRegisterGA[6];
	uint8_t statusRegisterGB[6];
	uint8_t CRC_count = 0;

	CRC_count = 0;
	while(sendAdressReadCommand(spi_channel,CC_RDSTATA,statusRegisterGA,6,adress,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}
	CRC_count = 0;
	while(sendAdressReadCommand(spi_channel,CC_RDSTATB,statusRegisterGB,6,adress,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}
	*sumOfCells = (statusRegisterGA[1] << 8 | statusRegisterGA[0])*100*20;//microVolts
	*internalDieTemperature = (int32_t)(statusRegisterGA[3] << 8 | statusRegisterGA[2])*1000/75-273000;//miliCelsius
	*analogVoltage = (statusRegisterGA[5] << 8 | statusRegisterGA[4])*100;//microVolts
	*digitalVoltage = (statusRegisterGB[1] << 8 | statusRegisterGB[0])*100;//microVolts
	return 1;
}

//This function clears Cell Voltage Registers A,B,C,D (clear means that they're set to 0xFF)
void clearCellVoltagesRegisterGroup(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	sendGenCommand(spi_channel,CC_CLRCLL,CS_Port,CS_Pin);
}
void clearAdressCellVoltagesRegisterGroup(SPI_HandleTypeDef spi_channel,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	sendGenAdressCommand(spi_channel,CC_CLRCLL,adress,CS_Port,CS_Pin);
}

//This function clears Auxiliary Registers Groups A and B
void clearAuxiliaryRegisterGroup(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	sendGenCommand(spi_channel,CC_CLRAUX,CS_Port,CS_Pin);
}

//This function clears Status Register Groups A and B
void clearStatusRegisterGroup(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	sendGenCommand(spi_channel,CC_CLRSTAT,CS_Port,CS_Pin);
}

//This function clears S control Register Group
void clearSControlRegisterGroup(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	sendGenCommand(spi_channel,CC_CLRSCTRL,CS_Port,CS_Pin);
}

//This functions sends Polling Commands
void initMUXDiag(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	sendGenCommand(spi_channel,CC_DIAGN,CS_Port,CS_Pin);
}

//Read MUX state, 1 means failed, 0 means passed
uint8_t readMUXState(SPI_HandleTypeDef spi_channel,uint8_t *muxState,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t statusRegisterGB[6];
	uint8_t CRC_count = 0;

	CRC_count = 0;
	while(sendReadCommand(spi_channel,CC_RDSTATB,statusRegisterGB,6,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}

	//Read and return MUXFAIL bit
	*muxState = (uint8_t)((statusRegisterGB[5]&(0x02))>>1);
	return 1;
}

uint8_t readAdressMUXState(SPI_HandleTypeDef spi_channel,uint8_t *muxState,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t statusRegisterGB[6];
	uint8_t CRC_count = 0;

	CRC_count = 0;
	while(sendAdressReadCommand(spi_channel,CC_RDSTATB,statusRegisterGB,6,adress,CS_Port,CS_Pin) != 1 && CRC_count < CRC_LIMIT){CRC_count++;};
	if(CRC_count == CRC_LIMIT){
		return 0;
	}

	//Read and return MUXFAIL bit
	*muxState = (uint8_t)((statusRegisterGB[5]&(0x02))>>1);
	return 1;
}
//Read Device Revision Code
uint8_t readRevisionCode(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	uint8_t statusRegisterGB[6];

	sendReadCommand(spi_channel,CC_RDSTATB,statusRegisterGB,6,CS_Port,CS_Pin);

	//Read and return Device Revisison Code
	return (uint8_t)((statusRegisterGB[5]&(0xF0))>>4);
}

//Send Poll ADC Conversion Status and Read ADC Status
//It tries POLL_ADC_TIMES to read 0xFF in SDO Line from LTC6811
uint8_t sendPollADCStatus(SPI_HandleTypeDef spi_channel,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	//Send Poll ADC Command
	uint8_t data2send[4];
	uint16_t aux;
	uint16_t pec;
	//Data conversion of CMD to 4 byte array formed by CMD and CRC
	uint16_t CMD = CC_PLADC;
	//CMD Data conversion
	aux = (CMD&(0x0700))>>8;
	data2send[0] = (uint8_t)aux;
	aux = CMD&(0x00FF);
	data2send[1] = (uint8_t)aux;
	//CRC Calculation
	pec = pec15(data2send,2);
	//CRC Data Conversion
	aux = (pec&(0xFF00))>>8;
	data2send[2] = (uint8_t)aux;
	//It's important to point out that final 0 has been included in pec15() function
	aux = pec&(0x00FF);
    data2send[3] = (uint8_t)aux;
    //Set CS to Low Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_RESET);
    //Delay
    delayUS_ASM(10);
    //Send command data trough SPI
    HAL_SPI_Transmit(&spi_channel,data2send,4,SPI_TIMEOUT);
    //Check POLL_ADC_TIMES times if the conversion has been finished
    uint8_t finished = 0;
    uint8_t auxdata;
    /*uint8_t timesToCheck = (uint8_t)POLL_ADC_TIMES;
    for(uint8_t i = 0;i<timesToCheck && finished == 0;i++){
        HAL_SPI_Receive(&hspi3,&auxdata,1,SPI3_TIMEOUT);
        if(auxdata != 0x00){
        	finished = 1;
        }
    }*/
    HAL_SPI_Receive(&spi_channel,&auxdata,1,1);
    if(auxdata != 0x00){ //Com que MISO és open-drain, si no rebem re auxdata = 1
		finished = 1;
	}
    //Delay
    delayUS_ASM(10);
    //Set CS High
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_SET);
    return finished;
}
//It tries POLL_ADC_TIMES to read 0xFF in SDO Line from LTC6811
uint8_t sendAdressPollADCStatus(SPI_HandleTypeDef spi_channel,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	//Send Poll ADC Command
	uint8_t data2send[4];
	uint16_t aux;
	uint16_t pec;

	//Data conversion of CMD to 4 byte array formed by CMD and CRC
	uint16_t CMD = CC_PLADC;
	//CMD Data conversion
	aux = ((CMD&(0x0700))>>8)|((adress|0x10)<<3);
	data2send[0] = (uint8_t)aux;

	aux = CMD&(0x00FF);
	data2send[1] = (uint8_t)aux;

	//CRC Calculation
	pec = pec15(data2send,2);

	//CRC Data Conversion
	aux = (pec&(0xFF00))>>8;
	data2send[2] = (uint8_t)aux;

	//It's important to point out that final 0 has been included in pec15() function
	aux = pec&(0x00FF);
    data2send[3] = (uint8_t)aux;

    //Set CS to Low Level
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_RESET);

    //Delay
    delayUS_ASM(10);

    //Send command data trough SPI
    HAL_SPI_Transmit(&spi_channel,data2send,4,SPI_TIMEOUT);

    //Check POLL_ADC_TIMES times if the conversion has been finished
    uint8_t finished = 0;
    uint8_t auxdata;

    /*uint8_t timesToCheck = (uint8_t)POLL_ADC_TIMES;
    for(uint8_t i = 0;i<timesToCheck && finished == 0;i++){
        HAL_SPI_Receive(&hspi3,&auxdata,1,SPI3_TIMEOUT);
        if(auxdata != 0x00){
        	finished = 1;
        }
    }*/

    HAL_SPI_Receive(&spi_channel,&auxdata,1,1);
    if(auxdata != 0x00){
		finished = 1;
	}

    //Delay
    delayUS_ASM(10);

    //Set CS High
    HAL_GPIO_WritePin(CS_Port,CS_Pin,GPIO_PIN_SET);
    return finished;
}
//This function starts the cell voltage self test in ST[1..0] mode
void initCellVoltageSelfTest(SPI_HandleTypeDef spi_channel,uint16_t selfTest,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	if((selfTest == ((uint16_t)ST_1)) || (selfTest == ((uint16_t)ST_2))){
		sendGenCommand(spi_channel,CC_CVST | cellsVoltageMeasurementMode | selfTest,CS_Port,CS_Pin);
	}
}
void initAdressCellVoltageSelfTest(SPI_HandleTypeDef spi_channel,uint16_t selfTest,uint8_t adress,GPIO_TypeDef* CS_Port,uint16_t CS_Pin){
	if((selfTest == ((uint16_t)ST_1)) || (selfTest == ((uint16_t)ST_2))){
		sendGenAdressCommand(spi_channel,CC_CVST | cellsVoltageMeasurementMode | selfTest,adress,CS_Port,CS_Pin);
	}
}
void delayUS_ASM(uint32_t delay_us) {
	__HAL_TIM_SET_COUNTER(&htim2,0);
	while ((__HAL_TIM_GET_COUNTER(&htim2)) <delay_us) {
	}
		
}
