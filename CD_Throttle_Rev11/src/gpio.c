// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Filename:      gpio.c
// Author:        Rick Wright
// Revision:      C
//
// Description:  This file contains GPIO support routines
//
// ------------------------------------------------------------------------
//
// ------------------------------------------------------------------------
//
// Revision Log:
//
//  A           CRU   12/21/2021   Baseline Release
//  B           RLW   06/13/2022   Changes per PR #19 in PACT
//  C           CRU   06/30/2022   Changes per PR #22 in PACT
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
//  STM32_GPIO_EXTI_IRQHandler - This routine is used to handle GPIO pin toggle interrupts
//
//  Params:   GPIO_Pin - The GPIO pin number that will generate an interrupt when toggling
//
//  returns:  none
//
void STM32_GPIO_EXTI_IRQHandler(uint32_t GPIO_Pin) {

    if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
        HAL_GPIO_EXTI_Callback(GPIO_Pin);
    }
}

// ------------------------------------------------------------------------
//
//  STM32_GPIO_WritePin - This routine is used to write to a GPIO pin high or low depending on the parameters
//
//  Params:   GPIOx - Selects the GPIO peripheral where the GPIO pin is located on the MCU
//            GPIO_Pin - The GPIO pin number that will generate an interrupt when toggling
//            PinState - enumerator GPIO_PinState Specifies the value to be written to the pin (high or low)
//
//  returns:  none
//
void STM32_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, enumGPIO_PinState PinState) {

    if (PinState != GPIO_PIN_RESET) {
        GPIOx->BSRR = (uint32_t)GPIO_Pin;
    } else {
        GPIOx->BRR = (uint32_t)GPIO_Pin;
    }
}

// ------------------------------------------------------------------------
//
//  STM32_GPIO_ReadPin - This routine is used to read from a GPIO pin
//
//  Params:   GPIOx - Selects the GPIO peripheral where the GPIO pin is located on the MCU
//            GPIO_Pin - The GPIO pin number that will generate an interrupt when toggling
//
//  returns:  uint32_t value set high (1) or low (0) depending on the GPIO pin state
//
uint32_t STM32_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin) {

    return (GPIOx->IDR & GPIO_Pin) != 0;
}

// ------------------------------------------------------------------------
//
//  CalcBitPosition - This returns the location of a signle bit set high in a bitmask.
//
//  Params:   BitMask - The mask to find which one bit is set high (masking)
//
//  returns:  The position within a binary value set high or 0xFF if multiple binary values are set high
//
static uint32_t  CalcBitPosition(uint32_t BitMask) {

    for (uint32_t i = 0; i < 32; i++) {
        if ((BitMask & 1) != 0) {
            return i;
        }
        BitMask >>= 1;
    }
    Error_Handler(FILE_GPIO, __LINE__);
    return 0;
}

