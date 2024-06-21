// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Filename:      throttle.c
// Author:        Charlie Urban
// Revision:      0.9
//
// Description:   This module contains functions that:
//						- Calculate the KmaIndex in an easy to transmit way
//
//
// ------------------------------------------------------------------------
//
// ------------------------------------------------------------------------
//
// Revision Log:
//
//  0.8		07/18/2023 - Sets the correct error bits when a sensor fault occurs
//
// ------------------------------------------------------------------------
//
// Design Details and Explanations:
//
// ------------------------------------------------------------------------
//
//					This code is for CAN communication for Throttle
//
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

// -------------------------------------------------------------------------
//									Local Variables
// -------------------------------------------------------------------------

static int32_t Tmp_Min, Tmp_Max, Avg_Noise;
static uint8_t Noise_Counter;
// ------------------------------------------------------------------------
//
// ThrottleStatus - First checks for any local errors and then looks for foreign errors
//                  to determine how to software should be proceed and what CAN messages
//                  should be sent out
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void ThrottleStatus (void) {

  uint8_t Tmp;

  SysVars.ErrorStatus = 0;

  // Check for an error on the local sensor card
  // and use this value to setup throttle error codes using the following
  // 0 = no error, 1 = SysVars.ErrorDetected, 2 = Setpoint A error,
  // 4 = Setpoint B error, 8 = RS485 error, 16 = Setpoint A and B mismatch
  // 32 = CAN not Updating, 64 = Local sensor lost mismatch comparison,
  // 128 = Using default flash or factory configuration -> FLASH was corrupted
  // Note that these are OR'd together
  // Find if any error other than those specific to the throttle exists
  for (uint8_t i = 0; i <= GENERAL_SP_FB_DIFF; i++) {
    if (SignalStatus.bErrorPresent[i] != 0) {
      SysVars.ErrorStatus |= 0x01u;
    }
  }
  for (uint8_t i = GENERAL_BAD_FIRMWARE_CRC; i <= GENERAL_BAD_FUZZY_CRC; i++) {
    if (SignalStatus.bErrorPresent[i] != 0) {
      SysVars.ErrorStatus |= 0x01u;
    }
  }

  SysVars.ErrorStatus |= (((SignalStatus.bErrorPresent[GENERAL_CAN_SETPOINT_A_LOW_LIMIT] || SignalStatus.bErrorPresent[GENERAL_CAN_SETPOINT_A_HIGH_LIMIT] || SignalStatus.bErrorPresent[GENERAL_SENSOR_A_QUALITY]) << 1) |
                        ((SignalStatus.bErrorPresent[GENERAL_CAN_SETPOINT_B_LOW_LIMIT] || SignalStatus.bErrorPresent[GENERAL_CAN_SETPOINT_B_HIGH_LIMIT] || SignalStatus.bErrorPresent[GENERAL_SENSOR_B_QUALITY]) << 2) |
                        ((SignalStatus.bErrorPresent[GENERAL_RS485_NOT_UPDATING]) << 3) |
                        ((SignalStatus.bErrorPresent[GENERAL_SENSOR_MISMATCH]) << 4) |
                        (((SignalStatus.bErrorPresent[GENERAL_CAN_RECEPTION_ERROR]) || (SignalStatus.bErrorPresent[GENERAL_CAN_TRANSMISSION_EEROR]) ||
                         (SignalStatus.bErrorPresent[GENERAL_CAN_ECHO_ERROR]) || (SignalStatus.bErrorPresent[GENERAL_CAN_CRC_ERROR])) << 5) |
                         (((bUseDefaultFlash != 0) || (bUseDefaultFactory != 0)) << 6));

  // Reset error bits before setting
  SysVars.MinorFault = 0x0000u;
  SysVars.MajorFault = 0x0000u;
  SysVars.CriticalFault = 0x0000u;
  SysVars.ErrorType = 0u;
  SysVars.MatchStatus = 0x01u; // Assume they match to start

  if (SysVars.ErrorStatus != 0 || ForeignSysVars.ErrorStatus != 0) {

    //  RS485 Error
    if (SysVars.ErrorStatus & 0x08u) {
      SysVars.ErrorType |= 0x02u; // Major Error Type
      SysVars.MatchStatus = 0x03u; // Not available due to insufficient data
      // The opposite sensor is unavailable
      if (SysVars.SensorLetter == 0) {
        Tmp = 1;
      }
      else {
        Tmp = 0;
      }
      SysVars.MajorFault |= 0x0100u << Tmp; // Sensor A or Sensor B Unavailble
      // If RS485 Error occurs - either the opposite sensor lost power
      // or the RS485 connection is bad, etc. Therefore, overwrite the foregin
      // sensor's setpoint data with the local
      ForeignSysVars.ThrottleSetpoint = SysVars.ThrottleSetpoint;
      ForeignSysVars.CanSetpoint = SysVars.CanSetpoint;
      ForeignSysVars.SensorLetter = Tmp;
    }
    else {
      // Sensor Mismatch - Cannot have RS485 error
      if ((SysVars.ErrorStatus & 0x10u)) {
        SysVars.ErrorType |= 0x02u; //Major Error Type
        SysVars.MatchStatus = 0x00u; // Sensors do not match
        SysVars.MajorFault |= 0x04u; // Mismatch compare
        ThrottleMisMatch();

      }
      // Error on CAN A and CAN B, cannot have RS485 Error
      if ((SysVars.ErrorStatus & 0x20u) && (ForeignSysVars.ErrorStatus & 0x20u)) {
        SysVars.ErrorType |= 0x01u; // Hazardous
        SysVars.CriticalFault |= 0x02u; // CAN A and CAN B failure
      }
      // Error only on foreign CAN Bus, canont have RS485 error
      if (((SysVars.ErrorStatus & 0x20u) == 0x00u) && (ForeignSysVars.ErrorStatus & 0x20u)) {
        SysVars.ErrorType |= 0x02u; // Major
        SysVars.MajorFault |= 0x10u << ForeignSysVars.SensorLetter; // CAN A/B failure depending on foreign sensor
      }
      // Both sensors are using default factory/flash
      if ((SysVars.ErrorStatus & 0x40u) && (ForeignSysVars.ErrorStatus & 0x40u)) {
        SysVars.ErrorType |= 0x01u; // Hazardous
        SysVars.CriticalFault |= 0x01u;
      }
      // Foreign sensor using default factory/flash
      // Using default Flash/factory settings - indicated FLASH was corrupted
      if (ForeignSysVars.ErrorStatus & 0x40u) {
        SysVars.ErrorType |= 0x02u; // Major error type
        SysVars.MajorFault |= 0x04u << ForeignSysVars.SensorLetter;
      }
      // If both sensors have other failure then raise error
      if ((SysVars.ErrorStatus == 0x01u) && (ForeignSysVars.ErrorStatus == 0x01u)) {
        SysVars.ErrorType |= 0x01u; // Hazardous
        SysVars.CriticalFault |= 0x01u;
      }

      // If only another type of error exists - then sensor failure
      if (ForeignSysVars.ErrorStatus == 0x01u) {
        SysVars.ErrorType |= 0x02u;
        SysVars.MajorFault |= 0x40u << ForeignSysVars.SensorLetter;
      }
    }
    // Setpoint/Sensor A Error and Setpoint/Sensor B Error
    if ((SysVars.ErrorStatus & 0x02u) && (SysVars.ErrorStatus & 0x04u)) {
      SysVars.ErrorType |= 0x01u; // Hazardous error
      SysVars.CriticalFault |= 0x01u;
    }

    // Setpoint/Sensor A Error, no Setpoint/Sensor B Error
    if ((SysVars.ErrorStatus & 0x02u) && ((SysVars.ErrorStatus & 0x04u) == 0)) {
      SysVars.ErrorType |= 0x02u; // Major error
      SysVars.MajorFault |= 0x40u; // Sensor A Failure
    }
    // Setpoint/Sensor A Error, no Setpoint/Sensor B Error
    if (((SysVars.ErrorStatus & 0x02u) == 0) && (SysVars.ErrorStatus & 0x04u)) {
      SysVars.ErrorType |= 0x02u; // Major error
      SysVars.MajorFault |= 0x80u; // Sensor B Failure
    }
    // Error on local CAN Bus
    if (SysVars.ErrorStatus & 0x20u) {
      SysVars.ErrorType |= 0x02u; // Major
      SysVars.MajorFault |= 0x10u << SysVars.SensorLetter;
    }
    // Using default Flash/factory settings - indicated FLASH was corrupted
    if (SysVars.ErrorStatus & 0x40u) {
      SysVars.ErrorType |= 0x02u; // Major error type
      SysVars.MajorFault |= 0x04u << SysVars.SensorLetter;
    }
    // If only another type of error exists - then sensor failure
    if (SysVars.ErrorStatus == 0x01u) {
      SysVars.ErrorType |= 0x02u;
      SysVars.MajorFault |= 0x40u << SysVars.SensorLetter;
    }
  }
  // Set CanBus control mode message = 7 for Torque mode (% Nm)
  SysVars.ControlMode = 0x07;
}

