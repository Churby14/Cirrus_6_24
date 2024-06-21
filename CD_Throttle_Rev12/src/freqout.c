// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: freqout.c
// Author  : Rick Wright
// Revision: C
//
// Description:     Frequency Output
//
// ------------------------------------------------------------------------
//
// ------------------------------------------------------------------------
//
// Revision Log:
//
//  A           CRU   Baseline Release   12/21/2021
//  B           RLW   06/27/2022   Changes per PR #19 in PACT
//  C           CRU   06/30/2022   Changes per PR #22 in PACT
//
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
// ProcessFrequency Outputs - This routine is called every 50mS (on average).
// Set the output frequency of any Digital I/O configured as a Temperature Frequency Output
//
// Params:    None
//
// Returns:   None
//
void ProcessFrequencyOutputs(void) {

  int32_t i;

  ExecutionStart(EXECUTION_PROCESS_FREQUENCY_OUTPUTS);
  //
  // Set Digital I/O for any that are configurated as Temperature Frequency
  //       Outputs
  //
  for (i = 0; i < 6; i++) {
    if (DIG_HandleStruct[i].DIG_InitStruct.Config == CONFIG_DIG_TEMP_FREQ_OUT) {
      DIG_SetFrequency(i, SysVars.TemperatureFrequency);
    }
    if (DIG_HandleStruct[i].DIG_InitStruct.Config == CONFIG_DIG_KMA_FREQ_OUT) {
      DIG_SetFrequency(i, SysVars.KmaFrequency);
    }
    if (DIG_HandleStruct[i].DIG_InitStruct.Config == CONFIG_DIG_MASTER_FREQ_OUT) {
      DIG_SetFrequency(i, SysVars.MasterFrequency);
    }
  }
  ExecutionEnd(EXECUTION_PROCESS_FREQUENCY_OUTPUTS);
}

// ------------------------------------------------------------------------
//
// ProcessMasterFrequencyOutput - This routine is called every 20mS (on average)
//
// Params:    None
//
// Returns:   None
//
void ProcessMasterFrequencyOutput(void) {

  int32_t i, IndexRange, SetpointRange;

  ExecutionStart(EXECUTION_PROCESS_MASTER_FREQUENCY_OUTPUT);

  //
  // If in ErrorMode, then toggle the Master Frequency Output between 10.00Hz and 20.00Khz
  //

  //
  // Calculate Master Frequency.
  //
  // In compatibly mode, Master Frequency = 65536 / Master Index
  // In Frequency Table Mode, Master Frequency is based on table
  //
  if (bFlashConfigurationValid != 0) {
    if (FlashConfiguration->MasterFreqMode != 0) {
      //
      // SetpointIndex
      //
      if (SysVars.SetpointIndex != 0) {
        SysVars.MasterFrequency = MultDivideWithRounding64(655360000 / (SlaveCount + 1), 1, SysVars.SetpointIndex, RESULT_32);
      }
      } else {
        for (i = 1; i < FlashConfiguration->MasterFreqTable.TableLength; i++) {
          if (SysVars.SetpointIndex <= FlashConfiguration->MasterFreqTable.TablePoint[i].Point1) {
            IndexRange = FlashConfiguration->MasterFreqTable.TablePoint[i].Point1 - FlashConfiguration->MasterFreqTable.TablePoint[i - 1].Point1;
            SetpointRange = FlashConfiguration->MasterFreqTable.TablePoint[i].Point2 - FlashConfiguration->MasterFreqTable.TablePoint[i - 1].Point2;
            if (IndexRange != 0) {
              SysVars.MasterFrequency = MultDivideWithRounding64(SysVars.SetpointIndex - FlashConfiguration->MasterFreqTable.TablePoint[i - 1].Point1, SetpointRange, IndexRange, RESULT_32) +
              (FlashConfiguration->MasterFreqTable.TablePoint[i - 1].Point2);
            }
            break;
          }
        }
      }
  }
  ExecutionEnd(EXECUTION_PROCESS_MASTER_FREQUENCY_OUTPUT);
}
