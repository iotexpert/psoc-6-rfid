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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <gpio/cy_gpio.h>

typedef uint8_t byte ;
typedef const char *__FlashStringHelper ;

#define RFID_MF_KEY_SIZE             (6)
#define RFID_MF_ACK                  (0x0A)

/// @brief A handle to an RFID device
typedef uint32_t RFIDHandle ;

/// @brief Value returned to indicate an error initializing the RFID block
extern RFIDHandle BadRFIDHandle ;

typedef enum {
		// Page 0: Command and status
		//						  0x00			// reserved for future use
		CommandReg				= 0x01 << 1,	// starts and stops command execution
		ComIEnReg				= 0x02 << 1,	// enable and disable interrupt request control bits
		DivIEnReg				= 0x03 << 1,	// enable and disable interrupt request control bits
		ComIrqReg				= 0x04 << 1,	// interrupt request bits
		DivIrqReg				= 0x05 << 1,	// interrupt request bits
		ErrorReg				= 0x06 << 1,	// error bits showing the error status of the last command executed 
		Status1Reg				= 0x07 << 1,	// communication status bits
		Status2Reg				= 0x08 << 1,	// receiver and transmitter status bits
		FIFODataReg				= 0x09 << 1,	// input and output of 64 byte FIFO buffer
		FIFOLevelReg			= 0x0A << 1,	// number of bytes stored in the FIFO buffer
		WaterLevelReg			= 0x0B << 1,	// level for FIFO underflow and overflow warning
		ControlReg				= 0x0C << 1,	// miscellaneous control registers
		BitFramingReg			= 0x0D << 1,	// adjustments for bit-oriented frames
		CollReg					= 0x0E << 1,	// bit position of the first bit-collision detected on the RF interface
		//						  0x0F			// reserved for future use
		
		// Page 1: Command
		// 						  0x10			// reserved for future use
		ModeReg					= 0x11 << 1,	// defines general modes for transmitting and receiving 
		TxModeReg				= 0x12 << 1,	// defines transmission data rate and framing
		RxModeReg				= 0x13 << 1,	// defines reception data rate and framing
		TxControlReg			= 0x14 << 1,	// controls the logical behavior of the antenna driver pins TX1 and TX2
		TxASKReg				= 0x15 << 1,	// controls the setting of the transmission modulation
		TxSelReg				= 0x16 << 1,	// selects the internal sources for the antenna driver
		RxSelReg				= 0x17 << 1,	// selects internal receiver settings
		RxThresholdReg			= 0x18 << 1,	// selects thresholds for the bit decoder
		DemodReg				= 0x19 << 1,	// defines demodulator settings
		// 						  0x1A			// reserved for future use
		// 						  0x1B			// reserved for future use
		MfTxReg					= 0x1C << 1,	// controls some MIFARE communication transmit parameters
		MfRxReg					= 0x1D << 1,	// controls some MIFARE communication receive parameters
		// 						  0x1E			// reserved for future use
		SerialSpeedReg			= 0x1F << 1,	// selects the speed of the serial UART interface
		
		// Page 2: Configuration
		// 						  0x20			// reserved for future use
		CRCResultRegH			= 0x21 << 1,	// shows the MSB and LSB values of the CRC calculation
		CRCResultRegL			= 0x22 << 1,
		// 						  0x23			// reserved for future use
		ModWidthReg				= 0x24 << 1,	// controls the ModWidth setting?
		// 						  0x25			// reserved for future use
		RFCfgReg				= 0x26 << 1,	// configures the receiver gain
		GsNReg					= 0x27 << 1,	// selects the conductance of the antenna driver pins TX1 and TX2 for modulation 
		CWGsPReg				= 0x28 << 1,	// defines the conductance of the p-driver output during periods of no modulation
		ModGsPReg				= 0x29 << 1,	// defines the conductance of the p-driver output during periods of modulation
		TModeReg				= 0x2A << 1,	// defines settings for the internal timer
		TPrescalerReg			= 0x2B << 1,	// the lower 8 bits of the TPrescaler value. The 4 high bits are in TModeReg.
		TReloadRegH				= 0x2C << 1,	// defines the 16-bit timer reload value
		TReloadRegL				= 0x2D << 1,
		TCounterValueRegH		= 0x2E << 1,	// shows the 16-bit timer value
		TCounterValueRegL		= 0x2F << 1,
		
		// Page 3: Test Registers
		// 						  0x30			// reserved for future use
		TestSel1Reg				= 0x31 << 1,	// general test signal configuration
		TestSel2Reg				= 0x32 << 1,	// general test signal configuration
		TestPinEnReg			= 0x33 << 1,	// enables pin output driver on pins D1 to D7
		TestPinValueReg			= 0x34 << 1,	// defines the values for D1 to D7 when it is used as an I/O bus
		TestBusReg				= 0x35 << 1,	// shows the status of the internal test bus
		AutoTestReg				= 0x36 << 1,	// controls the digital self-test
		VersionReg				= 0x37 << 1,	// shows the software version
		AnalogTestReg			= 0x38 << 1,	// controls the pins AUX1 and AUX2
		TestDAC1Reg				= 0x39 << 1,	// defines the test value for TestDAC1
		TestDAC2Reg				= 0x3A << 1,	// defines the test value for TestDAC2
		TestADCReg				= 0x3B << 1		// shows the value of ADC I and Q channels
		// 						  0x3C			// reserved for production tests
		// 						  0x3D			// reserved for production tests
		// 						  0x3E			// reserved for production tests
		// 						  0x3F			// reserved for production tests
} PCD_Register ;

