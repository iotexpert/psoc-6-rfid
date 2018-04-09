#include "SPI.h"
#include <assert.h>
#include <stdio.h>

#include "scb/cy_scb_common.h"
#include "scb/cy_scb_spi.h"

SPIClass SPI ;



SPIClass::SPIClass()
{
    printf("Created SPI Class this=%X\r\n",this);
}

int SPIClass::init(CySCB_Type * scbinst)
{
    m_scbinst = scbinst ;
    
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
    //assert(0) ;
    
    // For now assume that SPI port is configured
    
}

uint8_t SPIClass::transfer(uint8_t data)
{
    uint8_t rval;
    
    Cy_SCB_SPI_Write(m_scbinst,data);
    
    while(Cy_SCB_SPI_GetNumInRxFifo(m_scbinst) == 0); // busy wait until there is a byte
    
    rval = Cy_SCB_SPI_Read(m_scbinst);
    
//    printf("Wrote=0x%X Read=%X\r\n",data,rval);
    return rval;

}

void SPIClass::endTransaction()
{
    //
    // TODO - end the current transaction
    //
    //assert(0) ;
    // for now assume that you dont need to do anything.
    
}

/* [] END OF FILE */
