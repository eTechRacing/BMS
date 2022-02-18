/*
NOMBRE DEL ARCHIVO:             Master.c

DESCRIPCIÓN:                   Firmware de la Master de BMS en su versión 
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
    - Definición de external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control)
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
void external_CAN_management ()
    //* RELAYS MANAGEMENT *//

{
    
    if( (do_precharge == 1 && sequence_state == 0) || do_precharge_charger == 1 )
    {
        AIRs_state = 6;
        relays_state = 0;
        do_precharge = 0;
        do_precharge_charger = 0;
        sequence_state = 1;
        AIRminus_Control = 1;
        AIRminus_state = 1;
        Precharge_Control = 1;
        precharge_state = 1;
        tt_buffer = 0; //??
        while( C1TR01CONbits.TXREQ1 )
        {
            if( tt_buffer >= 5 ) // cada vez que veo un tt lo entiendo menos
            {
                break;
            }
        }
        can1_writebytes (1, 1, 145, 6, 0, 0, 0, 0, 0, 0, 0);
        C1TR01CONbits.TXREQ1 = 1; 
        secuence_0 = 0;
        LED1 = 1;
    }
        
    if( (close_AIRs == 1 && sequence_state == 1) || close_AIRs_charger == 1 )  
    {
        AIRs_state = 3;
        relays_state = 1;
        close_AIRs = 0;
        close_AIRs_charger = 0;
        AIRplus_Control = 1;
        AIRplus_state = 1;
        Precharge_Control = 0;
        precharge_state = 0;
        tt_buffer = 0;
        while( C1TR01CONbits.TXREQ1 ) 
        {
            if( tt_buffer >= 5 )
            {
                break;
            }
        }
        can1_writebytes (1, 1, 145, 3, 0, 0, 0, 0, 0, 0, 0);
        C1TR01CONbits.TXREQ1 = 1;
        secuence_0 = 0;
        LED1 = 1;
    }
        
    if ( CAN_alive > 4000 || open_relays == 1 )
    {
        if( secuence_0 == 0 )
        {
            tt_buffer = 0;
            while( C1TR01CONbits.TXREQ1 ) 
            {
                if( tt_buffer >= 5 )
                {
                    break;
                }
            }
            can1_writebytes (1, 1, 145, 0, 0, 0, 0, 0, 0, 0, 0);
            C1TR01CONbits.TXREQ1 = 1;
        }
        AIRs_state = 0;
        relays_state=2;
        open_relays = 0;
        sequence_state = 0;
        AIRminus_Control = 0;
        AIRminus_state = 0;
        Precharge_Control = 0;
        precharge_state = 0;
        AIRplus_Control = 0;
        AIRplus_state = 0;
        secuence_0 = 1;
        LED1 = 0;
    }

    if ( synchronism == 1 )
    {
        CAN_alive = 0;
        synchronism = 0;

        //  can1_writebytes (buffer, dlc, cobid, byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8);
        if (CAN_count == 1)
        {
            tt_buffer = 0;
            while( C1TR01CONbits.TXREQ1 ) 
            {
                if(tt_buffer >= 5)
                {
                    break;
                }
            }
            can1_writebytes (1, 1, 145, AIRs_state, 0, 0, 0, 0, 0, 0, 0);
            C1TR01CONbits.TXREQ1 = 1;
        }
        if ( CAN_count == 2 || CAN_count == 4 || CAN_count == 6 || CAN_count == 8 || CAN_count == 10 || CAN_count == 12 )
        {
            tt_buffer = 0; //?
            while( C1TR01CONbits.TXREQ0 ) 
            {
                if(tt_buffer >= 5)
                {
                    break;
                }
            }
            can1_writebytes (0, 7, 146, min_cell_voltage_actual>>8, min_cell_voltage_actual, max_cell_voltage_actual>>8, max_cell_voltage_actual, total_voltage_actual>>16, total_voltage_actual>>8, total_voltage_actual, 0);
            C1TR01CONbits.TXREQ0 = 1;
            
            tt_buffer = 0;
            while( C1TR01CONbits.TXREQ1 ) 
            {
                if( tt_buffer >= 5 )
                {
                    break;
                }
            }
            can1_writebytes (1, 8, 148, current_buffer[0]>>8, current_buffer[0], current_buffer[1]>>8, current_buffer[1], SoC_actual>>8, SoC_actual, V_init>>8, V_init);
            C1TR01CONbits.TXREQ1 = 1;  
        }
        if ( CAN_count == 3 || CAN_count == 5 || CAN_count == 7 || CAN_count == 9 || CAN_count == 11 )
        {
            tt_buffer = 0;
            while( C1TR01CONbits.TXREQ0 ) 
            {
                if( tt_buffer >= 5 )
                {
                    break;
                }
            }
            can1_writebytes (0, 6, 147, min_cell_temperature_actual>>8, min_cell_temperature_actual, max_cell_temperature_actual>>8, max_cell_temperature_actual, avg_cell_temperature_actual>>8, avg_cell_temperature_actual, 0, 0);
            C1TR01CONbits.TXREQ0 = 1;
            
            tt_buffer = 0;
            while( C1TR01CONbits.TXREQ1 ) 
            {
                if( tt_buffer >= 5 )
                {
                    break;
                }
            }
            can1_writebytes (1, 6, 158, min_onboard_temperature_actual>>8, min_onboard_temperature_actual, max_onboard_temperature_actual>>8, max_onboard_temperature_actual, avg_onboard_temperature_actual>>8, avg_onboard_temperature_actual, 0, 0);
            C1TR01CONbits.TXREQ1 = 1;
        }
        
        if( CAN_count == 12 )
        {
            CAN_count=0;
            charger_count++;
        }
//    if( CAN_count == 12 )
//    {
//      CAN_count=0;
//      charger_count++;
//      tt_buffer = 0;
//      while( C1TR01CONbits.TXREQ1 ) 
//      {
//        if( tt_buffer >= 5 )
//        {
//          break;
//        }
//      }
//      can1_writebytes (1, 8, 148, current_buffer[1]/(2*6800/3600) // SoC[k+1] = SoC[k]-int[0]>>8, current_buffer[1]/(2*6800/3600) // SoC[k+1] = SoC[k]-int[0], current_buffer[1]/(2*6800/3600) // SoC[k+1] = SoC[k]-int[1]>>8, current_buffer[1]/(2*6800/3600) // SoC[k+1] = SoC[k]-int[1], SoC>>8, SoC, SoH>>8, SoH);
//      C1TR01CONbits.TXREQ1 = 1;
//    }
        CAN_count++;
        
        tt_buffer = 0;
        while( C1TR23CONbits.TXREQ2 ) 
        {
            if( tt_buffer >= 5 )
            {
                break;
            }
        }
        can1_writebytes (2, 4, 149, sfr1, sfr2, sfr3, 0, 0, 0, 0, 0);
        C1TR23CONbits.TXREQ2 = 1;

        if(charger_count==20)
        {
            charger_count=0;
            tt_buffer = 0;
            while( C1TR01CONbits.TXREQ0 ) 
            {
                if( tt_buffer >= 5 )
                {
                    break;
                }
            }
            can1_writebytes (0, 3, 157, max_charging_voltage>>8, max_charging_voltage, max_charging_current, 0, 0, 0, 0, 0);
            C1TR01CONbits.TXREQ0 = 1;
        }
        
        if(alive_count==4)
        {
            alive_count=0;
            tt_buffer = 0;
            while( C1TR23CONbits.TXREQ3 ) 
            {
                if( tt_buffer >= 5 )
                {
                    break;
                }
            }
            can1_writebytes (3, 1, 159, BMS_alive, 0, 0, 0, 0, 0, 0, 0);
            C1TR23CONbits.TXREQ3 = 1;

            BMS_alive++;
            if( BMS_alive >= 255 )
            {
                BMS_alive = 0;
            }
        }
        alive_count++;
    }
    switch (relays_state)
    {
        case 0 :
            AIRs_state = 6;
            Precharge_Control = 1;
            precharge_state = 1;
            AIRminus_Control = 1;
            AIRminus_state = 1;
            AIRplus_Control = 0;
            AIRplus_state = 0;
            LED1 = 1;
            break;
        case 1 :
            AIRs_state = 3;
            Precharge_Control = 0;
            precharge_state = 0;
            AIRminus_Control = 1;
            AIRminus_state = 1;
            AIRplus_Control = 1;
            AIRplus_state = 1;
            LED1 = 1;
            break;
        case 2 :
            AIRs_state = 0;
            Precharge_Control = 0;
            precharge_state = 0;
            AIRminus_Control = 0;
            AIRminus_state = 0;
            AIRplus_Control = 0;
            AIRplus_state = 0;
            sequence_state = 0;
            LED1 = 0;
            break;
    }      
}

