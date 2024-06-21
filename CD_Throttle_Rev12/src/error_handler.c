// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------
//
// ------------------------------------------------------------------------
// Filename: error_handler.c
// Author  : Rick Wright
// Revision: C
//
// Description:       Places STM32 into an infinite loop.  Only exit from
//                     loop is when Watchdog Timer resets STM32 processor.
//
// ------------------------------------------------------------------------
//
// ------------------------------------------------------------------------
//
// Revision Log:
//
//  A           CRU   12/21/2021   Baseline Release
//  B           RLW   06/16/2022   Changes per PR #19 in PACT
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
// Error_Hander - Enter an infinite loop if a critical error has been detected.  The
//  only exit is when the Watchdog Timer times-out and resets the Microcontroller.
//
// Params:    FileCode - Value associated with each source code file.
//            LineNumber - Line within the source code file that called the function.
//
//  Return:   None
//
void Error_Handler(int32_t FileCode, int32_t LineNumber) {

    static uint8_t      bRunningFlag;


    if (!bRunningFlag) {
        bRunningFlag = 1;
        LogEvent(EVENT_MESSAGE_ID_102, FileCode, LineNumber);
        //
        // Wait for Event to be written to EEPROM
        //
        while (MessageIndexIn != MessageIndexOut) {
            ProcessEventMessageQueue();
            ProcessAccelerometer();
            ProcessI2C2();
        }
    }

    while (1) {
    }
}

