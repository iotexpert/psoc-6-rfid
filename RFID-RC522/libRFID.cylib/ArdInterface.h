#pragma once
#include "gpio/cy_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct  {
    int ardNumber;
    GPIO_PRT_Type* psocPort;
    int psocPin;
} psocPin_t;

typedef struct {
    void (*delayFcn)(uint32_t ms);
    unsigned long (*millisFcn)();
    void (*delayMicrosecondsFcn)(uint32_t us);
} arduino_timing_t;

void psocArduinoConfigure(psocPin_t *pinMap , int pinMapSize, arduino_timing_t *ardTimingFunctions);

void psocArduinoTestPin(int pin);
void psocArduinoTestPin1(int inpin, int outpin);
void psocArduinoTestPin2(int inpin, int outpin);

#ifdef __cplusplus
} ;
#endif