void __attribute__ ( (interrupt, no_auto_psv) ) _C1Interrupt( void )
{
    IFS2bits.C1IF = 0; // Clear Interrupt Flag
    if ( C1INTFbits.TBIF ) 
    {
        C1INTFbits.TBIF = 0; // Clear Interrupt
    }
    
    if ( C1INTFbits.RBIF )
    {
        rbuffer_external = can1_getrxbuf(); // Extract the message from the buffer (external CAN)
        cobid_external = can1_getcobid(rbuffer_external); // Extract the cobid from the buffer message (external CAN)
        
        if( cobid_external == 128 ) // Synchronism message received
        {
            synchronism = 1;         
        }
        if( cobid_external == 144 ) // Start message received
        {
            AIRs_request = can1_getdata(rbuffer_external,0);
            AIRs_request = AIRs_request>>8; // 1 byte shift
            

            /* 

            **AIRs_request states**
            

            WITH THE CAR

                0: Accumulator isolated from TS
                    AIR+        open
                    AIR-        open
                    Precharge   open

                3: Accumulator connected to TS
                    AIR+        closed
                    AIR-        closed
                    Precharge   open

                6: Accumulator precharging inverters
                    AIR+        open
                    AIR-        closed
                    Precharge   closed

            WITH THE CHARGER (CITCEA)

                0: Accumulator isolated from charger
                    AIR+        open
                    AIR-        open
                    Precharge   open

                8: Accumulator precharging charger
                    AIR+        open
                    AIR-        closed
                    Precharge   closed

                9: Accumulator connected to charger
                    AIR+        closed
                    AIR-        closed
                    Precharge   open
                                
                
            */


            if ( AIRs_request == 6 )
            {
                do_precharge = 1;
                LED1 = 1;
            }
            else
            {
                if( AIRs_request == 3 )
                {
                    close_AIRs = 1;
                    LED1 = 1;
                }
                else
                {
                    if( AIRs_request == 8 )
                    {   
                        do_precharge_charger = 1;
                        LED1 = 0;
                    }
                    else
                    {
                        if( AIRs_request == 9 )
                        {
                            close_AIRs_charger = 1;
                            LED1 = 1;
                        }
                        else
                        {
                            if( AIRs_request == 0 )
                            {
                                open_relays = 1;
                                LED1 = 0;
                            }
                        }
                    }            
                }
            }
        }
        
        C1RXFUL1 = 0; // Uncheck the buffer as full 
        C1RXFUL2 = 0; // Uncheck the buffer as full 
        C1INTFbits.RBIF = 0; //Clear interrupt
    }
}


