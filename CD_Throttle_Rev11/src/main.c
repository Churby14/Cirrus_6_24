// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: main.c
// Author  : Rick Wright/Charlie Urban
// Revision: 3.2
//
//  Description:       Main Loop
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
//
// Main - main execution routine for all Eexecutable Object Code
//
// Params:  None
//
// Returns:   None
//
// Requirement(s):
//
int main(void) {

  InitializeSystem();

  while (1) {

    ExecutionStart(EXECUTION_MAIN_LOOP);
    ProcessTickCounters();
    ProcessI2C1();
    ProcessI2C2();
    ProcessMcp3421();

    if (Ticks1ms >= 1) {
      Ticks1ms -= 1;
      ProcessRxSerial(&SerialUsart1);
      ProcessRxSerial(&SerialUsart2);
      ProcessReadAdc();
      ProcessAccelerometer();
      if (Ticks5ms >= 5) {
        Ticks5ms -= 5;
        ProcessAdcFilter();                         // Run Filtering every 5mS
        if (Ticks10ms >= 10) {
          Ticks10ms -= 10;
          ProcessMasterIndex();
          ProcessAnalogReadings();
          ProcessPwmOutput();
          ProcessSignalStatus();
          IWDG_TriggerWatchdog();
          ProcessClearEventLog();
          ProcessEventMessageQueue();
          ProcessLogDefaultProfile();
          ProcessDefaultProfile();
          ProcessThrottle();
          ProcessCan();
          if (Ticks20ms >= 20) {
            Ticks20ms -= 20;
            ProcessFrequencyOutputs();
            ProcessMasterFrequencyOutput();
            ProcessTestInput();
            ProcessErrorCondition();
            if (Ticks1000ms >= 1000) {              // One second
              Ticks1000ms -= 1000;
              ProcessVerifyStack();
              ExecutionAverage();
            }
          }
        }
      }
    }
    ExecutionEnd(EXECUTION_MAIN_LOOP);
  }
}