typedef enum {
	STATUS_OK				,	// Success
	STATUS_ERROR			,	// Error in communication
	STATUS_COLLISION		,	// Collission detected
	STATUS_TIMEOUT			,	// Timeout in communication.
	STATUS_NO_ROOM			,	// A buffer is not big enough.
	STATUS_INTERNAL_ERROR	,	// Internal error in the code. Should not happen ;-)
	STATUS_INVALID			,	// Invalid argument.
	STATUS_CRC_WRONG		,	// The CRC_A does not match
	STATUS_MIFARE_NACK		= 0xff	// A MIFARE PICC responded with NAK.
} StatusCode ;

typedef struct {
	byte		size;			// Number of bytes in the UID. 4, 7 or 10.
	byte		uidByte[10];
	byte		sak;			// The SAK (Select acknowledge) byte returned from the PICC after successful selection.
} Uid;

typedef struct {
	byte		keyByte[RFID_MF_KEY_SIZE];
} MIFARE_Key;

typedef enum {
	PICC_TYPE_UNKNOWN		,
	PICC_TYPE_ISO_14443_4	,	// PICC compliant with ISO/IEC 14443-4 
	PICC_TYPE_ISO_18092		, 	// PICC compliant with ISO/IEC 18092 (NFC)
	PICC_TYPE_MIFARE_MINI	,	// MIFARE Classic protocol, 320 bytes
	PICC_TYPE_MIFARE_1K		,	// MIFARE Classic protocol, 1KB
	PICC_TYPE_MIFARE_4K		,	// MIFARE Classic protocol, 4KB
	PICC_TYPE_MIFARE_UL		,	// MIFARE Ultralight or Ultralight C
	PICC_TYPE_MIFARE_PLUS	,	// MIFARE Plus
	PICC_TYPE_MIFARE_DESFIRE,	// MIFARE DESFire
	PICC_TYPE_TNP3XXX		,	// Only mentioned in NXP AN 10833 MIFARE Type Identification Procedure
	PICC_TYPE_NOT_COMPLETE	= 0xff	// SAK indicates UID is not complete.
} PICC_Type ;

/// @brief Create a software rfid module.
/// @param spi the index of the SCB block to use for the SPI connection to the RFID hardware
/// @param serial the index of the SCB block to use for serial communications
/// @param chipsel the chip select pin for the device
/// @param reset the reset pin for the device
/// @returns BadRFIDHandle if there is an error, otherwise an HANDLE to the RFID hardware
RFIDHandle createRFID(int spi, int serial, int chipsel, int reset) ;

/// @brief Destroy a software RFID module
/// @param h the handle to the module
void destroyRFID(RFIDHandle h) ;

/////////////////////////////////////////////////////////////////////////////////////
// Basic interface functions for communicating with the MFRC522
/////////////////////////////////////////////////////////////////////////////////////
void PCD_WriteRegister(RFIDHandle h, PCD_Register reg, byte value);
void PCD_WriteRegisterMult(RFIDHandle h, PCD_Register reg, byte count, byte *values);
byte PCD_ReadRegister(RFIDHandle h, PCD_Register reg);
void PCD_ReadRegisterMult(RFIDHandle h, PCD_Register reg, byte count, byte *values, byte rxAlign);
void PCD_SetRegisterBitMask(RFIDHandle h, PCD_Register reg, byte mask);
void PCD_ClearRegisterBitMask(RFIDHandle h, PCD_Register reg, byte mask);
StatusCode PCD_CalculateCRC(RFIDHandle h, byte *data, byte length, byte *result);
	
/////////////////////////////////////////////////////////////////////////////////////
// Functions for manipulating the MFRC522
/////////////////////////////////////////////////////////////////////////////////////
void PCD_Init(RFIDHandle h);
void PCD_InitWithPins(RFIDHandle h, byte chipSelectPin, byte resetPowerDownPin);
void PCD_Reset(RFIDHandle h);
void PCD_AntennaOn(RFIDHandle h);
void PCD_AntennaOff(RFIDHandle h);
byte PCD_GetAntennaGain(RFIDHandle h);
void PCD_SetAntennaGain(RFIDHandle h, byte mask);
bool PCD_PerformSelfTest(RFIDHandle h);
	
