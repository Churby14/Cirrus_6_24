// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
// Filename: indexes.c
// Author  : Rick Wright/Charlie Urban
// Revision: 3.2
//
//  Description:        Index Calculation Logic
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

#define MOSFET_MAXPOWER         575         // x.xxx Watts


// ------------------------------------------------------------------------
//                  Local Conditional Compilation Switches
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//                          Local Variables
// ------------------------------------------------------------------------




// ------------------------------------------------------------------------
//
// ProcessResistanceOutputControl - This routine is called when the R/V Output is configured for resistance
//
// This routine adjusts the R/V Output voltage to maintain the correct
// output resistance.
//
// Params:    None
//
// Returns:   None
//
// ------------------------------------------------------------------------
//
// ProcessResistanceOutputControl - This routine is called when the R/V Output is configured for resistance
//
// This routine adjusts the R/V Output voltage to maintain the correct
// output resistance.
//
// Params:    None
//
// Returns:   None
//
static void ProcessResistanceOutputControl(void) {

    int32_t           Error, PwmPerOhm;
    static int32_t    UpdateDelayTimer, OldFeedback, P, D, DelayTimer;
    static int64_t    I;

    if (bRVUpdate != 0) {
        bRVUpdate = FALSE;

        DelayTimer++;
        if (DelayTimer < 5) {
            return;
        }
        DelayTimer = 0;

        SysVars.PwmDutyCycleFineTune = 0;
        PwmPerOhm = 100;
        if ((SysVars.RVFeedback != 0) && (SysVars.PwmDutyCycle != 0)) {
            PwmPerOhm = MultDivideWithRounding64(SysVars.PwmDutyCycle, 10000, SysVars.RVFeedback, RESULT_32);
        }
        Error = SysVars.RVSetpoint - SysVars.RVFeedback;
        P = MultDivideWithRounding64(Error, PwmPerOhm, 40000, RESULT_32);
        I += MultDivideWithRounding64(Error, PwmPerOhm, 100, RESULT_64);     //  xxx.xxxxxx

        if (I < 0) {
            I = 0;
        }
        if (I > (PWM_MAX_DUTY_CYCLE * 1000)) {
            I = PWM_MAX_DUTY_CYCLE * 1000;
        }

        UpdateDelayTimer++;
        if (UpdateDelayTimer >= 2) {
            UpdateDelayTimer = 0;
            D = ((SysVars.RVFeedback - OldFeedback) * 10) / 1000;
            OldFeedback = SysVars.RVFeedback;
        }

        SysVars.PwmDutyCycle = P + (uint32_t)(I / 1000) + D;
        if (SysVars.PwmDutyCycle > PWM_MAX_DUTY_CYCLE) {
            SysVars.PwmDutyCycle = PWM_MAX_DUTY_CYCLE;
        }
        if (SysVars.PwmDutyCycle < 0) {
            SysVars.PwmDutyCycle = 0;
        }
    }
}

// ------------------------------------------------------------------------
//
// This routine is called when the R/V Output is configured for voltage
//
// This routine adjusts the R/V Output voltage to maintain the correct
// output voltage.
//
// Params:    None
//
// Returns:   None
//
static void ProcessVoltageOutputControl(void) {

  int32_t NewDutyCycle;
  static uint8_t FineTuneDelay;
  //
  // 100000 = 100.000 %
  //
  NewDutyCycle = MultDivideWithRounding64(SysVars.RVSetpoint, FactoryConfiguration->Calibration33 * 3011, FactoryConfiguration->PwmOutputKFactor * 100, RESULT_32);
  //
  // In case of calibration error, fine tune PWM Duty Cycle
  //
  FineTuneDelay++;
  if (FineTuneDelay > 5) {             // 50mS
    FineTuneDelay = 0;

    if (((SysVars.RVSetpoint > SysVars.RVFeedback) && (SysVars.PwmDutyCycle < PWM_MAX_DUTY_CYCLE)) || ((SysVars.PwmDutyCycle + SysVars.PwmDutyCycleFineTune) < 0)) {
      SysVars.PwmDutyCycleFineTune++;
    }
    if (((SysVars.RVSetpoint < SysVars.RVFeedback) && (SysVars.PwmDutyCycle > 0)) || ((SysVars.PwmDutyCycle + SysVars.PwmDutyCycleFineTune) > PWM_MAX_DUTY_CYCLE)) {
      SysVars.PwmDutyCycleFineTune--;
    }
    //
    // Limit range of Fine Tuning
    //
    if (SysVars.PwmDutyCycleFineTune > 500) {
      SysVars.PwmDutyCycleFineTune = 500;
    }
    if (SysVars.PwmDutyCycleFineTune < -500) {
      SysVars.PwmDutyCycleFineTune = -500;
    }

  }

  SysVars.PwmDutyCycle = NewDutyCycle + SysVars.PwmDutyCycleFineTune;
  if (SysVars.PwmDutyCycle > PWM_MAX_DUTY_CYCLE) {
    SysVars.PwmDutyCycle = PWM_MAX_DUTY_CYCLE;
  }
  if (SysVars.PwmDutyCycle < 0) {
    SysVars.PwmDutyCycle = 0;
  }
}

