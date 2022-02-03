/*
NOMBRE DEL ARCHIVO:             Master.c

DESCRIPCIÓN:                    Firmware de la Master de BMS en su versión 
                                beta. Funciona a través de una línea de CAN
                                con 15 slaves y el transductor de corriente "CAB
                                300-C/SP3-000". Externamente se puede comunicar 
                                con el coche de la temporada 2018-2019 (Romeo) y
                                con el de la 2020-2021 (e-Mili).

AUTORES                         Alex Sánchez
(hackermans supremos):          Lucas Bouzón
       
MENSAJES CAN ENVIADOS:
    - AIRs_State:               Estado de los AIRs y del relé de precarga
    - Voltages:                 Voltaje máximo, mínimo y total del Accumulator
    - Cell_Temperatures:        Temperatura más alta, más baja y media de las
                                celdas
    - Onboard_Temperature_actuals:     Temperatura más alta, más baja y media de las
                                resistencias de balanceo
    - Current_SoC_SoH:          Corriente que sale o entra del Accumulator, State
                                of Charge (no calculado aún) y State of Health (no
                                calculado aún)
    - StateAndFailReport:       Estado de los diferentes errores que se pueden dar
    - Charger_Params:           Parámetros máximos del cargador
    - Keep_Alive:               Señal de Keep Alive.

FECHA DE LA VERSIÓN ORIGINAL:   ‎16‎ de ‎agosto‎ de ‎2019

FECHA DE LA REVISIÓN:           Diciembre de 2021

VERSIÓN:                        Beta 1.01

e-Tech Racing 2018-2021 ©

*/


/*
El programa está estructurado de la siguiente manera:
    - Importación de librerías
    - Definición de pins
    - Definición de variables
    - Definición de constantes
    - ? (lo de los tt_)
    - Interrupción de CAN 1 (coche/cargador)
    - Interrupción de CAN 2 (AMS CAN)
    - ? (DMAs)
    - ? (CAN Clean)
    - Definición de external_CAN_management()
    - Definición de initialization()
    - main
        - Asignación inicial de variables
        - Cálculo de SoC inicial
        - Loop principal con los cálculos de variables y estados

*/

// ***************************************************************************** // 
//                           Importación de librerías                            // 
// ***************************************************************************** // 

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "../inc/ecan1_config.h"
#include "../inc/ecan2_config.h"
#include "../inc/common.h"
#include "../inc/main.h"
#include "../inc/calculations.h"
#include "../inc/state_and_control.h"
#include "../inc/constants.h"
#include "../inc/initialization.h"
#include "../inc/CAN1_mgmt.h"


// ***************************************************************************** // 
//                            Definición de pins                                 // 
// ***************************************************************************** // 

#define Error_LED               PORTAbits.RA0 
#define LED2                    PORTAbits.RA1
#define AIRplus_Control         PORTAbits.RA10
#define AIRminus_Control        PORTAbits.RA7
#define LED1                    PORTBbits.RB0
#define CAN_Control             PORTBbits.RB1
#define Interlock_Monitoring    PORTBbits.RB9
#define BMS_OK                  PORTBbits.RB11
#define Fans1_Control           PORTBbits.RB12
#define Fans2_Control           PORTBbits.RB13
#define Precharge_Control       PORTBbits.RB14
#define Balancing               PORTCbits.RC6


