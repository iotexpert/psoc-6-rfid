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
#include <assert.h>

//
// TODO - the RFID library refers to pins as a single integer.  We use
//        port and pin within the port to refer to pins.  This file will need
//        to contain a mapping from the pin number to a specific port
//        and pin.  For a one time use, this mapping can be hard coded here,
//        but it could be generalized, where we add a function to register
//        a mapping from a RFID pin to a PSoC pin.
//

void digitalWrite(int pin, int value)
{
    //
    // TODO - write the code to set the value of a pin
    //        that is configured as an output pin.  This
    //        should be done using PDL calls.
    //
    assert(0) ;     // Remove me when this is implemented
}

int digitalRead(int pin)
{
    //
    // TODO - write the code to get the value of a pin
    //        that is configured as an output pin.  This
    //        should be done using PDL calls.
    //
    assert(0) ;     // Remove me when this is implemented
    return 0 ;
}

void pinMode(int pin, int mode)
{
    //
    // TODO - set this pin to the type of pin given by mode
    //        mode will be INPUT or OUTPUT
    //
    assert(0) ;     // Remove me when this is implemented
}

void delayMicroseconds(int ms)
{
    //
    // TODO - delay a fixed number of microseconds
    //
    assert(0) ;     // Remove me when this is implemented
}

void delay(int ms)
{
    //
    // TODO - delay for a given number of milliseconds
    //
    assert(0) ;     // Remove me when this is implemented
}

unsigned long millis()
{
    //
    // TODO - return the current number of milliseconds since
    //        some type of epoch.  This is used to run loops that
    //        do work but need to time out after a given amount of
    //        time.
    //
    //        long start = millis() ;
    //        while (millis() - start < 50)
    //        {
    //             doSomethingForUpTo50Ms() ;
    //        }
    //
    assert(0) ;     // Remove me when this is implemented
}

/* [] END OF FILE */
