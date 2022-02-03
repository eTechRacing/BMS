#include <xc.h>
#include <stdint.h>
#include "../inc/ecan1_config.h"
#include "../inc/ecan2_config.h"

void initialization ( void ) {

    ANSELA = 0; // Port A - digital
    ANSELB = 0; // Port B - digital
    ANSELC = 0; // Port C - digital
    
    TRISAbits.TRISA0 = 0; // A0 output - Error LED control
    PORTAbits.RA0 = 0;  // Error LED (0-off, 1-on)
    
    TRISAbits.TRISA1 = 0; // A1 output - LED2 control
    PORTAbits.RA1 = 0;  // LED2 (0-off, 1-on)
    
    TRISAbits.TRISA7 = 0; // A7 output - AIR+ control
    PORTAbits.RA7 = 0;  // AIR+ (0-open, 1-closed)
    
    TRISAbits.TRISA10 = 0; // A10 output - AIR- control
    PORTAbits.RA10 = 0; // AIR- (0-open, 1-closed)
    
    TRISBbits.TRISB0 = 0; // B0 output - LED1 control
    PORTBbits.RB0 = 0;  // LED1 (0-off, 1-on)
    
    TRISBbits.TRISB1 = 0; // B1 output - CAN control
    PORTBbits.RB1 = 0;  // CAN (0-deactivated, 1-activated)
    
    TRISBbits.TRISB9 = 1; // B9 input - Interlock monitoring (0-no connected, 1-connected)
    
    TRISBbits.TRISB11 = 0; // B11 output - BMS OK control
    PORTBbits.RB11 = 0; // BMS_OK ()
    
    TRISBbits.TRISB12 = 0; // B12 output - Fans 1 control
    PORTBbits.RB12 = 0; // Fans 1 (0-deactivated, 1-activated)
    
    TRISBbits.TRISB13 = 0; // B13 output - Fans 2 control
    PORTBbits.RB13 = 0; // Fans 2 (0-deactivated, 1-activated)
    
    TRISBbits.TRISB14 = 0; // B14 output - Precharge relay control
    PORTBbits.RB14 = 0; // Precharge relay (0-open, 1-closed))
    
    TRISCbits.TRISC6 = 1; // C6 input - Balancing (0-activated, 1-deactivated)
    
    // Unused I/O configured as output and ¿driven to a logic low state?
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA8 = 0;
    TRISAbits.TRISA9 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB6 = 0;
    TRISBbits.TRISB7 = 0;
    TRISBbits.TRISB8 = 0;
    TRISBbits.TRISB10 = 0;
    TRISBbits.TRISB15 = 0;
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC7 = 0;
    TRISCbits.TRISC8 = 0;
    TRISCbits.TRISC9 = 0;
    
    // Clear Interrupt Flags
    IFS0 = 0;
    IFS1 = 0;
    IFS2 = 0;
    IFS3 = 0;
    IFS4 = 0;

    // Oscilator Configuration
    OSCCONbits.NOSC = 2; // Primary Oscillator (MS, HS, EC) without PLL
    while(OSCCONbits.COSC !=2);
    RCONbits.SWDTEN = 0; // Disable WDT
    
    // Timer 2 Configuration - overflow 820 us
    T2CONbits.TSIDL = 0; //Continues module operation in Idle mode
    T2CONbits.TCS = 0; // Select internal instruction cycle clock
    T2CONbits.TCKPS = 0; // Select 1:1 Prescaler
    T2CONbits.T32 = 0; // Disable 32-bit Timer mode
    T2CONbits.TGATE = 0; // Disable Gated Timer mode
    T2CONbits.TON = 1; // Start Timer
    TMR2= 0xD8F0; // Time Register
    IEC0bits.T2IE = 1; // Enable Timer2 interrupt
    
    // CAN Initialization
    Ecan1Init();
    Ecan2Init();
    
    // Enable CAN Interrupts
    IEC2bits.C1IE = 1;
    IEC3bits.C2IE = 1;

}

