//Library to control the LT6811-2 for BMS applications (ONLY VALID IF THE ADDRESS IS 0000)
//
//
//This library contains the functions to make the voltage ADC conversions, the reading of these registers and the balancing
//
//
//It is necessary to define in the main program the SPI configuration with #USE SPI (...)
//It is also necessary to define the Chip select pin CS. Example #DEFINE CS pin_A1
//It is also necessary to include the stdint library in the main program before this one. Ex: #include <stdint.h> #include <LT6811.c>
//
//

void SUMAMA(uint8_t &a, uint8_t &b, uint8_t *x)
{
   x[0]=a+b;
   x[1]=a*b;
}



//
// START_ADC_VOLTAGES is a fucntion to start the ADC conversions, it is necessary to do it before read the registers where these conversions are saved. It last about 35 ms.
//

void START_ADC_VOLTAGES ()
{
   output_low(CS_LTC6811);
   delay_us(500);
   output_high(CS_LTC6811);
   delay_us(10);
   output_low(CS_LTC6811);
   spi_xfer(SP, 0x83);
   spi_xfer(SP, 0x60);
   spi_xfer(SP, 0x84);
   spi_xfer(SP, 0x78);
   output_high(CS_LTC6811);
}



//
// READ_VOLTAGES is a function to read the voltajes of the cells. It must be used after the START_ADC_VOLTAGES function.
//
// An example to use this function in the main program is: READ_VOLTAGES (&voltaja[0], total_v, v_cell_min, v_cell_max);
// The voltajes of the cells are saved in the voltaja array, the sum of all the cell voltajes are save in the total_v variable, the minimum and maximum cell voltaje are saved in v_cell_min and
// v_cell_max, respectively
//

void READ_VOLTAGES(uint16_t *voltaje, uint16_t &total, uint16_t v_min_aux, uint16_t v_max_aux)
{
   int8 data[6], data1[6], data2[6], data3[6], i, am;

   //Primer set de registros
   output_low(CS_LTC6811);
   delay_us(10);
   output_high(CS_LTC6811);
   delay_us(10);
   output_low(CS_LTC6811);
   spi_xfer(SP, 0x80);  
   spi_xfer(SP, 0x04);  
   spi_xfer(SP, 0x77);  
   spi_xfer(SP, 0xD6);
   for(i=0;i<6;++i)
   {
      data[i]=spi_xfer(SP, 0xFF); 
   }
   output_high(CS_LTC6811);
   
   
   //Segundo set de registros
   output_low(CS_LTC6811);
   delay_us(10);
   output_high(CS_LTC6811);
   delay_us(10);
   output_low(CS_LTC6811);
   spi_xfer(SP, 0x80);  
   spi_xfer(SP, 0x06);  
   spi_xfer(SP, 0xEA);  
   spi_xfer(SP, 0x80);
   for(i=0;i<6;++i)
   {
      data1[i]=spi_xfer(SP, 0xFF); 
   }
   output_high(CS_LTC6811);
   
   //Tercer set de registros
   output_low(CS_LTC6811);
   delay_us(10);
   output_high(CS_LTC6811);
   delay_us(10);
   output_low(CS_LTC6811);
   spi_xfer(SP, 0x80);  
   spi_xfer(SP, 0x08);  
   spi_xfer(SP, 0x2E);  
   spi_xfer(SP, 0x46);
   for(i=0;i<6;++i)
   {
      data2[i]=spi_xfer(SP, 0xFF); 
   }
   output_high(CS_LTC6811);
   
   //Cuarto set de registros
   output_low(CS_LTC6811);
   delay_us(10);
   output_high(CS_LTC6811);
   delay_us(10);
   output_low(CS_LTC6811);
   spi_xfer(SP, 0x80);  
   spi_xfer(SP, 0x0A);  
   spi_xfer(SP, 0xB3);  
   spi_xfer(SP, 0x10);
   for(i=0;i<6;++i)
   {
      data3[i]=spi_xfer(SP, 0xFF); 
   }
   output_high(CS_LTC6811);
   
   //calculo de los voltajes de las celdas y el voltaje total
   am=0;
   total=0;
   V_min_aux=0xFFFF;
   V_max_aux=0;
   for(i=0;i<6;i=i+2)
   {
      voltaje[am]=make16(data[i+1],data[i])/10;
      total=total+voltaje[am];
      if(voltaje[am]<V_min_aux) V_min_aux=voltaje[am];
      if(voltaje[am]>V_max_aux) V_max_aux=voltaje[am];
      am=am+1;
   }
   for(i=0;i<6;i=i+2)
   {
      voltaje[am]=make16(data1[i+1],data1[i])/10;
      total=total+voltaje[am];
      if(voltaje[am]<V_min_aux) V_min_aux=voltaje[am];
      if(voltaje[am]>V_max_aux) V_max_aux=voltaje[am];
      am=am+1;
   }
   for(i=0;i<6;i=i+2)
   {
      voltaje[am]=make16(data2[i+1],data2[i])/10;
      total=total+voltaje[am];
      if(voltaje[am]<V_min_aux) V_min_aux=voltaje[am];
      if(voltaje[am]>V_max_aux) V_max_aux=voltaje[am];
      am=am+1;
   }
   for(i=0;i<6;i=i+2)
   {
      voltaje[am]=make16(data3[i+1],data3[i])/10;
      total=total+voltaje[am];
      if(voltaje[am]<V_min_aux) V_min_aux=voltaje[am];
      if(voltaje[am]>V_max_aux) V_max_aux=voltaje[am];
      am=am+1;
   }  
   
}



