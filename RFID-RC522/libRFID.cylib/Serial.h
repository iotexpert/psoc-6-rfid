#pragma once

#include <stdint.h>

#define DEC				(0)
#define HEX				(1)

class SerialClass
{
public:
    SerialClass() ;
	void print(const char * const * str_p) ;
   	void print(const char * str_p) ;
	void print(uint8_t, int base = DEC) ;
	
	void println(const char * const str_p) ;
	void println(const char * const * str_p) ;
	void println(uint8_t, int base = DEC) ;
	void println() ;
} ;

extern SerialClass Serial ;
