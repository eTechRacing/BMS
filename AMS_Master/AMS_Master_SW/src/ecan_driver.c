#include <xc.h>
#include "ecan1_config.h"
#include "ecan2_config.h"
#include <stdint.h>

//--------------------------Información útil-------------------------
// Para encontrar las líneas donde están definidos los puertos de CAN, hacer
// CNTRL+F y buscar SELECCIÓN


//*****************************************************************************
//                          ECAN1 FUNCTIONS
//*****************************************************************************

void can1_writefilter( int16_t n, int32_t identifier, uint16_t exide, uint16_t bufPnt, uint16_t maskSel )
{
    /******************************************************************************
 * Function:     void Ecan1WriteRxAcptFilter(int16_t n, int32_t identifier,
 *               uint16_t exide,uint16_t bufPnt,uint16_t maskSel)
 *
 * PreCondition:  None
 *
 * Input:         n-> Filter number [0-15]
 *                identifier-> Bit ordering is given below
 *                Filter Identifier (29-bits) :
 *                0b000f ffff ffff ffff ffff ffff ffff ffff
 *                     |____________|_____________________|
 *                        SID10:0          EID17:0
 *
 *               Filter Identifier (11-bits) :
 *               0b0000 0000 0000 0000 0000 0fff ffff ffff
 *                                           |___________|
 *                                             SID10:
 *               exide -> "0" for standard identifier
 *                        "1" for Extended identifier
 *               bufPnt -> Message buffer to store filtered message [0-15]
 *               maskSel -> Optinal Masking of identifier bits [0-3]
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:      Configures Acceptance filter "n" for ECAN1.
 *****************************************************************************/
    
    uint32_t    sid10_0 = 0;

    uint32_t    eid15_0 = 0;

    uint32_t    eid17_16 = 0;
    uint16_t    *sidRegAddr;
    uint16_t    *bufPntRegAddr;
    uint16_t    *maskSelRegAddr;
    uint16_t    *fltEnRegAddr;

    C1CTRL1bits.WIN = 1;

    // Obtain the Address of CiRXFnSID, CiBUFPNTn, CiFMSKSELn and CiFEN register for a given filter number "n"
    sidRegAddr = ( uint16_t * ) ( &C1RXF0SID + (n << 1) );
    bufPntRegAddr = ( uint16_t * ) ( &C1BUFPNT1 + (n >> 2) );
    maskSelRegAddr = ( uint16_t * ) ( &C1FMSKSEL1 + (n >> 3) );
    fltEnRegAddr = ( uint16_t * ) ( &C1FEN1 );

    // Bit-filed manupulation to write to Filter identifier register
    if( exide == 1 )
    {   // Filter Extended Identifier
        eid15_0 = ( identifier & 0xFFFF );
        eid17_16 = ( identifier >> 16 ) & 0x3;
        sid10_0 = ( identifier >> 18 ) & 0x7FF;

        *sidRegAddr = ( ((sid10_0) << 5) + 0x8 ) + eid17_16;    // Write to CiRXFnSID Register
        *( sidRegAddr + 1 ) = eid15_0;  // Write to CiRXFnEID Register
    }
    else
    {   // Filter Standard Identifier
        sid10_0 = ( identifier & 0x7FF );
        *sidRegAddr = ( sid10_0 ) << 5; // Write to CiRXFnSID Register
        *( sidRegAddr + 1 ) = 0;        // Write to CiRXFnEID Register
    }

    *bufPntRegAddr = ( *bufPntRegAddr ) & ( 0xFFFF - (0xF << (4 * (n & 3))) );      // clear nibble
    *bufPntRegAddr = ( (bufPnt << (4 * (n & 3))) | (*bufPntRegAddr) );              // Write to C1BUFPNTn Register
    *maskSelRegAddr = ( *maskSelRegAddr ) & ( 0xFFFF - (0x3 << ((n & 7) * 2)) );    // clear 2 bits
    *maskSelRegAddr = ( (maskSel << (2 * (n & 7))) | (*maskSelRegAddr) );           // Write to C1FMSKSELn Register
    *fltEnRegAddr = ( (0x1 << n) | (*fltEnRegAddr) );   // Write to C1FEN1 Register
    C1CTRL1bits.WIN = 0;
}

void can1_writemask( int16_t m, int32_t identifier, uint16_t mide, uint16_t exide )
{
    /******************************************************************************
 * Function:     void Ecan1WriteRxAcptMask(int16_t m, int32_t identifier,
 *               uint16_t mide, uint16_t exide)
 *
 * PreCondition:  None
 *
 * Input:        m-> Mask number [0-2]
                 identifier-> Bit ordering is given below n-> Filter number [0-15]
 *                identifier-> Bit ordering is given below
 *                Filter mask Identifier (29-bits) :
 *                0b000f ffff ffff ffff ffff ffff ffff ffff
 *                     |____________|_____________________|
 *                        SID10:0          EID17:0
 *
 *               Filter mask Identifier (11-bits) :
 *               0b0000 0000 0000 0000 0000 0fff ffff ffff
 *                                           |___________|
 *                                             SID10:
 *               mide ->  "0"  Match either standard or extended address message
 *                             if filters match
 *                        "1"  Match only message types that correpond to
 *                             'exide' bit in filter
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:      Configures Acceptance filter "n" for ECAN1.
 *****************************************************************************/
    
    uint32_t    sid10_0 = 0;

    uint32_t    eid15_0 = 0;

    uint32_t    eid17_16 = 0;
    uint16_t    *maskRegAddr;

    C1CTRL1bits.WIN = 1;

    // Obtain the Address of CiRXMmSID register for given Mask number "m"
    maskRegAddr = ( uint16_t * ) ( &C1RXM0SID + (m << 1) );

    // Bit-filed manupulation to write to Filter Mask register
    if( exide == 1 )
    {   // Filter Extended Identifier
        eid15_0 = ( identifier & 0xFFFF );
        eid17_16 = ( identifier >> 16 ) & 0x3;
        sid10_0 = ( identifier >> 18 ) & 0x7FF;

        if( mide == 1 )
        {
            *maskRegAddr = ( (sid10_0) << 5 ) + 0x0008 + eid17_16;  // Write to CiRXMnSID Register
        }
        else
        {
            *maskRegAddr = ( (sid10_0) << 5 ) + eid17_16;           // Write to CiRXMnSID Register
        }

        *( maskRegAddr + 1 ) = eid15_0; // Write to CiRXMnEID Register
    }
    else
    {   // Filter Standard Identifier
        sid10_0 = ( identifier & 0x7FF );
        if( mide == 1 )
        {
            *maskRegAddr = ( (sid10_0) << 5 ) + 0x0008; // Write to CiRXMnSID Register
        }
        else
        {
            *maskRegAddr = ( sid10_0 ) << 5;            // Write to CiRXMnSID Register
        }

        *( maskRegAddr + 1 ) = 0;                       // Write to CiRXMnEID Register
    }

    C1CTRL1bits.WIN = 0;
}

