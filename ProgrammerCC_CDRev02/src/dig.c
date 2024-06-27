//
// ******************************************************************************
//
// Description:     Digital I/O routines
//
// Release History:
//
//  2018-10-12  RLW     Initial Release
//
// ******************************************************************************

#include "global.h"

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// Ch:  0 to 5
//
// HLR-xxxx
// LLR-xxxx
//
int32_t DIG_Init(int32_t Ch, DIG_InitTypeDef DIG_InitStruct) {

  GPIO_InitTypeDef    GPIO_InitStruct;
  TIM_OC_InitTypeDef  sConfigOC;
  int32_t             MinPeriod;

  if (Ch == 0) {
      HAL_NVIC_DisableIRQ(USART2_IRQn);
  }

  switch ( Ch ) {
  case 0:
    DIG_HandleStruct[Ch].Handle = &htim15;
    DIG_HandleStruct[Ch].Handle->Instance = TIM15;
    DIG_HandleStruct[Ch].TIM_Channel = TIM_CHANNEL_2;
    DIG_HandleStruct[Ch].Dir_Port = DIR1_GPIO_Port;
    DIG_HandleStruct[Ch].Dir_Pin = DIR1_Pin;
    DIG_HandleStruct[Ch].Dig_IO_Port = DIG_IO1_GPIO_Port;
    DIG_HandleStruct[Ch].Dig_IO_Pin = DIG_IO1_Pin;
    break;
  case 1:
    DIG_HandleStruct[Ch].Handle = &htim16;
    DIG_HandleStruct[Ch].Handle->Instance = TIM16;
    DIG_HandleStruct[Ch].TIM_Channel = TIM_CHANNEL_1;
    DIG_HandleStruct[Ch].Dir_Port = DIR2_GPIO_Port;
    DIG_HandleStruct[Ch].Dir_Pin = DIR2_Pin;
    DIG_HandleStruct[Ch].Dig_IO_Port = DIG_IO2_GPIO_Port;
    DIG_HandleStruct[Ch].Dig_IO_Pin = DIG_IO2_Pin;
    break;
  case 2:
    DIG_HandleStruct[Ch].Handle = &htim17;
    DIG_HandleStruct[Ch].Handle->Instance = TIM17;
    DIG_HandleStruct[Ch].TIM_Channel = TIM_CHANNEL_1;
    DIG_HandleStruct[Ch].Dir_Port = DIR3_GPIO_Port;
    DIG_HandleStruct[Ch].Dir_Pin = DIR3_Pin;
    DIG_HandleStruct[Ch].Dig_IO_Port = DIG_IO3_GPIO_Port;
    DIG_HandleStruct[Ch].Dig_IO_Pin = DIG_IO3_Pin;
    break;
  case 3:
    DIG_HandleStruct[Ch].Handle = &htim3;
    DIG_HandleStruct[Ch].Handle->Instance = TIM3;
    DIG_HandleStruct[Ch].TIM_Channel = TIM_CHANNEL_4;
    DIG_HandleStruct[Ch].Dir_Port = DIR4_GPIO_Port;
    DIG_HandleStruct[Ch].Dir_Pin = DIR4_Pin;
    DIG_HandleStruct[Ch].Dig_IO_Port = DIG_IO4_GPIO_Port;
    DIG_HandleStruct[Ch].Dig_IO_Pin = DIG_IO4_Pin;
    break;
  case 4:
    DIG_HandleStruct[Ch].Handle = &htim2;
    DIG_HandleStruct[Ch].Handle->Instance = TIM2;
    DIG_HandleStruct[Ch].TIM_Channel = TIM_CHANNEL_3;
    DIG_HandleStruct[Ch].Dir_Port = DIR5_GPIO_Port;
    DIG_HandleStruct[Ch].Dir_Pin = DIR5_Pin;
    DIG_HandleStruct[Ch].Dig_IO_Port = DIG_IO5_GPIO_Port;
    DIG_HandleStruct[Ch].Dig_IO_Pin = DIG_IO5_Pin;
    break;
  case 5:
    DIG_HandleStruct[Ch].Handle = &htim1;
    DIG_HandleStruct[Ch].Handle->Instance = TIM1;
    DIG_HandleStruct[Ch].TIM_Channel = TIM_CHANNEL_1;
    DIG_HandleStruct[Ch].Dir_Port = DIR6_GPIO_Port;
    DIG_HandleStruct[Ch].Dir_Pin = DIR6_Pin;
    DIG_HandleStruct[Ch].Dig_IO_Port = DIG_IO6_GPIO_Port;
    DIG_HandleStruct[Ch].Dig_IO_Pin = DIG_IO6_Pin;
    break;
  default:
    return -1;         // ERROR  (Invalid Ch Number)
    break;
  }

  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pin = DIG_HandleStruct[Ch].Dig_IO_Pin;
  HAL_GPIO_Init(DIG_HandleStruct[Ch].Dig_IO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(DIG_HandleStruct[Ch].Dir_Port, DIG_HandleStruct[Ch].Dir_Pin, GPIO_PIN_RESET);
  DIG_HandleStruct[Ch].DIG_InitStruct = DIG_InitStruct;

  switch ( DIG_HandleStruct[Ch].DIG_InitStruct.Mode ) {

  case DIG_IN:
    return 0;
    break;

  case DIG_OUT:
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin = DIG_HandleStruct[Ch].Dig_IO_Pin;
    HAL_GPIO_WritePin(DIG_HandleStruct[Ch].Dir_Port, DIG_HandleStruct[Ch].Dir_Pin, GPIO_PIN_SET);
    HAL_GPIO_Init(DIG_HandleStruct[Ch].Dig_IO_Port, &GPIO_InitStruct);
    return 0;
    break;

  case DIG_SERIAL_IN:
  case DIG_SERIAL_INOUT:
    //
    //  This Mode is only valid on DIG_IO1
    //
    if ( Ch == 0 ) {
      //
      // Assign PA3 as USART2_RX and enable Interrupts
      //
      GPIO_InitStruct.Pin = DIG_HandleStruct[Ch].Dig_IO_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_PULLUP;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
      HAL_GPIO_Init(DIG_HandleStruct[Ch].Dig_IO_Port, &GPIO_InitStruct);
      HAL_UART_Receive_IT(&huart2, &SerialUsart2.RxChar, 1);
      HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
    return 0;
    break;

  case DIG_SERIAL_OUT:
    //
    //  DIG_SERIAL_OUT is only valid on DIG_IO2
    //
    if ( Ch == 1 ) {
      //
      // NOTE:  The DIG_IO2 port pin (PB8) must be set as an input
      //          the Buffer is then set as an output.  The DIG_IO2
      //          signal wire is then driven by the USART2_TX (PA2)
      //          signal through a 4.7K resistor (R6).
      //
      GPIO_InitStruct.Pin = DIG_HandleStruct[Ch].Dig_IO_Pin;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      HAL_GPIO_Init(DIG_HandleStruct[Ch].Dig_IO_Port, &GPIO_InitStruct);
      //
      // Finally, configure Buffer as an output
      //
      HAL_GPIO_WritePin(DIG_HandleStruct[Ch].Dir_Port, DIG_HandleStruct[Ch].Dir_Pin, GPIO_PIN_SET);        // Set as an output
    }
    return 0;
    break;

  case DIG_FREQ_IN:

     if (Ch == 5) {
         return -1;
     }
     //
     // Setup Port pin to generate an interrupt on Rising Edges
     //
    GPIO_InitStruct.Pin = DIG_HandleStruct[Ch].Dig_IO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DIG_HandleStruct[Ch].Dig_IO_Port, &GPIO_InitStruct);

    switch ( Ch ) {
    case 0:
      HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);               // GPIO_PIN_3
      HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
      break;
    case 1:
      HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);               // GPIO_PIN_8
      HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
      break;
    case 2:
      HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);               // GPIO_PIN_9
      HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
      break;
    case 3:
      HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);               // GPIO_PIN_4
      HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
      break;
    case 4:
      HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);               // GPIO_PIN_10
      HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
      break;
    case 5:
      break;
    }
    return 0;
    break;

  case DIG_FREQ_OUT:
    //
    // Frequency must be > 0 and less than 200KHz
    // MinFrequency must be >= 0 and less than Frequency
    //

    switch ( Ch ) {
    case 0:
      __HAL_RCC_TIM15_CLK_ENABLE();
      break;
    case 1:
      __HAL_RCC_TIM16_CLK_ENABLE();
      break;
    case 2:
      __HAL_RCC_TIM17_CLK_ENABLE();
      break;
    case 3:
      __HAL_RCC_TIM3_CLK_ENABLE();
      break;
    case 4:
      __HAL_RCC_TIM2_CLK_ENABLE();
      break;
    case 5:
      __HAL_RCC_TIM1_CLK_ENABLE();
      break;
    }
    int32_t Freq = DIG_HandleStruct[Ch].DIG_InitStruct.Frequency;
    if ( (Freq <= 0) || (Freq > 20000000) ) {
      return -1;
    }
    //
    // Calculate a Prescaler value
    //
    MinPeriod = 2400000000 / Freq;
    DIG_HandleStruct[Ch].Prescaler = MinPeriod / 65000;
    DIG_HandleStruct[Ch].Period = (2400000000 / (DIG_HandleStruct[Ch].Prescaler + 1)) / DIG_HandleStruct[Ch].DIG_InitStruct.Frequency;
    //
    // Set Buffer Direction to Output
    //
    HAL_GPIO_WritePin(DIG_HandleStruct[Ch].Dir_Port, DIG_HandleStruct[Ch].Dir_Pin, GPIO_PIN_SET);
    //
    // Initialize Timer Base
    //
    DIG_HandleStruct[Ch].Handle->Init.Prescaler         = DIG_HandleStruct[Ch].Prescaler;
    DIG_HandleStruct[Ch].Handle->Init.CounterMode       = TIM_COUNTERMODE_DOWN;
    DIG_HandleStruct[Ch].Handle->Init.Period            = DIG_HandleStruct[Ch].Period - 1;
    DIG_HandleStruct[Ch].Handle->Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    DIG_HandleStruct[Ch].Handle->Init.RepetitionCounter = 0;
    DIG_HandleStruct[Ch].Handle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if ( HAL_TIM_Base_Init(DIG_HandleStruct[Ch].Handle) != HAL_OK ) {
      return -1;
    }
    //
    // Initialize Timer Channel
    //
    sConfigOC.OCMode        = TIM_OCMODE_TOGGLE;
    sConfigOC.Pulse         = 0;
    sConfigOC.OCPolarity    = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity   = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode    = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState   = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState  = TIM_OCNIDLESTATE_RESET;
    if ( HAL_TIM_PWM_ConfigChannel(DIG_HandleStruct[Ch].Handle, &sConfigOC, DIG_HandleStruct[Ch].TIM_Channel) != HAL_OK ) {
      return -1;
    }
    HAL_TIM_MspPostInit(DIG_HandleStruct[Ch].Handle);
    if ( HAL_TIM_PWM_Start(DIG_HandleStruct[Ch].Handle, DIG_HandleStruct[Ch].TIM_Channel) != HAL_OK ) {
      return -1;
    }
    DIG_HandleStruct[Ch].bTimerUsed = 1;
    return 0;
    break;

  case DIG_PWM_OUT:
    //
    // Freq must be > 0 and less than 200KHz
    //
    if ( (DIG_HandleStruct[Ch].DIG_InitStruct.Frequency <= 0) || (DIG_HandleStruct[Ch].DIG_InitStruct.Frequency > 20000000) ) {
      return -1;
    }
    switch ( Ch ) {
    case 0:
      __HAL_RCC_TIM15_CLK_ENABLE();
      break;
    case 1:
      __HAL_RCC_TIM16_CLK_ENABLE();
      break;
    case 2:
      __HAL_RCC_TIM17_CLK_ENABLE();
      break;
    case 3:
      __HAL_RCC_TIM3_CLK_ENABLE();
      break;
    case 4:
      __HAL_RCC_TIM2_CLK_ENABLE();
      break;
    case 5:
      __HAL_RCC_TIM1_CLK_ENABLE();
      break;
    }

    MinPeriod = 4800000000 / DIG_HandleStruct[Ch].DIG_InitStruct.Frequency;
    DIG_HandleStruct[Ch].Prescaler = MinPeriod / 65000;
    DIG_HandleStruct[Ch].Period = (4800000000 / (DIG_HandleStruct[Ch].Prescaler + 1)) / DIG_HandleStruct[Ch].DIG_InitStruct.Frequency;
    DIG_HandleStruct[Ch].Pulse = (DIG_HandleStruct[Ch].Period * DIG_HandleStruct[Ch].DIG_InitStruct.DutyCycle) / 10000;
    //
    // Set Buffer Direction to Output
    //
    HAL_GPIO_WritePin(DIG_HandleStruct[Ch].Dir_Port, DIG_HandleStruct[Ch].Dir_Pin, GPIO_PIN_SET);
    //
    // Initialize Timer Base
    //
    DIG_HandleStruct[Ch].Handle->Init.Prescaler = DIG_HandleStruct[Ch].Prescaler;
    DIG_HandleStruct[Ch].Handle->Init.CounterMode = TIM_COUNTERMODE_UP;
    DIG_HandleStruct[Ch].Handle->Init.Period = DIG_HandleStruct[Ch].Period - 1;
    DIG_HandleStruct[Ch].Handle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    DIG_HandleStruct[Ch].Handle->Init.RepetitionCounter = 0;
    DIG_HandleStruct[Ch].Handle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if ( HAL_TIM_Base_Init(DIG_HandleStruct[Ch].Handle) != HAL_OK ) {
      return -1;
    }
    //
    // Initialize Timer Channel
    //
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = DIG_HandleStruct[Ch].Pulse;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if ( HAL_TIM_PWM_ConfigChannel(DIG_HandleStruct[Ch].Handle, &sConfigOC, DIG_HandleStruct[Ch].TIM_Channel) != HAL_OK ) {
      return -1;
    }
    HAL_TIM_MspPostInit(DIG_HandleStruct[Ch].Handle);
    if ( HAL_TIM_PWM_Start(DIG_HandleStruct[Ch].Handle, DIG_HandleStruct[Ch].TIM_Channel) != HAL_OK ) {
      return -1;
    }
    DIG_HandleStruct[Ch].bTimerUsed = 1;
    return 0;
    break;

  default:
    return -1;
  }
}

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// NewFreq - xxx.xx Hz
//
// HLR-xxxx
// LLR-xxxx
//
int32_t DIG_SetFrequency(int32_t Ch, int32_t NewFreq) {

  if ( DIG_HandleStruct[Ch].DIG_InitStruct.Mode != DIG_FREQ_OUT ) {           // Function is only valid if Channel is set to DIG_FREQ_OUT
    return -1;
  }
//
// Verify NewFreq is within Range
//
  if ( (NewFreq == 0) || (NewFreq > 20000000) ) {
    return -1;
  }
  DIG_HandleStruct[Ch].DIG_InitStruct.Frequency = NewFreq;
  int32_t MinPeriod = 2400000000 / DIG_HandleStruct[Ch].DIG_InitStruct.Frequency;
  DIG_HandleStruct[Ch].Prescaler = MinPeriod / 65000;
  DIG_HandleStruct[Ch].Period = (2400000000 / (DIG_HandleStruct[Ch].Prescaler + 1)) / DIG_HandleStruct[Ch].DIG_InitStruct.Frequency;
  if ( DIG_HandleStruct[Ch].Handle->Instance->PSC != DIG_HandleStruct[Ch].Prescaler ) {
    DIG_HandleStruct[Ch].Handle->Instance->PSC = DIG_HandleStruct[Ch].Prescaler;
  }
  DIG_HandleStruct[Ch].Handle->Instance->ARR = DIG_HandleStruct[Ch].Period - 1;
  return 0;
}

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// Freq - xxx.xx Hz
//
// HLR-xxxx
// LLR-xxxx
//
int32_t DIG_SetDutyCycle(int32_t Ch, int32_t NewDutyCycle) {

  if ( DIG_HandleStruct[Ch].DIG_InitStruct.Mode != DIG_PWM_OUT ) {           // Function is only valid if Channel is set to DIG_PWM_OUT
    return -1;
  }
  DIG_HandleStruct[Ch].DIG_InitStruct.DutyCycle = NewDutyCycle;
  DIG_HandleStruct[Ch].Pulse = (DIG_HandleStruct[Ch].Period * DIG_HandleStruct[Ch].DIG_InitStruct.DutyCycle) / 10000;
  DIG_HandleStruct[Ch].Handle->Instance->CCR1 = DIG_HandleStruct[Ch].Pulse;
  return 0;
}

