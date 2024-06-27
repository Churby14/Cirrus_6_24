/**
 ******************************************************************************
 * File Name:       ticks.c
 *
 * Description:     Tick Count Logic
 *
 * Release History:
 *
 *  Ver 2017-02-26  RLW     Initial Release
 *
 *
 ******************************************************************************
 **/

#include "global.h"

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// HLR-xxxx
// LLR-xxxx
//
void ProcessTickCounters(void) {

  static uint32_t OldTicks;
  uint32_t CurrentTicks;
  int16_t DeltaTicks, i;

  CurrentTicks = HAL_GetTick();
  DeltaTicks = CurrentTicks - OldTicks;
  if (DeltaTicks == 0) {
      goto ExitHere;
  }

  OldTicks = CurrentTicks;
  Ticks1ms += DeltaTicks;
  Ticks5ms += DeltaTicks;
  Ticks1000ms += DeltaTicks;
  AccelTicks += DeltaTicks;
  for ( i = 0; i < 6; i++ ) {
    DIG_HandleStruct[i].FreqInputTicks += DeltaTicks;
  }

ExitHere:
    return;
}

