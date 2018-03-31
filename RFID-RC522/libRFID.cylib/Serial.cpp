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

#include "Serial.h"

SerialClass Serial ;

SerialClass::SerialClass()
{
    //
    // TODO - initialize the UART to output debug output through a given
    //        SCB block.  If the UART used for debugging is initialized
    //        elsewhere, than this can be empty and the methods below just
    //        talk to the already initialized SCB block to send the output
    //        over the UART.
    //
}

void SerialClass::print(const char * const * str_p)
{
    //
    // TODO - print the string to the uart output
    //
}

void SerialClass::print(const char * str_p)
{
    //
    // TODO - print the string to the uart output
    //
}

void SerialClass::print(uint8_t v, int base)
{
    //
    // TODO - print the eight bit value in the base given
    //
}

void SerialClass::println(const char * const str_p)
{
    print(str_p) ;
    println() ;
}

void SerialClass::println(const char * const * str_p)
{
    print(str_p) ;
    println() ;
}

void SerialClass::println(uint8_t v, int base)
{
    print(v, base) ;
    println() ;
}

void SerialClass::println()
{
    //
    // TODO - print a newline character
    //
}


/* [] END OF FILE */
