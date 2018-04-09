
#include "project.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "MFRC522Interface.h"
#include "ArdInterface.h"
#include "ArdSerialInterface.h"
#include "ArdSpiInterface.h"


const psocPin_t pinMap[] = {
        {1,GPIO_PRT10,3},
        {2,MFRC522_RST_PORT,MFRC522_RST_NUM}  
    };


const arduino_timing_t timingFunctions = 
    {
        .delayFcn = vTaskDelay,
        .millisFcn = xTaskGetTickCount,
        .delayMicrosecondsFcn =  CyDelayUs
    };

void setupArduino()
{
    
    psocArduinoConfigure(pinMap , sizeof(pinMap)/sizeof(pinMap[0]), &timingFunctions);
    
    #if 0
        
    psocArduinoTestPin2(3,1);
    psocArduinoTestPin1(2,1);    
    psocArduinoTestPin(1); // this will never return;
    #endif
    
    
    psocArduinoSerialSetup(UART_1_HW);  
//    psocArduinoSerialTest();
//    while(1);
    
}

void testReader(RFIDHandle h)
{
  
  
  printf(("*****************************\r\n"));
  printf(("MFRC522 Digital self test\r\n"));
  printf(("*****************************\r\n"));
  PCD_DumpVersionToSerial(h);  // Show version of PCD - MFRC522 Card Reader
  printf(("-----------------------------\r\n"));
  printf(("Only known versions supported\r\n"));
  printf(("-----------------------------\r\n"));
  printf(("Performing test...\r\n"));
  bool result = PCD_PerformSelfTest(h); // perform the test
  printf("-----------------------------\r\n");
  printf("Result: ");
  if (result)
    printf(("OK"));
  else
    printf(("DEFECT or UNKNOWN\r\n"));
  printf("\r\n");
    
}


void uartTask(void *arg)
{
    (void)arg;
    printf("Started UART Task\r\n");
    setvbuf( stdin, NULL, _IONBF, 0 ); 
    
    RFIDHandle h ;
    setupArduino();
    while(1)
    {
        char c;
        c = getchar();
        uint32_t val;
        uint8_t reg = 0x37;
        
        switch(c)
        {
            case 'q':
             printf("CreateRFID call\r\n");
             h = createRFID(SPI_1_HW,1, 2) ;
             printf("CreateRFID Done\r\n");
            break;
            
            case 'w':
                printf("Testing Reader\r\n");   
                testReader(h);
                printf("Test Reader Succeeded\r\n");
            break;
                
            case 'a':
                Cy_GPIO_Write(MFRC522_SS_PORT,MFRC522_SS_NUM,0);
                vTaskDelay(1);
                    
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
                Cy_GPIO_Write(MFRC522_SS_PORT,MFRC522_SS_NUM,1);
                
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
