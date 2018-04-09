#include "Arduino.h"
#include "gpio/cy_gpio.h"
#include <assert.h>
#include "ArdInterface.h"
#include <stdio.h>

psocPin_t *psocPinMap;
int psocPinMapSize=0;
arduino_timing_t *arduino_timing;

void psocArduinoConfigure(psocPin_t *pinMap , int pinMapSize, arduino_timing_t *ardTimingFunctions)
{
    psocPinMap = pinMap;
    psocPinMapSize = pinMapSize; 
    arduino_timing = ardTimingFunctions;
}


void psocArduinoTestPin(int pin)
{
    pinMode(pin,OUTPUT);
    while(1)
    {
        int current = digitalRead(pin);
        current = !current;
        
        digitalWrite(pin,!digitalRead(pin));
        delay(200);
    }
}

void psocArduinoTestPin1(int inpin, int outpin)
{
    int val;
    pinMode(inpin,INPUT);
    pinMode(outpin,OUTPUT);
    while(1)
    {
        val = digitalRead(inpin);
        digitalWrite(outpin,val);
    }
}

void psocArduinoTestPin2(int inpin, int outpin)
{
    int val;
    pinMode(inpin,INPUT_PULLUP);
    pinMode(outpin,OUTPUT);
    while(1)
    {
        val = digitalRead(inpin);
        digitalWrite(outpin,val);
    }
}


psocPin_t *findArduinoPin(int pin)
{
    for(int i = 0;i<psocPinMapSize; i++)
    {
        if(psocPinMap[i].ardNumber == pin)
        {
            return &psocPinMap[i];
        }
    }
    return 0;
}

void digitalWrite(int pin, int value)
{
    psocPin_t *myPin;
    myPin = findArduinoPin(pin);
    assert(myPin != 0);
    
    Cy_GPIO_Write(myPin->psocPort,myPin->psocPin,value);
}

int digitalRead(int pin)
{
    psocPin_t *myPin;
    myPin = findArduinoPin(pin);
    assert(myPin != 0);
    int rval = Cy_GPIO_Read(myPin->psocPort,myPin->psocPin);
    rval = rval>0?1:0;
    return  rval;
}

void pinMode(int pin, int mode)
{

    psocPin_t *myPin;
    myPin = findArduinoPin(pin);
    assert(myPin != 0);
    
    //printf("Initialzing pin=%d PSoC Port=0x%X PSoC Pin=%d\r\n",pin,myPin->psocPort,myPin->psocPin); 
    
    cy_stc_gpio_pin_config_t pinConfig = {
        /*.outVal =*/ 0UL,                  /* Output = High */
        /*.driveMode =*/ CY_GPIO_DM_PULLUP, /* Resistive pull-up, input buffer on */
        /*.hsiom =*/ (en_hsiom_sel_t)0,     /* Software controlled pin */
        /*.intEdge =*/ CY_GPIO_INTR_DISABLE, /* Rising edge interrupt */
        /*.intMask =*/ 0UL,                 /* Disable port interrupt for this pin */
        /*.vtrip =*/ CY_GPIO_VTRIP_CMOS,    /* CMOS voltage trip */
        /*.slewRate =*/ CY_GPIO_SLEW_FAST,  /* Fast slew rate */
        /*.driveSel =*/ CY_GPIO_DRIVE_FULL, /* Full drive strength */
        /*.vregEn =*/ 0UL,                  /* SIO-specific setting - ignored */
        /*.ibufMode =*/ 0UL,                /* SIO-specific setting - ignored */
        /*.vtripSel =*/ 0UL,                /* SIO-specific setting - ignored */
        /*.vrefSel =*/ 0UL,                 /* SIO-specific setting - ignored */
        /*.vohSel =*/ 0UL                   /* SIO-specific setting - ignored */
    };
    
    if(mode == INPUT)
    {
        pinConfig.driveMode = CY_GPIO_DM_HIGHZ;
    }
    else if (mode == OUTPUT)
    {
        pinConfig.driveMode = CY_GPIO_DM_STRONG;
    }
    
    else if (mode == INPUT_PULLUP)
    {
        pinConfig.outVal = 1;
        pinConfig.driveMode = CY_GPIO_DM_PULLUP;
    }
    
    else
    {
        assert(0); // 
    }
    
    if(CY_GPIO_SUCCESS != Cy_GPIO_Pin_Init(myPin->psocPort, myPin->psocPin, &pinConfig))
    {
        assert(0);
    }

}

void delayMicroseconds(int us)
{
    (*arduino_timing->delayMicrosecondsFcn)(us);
}

void delay(int ms)
{
    (*arduino_timing->delayFcn)(ms);
}

unsigned long millis()
{
    return (*arduino_timing->millisFcn)();
}
