#include <24HJ128GP502.h>

#FUSES HS
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES CKSFSM                   //Clock Switching is enabled, fail Safe clock monitor is enabled
#FUSES NOJTAG                   //JTAG disabled
#FUSES NOIESO
#FUSES NOPUT

#device ICSP=1
#use delay(crystal=20000000)

#define CAN_USE_EXTENDED_ID   FALSE

//CAN config PICWizard
#define CAN_BRG_PRESCALAR   0
#define CAN_BRG_PROPAGATION_TIME   1
#define CAN_BRG_PHASE_SEGMENT_1   4
#define CAN_BRG_PHASE_SEGMENT_2   1
#define CAN_BRG_SYNCH_JUMP_WIDTH   0
#define CAN_BRG_SAM   FALSE
#define CAN_BRG_WAKE_FILTER   FALSE
#define CAN_BRG_SEG_2_PHASE_TS   TRUE

#define CS_LTC6811 PIN_A0
#define LED1 PIN_B13
#define LED2 PIN_B12
#define ERROR_LED PIN_B2
#define CS_LTC1296 PIN_A4

#PIN_SELECT SDO1=PIN_B6
#PIN_SELECT SCK1OUT=PIN_B5
#PIN_SELECT SDI1=PIN_B7


#use spi(MASTER, CLK=PIN_B5, DI=PIN_B7, DO=PIN_B6, BAUD=9600, MODE=3, BITS=8, stream=SP, MSB_FIRST)

#use rs232 (baud=9600, xmit=PIN_B11, rcv=PIN_B10)

#include <stdint.h>
#include <LT6811.c>
#include <LTC1296.c>
#include <can-PIC24.c>

#PIN_SELECT C1RX=PIN_B14
#PIN_SELECT C1TX=PIN_B15


//se definen las variables que se utilizan en el programa
int8 a=0,i=0, data_tosend[8], cob=1, sync=0;
int16 voltaje[12]={4121,4115,4125,4125,4125,4125,4125,4125,4125,4125,4125,4115}, total, v_cell_min,v_cell_max, temp[8], temp_prev[8];

int16 v_cell_minn=4120, v_min=4050, v_balanceo=4195, temp_max=700, temp_slave_max=690;
int8 diff_max=2, diff_min=10;
int32 tim=0;

struct rx_stat rxstat;
unsigned int32 rx_id;
unsigned int8 rx_buffer[8];
unsigned int8 rx_len;

int8 slave=10;

//TIMER2 INTERRUPT
#INT_TIMER2
void  timer2_isr(void) 
{
   tim++;
}

//CAN INTERRUPTS
#INT_CAN1
void can1_isr(void)
{
   // TODO: add CAN receive code here
}

#INT_C1RX
void  c1rx_isr(void) 
{
   if(can_kbhit())//Returns true if there is data in one of the receive buffers for CAN1
   {
      can_getd(rx_id, &rx_buffer[0], rx_len, rxstat); //Returns specified message/request and ID for CAN1
      if(rx_id==0x80)   
      {
         sync=1;
         v_cell_minn=(rx_buffer[0]<<8)|rx_buffer[1];
         v_min=(rx_buffer[2]<<8)|rx_buffer[3];
         v_balanceo=(rx_buffer[4]<<8)|rx_buffer[5];
         temp_max=(rx_buffer[6]<<8)|rx_buffer[7];
      }
      if(rx_id==0x81)
      {
         diff_max=rx_buffer[0];
      }
   }
}


//Programa principal
void main (void)
{
   output_high(LED1);
   output_high(LED2);
   setup_timer2(TMR_INTERNAL| TMR_DIV_BY_1, 4999);   //TIMER2 OVERFLOW every 0,5us

   can_init();

   can_enable_b_transfer(TRB0);  //make buffer 0 a transmit buffer  // 
   can_enable_b_transfer(TRB1);
   can_enable_b_transfer(TRB2);
   can_enable_b_transfer(TRB3);
   can_enable_b_transfer(TRB4); 
   can_enable_b_transfer(TRB5);
   can_enable_b_transfer(TRB6); 
   can_enable_b_transfer(TRB7);
   
   can_enable_interrupts(RB);
   enable_interrupts(INT_CAN1);

   enable_interrupts(INT_C1RX);
   enable_interrupts(INT_CAN1);
   enable_interrupts(INT_TIMER2);
   enable_interrupts(INTR_GLOBAL);
   output_low(LED1);
   
   //ADC voltage conversions
   START_ADC_VOLTAGES();
   tim=0;
   
   //Temperature readings
   ADC_LTC1296(&temp[0]);
   
   for(i=0;i<8;i++)
   {
      temp_prev[i]=temp[i];
   }
   
   while(tim<70) {}  //Waiting until conversions have been done
   
   //Voltage readings
   READ_VOLTAGES (&voltaje[0], total, v_cell_min, v_cell_max);
 
   delay_ms(2000);
   output_high(CS_LTC1296);
   output_high(CS_LTC6811);
   sync=0;

   while(true)
   {

      if(sync==1)
      {
         output_high(LED1);
         sync=0;
         tim=0;
         
         //CAN MANAGEMENT
         a=0;
         for(i=0;i<8;i=i+2)
         { 
            data_tosend[i]=voltaje[a]>>8;
            data_tosend[i+1]=voltaje[a];
            a++;
         }
         can_putd(0x82+slave*3,&data_tosend[0],8,0,0,0);
         for(i=0;i<8;i=i+2)
         {
            data_tosend[i]=voltaje[a]>>8;
            data_tosend[i+1]=voltaje[a];
            a++;
         }
         can_putd(0x83+slave*3,&data_tosend[0],8,0,0,0);
         if (slave!=1 && slave!=4 && slave!=7 && slave!=10 && slave!=13)
         {
            for(i=0;i<4;i=i+2)
            {
               data_tosend[i]=voltaje[a]>>8;
               data_tosend[i+1]=voltaje[a];
               a++;
            }
            can_putd(0x84+slave*3,&data_tosend[0],4,0,0,0);
         }
         
         a=0;
         for(i=0;i<8;i=i+2)
         { 
            data_tosend[i]=temp[a]>>8;
            data_tosend[i+1]=temp[a];
            a++;
         }
         can_putd((0xC0+slave*2),&data_tosend[0],8,0,0,0);
         
         for(i=0;i<8;i=i+2)
         { 
            data_tosend[i]=temp[a]>>8;
            data_tosend[i+1]=temp[a];
            a++;
         }
         can_putd((0xC1+slave*2),&data_tosend[0],8,0,0,0);

         //ADC voltage conversions
         START_ADC_VOLTAGES();
         tim=0;
         
         //Balancing evaluation
         BLANCEO(v_cell_minn, v_balanceo, v_min, diff_max, diff_min, &voltaje[0], temp_max, temp_slave_max);
         
         //Temperature readings
         ADC_LTC1296(&temp[0]);
         
         //Lowpass filter for temp readings
         for(i=0;i<8;i++)
         {
            temp[i]=(temp[i]+temp_prev[i])/2;
            temp_prev[i]=temp[i];
         }
         
         while(tim<70) {}  //Waiting until conversions have been done
         
         //Voltage readings
         READ_VOLTAGES (&voltaje[0], total, v_cell_min, v_cell_max);
      }
      else
      {
        output_low(LED1);
      }
   }
}