void can1_setbufferid( uint16_t buf, int32_t txIdentifier, uint16_t ide, uint16_t remoteTransmit )
{
    /******************************************************************************
 * Function:     void Ecan1WriteTxMsgBufId(uint16_t buf, int32_t txIdentifier, uint16_t ide,
 *               uint16_t remoteTransmit)
 *
 * PreCondition:  None
 *
 * Input:        buf    -> Transmit Buffer Number
 *               txIdentifier ->
 *               Extended Identifier (29-bits):
 *                0b000f ffff ffff ffff ffff ffff ffff ffff
 *                     |____________|_____________________|
 *                        SID10:0          EID17:0
 *
 *               Standard Identifier (11-bits) :
 *               0b0000 0000 0000 0000 0000 0fff ffff ffff
 *                                           |___________|
 *                                             SID10:
 *                 Standard Message Format:
 *                                             Word0 : 0b000f ffff ffff ffff
 *                                                          |____________|||___
 *                                                             SID10:0   SRR   IDE
 *                                             Word1 : 0b0000 0000 0000 0000
 *                                                            |____________|
 *                                                               EID17:6
 *                                             Word2 : 0b0000 00f0 0000 ffff
 *                                                       |_____||           |__|
 *                                                       EID5:0 RTR         DLC
 *                Extended Message Format:
 *                                          Word0 : 0b000f ffff ffff ffff
 *                                                       |____________|||___
 *                                                          SID10:0   SRR   IDE
 *                                          Word1 : 0b0000 ffff ffff ffff
 *                                                         |____________|
 *                                                               EID17:6
 *                                          Word2 : 0bffff fff0 0000 ffff
 *                                                    |_____||           |__|
 *                                                    EID5:0 RTR         DLC
 *             ide -> "0"  Message will transmit standard identifier
 *                    "1"  Message will transmit extended identifier
 *
 *            remoteTransmit -> "0" Message transmitted is a normal message
 *                              "1" Message transmitted is a remote message
 *            Standard Message Format:
 *                                          Word0 : 0b000f ffff ffff ff1f
 *                                                       |____________|||___
 *                                                          SID10:0   SRR   IDE
 *                                          Word1 : 0b0000 0000 0000 0000
 *                                                         |____________|
 *                                                            EID17:6
 *                                          Word2 : 0b0000 0010 0000 ffff
 *                                                  |_____||           |__|
 *                                                  EID5:0 RTR         DLC
 *
 *         Extended Message Format:
 *                                         Word0 : 0b000f ffff ffff ff1f
 *                                                      |____________|||___
 *                                                        SID10:0   SRR   IDE
 *                                         Word1 : 0b0000 ffff ffff ffff
 *                                                        |____________|
 *                                                          EID17:6
 *                                         Word2 : 0bffff ff10 0000 ffff
 *                                                   |_____||           |__|
 *                                                   EID5:0 RTR         DLC
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:      This function configures ECAN1 message buffer.
 *****************************************************************************/
    
    uint32_t    word0 = 0;

    uint32_t    word1 = 0;

    uint32_t    word2 = 0;
    uint32_t    sid10_0 = 0;
    uint32_t    eid5_0 = 0;
    uint32_t    eid17_6 = 0;

    if( ide )
    {
        eid5_0 = ( txIdentifier & 0x3F );
        eid17_6 = ( txIdentifier >> 6 ) & 0xFFF;
        sid10_0 = ( txIdentifier >> 18 ) & 0x7FF;
        word1 = eid17_6;
    }
    else
    {
        sid10_0 = ( txIdentifier & 0x7FF );
    }

    if( remoteTransmit == 1 )
    {   // Transmit Remote Frame
        word0 = ( (sid10_0 << 2) | ide | 0x2 );
        word2 = ( (eid5_0 << 10) | 0x0200 );
    }
    else
    {
        word0 = ( (sid10_0 << 2) | ide );
        word2 = ( eid5_0 << 10 );
    }

    // Obtain the Address of Transmit Buffer in DMA RAM for a given Transmit Buffer number
    if( ide )
    {
        ecan1msgBuf[buf][0] = ( word0 | 0x0002 );
    }
    else
    {
        ecan1msgBuf[buf][0] = word0;
    }

    ecan1msgBuf[buf][1] = word1;
    ecan1msgBuf[buf][2] = word2;
}

void can1_write( uint16_t buf, uint16_t dataLength, uint16_t cobid, uint16_t data1, uint16_t data2, uint16_t data3, uint16_t data4 )
{ 
    /******************************************************************************
 * Function:     void Ecan1WriteTxMsgBufData(uint16_t buf, uint16_t dataLength,
 *    uint16_t data1, uint16_t data2, uint16_t data3, uint16_t data4)
 *
 * PreCondition:  None
 *
 * Input:            buf    -> Transmit Buffer Number
 *              dataLength  -> data length in bytes.
 *               actual data -> data1, data2, data3, data4
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:      This function transmits ECAN data.
 *****************************************************************************/
    
    can1_setbufferid(buf,cobid,0,0);
    
    ecan1msgBuf[buf][2] = ( (ecan1msgBuf[buf][2] & 0xFFF0) + dataLength );
 
    ecan1msgBuf[buf][3] = ((data1<<8)&(0xFF00))|((data1>>8)&(0x00FF));
    ecan1msgBuf[buf][4] = ((data2<<8)&(0xFF00))|((data2>>8)&(0x00FF));
    ecan1msgBuf[buf][5] = ((data3<<8)&(0xFF00))|((data3>>8)&(0x00FF));
    ecan1msgBuf[buf][6] = ((data4<<8)&(0xFF00))|((data4>>8)&(0x00FF));
}

void can1_writebytes( uint16_t buf, uint16_t dataLength, uint16_t cobid, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8 )
{
    uint16_t data1, data2, data3, data4, data5, data6, data7, data8;
 
    can1_setbufferid(buf,cobid,0,0);
    
    data1 = byte1;
    data2 = byte2;
    data3 = byte3;
    data4 = byte4;
    data5 = byte5;
    data6 = byte6;
    data7 = byte7;
    data8 = byte8;
    
    ecan1msgBuf[buf][2] = ( (ecan1msgBuf[buf][2] & 0xFFF0) + dataLength );

    ecan1msgBuf[buf][3] = ((data2<<8)&(0xFF00))|(data1&(0x00FF));
    ecan1msgBuf[buf][4] = ((data4<<8)&(0xFF00))|(data3&(0x00FF));
    ecan1msgBuf[buf][5] = ((data6<<8)&(0xFF00))|(data5&(0x00FF));
    ecan1msgBuf[buf][6] = ((data8<<8)&(0xFF00))|(data7&(0x00FF));
}