//
// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// HLR-xxxx
// LLR-xxxx
//
int32_t DIG_SetPin(int32_t Ch) {

  if ( DIG_HandleStruct[Ch].DIG_InitStruct.Mode != DIG_OUT ) {           // Function is only valid if Channel is set to DIG_OUT
    return -1;
  }
  HAL_GPIO_WritePin(DIG_HandleStruct[Ch].Dig_IO_Port, DIG_HandleStruct[Ch].Dig_IO_Pin, GPIO_PIN_SET);
  return 0;
}

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// HLR-xxxx
// LLR-xxxx
//
int32_t DIG_ResetPin(int32_t Ch) {

  if ( DIG_HandleStruct[Ch].DIG_InitStruct.Mode != DIG_OUT ) {           // Function is only valid if Channel is set to DIG_OUT
    return -1;
  }
  HAL_GPIO_WritePin(DIG_HandleStruct[Ch].Dig_IO_Port, DIG_HandleStruct[Ch].Dig_IO_Pin, GPIO_PIN_RESET);
  return 0;
}

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// HLR-xxxx
// LLR-xxxx
//
int32_t DIG_ReadPin(int32_t Ch) {

  return HAL_GPIO_ReadPin(DIG_HandleStruct[Ch].Dig_IO_Port, DIG_HandleStruct[Ch].Dig_IO_Pin);
}

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// HLR-xxxx
// LLR-xxxx
//
void InitializeDigitalIO(int8_t Mode) {

    DIG_InitTypeDef   DIG_InitStruct;

  DIG_InitStruct.Mode = DIG_IN;
  DIG_Init(0, DIG_InitStruct);
  DIG_Init(1, DIG_InitStruct);
  DIG_Init(2, DIG_InitStruct);
  DIG_Init(3, DIG_InitStruct);
  DIG_Init(4, DIG_InitStruct);
  DIG_Init(5, DIG_InitStruct);

}
