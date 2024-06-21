// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: dig.c
// Author  : Rick Wright/Charlie Urban
// Revision: 3.2
//
// Description:     Digital I/O routines
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
//
// Design Details and Explanations:
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Implementation Notes:
//
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
//  DIG_Init - This routine is used to initialize a Digital I/O output
//
//  Params:   Ch - Channel Number, 0 to 5, to be initialized
//           DIG_InitStruct - A DIG_InitTypeDef structure containing the initialization information
//
//  returns:  none
//
void DIG_Init(int32_t Ch, DIG_InitTypeDef DIG_InitStruct) {

  TIM_OC_InitTypeDef  sConfigOC;
  uint32_t             MinPeriod;
  //
  // Setup DIG_HandleStruct data structure based on selected Ch (0 to 5)
  //
  switch (Ch) {
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
    DIG_HandleStruct[Ch].TIM_Channel = TIM_CHANNEL_1;
    DIG_HandleStruct[Ch].Dir_Port = DIR4_GPIO_Port;
    DIG_HandleStruct[Ch].Dir_Pin = DIR4_Pin;
    DIG_HandleStruct[Ch].Dig_IO_Port = DIG_IO4_GPIO_Port;
    DIG_HandleStruct[Ch].Dig_IO_Pin = DIG_IO4_Pin;
    break;
  case 4:
    DIG_HandleStruct[Ch].Handle = NULL;               // Digital I/O #5 has no timer availiable
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
    Error_Handler(FILE_DIG, __LINE__);
    break;
  }
  //
  // Save InitStruct
  //
  DIG_HandleStruct[Ch].DIG_InitStruct = DIG_InitStruct;
  //
  // Initialize GPIO based on selected Mode
  //
  switch (DIG_HandleStruct[Ch].DIG_InitStruct.Mode) {
  case DIG_IN:
    //
    // Configure GPIO as an input without internal pull-up enabled
    // Configure Buffer Chip as an input also
    //
    STM32_GPIO_Init(DIG_HandleStruct[Ch].Dig_IO_Port, DIG_HandleStruct[Ch].Dig_IO_Pin, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, 0);
    STM32_GPIO_WritePin(DIG_HandleStruct[Ch].Dir_Port, DIG_HandleStruct[Ch].Dir_Pin, GPIO_PIN_RESET);
    break;

  case DIG_OUT:
    //
    //  Configure Buffer Chip as an output
    //  Configure GPIO as an output
    //
    STM32_GPIO_WritePin(DIG_HandleStruct[Ch].Dir_Port, DIG_HandleStruct[Ch].Dir_Pin, GPIO_PIN_SET);
    STM32_GPIO_Init(DIG_HandleStruct[Ch].Dig_IO_Port, DIG_HandleStruct[Ch].Dig_IO_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0, RESET);
    break;


  case DIG_SERIAL_INOUT:
    //
    //  This Mode is only valid on DIG_IO1
    //  Configure GPIO as a UART2 RX input
    //  Configure Buffer Chip as an input
    //
    MX_USART2_UART_Init();     // RX connected to DIGIO1, TX connected to DIGIO1 and DIGIO2
    STM32_GPIO_Init(DIG_HandleStruct[Ch].Dig_IO_Port, DIG_HandleStruct[Ch].Dig_IO_Pin, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF1_USART2, 0);
    STM32_GPIO_WritePin(DIG_HandleStruct[Ch].Dir_Port, DIG_HandleStruct[Ch].Dir_Pin, GPIO_PIN_RESET);
    break;

  case DIG_SERIAL_OUT:
    //
    //  DIG_SERIAL_OUT is only valid on DIG_IO2
    //
    // NOTE:  The DIG_IO2 port pin (PB8) must be set as an input.
    //          The Buffer Chip is then set as an output.  The DIG_IO2
    //          signal wire is then driven by the USART2_TX (PA2)
    //          signal through a 4.7K resistor (R6).
    //
    STM32_GPIO_Init(DIG_HandleStruct[Ch].Dig_IO_Port, DIG_HandleStruct[Ch].Dig_IO_Pin, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0, 0);
    //
    // Finally, configure Buffer (IC14) as an output
    //
    STM32_GPIO_WritePin(DIG_HandleStruct[Ch].Dir_Port, DIG_HandleStruct[Ch].Dir_Pin, GPIO_PIN_SET);        // Set as an output
    break;

  case DIG_FREQ_IN:
    //
    //  Configure GPIO as an input in Interrupt mode on Rising Edge
    //  Configure Buffer Chip as an input
    //
    STM32_GPIO_Init(DIG_HandleStruct[Ch].Dig_IO_Port, DIG_HandleStruct[Ch].Dig_IO_Pin, GPIO_MODE_IT_RISING, GPIO_NOPULL, 0, 0, 0);
    STM32_GPIO_WritePin(DIG_HandleStruct[Ch].Dir_Port, DIG_HandleStruct[Ch].Dir_Pin, GPIO_PIN_RESET);
    //
    // Now, enable the interrupt.  NOTE:  As both Digital I/O #2 and #6 share the same interrupt pin, only one of them can be configured as a Frequency
    // Input at any one time.
    //
    switch (Ch) {
    case 0:
      STM32_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);                 // GPIO_PIN_3
      STM32_NVIC_EnableIRQ(EXTI2_3_IRQn);
      break;
    case 1:
      STM32_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);               // GPIO_PIN_8
      STM32_NVIC_EnableIRQ(EXTI4_15_IRQn);
      break;
    case 2:
      STM32_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);               // GPIO_PIN_9
      STM32_NVIC_EnableIRQ(EXTI4_15_IRQn);
      break;
    case 3:
      STM32_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);               // GPIO_PIN_4
      STM32_NVIC_EnableIRQ(EXTI4_15_IRQn);
      break;
    case 4:
      STM32_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);               // GPIO_PIN_1
      STM32_NVIC_EnableIRQ(EXTI0_1_IRQn);
      break;
    case 5:
      STM32_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);               // GPIO_PIN_8
      STM32_NVIC_EnableIRQ(EXTI4_15_IRQn);
      break;
    default:
      Error_Handler(FILE_DIG, __LINE__);
      break;
    }
    break;

  case DIG_FREQ_OUT:
    //
    // Set Buffer Chip direction to Output
    //
    STM32_GPIO_WritePin(DIG_HandleStruct[Ch].Dir_Port, DIG_HandleStruct[Ch].Dir_Pin, GPIO_PIN_SET);
    //
    // Calculate Prescaler and Period.  The Prescaler is set to give the largest possible Period value that is less than 65000.
    //  This will result in the best possible frequency accuracy
    //
    MinPeriod = (uint32_t)(2400000000uLL / DIG_HandleStruct[Ch].DIG_InitStruct.Frequency);         // 24.00 Mhz / xxxxxxx.xx Hz
    DIG_HandleStruct[Ch].Prescaler = MinPeriod / 65000;
    DIG_HandleStruct[Ch].Period = (uint32_t)((2400000000uLL / (DIG_HandleStruct[Ch].Prescaler + 1)) / DIG_HandleStruct[Ch].DIG_InitStruct.Frequency);
    //
    // Initialize Timer Base
    //
    STM32_TIM_Base_Init(DIG_HandleStruct[Ch].Handle->Instance, DIG_HandleStruct[Ch].Prescaler, DIG_HandleStruct[Ch].Period - 1, TIM_AUTORELOAD_PRELOAD_DISABLE);
    //
    // Initialize Timer Channel
    //
    sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    STM32_TIM_PWM_ConfigChannel(DIG_HandleStruct[Ch].Handle->Instance, &sConfigOC, DIG_HandleStruct[Ch].TIM_Channel);
    STM32_TIM_MspPostInit(DIG_HandleStruct[Ch].Handle->Instance);
    STM32_TIM_PWM_Start(DIG_HandleStruct[Ch].Handle->Instance, DIG_HandleStruct[Ch].TIM_Channel);
    DIG_HandleStruct[Ch].bTimerUsed = 1;
    break;

  default:
    Error_Handler(FILE_DIG, __LINE__);
    break;
  }
}

