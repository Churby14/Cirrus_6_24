// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: logevent.c
// Author  : Rick Wright
// Revision: C
//
//  Description:     Logs events to EEPROM
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
//
// The event log uses 32,768 bytes of the EEPROM, which is 2048 entries of 16 bytes each.

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
//

// ------------------------------------------------------------------------
//                  Local Conditional Compilation Switches
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//                          Local Variables
// ------------------------------------------------------------------------
static uint8_t  TmpBuffer[128];

// ------------------------------------------------------------------------
//
// InitializeEventLog - This routine is called during the initialization phase of main loop.  The code checks the Index (first 4 bytes) of each
// record, looking for one of two things:
//
//  1. Either a -1, which would indicate the end of the event log, or
//  2. Looks for the record with the lowest index number.
//
//  If a -1 isn't found, then this indicates that the Event Log is full and has rolled-over back to the beginning (ring buffer).
//  In this case, the next entry into the log is the record with the lowest index number.
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void InitializeEventLog(void) {

    uint16_t              i;
    stEventMessage        EventMessage;
    //
    // 16 * 2048 = 32,768 bytes, but last 24 entries are for the default output, thus, event log is only the first 2024 pages
    // Uses 2024 given this is perfectly divisible by 128 which is how much data is written to a page when clearing the log
    //
    // There is one specical case:  Last message was stored at the end of the Event Log.  In this case, the EntryNumber
    //  doesn't drop.  Too account for this, initialize as follows:
    //
    EventData.NumberOfEntries = 0;
    EventData.NextEntry = 0;
    for (i = 0; i < 2024; i++) {
        I2C2Control.Address = EEPROM_EVENT_DATA_OFFSET + (i * 16);
        I2C2Control.Length = 4;
        I2C2Control.Buffer = (uint8_t *)&EventMessage;
        I2C2Control.Mode = I2C_MODE_READ_EEPROM;
        do {
            ProcessI2C2();
        }  while (I2C2Control.Mode != I2C_MODE_DONE);
        I2C2Control.Mode = I2C_MODE_IDLE;
        //
        //  There are two possiblies -  One, the table hasn't been completely filled.
        //                              Two, the table has been completely filled.
        //
        // If the table hasn't been completely filled, then we will find an EntryNumber of -1.
        // This would indicate the end of the Event Log and would be where the next message should be stored.
        //
        //  If the Event Log has been completely filled and rolled-over, then the location where the next message
        //  should be stored will be where the Index value drops in value.

        if (EventMessage.EntryNumber == -1) {       // End of Event Log has been reached
            EventData.NextEntry = i;
            EventData.NumberOfEntries = i;
            break;
        } else if (EventMessage.EntryNumber < EventData.NumberOfEntries) {   // If new EntryNumber is less than Max, then this is where the next message will be stored
            EventData.NextEntry = i;
            break;
        } else {                                          // Update MexEntryNumber
            EventData.NumberOfEntries = EventMessage.EntryNumber;
        }
    }
    SysVars.NumberOfEventLogEntries = EventData.NumberOfEntries;
}

