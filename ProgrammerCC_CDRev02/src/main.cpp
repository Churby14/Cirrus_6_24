//
// ****************************************************************************
//
//  Description:       Main Loop
//
//  Release History:
//
//  2018-10-12  RLW     Initial Release
//
// ****************************************************************************
//
#include "global.h"
#include "math.h"
//
// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
int main(void) {

    uint32_t    LoopCounter = 0;
    uint8_t     CDTxOffTimer = 0;
    uint8_t     CDRxOffTimer = 0;

    InitializeSystem();

    while (1) {

        LoopCounter++;
        ProcessTickCounters();

        if (CDTxBufferIn != CDTxBufferOut) {
            if (CDTxOffTimer == 0) {
                HAL_GPIO_WritePin( RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET);                              // Turn on RS422/485 Driver
            }
            if ( (USART4->ISR & (1 << 6)) != 0 ) {                           // Wait for Byte to be sent (Transmission Complete)
                USART4->TDR = CDTxBuffer[CDTxBufferOut];                          // Send Data
                CDTxBufferOut++;
            }
            CDTxOffTimer = 48;
        } else if (CDTxOffTimer != 0) {
            if (--CDTxOffTimer == 0) {
                HAL_GPIO_WritePin( RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET);            // Turn off RS422/485 Driver
                SendMessageToCD = 0;
                SendMessageToPC = 1;
            }
        }

        if (CDRxBufferIn != CDRxBufferOut) {
            if (CDRxOffTimer == 0) {
                HAL_GPIO_WritePin( PC_RS485_EN_GPIO_Port, PC_RS485_EN_Pin, GPIO_PIN_SET);                              // Turn on RS422/485 Driver
            }
            if ((USART1->ISR & (1 << 6)) != 0 ) {                           // Wait for Byte to be sent (Transmission Complete)
                USART1->TDR = CDRxBuffer[CDRxBufferOut];                          // Send Data
                CDRxBufferOut++;
            }
            CDRxOffTimer = 48;
        } else if (CDRxOffTimer != 0) {
            if (--CDRxOffTimer == 0) {
                HAL_GPIO_WritePin( PC_RS485_EN_GPIO_Port, PC_RS485_EN_Pin, GPIO_PIN_RESET);            // Turn off RS422/485 Driver
                SendMessageToPC = 0;
            }
        }

        if (Ticks1ms >= 1) {
            Ticks1ms -= 1;
            ProcessI2C1();
            ProcessFPGACommunications();
        }

        if (Ticks5ms >= 5) {
            Ticks5ms -= 5;
            CalculuateFrequencyInput();
            if (CDTxOffTimer == 0) {
                ProcessAdc();
            }
        }

        if (Ticks1000ms >= 1000) {              // One second
            Ticks1000ms -= 1000;
            SysVars.TestData[0] = LoopCounter;
            LoopCounter = 0;
        }
    }
}
