#include <xc.h>
#include <stdint.h>
extern int rbuffer_external, cobid_external;
extern int AIRs_request, AIRs_state, do_precharge, do_precharge_charger, open_relays, secuence_0;
extern int AIRminus_state, AIRplus_state, precharge_state;
extern short close_AIRs, close_AIRs_charger, relays_state, sequence_state;
extern int synchronism, CAN_count, CAN_count_int, charger_count, BMS_alive;
extern long CAN_alive;
extern short alive_count;
extern int current_buffer[4], SoC, V_init, SoC_init, SoC_actual;
extern long total_voltage_actual, current; 
extern int min_cell_voltage_actual, max_cell_voltage_actual;
extern int min_cell_temperature_actual, max_cell_temperature_actual, avg_cell_temperature_actual, min_onboard_temperature_actual, max_onboard_temperature_actual, avg_onboard_temperature_actual, temperature_type;
extern int tt_buffer;
extern int sfr1, sfr2, sfr3;


void external_CAN_management ( int AIRminus_Control, int AIRplus_Control,  int Precharge_Control) 
{   
    //* RELAYS MANAGEMENT *//
    
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