// ------------------------------------------------------------------------
//
// DIG_SetFrequency - Digital I/O Support routine used to set the Output Frequency of the selected Digital I/O
//
// Params:  Ch - Channel Number, 0 to 5, of the Digital I/O being changed
//          NewFreq - New Output Frequency, xxxxx.xx Hz
//
// Returns:   None
//
void DIG_SetFrequency(int32_t Ch, int32_t NewFreq) {

    uint32_t MinPeriod;

    DIG_HandleStruct[Ch].DIG_InitStruct.Frequency = (uint32_t)NewFreq;
    MinPeriod = (uint32_t)(2400000000uLL / DIG_HandleStruct[Ch].DIG_InitStruct.Frequency);
    DIG_HandleStruct[Ch].Prescaler = MinPeriod / 65000;
    DIG_HandleStruct[Ch].Period = (uint32_t)((2400000000uLL / (DIG_HandleStruct[Ch].Prescaler + 1)) / DIG_HandleStruct[Ch].DIG_InitStruct.Frequency);
    if (DIG_HandleStruct[Ch].Handle->Instance->PSC != DIG_HandleStruct[Ch].Prescaler) {
        DIG_HandleStruct[Ch].Handle->Instance->PSC = DIG_HandleStruct[Ch].Prescaler;
    }
    DIG_HandleStruct[Ch].Handle->Instance->ARR = DIG_HandleStruct[Ch].Period - 1;
}