// CAN2 (BMS_INTERNAL_CAN) general interruption
// Reads voltages, temperatures and balancing states from BMS Slaves and current from a Fluxgate sensor 

void __attribute__ ( (interrupt, no_auto_psv) ) _C2Interrupt( void )
{
    IFS3bits.C2IF = 0; // Clear Interrupt Flag
    if( C2INTFbits.TBIF )
    {
        C2INTFbits.TBIF = 0; //Clear Interrupt
    }
    
    if( C2INTFbits.RBIF )
    {
        rbuffer_internal = can2_getrxbuf(); // Extract the message from the buffer (internal CAN)
        cobid_internal = can2_getcobid(rbuffer_internal); // Extract the cobid from the buffer message (internal CAN)

        switch (cobid_internal)
        {
            case 0xFF:
                CAN_alive = 0; //????????????????
                break;
        
            //* SLAVES VOLTAGES *//
                
            // Slave 0 voltages
            case 0x82:
                tt_slave0 = 0;
                voltage[0] = can2_getdata(rbuffer_internal,0);
                voltage[1] = can2_getdata(rbuffer_internal,1);
                voltage[2] = can2_getdata(rbuffer_internal,2);
                voltage[3] = can2_getdata(rbuffer_internal,3);
                break;
            case 0x83:
                voltage[4] = can2_getdata(rbuffer_internal,0);
                voltage[5] = can2_getdata(rbuffer_internal,1);
                voltage[6] = can2_getdata(rbuffer_internal,2);
                voltage[7] = can2_getdata(rbuffer_internal,3);
                break;
            case 0x84:
                voltage[8] = can2_getdata(rbuffer_internal,0);
                voltage[9] = can2_getdata(rbuffer_internal,1);
                break;
            // Slave 1 voltages
            case 0x85:
                tt_slave1 = 0;
                voltage[10] = can2_getdata(rbuffer_internal,0);
                voltage[11] = can2_getdata(rbuffer_internal,1);
                voltage[12] = can2_getdata(rbuffer_internal,2);
                voltage[13] = can2_getdata(rbuffer_internal,3);
                break;
            case 0x86:
                voltage[14] = can2_getdata(rbuffer_internal,0);
                voltage[15] = can2_getdata(rbuffer_internal,1);
                voltage[16] = can2_getdata(rbuffer_internal,2);
                voltage[17] = can2_getdata(rbuffer_internal,3);
                break;
            // Slave 2 voltages
            case 0x88:
                tt_slave2 = 0;
                voltage[18] = can2_getdata(rbuffer_internal,0);
                voltage[19] = can2_getdata(rbuffer_internal,1);
                voltage[20] = can2_getdata(rbuffer_internal,2);
                voltage[21] = can2_getdata(rbuffer_internal,3);
                break;
            case 0x89:
                voltage[22] = can2_getdata(rbuffer_internal,0);
                voltage[23] = can2_getdata(rbuffer_internal,1);
                voltage[24] = can2_getdata(rbuffer_internal,2);
                voltage[25] = can2_getdata(rbuffer_internal,3);
                break;
            case 0x8A:
                voltage[26] = can2_getdata(rbuffer_internal,0);
                voltage[27] = can2_getdata(rbuffer_internal,1);
                break;
            // Slave 3 voltages
            case 0x8B:
                tt_slave3 = 0;
                voltage[28] = can2_getdata(rbuffer_internal,0);
                voltage[29] = can2_getdata(rbuffer_internal,1);
                voltage[30] = can2_getdata(rbuffer_internal,2);
                voltage[31] = can2_getdata(rbuffer_internal,3);
                break;
            case 0x8C:
                voltage[32] = can2_getdata(rbuffer_internal,0);
                voltage[33] = can2_getdata(rbuffer_internal,1);
                voltage[34] = can2_getdata(rbuffer_internal,2);
                voltage[35] = can2_getdata(rbuffer_internal,3);
                break;
            case 0x8D:
                voltage[36] = can2_getdata(rbuffer_internal,0);
                voltage[37] = can2_getdata(rbuffer_internal,1);
                break;  
            // Slave 4 voltages
            case 0x8E:
                tt_slave4 = 0;
                voltage[38] = can2_getdata(rbuffer_internal,0);
                voltage[39] = can2_getdata(rbuffer_internal,1);
                voltage[40] = can2_getdata(rbuffer_internal,2);
                voltage[41] = can2_getdata(rbuffer_internal,3);
                break;
            case 0x8F:
                voltage[42] = can2_getdata(rbuffer_internal,0);
                voltage[43] = can2_getdata(rbuffer_internal,1);
                voltage[44] = can2_getdata(rbuffer_internal,2);
                voltage[45] = can2_getdata(rbuffer_internal,3);
                break;
            // Slave 5 voltages 
            case 0x91:
                tt_slave5 = 0;
                voltage[46] = can2_getdata(rbuffer_internal,0);
                voltage[47] = can2_getdata(rbuffer_internal,1);
                voltage[48] = can2_getdata(rbuffer_internal,2);
                voltage[49] = can2_getdata(rbuffer_internal,3);
                break;
            case 0x92:
                voltage[50] = can2_getdata(rbuffer_internal,0);
                voltage[51] = can2_getdata(rbuffer_internal,1);
                voltage[52] = can2_getdata(rbuffer_internal,2);
                voltage[53] = can2_getdata(rbuffer_internal,3);
                break;
            case 0x93:
                voltage[54] = can2_getdata(rbuffer_internal,0);
                voltage[55] = can2_getdata(rbuffer_internal,1);
                break;
            // Slave 6 voltages
            case 0x94:
                tt_slave6 = 0;
                voltage[56] = can2_getdata(rbuffer_internal,0);
                voltage[57] = can2_getdata(rbuffer_internal,1);
                voltage[58] = can2_getdata(rbuffer_internal,2);
                voltage[59] = can2_getdata(rbuffer_internal,3);
                break;
            case 0x95:
                voltage[60] = can2_getdata(rbuffer_internal,0);
                voltage[61] = can2_getdata(rbuffer_internal,1);
                voltage[62] = can2_getdata(rbuffer_internal,2);
                voltage[63] = can2_getdata(rbuffer_internal,3);
                break;
            case 0x96:
                voltage[64] = can2_getdata(rbuffer_internal,0);
                voltage[65] = can2_getdata(rbuffer_internal,1);
                break;   
            // Slave 7 voltages
            case 0x97:
                tt_slave7 = 0;
                voltage[66] = can2_getdata(rbuffer_internal,0);
                voltage[67] = can2_getdata(rbuffer_internal,1);
                voltage[68] = can2_getdata(rbuffer_internal,2);
                voltage[69] = can2_getdata(rbuffer_internal,3);
                break;
            case 0x98:
                voltage[70] = can2_getdata(rbuffer_internal,0);
                voltage[71] = can2_getdata(rbuffer_internal,1);
                voltage[72] = can2_getdata(rbuffer_internal,2);
                voltage[73] = can2_getdata(rbuffer_internal,3);
                break;
            // Slave 8 voltages
            case 0x9A:
                tt_slave8 = 0;
                voltage[74] = can2_getdata(rbuffer_internal,0);
                voltage[75] = can2_getdata(rbuffer_internal,1);
                voltage[76] = can2_getdata(rbuffer_internal,2);
                voltage[77] = can2_getdata(rbuffer_internal,3);
                break;
            case 0x9B:
                voltage[78] = can2_getdata(rbuffer_internal,0);
                voltage[79] = can2_getdata(rbuffer_internal,1);
                voltage[80] = can2_getdata(rbuffer_internal,2);
                voltage[81] = can2_getdata(rbuffer_internal,3);
                break;
            case 0x9C:
                voltage[82] = can2_getdata(rbuffer_internal,0);
                voltage[83] = can2_getdata(rbuffer_internal,1);
                break;
            // Slave 9 voltages
            case 0x9D:
                tt_slave9 = 0;
                voltage[84] = can2_getdata(rbuffer_internal,0);
                voltage[85] = can2_getdata(rbuffer_internal,1);
                voltage[86] = can2_getdata(rbuffer_internal,2);
                voltage[87] = can2_getdata(rbuffer_internal,3);
                break;
            case 0x9E:
                voltage[88] = can2_getdata(rbuffer_internal,0);
                voltage[89] = can2_getdata(rbuffer_internal,1);
                voltage[90] = can2_getdata(rbuffer_internal,2);
                voltage[91] = can2_getdata(rbuffer_internal,3);
                break;
            case 0x9F:
                voltage[92] = can2_getdata(rbuffer_internal,0);
                voltage[93] = can2_getdata(rbuffer_internal,1);
                break;
            // Slave 10 voltages
            case 0xA0:
                tt_slave10 = 0;
                voltage[94] = can2_getdata(rbuffer_internal,0);
                voltage[95] = can2_getdata(rbuffer_internal,1);
                voltage[96] = can2_getdata(rbuffer_internal,2);
                voltage[97] = can2_getdata(rbuffer_internal,3);
                break;
            case 0xA1:
                voltage[98] = can2_getdata(rbuffer_internal,0);
                voltage[99] = can2_getdata(rbuffer_internal,1);
                voltage[100] = can2_getdata(rbuffer_internal,2);
                voltage[101] = can2_getdata(rbuffer_internal,3);
                break;
            // Slave 11 voltages
            case 0xA3:
                tt_slave11 = 0;
                voltage[102] = can2_getdata(rbuffer_internal,0);
                voltage[103] = can2_getdata(rbuffer_internal,1);
                voltage[104] = can2_getdata(rbuffer_internal,2);
                voltage[105] = can2_getdata(rbuffer_internal,3);
                break;
            case 0xA4:
                voltage[106] = can2_getdata(rbuffer_internal,0);
                voltage[107] = can2_getdata(rbuffer_internal,1);
                voltage[108] = can2_getdata(rbuffer_internal,2);
                voltage[109] = can2_getdata(rbuffer_internal,3);
                break;
            case 0xA5:
                voltage[110] = can2_getdata(rbuffer_internal,0);
                voltage[111] = can2_getdata(rbuffer_internal,1);
                break;
            // Slave 12 voltages 
            case 0xA6:
                tt_slave12 = 0;
                voltage[112] = can2_getdata(rbuffer_internal,0);
                voltage[113] = can2_getdata(rbuffer_internal,1);
                voltage[114] = can2_getdata(rbuffer_internal,2);
                voltage[115] = can2_getdata(rbuffer_internal,3);
                break;
            case 0xA7:
                voltage[116] = can2_getdata(rbuffer_internal,0);
                voltage[117] = can2_getdata(rbuffer_internal,1);
                voltage[118] = can2_getdata(rbuffer_internal,2);
                voltage[119] = can2_getdata(rbuffer_internal,3);
                break;
            case 0xA8:
                voltage[120] = can2_getdata(rbuffer_internal,0);
                voltage[121] = can2_getdata(rbuffer_internal,1);
                break;
            // Slave 13 voltages
            case 0xA9:
                tt_slave13 = 0;
                voltage[122] = can2_getdata(rbuffer_internal,0);
                voltage[123] = can2_getdata(rbuffer_internal,1);
                voltage[124] = can2_getdata(rbuffer_internal,2);
                voltage[125] = can2_getdata(rbuffer_internal,3);
                break;
            case 0xAA:
                voltage[126] = can2_getdata(rbuffer_internal,0);
                voltage[127] = can2_getdata(rbuffer_internal,1);
                voltage[128] = can2_getdata(rbuffer_internal,2);
                voltage[129] = can2_getdata(rbuffer_internal,3); 
                break;
            // Slave 14 voltages 
            case 0xAC:
                tt_slave14 = 0;
                voltage[130] = can2_getdata(rbuffer_internal,0);
                voltage[131] = can2_getdata(rbuffer_internal,1);
                voltage[132] = can2_getdata(rbuffer_internal,2);
                voltage[133] = can2_getdata(rbuffer_internal,3);
                break;
            case 0xAD:
                voltage[134] = can2_getdata(rbuffer_internal,0);
                voltage[135] = can2_getdata(rbuffer_internal,1);
                voltage[136] = can2_getdata(rbuffer_internal,2);
                voltage[137] = can2_getdata(rbuffer_internal,3);
                break;
            case 0xAE:
                voltage[138] = can2_getdata(rbuffer_internal,0);
                voltage[139] = can2_getdata(rbuffer_internal,1);
                break;
        
            //* SLAVES TEMPERATURES *//
                
            // Slave 0 temperatures
            case 0xC0:
                temperature[0]=can2_getdata(rbuffer_internal,0);
                temperature[1]=can2_getdata(rbuffer_internal,1);
                temperature[2]=can2_getdata(rbuffer_internal,2);
                temperature[3]=can2_getdata(rbuffer_internal,3);
                break;
            case 0xC1:
                temperature[4]=can2_getdata(rbuffer_internal,0);
                temperature[5]=can2_getdata(rbuffer_internal,1);
                temperature[6]=can2_getdata(rbuffer_internal,2);
                temperature[7]=can2_getdata(rbuffer_internal,3);
            // Slave 1 temperatures
            case 0xC2:
                temperature[8]=can2_getdata(rbuffer_internal,0);
                temperature[9]=can2_getdata(rbuffer_internal,1);
                temperature[10]=can2_getdata(rbuffer_internal,2);
                temperature[11]=can2_getdata(rbuffer_internal,3);
                break;
            case 0xC3:
                temperature[12]=can2_getdata(rbuffer_internal,0);
                temperature[13]=can2_getdata(rbuffer_internal,1);
                temperature[14]=can2_getdata(rbuffer_internal,2);
                temperature[15]=can2_getdata(rbuffer_internal,3);
                break;
            // Slave 2 temperatures
            case 0xC4:
                temperature[16]=can2_getdata(rbuffer_internal,0);
                temperature[17]=can2_getdata(rbuffer_internal,1);
                temperature[18]=can2_getdata(rbuffer_internal,2);
                temperature[19]=can2_getdata(rbuffer_internal,3);
                break;
            case 0xC5:
                temperature[20]=can2_getdata(rbuffer_internal,0);
                temperature[21]=can2_getdata(rbuffer_internal,1);
                temperature[22]=can2_getdata(rbuffer_internal,2);
                temperature[23]=can2_getdata(rbuffer_internal,3);
                break;
            // Slave 3 temperatures
            case 0xC6:
                temperature[24]=can2_getdata(rbuffer_internal,0);
                temperature[25]=can2_getdata(rbuffer_internal,1);
                temperature[26]=can2_getdata(rbuffer_internal,2);
                temperature[27]=can2_getdata(rbuffer_internal,3);
                break;
            case 0xC7:
                temperature[28]=can2_getdata(rbuffer_internal,0);
                temperature[29]=can2_getdata(rbuffer_internal,1);
                temperature[30]=can2_getdata(rbuffer_internal,2);
                temperature[31]=can2_getdata(rbuffer_internal,3);
                break;
            // Slave 4 temperatures
            case 0xC8:
                temperature[32]=can2_getdata(rbuffer_internal,0);
                temperature[33]=can2_getdata(rbuffer_internal,1);
                temperature[34]=can2_getdata(rbuffer_internal,2);
                temperature[35]=can2_getdata(rbuffer_internal,3);
                break;
            case 0xC9:
                temperature[36]=can2_getdata(rbuffer_internal,0);
                temperature[37]=can2_getdata(rbuffer_internal,1);
                temperature[38]=can2_getdata(rbuffer_internal,2);
                temperature[39]=can2_getdata(rbuffer_internal,3);
                break;
            // Slave 5 temperatures
            case 0xCA:
                temperature[40]=can2_getdata(rbuffer_internal,0);
                temperature[41]=can2_getdata(rbuffer_internal,1);
                temperature[42]=can2_getdata(rbuffer_internal,2);
                temperature[43]=can2_getdata(rbuffer_internal,3);
                break;
            case 0xCB:
                temperature[44]=can2_getdata(rbuffer_internal,0);
                temperature[45]=can2_getdata(rbuffer_internal,1);
                temperature[46]=can2_getdata(rbuffer_internal,2);
                temperature[47]=can2_getdata(rbuffer_internal,3);
                break;
            // Slave 6 temperatures
            case 0xCC:
                temperature[48]=can2_getdata(rbuffer_internal,0);
                temperature[49]=can2_getdata(rbuffer_internal,1);
                temperature[50]=can2_getdata(rbuffer_internal,2);
                temperature[51]=can2_getdata(rbuffer_internal,3);
                break;
            case 0xCD:
                temperature[52]=can2_getdata(rbuffer_internal,0);
                temperature[53]=can2_getdata(rbuffer_internal,1);
                temperature[54]=can2_getdata(rbuffer_internal,2);
                temperature[55]=can2_getdata(rbuffer_internal,3);
                break;
            // Slave 7 temperatures
            case 0xCE:
                temperature[56]=can2_getdata(rbuffer_internal,0);
                temperature[57]=can2_getdata(rbuffer_internal,1);
                temperature[58]=can2_getdata(rbuffer_internal,2);
                temperature[59]=can2_getdata(rbuffer_internal,3);
                break;
            case 0xCF:
                temperature[60]=can2_getdata(rbuffer_internal,0);
                temperature[61]=can2_getdata(rbuffer_internal,1);
                temperature[62]=can2_getdata(rbuffer_internal,2);
                temperature[63]=can2_getdata(rbuffer_internal,3);
                break;
            // Slave 8 temperatures
            case 0xD0:
                temperature[64]=can2_getdata(rbuffer_internal,0);
                temperature[65]=can2_getdata(rbuffer_internal,1);
                temperature[66]=can2_getdata(rbuffer_internal,2);
                temperature[67]=can2_getdata(rbuffer_internal,3);
                break;
            case 0xD1:
                temperature[68]=can2_getdata(rbuffer_internal,0);
                temperature[69]=can2_getdata(rbuffer_internal,1);
                temperature[70]=can2_getdata(rbuffer_internal,2);
                temperature[71]=can2_getdata(rbuffer_internal,3);
                break;
            // Slave 9 temperatures
            case 0xD2:
                temperature[72]=can2_getdata(rbuffer_internal,0);
                temperature[73]=can2_getdata(rbuffer_internal,1);
                temperature[74]=can2_getdata(rbuffer_internal,2);
                temperature[75]=can2_getdata(rbuffer_internal,3);
                break;
            case 0xD3:
                temperature[76]=can2_getdata(rbuffer_internal,0);
                temperature[77]=can2_getdata(rbuffer_internal,1);
                temperature[78]=can2_getdata(rbuffer_internal,2);
                temperature[79]=can2_getdata(rbuffer_internal,3);
                break;
            // Slave 10 temperatures
            case 0xD4:
                temperature[80]=can2_getdata(rbuffer_internal,0);
                temperature[81]=can2_getdata(rbuffer_internal,1);
                temperature[82]=can2_getdata(rbuffer_internal,2);
                temperature[83]=can2_getdata(rbuffer_internal,3);
                break;
            case 0xD5:
                temperature[84]=can2_getdata(rbuffer_internal,0);
                temperature[85]=can2_getdata(rbuffer_internal,1);
                temperature[86]=can2_getdata(rbuffer_internal,2);
                temperature[87]=can2_getdata(rbuffer_internal,3);
                break;
            // Slave 11 temperatures
            case 0xD6:
                temperature[88]=can2_getdata(rbuffer_internal,0);
                temperature[89]=can2_getdata(rbuffer_internal,1);
                temperature[90]=can2_getdata(rbuffer_internal,2);
                temperature[91]=can2_getdata(rbuffer_internal,3);
                break;
            case 0xD7:
                temperature[92]=can2_getdata(rbuffer_internal,0);
                temperature[93]=can2_getdata(rbuffer_internal,1);
                temperature[94]=can2_getdata(rbuffer_internal,2);
                temperature[95]=can2_getdata(rbuffer_internal,3);
                break;
            // Slave 12 temperatures
            case 0xD8:
                temperature[96]=can2_getdata(rbuffer_internal,0);
                temperature[97]=can2_getdata(rbuffer_internal,1);
                temperature[98]=can2_getdata(rbuffer_internal,2);
                temperature[99]=can2_getdata(rbuffer_internal,3);
                break;
            case 0xD9:
                temperature[100]=can2_getdata(rbuffer_internal,0);
                temperature[101]=can2_getdata(rbuffer_internal,1);
                temperature[102]=can2_getdata(rbuffer_internal,2);
                temperature[103]=can2_getdata(rbuffer_internal,3);
                break;
            // Slave 13 temperatures
            case 0xDA:
                temperature[104]=can2_getdata(rbuffer_internal,0);
                temperature[105]=can2_getdata(rbuffer_internal,1);
                temperature[106]=can2_getdata(rbuffer_internal,2);
                temperature[107]=can2_getdata(rbuffer_internal,3);
                break;
            case 0xDB:
                temperature[108]=can2_getdata(rbuffer_internal,0);
                temperature[109]=can2_getdata(rbuffer_internal,1);
                temperature[110]=can2_getdata(rbuffer_internal,2);
                temperature[111]=can2_getdata(rbuffer_internal,3);
                break;
            // Slave 14 temperatures
            case 0xDC:
                temperature[112]=can2_getdata(rbuffer_internal,0);
                temperature[113]=can2_getdata(rbuffer_internal,1);
                temperature[114]=can2_getdata(rbuffer_internal,2);
                temperature[115]=can2_getdata(rbuffer_internal,3);
                break;
            case 0xDD:
                temperature[116]=can2_getdata(rbuffer_internal,0);
                temperature[117]=can2_getdata(rbuffer_internal,1);
                temperature[118]=can2_getdata(rbuffer_internal,2);
                temperature[119]=can2_getdata(rbuffer_internal,3);
                break;
                
            //* SLAVES BALANCING *//
                
            // Balancing flags
            case 0xE0:
                balancing_0 = can2_getdata(rbuffer_internal,0)>>8;
                break;
            case 0xE1:
                balancing_1 = can2_getdata(rbuffer_internal,0)>>8;
                break;
            case 0xE2:
                balancing_2 = can2_getdata(rbuffer_internal,0)>>8;
                break;
            case 0xE3:
                balancing_3 = can2_getdata(rbuffer_internal,0)>>8;
                break;
            case 0xE4:
                balancing_4 = can2_getdata(rbuffer_internal,0)>>8;
                break;
            case 0xE5:
                balancing_5 = can2_getdata(rbuffer_internal,0)>>8;
                break;
            case 0xE6:
                balancing_6 = can2_getdata(rbuffer_internal,0)>>8;
                break;
            case 0xE7:
                balancing_7 = can2_getdata(rbuffer_internal,0)>>8;
                break;
            case 0xE8:
                balancing_8 = can2_getdata(rbuffer_internal,0)>>8;
                break;
            case 0xE9:
                balancing_9 = can2_getdata(rbuffer_internal,0)>>8;
                break;
            case 0xEA:
                balancing_10 = can2_getdata(rbuffer_internal,0)>>8;
                break;
            case 0xEB:
                balancing_11 = can2_getdata(rbuffer_internal,0)>>8;
                break;
            case 0xEC:
                balancing_12 = can2_getdata(rbuffer_internal,0)>>8;
                break;
            case 0xED:
                balancing_13 = can2_getdata(rbuffer_internal,0)>>8;
                break;
            case 0xEE:
                balancing_14 = can2_getdata(rbuffer_internal,0)>>8;
                break;
             
            //* HALL SENSOR CURRENT *//    
                
            case 0x3C0:
                current_synchronism = 1;
                current_buffer[0] = can2_getdata(rbuffer_internal,0);
                current_buffer[1] = can2_getdata(rbuffer_internal,1);
                break;
        }
        
        C2RXFUL1 = 0; // Uncheck the buffer as full 
        C2RXFUL2 = 0; // Uncheck the buffer as full 
        C2INTFbits.RBIF = 0; // Clear interrupt
    }
}



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
        external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
        
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
             
            
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
            
            // Calculate maximum and minumum cell, the total voltage and check the SOA
            max_cell_voltage_actual = max_cell_voltage(voltage);
            min_cell_voltage_actual = min_cell_voltage(voltage);
            total_voltage_actual = total_voltage(voltage);
            
            // lacks voltage disconnection detection
            
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
            
            // SoC calculation [Couloumb Counting]

            // SoC_actual = SoC_actual - current_buffer[1]*12e-3/(2*6800/3600); // SoC(t+1) = SoC(t)-(I[A]*delta_t(s))/(n_parallel_cells*capacity[A*h]*(1[h]/3600[s]))

            overvoltage = overvoltage_check(max_cell_voltage_actual, max_OK_voltage);
            
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
            
            if( min_cell_voltage_actual < min_OK_voltage ) //Checks the minimum voltage allowed.
            {
                shutdown_signal = 1;
                undervoltage = 0; // Undervoltage flag ON
            }
            
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
            
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
            
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
            
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
            
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);

            max_cell_temperature_actual = max_cell_temperature(temperature);
            min_cell_temperature_actual = min_cell_temperature(temperature);
            avg_cell_temperature_actual = avg_cell_temperature(temperature);
            max_onboard_temperature_actual = max_onboard_temperature(temperature);
            min_onboard_temperature_actual = min_onboard_temperature(temperature);
            avg_onboard_temperature_actual = avg_onboard_temperature(temperature);
            
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
            
            fans_state = fans_control(fans_temperature, max_cell_temperature_actual, Fans1_Control, Fans2_Control);   
    
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
            
            if ( max_cell_temperature_actual > max_OK_temperature ) //Check if any cell is over the maximum allowed
            {
                shutdown_signal = 1;
                overtemperature = 0; // Overtemperature flag ON
            } 
            
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
            
            if ( min_cell_temperature_actual < min_OK_temperature ) //Check if any NTC is disconected
            {
                shutdown_signal = 1;
                undertemperature = 0; // Undertemperature flag ON
            }
            
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
            
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
            
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
            
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
            
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
            
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
            
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
           
            if( Interlock_Monitoring == 1 ) //Check the interlock state
            {
                interlock_state = 1;
            }
            else
            {
                interlock_state = 0;
            }
             
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
           
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
           
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
           
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
           
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
            
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
            
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
            
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
           
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
            
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
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
            
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
            
            external_CAN_management(AIRminus_Control, AIRplus_Control,  Precharge_Control);
            
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
