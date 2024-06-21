// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Filename:      timer.c
// Author:        Rick Wright
// Revision:      C
//
// Description: This file contains STM32 TIMER support routines
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

// ------------------------------------------------------------------------
//
// STM32_TIM_Base_Init - Initializes the TIM time base unit
//
// Params:    TIMx - Pointer to the structure with configuration information for TIM module
//            Prescaler - Value of period divide by number of counts in the timer
//            Period - The time of one pulse for frequency output
//            AutoReloadPreload - bit for time to use the internal comparator latch (1) or delay to the next update event (0)
//
// Returns:   None
//
// Requirement(s):
//
void STM32_TIM_Base_Init(TIM_TypeDef *TIMx, uint32_t Prescaler, uint32_t Period, uint32_t AutoReloadPreload) {

    uint32_t TmpControlReg1;

    TmpControlReg1 = TIMx->CR1;
    if ((TIMx == TIM1) || (TIMx == TIM2) || (TIMx == TIM3)) {
        TmpControlReg1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
        TmpControlReg1 |= TIM_COUNTERMODE_UP;
    }
    if ((TIMx == TIM1) || (TIMx == TIM2) || (TIMx == TIM3) || (TIMx == TIM14) || (TIMx == TIM15) ||
        (TIMx == TIM16) || (TIMx == TIM17)) {
        TmpControlReg1 &= ~TIM_CR1_CKD;
        TmpControlReg1 |= TIM_CLOCKDIVISION_DIV1;
    }
    TIMx->CR1 = (TmpControlReg1 & ~TIM_CR1_ARPE) | AutoReloadPreload;
    TIMx->ARR = Period;
    TIMx->PSC = Prescaler;
    if ((TIMx == TIM1) || (TIMx == TIM15) || (TIMx == TIM16) || (TIMx == TIM17)) {
        TIMx->RCR = 0;
    }
    TIMx->EGR = TIM_EGR_UG;
}

// ------------------------------------------------------------------------
//
// STM32_TIM_OC1_SetConfig - Sets the timer output compare 1 module configuration
//
// Params:    TIMx - TIM peripheral to be configured
//            OC_Config - Output configuration structure
//
// Returns:   None
//
// Requirement(s):
//
static void STM32_TIM_OC1_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config) {

    uint32_t TmpCaptureCompareModeRegx, TmpCaptureCompareEnableReg, TmpControlReg2;

    TIMx->CCER &= ~TIM_CCER_CC1E;
    TmpCaptureCompareEnableReg = TIMx->CCER;
    TmpControlReg2 = TIMx->CR2;
    TmpCaptureCompareModeRegx = TIMx->CCMR1;
    TmpCaptureCompareModeRegx &= ~TIM_CCMR1_OC1M;
    TmpCaptureCompareModeRegx &= ~TIM_CCMR1_CC1S;
    TmpCaptureCompareModeRegx |= OC_Config->OCMode;
    TmpCaptureCompareEnableReg &= ~TIM_CCER_CC1P;
    TmpCaptureCompareEnableReg |= OC_Config->OCPolarity;

    if ((TIMx == TIM1) || (TIMx == TIM15) || (TIMx == TIM16) || (TIMx == TIM17)) {
        TmpCaptureCompareEnableReg &= ~TIM_CCER_CC1NP;
        TmpCaptureCompareEnableReg |= OC_Config->OCNPolarity;
        TmpCaptureCompareEnableReg &= ~TIM_CCER_CC1NE;

        TmpControlReg2 &= ~TIM_CR2_OIS1;
        TmpControlReg2 &= ~TIM_CR2_OIS1N;
        TmpControlReg2 |= OC_Config->OCIdleState;
        TmpControlReg2 |= OC_Config->OCNIdleState;
    }

    TIMx->CR2 = TmpControlReg2;
    TIMx->CCMR1 = TmpCaptureCompareModeRegx;
    TIMx->CCR1 = OC_Config->Pulse;
    TIMx->CCER = TmpCaptureCompareEnableReg;
}

// ------------------------------------------------------------------------
//
// STM32_TIM_OC2_SetConfig - Sets the timer output compare 2 module configuration
//
// Params:    TIMx - TIM peripheral to be configured
//            OC_Config - Output configuration structure
//
// Returns:   None
//
// Requirement(s):
//
static void STM32_TIM_OC2_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config) {

    uint32_t TmpCaptureCompareModeRegx = 0u;
    uint32_t TmpCaptureCompareEnableReg = 0u;
    uint32_t TmpControlReg2 = 0u;

    TIMx->CCER &= ~TIM_CCER_CC2E;

    TmpCaptureCompareEnableReg = TIMx->CCER;
    TmpControlReg2 = TIMx->CR2;

    TmpCaptureCompareModeRegx = TIMx->CCMR1;
    TmpCaptureCompareModeRegx &= ~TIM_CCMR1_OC2M;
    TmpCaptureCompareModeRegx &= ~TIM_CCMR1_CC2S;
    TmpCaptureCompareModeRegx |= (OC_Config->OCMode << 8u);

    TmpCaptureCompareEnableReg &= ~TIM_CCER_CC2P;
    TmpCaptureCompareEnableReg |= (OC_Config->OCPolarity << 4u);

    if ((TIMx == TIM1) || (TIMx == TIM15) || (TIMx == TIM16) || (TIMx == TIM17)) {
        TmpControlReg2 &= ~TIM_CR2_OIS2;
        TmpControlReg2 &= ~TIM_CR2_OIS2N;
        TmpControlReg2 |= (OC_Config->OCIdleState << 2u);
        TmpControlReg2 |= (OC_Config->OCNIdleState << 2u);
    }
    TIMx->CR2 = TmpControlReg2;
    TIMx->CCMR1 = TmpCaptureCompareModeRegx;
    TIMx->CCR2 = OC_Config->Pulse;
    TIMx->CCER = TmpCaptureCompareEnableReg;
}

