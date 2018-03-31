#pragma once

#include <stdint.h>
#include <string.h>

#include "Serial.h"
#include "SPI.h"

//
// This keyboard is an attribute used to force a data structure
// into flash memory.
//
#define PROGMEM

#define F(str) reinterpret_cast<const char * const *>(str)

//
// Digital logic high and digital low
//
#define LOW			(0)
#define HIGH		(1)

//
// Pin modes
//
#define INPUT		(0)
#define OUTPUT		(1)

void digitalWrite(int pin, int value) ;
int digitalRead(int pin) ;
void pinMode(int pin, int mode) ;
void delayMicroseconds(int ms) ;
void delay(int ms) ;

inline uint8_t pgm_read_byte(const uint8_t *ptr_p)
{
	return *ptr_p ;
}

unsigned long millis() ;
	

//
// Types defined by Arduino that we need to duplicate
//
typedef uint8_t byte ;
typedef const char *__FlashStringHelper ;

