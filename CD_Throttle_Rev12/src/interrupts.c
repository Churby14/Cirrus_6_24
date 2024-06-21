// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: interrupts.c
// Author  : Rick Wright/Charlie Urban
// Revision: 3.2
//
// Description:     System Interrupt routines
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Revision Log:
//
//  3.2    CRU     07/26/2022 Integration of released fuel sender software and throttle SW
//
// ------------------------------------------------------------------------
// Design Details and Explanations:
// ------------------------------------------------------------------------

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
// NMI_Handler - Routine to handle non maskable interrupts
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void NMI_Handler(void) {
    Error_Handler(FILE_INTERRUPTS, __LINE__);
}

// ------------------------------------------------------------------------
//
// HardFault_Handler - Routine used to hard reset the MCU by keeping the MCU in a while loop
//                     until the watchdog timer triggers the reset
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void HardFault_Handler(void) {
    Error_Handler(FILE_INTERRUPTS, __LINE__);
}

// ------------------------------------------------------------------------
//
// SVC_Handler - Routine to handle Supervisor call interrupts
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void SVC_Handler(void) {
    Error_Handler(FILE_INTERRUPTS, __LINE__);
}

// ------------------------------------------------------------------------
//
// PendSV_Handler - Routine for Pendable Service interrupt request
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void PendSV_Handler(void) {
    Error_Handler(FILE_INTERRUPTS, __LINE__);
}

// ------------------------------------------------------------------------
//
// EXTI0_1_IRQHandler - Routine to handle GPIO Pin 1 toggle interrupt
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void EXTI0_1_IRQHandler(void) {
    STM32_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

// ------------------------------------------------------------------------
//
// EXTI2_3_IRQHandler - Routine to handle GPIO Pin 3 toggle interrupt
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void EXTI2_3_IRQHandler(void) {
    STM32_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

// ------------------------------------------------------------------------
//
// EXTI4_15_IRQHandler - Routine to handle GPIO Pins 4, 8 and 9 toggle interrupt
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void EXTI4_15_IRQHandler(void) {
    STM32_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
    STM32_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
    STM32_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}

// ------------------------------------------------------------------------
//
// SysTick_Handler - Routine to increment uTicks everytime a SysTick interrupt occurs
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void SysTick_Handler(void) {
    uTicks++;
}

// ------------------------------------------------------------------------
//
// USART1_IRQHandler - Routine to handle USART 1 interrupts
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void USART1_IRQHandler(void) {
    STM32_UART_IRQHandler(&huart1);
}

// ------------------------------------------------------------------------
//
// USART2_IRQHandler - Routine to handle USART 2 interrupts
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void USART2_IRQHandler(void) {
    STM32_UART_IRQHandler(&huart2);
}

// ------------------------------------------------------------------------
//
// USART3_4_IRQHandler - Routine to handle USART 4 interrupts
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void USART3_4_IRQHandler(void) {
    STM32_UART_IRQHandler(&huart4);
}

// ------------------------------------------------------------------------
//
// DMA1_Channel1_IRQHandler - Routine to handle interrupts on DMA Channel 1 - used by ADC
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void DMA1_Channel1_IRQHandler(void) {
    STM32_DMA_IRQHandler(&hdma_adc);
}

// ------------------------------------------------------------------------
//
// DMA1_Channel2_3_IRQHandler - Routine to handle interrupts on DMA Channels 2 and 3
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void DMA1_Channel2_3_IRQHandler(void) {
    STM32_DMA_IRQHandler(&hdma_usart1_tx);
    STM32_DMA_IRQHandler(&hdma_usart1_rx);
}

// ------------------------------------------------------------------------
//
// DMA1_Channel4_5_6_7_IRQHandler - Routine to handle interrupts on DMA Channels 4, 5, 6 and 7
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void DMA1_Channel4_5_6_7_IRQHandler(void) {
    STM32_DMA_IRQHandler(&hdma_usart2_tx);
    STM32_DMA_IRQHandler(&hdma_usart2_rx);
}


// -------------------------------------------------------------------------
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void CAN_IRQHandler(void) {
  // TESTING - ADD CODE HERE
}