// ***************************************************************************** //
//                     Definición de variables globales                          //
// ***************************************************************************** //
int rbuffer_internal = 0, cobid_internal = 0, rbuffer_external = 0, cobid_external = 0;
int AIRs_request, AIRs_state, do_precharge, do_precharge_charger, open_relays, secuence_0 = 0;
int AIRminus_state, AIRplus_state, precharge_state;
short close_AIRs, close_AIRs_charger, relays_state, sequence_state;
int voltage_disconnection, overvoltage, undervoltage, temperature_disconnection, overtemperature, undertemperature, current_disconnection, CAN_disconnection;
int CAN_NOK, BMS_OK_state, stop_charging;
short shutdown_signal, voltage_NOK, temperature_NOK, current_NOK, shutdown_NOK;
int synchronism, current_synchronism, CAN_count, CAN_count_int, charger_count, BMS_alive;
long CAN_alive;
short alive_count;
int voltage[140], temperature[120];
int current_buffer[4], SoC, V_init, SoC_init, SoC_actual;
// NO SÉ COMO VA EXACTAMENTE EL LEM300, POR QUÉ UN BUFFER (interpreto que el sensor te manda el valor de corriente partido en dos bytes pero el buffer es de 5 posiciones lol. segun datasheet son 32 bits que son 4 bytes es que yo no entiendo nada de verdad)
long total_voltage_actual, current; 
int min_cell_voltage_actual, max_cell_voltage_actual;
int min_cell_temperature_actual, max_cell_temperature_actual, avg_cell_temperature_actual, min_onboard_temperature_actual, max_onboard_temperature_actual, avg_onboard_temperature_actual, temperature_type;
int fans_state, balancing_state, balancing_button, interlock_state, divide_current, sfr1, sfr2, sfr3;
int balancing_0 = 0, balancing_1 = 0, balancing_2 = 0, balancing_3 = 0, balancing_4 = 0, balancing_5 = 0, balancing_6 = 0, balancing_7 = 0, balancing_8 = 0, balancing_9 = 0, balancing_10 = 0, balancing_11 = 0, balancing_12 = 0, balancing_13 = 0, balancing_14 = 0;
int tt_slave0, tt_slave1, tt_slave2, tt_slave3, tt_slave4, tt_slave5, tt_slave6, tt_slave7, tt_slave8, tt_slave9, tt_slave10, tt_slave11, tt_slave12, tt_slave13, tt_slave14, tt_hall;
int divide_count, charge;
short sleep_mode;
int tt_synchronism, tt_buffer;
long tt_voltage, tt_temperature, tt_current, tt_divide, tt_CAN, tt_balancing;
short tt_shutdown;
int i,k;



// ----------------------------------------------------------------------------------------


//TIMER2
// Entiendo que es una función que tiene unos contadores a los que suma uno cada vez que 
// el timer interno da un pulso. No lo acabo de pillar porque no se llama nunca.
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
{   
    TMR2 = 0xD8F0;
    CAN_alive++;
    if ( voltage_NOK == 1 )
    {
        tt_voltage++;
    }
    if ( temperature_NOK == 1 )
    {
        tt_temperature++;
    }
    if ( current_NOK == 1 )
    {
        tt_current++;
    }
    if ( shutdown_NOK == 1 )
    {
        tt_shutdown++;
    }
    tt_synchronism++;
    tt_hall++;
    tt_buffer++;
    tt_divide++;
    tt_CAN++;
    tt_balancing++;
    if( CAN_NOK == 1 )
    {
        tt_slave0++;
        tt_slave1++;
        tt_slave2++;
        tt_slave3++;
        tt_slave4++;
        tt_slave5++;
        tt_slave6++;
        tt_slave7++;
        tt_slave8++;
        tt_slave9++;
        tt_slave10++;
        tt_slave11++;
        tt_slave12++;
        tt_slave13++;
        tt_slave14++;
    }
    IFS0bits.T2IF = 0;
    
}



//CAN1 (CAR_EXTERNAL_CAN) general interruption
// BMS only needs synchronism AIRs_request from ETAS or charger.


// CAN2 (BMS_INTERNAL_CAN) general interruption
// Reads voltages, temperatures and balancing states from BMS Slaves and current from a Fluxgate sensor 




//DMAs

// Por qué se tiene que hacer esto?

void __attribute__ ( (interrupt, no_auto_psv) ) _DMA0Interrupt( void )
{
    IFS0bits.DMA0IF = 0;    // Clear the DMA0 Interrupt Flag;
}

void __attribute__ ( (interrupt, no_auto_psv) ) _DMA1Interrupt( void )
{
    IFS0bits.DMA1IF = 0;    // Clear the DMA1 Interrupt Flag;
}

void __attribute__ ( (interrupt, no_auto_psv) ) _DMA2Interrupt( void )
{
    IFS1bits.DMA2IF = 0;    // Clear the DMA2 Interrupt Flag;
}

void __attribute__ ( (interrupt, no_auto_psv) ) _DMA3Interrupt( void )
{
    IFS2bits.DMA3IF = 0;    // Clear the DMA3 Interrupt Flag;
}