// ------------------------------------------------------------------------
//
// STM32_TIM_PWM_ConfigChannel - Initializes the TIM PWM channels
//
// Params:    TIMx - Pointer to a TIM structure containing the configuration information for the TIM module
//            sConfig - TIM PWM configuration structure
//            Channel - TIM channels to be enabled (1, 2, 3 or 4)
//
// Returns:   None
//
// Requirement(s):
//
void STM32_TIM_PWM_ConfigChannel(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *sConfig, uint32_t Channel) {

    switch (Channel) {
    case TIM_CHANNEL_1:
        STM32_TIM_OC1_SetConfig(TIMx, sConfig);
        TIMx->CCMR1 |= TIM_CCMR1_OC1PE;
        TIMx->CCMR1 &= ~TIM_CCMR1_OC1FE;
        TIMx->CCMR1 |= sConfig->OCFastMode;
        break;

    case TIM_CHANNEL_2:
        STM32_TIM_OC2_SetConfig(TIMx, sConfig);
        TIMx->CCMR1 |= TIM_CCMR1_OC2PE;
        TIMx->CCMR1 &= ~TIM_CCMR1_OC2FE;
        TIMx->CCMR1 |= sConfig->OCFastMode << 8u;
        break;
    default:
        Error_Handler(FILE_TIMER, __LINE__);
        break;
    }
}

// ------------------------------------------------------------------------
//
// STM32_TIM_MspPostInit - Initiates the timmer for PWM output pins
//
// Params:    TIMx - Pointer to a TIM structure containing the configuration information for the TIM module
//
// Returns:   None
//
// Requirement(s):
//
void STM32_TIM_MspPostInit(TIM_TypeDef *TIMx) {

    if (TIMx == TIM1) {
        STM32_GPIO_Init(DIG_IO6_GPIO_Port, DIG_IO6_Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF2_TIM1, 0);
    } else if (TIMx == TIM3) {
        STM32_GPIO_Init(DIG_IO4_GPIO_Port, DIG_IO4_Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF1_TIM3, 0);
    } else if (TIMx == TIM15) {
        STM32_GPIO_Init(DIG_IO1_GPIO_Port, DIG_IO1_Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF0_TIM15, 0);
    } else if (TIMx == TIM16) {
        STM32_GPIO_Init(DIG_IO2_GPIO_Port, DIG_IO2_Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF2_TIM16, 0);
    } else if (TIMx == TIM17) {
        STM32_GPIO_Init(DIG_IO3_GPIO_Port, DIG_IO3_Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF2_TIM17, 0);
    } else {
        Error_Handler(FILE_TIMER, __LINE__);
    }
}

// ------------------------------------------------------------------------
//
// STM32_TIM_PWM_Start - Routin starts the PWM TIM peripheral
//
// Params:    TIMx - Pointer to a TIM structure containing the configuration information for the TIM module
//            Channel - TIM channels to be enabled (1, 2, 3 or 4)
//
// Returns:   None
//
// Requirement(s):
//
void STM32_TIM_PWM_Start(TIM_TypeDef *TIMx, uint32_t Channel) {

    uint32_t Tmp;

    Tmp = TIM_CCER_CC1E << Channel;
    TIMx->CCER &= ~Tmp;
    TIMx->CCER |= TIM_CCx_ENABLE << Channel;

    if ((TIMx == TIM1) || (TIMx == TIM15) || (TIMx == TIM16) || (TIMx == TIM17)) {
        TIMx->BDTR |= TIM_BDTR_MOE;
    }
    TIMx->CR1 |= TIM_CR1_CEN;
}

// ------------------------------------------------------------------------
//
// STM32_TIM_Base_DeInit - Routine to reset the TIM peripheral
//
// Params:    TIMx - Pointer to a TIM structure containing the configuration information for the TIM module
//
// Returns:   None
//
// Requirement(s):
//
void STM32_TIM_Base_DeInit(TIM_TypeDef *TIMx) {

    if ((TIMx->CCER & TIM_CCER_CCxE_MASK) == 0) {
        if ((TIMx->CCER & TIM_CCER_CCxNE_MASK) == 0) {
            TIMx->CR1 &= ~TIM_CR1_CEN;
        }
    }
}

// ------------------------------------------------------------------------
//
// STM32_TIM_Base_Start - Routine to start the TIM generation
//
// Params:    TIMx - Pointer to a TIM structure containing the configuration information for the TIM module
//
// Returns:   None
//
// Requirement(s):
//
void STM32_TIM_Base_Start(TIM_TypeDef *TIMx) {

    TIMx->CR1 |= TIM_CR1_CEN;
}

// ------------------------------------------------------------------------
//
// MX_TIM2_Init - Routine to initialize the TIM2 peripheral using the internal comparator latch on call
//                given Timer2 is used for frequency input capturing
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void MX_TIM2_Init(void) {
    STM32_TIM_Base_Init(TIM2, 3 - 1, 0xFFFFFFFF, TIM_AUTORELOAD_PRELOAD_ENABLE);
}

