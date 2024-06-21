// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Filename:      uart.c
// Author:        Rick Wright/Charlie Urban
// Revision:      3.2
//
// Description: This file contains STM32 UART support routiens
//
// ------------------------------------------------------------------------
//
// ------------------------------------------------------------------------
//
// Revision Log:
//
//  3.2    CRU     07/26/2022 Integration of released fuel sender software and throttle SW
//
// ------------------------------------------------------------------------
// Design Details and Explanations:
// ------------------------------------------------------------------------
//
//
// ------------------------------------------------------------------------
// Implementation Notes:
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//                              Include Files
// ------------------------------------------------------------------------

#include "global.h"

// ------------------------------------------------------------------------
//                               Definitions
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//                  Local Conditional Compilation Switches
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//                          Local Variables
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//
// HAL_UART_TxCpltCallback - This routine is called on TX Complete by ISR
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {

    if (huart->Instance == USART1) {
        STM32_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET);            // Turn off RS422/485 Driver
    }
    if (huart->Instance == USART2) {
        if (DIG_HandleStruct[0].DIG_InitStruct.Mode == DIG_SERIAL_INOUT) {
            STM32_GPIO_WritePin(DIG_HandleStruct[0].Dir_Port, DIG_HandleStruct[0].Dir_Pin, GPIO_PIN_RESET);  // Switch Digital I/O #1 Buffer back to an Input
        }
        bSendingMessage = FALSE;
    }
}

// ------------------------------------------------------------------------
//
// UART_DMATransmitCplt - DMA UART transmit complete callback - interrupt
//
// Params:    hdma - dma handle
//
// Returns:   None
//
// Requirement(s):
//
static void UART_DMATransmitCplt(DMA_HandleTypeDef *hdma) {

    UART_HandleTypeDef *huart = (UART_HandleTypeDef *)(hdma->Parent);

    huart->Instance->CR3 &= ~USART_CR3_DMAT;        /* Disable the DMA transfer */
    huart->Instance->CR1 |= USART_CR1_TCIE;          /* Enable the UART Transmit Complete Interrupt */
}

// ------------------------------------------------------------------------
//
// STM32_UART_Transmit_DMA - Sends data in non blocking mode
//
// Params:    huart - UART handle
//            pData - pointer to data being sent
//            Size - size (in bytes) of data being sent via UART
//
// Returns:   None
//
// Requirement(s):
//
void STM32_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size) {

    huart->hdmatx->XferCpltCallback = UART_DMATransmitCplt;
    huart->hdmatx->Instance->CCR &= ~DMA_CCR_EN;
    huart->hdmatx->DmaBaseAddress->IFCR = (DMA_FLAG_GL1 << huart->hdmatx->ChannelIndex);
    huart->hdmatx->Instance->CNDTR = Size;
    huart->hdmatx->Instance->CPAR = (uint32_t)&huart->Instance->TDR;
    huart->hdmatx->Instance->CMAR = (uint32_t)pData;
    huart->hdmatx->Instance->CCR |= (DMA_IT_TC | DMA_IT_TE);
    huart->hdmatx->Instance->CCR &= ~DMA_IT_HT;
    huart->hdmatx->Instance->CCR |= DMA_CCR_EN;
    huart->Instance->ICR = UART_CLEAR_TCF;
    huart->Instance->CR3 |= USART_CR3_DMAT;
}

// ------------------------------------------------------------------------
//
// UART_EndTransmit_IT - Ends transmission in non blocking mode
//
// Params:    huart - UART handle
//
// Returns:   None
//
// Requirement(s):
//
static void UART_EndTransmit_IT(UART_HandleTypeDef *huart) {

    huart->Instance->CR1 &= ~USART_CR1_TCIE;
    HAL_UART_TxCpltCallback(huart);
}

// ------------------------------------------------------------------------
//
// STM32_UART_IRQHandler - Interrupt to receive data via UART
//
// Params:    huart - UART handle
//
// Returns:   None
//
// Requirement(s):
//
void STM32_UART_IRQHandler(UART_HandleTypeDef *huart) {

    uint32_t ISRFlags;
    uint32_t ControlReg1its;

    ISRFlags = huart->Instance->ISR;
    ControlReg1its = huart->Instance->CR1;

    //
    // Clear Errors - even if there aren't any
    //
    huart->Instance->ICR = UART_CLEAR_PEF | UART_CLEAR_FEF | UART_CLEAR_NEF | UART_CLEAR_OREF;

    /* UART in mode Receiver */
    if (huart->Instance == USART1) {
        if (((ISRFlags & USART_ISR_RXNE) != 0) && ((ControlReg1its & USART_CR1_RXNEIE) != 0)) {
            SerialUsart1.DmaRingBuffer[SerialUsart1.RingBufferIn++] = (uint8_t)huart->Instance->RDR;
            return;
        }
    }
    if (huart->Instance == USART2) {
        if (((ISRFlags & USART_ISR_RXNE) != 0) && ((ControlReg1its & USART_CR1_RXNEIE) != 0)) {
            SerialUsart2.DmaRingBuffer[SerialUsart2.RingBufferIn++] = (uint8_t)huart->Instance->RDR;
            return;
        }
    }

    /* UART in mode Transmitter (transmission end) -----------------------------*/
    if (((ISRFlags & USART_ISR_TC) != 0) && ((ControlReg1its & USART_CR1_TCIE) != 0)) {
        UART_EndTransmit_IT(huart);
    }
}