// ------------------------------------------------------------------------
//
// Noise - Calculate the noise of sensor KMA data. Noise is defined as:
//           - Rolling average of 100 KmaCounts where the difference between
//             the max and min KmaCounts is less than 40.
//                 - Difference of 100 KmaCounts was chosen after testing
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void Noise(void) {

  // Initialize old_min to be much greater than the real min
  // Initialize old_max to be much smaller than the real max
  int32_t old_min, old_max, current_noise;

  if (Noise_Counter < 100) {

    old_min = Min(Tmp_Min, SysVars.KmaCounts);
    old_max = Max(Tmp_Max, SysVars.KmaCounts);
    Tmp_Min = old_min;
    Tmp_Max = old_max;
    Noise_Counter++;

  }
  else if (Noise_Counter == 100) {

    current_noise = Tmp_Max - Tmp_Min;
    Noise_Counter++;

    // 40 counts was chosen after inital testing as a good indication
    // whether the throttle arm was moving or now
    if (current_noise < 40) {
      ThrottleInfo.Moving = FALSE;
      if (Avg_Noise == 0) {
        Avg_Noise = current_noise;
      }
      else {
        Avg_Noise = ExpAverage(Avg_Noise, current_noise);
      }
    }
    else {
      ThrottleInfo.Moving = TRUE;
    }
  }
  else {
    SysVars.Noise = Avg_Noise * 10;
    if (SysVars.bStartupComplete == 0 && SysVars.Noise >= 0 && SysVars.Noise < 100) {
      SysVars.bStartupComplete = 1;
    }
    Noise_Counter = 0;
    Tmp_Min = SysVars.KmaCounts;
    Tmp_Max = SysVars.KmaCounts;
  }
}