void can1_disablefilter( int16_t n )
{
    /******************************************************************************
 * Function:     void Ecan1DisableRXFilter(int16_t n)
 *
 * PreCondition:  None
 *
 * Input:          n -> Filter number [0-15]
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:          Disables RX Acceptance Filter.
 *****************************************************************************/
    
    uint16_t    *fltEnRegAddr;
    C1CTRL1bits.WIN = 1;
    fltEnRegAddr = ( uint16_t * ) ( &C1FEN1 );
    *fltEnRegAddr = ( *fltEnRegAddr ) & ( 0xFFFF - (0x1 << n) );
    C1CTRL1bits.WIN = 0;
}

uint8_t can1_getrxbuf(void)
{
    uint8_t a;
    
    if(C1RXFUL1bits.RXFUL0)
    {
        a=0;
    }
    else if(C1RXFUL1bits.RXFUL1)
    {
        a=1;
    }
    else if(C1RXFUL1bits.RXFUL2)
    {
        a=2;
    }
    else if(C1RXFUL1bits.RXFUL3)
    {
        a=3;
    }
    else if(C1RXFUL1bits.RXFUL4)
    {
        a=4;
    }
    else if(C1RXFUL1bits.RXFUL5)
    {
        a=5;
    }
    else if(C1RXFUL1bits.RXFUL6)
    {
        a=6;
    }
    else if(C1RXFUL1bits.RXFUL7)
    {
        a=7;
    }
    else if(C1RXFUL1bits.RXFUL8)
    {
        a=8;
    }
    else if(C1RXFUL1bits.RXFUL9)
    {
        a=9;
    }
    else if(C1RXFUL1bits.RXFUL10)
    {
        a=10;
    }
    else if(C1RXFUL1bits.RXFUL11)
    {
        a=11;
    }
    else if(C1RXFUL1bits.RXFUL12)
    {
        a=12;
    }
    else if(C1RXFUL1bits.RXFUL13)
    {
        a=13;
    }
    else if(C1RXFUL1bits.RXFUL14)
    {
        a=14;
    }
    else if(C1RXFUL1bits.RXFUL15)
    {
        a=15;
    }
    else if(C1RXFUL2bits.RXFUL16)
    {
        a=16;
    }
    else if(C1RXFUL2bits.RXFUL17)
    {
        a=17;
    }
    else if(C1RXFUL2bits.RXFUL18)
    {
        a=18;
    }
    else if(C1RXFUL2bits.RXFUL19)
    {
        a=19;
    }
    else if(C1RXFUL2bits.RXFUL20)
    {
        a=20;
    }
    else if(C1RXFUL2bits.RXFUL21)
    {
        a=21;
    }
    else if(C1RXFUL2bits.RXFUL22)
    {
        a=22;
    }
    else if(C1RXFUL2bits.RXFUL23)
    {
        a=23;
    }
    else if(C1RXFUL2bits.RXFUL24)
    {
        a=24;
    }
    else if(C1RXFUL2bits.RXFUL25)
    {
        a=25;
    }
    else if(C1RXFUL2bits.RXFUL26)
    {
        a=26;
    }
    else if(C1RXFUL2bits.RXFUL27)
    {
        a=27;
    }
    else if(C1RXFUL2bits.RXFUL28)
    {
        a=28;
    }
    else if(C1RXFUL2bits.RXFUL29)
    {
        a=29;
    }
    else if(C1RXFUL2bits.RXFUL30)
    {
        a=30;
    }
    else if(C1RXFUL2bits.RXFUL31)
    {
        a=31;
    }
    else
    {
        a=255;
    }
    
    return(a);
    
}

uint8_t can1_isempty(void)
{
    if ((C1RXFUL1==0)&(C1RXFUL2==0))
    {
        return(1);
    }
    else
    {
        return(0);
    }
}

uint32_t can1_getcobid(uint8_t buf)
{
    uint32_t cobid;
    uint32_t c_ext_1;
    uint32_t c_ext_2;
    uint32_t c_ext_3;
    uint16_t ext;
    
    ext = ecan1msgBuf[buf][0] & 0x1;
    
    if (ext==0)
    {
        cobid = ((ecan1msgBuf[buf][0]>>2)&(0x7FF));
    }
    else
    {
        c_ext_1 = ecan1msgBuf[buf][0];
        c_ext_1 = (c_ext_1<<16)&(0x7FF0000);
        c_ext_2 = ecan1msgBuf[buf][1];
        c_ext_2 = (c_ext_2<<6)&(0x3FFC0);
        c_ext_3 = (ecan1msgBuf[buf][2]>>10)&(0x3F);
        
        cobid = c_ext_1|c_ext_2|c_ext_3;
    }
    
    return(cobid);
    
}

uint8_t can1_getlength(uint8_t buf)
{
    uint8_t leng;
    
    leng = ecan1msgBuf[buf][2]&0xF;
    return(leng);
}

uint16_t can1_getdata(uint8_t buf, uint8_t num)
{
    uint16_t data;
    
    data = ecan1msgBuf[buf][num+3];
    data = ((data<<8)&(0xFF00))|((data>>8)&(0x00FF));
    
    return(data);
}


//*****************************************************************************
//                          ECAN2 FUNCTIONS
//*****************************************************************************