// ------------------------------------------------------------------------
//
// Params:    None
//
// Returns:   None
//
void ProcessMasterIndex(void) {

  int16_t i;
  int32_t SetpointRange, IndexRange;

  ExecutionStart(EXECUTION_PROCESS_MASTER_INDEX);

  //
  // Calculate Master Index value.
  //  Master Index = KMAIndex + all the Slave Indexes
  //
  SysVars.MasterIndex = SysVars.KmaIndex;

  //
  //  KMA Index:  4.00 to 1023.00
  //
  SysVars.SetpointIndex = SysVars.MasterIndex;
  //
  // NOTE:  bManualSetpointEnable is set in serial.c
  //
  if (HostData.bManualSetpointEnable != 0) {
    SysVars.SetpointIndex = HostData.ManualSetpoint;
  }

  if (SysVars.SetpointIndex < MIN_INDEX) {
    SysVars.SetpointIndex = MIN_INDEX;
  }
  if (SysVars.SetpointIndex > MAX_INDEX) {
    SysVars.SetpointIndex = MAX_INDEX;
  }
  //
  // If the DIG_IO assigned as a Test Input is active (high), then set SetpointIndex to full scale (Index = 400 x.xx)
  //
  // NOTE:  TestInput is calculated in testinput.c
  //
  if (SysVars.TestInput != 0) {
    SysVars.SetpointIndex = MIN_INDEX;
  }
  //
  // If the Startup routine is not finished, all outputs go to their minimums
  //
  if (SysVars.bStartupComplete == 0) {
    SysVars.SetpointIndex = MIN_INDEX;
  }

  if (bFlashConfigurationValid != 0) {
    //
    // Lookup Setpoint from table based on SetpointIndex for the RV Output
    //
    // RV = Resistance or Voltage
    //
    //    Index   Setpoint    // Number of entries is 2 to 32
    //      4       xxx
    //      ...     ...
    //      1023    xxx
    //
    for (i = 1; i < FlashConfiguration->SetpointTable.TableLength; i++) {
      if (SysVars.SetpointIndex <= FlashConfiguration->SetpointTable.TablePoint[i].Point1) {
        IndexRange = FlashConfiguration->SetpointTable.TablePoint[i].Point1 - FlashConfiguration->SetpointTable.TablePoint[i - 1].Point1;
        SetpointRange = FlashConfiguration->SetpointTable.TablePoint[i].Point2 - FlashConfiguration->SetpointTable.TablePoint[i - 1].Point2;
        if (IndexRange != 0) {
          SysVars.RVSetpoint = MultDivideWithRounding64(SysVars.SetpointIndex - FlashConfiguration->SetpointTable.TablePoint[i - 1].Point1,
                             SetpointRange, IndexRange, RESULT_32) + (FlashConfiguration->SetpointTable.TablePoint[i - 1].Point2);
        }
        break;
      }
    }
    //
    // Lookup setpoint for Throttle
    //
    for (i = 1; i < FlashConfiguration->CanTable.TableLength; i++) {
      if (SysVars.SetpointIndex <= FlashConfiguration->CanTable.TablePoint[i].Point1) {
        IndexRange = FlashConfiguration->CanTable.TablePoint[i].Point1 - FlashConfiguration->CanTable.TablePoint[i - 1].Point1;
        SetpointRange = FlashConfiguration->CanTable.TablePoint[i].Point2 - FlashConfiguration->CanTable.TablePoint[i - 1].Point2;
        SysVars.CanSetpoint = MultDivideWithRounding64(SysVars.SetpointIndex - FlashConfiguration->CanTable.TablePoint[i - 1].Point1,
          SetpointRange, IndexRange, RESULT_32) + (FlashConfiguration->CanTable.TablePoint[i - 1].Point2);
        break;
      }
    }
  }

  if ((bFactoryConfigurationValid != 0) && (bFlashConfigurationValid != 0) && (((FlashConfiguration->RVOutputMode == CONFIG_RV_OUTPUT_MODE_DISABLED) || (FactoryConfiguration->RVOutputPresent == 0)))) {
    SysVars.RVSetpoint = 0;
    SysVars.RVFeedback = 0;
    SysVars.PwmDutyCycle = 0;
    SysVars.PwmDutyCycleFineTune = 0;
  }
  else if ((bFlashConfigurationValid != 0) && (FlashConfiguration->RVOutputMode == CONFIG_RV_OUTPUT_MODE_OHMS)) {
    if (SysVars.RVOutputOhms < 1000000) {
      SysVars.RVFeedback = SysVars.RVOutputOhms * 10;
    }
    else {
      SysVars.RVFeedback = 10000000;     // Limit to 10000.000 ohms
    }
    ProcessResistanceOutputControl();
  }
  else if (FlashConfiguration->RVOutputMode == CONFIG_RV_OUTPUT_MODE_VOLTAGE) {
    SysVars.RVFeedback = SysVars.RVOutputVoltage / 10;
    ProcessVoltageOutputControl();
  }
  // Both Throttles need a common setpoint, therefore, set the Local sensor setpoint value
  //
  // Master Frequency output
  //
  if (FlashConfiguration->DefaultOutput == 0 || FlashConfiguration->DefaultOutput == 1) {
    SysVars.ThrottleSetpoint = SysVars.MasterFrequency;
  }
  else if (FlashConfiguration->DefaultOutput == 2 || FlashConfiguration->DefaultOutput == 3) {
    SysVars.ThrottleSetpoint = SysVars.RVSetpoint;
  }
  else {
    SysVars.ThrottleSetpoint = SysVars.CanSetpoint;
  }


  ExecutionEnd(EXECUTION_PROCESS_MASTER_INDEX);
}
