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

#include "Arduino.h"
#include "MFRC522.h"
#include "MFRC522Interface.h"
#include <assert.h>

//
// Returned value to indicate an RFID device cannot be created
//
RFIDHandle BadRFIDHandle = 0 ;

#ifdef MULTIPLE_RFID_DEVICES

//
// The max number of RFID devices supported
//
#define CYMAX_RFID_DEVICES          (4)

struct SpiRFIDPair
{
    int used ;
    SPIClass *spi_p ;
    MFRC522 *rfid_p ;
} ;

SpiRFIDPair pairs[4] ;

//
// If zero, the module has not been initialized, initialize it
//
static int inited = 0 ;

void initModule()
{
    for(int i = 0 ; i < sizeof(pairs)/sizeof(pairs[0]) ; i++)
    {
        pairs[i].used = 0 ;
    }
}

int findPair()
{
    for(int i = 0 ; i < sizeof(pairs) / sizeof(pairs[0]) ; i++)
    {
        if (paris[i].used == 0)
            return i ;
    }
    
    return -1 ;
}
#else
//
// The RFID hardware block
//
MFRC522 rfiddev ;
#endif


RFIDHandle createRFID(CySCB_Type * spi, int chipsel, int reset)
{
    RFIDHandle h = BadRFIDHandle ;
    
#ifdef MULTIPLE_RFID_DEVICES
    //
    // Note, this is not thread safe
    //

    int pair ;
    if (!inited)
        initModule() ;
        
    pair = findPair() ;
    if (pair == -1)
        return BadRFIDHandle ;
    
    pairs[pair].spi_p = new SPI() ;
    if (pairs[pair].spi_p == nullptr)
    {
        return BadRFIDHandle ;
    }
    
    pairs[pair].rfid_p = new MFRC522() ;
    if (pairs[pair].rfid_p == nullptr)
    {
        delete pairs[pair].spi_p ;
        return BadRFIDHandle ;
    }
    
    if (pairs[pair].spi_p->init(spi))
    {
        //
        // The SPI failed to initialize
        //
        delete pairs[pair].spi_p ;
        delete pairs[pair].rfid_p ;
        return BadRFIDHandle ;
    }
    
    pairs[pair].rfid_p->PCD_Init(chipsel, reset) ;   
    pairs[pair].used = 1 ;    
    
    h = reinterpret_cast<RFIDHandle>(pairs[pair].rfid_p) ;
    
#else
    //
    // Initialize the SPI module, the SPI class should initialize
    // an SCB based SPI block to talk to the RFID hardware
    //
    if (SPI.init(spi))
        return BadRFIDHandle ;
        
    rfiddev.PCD_Init(chipsel, reset,&SPI) ;
    
    h = reinterpret_cast<RFIDHandle>(&rfiddev) ;
    
#endif

    return h ;
}

void destroyRFID(RFIDHandle h)
{
#ifdef MULTIPLE_RFID_DEVICES
    int pair = -1 ;
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    
    for(int i = 0 ; i < sizeof(pairs)/sizeof(pairs[0]) ; i++)
    {
        if (pairs[i].rfid_p == dev_p)
        {
            pair = i ;
            break ;
        }
    }
    //
    // If i is negative one, this means we were given a handle not created
    // by the createRFID function above
    //
    assert(i != -1) ;
    
    delete pairs[i].spi_p ;
    delete pairs[i].rfid_p ;
    pairs[i].used = 0 ;
#endif
}


/////////////////////////////////////////////////////////////////////////////////////
// Basic interface functions for communicating with the MFRC522
/////////////////////////////////////////////////////////////////////////////////////
void PCD_WriteRegister(RFIDHandle h, PCD_Register reg, byte value)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    dev_p->PCD_WriteRegister(static_cast<MFRC522::PCD_Register>(reg), value) ;
}

void PCD_WriteRegisterMult(RFIDHandle h, PCD_Register reg, byte count, byte *values)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    dev_p->PCD_WriteRegister(static_cast<MFRC522::PCD_Register>(reg), count, values) ;
}

byte PCD_ReadRegister(RFIDHandle h, PCD_Register reg)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return dev_p->PCD_ReadRegister(static_cast<MFRC522::PCD_Register>(reg)) ;
}

void PCD_ReadRegisterMult(RFIDHandle h, PCD_Register reg, byte count, byte *values, byte rxAlign)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return dev_p->PCD_ReadRegister(static_cast<MFRC522::PCD_Register>(reg), count, values, rxAlign) ;
}

void PCD_SetRegisterBitMask(RFIDHandle h, PCD_Register reg, byte mask)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    dev_p->PCD_SetRegisterBitMask(static_cast<MFRC522::PCD_Register>(reg), mask) ;
}

void PCD_ClearRegisterBitMask(RFIDHandle h, PCD_Register reg, byte mask)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    dev_p->PCD_ClearRegisterBitMask(static_cast<MFRC522::PCD_Register>(reg), mask) ;
}

