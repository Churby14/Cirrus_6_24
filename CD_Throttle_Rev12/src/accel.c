// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: accel.c
// Author  : Rick Wright
// Revision: C
//
// Description:     This module contains functions that:
//                      - Initialize the Accelerometer
//                      - Read the Accelerometer
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

static uint8_t  bInitializeAccel;

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
//
// InitializeAccel - This routine is called once during power-up and is used to initialize the Accelerometer
//                  chip to the correct full-scale based on the AccelerometerScaling PDI
//
// NOTE:  THIS ROUTINE IS BLOCKING!!  The routine does not exist until the I2C communications is finished.  As
//          this is only called once during initialization, blocking is acceptable.
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):  SYS-040
//
void InitializeAccel(void) {

    //
    // Only Initialize if FactoryConfiguration is valid,
    //
    if ((bFlashConfigurationValid != 0) && (bFactoryConfigurationValid != 0) && (FactoryConfiguration->AccelPresent != 0)) {
      bInitializeAccel = 1;
    }
}
//
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
//
// ProcessAccelerometer - This routine is called every 1mS (on average) and reads the Accelerometer X/Y/Z data registers.
//
//  The Accelerometer is only read if:
//          I2C bus is idle (not being used by EEPROM)
//          Factory Configuration is valid
//          Accelerometer is flagged as being present
//          There are no messages waiting to be written to EEPROM
//
// Params:    None
//
// Returns:   Accelerometer Data is stored into SysVar data structure
//
// Requirement(s):  SYS-040
//
void ProcessAccelerometer(void) {

    static uint8_t State, AccelDelayTimer;
    static uint8_t TmpBuffer[6];

    ExecutionStart(EXECUTION_PROCESS_ACCELEROMETER);

    switch (State) {
    case 0:
        //
        // Only proceed if I2C bus is ready and not being used
        //
        if (AccelDelayTimer < 5) {
          AccelDelayTimer++;
        }
        if ((I2C2Control.Mode == I2C_MODE_IDLE) && (bFlashConfigurationValid != 0) && (bFactoryConfigurationValid != 0) && (FactoryConfiguration->AccelPresent != 0) &&
          !bClearEventLog && (MessageIndexIn == MessageIndexOut) && (AccelDelayTimer >= 5)) {
            AccelDelayTimer = 0;
            if (bInitializeAccel != 0) {
              TmpBuffer[0] = 0;
              I2C2Control.Address = 0x2A;
              I2C2Control.Length = 1;
              I2C2Control.Buffer = &TmpBuffer[0];
              I2C2Control.Mode = I2C_MODE_WRITE_ACCEL;
              State = 2;

            } else {
              I2C2Control.Address = 0x1;
              I2C2Control.Buffer = &TmpBuffer[0];               // Read 6 bytes, starting at address 0x1
              I2C2Control.Length = 6;
              I2C2Control.Mode = I2C_MODE_READ_ACCEL;
              State = 1;
            }
        }
        break;
    case 1:
        //
        //  Wait for X/Y/Z data to be read
        //
        //  Convert data so 0 Counts == -Full Scale, and 4095 counts == +Full Scale
        //
        if (I2C2Control.Mode == I2C_MODE_DONE) {
            SysVars.AccelXCounts = HostData.bManualAccelXEnable ? HostData.ManualAccelX : (int32_t)(((uint16_t)TmpBuffer[0] << 4) + ((TmpBuffer[1] >> 4) & 0xF)) ^ 0x800;
            SysVars.AccelYCounts = HostData.bManualAccelYEnable ? HostData.ManualAccelY : (int32_t)(((uint16_t)TmpBuffer[2] << 4) + ((TmpBuffer[3] >> 4) & 0xF)) ^ 0x800;
            SysVars.AccelZCounts = HostData.bManualAccelZEnable ? HostData.ManualAccelZ : (int32_t)(((uint16_t)TmpBuffer[4] << 4) + ((TmpBuffer[5] >> 4) & 0xF)) ^ 0x800;
            I2C2Control.Mode = I2C_MODE_IDLE;
            State = 0;
        }
        break;
    case 2:
        if (I2C2Control.Mode == I2C_MODE_DONE) {
            TmpBuffer[0] = (uint8_t)FlashConfiguration->AccelerometerScaling;
            I2C2Control.Address = 0x0E;
            I2C2Control.Length = 1;
            I2C2Control.Buffer = &TmpBuffer[0];
            I2C2Control.Mode = I2C_MODE_WRITE_ACCEL;
            State = 3;
        }
        break;
    case 3:
        if (I2C2Control.Mode == I2C_MODE_DONE) {
            TmpBuffer[0] = 1;
            I2C2Control.Address = 0x2A;
            I2C2Control.Length = 1;
            I2C2Control.Buffer = &TmpBuffer[0];
            I2C2Control.Mode = I2C_MODE_WRITE_ACCEL;
            State = 4;
        }
        break;
    case 4:
        if (I2C2Control.Mode == I2C_MODE_DONE) {
            bInitializeAccel = 0;
            I2C2Control.Mode = I2C_MODE_IDLE;
            State = 0;
        }
        break;
    default:
        Error_Handler(FILE_ACCEL, __LINE__);
        break;
    }
    ExecutionEnd(EXECUTION_PROCESS_ACCELEROMETER);
}
