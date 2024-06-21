// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: errorcondition.c
// Author  : Rick Wright/Charlie Urban
// Revision: 1.2
//
// Description:     Set/Reset any Digital I/O configured for Error Condition Output
//
// ------------------------------------------------------------------------
//
// ------------------------------------------------------------------------
//
// Revision Log:
//
//  1.2    CRU     07/26/2022 Integration of released fuel sender software and throttle SW
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
// ProcessErrorCondition - This routine is called every 20mS and if an error is present
//                         and a digital IO is set to CONFIG_DIG_ERROR_CONDITION, then
//                         that digital IO will be set to os FLASH programmed error state.
//
//
// Params:      None
//
// Returns:     None
//
void ProcessErrorCondition(void) {

  int8_t    ErrorLevel, bError;
  int32_t   i;

  ExecutionStart(EXECUTION_PROCESS_ERROR_CONDITION);
  //
  // If bErrorPresent flag is set, then there is currently an error being detected
  //
  bError = (SignalStatus.bErrorPresent[GENERAL_MOSFET_POWER] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_5VDC_LOW_LIMIT] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_5VDC_HIGH_LIMIT] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_LOW_INTERNAL_REFERENCE_LIMIT] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_HIGH_INTERNAL_REFERENCE_LIMIT] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_POWER_INPUT_LOW_LIMIT] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_POWER_INPUT_HIGH_LIMIT] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_KMA_LOW_LIMIT] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_KMA_HIGH_LIMIT] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_SP_FB_DIFF] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_CAN_SETPOINT_A_LOW_LIMIT] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_CAN_SETPOINT_A_HIGH_LIMIT] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_CAN_SETPOINT_B_LOW_LIMIT] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_CAN_SETPOINT_B_HIGH_LIMIT] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_RS485_NOT_UPDATING] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_SENSOR_MISMATCH] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_CAN_RECEPTION_ERROR] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_CAN_TRANSMISSION_EEROR] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_CAN_ECHO_ERROR] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_CAN_CRC_ERROR] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_BAD_FIRMWARE_CRC] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_BAD_CONFIGURATION_CRC] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_BAD_FACTORY_CRC] != 0) ||
           (SignalStatus.bErrorPresent[GENERAL_BAD_FUZZY_CRC] != 0);

  SysVars.ErrorDetected = bError;

  if (bFlashConfigurationValid != 0) {
    ErrorLevel = (FlashConfiguration->ErrorConditionActiveState == bError);

    for (i = 0; i < 6; i++) {
      if (DIG_HandleStruct[i].DIG_InitStruct.Config == CONFIG_DIG_ERROR_CONDITION) {
        if (ErrorLevel != 0) {
          DIG_SetPin(i);
        } else {
          DIG_ResetPin(i);
        }
      }
    }
  }
    ExecutionEnd(EXECUTION_PROCESS_ERROR_CONDITION);
}