StatusCode PCD_CalculateCRC(RFIDHandle h, byte *data, byte length, byte *result)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)(dev_p->PCD_CalculateCRC(data, length, result)) ;
}
	
/////////////////////////////////////////////////////////////////////////////////////
// Functions for manipulating the MFRC522
/////////////////////////////////////////////////////////////////////////////////////
void PCD_Init(RFIDHandle h)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    dev_p->PCD_Init() ;
}

void PCD_InitWithPins(RFIDHandle h, byte chipSelectPin, byte resetPowerDownPin)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    dev_p->PCD_Init(chipSelectPin, resetPowerDownPin) ;
}

void PCD_Reset(RFIDHandle h)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    dev_p->PCD_Reset() ;
}

void PCD_AntennaOn(RFIDHandle h)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    dev_p->PCD_AntennaOn() ;
}

void PCD_AntennaOff(RFIDHandle h)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    dev_p->PCD_AntennaOff() ;
}

byte PCD_GetAntennaGain(RFIDHandle h)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return dev_p->PCD_GetAntennaGain() ;
}

void PCD_SetAntennaGain(RFIDHandle h, byte mask)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    dev_p->PCD_SetAntennaGain(mask) ;
}

bool PCD_PerformSelfTest(RFIDHandle h)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return dev_p->PCD_PerformSelfTest() ;
}
	
/////////////////////////////////////////////////////////////////////////////////////
// Power control functions
/////////////////////////////////////////////////////////////////////////////////////
void PCD_SoftPowerDown(RFIDHandle h)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    dev_p->PCD_SoftPowerDown() ;
}

void PCD_SoftPowerUp(RFIDHandle h)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    dev_p->PCD_SoftPowerUp() ;
}
	
/////////////////////////////////////////////////////////////////////////////////////
// Functions for communicating with PICCs
/////////////////////////////////////////////////////////////////////////////////////
StatusCode PCD_TransceiveData(RFIDHandle h, byte *sendData, byte sendLen, byte *backData, byte *backLen, byte *validBits, byte rxAlign, bool checkCRC)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)(dev_p->PCD_TransceiveData(sendData, sendLen, backData, backLen, validBits, rxAlign, checkCRC)) ;
}

StatusCode PCD_CommunicateWithPICC(RFIDHandle h, byte command, byte waitIRq, byte *sendData, byte sendLen, byte *backData, 
                                    byte *backLen, byte *validBits, byte rxAlign, bool checkCRC)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)(dev_p->PCD_CommunicateWithPICC(command, waitIRq, sendData, sendLen, backData, backLen, validBits, rxAlign, checkCRC)) ;
}

StatusCode PICC_RequestA(RFIDHandle h, byte *bufferATQA, byte *bufferSize)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)(dev_p->PICC_RequestA(bufferATQA, bufferSize)) ;
}

StatusCode PICC_WakeupA(RFIDHandle h, byte *bufferATQA, byte *bufferSize)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)(dev_p->PICC_WakeupA(bufferATQA, bufferSize)) ;
}

StatusCode PICC_REQA_or_WUPA(RFIDHandle h, byte command, byte *bufferATQA, byte *bufferSize)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)(dev_p->PICC_REQA_or_WUPA(command, bufferATQA, bufferSize)) ;
}

StatusCode PICC_Select(RFIDHandle h, Uid *uid, byte validBits)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    MFRC522::Uid *uid_p = reinterpret_cast<MFRC522::Uid *>(uid) ;    
    return (StatusCode)(byte)(dev_p->PICC_Select(uid_p, validBits)) ;
}

StatusCode PICC_HaltA(RFIDHandle h)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)(dev_p->PICC_HaltA()) ;
}

/////////////////////////////////////////////////////////////////////////////////////
// Functions for communicating with MIFARE PICCs
/////////////////////////////////////////////////////////////////////////////////////
StatusCode PCD_Authenticate(RFIDHandle h, byte command, byte blockAddr, MIFARE_Key *key, Uid *uid)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    MFRC522::MIFARE_Key *key_p = reinterpret_cast<MFRC522::MIFARE_Key *>(key) ;
    MFRC522::Uid *uid_p = reinterpret_cast<MFRC522::Uid *>(uid) ;
    return (StatusCode)(byte)(dev_p->PCD_Authenticate(command, blockAddr, key_p, uid_p)) ;
}

void PCD_StopCrypto1(RFIDHandle h)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    dev_p->PCD_StopCrypto1() ;
}

StatusCode MIFARE_Read(RFIDHandle h, byte blockAddr, byte *buffer, byte *bufferSize)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)(dev_p->MIFARE_Read(blockAddr, buffer, bufferSize)) ;
}

StatusCode MIFARE_Write(RFIDHandle h, byte blockAddr, byte *buffer, byte bufferSize)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)(dev_p->MIFARE_Write(blockAddr, buffer, bufferSize)) ;
}

StatusCode MIFARE_Ultralight_Write(RFIDHandle h, byte page, byte *buffer, byte bufferSize)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)(dev_p->MIFARE_Ultralight_Write(page, buffer, bufferSize)) ;
}