// ------------------------------------------------------------------------
//
// ExpAverage - Calculate the exponential average of the sample data
//
// Params:    avg: previous average
//            sample: Newest data point
//
// Returns:   The newest exponential average
//
// Requirement(s):
//
int32_t ExpAverage(int32_t avg, int32_t sample) {

  int32_t average;
  int32_t alpha; // alpha controls the weight put on the current sample

  // Average = Average * (1-alpha) + current_sample * alpha
  alpha = 10; // value between 1-100 (0.01-1.00)

  average = (avg*alpha + sample * (100-alpha))/100;

  return average;
}

// ------------------------------------------------------------------------
//
// ThrottleMisMatch - If the two throttle sensors disagree, this function determines
//                    which sensor is correct by running the fuzzy logic tests
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void ThrottleMisMatch(void) {

  // Code to run if throttle sensors mismatch
  // But only run the code on the first mismatch error
  if (bFuzzyPageValid != 0 && SysVars.bStartupComplete != 0) {
    while (SysVars.Quality1 == 0) {

      Qual_1(); // First run quality check 1 to see if there is a clear error
    }
  }
}

// ------------------------------------------------------------------------
//
// AngularConversion - Converts SysVars.KmaAngle to angle where the first index in the setpoint
//                     table corresponds to 0 deg
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void AngularConversion(void) {

  uint8_t i;
  int32_t SetpointRange, IndexRange;

  for (i = 1; i < FlashConfiguration->AngularOutput.TableLength; i++) {
    if (SysVars.SetpointIndex <= FlashConfiguration->AngularOutput.TablePoint[i].Point1) {
      IndexRange = FlashConfiguration->AngularOutput.TablePoint[i].Point1 - FlashConfiguration->AngularOutput.TablePoint[i - 1].Point1;
      SetpointRange = FlashConfiguration->AngularOutput.TablePoint[i].Point2 - FlashConfiguration->AngularOutput.TablePoint[i - 1].Point2;
      if (IndexRange != 0) {
        SysVars.SensorAngle = MultDivideWithRounding64(SysVars.SetpointIndex - FlashConfiguration->AngularOutput.TablePoint[i - 1].Point1,
                           SetpointRange, IndexRange, RESULT_32) + (FlashConfiguration->AngularOutput.TablePoint[i - 1].Point2);
      }
      break;
    }
  }

}

// ------------------------------------------------------------------------
//
// ProcessThrottle - Controls the Power Lever functions
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void ProcessThrottle(void) {

  ExecutionStart(EXECUTION_PROCESS_THROTTLE);

  AngularConversion();

  Noise();
  ThrottleStatus();

  ExecutionEnd(EXECUTION_PROCESS_THROTTLE);
}