// *****************************************************************************
//                                    MAIN
// *****************************************************************************

void CANCLEAN(void)
{
    if (C1RXFUL1 != 0)
    {
        C1RXFUL1 = 0;         //Desmarcamos el buffer como lleno
    }

    if (C1RXFUL2 != 0)

 {
        C1RXFUL2 = 0;         //Desmarcamos el buffer como lleno
    }

    // ----------- EL FORNT ES EL PUTOS ----------------------

    // No sé qué es esto pero sí, el fornt sabe cosas
    C1TR01CONbits.TXABT0 = 0;
    C1TR01CONbits.TXLARB0 = 0;
    C1TR01CONbits.TXERR0 = 0;
    C1TR01CONbits.TXABT1 = 0;
    C1TR01CONbits.TXLARB1 = 0;
    C1TR01CONbits.TXERR1 = 0;

    //C1TR23CONbits.TXABT2 = 0;
    //C1TR23CONbits.TXLARB2 = 0;
    //C1TR23CONbits.TXERR2 = 0;
    //C1TR23CONbits.TXABT3 = 0;
    //C1TR23CONbits.TXLARB3 = 0;
    //C1TR23CONbits.TXERR3 = 0;
}



int main( void ) {

    initialization();
    
    overvoltage = 1; // Mirar porque la logica de esta señal va al reves (1-OK, 0-error)
    undervoltage = 1; // Mirar porque la logica de esta señal va al reves (1-OK, 0-error)
    overtemperature = 1; // Mirar porque la logica de esta señal va al reves (1-OK, 0-error)
    undertemperature = 1; // Mirar porque la logica de esta señal va al reves (1-OK, 0-error)
    voltage_disconnection = 1; // Mirar porque la logica de esta señal va al reves (1-OK, 0-error)
    temperature_disconnection = 1; // Mirar porque la logica de esta señal va al reves (1-OK, 0-error)
    current_disconnection = 1; // Mirar porque la logica de esta señal va al reves (1-OK, 0-error)
    CAN_disconnection = 1; // Mirar porque la logica de esta señal va al reves (1-OK, 0-error)
    BMS_OK_state = 1; // Mirar porque la logica de esta señal va al reves (1-OK, 0-error)
    stop_charging = 1; // Mirar porque la logica de esta señal va al reves (1-OK, 0-error)
    
    sfr1= (interlock_state<<7) + (AIRminus_state<<6) + (AIRplus_state<<5) + (precharge_state<<4) + (undervoltage<<3) + (overvoltage<<2) + (overtemperature<<1) + undertemperature;
    sfr2= (current_disconnection<<7) + (temperature_disconnection<<6) + (voltage_disconnection<<5) + (CAN_disconnection<<4) + (BMS_OK_state<<3) + (divide_current<<2) + (fans_state<<1) + balancing_state;
    sfr3= (stop_charging<<2) + (charge<<1);
    
    interlock_state = 0;
    sequence_state = 0;
    relays_state = 5;
    AIRminus_Control = 0;
    AIRminus_state = 0;
    AIRplus_Control = 0;
    AIRplus_state = 0;
    Precharge_Control = 0;
    precharge_state = 0;
    fans_state = 0;
    divide_current = 0;
    balancing_state = 0;
    synchronism = 0;
    current_synchronism = 0;
    sleep_mode = 0;
    tt_synchronism = 0;
    tt_hall = 0;
    CAN_Control= 1;
    CAN_alive = 0;
    BMS_alive = 0;
    CAN_count = 0;
    charge = 0;
    Error_LED = 0;
    LED1 = 0;
    LED2 = 0;
    

    // Initial voltage signal for SoC calculation purposes (Accumulator_SoC = min_cell_voltage_SoC_actual)
    
    

    while ( 1 )
    {
        external_CAN_management();
        
        if( CAN_alive > 5000 )
        {
            sleep_mode = 1;
            tt_slave0 = 0; // repeat for all Slaves
            tt_slave1 = 0;
            tt_slave2 = 0;
            tt_slave3 = 0;
            tt_slave4 = 0;
            tt_slave5 = 0;
            tt_slave6 = 0;
            tt_slave7 = 0;
            tt_slave8 = 0;
            tt_slave9 = 0;
            tt_slave10 = 0;
            tt_slave11 = 0;
            tt_slave12 = 0;
            tt_slave13 = 0;
            tt_slave14 = 0;
            tt_synchronism = 0;
            tt_hall = 0;
            tt_balancing=0;
            tt_divide = 0;
        }
        else
        {
            sleep_mode = 0;
        }
        if( tt_synchronism >= 120 && sleep_mode == 0)
        {
            // ? tt_mCAN
            LED2 = 1;
            
            tt_synchronism = 0;
            
            if( Balancing == 0 ) //Check the button state
            {
                balancing_button = 1;
            }
            else if ( Balancing == 1 )
            {
                balancing_button = 0;
            }
             
            
            external_CAN_management();
            
            // Calculate maximum and minumum cell, the total voltage and check the SOA
            max_cell_voltage_actual = max_cell_voltage(voltage);
            min_cell_voltage_actual = min_cell_voltage(voltage);
            total_voltage_actual = total_voltage(voltage);
            
            // lacks voltage disconnection detection
            
            external_CAN_management();
            
            // SoC calculation [Couloumb Counting]

            // SoC_actual = SoC_actual - current_buffer[1]*12e-3/(2*6800/3600); // SoC(t+1) = SoC(t)-(I[A]*delta_t(s))/(n_parallel_cells*capacity[A*h]*(1[h]/3600[s]))

            overvoltage = overvoltage_check(max_cell_voltage_actual, max_OK_voltage);
            
            external_CAN_management();
            
            if( min_cell_voltage_actual < min_OK_voltage ) //Checks the minimum voltage allowed.
            {
                shutdown_signal = 1;
                undervoltage = 0; // Undervoltage flag ON
            }
            
            external_CAN_management();
            
            if( shutdown_signal == 1 )
            {
                voltage_NOK = 1; // Enable the timer of 500 ms for voltage problems
            }
            else
            {
                shutdown_signal = 0;
                tt_voltage = 0;
                undervoltage = 1; // Mirar porque la logica de esta señal va al reves (1-OK, 0-error)
                overvoltage = 1; // Mirar porque la logica de esta señal va al reves (1-OK, 0-error)
                voltage_disconnection = 1; // Mirar porque la logica de esta señal va al reves (1-OK, 0-error)
                voltage_NOK = 0;
            }
            
            external_CAN_management();
            
            if( tt_divide >= 200 )
            {
                tt_divide = 0;
                divide_current = 0;
                divide_count++;
                if( (divide_count >= 15) )
                {
                    divide_count = 0;
                    if ( (max_cell_voltage_actual >= divide_current_voltage) && ( (max_cell_voltage_actual - min_cell_voltage_actual) >= 50) )
                    {
                        divide_current = 1;
                    } 
                }
            }
            
            external_CAN_management();

            max_cell_temperature_actual = max_cell_temperature(temperature);
            min_cell_temperature_actual = min_cell_temperature(temperature);
            avg_cell_temperature_actual = avg_cell_temperature(temperature);
            max_onboard_temperature_actual = max_onboard_temperature(temperature);
            min_onboard_temperature_actual = min_onboard_temperature(temperature);
            avg_onboard_temperature_actual = avg_onboard_temperature(temperature);
            
            external_CAN_management();
            
            fans_state = fans_control(fans_temperature, max_cell_temperature_actual, Fans1_Control, Fans2_Control);   
    
            external_CAN_management();
            
            if ( max_cell_temperature_actual > max_OK_temperature ) //Check if any cell is over the maximum allowed
            {
                shutdown_signal = 1;
                overtemperature = 0; // Overtemperature flag ON
            } 
            
            external_CAN_management();
            
            if ( min_cell_temperature_actual < min_OK_temperature ) //Check if any NTC is disconected
            {
                shutdown_signal = 1;
                undertemperature = 0; // Undertemperature flag ON
            }
            
            external_CAN_management();
            
            if( shutdown_signal == 1 )
            {
                temperature_NOK = 1; // Enable the timer of 1 s for temperatures problems 
            }
            else        //If everything OK restart timing counting and clear flags
            {
                shutdown_signal = 0;
                tt_temperature = 0;
                overtemperature = 1;
                undertemperature = 1;
                temperature_disconnection = 1;
                temperature_NOK = 0;
            }
            
            external_CAN_management();
            
//      current = ((current_buffer[0]>>16) + current_buffer[1]/(2*6800/3600) // SoC[k+1] = SoC[k]-int[1])-350000;
            
//      if ( current > 350000 ) //Check if the current connector is disconnected (350000 or 0)
//      {
//        shutdown_signal = 1;
//        current_disconnection=0;
//      }
//      else
//      {
//        shutdown_signal = 0;
//        current_disconnection=1;
//      }
            
            external_CAN_management();
            
            if ( current > max_OK_current ) //Check the minimum current.
            {
                shutdown_signal = 1;
            }
            
            if ( current_synchronism == 1 )
            {
                current_synchronism = 0;
                current_disconnection = 1;
 
            }
            else if ( current_synchronism == 0 )
            {
                shutdown_signal = 1;
                current_disconnection = 0;
            }
            
            if( shutdown_signal == 1 )
            {
                current_NOK = 1;
            }
            else
            {
                shutdown_signal = 0;
                current_disconnection = 1;
                tt_current = 0;
                current_NOK = 0;
            }
            
            external_CAN_management();
           
            if( Interlock_Monitoring == 1 ) //Check the interlock state
            {
                interlock_state = 1;
            }
            else
            {
                interlock_state = 0;
            }
             
            external_CAN_management();
           
            if( tt_slave0 > 750 || tt_slave1 > 750 || tt_slave2 > 750 || tt_slave3 > 750 || tt_slave4 > 750 || tt_slave5 > 750 || tt_slave6 > 750 || tt_slave7 > 750 || tt_slave8 > 750 || tt_slave9 > 750 || tt_slave10 > 750 || tt_slave11 > 750 || tt_slave12 > 750 || tt_slave13 > 750 || tt_slave14 > 750 )
            {
                CAN_disconnection = 0;
                CAN_NOK = 0;
            }
            else
            {
                CAN_disconnection = 1;
                CAN_NOK = 1;
            }
           
            external_CAN_management();
           
            if ( tt_voltage > 375 || tt_temperature > 875 || tt_current > 375 || CAN_disconnection == 0 ) //If timing of voltage, temperature and current has expired, BMS_OK is NOK and open precharge relay and AIRs.
            {
                shutdown_NOK = 1;
                stop_charging = 0;
                Error_LED = 1;   
            } 
            else
            {
                tt_shutdown = 0;
                stop_charging = 1;
                Error_LED = 0;
            }
           
            external_CAN_management();
            
            if( interlock_state == 1 )
            {
                sequence_state = 0;
                relays_state = 2;
                AIRminus_Control = 0;
                AIRminus_state = 0;
                AIRplus_Control = 0;
                AIRplus_state = 0;
                Precharge_Control = 0;
                precharge_state = 0;
                if( secuence_0 == 0 )
                {
                    while( C1TR01CONbits.TXREQ1 ) {}
                    can1_writebytes ( 1, 1, 145, 0, 0, 0, 0, 0, 0, 0, 0);
                    C1TR01CONbits.TXREQ1 = 1;
                }
                secuence_0 = 1;
            }
            
            external_CAN_management();
            
            if( tt_shutdown > 75 )
            {
                shutdown_NOK = 0;
                BMS_OK = 1;
                BMS_OK_state = 0;
                sequence_state = 0;
                relays_state = 2;
                AIRminus_Control = 0;
                AIRminus_state = 0;
                AIRplus_Control = 0;
                AIRplus_state = 0;
                Precharge_Control = 0;
                precharge_state = 0;
                LED1 = 0;
                Error_LED = 1;
                if( secuence_0 == 0 )
                {
                    while( C1TR01CONbits.TXREQ1 ) {}
                    can1_writebytes ( 1, 1, 145, 0, 0, 0, 0, 0, 0, 0, 0);
                    C1TR01CONbits.TXREQ1 = 1;
                }
                secuence_0 = 1;
            }
            else
            {
                BMS_OK = 0;
                BMS_OK_state = 1;
                secuence_0 = 0;
                Error_LED = 0;
            }
           
            external_CAN_management();
            
            balancing_state = balancing_0 | balancing_1 | balancing_2 | balancing_3 | balancing_4 | balancing_5 | balancing_6 | balancing_7 | balancing_8 | balancing_9 | balancing_10 | balancing_11 | balancing_12 | balancing_13 | balancing_14;
            
            sfr1= (interlock_state<<7) + (AIRminus_state<<6) + (AIRplus_state<<5) + (precharge_state<<4) + (undervoltage<<3) + (overvoltage<<2) + (overtemperature<<1) + undertemperature;
            sfr2= (current_disconnection<<7) + (temperature_disconnection<<6) + (voltage_disconnection<<5) + (CAN_disconnection<<4) + (BMS_OK_state<<3) + (divide_current<<2) + (fans_state<<1) + balancing_state;
            sfr3= (stop_charging<<2) + (charge<<1);
            
//      if(tt_balancing>15000 && tt_balancing<75000)
//      //if(tt_balancing>50000)
//      {
//        v_min_master_aux=80000;
//        balancing_enable=0;
//      }
//      if(tt_balancing>100000)
//      {
//        tt_balancing=0;
//        v_min_master_aux=v_min_master;
//        balancing_enable=1;
//      }
            external_CAN_management();
            
            tt_buffer = 0;
            while(C2TR01CONbits.TXREQ0) 
            {
                if(tt_buffer >= 5)
                {
                    break;
                }
            }
            can2_writebytes (0, 8, 0x80, min_cell_voltage_actual>>8, min_cell_voltage_actual, voltage_balancing>>8, voltage_balancing, voltage_balancing>>8, voltage_balancing, (( max_difference<<2) & 0xFC) | ((max_balancing_temperature>>8) & 0x03), max_balancing_temperature);
            C2TR01CONbits.TXREQ0 = 1;
            tt_buffer = 0;
            while(C2TR45CONbits.TXREQ5) 
            {
                if( tt_buffer >= 5 )
                {
                    break;
                }
            }
            can2_writebytes (5, 7, 0xB3,sfr1,sfr2,sfr3,0,0,0,0,0);
            C2TR45CONbits.TXREQ5 = 1;
            
            external_CAN_management();
            
            if ( CAN_count_int == 1 || CAN_count_int == 3 || CAN_count_int == 5 || CAN_count_int == 7 )
            {
                tt_buffer = 0;
                while(C2TR23CONbits.TXREQ3) 
                {
                    if( tt_buffer >= 5 )
                    {
                        break;
                    }
                }
                can2_writebytes (3, 7, 0xF0, min_cell_voltage_actual>>8, min_cell_voltage_actual, max_cell_voltage_actual>>8, max_cell_voltage_actual, total_voltage_actual>>16, total_voltage_actual>>8, total_voltage_actual, 0);
                C2TR23CONbits.TXREQ3 = 1;
            }
            if ( CAN_count_int == 0 || CAN_count_int == 2 || CAN_count_int == 4 || CAN_count_int == 6 )
            {
                tt_buffer = 0;
                while(C2TR23CONbits.TXREQ3) 
                {
                    if( tt_buffer >= 5 )
                    {
                        break;
                    }
                }
                can2_writebytes (3, 6, 0xF1, min_cell_temperature_actual>>8, min_cell_temperature_actual, max_cell_temperature_actual>>8, max_cell_temperature_actual, avg_cell_temperature_actual>>8, avg_cell_temperature_actual, 0, 0);
                C2TR23CONbits.TXREQ3 = 1;
                tt_buffer = 0;
                while(C2TR45CONbits.TXREQ4) 
                {
                    if( tt_buffer >= 5 )
                    {
                        break;
                    }
                }
                can2_writebytes (4, 6, 0xF2, min_onboard_temperature_actual>>8, min_onboard_temperature_actual, max_onboard_temperature_actual>>8, max_onboard_temperature_actual, avg_onboard_temperature_actual>>8, avg_onboard_temperature_actual, 0, 0);
                C2TR45CONbits.TXREQ4 = 1;
            }
            CAN_count_int++;
            if ( CAN_count_int >= 8 )
            {
                CAN_count_int = 0;
            }
        }
        LED2 = 0;
    }
}