void can2_writefilter( int16_t n, int32_t identifier, uint16_t exide, uint16_t bufPnt, uint16_t maskSel )
{
    /******************************************************************************
 * Function:     void Ecan2WriteRxAcptFilter(int16_t n, int32_t identifier,
 *               uint16_t exide,uint16_t bufPnt,uint16_t maskSel)
 *
 * PreCondition:  None
 *
 * Input:         n-> Filter number [0-15]
 *                identifier-> Bit ordering is given below
 *                Filter Identifier (29-bits) :
 *                0b000f ffff ffff ffff ffff ffff ffff ffff
 *                     |____________|_____________________|
 *                        SID10:0          EID17:0
 *
 *               Filter Identifier (11-bits) :
 *               0b0000 0000 0000 0000 0000 0fff ffff ffff
 *                                           |___________|
 *                                             SID10:
 *               exide -> "0" for standard identifier
 *                        "1" for Extended identifier
 *               bufPnt -> Message buffer to store filtered message [0-15]
 *               maskSel -> Optinal Masking of identifier bits [0-3]
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:      Configures Acceptance filter "n" for ECAN2.
 *****************************************************************************/
    
    uint32_t    sid10_0 = 0;

    uint32_t    eid15_0 = 0;

    uint32_t    eid17_16 = 0;
    uint16_t    *sidRegAddr;
    uint16_t    *bufPntRegAddr;
    uint16_t    *maskSelRegAddr;
    uint16_t    *fltEnRegAddr;

    C2CTRL1bits.WIN = 1;

    // Obtain the Address of CiRXFnSID, CiBUFPNTn, CiFMSKSELn and CiFEN register for a given filter number "n"
    sidRegAddr = ( uint16_t * ) ( &C2RXF0SID + (n << 1) );
    bufPntRegAddr = ( uint16_t * ) ( &C2BUFPNT1 + (n >> 2) );
    maskSelRegAddr = ( uint16_t * ) ( &C2FMSKSEL1 + (n >> 3) );
    fltEnRegAddr = ( uint16_t * ) ( &C2FEN1 );

    // Bit-filed manupulation to write to Filter identifier register
    if( exide == 1 )
    {   // Filter Extended Identifier
        eid15_0 = ( identifier & 0xFFFF );
        eid17_16 = ( identifier >> 16 ) & 0x3;
        sid10_0 = ( identifier >> 18 ) & 0x7FF;

        *sidRegAddr = ( ((sid10_0) << 5) + 0x8 ) + eid17_16;    // Write to CiRXFnSID Register
        *( sidRegAddr + 1 ) = eid15_0;  // Write to CiRXFnEID Register
    }
    else
    {   // Filter Standard Identifier
        sid10_0 = ( identifier & 0x7FF );
        *sidRegAddr = ( sid10_0 ) << 5; // Write to CiRXFnSID Register
        *( sidRegAddr + 1 ) = 0;        // Write to CiRXFnEID Register
    }

    *bufPntRegAddr = ( *bufPntRegAddr ) & ( 0xFFFF - (0xF << (4 * (n & 3))) );      // clear nibble
    *bufPntRegAddr = ( (bufPnt << (4 * (n & 3))) | (*bufPntRegAddr) );              // Write to C1BUFPNTn Register
    *maskSelRegAddr = ( *maskSelRegAddr ) & ( 0xFFFF - (0x3 << ((n & 7) * 2)) );    // clear 2 bits
    *maskSelRegAddr = ( (maskSel << (2 * (n & 7))) | (*maskSelRegAddr) );           // Write to C1FMSKSELn Register
    *fltEnRegAddr = ( (0x1 << n) | (*fltEnRegAddr) );   // Write to C1FEN1 Register
    C2CTRL1bits.WIN = 0;
}

void can2_writemask( int16_t m, int32_t identifier, uint16_t mide, uint16_t exide )
{
    /******************************************************************************
 * Function:     void Ecan2WriteRxAcptMask(int16_t m, int32_t identifier,
 *               uint16_t mide, uint16_t exide)
 *
 * PreCondition:  None
 *
 * Input:        m-> Mask number [0-2]
                 identifier-> Bit ordering is given below n-> Filter number [0-15]
 *                identifier-> Bit ordering is given below
 *                Filter mask Identifier (29-bits) :
 *                0b000f ffff ffff ffff ffff ffff ffff ffff
 *                     |____________|_____________________|
 *                        SID10:0          EID17:0
 *
 *               Filter mask Identifier (11-bits) :
 *               0b0000 0000 0000 0000 0000 0fff ffff ffff
 *                                           |___________|
 *                                             SID10:
 *               mide ->  "0"  Match either standard or extended address message
 *                             if filters match
 *                        "1"  Match only message types that correpond to
 *                             'exide' bit in filter
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:      Configures Acceptance filter "n" for ECAN2.
 *****************************************************************************/
    
    uint32_t    sid10_0 = 0;

    uint32_t    eid15_0 = 0;

    uint32_t    eid17_16 = 0;
    uint16_t    *maskRegAddr;

    C2CTRL1bits.WIN = 1;

    // Obtain the Address of CiRXMmSID register for given Mask number "m"
    maskRegAddr = ( uint16_t * ) ( &C2RXM0SID + (m << 1) );

    // Bit-filed manupulation to write to Filter Mask register
    if( exide == 1 )
    {   // Filter Extended Identifier
        eid15_0 = ( identifier & 0xFFFF );
        eid17_16 = ( identifier >> 16 ) & 0x3;
        sid10_0 = ( identifier >> 18 ) & 0x7FF;

        if( mide == 1 )
        {
            *maskRegAddr = ( (sid10_0) << 5 ) + 0x0008 + eid17_16;  // Write to CiRXMnSID Register
        }
        else
        {
            *maskRegAddr = ( (sid10_0) << 5 ) + eid17_16;           // Write to CiRXMnSID Register
        }

        *( maskRegAddr + 1 ) = eid15_0; // Write to CiRXMnEID Register
    }
    else
    {   // Filter Standard Identifier
        sid10_0 = ( identifier & 0x7FF );
        if( mide == 1 )
        {
            *maskRegAddr = ( (sid10_0) << 5 ) + 0x0008; // Write to CiRXMnSID Register
        }
        else
        {
            *maskRegAddr = ( sid10_0 ) << 5;            // Write to CiRXMnSID Register   
        }

        *( maskRegAddr + 1 ) = 0;                       // Write to CiRXMnEID Register
    }

    C2CTRL1bits.WIN = 0;
}