// ------------------------------------------------------------------------
//
// DIG_SetPin - Digital I/O Support routine used to set a Digital I/O Output
//
// Params:  Ch - Channel Number, 0 to 5, of the Digital I/O being set
//
// Returns:   None
//
void DIG_SetPin(int32_t Ch) {
  STM32_GPIO_WritePin(DIG_HandleStruct[Ch].Dig_IO_Port, DIG_HandleStruct[Ch].Dig_IO_Pin, GPIO_PIN_SET);
}

// ------------------------------------------------------------------------
//
// DIG_ResetPin - Digital I/O Support routine used to reset a Digital I/O Output
//
// Params:  Ch - Channel Number, 0 to 5, of the Digital I/O being reset
//
// Returns:   None
//
void DIG_ResetPin(int32_t Ch) {
  STM32_GPIO_WritePin(DIG_HandleStruct[Ch].Dig_IO_Port, DIG_HandleStruct[Ch].Dig_IO_Pin, GPIO_PIN_RESET);
}

// ------------------------------------------------------------------------
//
// DIG_ReadPin - Digital I/O Support routine used to read a Digital I/O Input
//
// Params:  Ch - Channel Number, 0 to 5, of the Digital I/O being read
//
// Returns:   None
//
int32_t DIG_ReadPin(int32_t Ch) {
  return STM32_GPIO_ReadPin(DIG_HandleStruct[Ch].Dig_IO_Port, DIG_HandleStruct[Ch].Dig_IO_Pin);
}

