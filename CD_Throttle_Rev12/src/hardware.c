// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: hardware.c
// Author  : Rick Wright
// Revision: C
//
// Description:     This module provides Hardware Support Routines for the STM32
//
// ------------------------------------------------------------------------
//
// ------------------------------------------------------------------------
//
// Revision Log:
//
//  A           CRU   Baseline Release   12/21/2021
//  B           RLW   06/27/2022   Changes per PR #19 in PACT
//  C           CRU   06/30/2022   Changes per PR #22 in PACT
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

const uint8_t AHBPrescTable[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9 };
const uint8_t APBPrescTable[8] = { 0, 0, 0, 0, 1, 2, 3, 4 };

// ------------------------------------------------------------------------
//
//  EnableInterrupts - Assembly routine to change the processor state and enable interrupts
//
//  Params:   None
//
//  returns:  None
//
__attribute__((always_inline)) __INLINE void EnableInterrupts(void) {
    __ASM volatile ("cpsie i" : : : "memory");
}

// ------------------------------------------------------------------------
//
//  DisableInterrupts - Assembly routine to change the processor state and disable interrupts
//
//  Params:   None
//
//  returns:  None
//
__attribute__((always_inline)) __INLINE void DisableInterrupts(void) {
    __ASM volatile ("cpsid i" : : : "memory");
}

// ------------------------------------------------------------------------
//
//  STM32_GetTick - Routine to get the SysTick counter value for timing executions
//
//  Params:   None
//
//  returns:  Returns the current SysTick counter value
//
uint32_t STM32_GetTick(void) {
    return uTicks;
}

// ------------------------------------------------------------------------
//
//  STM32_Delay - Routine to implemenent a delay using the SysTick timer
//
//  Params:   Delay - The amount of time (in milliseconds) to delay
//
//  returns:  None
//
void STM32_Delay(volatile uint32_t Delay) {

    uint32_t tickstart, wait;

    tickstart = uTicks;
    wait = Delay + 1;
    do {
    } while ((STM32_GetTick() - tickstart) < wait);
}

// ------------------------------------------------------------------------
//
//  STM32_NVIC_SetPriority - Routine set the priority of an interrupt
//
//  Params:   IRQn - Enumerator of the IRQn_Type which corresponds to the external interrupt number
//            PreemptPriority - Value between 0-15 to set the interrupt preemption priority (A lower value is a higher priority)
//            SubPriority - Value between 0-15 to set the interrupt subpriority level
//
//  returns:  None
//
void STM32_NVIC_SetPriority(enumIRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority) {

    uint32_t  Bit_Shift, Shp_Index, Ip_Index;

    Bit_Shift = (IRQn & 3) * 8;
    if ((int32_t)(IRQn) < 0) {
        Shp_Index = ((IRQn & 0xFu) - 8u) >> 2u;
        SCB->SHP[Shp_Index] = ((uint32_t)(SCB->SHP[Shp_Index] & ~(0xFFUL << Bit_Shift)) |
                               (((PreemptPriority << (8 - 2)) & 0xFFU) << Bit_Shift));
    } else {
        Ip_Index = IRQn >> 2u;
        NVIC->IP[Ip_Index] = ((uint32_t)(NVIC->IP[Ip_Index] & ~(0xFFUL << Bit_Shift)) |
                              (((PreemptPriority << (8 - 2)) & 0xFFU) << Bit_Shift));
    }
}

// ------------------------------------------------------------------------
//
//  STM32_NVIC_EnableIRQ - Routine to enable a specific interrupt in the NVIC interrupt controller
//
//  Params:   IRQn - Enumerator of the IRQn_Type which corresponds to the external interrupt number
//
//  returns:  None
//
void STM32_NVIC_EnableIRQ(enumIRQn_Type IRQn) {

    NVIC->ISER[0u] = (uint32_t)(1uL << (((uint32_t)(int32_t)IRQn) & 0x1FuL));
}

// ------------------------------------------------------------------------
//
//  STM32_NVIC_DisableIRQ - Routine to disable a specific interrupt in the NVIC interrupt controller
//
//  Params:   IRQn - Enumerator of the IRQn_Type which corresponds to the external interrupt number
//
//  returns:  None
//
void STM32_NVIC_DisableIRQ(enumIRQn_Type IRQn) {

    NVIC->ICER[0u] = (uint32_t)(1uL << (((uint32_t)(int32_t)IRQn) & 0x1FuL));
}

