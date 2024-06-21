// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: freqin.c
// Author  : Rick Wright
// Revision: -
//
// Description:     Capture of Frequency Input and Calculation of Frequency
//
// ------------------------------------------------------------------------
//
// ------------------------------------------------------------------------
//
// Revision Log:
//
//  -          RLW   Pre-baseline
//
// ------------------------------------------------------------------------
//
// Design Details and Explanations:
//
// ------------------------------------------------------------------------
//
// Originally, The input frequency was determined by using the a Timer.  The
// Timer was configured to divide the Input Frequency by 8, and then to provide
// an interrupt on each rising edge of input.
//
// In the interrupt routine, the Execution Timer (TIMER2) was read and difference
// (delta) from the last interrupt as accumulated.  When the accumulated value
// reached 200000 (or 0.5 seconds), the input frequency was calculated.
//
// This logic worked well until a slow rising input was used.  As the Bi-Directional
// Buffers have no hysthisis, the output from the buffers would oscillate, causing
// the input frequency to be incorrect.
//
// To solve this problem, the logic was changed to cause an interrupt using the GPIO
// module, not the TIMER module.  The GPIO was configured to cause an interrupt on
// each rising edge of the input frequency.
//
// While there are still multiple rising edges on a slow rising input, the program
// will only see the first one.  By the time the interrupt routine starts executing,
// the remaining edges will have passed.
//
// The down side is the interrupt rate is now 8 times higher than when the Timer was
// being used.  If multiple high frequency inputs are being measured, at some point
// the interrupt rate will become too high and prevent the program from executing
// correctly.
//
// TODO: To prevent this type of failure condition, look into using the
// DMA controller.
//
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
// HAL_GPIO_EXTI_Callback - This interrupt callback routine is called on the rising edge of an externally enabled interrupt, which would be any DIG_IO assigned as a
// Slave Freqency Input.
//
// If the signal has a slow rise time, then there is the possibly that there will be glitches (multiple edges on both rising and
// falling edge of input signal.  (wasn't able to find a bi-directional level-shifting buffer with hysthisis)
//
// The multiple edges are handled as follows:
//
//  On the first rising edge (the real one), an interrupt is generated and the ISR is executed.  While the ISR is being executed,
//  the glitches are coming into the port pin.  But, as the interrupts are still disabled, the glitches aren't seen by the CPU - So
//  life is good.
//
//  The interrupts are setup for rising edge only, but, on the slow falling edge, the glitches will create a rising edge.  As the glitches
//  will have passed by the time this code is executed, we can check if the port pin is high or low.  If low, then it was a glitch on
//  the folling edge that created the interrupt, so exit this routine without any futher processing of the edge.  If high, then continue
//  processing the edge, as it is a real rising edge.
//
// Params:    GPIO_Pin - Pin Number that created the interrupt.
//
// Return:    None
//
void HAL_GPIO_EXTI_Callback(uint32_t GPIO_Pin) {

  int16_t Ch;
  //
  // Determine Channel.
  //
  if (GPIO_Pin == GPIO_PIN_3) {
    Ch = 0;
  } else if (GPIO_Pin == GPIO_PIN_8) {
    if (DIG_HandleStruct[1].DIG_InitStruct.Mode == DIG_FREQ_IN) {
      Ch = 1;
    } else {
      Ch = 5;
    }
  } else if (GPIO_Pin == GPIO_PIN_9) {
    Ch = 2;
  } else if (GPIO_Pin == GPIO_PIN_4) {
    Ch = 3;
  } else if (GPIO_Pin == GPIO_PIN_1) {
    Ch = 4;
  } else {
    Error_Handler(FILE_FREQIN, __LINE__);
    }
  //
  //  If interrupt was from a real leading edge, then pin should be high.  If pin is low, then interrupt was from a falling edge with glitches.
  //  By the time this logic is executed, any glitches on the input frequency should have passed by this time this code is executed.
  //
  if (STM32_GPIO_ReadPin(DIG_HandleStruct[Ch].Dig_IO_Port, DIG_HandleStruct[Ch].Dig_IO_Pin) != 0) {
    uint32_t Timer2Counts = TIM2->CNT;                // running at 16Mhz

    uint32_t Diff2 = Timer2Counts - DIG_HandleStruct[Ch].Timer2Counts;
    DIG_HandleStruct[Ch].Timer2Counts = Timer2Counts;
    DIG_HandleStruct[Ch].Accum += Diff2;
    DIG_HandleStruct[Ch].AccumCounter++;
    //
    // Once we have accumulated at least 4,000,000 counts (0.25 seconds max), save Accumulated Counts and Edge Counter
    //  Then, zero Accumulator and Edge Counter to start
    //
    //  Lowest Slave Frequency = 64.06Hz = 15.61mS period = 249,760 counts  (249760 * 17 edges = 4245920 counts)      Resolution: 1 count = 0.0000158Hz
    //  Highest Slave Frequency=  16,384Hz = 61.035uS period = 976.56 counts  (976.56 * 4097 edges = 4000966 counts)   Resolution: 1 count = 0.0041Hz
    //
    if (DIG_HandleStruct[Ch].Accum >= 4000000) {                                              // .25 seconds @ 16Mhz
      DIG_HandleStruct[Ch].CaptureAccum = DIG_HandleStruct[Ch].Accum;
      DIG_HandleStruct[Ch].CaptureAccumCounter = DIG_HandleStruct[Ch].AccumCounter;
      DIG_HandleStruct[Ch].CapturePeriodCounter++;
      DIG_HandleStruct[Ch].Accum = 0;
      DIG_HandleStruct[Ch].AccumCounter = 0;
    }
  }
}
