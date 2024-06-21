// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: watchdog.c
// Author  : Rick Wright/Charlie Urban
// Revision: 3.2
//
// Description:     This module controls the IWDG module
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
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
//                          Local Variables
// ------------------------------------------------------------------------

static uint32_t LastTicks, AccumTicks;

// ------------------------------------------------------------------------
//
// IWDG_TriggerWatchdog - This routine is called every 10mS
//
// The Timeout is set to 1.25 seconds, and the Window at 0.75 second.  This means the HAL_IWDG_Refresh routine needs
// to be called before 1.25 seconds, but not before 0.75 second since the last call.
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//

void InitializeWatchDog (void) {
    //
    // Configure Watchdog for a 0.75 to 1.25 second window
    //
    IWDG->KR = IWDG_KEY_ENABLE;
    IWDG->KR = IWDG_KEY_WRITE_ACCESS_ENABLE;
    IWDG->PR = IWDG_PRESCALER_256;
    IWDG->RLR = (1250 * 10) / 64;
    do {
    } while ((IWDG->SR & 0x7) != RESET);
    IWDG->WINR = (750 * 10) / 64;
    LastTicks = STM32_GetTick();
    AccumTicks = 0;
}

//************************************************************************************************************************
//************************************************************************************************************************

void IWDG_TriggerWatchdog(void) {

    static uint8_t    BadCounter;
    uint8_t           bGood;
    uint32_t          Ticks;


    Ticks = STM32_GetTick();
    AccumTicks += Ticks - LastTicks;
    LastTicks = Ticks;

    if (AccumTicks < 1000) {
        return;
    }
    AccumTicks = 0;

    bGood =  (ExecutionTimeReport[EXECUTION_PROCESS_ADC_FILTER].CallsPerSecond == 200);
    bGood &= (ExecutionTimeReport[EXECUTION_PROCESS_ANALOG_READINGS].CallsPerSecond == 100);
    bGood &= (ExecutionTimeReport[EXECUTION_PROCESS_SIGNAL_STATUS].CallsPerSecond == 100);
    bGood &= (ExecutionTimeReport[EXECUTION_PROCESS_MASTER_INDEX].CallsPerSecond == 100);
    bGood &= (ExecutionTimeReport[EXECUTION_PROCESS_FREQUENCY_OUTPUTS].CallsPerSecond == 50);
    bGood &= (ExecutionTimeReport[EXECUTION_PROCESS_MASTER_FREQUENCY_OUTPUT].CallsPerSecond == 50);
    bGood &= (ExecutionTimeReport[EXECUTION_PROCESS_ERROR_CONDITION].CallsPerSecond == 50);
    bGood &= (ExecutionTimeReport[EXECUTION_PROCESS_ADC].CallsPerSecond == 1000);
    bGood &= (ExecutionTimeReport[EXECUTION_PROCESS_ACCELEROMETER].CallsPerSecond == 1000);
    bGood &= (ExecutionTimeReport[EXECUTION_PROCESS_TEST_INPUT].CallsPerSecond == 50);
    bGood &= (ExecutionTimeReport[EXECUTION_PROCESS_PWM_OUT].CallsPerSecond == 100);
    bGood &= (ExecutionTimeReport[EXECUTION_PROCESS_CLEAR_LOG].CallsPerSecond == 100);
    bGood &= (ExecutionTimeReport[EXECUTION_PROCESS_MESSAGE_QUEUE].CallsPerSecond == 100);
    bGood &= (ExecutionTimeReport[EXECUTION_PROCESS_THROTTLE].CallsPerSecond == 100);
    bGood &= (ExecutionTimeReport[EXECUTION_PROCESS_CAN].CallsPerSecond == 100);


    //
    // If execution times are all good, reset BadCounter.  If not, then increment BadCounter.  Once BadCounter
    //    reaches 5, then don't retrigger Watchdog.  This will allow Watchdog Timer to time-out and reset
    //    the processor.  This is basically debounce logic.
    //
    if (bGood != 0) {
        BadCounter = 0;
    } else {
        if (++BadCounter < 5) {
            bGood = 1;
        }
    }
    //
    // Before Resetting
    //
    if (bGood != 0) {
        IWDG->KR = IWDG_KEY_RELOAD;
    } else {
        //
        // This code is executed if a problem has been detected and the processor needs to be reset.
        //    This will log a message to the EEPROM about the reset.  As the reset won't actually happen
        //    for another .25 seconds, that should be plenty of time for message to actually be written
        //    into the EEPROM.  Therefore, there is no need to call ProcessEventMessageQueue() and
        //    ProcessI2C2 as was needed when updating firmware (see serial.c, command 0xCC)
        //
        LogEvent(EVENT_MESSAGE_ID_2, 0, 0);
    }
}
