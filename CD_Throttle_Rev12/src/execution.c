// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: execution.c
// Author  : Rick Wright
// Revision: C
//
// Description:     Monitors Min, Max, and Average Execution Times
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
// InitializeExecutionTimers - Initialize TIMER2 to 16Mhz - providing a resolution of 0.0625 uS
//
// Params:    None
//
// Return:    None
//
void InitializeExecutionTimers(void) {
  //
  // Initialize Execution Timer, TIMER2 - Also used to measure Slave Frequency Inputs
  //
  MX_TIM2_Init();
  TIM2->CNT = 0;
  STM32_TIM_Base_Start(TIM2);
}

// ------------------------------------------------------------------------
//
// ExecutionStart - This routine is called at the beginning of each major function/procedure.    The routine records the value of TIMER2
//
//  Params:   Process - Process ID Number
//
//  Return:   None
//
void ExecutionStart(int16_t Process) {
  ExecutionTimeData[Process].StartTime2 = TIM2->CNT;
}

// ------------------------------------------------------------------------
//
// ExecutionEnd - This routine is called at the end of each major function/procedure.  The routine reads the value of TIMER2.
// The routine then The subtracts the beginning time from the current timer value, resulting in the exection time
// of the routine.  The routine then checks if current execution time is less than Min or greater than Max.
// Routine also adds current time to accumulater, which is used later to calculate the average execution time
//
//  Params:   Process - Process ID Number
//
//  Return:   None
//
void ExecutionEnd(int16_t Process) {

  uint32_t NewCounts;

  NewCounts = TIM2->CNT - ExecutionTimeData[Process].StartTime2;

  if (NewCounts < ExecutionTimeData[Process].CurrentMinTime) {
    ExecutionTimeData[Process].CurrentMinTime = NewCounts;
  }
  if (NewCounts > ExecutionTimeData[Process].CurrentMaxTime) {
    ExecutionTimeData[Process].CurrentMaxTime = NewCounts;
  }
  ExecutionTimeData[Process].Accum += NewCounts;
  ExecutionTimeData[Process].AccumCount++;
}

// ------------------------------------------------------------------------
//
// ExecutionAverage - This routine is called every 1 second (on average).  It calculates the average time
// based on accumulated time, and it also saves the Min/Max times, then
// resets Min/Max and accumulator
//
//  NOTE:  AverageTime is xx.xxx uS
//
// Params:    None
//
// Return:    None
//
void ExecutionAverage(void) {

  uint16_t i;

  for (i = 0; i < EXECUTION_PROCESS_COUNT; i++) {
    ExecutionTimeReport[i].CallsPerSecond = ExecutionTimeData[i].AccumCount;
    ExecutionTimeReport[i].MinTime = (ExecutionTimeData[i].CurrentMinTime * 100) / 16;      // xxx.xx uS
    ExecutionTimeReport[i].MaxTime = (ExecutionTimeData[i].CurrentMaxTime * 100) / 16;      // xxx.xx uS
    ExecutionTimeReport[i].AverageTime = 0;
    if (ExecutionTimeData[i].AccumCount != 0) {
      ExecutionTimeReport[i].AverageTime = MultDivideWithRounding64(ExecutionTimeData[i].Accum, 1000, ExecutionTimeData[i].AccumCount * 16, RESULT_32);
    }
    ExecutionTimeData[i].CurrentMinTime = 20000000;         // Start with a .125 seconds min value
    ExecutionTimeData[i].CurrentMaxTime = 0;
    ExecutionTimeData[i].AccumCount = 0;
    ExecutionTimeData[i].Accum = 0;
  }
}
