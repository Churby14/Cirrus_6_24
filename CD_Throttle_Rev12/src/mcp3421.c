// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: mcp3421.c
// Author  : Rick Wright
// Revision: C
//
// Description:       When I2C, which is connected to MCP3421 A/D chip, is
//                      idle, place I2C back into read mode.
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Revision Log:
//
//  A           CRU   12/21/2021   Baseline Release
//  B           RLW   12/21/2021   Changes per PR #19 in PACT
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
#include "math.h"

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
// InitializeMcp3421 - Routine to initialize the external Analog-to-Digital
//                     Converter used to read R/V output
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void InitializeMcp3421(void) {

    static uint8_t TmpBuf[1];

    if ((bFactoryConfigurationValid != 0) && (FactoryConfiguration->RVOutputPresent != 0)) {
        TmpBuf[0] = 0;
        I2C1Control.Length = 1;
        I2C1Control.Buffer = &TmpBuf[0]; // Set to One-Shot Conversion Mode
        I2C1Control.Mode = I2C_MODE_WRITE_MCP3421;
        do {
            ProcessI2C1();
        } while (I2C1Control.Mode != I2C_MODE_DONE);
        I2C1Control.Mode = I2C_MODE_IDLE;
    }
}

// ------------------------------------------------------------------------
//
// ProcessMcp3421 - Called at the beginning of the main() routine, this
//                  routine is used to handle the processing of data from the ADC
//
// Params:    None
//
// Returns:   Edits the SysVars data structure
//
// Requirement(s):
//
void ProcessMcp3421(void) {

    int32_t        AdcCounts;
    static uint8_t State;
    static uint8_t TmpBuffer[3];

    ExecutionStart(EXECUTION_PROCESS_MCP3421);

    switch (State) {
    case 0:
        STM32_GPIO_WritePin(MUX_SEL_GPIO_Port, MUX_SEL_Pin, GPIO_PIN_RESET);
        TmpBuffer[0] = (1u << 7) | (1u << 2) | (0u << 0); // RDY=1, Sample Rate = 60 SPS (14 bits), PGA=x1    R/V Output Voltage
        I2C1Control.Buffer = &TmpBuffer[0];
        I2C1Control.Length = 1;
        I2C1Control.Mode = I2C_MODE_WRITE_MCP3421;
        State++;
        break;
    case 1:
        if (I2C1Control.Mode == I2C_MODE_DONE) {
            I2C1Control.Length = 3;
            I2C1Control.Buffer = &TmpBuffer[0];
            I2C1Control.Mode = I2C_MODE_READ_MCP3421;
            State++;
        }
        break;
    case 2:
        if (I2C1Control.Mode == I2C_MODE_DONE) {
            if ((TmpBuffer[2] & 0x80) != 0) {
                State = 1;
            } else {
                AdcCounts = (int32_t)(((uint16_t)TmpBuffer[0] << 8) | TmpBuffer[1]);
                SysVars.RVOutputVoltageCounts = HostData.bManualRVOutputVoltageEnable ? HostData.ManualRVOutputVoltage : AdcCounts; // R/V Voltage
                State++;
            }
        }
        break;
    case 3:
        STM32_GPIO_WritePin(MUX_SEL_GPIO_Port, MUX_SEL_Pin, GPIO_PIN_SET);
        TmpBuffer[0] = (1u << 7) | (1u << 2) | (3u << 0); // RDY=1, Sample Rate = 60 SPS (14 bits), PGA=8     R/V Output Current
        I2C1Control.Buffer = &TmpBuffer[0];
        I2C1Control.Length = 1;
        I2C1Control.Mode = I2C_MODE_WRITE_MCP3421;
        State++;
        break;
    case 4:
        if (I2C1Control.Mode == I2C_MODE_DONE) {
            I2C1Control.Length = 3;
            I2C1Control.Buffer = &TmpBuffer[0];
            I2C1Control.Mode = I2C_MODE_READ_MCP3421;
            State++;
        }
        break;
    case 5:
        if (I2C1Control.Mode == I2C_MODE_DONE) {
            if ((TmpBuffer[2] & 0x80) != 0) {
                State = 4;
            } else {
                AdcCounts = (int32_t)(((uint16_t)TmpBuffer[0] << 8) | TmpBuffer[1]);
                SysVars.RVOutputCurrentCounts = HostData.bManualRVOutputCurrentEnable ? HostData.ManualRVOutputCurrent : AdcCounts; // R/V Voltage
                bRVUpdate = TRUE;
                I2C1Control.Mode = I2C_MODE_IDLE;
                State = 0;
            }
        }
        break;
    default:
        Error_Handler(FILE_MCP3421, __LINE__);
        break;
    }
    ExecutionEnd(EXECUTION_PROCESS_MCP3421);
}
