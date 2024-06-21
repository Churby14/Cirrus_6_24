// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: ticks.c
// Author  : Rick Wright
// Revision: C
//
// Description: Function increments counters by milliseconds.
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Revision Log:
//
//  A           CRU   12/21/2021   Baseline Release
//  B           RLW   06/13/2022   Changes per PR #19 in PACT
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
// ProcessTickCounters - Increments all the counters by number of milliseconds passed
//                       since the last call of the function
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void ProcessTickCounters(void) {

    static uint32_t OldTicks;
    uint32_t CurrentTicks, DeltaTicks;
    int16_t i;

    ExecutionStart(EXECUTION_PROCESS_TICK_COUNTERS);

    CurrentTicks = STM32_GetTick();
    if (CurrentTicks != OldTicks) {
        DeltaTicks = CurrentTicks - OldTicks;
        OldTicks = CurrentTicks;
        Ticks1ms += DeltaTicks;
        Ticks5ms += DeltaTicks;
        Ticks10ms += DeltaTicks;
        Ticks20ms += DeltaTicks;
        Ticks1000ms += DeltaTicks;
        for (i = 0; i < 6; i++) {
            DIG_HandleStruct[i].FreqInputTicks += DeltaTicks;
        }
    }

    ExecutionEnd(EXECUTION_PROCESS_TICK_COUNTERS);
}

