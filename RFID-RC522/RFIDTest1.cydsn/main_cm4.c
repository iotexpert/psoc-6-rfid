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
#include "project.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#include "rfid.h"

void uartTask(void *arg)
{
    (void)arg;
    printf("Started UART Task\r\n");
    setvbuf( stdin, NULL, _IONBF, 0 ); 
    
    RFIDHandle h ;
    
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    //
    // Note, pins 0 and 1 for chipsel and reset are not physical pins.  The
    // RFID library refers to pins with this integer, and the Arduino.cpp file
    // in the RFID library must be updated to refer to the correct PSoC pin
    // when it sees pin 0 or pin 1.
    //
    h = createRFID(0, 1, 0, 1) ;
    
    
    while(1)
    {
        char c;
        c = getchar();
        uint32_t val;
        uint8_t reg = 0x37;
        
        switch(c)
        {
            case 'a':
                printf("Reading register %X\r\n",reg);
                Cy_SCB_SPI_ClearRxFifo(SPI_1_HW);
                
                //Cy_SCB_SPI_SetActiveSlaveSelect(SPI_1_HW,CY_SCB_SPI_SLAVE_SELECT0);
                Cy_SCB_SPI_Write(SPI_1_HW,0xC7); // register
                
                //while(Cy_SCB_SPI_IsBusBusy(SPI_1_HW));
                
                Cy_SCB_SPI_Write(SPI_1_HW,0x37);
                
               
                printf("Num in tx fifo = %d\r\n",Cy_SCB_SPI_GetNumInTxFifo(SPI_1_HW));
                
                
                while(Cy_SCB_SPI_GetNumInTxFifo(SPI_1_HW));
                
                printf("Num in rx fifo = %d\r\n",Cy_SCB_SPI_GetNumInRxFifo(SPI_1_HW));
                
                (void)Cy_SCB_SPI_Read(SPI_1_HW); // ignore
                val = Cy_SCB_SPI_Read(SPI_1_HW);
                printf("Val 1 = %X\r\n",val);
                
                
                printf("Num in rx fifo = %d\r\n",Cy_SCB_SPI_GetNumInRxFifo(SPI_1_HW));
                
                break;
                    
            case '?':
                printf("?\tHelp\r\n");
            break;
        }
    }
    
}

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_1_Start();
    SPI_1_Start();
    
    xTaskCreate(uartTask,"uartTask",1024,0,1,0);
    vTaskStartScheduler();
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