void can2_setbufferid( uint16_t buf, int32_t txIdentifier, uint16_t ide, uint16_t remoteTransmit )
{
    /******************************************************************************
 * Function:     void Ecan2WriteTxMsgBufId(uint16_t buf, int32_t txIdentifier, uint16_t ide,
 *               uint16_t remoteTransmit)
 *
 * PreCondition:  None
 *
 * Input:        buf    -> Transmit Buffer Number
 *               txIdentifier ->
 *               Extended Identifier (29-bits):
 *                0b000f ffff ffff ffff ffff ffff ffff ffff
 *                     |____________|_____________________|
 *                        SID10:0          EID17:0
 *
 *               Standard Identifier (11-bits) :
 *               0b0000 0000 0000 0000 0000 0fff ffff ffff
 *                                           |___________|
 *                                             SID10:
 *                 Standard Message Format:
 *                                             Word0 : 0b000f ffff ffff ffff
 *                                                          |____________|||___
 *                                                             SID10:0   SRR   IDE
 *                                             Word1 : 0b0000 0000 0000 0000
 *                                                            |____________|
 *                                                               EID17:6
 *                                             Word2 : 0b0000 00f0 0000 ffff
 *                                                       |_____||           |__|
 *                                                       EID5:0 RTR         DLC
 *                Extended Message Format:
 *                                          Word0 : 0b000f ffff ffff ffff
 *                                                       |____________|||___
 *                                                          SID10:0   SRR   IDE
 *                                          Word1 : 0b0000 ffff ffff ffff
 *                                                         |____________|
 *                                                               EID17:6
 *                                          Word2 : 0bffff fff0 0000 ffff
 *                                                    |_____||           |__|
 *                                                    EID5:0 RTR         DLC
 *             ide -> "0"  Message will transmit standard identifier
 *                    "1"  Message will transmit extended identifier
 *
 *            remoteTransmit -> "0" Message transmitted is a normal message
 *                              "1" Message transmitted is a remote message
 *            Standard Message Format:
 *                                          Word0 : 0b000f ffff ffff ff1f
 *                                                       |____________|||___
 *                                                          SID10:0   SRR   IDE
 *                                          Word1 : 0b0000 0000 0000 0000
 *                                                         |____________|
 *                                                            EID17:6
 *                                          Word2 : 0b0000 0010 0000 ffff
 *                                                  |_____||           |__|
 *                                                  EID5:0 RTR         DLC
 *
 *         Extended Message Format:
 *                                         Word0 : 0b000f ffff ffff ff1f
 *                                                      |____________|||___
 *                                                        SID10:0   SRR   IDE
 *                                         Word1 : 0b0000 ffff ffff ffff
 *                                                        |____________|
 *                                                          EID17:6
 *                                         Word2 : 0bffff ff10 0000 ffff
 *                                                   |_____||           |__|
 *                                                   EID5:0 RTR         DLC
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:      This function configures ECAN2 message buffer.
 *****************************************************************************/
    
    uint32_t    word0 = 0;

    uint32_t    word1 = 0;

    uint32_t    word2 = 0;
    uint32_t    sid10_0 = 0;
    uint32_t    eid5_0 = 0;
    uint32_t    eid17_6 = 0;

    if( ide )
    {
        eid5_0 = ( txIdentifier & 0x3F );
        eid17_6 = ( txIdentifier >> 6 ) & 0xFFF;
        sid10_0 = ( txIdentifier >> 18 ) & 0x7FF;
        word1 = eid17_6;
    }
    else
    {
        sid10_0 = ( txIdentifier & 0x7FF );
    }

    if( remoteTransmit == 1 )
    {   // Transmit Remote Frame
        word0 = ( (sid10_0 << 2) | ide | 0x2 );
        word2 = ( (eid5_0 << 10) | 0x0200 );
    }
    else
    {
        word0 = ( (sid10_0 << 2) | ide );
        word2 = ( eid5_0 << 10 );
    }

    // Obtain the Address of Transmit Buffer in DMA RAM for a given Transmit Buffer number
    if( ide )
    {
        ecan2msgBuf[buf][0] = ( word0 | 0x0002 );
    }
    else
    {
        ecan2msgBuf[buf][0] = word0;
    }

    ecan2msgBuf[buf][1] = word1;
    ecan2msgBuf[buf][2] = word2;
}

void can2_write( uint16_t buf, uint16_t dataLength, uint16_t cobid, uint16_t data1, uint16_t data2, uint16_t data3, uint16_t data4 )
{
    /******************************************************************************
 * Function:     void Ecan2WriteTxMsgBufData(uint16_t buf, uint16_t dataLength,
 *    uint16_t data1, uint16_t data2, uint16_t data3, uint16_t data4)
 *
 * PreCondition:  None
 *
 * Input:            buf    -> Transmit Buffer Number
 *              dataLength  -> data length in bytes.
 *               actual data -> data1, data2, data3, data4
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:      This function transmits ECAN data.
 *****************************************************************************/
    
    can2_setbufferid(buf,cobid,0,0);
    
    ecan2msgBuf[buf][2] = ( (ecan2msgBuf[buf][2] & 0xFFF0) + dataLength );

    ecan2msgBuf[buf][3] = ((data1<<8)&(0xFF00))|((data1>>8)&(0x00FF));
    ecan2msgBuf[buf][4] = ((data2<<8)&(0xFF00))|((data2>>8)&(0x00FF));
    ecan2msgBuf[buf][5] = ((data3<<8)&(0xFF00))|((data3>>8)&(0x00FF));
    ecan2msgBuf[buf][6] = ((data4<<8)&(0xFF00))|((data4>>8)&(0x00FF));
}

void can2_writebytes( uint16_t buf, uint16_t dataLength, uint16_t cobid, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8 )
{
    uint16_t data1, data2, data3, data4, data5, data6, data7, data8;
    
    can2_setbufferid(buf,cobid,0,0);
    
    data1 = byte1;
    data2 = byte2;
    data3 = byte3;
    data4 = byte4;
    data5 = byte5;
    data6 = byte6;
    data7 = byte7;
    data8 = byte8;
    
    ecan2msgBuf[buf][2] = ( (ecan2msgBuf[buf][2] & 0xFFF0) + dataLength );

    ecan2msgBuf[buf][3] = ((data2<<8)&(0xFF00))|(data1&(0x00FF));
    ecan2msgBuf[buf][4] = ((data4<<8)&(0xFF00))|(data3&(0x00FF));
    ecan2msgBuf[buf][5] = ((data6<<8)&(0xFF00))|(data5&(0x00FF));
    ecan2msgBuf[buf][6] = ((data8<<8)&(0xFF00))|(data7&(0x00FF));
}

void can2_disablefilter( int16_t n )
{
    /******************************************************************************
 * Function:     void Ecan1DisableRXFilter(int16_t n)
 *
 * PreCondition:  None
 *
 * Input:          n -> Filter number [0-15]
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:          Disables RX Acceptance Filter.
 *****************************************************************************/
    
    uint16_t    *fltEnRegAddr;
    C2CTRL1bits.WIN = 1;
    fltEnRegAddr = ( uint16_t * ) ( &C2FEN1 );
    *fltEnRegAddr = ( *fltEnRegAddr ) & ( 0xFFFF - (0x1 << n) );
    C2CTRL1bits.WIN = 0;
}

uint8_t can2_getrxbuf(void)
{
    uint8_t a;
    
    if(C2RXFUL1bits.RXFUL0)
    {
        a=0;
    }
    else if(C2RXFUL1bits.RXFUL1)
    {
        a=1;
    }
    else if(C2RXFUL1bits.RXFUL2)
    {
        a=2;
    }
    else if(C2RXFUL1bits.RXFUL3)
    {
        a=3;
    }
    else if(C2RXFUL1bits.RXFUL4)
    {
        a=4;
    }
    else if(C2RXFUL1bits.RXFUL5)
    {
        a=5;
    }
    else if(C2RXFUL1bits.RXFUL6)
    {
        a=6;
    }
    else if(C2RXFUL1bits.RXFUL7)
    {
        a=7;
    }
    else if(C2RXFUL1bits.RXFUL8)
    {
        a=8;
    }
    else if(C2RXFUL1bits.RXFUL9)
    {
        a=9;
    }
    else if(C2RXFUL1bits.RXFUL10)
    {
        a=10;
    }
    else if(C2RXFUL1bits.RXFUL11)
    {
        a=11;
    }
    else if(C2RXFUL1bits.RXFUL12)
    {
        a=12;
    }
    else if(C2RXFUL1bits.RXFUL13)
    {
        a=13;
    }
    else if(C2RXFUL1bits.RXFUL14)
    {
        a=14;
    }
    else if(C2RXFUL1bits.RXFUL15)
    {
        a=15;
    }
    else if(C2RXFUL2bits.RXFUL16)
    {
        a=16;
    }
    else if(C2RXFUL2bits.RXFUL17)
    {
        a=17;
    }
    else if(C2RXFUL2bits.RXFUL18)
    {
        a=18;
    }
    else if(C2RXFUL2bits.RXFUL19)
    {
        a=19;
    }
    else if(C2RXFUL2bits.RXFUL20)
    {
        a=20;
    }
    else if(C2RXFUL2bits.RXFUL21)
    {
        a=21;
    }
    else if(C2RXFUL2bits.RXFUL22)
    {
        a=22;
    }
    else if(C2RXFUL2bits.RXFUL23)
    {
        a=23;
    }
    else if(C2RXFUL2bits.RXFUL24)
    {
        a=24;
    }
    else if(C2RXFUL2bits.RXFUL25)
    {
        a=25;
    }
    else if(C2RXFUL2bits.RXFUL26)
    {
        a=26;
    }
    else if(C2RXFUL2bits.RXFUL27)
    {
        a=27;
    }
    else if(C2RXFUL2bits.RXFUL28)
    {
        a=28;
    }
    else if(C2RXFUL2bits.RXFUL29)
    {
        a=29;
    }
    else if(C2RXFUL2bits.RXFUL30)
    {
        a=30;
    }
    else if(C2RXFUL2bits.RXFUL31)
    {
        a=31;
    }
    else
    {
        a=255;
    }
    
    return(a);
    
}

