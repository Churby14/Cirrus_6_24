//
// ********************************************************************************************************************************
//
// Description:     This module contains functions that:
//                      - Read A/D inputs
//                      - Filters A/D inputs
//                      - Convert Filtered A/D counts into A/D Filtered voltage
//                      - Convert A/D Filtered voltage into Engineering Units
//
// Release History:
//
//  2019-01-24  RLW     Initial Release
//
// ********************************************************************************************************************************

#include "global.h"


void InitializeSystem (void) {

    uint16_t            *PagePtr;

    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_CAN_Init();
    MX_CRC_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_USART4_UART_Init();

    MX_TIM6_Init();
    __HAL_TIM_SET_COUNTER(&htim6, 0); // Setup to run at 200Khz.  Rollover every 327.68mS
    HAL_TIM_Base_Start(&htim6);
    MX_TIM7_Init();
    __HAL_TIM_SET_COUNTER(&htim7, 0); // Setup to run at 1Mhz.   Rollover every 16.384mS
    HAL_TIM_Base_Start(&htim7);

    PagePtr = (uint16_t*)(0x08000000 + (2048 * 56));
    if ((memcmp((void *)(0x08000000 + (2048 * 55)), (void *)(0x08000000 + (2048 * 56)), 2048) != 0) && (PagePtr[1023] == 0xAA55)) {
        ReflashTuningParameters(55, (uint16_t *)(0x08000000 + (2048 * 56)));
    }

    //
    // Setup pointers to Configuration structures stored at end of FLASH.  The flash within the STM32 consists of 64 sectors,
    //  with each sector being 2048 bytes (64 * 2K = 128KB).  The last sector is used for User Configuration, the next to last
    //  sector contains Factory Configuration.
    //
    FlashConfiguration = (stFlashConfiguration *) (0x08000000 + (2048 * 63));
    FactoryConfiguration = (stFactoryConfiguration *) (0x08000000 + (2048 * 62));
	//
	//  Initiailize Instance Number of SerialUsartx data structure
	//
    // Used to communicate with PC
    //
	SerialUsart1.InstanceNumber = 1;
    SerialUsart1.ReceiveMode = 1;
    SerialUsart1.TxRxBuffer = SerialBuffer1;
    SerialUsart1.TxRxBufferSize = sizeof(SerialBuffer1);
    //
    // Used to communicate with Sender via DIG_IO1/2
    //
    SerialUsart2.InstanceNumber = 2;
    SerialUsart2.ReceiveMode = 1;
    SerialUsart2.TxRxBuffer = SerialBuffer2;
    SerialUsart2.TxRxBufferSize = sizeof(SerialBuffer2);
    //
    // Used to communicate with Sender via RS485 interface
    //
    SerialUsart4.InstanceNumber = 4;
    SerialUsart4.ReceiveMode = 1;
    SerialUsart4.TxRxBuffer = SerialBuffer4;
    SerialUsart4.TxRxBufferSize = sizeof(SerialBuffer4);
    //
    // Enable TSEN, VBATEN, and VREFEN
    //
    ADC1_COMMON->CCR |= ADC_CCR_TSEN | ADC_CCR_VBATEN | ADC_CCR_VREFEN;


    I2C1Control.hi2c = &hi2c1;
    HAL_UART_Receive_IT(&huart1, &SerialUsart1.RxChar, 1);
    HAL_UART_Receive_IT(&huart4, &SerialUsart4.RxChar, 1);

    InitializeDigitalIO(0); // Configure DIG_IO1 as SERIAL_INOUT, all other DIG I/O as Digital Inputs (disabled)
    InitializeNAU7802();
    __enable_irq();

}
