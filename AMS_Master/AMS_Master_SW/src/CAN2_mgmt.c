#include <xc.h>
#include <stdint.h>
extern int rbuffer_internal, cobid_internal;
extern int current_synchronism;
extern long CAN_alive;
extern int voltage[140], temperature[120];
extern int current_buffer[4];
extern long current; 
extern int balancing_0, balancing_1, balancing_2, balancing_3, balancing_4, balancing_5, balancing_6, balancing_7, balancing_8, balancing_9, balancing_10, balancing_11, balancing_12, balancing_13, balancing_14;
extern int tt_slave0, tt_slave1, tt_slave2, tt_slave3, tt_slave4, tt_slave5, tt_slave6, tt_slave7, tt_slave8, tt_slave9, tt_slave10, tt_slave11, tt_slave12, tt_slave13, tt_slave14, tt_hall;


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
