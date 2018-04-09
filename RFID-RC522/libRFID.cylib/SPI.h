#pragma once

#include <stdint.h>
#include "scb/cy_scb_common.h"

#ifndef LSBFIRST
#define LSBFIRST 0
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

class SPISettings
{
public:
    SPISettings(uint8_t clock, uint8_t order, uint8_t mode)
    {
        m_clock = clock ;
        m_order = order ;
        m_mode = mode ;
    }
    
    uint8_t getClock() const
    {
        return m_clock ;
    }
    
    uint8_t getOrder() const
    {
        return m_order ;
    }
    
    uint8_t getMode() const
    {
        return m_mode ;
    }

private:
    uint8_t m_clock ;
    uint8_t m_order ;
    uint8_t m_mode ;
} ;

class SPIClass
{
public:
    SPIClass() ;
    int init(CySCB_Type * scbinst) ;
    void beginTransaction(const SPISettings &settings) ;
    uint8_t transfer(uint8_t data) ;
    void endTransaction() ;
    
private:
    CySCB_Type *m_scbinst;
} ;

extern SPIClass SPI ;
