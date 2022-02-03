#ifndef _MAIN_
#define	_MAIN_


#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include "ecan1_config.h"
#include "ecan2_config.h"
#include "common.h"


// *****************************************************************************
//                              FUSES
// *****************************************************************************

#pragma config ICS = PGD1           // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config JTAGEN = OFF         // JTAG Enable bit (JTAG is disabled)

// FPOR
//#pragma config BOREN = ON         //*SOLO dsPIC256 - Brown-out Reset (BOR) Detection Enable bit (BOR is enabled)
#pragma config ALTI2C1 = ON         // Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF        // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)
#pragma config WDTWIN = WIN25       // Watchdog Window Select bits (WDT Window is 25% of WDT period)

// FWDT
#pragma config WDTPOST = PS32768    // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128       // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON          // PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF         // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF         // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FOSC
#pragma config POSCMD = HS          // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
#pragma config OSCIOFNC = OFF       // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = OFF        // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSECME       // Clock Switching Mode bits (Clock switching is enabled,Fail-safe Clock Monitor is disabled)

// FOSCSEL
#pragma config FNOSC = PRI          // Oscillator Source Selection (Internal Fast RC (FRC))
//#pragma config PWMLOCK = ON       //*SOLO dsPIC256 - PWM Lock Enable bit (Certain PWM registers may only be written after key sequence)
#pragma config IESO = OFF           // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

// FGS
#pragma config GWRP = OFF           // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF            // General Segment Code-Protect bit (General Segment Code protect is Disabled)


// *****************************************************************************
//                           BUFFERS DE CAN
// *****************************************************************************

// Define ECAN Message Buffers
__eds__ ECAN1MSGBUF ecan1msgBuf __attribute__( (space(eds), aligned(ECAN1_MSG_BUF_LENGTH * 16)) );
__eds__ ECAN2MSGBUF ecan2msgBuf __attribute__( (space(eds), aligned(ECAN2_MSG_BUF_LENGTH * 16)) );


// *****************************************************************************
//                   Declaración de funciones externas
// *****************************************************************************

//---------------------------USO GENERAL---------------------------------------
void Timer2config (void);
void OscConfig( void );
void ClearIntrflags( void );
void Initialization (void);

//---------------------------CAN 1 Y 2-----------------------------------------

void can1_setbufferid( uint16_t buf, int32_t txIdentifier, uint16_t ide, uint16_t remoteTransmit );
void can1_write( uint16_t buf, uint16_t dataLength, uint16_t cobid, uint16_t data1, uint16_t data2, uint16_t data3, uint16_t data4 );
void can1_writebytes( uint16_t buf, uint16_t dataLength, uint16_t cobid, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8 );
uint8_t can1_getrxbuf(void);
uint8_t can1_isempty(void);
uint32_t can1_getcobid(uint8_t buf);
uint8_t can1_getlength(uint8_t buf);
uint16_t can1_getdata(uint8_t buf, uint8_t num);
void can2_setbufferid( uint16_t buf, int32_t txIdentifier, uint16_t ide, uint16_t remoteTransmit );
void can2_write( uint16_t buf, uint16_t dataLength, uint16_t cobid, uint16_t data1, uint16_t data2, uint16_t data3,uint16_t data4 );
void can2_writebytes( uint16_t buf, uint16_t dataLength, uint16_t cobid, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8 );
uint8_t can2_getrxbuf(void);
uint8_t can2_isempty(void);
uint32_t can2_getcobid(uint8_t buf);
uint8_t can2_getlength(uint8_t buf);
uint16_t can2_getdata(uint8_t buf, uint8_t num);

//--------------------------------I2C-----------------------------------------

void I2C2Write (short byte1, short byte2, short byte3, short byte4, short byte5, short byte6, short byte7, short byte8);
void I2C1Write (short data);
void I2C2Init(void);
void I2C1Init(void);

//--------------------------------SPI-------------------------------------------

void Init_SPI ( void );
void Write_SPI ( short command );
short ReadWrite_SPI ( short command );

//--------------------------------UART-----------------------------------------

void UARTconfig (void);

//-----------------------------LTC6811-----------------------------------------

void START_ADC_VOLTAGES (void);
void READ_VOLTAGES(uint16_t *voltaje, uint16_t total, uint16_t v_min, uint16_t v_max);
void BLANCEO(uint16_t v_cell_min, uint16_t v_balanceo, uint16_t v_min, uint8_t diff_max, uint16_t *voltaje, uint16_t temp_max, uint16_t temp_slave_max, uint8_t balan);

//-----------------------------LTC1296-----------------------------------------
void ADC_LTC1296(uint16_t *temp);

//-----------------------------LTC2400-----------------------------------------
long ADC_LTC2400(uint8_t charge, uint32_t offset);
//long ADC_LTC2400(uint8_t charge);
long Calibrate_ADC_LTC2400(void);

//-----------------------------FORNT------------------------------------
void CANCLEAN(void);


#endif