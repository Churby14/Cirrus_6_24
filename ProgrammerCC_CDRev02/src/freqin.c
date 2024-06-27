//
// ******************************************************************************
//
// Description:     Capture of Frequency Input and Calculation of Frequency
//
// Release History:
//
//  2018-10-12  RLW     Initial Release
//
// ******************************************************************************

#include "global.h"

static uint16_t OldCapturePeriodCounter[6];

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// This interrupt callback routine is called on the rising edge of an externally enabled interrupt, which would be DigIO_1 through DigIO6.
//  If the signal as a slow rise time, then there is the possibly that there will be glitches (multiple edges on both rising and
// falling edge of input signal.
//
// The multiple edges are handled as follows"
//
//  On the first rising edge (the real one), an interrupt is generated and the ISR is executed.  But, while the ISR is being executed,
//  the glitches are coming into the port pin.  But, as the interrupts are still disabled, the glitches aren't seen by the CPU - So
//  life is good.
//
//  The interrupts are setup for rising edge only, but, on the falling edge, the glitches will create a rising edge.  As the glitches
//  will have passed by the time this code is executed, we can check if the port pin is high or low.  If low, then it was a glitch that
//  created the interrupt, so exit this routine without any futher processing of the edge.  If high, then continue processing the edge,
//  as it is a real rising edge.
//
//
// HLR-xxxx
// LLR-xxxx
//
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

  int16_t Ch;
  //
  // Determine Channel.
  //
  if ( GPIO_Pin == GPIO_PIN_3 ) {
    Ch = 0;
  } else if ( GPIO_Pin == GPIO_PIN_8 ) {
    if ( DIG_HandleStruct[1].DIG_InitStruct.Mode == DIG_FREQ_IN ) {
      Ch = 1;
    } else {
      Ch = 5;
    }
  } else if ( GPIO_Pin == GPIO_PIN_9 ) {
    Ch = 2;
  } else if ( GPIO_Pin == GPIO_PIN_4 ) {
    Ch = 3;
  } else if ( GPIO_Pin == GPIO_PIN_10 ) {
    Ch = 4;
  } else {
    return;
  }
  //
  //  If interrupt was from a real leading edge, then pin should be high.  If pin is low, then interrupt was from a falling edge with glitches.
  //  By the time this logic is executed, any glitches on the input frequency should have passed by this point in the code.
  //
  if ( HAL_GPIO_ReadPin(DIG_HandleStruct[Ch].Dig_IO_Port, DIG_HandleStruct[Ch].Dig_IO_Pin) == 0 ) return;  //

  uint16_t Timer7Counts = __HAL_TIM_GET_COUNTER(&htim7);                // running at 1Mhz, but will rollover every 65.5mS
  uint16_t Timer6Counts = __HAL_TIM_GET_COUNTER(&htim6);                // running at 200KHz, rollover every 327.68mS

  uint16_t Diff7 = Timer7Counts - DIG_HandleStruct[Ch].Timer7Counts;
  uint16_t Diff6 = Timer6Counts - DIG_HandleStruct[Ch].Timer6Counts;
  DIG_HandleStruct[Ch].Timer7Counts = Timer7Counts;
  DIG_HandleStruct[Ch].Timer6Counts = Timer6Counts;
  //
  // If Diff6 (200KHz) counter is less than 10000, then DIff7 (1Mhz) counter will be less than 50000.  Which means Diff7 didn't roll-over
  //
  if ( Diff6 < 10000 ) {
    DIG_HandleStruct[Ch].Accum += Diff7;
  } else {
    DIG_HandleStruct[Ch].Accum += Diff6 * 5;
  }
  DIG_HandleStruct[Ch].AccumCounter++;

  if ( DIG_HandleStruct[Ch].Accum >= 500000 ) {
    DIG_HandleStruct[Ch].CaptureAccum = DIG_HandleStruct[Ch].Accum;
    DIG_HandleStruct[Ch].CaptureAccumCounter = DIG_HandleStruct[Ch].AccumCounter;
    DIG_HandleStruct[Ch].CapturePeriodCounter++;
    DIG_HandleStruct[Ch].Accum = 0;
    DIG_HandleStruct[Ch].AccumCounter = 0;
  }
}

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
//  This routine is called every time through main loop
//
//  At minimum input frequency of 20 Hz, rise-to-rise time will be 50 mS
//  At maximum input frequency of 20 KHz, rise-to-rise time will be 50uS
//
//  CapturePeriodCounter, CaptureAccumCounter, and CaptureAccum are updated in
//      HAL_GPIO_EXTI_Callback function
//
void CalculuateFrequencyInput(void) {

  int16_t i;

      for ( i = 0; i < 6; i++ ) {
        //
        // If DIG I/O isn't configured as Frequency Input, or
        //  rise-to-rise > 500 mS, then set CaptureFrequency to 0
        //
        if ( (DIG_HandleStruct[i].DIG_InitStruct.Mode != DIG_FREQ_IN) || (DIG_HandleStruct[i].FreqInputTicks > 1000) ) {
          DIG_HandleStruct[i].CaptureFrequency = 0;
          DIG_HandleStruct[i].FreqInputTicks = 0;
          //
          // A change in CapturePeriodCounter means a rising
          //  edge has been detected.
          //
        } else if ( DIG_HandleStruct[i].CapturePeriodCounter != OldCapturePeriodCounter[i] ) {
          OldCapturePeriodCounter[i] = DIG_HandleStruct[i].CapturePeriodCounter;
          DIG_HandleStruct[i].CaptureFrequency = MultDivideWithRounding64((100000000), DIG_HandleStruct[i].CaptureAccumCounter, DIG_HandleStruct[i].CaptureAccum, RESULT_32);
          DIG_HandleStruct[i].FreqInputTicks = 0;
        }
        SysVars.DigIO_InputFrequency[i] = DIG_HandleStruct[i].CaptureFrequency;
      }
}