// ------------------------------------------------------------------------
//
//  STM32_GPIO_Init - This routine is used to initialize a GPIO pin
//
//  Params:   GPIOx - GPIO data structure containing the configuration information peripheral
//            Pin - The GPIO pin number being initialized
//            Mode - Input/Output mode
//            Pull - Pullup or pulldown or no internal resistor activation
//            Speed - Slew rate which outputs can change between high and low
//            Alternate - Set pin to alternate mode or not
//            OutputState - Set output pins as high or low
//
//  returns:  None
//
void STM32_GPIO_Init(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode, uint32_t Pull, uint32_t Speed, uint32_t Alternate, uint32_t OutputState) {

    uint32_t      Tmp;
    uint32_t      Index;

    uint32_t BitPosition = CalcBitPosition(Pin);     // 0 to 15(error)

    if ((Mode == GPIO_MODE_OUTPUT_PP) || (Mode == GPIO_MODE_AF_PP) || (Mode == GPIO_MODE_OUTPUT_OD) || (Mode == GPIO_MODE_AF_OD)) {
        Tmp = GPIOx->OSPEEDR;
        Tmp &= ~(GPIO_OSPEEDR_OSPEEDR0 << (BitPosition * 2));     // Clear old bits
        Tmp |= (Speed << (BitPosition * 2));            // Set new bits
        GPIOx->OSPEEDR = Tmp;

        Tmp = GPIOx->OTYPER;
        Tmp &= ~(GPIO_OTYPER_OT_0 << BitPosition);
        Tmp |= (((Mode & GPIO_OUTPUT_TYPE) >> 4) << BitPosition);
        GPIOx->OTYPER = Tmp;

        STM32_GPIO_WritePin(GPIOx, Pin, OutputState);
    }

    /* Activate the Pull-up or Pull down resistor for the current IO */
    Tmp = GPIOx->PUPDR;
    Tmp &= ~(GPIO_PUPDR_PUPDR0 << (BitPosition * 2u));
    Tmp |= ((Pull) << (BitPosition * 2u));
    GPIOx->PUPDR = Tmp;

    if ((Mode == GPIO_MODE_AF_PP) || (Mode == GPIO_MODE_AF_OD)) {
        Tmp = GPIOx->AFR[BitPosition >> 3u];
        Tmp &= ~(0xFu << ((BitPosition & 0x07u) * 4u));
        Tmp |= Alternate << ((BitPosition & 0x07u) * 4u);
        GPIOx->AFR[BitPosition >> 3u] = Tmp;
    }

    /* Configure IO Direction mode (Input, Output, Alternate or Analog) */
    Tmp = GPIOx->MODER;
    Tmp &= ~(GPIO_MODER_MODER0 << (BitPosition * 2u));
    Tmp |= ((Mode & GPIO_MODE) << (BitPosition * 2u));
    GPIOx->MODER = Tmp;

    if ((Mode & EXTI_MODE) == EXTI_MODE) {
        Tmp = SYSCFG->EXTICR[BitPosition >> 2u];
        Tmp &= ~(0x0FuL << (4u * (BitPosition & 0x03u)));
        if (GPIOx == GPIOA) {
            Index = 0;
        } else if (GPIOx == GPIOB) {
            Index = 1;
        } else {
            Error_Handler(FILE_GPIO, __LINE__);
        }
        Tmp |= (Index << (4u * (BitPosition & 0x03u)));
        SYSCFG->EXTICR[BitPosition >> 2u] = Tmp;

        /* Clear EXTI line configuration */
        Tmp = EXTI->IMR;
        Tmp &= ~(Pin);
        if ((Mode & GPIO_MODE_IT) == GPIO_MODE_IT) {
            Tmp |= Pin;
        }
        EXTI->IMR = Tmp;

        Tmp = EXTI->EMR;
        Tmp &= ~(Pin);
        EXTI->EMR = Tmp;

        /* Clear Rising Falling edge configuration */
        Tmp = EXTI->RTSR;
        Tmp &= ~(Pin);
        if ((Mode & RISING_EDGE) == RISING_EDGE) {
            Tmp |= Pin;
        }
        EXTI->RTSR = Tmp;

        Tmp = EXTI->FTSR;
        Tmp &= ~(Pin);
        EXTI->FTSR = Tmp;
    }
}

// ------------------------------------------------------------------------
//
//  STM32_GPIO_DeInit - This routine clears the initialization of a GPIO pin
//
//  Params:   GPIOx - Selects the GPIO peripheral where the GPIO pin is located on the MCU
//            GPIO_Pin - The GPIO pin number that be cleared
//
//  returns:  None
//
void STM32_GPIO_DeInit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin) {

    uint32_t Index, BitPosition, Tmp;

    BitPosition = CalcBitPosition(GPIO_Pin);
    Tmp = SYSCFG->EXTICR[BitPosition >> 2u];
    Tmp &= (0x0FuL << (4u * (BitPosition & 0x03u)));
    if (GPIOx == GPIOA) {
        Index = 0;
    } else if (GPIOx == GPIOB) {
        Index = 1;
    } else {
        Error_Handler(FILE_GPIO, __LINE__);
    }
    if (Tmp == (Index << (4u * (BitPosition & 0x03u)))) {
        EXTI->IMR &= ~GPIO_Pin;           /* Clear EXTI line configuration */
        EXTI->EMR &= ~GPIO_Pin;
        EXTI->RTSR &= ~GPIO_Pin;            /* Clear Rising Falling edge configuration */
        EXTI->FTSR &= ~GPIO_Pin;
        Tmp = 0x0FuL << (4u * (BitPosition & 0x03u));       /* Configure the External Interrupt or event for the current IO */
        SYSCFG->EXTICR[BitPosition >> 2u] &= ~Tmp;
    }

    /* Configure IO Direction in Input Floating Mode */
    GPIOx->MODER &= ~(GPIO_MODER_MODER0 << (BitPosition * 2u));

    /* Configure the default Alternate Function in current IO */
    GPIOx->AFR[BitPosition >> 3u] &= ~(0xFu << ((uint32_t)(BitPosition & 0x07u) * 4u));

    /* Deactivate the Pull-up and Pull-down resistor for the current IO */
    GPIOx->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (BitPosition * 2u));

    /* Configure the default value IO Output Type */
    GPIOx->OTYPER &= ~(GPIO_OTYPER_OT_0 << BitPosition);

    /* Configure the default value for IO Speed */
    GPIOx->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR0 << (BitPosition * 2u));
}