uint8_t can2_isempty(void)
{
    if ((C2RXFUL1==0)&(C2RXFUL2==0))
    {
        return(1);
    }
    else
    {
        return(0);
    }
}

uint32_t can2_getcobid(uint8_t buf)
{
    uint32_t cobid;
    uint32_t c_ext_1;
    uint32_t c_ext_2;
    uint32_t c_ext_3;
    uint16_t ext;
    
    ext = ecan2msgBuf[buf][0] & 0x1;
    
    if (ext==0)
    {
        cobid = ((ecan2msgBuf[buf][0]>>2)&(0x7FF));
    }
    else
    {
        c_ext_1 = ecan2msgBuf[buf][0];
        c_ext_1 = (c_ext_1<<16)&(0x7FF0000);
        c_ext_2 = ecan2msgBuf[buf][1];
        c_ext_2 = (c_ext_2<<6)&(0x3FFC0);
        c_ext_3 = (ecan2msgBuf[buf][2]>>10)&(0x3F);
        
        cobid = c_ext_1|c_ext_2|c_ext_3;
    }
    
    return(cobid);
    
}

uint8_t can2_getlength(uint8_t buf)
{
    uint8_t leng;
    
    leng = ecan2msgBuf[buf][2]&0xF;
    return(leng);
}

uint16_t can2_getdata(uint8_t buf, uint8_t num)
{
    uint16_t data;
    
    data = ecan2msgBuf[buf][num+3];
    data = ((data<<8)&(0xFF00))|((data>>8)&(0x00FF));
    
    return(data);
}


//*****************************************************************************
//                          ECAN1 CONFIGURATION
//*****************************************************************************

void DMA0Init( void )
{
    
    /******************************************************************************
 * Function:      void DMA0Init(void)
 *
 * PreCondition:  None
 *
 * Input:         None
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:      DMA0 initialization/configuration function.
 *                Direction: Read from RAM and write to the C1TXD register
 *                AMODE: Register indirect with post increment
 *                MODE: Continuous, Ping-Pong Mode
 *                IRQ: ECAN1 Transmit Interrupt
 *****************************************************************************/
    DMA0CON = 0x2020;
    DMA0PAD = ( int ) &C1TXD;   /* ECAN 1 (C1TXD) */
    DMA0CNT = 0x0007;
    DMA0REQ = 0x0046;           /* ECAN 1 Transmit */

    #ifdef _HAS_DMA_
    DMA0STAL = __builtin_dmaoffset( ecan1msgBuf );
    DMA0STAH = __builtin_dmapage( ecan1msgBuf );
    #else
    DMA0STAL = (uint16_t)(int_least24_t)(&ecan1msgBuf);
    DMA0STAH = 0;
    #endif
    DMA0CONbits.CHEN = 1;
}

void DMA2Init( void )
{
   /******************************************************************************
 * Function:      void DMA2Init(void)
 *
 * PreCondition:  None
 *
 * Input:         None
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:      DMA0 initialization/configuration function.
 *                Direction: Read from RAM and write to the C1RXD register
 *                AMODE: Register indirect with post increment
 *                MODE: Continuous, Ping-Pong Mode
 *                IRQ: ECAN1 Transmit Interrupt
 *****************************************************************************/
    
    DMA2CON = 0x0020;
    DMA2PAD = ( int ) &C1RXD;   /* ECAN 1 (C1RXD) */
    DMA2CNT = 0x0007;
    DMA2REQ = 0x0022;           /* ECAN 1 Receive */

    #ifdef _HAS_DMA_
    DMA2STAL = __builtin_dmaoffset( ecan1msgBuf );
    DMA2STAH = __builtin_dmapage( ecan1msgBuf );
    #else
    DMA2STAL = (uint16_t)(int_least24_t)(&ecan1msgBuf);
    DMA2STAH = 0;
    #endif
    DMA2CONbits.CHEN = 1;
}

void Ecan1ClkInit( void )
{
    /******************************************************************************
 * Function:      void Ecan1ClkInit(void)
 *
 * PreCondition:  None
 *
 * Input:         None
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:      ECAN1 clock initialization function
 *                This function is used to configure the clock used for the
 *                ECAN1 module during transmission/reception.
 *****************************************************************************/
    
    /* FCAN is selected to be FCY
   FCAN = FCY = 40MHz */
    //    C1CTRL1bits.CANCKS = 0x1;
    /*
Bit Time = (Sync Segment + Propagation Delay + Phase Segment 1 + Phase Segment 2)=20*TQ
Phase Segment 1 = 8TQ
Phase Segment 2 = 6Tq
Propagation Delay = 5Tq
Sync Segment = 1TQ
CiCFG1<BRP> =(FCAN /(2 ×N×FBAUD))– 1
*/
    /* Synchronization Jump Width set to 4 TQ */
    C1CFG1bits.SJW = 0;

    /* Baud Rate Prescaler */
    C1CFG1bits.BRP = 0;

    /* Phase Segment 1 time is 8 TQ */
    C1CFG2bits.SEG1PH = 4;

    /* Phase Segment 2 time is set to be programmable */
    C1CFG2bits.SEG2PHTS = 1;

    /* Phase Segment 2 time is 6 TQ */
    C1CFG2bits.SEG2PH = 1;

    /* Propagation Segment time is 5 TQ */
    C1CFG2bits.PRSEG = 1;

    /* Bus line is sampled three times at the sample point */
    C1CFG2bits.SAM = 0;
}

