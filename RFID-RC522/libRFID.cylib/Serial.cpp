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
#include "scb/cy_scb_common.h"
#include "scb/cy_scb_uart.h"


#include "ArdSerialInterface.h"
#include <stdio.h>

SerialClass Serial ;

CySCB_Type *myUart;

     
void psocArduinoSerialSetup(CySCB_Type *scb)
{
    myUart = scb;
}

void psocArduinoSerialTest()
{
    Serial.print("Serial.print\r\n");
    Serial.print("22=");
    Serial.print(22,DEC);
    Serial.println();
    Serial.print("50=");
    Serial.print(50,HEX);
    Serial.println();
    Serial.println("Serial.println");
    
    Serial.print("22=");
    Serial.println(22,DEC);
    Serial.print("50=");
    Serial.println(50,HEX); 

}


SerialClass::SerialClass()
{
    // For now assume that they setup the scb as a uart    
}

void SerialClass::print(const char * const * str_p)
{
    Cy_SCB_UART_PutString(myUart,(char *)str_p);
}

void SerialClass::print(const char * str_p)
{
    Cy_SCB_UART_PutString(myUart,str_p);

}

void SerialClass::print(uint8_t v, int base)
{
    char buff[4]; 
    switch(base)
    {
        case HEX:
            sprintf(buff,"%X",v);
        break;
        
        default:
        case DEC:
            sprintf(buff,"%u",v);
        break;
       
    }
    Cy_SCB_UART_PutString(myUart,buff);

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
    Cy_SCB_UART_PutString(myUart,"\r\n");
}
