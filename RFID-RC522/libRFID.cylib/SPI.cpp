/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "SPI.h"
#include <assert.h>

SPIClass SPI ;

SPIClass::SPIClass()
{
}

int SPIClass::init(int scbinst)
{
    m_scbinst = scbinst ;
    
    //
    // TODO - initialize the given SCB block as SPI
    //
    
    assert(0) ;
    return 0 ;
}

void SPIClass::beginTransaction(const SPISettings &settings)
{
    //
    // TODO - initialize the SCB block to the given settings for
    //        mode and MSB vs LSB first.  The clock will have to be
    //        faked.  You will need to understand the frequency you
    //        get with the various clock dividers on a Arduino, and
    //        the write the code to get the same.
    //
    assert(0) ;
}

uint8_t SPIClass::transfer(uint8_t data)
{
    //
    // TODO - write the code to transfer out a single byte and receive
    //        a single byte at the same time
    //
    assert(0) ;
    return 0 ;
}

void SPIClass::endTransaction()
{
    //
    // TODO - end the current transaction
    //
    assert(0) ;
}

/* [] END OF FILE */