void Ecan1Init( void )
{
    /******************************************************************************
 * Function:     void Ecan1Init(void)
 *
 * PreCondition:  None
 *
 * Input:         None
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:      ECAN1 initialization function.This function is used to
 *                initialize the ECAN1 module by configuring the message
 *                buffers, and the acceptance filters and
 *                setting appropriate masks for the same.
 *****************************************************************************/
    
    /* Request Configuration Mode */
    C1CTRL1bits.REQOP = 4;
    while( C1CTRL1bits.OPMODE != 4 );

    Ecan1ClkInit();

    C1FCTRLbits.FSA = 0b01000;     /* FIFO Starts at Message Buffer 8 */
    C1FCTRLbits.DMABS = 0b110;     /* 32 CAN Message Buffers in DMA RAM */

    /*	Filter Configuration

    Ecan1WriteRxAcptFilter(int n, long identifier, unsigned int exide,unsigned int bufPnt,unsigned int maskSel)

    n = 0 to 15 -> Filter number

    identifier -> SID <10:0> : EID <17:0> 

    exide = 0 -> Match messages with standard identifier addresses 
    exide = 1 -> Match messages with extended identifier addresses 

    bufPnt = 0 to 14  -> RX Buffer 0 to 14
    bufPnt = 15 -> RX FIFO Buffer

    maskSel = 0    ->    Acceptance Mask 0 register contains mask
    maskSel = 1    ->    Acceptance Mask 1 register contains mask
    maskSel = 2    ->    Acceptance Mask 2 register contains mask
    maskSel = 3    ->    No Mask Selection
    
*/
    can1_writefilter( 1, 0x1FFEFFFF, 1, 15, 0 );

    /*    Mask Configuration

    Ecan1WriteRxAcptMask(int m, long identifierMask, unsigned int mide, unsigned int exide)

    m = 0 to 2 -> Mask Number

    identifier -> SID <10:0> : EID <17:0> 

    mide = 0 -> Match either standard or extended address message if filters match 
    mide = 1 -> Match only message types that correpond to 'exide' bit in filter

    exide = 0 -> Match messages with standard identifier addresses 
    exide = 1 -> Match messages with extended identifier addresses
    
*/
    can1_writemask( 1, 0x1FFFFFFF, 1, 1 );
    
    //Prueba de mask (en principio solo deja pasar el sincronismo)
    can1_writefilter (1,0x80,0,0xF,1);
    can1_writemask(1,0x700, 1,0); 
    
    /* Enter Normal Mode */
    C1CTRL1bits.REQOP = 0;
    while( C1CTRL1bits.OPMODE != 0 );

    /* ECAN transmit/receive message control */
    C1RXFUL1 = C1RXFUL2 = C1RXOVF1 = C1RXOVF2 = 0x0000;
    C1TR01CONbits.TXEN0 = 1;        /* ECAN1, Buffer 0 is a Transmit Buffer */
    C1TR01CONbits.TXEN1 = 1;        /* ECAN1, Buffer 0 is a Transmit Buffer */
    C1TR23CONbits.TXEN2 = 1;        /* ECAN1, Buffer 0 is a Transmit Buffer */
    C1TR23CONbits.TXEN3 = 1;        /* ECAN1, Buffer 0 is a Transmit Buffer */
    C1TR45CONbits.TXEN4 = 1;        /* ECAN1, Buffer 0 is a Transmit Buffer */
    C1TR45CONbits.TXEN5 = 1;        /* ECAN1, Buffer 0 is a Transmit Buffer */
    
    C1TR01CONbits.TX0PRI = 0b11;   /* Message Buffer 0 Priority Level */
    C1TR01CONbits.TX1PRI = 0b11;   /* Message Buffer 0 Priority Level */
    C1TR23CONbits.TX2PRI = 0b11;   /* Message Buffer 0 Priority Level */
    C1TR23CONbits.TX3PRI = 0b11;   /* Message Buffer 0 Priority Level */
    C1TR45CONbits.TX4PRI = 0b11;   /* Message Buffer 0 Priority Level */
    C1TR45CONbits.TX5PRI = 0b11;   /* Message Buffer 0 Priority Level */

    /* Setup I/O pins */
    // The PPS configuration varies from device to device. Refer the datasheet of the device being used and
    // use the appropriate values for the RPINR/RPOR registers.
    RPINR26 = 0;                    //clear register
    
    //--------------------------------------------------------------------------
    //------------------SELECCIÓN DE PUERTOS DE CAN1----------------------------
    //--------------------------------------------------------------------------   
    
    //Correctos
    RPINR26bits.C1RXR = 52;         //set CAN1 RX to RP52    
    RPOR1bits.RP37R = 14;           //set CAN1TX to RP37
    
    DMA0Init();                 //Iniciar DMAs de CAN1
    DMA2Init();
    
    C1INTEbits.TBIE = 1;        // Enable Interrupción de CAN por transmisión
    C1INTEbits.RBIE = 1;        // Enable Interrupción de CAN por recepción
    
}

//*****************************************************************************
//                          ECAN2 CONFIGURATION
//*****************************************************************************

void DMA1Init( void )
{
    /******************************************************************************
 * Function:      void DMA1Init(void)
 *
 * PreCondition:  None
 *
 * Input:         None
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:      DMA1 initialization/configuration function.
 *                Direction: Read from RAM and write to the C1TXD register
 *                AMODE: Register indirect with post increment
 *                MODE: Continuous, Ping-Pong Mode
 *                IRQ: ECAN2 Transmit Interrupt
 *****************************************************************************/
    
    DMA1CON = 0x2020;
    DMA1PAD = ( int ) &C2TXD;   /* ECAN 2 (C2TXD) */
    DMA1CNT = 0x0007;
    DMA1REQ = 0x0047;           /* ECAN 2 Transmit */

    #ifdef _HAS_DMA_
    DMA1STAL = __builtin_dmaoffset( ecan2msgBuf );
    DMA1STAH = __builtin_dmapage( ecan2msgBuf );
    #else
    DMA1STAL = (uint16_t)(int_least24_t)(&ecan2msgBuf);
    DMA1STAH = 0;
    #endif
    DMA1CONbits.CHEN = 1;
}

void DMA3Init( void )
{
   /******************************************************************************
 * Function:      void DMA3Init(void)
 *
 * PreCondition:  None
 *
 * Input:         None
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:      DMA3 initialization/configuration function.
 *                Direction: Read from RAM and write to the C1RXD register
 *                AMODE: Register indirect with post increment
 *                MODE: Continuous, Ping-Pong Mode
 *                IRQ: ECAN2 Transmit Interrupt
 *****************************************************************************/
    
    DMA3CON = 0x0020;
    DMA3PAD = ( int ) &C2RXD;   /* ECAN 2 (C2RXD) */
    DMA3CNT = 0x0007;
    DMA3REQ = 0x0037;           /* ECAN 2 Receive */

    #ifdef _HAS_DMA_
    DMA3STAL = __builtin_dmaoffset( ecan2msgBuf );
    DMA3STAH = __builtin_dmapage( ecan2msgBuf );
    #else
    DMA3STAL = (uint16_t) (int_least24_t)(&ecan2msgBuf );
    DMA3STAH = 0;
    #endif
    DMA3CONbits.CHEN = 1;
}