// ------------------------------------------------------------------------
//
//  ProcessClearEventLog - This routine is called from every 10ms from Main Loop  The log is cleared by writing
//  0xFF to all 32,768 bytes.  This is done 128 bytes at a time, as that is the page size
//  of the EEPROM.
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void ProcessClearEventLog(void) {

    static uint8_t    State;
    static uint16_t   Index;

    ExecutionStart(EXECUTION_PROCESS_CLEAR_LOG);

    if (bClearEventLog == 0) {
        State = 0;
    }

    switch (State) {
    case 0:
        if ((I2C2Control.Mode == I2C_MODE_IDLE) && (bClearEventLog != 0)) {
            memset(&TmpBuffer[0], 0xFF, 128);
            Index = 0;
            I2C2Control.Address = EEPROM_EVENT_DATA_OFFSET + (Index * 128);
            I2C2Control.Buffer = &TmpBuffer[0];
            I2C2Control.Length = 128;
            I2C2Control.Mode = I2C_MODE_WRITE_EEPROM;
            State = 2;
        }
        break;
    case 1:
        I2C2Control.Address = EEPROM_EVENT_DATA_OFFSET + (Index * 128);
        I2C2Control.Buffer = &TmpBuffer[0];
        I2C2Control.Length = 128;
        I2C2Control.Mode = I2C_MODE_WRITE_EEPROM;
        State = 2;
        break;
    case 2:
        if (I2C2Control.Mode == I2C_MODE_DONE) {
            Index++;
            // Index is
            if (Index == 238) {
                EventData.NextEntry = 0;
                EventData.NumberOfEntries = 0;
                SysVars.NumberOfEventLogEntries = 0;
                bClearEventLog = 0;
                I2C2Control.Mode = I2C_MODE_IDLE;
                LogEvent(EVENT_MESSAGE_ID_0, 0, 0);
                State = 0;
            } else {
                State = 1;
            }
        }
        break;
    default:
        Error_Handler(FILE_LOGEVENT, __LINE__);
        break;
    }
    ExecutionEnd(EXECUTION_PROCESS_CLEAR_LOG);
}

// ------------------------------------------------------------------------
//
//  LogEvent - Each Event Message is 64 bytes.
//  The Event Log stores the last 506 messages (ring buffer).  With each messages being 64 bytes, that uses 32,768 bytes of EEPROM
//
//  When the end of the log buffer is reached, loop back to the beginning (ring buffer)
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void LogEvent(uint32_t EventId, int32_t EventValue1, int32_t EventValue2) {
    //
    // While Event Log is being Cleared, don't write any new messages
    //
    EventData.NumberOfEntries++;
    MessageQueue[MessageIndexIn].EventId = EventId;
    MessageQueue[MessageIndexIn].EntryNumber = EventData.NumberOfEntries;
    MessageQueue[MessageIndexIn].EventValue1 = EventValue1;
    MessageQueue[MessageIndexIn].EventValue2 = EventValue2;
    MessageQueue[MessageIndexIn].Timestamp = STM32_GetTick() / 1000;     // Milliseconds to seconds
    MessageIndexIn = (MessageIndexIn + 1) & 0xF;
}

// ------------------------------------------------------------------------
//
//  ProcessEventMessageQueue - Called every 10 ms, routine handles the storage/logging
//                             of the most recent EEPROM messages
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void ProcessEventMessageQueue(void) {

    static uint8_t  State1;

    ExecutionStart(EXECUTION_PROCESS_MESSAGE_QUEUE);
    switch (State1) {
    case 0:
        if ((MessageIndexIn != MessageIndexOut) && (I2C2Control.Mode == I2C_MODE_IDLE)) {
            I2C2Control.Address = EEPROM_EVENT_DATA_OFFSET + (EventData.NextEntry * 16);
            I2C2Control.Buffer = (uint8_t *)&MessageQueue[MessageIndexOut];
            I2C2Control.Length = 16;
            I2C2Control.Mode = I2C_MODE_WRITE_EEPROM;
            State1++;
        }
        break;
    case 1:
        if (I2C2Control.Mode == I2C_MODE_DONE) {
            State1 = 2;
        }
        break;
    case 2:
        //
        // NOTE:  While this logic could be place within the if statement
        //          in the previous case, it needs to be in a seperate case
        //           for timing.  10mS isn't always enough time for EEPROM
        //          to write data.  The extra case extends the time before
        //          the next write to 20mS.
        //
        EventData.NextEntry++;
        // 1904 = 2048 - (2304/16) Subtract backup flash configuration storage
        if (EventData.NextEntry >= 1904) {
            EventData.NextEntry = 0;
        }
        SysVars.NumberOfEventLogEntries = EventData.NumberOfEntries;
        MessageIndexOut = (MessageIndexOut + 1) & 0xF;
        I2C2Control.Mode = I2C_MODE_IDLE;
        State1 = 0;
        break;
    default:
        Error_Handler(FILE_LOGEVENT, __LINE__);
        break;
    }
    ExecutionEnd(EXECUTION_PROCESS_MESSAGE_QUEUE);
}