// ------------------------------------------------------------------------
//
// GetDigIo - Digital I/O Support routine used to read a Digital I/O Input
//
// Params:  Ch - Channel Number, 0 to 5, of the Digital I/O being read
//
// Returns:   None
//
int32_t GetDigIo(int32_t Ch) {

  int32_t Digio;

  switch (Ch) {
  case 0:
      Digio = FlashConfiguration->Digio1;
      break;
  case 1:
      Digio = FlashConfiguration->Digio2;
      break;
  case 2:
      Digio = FlashConfiguration->Digio3;
      break;
  case 3:
      Digio = FlashConfiguration->Digio4;
      break;
  case 4:
      Digio = FlashConfiguration->Digio5;
      break;
  case 5:
      Digio = FlashConfiguration->Digio6;
      break;
  default:
      Error_Handler(FILE_DIG, __LINE__);
      break;
  }
  return Digio;
}
// ------------------------------------------------------------------------
//
// InitializeDigitalIO - Digital I/O Support routine used to initialize all Digital I/O
//
// Params:  Mode - DIG_MODE_POWERUP or DIG_MODE_NORMAL
//
// Returns:   None
//
void InitializeDigitalIO(int8_t Mode) {

  DIG_InitTypeDef   DIG_InitStruct;
  int32_t           i;

  DIG_Mode = Mode;

  if (Mode == DIG_MODE_POWERUP) {
    for (i = 0; i < 6; i++) {
      DIG_InitStruct.Config = CONFIG_DIG_DISABLED;
      DIG_InitStruct.Mode = DIG_IN;
      DIG_Init(i, DIG_InitStruct);
    }
  }
  //
  // Disable all Digital I/O Interrupts
  //
  STM32_NVIC_DisableIRQ(TIM1_CC_IRQn);
  STM32_NVIC_DisableIRQ(TIM3_IRQn);
  STM32_NVIC_DisableIRQ(TIM15_IRQn);
  STM32_NVIC_DisableIRQ(TIM16_IRQn);
  STM32_NVIC_DisableIRQ(TIM17_IRQn);
  STM32_NVIC_DisableIRQ(USART2_IRQn);
  STM32_NVIC_DisableIRQ(USART3_4_IRQn);
  STM32_NVIC_DisableIRQ(EXTI0_1_IRQn);
  STM32_NVIC_DisableIRQ(EXTI2_3_IRQn);
  STM32_NVIC_DisableIRQ(EXTI4_15_IRQn);
  //
  // Disable All Digital I/O Timers
  // Disable All Digital I/O Data Port Pins (basically turns all Data Ports into inputs
  // Set all Digital I/O External Bi-Directional Buffers as inputs
  //
  for (i = 0; i < 6; i++) {
    if (i != 4) {
      STM32_TIM_Base_DeInit(DIG_HandleStruct[i].Handle->Instance);
    }
    STM32_GPIO_DeInit(DIG_HandleStruct[i].Dig_IO_Port, DIG_HandleStruct[i].Dig_IO_Pin);
    STM32_GPIO_WritePin(DIG_HandleStruct[i].Dir_Port, DIG_HandleStruct[i].Dir_Pin, GPIO_PIN_RESET);
  }

  for (i = 0; i < 6; i++) {
    if ((i == 0) && ((Mode == DIG_MODE_POWERUP) || (bFlashConfigurationValid == 0))) {
      DIG_InitStruct.Config = CONFIG_DIG_SERIAL_INOUT;
    } else {
      DIG_InitStruct.Config = GetDigIo(i);
      if (bFlashConfigurationValid == 0) {
        DIG_InitStruct.Config = CONFIG_DIG_DISABLED;
      }
    }

    switch (DIG_InitStruct.Config) {
    case CONFIG_DIG_DISABLED:
    case CONFIG_DIG_TEST_IN:
      DIG_InitStruct.Mode = DIG_IN;
      DIG_Init(i, DIG_InitStruct);
      break;
    case CONFIG_DIG_MASTER_FREQ_OUT:
    case CONFIG_DIG_KMA_FREQ_OUT:
    case CONFIG_DIG_TEMP_FREQ_OUT:
      if (i != 4) {                               // Not Digital I/O #5
        DIG_InitStruct.Mode = DIG_FREQ_OUT;
        DIG_InitStruct.Frequency = 10000;        // xxx.xx Hz  10000 = 100Hz
        DIG_Init(i, DIG_InitStruct);
      }
      break;
    case CONFIG_DIG_ERROR_CONDITION:
      DIG_InitStruct.Mode = DIG_OUT;
      DIG_Init(i, DIG_InitStruct);
      break;
    case CONFIG_DIG_SERIAL_INOUT:
      if (i == 0) {                           // Only Digital I/O #1
        DIG_InitStruct.Mode = DIG_SERIAL_INOUT;
        DIG_Init(i, DIG_InitStruct);
        break;
      }
      break;
    default:
      Error_Handler(FILE_DIG, __LINE__);
      break;
    }
  }
}