/////////////////////////////////////////////////////////////////////////////////////
// Power control functions
/////////////////////////////////////////////////////////////////////////////////////
void PCD_SoftPowerDown(RFIDHandle h);
void PCD_SoftPowerUp(RFIDHandle h);
	
/////////////////////////////////////////////////////////////////////////////////////
// Functions for communicating with PICCs
/////////////////////////////////////////////////////////////////////////////////////
StatusCode PCD_TransceiveData(RFIDHandle h, byte *sendData, byte sendLen, byte *backData, byte *backLen, byte *validBits, byte rxAlign, bool checkCRC);
StatusCode PCD_CommunicateWithPICC(RFIDHandle h, byte command, byte waitIRq, byte *sendData, byte sendLen, byte *backData, byte *backLen, byte *validBits, byte rxAlign, bool checkCRC);
StatusCode PICC_RequestA(RFIDHandle h, byte *bufferATQA, byte *bufferSize);
StatusCode PICC_WakeupA(RFIDHandle h, byte *bufferATQA, byte *bufferSize);
StatusCode PICC_REQA_or_WUPA(RFIDHandle h, byte command, byte *bufferATQA, byte *bufferSize);
StatusCode PICC_Select(RFIDHandle h, Uid *uid, byte validBits);
StatusCode PICC_HaltA(RFIDHandle h);

/////////////////////////////////////////////////////////////////////////////////////
// Functions for communicating with MIFARE PICCs
/////////////////////////////////////////////////////////////////////////////////////
StatusCode PCD_Authenticate(RFIDHandle h, byte command, byte blockAddr, MIFARE_Key *key, Uid *uid);
void PCD_StopCrypto1(RFIDHandle h);
StatusCode MIFARE_Read(RFIDHandle h, byte blockAddr, byte *buffer, byte *bufferSize);
StatusCode MIFARE_Write(RFIDHandle h, byte blockAddr, byte *buffer, byte bufferSize);
StatusCode MIFARE_Ultralight_Write(RFIDHandle h, byte page, byte *buffer, byte bufferSize);
StatusCode MIFARE_Decrement(RFIDHandle h, byte blockAddr, int32_t delta);
StatusCode MIFARE_Increment(RFIDHandle h, byte blockAddr, int32_t delta);
StatusCode MIFARE_Restore(RFIDHandle h, byte blockAddr);
StatusCode MIFARE_Transfer(RFIDHandle h, byte blockAddr);
StatusCode MIFARE_GetValue(RFIDHandle h, byte blockAddr, int32_t *value);
StatusCode MIFARE_SetValue(RFIDHandle h, byte blockAddr, int32_t value);
StatusCode PCD_NTAG216_AUTH(RFIDHandle h, byte *passWord, byte pACK[]);
	
/////////////////////////////////////////////////////////////////////////////////////
// Support functions
/////////////////////////////////////////////////////////////////////////////////////
StatusCode PCD_MIFARE_Transceive(RFIDHandle h, byte *sendData, byte sendLen, bool acceptTimeout);
PICC_Type PICC_GetType(byte sak);
	
// Support functions for debuging - proxy for MFRC522Debug to keep backwarts compatibility
const __FlashStringHelper *GetStatusCodeName(StatusCode code);
const __FlashStringHelper *PICC_GetTypeName(PICC_Type type);
	
// Support functions for debuging
void PCD_DumpVersionToSerial(RFIDHandle h);
void PICC_DumpToSerial(RFIDHandle h, Uid *uid);
void PICC_DumpDetailsToSerial(RFIDHandle h, Uid *uid);
void PICC_DumpMifareClassicToSerial(RFIDHandle h, Uid *uid, PICC_Type piccType, MIFARE_Key *key);
void PICC_DumpMifareClassicSectorToSerial(RFIDHandle h, Uid *uid, MIFARE_Key *key, byte sector);
void PICC_DumpMifareUltralightToSerial(RFIDHandle h);
	
// Advanced functions for MIFARE
void MIFARE_SetAccessBits(RFIDHandle h, byte *accessBitBuffer, byte g0, byte g1, byte g2, byte g3);
	
/////////////////////////////////////////////////////////////////////////////////////
// Convenience functions - does not add extra functionality
/////////////////////////////////////////////////////////////////////////////////////
bool PICC_IsNewCardPresent(RFIDHandle h);
bool PICC_ReadCardSerial(RFIDHandle h);


#ifdef __cplusplus
} ;
#endif


/* [] END OF FILE */