void Ecan2ClkInit( void )
{
    /******************************************************************************
 * Function:      void Ecan2ClkInit(void)
 *
 * PreCondition:  None
 *
 * Input:         None
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:      ECAN2 clock initialization function
 *                This function is used to configure the clock used for the
 *                ECAN2 module during transmission/reception.
 *****************************************************************************/
    
    /* FCAN is selected to be FCY*/
    // FCAN = FCY = 40MHz
    //	C2CTRL1bits.CANCKS = 0x1;
    /*
Bit Time = (Sync Segment + Propagation Delay + Phase Segment 1 + Phase Segment 2)=20*TQ
Phase Segment 1 = 8TQ
Phase Segment 2 = 6Tq
Propagation Delay = 5Tq
Sync Segment = 1TQ
CiCFG1<BRP> =(FCAN /(2 ×N×FBAUD))– 1
*/
    /* Synchronization Jump Width set to 4 TQ */
    C2CFG1bits.SJW = 0;

    /* Baud Rate Prescaler */
    C2CFG1bits.BRP = 0;

    /* Phase Segment 1 time is 8 TQ */
    C2CFG2bits.SEG1PH = 4;

    /* Phase Segment 2 time is set to be programmable */
    C2CFG2bits.SEG2PHTS = 1;

    /* Phase Segment 2 time is 6 TQ */
    C2CFG2bits.SEG2PH = 1;

    /* Propagation Segment time is 5 TQ */
    C2CFG2bits.PRSEG = 1;

    /* Bus line is sampled three times at the sample point */
    C2CFG2bits.SAM = 0;
}

void Ecan2Init( void )
{
    /******************************************************************************
 * Function:     void Ecan2Init(void)
 *
 * PreCondition:  None
 *
 * Input:         None
 *
 * Output:        None
 *
 * Side Effects:  None
 *
 * Overview:      ECAN2 initialization function.This function is used to
 *                initialize the ECAN1 module by configuring the message
 *                buffers, and the acceptance filters and
 *                setting appropriate masks for the same.
 *****************************************************************************/
    
    
    /* Request Configuration Mode */
    C2CTRL1bits.REQOP = 4;
    while( C2CTRL1bits.OPMODE != 4 );

    Ecan2ClkInit();

    C2FCTRLbits.FSA = 0b01000;     /* FIFO Starts at Message Buffer 16 */
    C2FCTRLbits.DMABS = 0b110;     /* 32 CAN Message Buffers in DMA RAM */

    /*	Filter Configuration

	Ecan2WriteRxAcptFilter(int n, long identifier, unsigned int exide,unsigned int bufPnt,unsigned int maskSel)

	n = 0 to 15 -> Filter number

	identifier -> SID <10:0> : EID <17:0> 

	exide = 0 -> Match messages with standard identifier addresses 
	exide = 1 -> Match messages with extended identifier addresses 

	bufPnt = 0 to 14  -> RX Buffer 0 to 14
	bufPnt = 15 -> RX FIFO Buffer

	maskSel = 0	->	Acceptance Mask 0 register contains mask
	maskSel = 1	->	Acceptance Mask 1 register contains mask
	maskSel = 2	->	Acceptance Mask 2 register contains mask
	maskSel = 3	->	No Mask Selection
	
*/
    can2_writefilter( 1, 0x1FFEFFFF, 1, 15, 0 );

    /*	Mask Configuration

	Ecan2WriteRxAcptMask(int m, long identifierMask, unsigned int mide, unsigned int exide)

	m = 0 to 2 -> Mask Number

	identifier -> SID <10:0> : EID <17:0> 

	mide = 0 -> Match either standard or extended address message if filters match 
	mide = 1 -> Match only message types that correpond to 'exide' bit in filter

	exide = 0 -> Match messages with standard identifier addresses 
	exide = 1 -> Match messages with extended identifier addresses
	
*/
    can2_writemask( 1, 0x1FFFFFFF, 1, 1 );

    /* Enter Normal Mode */
    C2CTRL1bits.REQOP = 0;
    while( C2CTRL1bits.OPMODE != 0 );

    /* ECAN transmit/receive message control */
    C2RXFUL1 = C2RXFUL2 = C2RXOVF1 = C2RXOVF2 = 0x0000;
    C2TR01CONbits.TXEN0 = 1;        /* ECAN2, Buffer 0 is a Transmit Buffer */
    C2TR01CONbits.TXEN1 = 1;        /* ECAN2, Buffer 0 is a Transmit Buffer */
    C2TR23CONbits.TXEN2 = 1;        /* ECAN2, Buffer 0 is a Transmit Buffer */
    C2TR23CONbits.TXEN3 = 1;        /* ECAN2, Buffer 0 is a Transmit Buffer */
    C2TR45CONbits.TXEN4 = 1;        /* ECAN2, Buffer 0 is a Transmit Buffer */
    C2TR45CONbits.TXEN5 = 1;        /* ECAN2, Buffer 0 is a Transmit Buffer */

    C2TR01CONbits.TX0PRI = 0b11;   /* Message Buffer 0 Priority Level */
    C2TR01CONbits.TX1PRI = 0b11;   /* Message Buffer 1 Priority Level */
    C2TR23CONbits.TX2PRI = 0b11;   /* Message Buffer 0 Priority Level */
    C2TR23CONbits.TX3PRI = 0b11;   /* Message Buffer 1 Priority Level */
    C2TR45CONbits.TX4PRI = 0b11;   /* Message Buffer 0 Priority Level */
    C2TR45CONbits.TX5PRI = 0b11;   /* Message Buffer 1 Priority Level */

    /* Setup I/O pins */
    // The PPS configuration varies from device to device. Refer the datasheet of the device being used and
    // use the appropriate values for the RPINR/RPOR registers.
    
    //--------------------------------------------------------------------------
    //------------------SELECCIÓN DE PUERTOS DE CAN2----------------------------
    //--------------------------------------------------------------------------
    
    
    //Correcto
    RPINR26bits.C2RXR = 48;         //set CAN2 RX to RP40
    RPOR5bits.RP49R = 15;           //set CAN2TX to RP39	
    
    DMA1Init();                     //Iniciar DMAs de CAN2
    DMA3Init();

    C2INTEbits.TBIE = 1;        // Enable Interrupción de CAN por transmisión
    C2INTEbits.RBIE = 1;        // Enable Interrupción de CAN por recepción
}



