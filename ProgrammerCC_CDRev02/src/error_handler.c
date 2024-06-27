//
// ******************************************************************************
//
// Description:       Places STM32 into an infinite loop.  Only exit from
//                     loop is when Watchdog Timer resets STM32 processor.
//
// Release History:
//
//  2018-10-12  RLW     Initial Release
//
// ******************************************************************************

#include "global.h"
#include "math.h"

//********************************************************************************************************************************
//********************************************************************************************************************************
//
// HLR-xxxx
// LLR-xxxx
//
void Error_Handler(void) {

  while ( 1 ) {
    HAL_GPIO_TogglePin( EXT_PULLUP_GPIO_Port, EXT_PULLUP_Pin);
    HAL_Delay(5);
  }
}