StatusCode MIFARE_Decrement(RFIDHandle h, byte blockAddr, int32_t delta)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)(dev_p->MIFARE_Decrement(blockAddr, delta)) ;
}

StatusCode MIFARE_Increment(RFIDHandle h, byte blockAddr, int32_t delta)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)(dev_p->MIFARE_Increment(blockAddr, delta)) ;
}

StatusCode MIFARE_Restore(RFIDHandle h, byte blockAddr)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)(dev_p->MIFARE_Restore(blockAddr)) ;
}

StatusCode MIFARE_Transfer(RFIDHandle h, byte blockAddr)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)(dev_p->MIFARE_Transfer(blockAddr)) ;
}

StatusCode MIFARE_GetValue(RFIDHandle h, byte blockAddr, int32_t *value)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)(dev_p->MIFARE_GetValue(blockAddr, value)) ;
}

StatusCode MIFARE_SetValue(RFIDHandle h, byte blockAddr, int32_t value)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)(dev_p->MIFARE_SetValue(blockAddr, value)) ;
}

StatusCode PCD_NTAG216_AUTH(RFIDHandle h, byte *passWord, byte pACK[])
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)(dev_p->PCD_NTAG216_AUTH(passWord, pACK)) ;
}

	
/////////////////////////////////////////////////////////////////////////////////////
// Support functions
/////////////////////////////////////////////////////////////////////////////////////
StatusCode PCD_MIFARE_Transceive(RFIDHandle h, byte *sendData, byte sendLen, bool acceptTimeout)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return (StatusCode)(byte)dev_p->PCD_MIFARE_Transceive(sendData, sendLen, acceptTimeout)  ;
}

PICC_Type PICC_GetType(byte sak)
{
    return (PICC_Type)(byte)MFRC522::PICC_GetType(sak) ;
}

// Support functions for debuging - proxy for MFRC522Debug to keep backwarts compatibility
const __FlashStringHelper *GetStatusCodeName(StatusCode code)
{
    return MFRC522::GetStatusCodeName((MFRC522::StatusCode)code) ;
}

const __FlashStringHelper *PICC_GetTypeName(PICC_Type type)
{
    return MFRC522::PICC_GetTypeName(static_cast<MFRC522::PICC_Type>(type)) ;
}
	
// Support functions for debuging
void PCD_DumpVersionToSerial(RFIDHandle h)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    dev_p->PCD_DumpVersionToSerial() ;
}

void PICC_DumpToSerial(RFIDHandle h, Uid *uid)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    MFRC522::Uid *uid_p = reinterpret_cast<MFRC522::Uid *>(uid) ;
    dev_p->PICC_DumpToSerial(uid_p) ;
}

void PICC_DumpDetailsToSerial(RFIDHandle h, Uid *uid)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    MFRC522::Uid *uid_p = reinterpret_cast<MFRC522::Uid *>(uid) ;
    dev_p->PICC_DumpDetailsToSerial(uid_p) ;
}

void PICC_DumpMifareClassicToSerial(RFIDHandle h, Uid *uid, PICC_Type piccType, MIFARE_Key *key)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    MFRC522::Uid *uid_p = reinterpret_cast<MFRC522::Uid *>(uid) ;
    MFRC522::MIFARE_Key *key_p = reinterpret_cast<MFRC522::MIFARE_Key *>(key) ;
    dev_p->PICC_DumpMifareClassicToSerial(uid_p, (MFRC522::PICC_Type)piccType, key_p) ;
}

void PICC_DumpMifareClassicSectorToSerial(RFIDHandle h, Uid *uid, MIFARE_Key *key, byte sector)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    MFRC522::Uid *uid_p = reinterpret_cast<MFRC522::Uid *>(uid) ;
    MFRC522::MIFARE_Key *key_p = reinterpret_cast<MFRC522::MIFARE_Key *>(key) ;
    dev_p->PICC_DumpMifareClassicSectorToSerial(uid_p, key_p, sector) ;
}

void PICC_DumpMifareUltralightToSerial(RFIDHandle h)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    dev_p->PICC_DumpMifareUltralightToSerial() ;
}
	
// Advanced functions for MIFARE
void MIFARE_SetAccessBits(RFIDHandle h, byte *accessBitBuffer, byte g0, byte g1, byte g2, byte g3)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    dev_p->MIFARE_SetAccessBits(accessBitBuffer, g0, g1, g2 ,g3) ;
}

	
/////////////////////////////////////////////////////////////////////////////////////
// Convenience functions - does not add extra functionality
/////////////////////////////////////////////////////////////////////////////////////
bool PICC_IsNewCardPresent(RFIDHandle h)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return dev_p->PICC_IsNewCardPresent() ;
}

bool PICC_ReadCardSerial(RFIDHandle h)
{
    MFRC522 *dev_p = reinterpret_cast<MFRC522 *>(h) ;
    return dev_p->PICC_ReadCardSerial() ;
}

/* [] END OF FILE */