// ------------------------------------------------------------------------
//
// MX_USART1_UART_Init -Routine to initialize UART 1 for serial RS485 communication
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void MX_USART1_UART_Init(void) {

    uint32_t Tmp;

    huart1.Instance = USART1;

    STM32_GPIO_Init(RS485_TXD_GPIO_Port, RS485_TXD_Pin, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF1_USART1, 0);
    STM32_GPIO_Init(RS485_RXD_GPIO_Port, RS485_RXD_Pin, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF1_USART1, 0);

    hdma_usart1_tx.Instance = DMA1_Channel2;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;

    Tmp = hdma_usart1_tx.Instance->CCR;
    Tmp &= ((uint32_t)~(DMA_CCR_PL | DMA_CCR_MSIZE | DMA_CCR_PSIZE | DMA_CCR_MINC | DMA_CCR_PINC | DMA_CCR_CIRC | DMA_CCR_DIR));
    Tmp |= DMA_MEMORY_TO_PERIPH | DMA_PINC_DISABLE | DMA_MINC_ENABLE |
        DMA_PDATAALIGN_BYTE | DMA_MDATAALIGN_BYTE | DMA_NORMAL | DMA_PRIORITY_LOW;
    hdma_usart1_tx.Instance->CCR = Tmp;
    hdma_usart1_tx.ChannelIndex = (((uint32_t)hdma_usart1_tx.Instance - (uint32_t)DMA1_Channel1) / ((uint32_t)DMA1_Channel2 - (uint32_t)DMA1_Channel1)) << 2u;
    hdma_usart1_tx.DmaBaseAddress = DMA1;
    hdma_usart1_tx.XferCpltCallback = NULL;

    huart1.hdmatx = &hdma_usart1_tx;
    hdma_usart1_tx.Parent = &huart1;

    STM32_NVIC_SetPriority(USART1_IRQn, 0, 0);
    STM32_NVIC_EnableIRQ(USART1_IRQn);

    USART1->CR1 &= ~USART_CR1_UE;
    USART1->CR1 = 0x0C;
    USART1->CR2 = 0x00;
    USART1->CR3 = 0x3000;
    USART1->BRR = 417;
    USART1->CR1 = 0x2D;
}

// ------------------------------------------------------------------------
//
// MX_USART2_UART_Init - UART2_RX - Connected to Digital I/O #1
// UART2_TX - Connected to Digital I/O #1 through 4.7K resistor and to Digital I/O #2 through 4.7K resistor.
//
// UART2_RX is used when Digital I/O #1 is configured in SERIAL INOUT mode.  This allows commands to be received through Digital I/O
// #1.
// UART2_TX is used for two purposes.  When Digital I/O #1 is configured as SERIAL_INOUT, after receiving a command, the Output Buffer
// Driver is switched from input to output mode.  Then a response message is sent via UART2_TX through the 4.7K resistor and out the
// Output Buffer chip onto Digital I/O #1.  This allows Digital I/O #1 to function in half duplex mode.
//
// UART2_TX is also used to drive Digital I/O #2 when it's configured as SERIAL_SLAVE_OUT or SERIAL_MASTER_OUT.
//
// HLR-xxxx
//
// DIG I/O #1 TX/RX
// DIG I/O #2 TX
//
//
//
void MX_USART2_UART_Init(void) {

    uint32_t Tmp;

    huart2.Instance = USART2;

    STM32_GPIO_Init(DIG_TX_GPIO_Port, DIG_TX_Pin, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF1_USART2, 0);
    STM32_GPIO_Init(DIG_RX_GPIO_Port, DIG_RX_Pin, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF1_USART2, 0);

    hdma_usart2_tx.Instance = DMA1_Channel4;
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;

    Tmp = hdma_usart2_tx.Instance->CCR;
    Tmp &= ((uint32_t)~(DMA_CCR_PL | DMA_CCR_MSIZE | DMA_CCR_PSIZE | DMA_CCR_MINC | DMA_CCR_PINC | DMA_CCR_CIRC | DMA_CCR_DIR));
    Tmp |= DMA_MEMORY_TO_PERIPH | DMA_PINC_DISABLE | DMA_MINC_ENABLE |
        DMA_PDATAALIGN_BYTE | DMA_MDATAALIGN_BYTE | DMA_NORMAL | DMA_PRIORITY_LOW;
    hdma_usart2_tx.Instance->CCR = Tmp;
    hdma_usart2_tx.ChannelIndex = (((uint32_t)hdma_usart2_tx.Instance - (uint32_t)DMA1_Channel1) / ((uint32_t)DMA1_Channel2 - (uint32_t)DMA1_Channel1)) << 2u;
    hdma_usart2_tx.DmaBaseAddress = DMA1;
    hdma_usart2_tx.XferCpltCallback = NULL;

    huart2.hdmatx = &hdma_usart2_tx;
    hdma_usart2_tx.Parent = &huart2;

    STM32_NVIC_SetPriority(USART2_IRQn, 0, 0);
    STM32_NVIC_EnableIRQ(USART2_IRQn);

    USART2->CR1 &= ~USART_CR1_UE;
    USART2->CR1 = 0x0C;
    USART2->CR2 = 0x00;
    USART2->CR3 = 0x3000;
    USART2->BRR = 417;
    USART2->CR1 = 0x2D;
}

