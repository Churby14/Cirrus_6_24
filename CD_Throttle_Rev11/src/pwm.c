// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: pwm.c
// Author  : Rick Wright
// Revision: C
//
// Description:     Initialization of PWM subsystem
//
// ------------------------------------------------------------------------

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
// PWM_Init - Initializes the PWM pin to be used to control the voltage/resistive output
//
// Params:    PWM_InitStruct - Pulse width modulation configuration structure
//
// Returns:   None
//
// Requirement(s):
//
static void PWM_Init(PWM_InitTypeDef PWM_InitStruct) {

    TIM_OC_InitTypeDef sConfigOC;

    PWM_HandleStruct.Handle = &htim14;
    PWM_HandleStruct.Handle->Instance = TIM14;
    PWM_HandleStruct.TIM_Channel = TIM_CHANNEL_1;
    PWM_HandleStruct.PWM_InitStruct = PWM_InitStruct;
    //
    //
    //
    PWM_HandleStruct.Prescaler = (uint32_t)((4800000000LL / PWM_HandleStruct.PWM_InitStruct.Frequency) / 65000);
    PWM_HandleStruct.Period = (uint32_t)((4800000000LL / (PWM_HandleStruct.Prescaler + 1)) / PWM_HandleStruct.PWM_InitStruct.Frequency);
    PWM_HandleStruct.Pulse = MultDivideWithRounding64(PWM_HandleStruct.Period, PWM_HandleStruct.PWM_InitStruct.DutyCycle, PWM_MAX_DUTY_CYCLE, RESULT_32);
    //
    // Initialize Timer Base
    //
    STM32_TIM_Base_Init(TIM14, PWM_HandleStruct.Prescaler, PWM_HandleStruct.Period - 1, TIM_AUTORELOAD_PRELOAD_DISABLE);
    //
    // Initialize Timer Channel
    //
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = PWM_HandleStruct.Pulse;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    STM32_TIM_PWM_ConfigChannel(TIM14, &sConfigOC, PWM_HandleStruct.TIM_Channel);

    STM32_GPIO_Init(PWM_OUTPUT_GPIO_Port, PWM_OUTPUT_Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF4_TIM14, 0);

    STM32_TIM_PWM_Start(TIM14, PWM_HandleStruct.TIM_Channel);
}

// ------------------------------------------------------------------------
//
// PWM_SetDutyCycle - Sets the duty cycle of the PWM signal controlling the R/V output
//                    based on PID feedback loop controlling the R/V output
//
// Params:    NewDutyCycle - Calculated duty cycle based on PID feedback loop controlling the
//                           R/V output to ensure the new duty cycle matches the intended R/V output
//
// Returns:   None
//
// Requirement(s):
//
void PWM_SetDutyCycle(int32_t NewDutyCycle) {

    if ((bFactoryConfigurationValid != 0) && (FactoryConfiguration->RVOutputPresent != 0)) {
        PWM_HandleStruct.PWM_InitStruct.DutyCycle = NewDutyCycle;
        PWM_HandleStruct.Pulse = MultDivideWithRounding64(PWM_HandleStruct.Period, PWM_HandleStruct.PWM_InitStruct.DutyCycle, PWM_MAX_DUTY_CYCLE, RESULT_32);
        PWM_HandleStruct.Handle->Instance->CCR1 = PWM_HandleStruct.Pulse;
        //
        // On rising edge of DutyCycle == 0, then configure GPIO as an Low Output
        //
        if ((PWM_HandleStruct.Pulse == 0) && (PWM_HandleStruct.OldPulse != 0)) {
            STM32_GPIO_Init(PWM_OUTPUT_GPIO_Port, PWM_OUTPUT_Pin, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0, 0);
        }
        //
        // On falling edge of DutyCycle == 0, then configure GPIO as a Timer Output
        //
        if ((PWM_HandleStruct.Pulse != 0) && (PWM_HandleStruct.OldPulse == 0)) {
            STM32_GPIO_Init(PWM_OUTPUT_GPIO_Port, PWM_OUTPUT_Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF4_TIM14, 0);
        }

        PWM_HandleStruct.OldPulse = PWM_HandleStruct.Pulse;
    }
}

// ------------------------------------------------------------------------
//
// InitializePWM - This routine is called once during power-up before main loop
//                 to initialize the pwm pin if R/V output is being used
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void InitializePWM(void) {

    PWM_InitTypeDef PWM_InitStruct;

    if ((bFactoryConfigurationValid != 0) && (FactoryConfiguration->RVOutputPresent != 0)) {
        SysVars.PwmDutyCycle = 1;                               // xxx.xxx %
        PWM_InitStruct.Frequency = (uint32_t)(4800000000uLL / PWM_COUNTS);     // 40000 = 1200.00Hz
        PWM_InitStruct.DutyCycle = SysVars.PwmDutyCycle;        // xxx.xxx %
        PWM_Init(PWM_InitStruct);
    }
}

// ------------------------------------------------------------------------
//
// ProcessPwmOutput - Ensures the PWM is within range and then sets the PWM output
//                    if R/V output is being used. Called every 10 ms
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void ProcessPwmOutput(void) {

    ExecutionStart(EXECUTION_PROCESS_PWM_OUT);

    if ((bFactoryConfigurationValid != 0) && (FactoryConfiguration->RVOutputPresent != 0)) {
        if (SysVars.PwmDutyCycle < 0) {
            SysVars.PwmDutyCycle = 0;
        }
        if (SysVars.PwmDutyCycle > PWM_MAX_DUTY_CYCLE) {
            SysVars.PwmDutyCycle = PWM_MAX_DUTY_CYCLE;
        }
        PWM_SetDutyCycle(SysVars.PwmDutyCycle);
    }
    ExecutionEnd(EXECUTION_PROCESS_PWM_OUT);
}