//
// BALANCEO is a function that evaluate the conditions of balancing and give the order to the IC.
//
// The alogorith works the following way, it evaluates individualy each cell and if its voltage is over a limit (v_min) and the balancing temperature (temp_slave_max) is below the maximum (temp_max)
// and (the voltage is over a upper limit (v_balanceo) or the difference between the cell voltaje is bigger than a difference (diff_max) with the minimum cell voltage (v_cell_min)).
//

void BLANCEO(uint16_t &v_cell_min, uint16_t &v_balanceo, uint16_t &v_min, uint8_t &diff_max, uint8_t &diff_min, uint16_t *voltaje, uint16_t &temp_max, uint16_t &temp_slave_max)
{
   int8 i, a, balanceo1, balanceo2;
   int1 bal;
   int8 pec[6];
   int16 remainder=0, addr=0,out;
   const unsigned int16 crc15Table[256] = {0x0,0xc599, 0xceab, 0xb32, 0xd8cf, 0x1d56, 0x1664, 0xd3fd, 0xf407, 0x319e, 0x3aac,  //!<precomputed CRC15 Table
   0xff35, 0x2cc8, 0xe951, 0xe263, 0x27fa, 0xad97, 0x680e, 0x633c, 0xa6a5, 0x7558, 0xb0c1, 
   0xbbf3, 0x7e6a, 0x5990, 0x9c09, 0x973b, 0x52a2, 0x815f, 0x44c6, 0x4ff4, 0x8a6d, 0x5b2e,
   0x9eb7, 0x9585, 0x501c, 0x83e1, 0x4678, 0x4d4a, 0x88d3, 0xaf29, 0x6ab0, 0x6182, 0xa41b,
   0x77e6, 0xb27f, 0xb94d, 0x7cd4, 0xf6b9, 0x3320, 0x3812, 0xfd8b, 0x2e76, 0xebef, 0xe0dd, 
   0x2544, 0x2be, 0xc727, 0xcc15, 0x98c, 0xda71, 0x1fe8, 0x14da, 0xd143, 0xf3c5, 0x365c, 
   0x3d6e, 0xf8f7,0x2b0a, 0xee93, 0xe5a1, 0x2038, 0x7c2, 0xc25b, 0xc969, 0xcf0, 0xdf0d, 
   0x1a94, 0x11a6, 0xd43f, 0x5e52, 0x9bcb, 0x90f9, 0x5560, 0x869d, 0x4304, 0x4836, 0x8daf,
   0xaa55, 0x6fcc, 0x64fe, 0xa167, 0x729a, 0xb703, 0xbc31, 0x79a8, 0xa8eb, 0x6d72, 0x6640,
   0xa3d9, 0x7024, 0xb5bd, 0xbe8f, 0x7b16, 0x5cec, 0x9975, 0x9247, 0x57de, 0x8423, 0x41ba,
   0x4a88, 0x8f11, 0x57c, 0xc0e5, 0xcbd7, 0xe4e, 0xddb3, 0x182a, 0x1318, 0xd681, 0xf17b, 
   0x34e2, 0x3fd0, 0xfa49, 0x29b4, 0xec2d, 0xe71f, 0x2286, 0xa213, 0x678a, 0x6cb8, 0xa921, 
   0x7adc, 0xbf45, 0xb477, 0x71ee, 0x5614, 0x938d, 0x98bf, 0x5d26, 0x8edb, 0x4b42, 0x4070, 
   0x85e9, 0xf84, 0xca1d, 0xc12f, 0x4b6, 0xd74b, 0x12d2, 0x19e0, 0xdc79, 0xfb83, 0x3e1a, 0x3528, 
   0xf0b1, 0x234c, 0xe6d5, 0xede7, 0x287e, 0xf93d, 0x3ca4, 0x3796, 0xf20f, 0x21f2, 0xe46b, 0xef59, 
   0x2ac0, 0xd3a, 0xc8a3, 0xc391, 0x608, 0xd5f5, 0x106c, 0x1b5e, 0xdec7, 0x54aa, 0x9133, 0x9a01, 
   0x5f98, 0x8c65, 0x49fc, 0x42ce, 0x8757, 0xa0ad, 0x6534, 0x6e06, 0xab9f, 0x7862, 0xbdfb, 0xb6c9, 
   0x7350, 0x51d6, 0x944f, 0x9f7d, 0x5ae4, 0x8919, 0x4c80, 0x47b2, 0x822b, 0xa5d1, 0x6048, 0x6b7a, 
   0xaee3, 0x7d1e, 0xb887, 0xb3b5, 0x762c, 0xfc41, 0x39d8, 0x32ea, 0xf773, 0x248e, 0xe117, 0xea25, 
   0x2fbc, 0x846, 0xcddf, 0xc6ed, 0x374, 0xd089, 0x1510, 0x1e22, 0xdbbb, 0xaf8, 0xcf61, 0xc453, 
   0x1ca, 0xd237, 0x17ae, 0x1c9c, 0xd905, 0xfeff, 0x3b66, 0x3054, 0xf5cd, 0x2630, 0xe3a9, 0xe89b, 
   0x2d02, 0xa76f, 0x62f6, 0x69c4, 0xac5d, 0x7fa0, 0xba39, 0xb10b, 0x7492, 0x5368, 0x96f1, 0x9dc3, 
   0x585a, 0x8ba7, 0x4e3e, 0x450c, 0x8095};
   
   balanceo1=0;
   balanceo2=0;
   
   for (a=0;a<12;a++)
   {
      
      if(voltaje[a]>v_min && temp_slave_max<temp_max && (voltaje[a]> v_balanceo || (voltaje[a]-v_cell_min)>diff_max))
      {
         bal=1;
         if (a<8)
         {
            balanceo1=(balanceo1|(1<<a));
         }
         else
         {
            balanceo2=(balanceo2|(1<<(a-8)));
         }
      }
      else
      {
         bal=0;
      }
      
      
      if(bal==1)
      {
         pec[0]=0xF8;
         pec[1]=0;
         pec[2]=0;
         pec[3]=0;
         pec[4]=balanceo1;
         pec[5]=balanceo2;
         remainder = 16;
         for(i = 0; i<6;i++) 
         {
            addr = ((remainder>>7)^pec[i])&0xff;
            remainder = (remainder<<8)^crc15Table[addr];
         }
         out=remainder*2;
         
         output_low(CS_LTC6811);
         delay_us(10);
         output_high(CS_LTC6811);
         delay_us(10);
         output_low(CS_LTC6811);
         spi_xfer(SP, 0x80);  
         spi_xfer(SP, 0x01);  
         spi_xfer(SP, 0x4D);  
         spi_xfer(SP, 0x7A);
         
         spi_xfer(SP, 0xF8);
         spi_xfer(SP, 0x00);
         spi_xfer(SP, 0x00);
         spi_xfer(SP, 0x00);
         spi_xfer(SP, balanceo1);
         spi_xfer(SP, balanceo2);
         
         spi_xfer(SP, (out>>8));  
         spi_xfer(SP, out);

         output_high(CS_LTC6811);   
      }
      else
      {
         output_low(CS_LTC6811);
         delay_us(10);
         output_high(CS_LTC6811);
         delay_us(10);
         output_low(CS_LTC6811);
         spi_xfer(SP, 0x80);  
         spi_xfer(SP, 0x01);  
         spi_xfer(SP, 0x4D);  
         spi_xfer(SP, 0x7A);
         
         spi_xfer(SP, 0xF8);
         spi_xfer(SP, 0x00);
         spi_xfer(SP, 0x00);
         spi_xfer(SP, 0x00);
         spi_xfer(SP, 0x00);
         spi_xfer(SP, 0x00);
         
         spi_xfer(SP, 0xBE);  
         spi_xfer(SP, 0xE2);
   
         output_high(CS_LTC6811);  
      }
   }
   
}
