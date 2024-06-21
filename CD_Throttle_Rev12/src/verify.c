// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: verify.c
// Author  : Rick Wright/Charlie Urban
// Revision: 3.2
//
// Description:     Verification of Signals
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Revision Log:
//
//  3.4    CRU     07/18/2023 Updated mismatch tolerance
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

#define POWERRAIL5VDC_LOWLIMIT      (5000 - 75)             // x.xxx volts
#define POWERRAIL5VDC_HIGHLIMIT     (5000 + 75)             // x.xxx volts
#define INTERNALREF_LOWLIMIT        1490                    // xxxx counts
#define INTERNALREF_HIGHLIMIT       1555                    // xxxx counts
#define POWERINPUT_LOWLIMIT         9000                    // x.xxx volts
#define POWERINPUT_HIGHLIMIT        33000                   // x.xxx volts
#define KMA_LOWLIMIT                4000                    // xx.xxx %
#define KMA_HIGHLIMIT               96000                   // xx.xxx %
#define TEMPERATURE_LOWLIMIT        -6000                   // xx.xx Deg F
#define TEMPERATURE_HIGHLIMIT       12000                   // xx.xx Deg F
#define SLAVEFREQ_LOWLIMIT          6405                    // xxxxx.xx Hz  64.06
#define SLAVEFREQ_HIGHLIMIT         1638500                 // xxxxx.xx Hz  16384
#define STACK_SLACK                 100                     // xxxx bytes
#define MOSFET_MAXPOWER             600                     // x.xxx W
#define CURRENT_SHUNT_RESISTANCE    100                     // x.xx Ohms
#define SETPOINT_TOLERANCE          768 // 3% of full scale

// ------------------------------------------------------------------------
//                  Local Conditional Compilation Switches
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//                          Local Variables
// ------------------------------------------------------------------------
static uint8_t    Old_RS485_Counter; // Keeps track of the previous counter term in RS485 message
static int16_t    PowerupDelay;

// ------------------------------------------------------------------------
//
// This routine is called every 1 second from Main Loop to verify stack size is within slack
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void ProcessVerifyStack(void) {

    uint32_t *Ptr;

    Ptr = &_ebss;
    SysVars.Slack = 0;
    while (1) {
        if ((Ptr == &_estack) || (*Ptr != 0x12345678)) {
            break;
        }
        SysVars.Slack++;
        Ptr++;
    }
    SysVars.StackUsed = (int32_t)(&_estack - Ptr);
    if (SysVars.Slack < STACK_SLACK) {
        Error_Handler(FILE_VERIFY, __LINE__);
    }
}

// ------------------------------------------------------------------------
//
// SetGeneralError - Checks that an error has occured for a specific amount of time before raising an error flag
//
// Params:    ErrorId - ID # of the error
//            bError - bit set if an error is present, reset if no error is present for the ID #
//            DebounceTime - Amount of time x10 (in ms) that an error must be present before raising error flag
//            Value1 - error value to be logged
//            Value2 - error limit that was exceeded to be logged
//
// Returns:   None
//
// Requirement(s):
//
static void SetGeneralError(uint32_t ErrorId, uint8_t bError, int16_t DebounceTime, int32_t Value1, int32_t Value2) {

    //
    // If No Current Error, then reset ErrorTimer back to 0.
    //
    if (bError == 0) {
        SignalStatus.GeneralErrorTimer[ErrorId] = 0;
        SignalStatus.bErrorPresent[ErrorId] = 0;
    } else {
        //
        // ErrorTimer is basically a debounce timer.  The error must be present for the debounce time before the
        // error is logged to EEPROM.
        //
        if (SignalStatus.GeneralErrorTimer[ErrorId] < DebounceTime) {
            SignalStatus.GeneralErrorTimer[ErrorId]++;
            if (SignalStatus.GeneralErrorTimer[ErrorId] == DebounceTime) {
                SignalStatus.bErrorPresent[ErrorId] = 1;
                LogEvent(EVENT_MESSAGE_ID_7 + ErrorId, Value1, Value2);
            }
        }
    }
}

// ------------------------------------------------------------------------
//
// ProcessSignalStatus - This routine is called every 10ms from Main Loop and sets the error flag requirements
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void ProcessSignalStatus(void) {

  int32_t   Diff, MinSetpoint, MaxSetpoint;
  uint8_t   bRvOutputEnabled, bKmaEnabled, bTemperatureEnabled;

  ExecutionStart(EXECUTION_PROCESS_SIGNAL_STATUS);
  //
  // Delay for 10 seconds before looking for errors
  //
  if (PowerupDelay < 500 || SysVars.bStartupComplete == 0) {
    PowerupDelay++;
    goto ExitHere;
  }

  bRvOutputEnabled = ((bFlashConfigurationValid != 0) && (bFactoryConfigurationValid != 0) && (FactoryConfiguration->RVOutputPresent != 0) && (FlashConfiguration->RVOutputMode != CONFIG_RV_OUTPUT_MODE_DISABLED));
  bKmaEnabled = ((bFactoryConfigurationValid != 0) && (FactoryConfiguration->KmaPresent != 0));
  bTemperatureEnabled = ((bFactoryConfigurationValid != 0) && (FactoryConfiguration->TempSensorPresent != 0));

  // If using CAN Bus, calculate min and max setpoint
  if (FlashConfiguration->CanUse != 0) {
    MinSetpoint = Min(FlashConfiguration->CanTable.TablePoint[FlashConfiguration->CanTable.TableLength - 1].Point2,
                      FlashConfiguration->CanTable.TablePoint[0].Point2);

    MaxSetpoint = Max(FlashConfiguration->CanTable.TablePoint[FlashConfiguration->CanTable.TableLength - 1].Point2,
                      FlashConfiguration->CanTable.TablePoint[0].Point2);
  }
  //
  // Verify Setpoint A is in bounds - CanUse must be 1 or 3, but not 0 or 2 which will be TRUE when bitwuse AND with 0x01
  //
  if (FlashConfiguration->CanUse != 0) {
    if (SysVars.SensorLetter == 0) {

      SetGeneralError(GENERAL_CAN_SETPOINT_A_LOW_LIMIT, SysVars.CanSetpoint < MinSetpoint, 100, SysVars.CanSetpoint, MinSetpoint);
      SetGeneralError(GENERAL_CAN_SETPOINT_A_HIGH_LIMIT, SysVars.CanSetpoint > MaxSetpoint, 100, SysVars.CanSetpoint, MaxSetpoint);
    }
    else {
      SetGeneralError(GENERAL_CAN_SETPOINT_A_LOW_LIMIT, ForeignSysVars.CanSetpoint < MinSetpoint, 100, ForeignSysVars.CanSetpoint, MinSetpoint);
      SetGeneralError(GENERAL_CAN_SETPOINT_A_HIGH_LIMIT, ForeignSysVars.CanSetpoint > MaxSetpoint, 100, ForeignSysVars.CanSetpoint, MaxSetpoint);
    }
  }
  //
  // Verify Setpoint B is in bounds is using CAN B
  //
  if (FlashConfiguration->CanUse != 0) {
    if (SysVars.SensorLetter == 0) {
      SetGeneralError(GENERAL_CAN_SETPOINT_B_LOW_LIMIT, ForeignSysVars.CanSetpoint < MinSetpoint, 100, ForeignSysVars.CanSetpoint, MinSetpoint);
      SetGeneralError(GENERAL_CAN_SETPOINT_B_HIGH_LIMIT, ForeignSysVars.CanSetpoint > MaxSetpoint, 100, ForeignSysVars.CanSetpoint, MaxSetpoint);
    }
    else {
      SetGeneralError(GENERAL_CAN_SETPOINT_B_LOW_LIMIT, SysVars.CanSetpoint < MinSetpoint, 100, SysVars.CanSetpoint, MinSetpoint);
      SetGeneralError(GENERAL_CAN_SETPOINT_B_HIGH_LIMIT, SysVars.CanSetpoint > MaxSetpoint, 100, SysVars.CanSetpoint, MaxSetpoint);
    }
  }
  //
  //
  // Verify RS485 Counter is Updating
  //
  SetGeneralError(GENERAL_RS485_NOT_UPDATING, (SysVars.RS485Counter == Old_RS485_Counter) && (FlashConfiguration->RS485MessageMode == 2), 100, Old_RS485_Counter, Old_RS485_Counter + 1);
  Old_RS485_Counter = SysVars.RS485Counter;
  //
  // Verify Kma Indices match
  //
  Diff = abs(SysVars.ThrottleSetpoint - ForeignSysVars.ThrottleSetpoint);
  SetGeneralError(GENERAL_SENSOR_MISMATCH, Diff > SETPOINT_TOLERANCE, 300, SETPOINT_TOLERANCE, Diff);
  //
  // Verify quality scores if sensors do not match
  //
  if (ForeignSysVars.Quality1 > 0 && SysVars.Quality1 > 0) { // Means that throttle_x has sent over its quality1 value
    if (SysVars.SensorLetter == 0) {
      // If sensor A quality is less than or equal to B, then B becomes the default
      SetGeneralError(GENERAL_SENSOR_A_QUALITY, (SysVars.Quality1 <= ForeignSysVars.Quality1) && (Diff > SETPOINT_TOLERANCE), 10, SysVars.Quality1, ForeignSysVars.Quality1);
      SetGeneralError(GENERAL_SENSOR_B_QUALITY, (SysVars.Quality1 > ForeignSysVars.Quality1) && (Diff > SETPOINT_TOLERANCE), 10, ForeignSysVars.Quality1, SysVars.Quality1);
    }
    else {
      SetGeneralError(GENERAL_SENSOR_B_QUALITY, (SysVars.Quality1 < ForeignSysVars.Quality1) && (Diff > SETPOINT_TOLERANCE), 10, ForeignSysVars.Quality1, SysVars.Quality1);
      SetGeneralError(GENERAL_SENSOR_A_QUALITY, (SysVars.Quality1 >= ForeignSysVars.Quality1) && (Diff > SETPOINT_TOLERANCE), 10, SysVars.Quality1, ForeignSysVars.Quality1);
    }
  }
  //
  // Verify CAN Bus Is Functioning
  //
  SetGeneralError(GENERAL_CAN_RECEPTION_ERROR, CanBus.Status & 0x01u, 1, 0, 1);
  SetGeneralError(GENERAL_CAN_TRANSMISSION_EEROR, CanBus.Status & 0x02u, 100, 0, 1);
  SetGeneralError(GENERAL_CAN_ECHO_ERROR, CanBus.Status & 0x04u, 100, 0, 1);
  SetGeneralError(GENERAL_CAN_CRC_ERROR, CanBus.Status & 0x08u, 100, 0, 1);

  //
  // Verify +5VDC Power Supply is within Limits
  //
  SetGeneralError(GENERAL_5VDC_LOW_LIMIT, SysVars.PowerRail5VDC < POWERRAIL5VDC_LOWLIMIT, 10, SysVars.PowerRail5VDC, POWERRAIL5VDC_LOWLIMIT);
  SetGeneralError(GENERAL_5VDC_HIGH_LIMIT, SysVars.PowerRail5VDC > POWERRAIL5VDC_HIGHLIMIT, 10, SysVars.PowerRail5VDC, POWERRAIL5VDC_HIGHLIMIT);
  //
  // Verify Internal Reference Counts are within limits
  //
  SetGeneralError(GENERAL_LOW_INTERNAL_REFERENCE_LIMIT, SysVars.IntRefCounts < INTERNALREF_LOWLIMIT, 10, SysVars.IntRefCounts, INTERNALREF_LOWLIMIT);
  SetGeneralError(GENERAL_HIGH_INTERNAL_REFERENCE_LIMIT, SysVars.IntRefCounts > INTERNALREF_HIGHLIMIT, 10, SysVars.IntRefCounts, INTERNALREF_HIGHLIMIT);
  //
  // Verify MOSFET Power
  //
  SetGeneralError(GENERAL_MOSFET_POWER, (bRvOutputEnabled != 0) && (SysVars.MosfetPower > MOSFET_MAXPOWER), 10, SysVars.MosfetPower, MOSFET_MAXPOWER);
  //
  // Verify Power Input Voltage is within limits
  //
  SetGeneralError(GENERAL_POWER_INPUT_LOW_LIMIT, SysVars.PowerInputVoltage < POWERINPUT_LOWLIMIT, 100, SysVars.PowerInputVoltage, POWERINPUT_LOWLIMIT);
  SetGeneralError(GENERAL_POWER_INPUT_HIGH_LIMIT, SysVars.PowerInputVoltage > POWERINPUT_HIGHLIMIT, 100, SysVars.PowerInputVoltage, POWERINPUT_HIGHLIMIT);
  //
  // Verify KMA Percentage is within limits
  //
  SetGeneralError(GENERAL_KMA_LOW_LIMIT, (bKmaEnabled != 0) && (SysVars.KmaPercentage < KMA_LOWLIMIT), 100, SysVars.KmaPercentage, KMA_LOWLIMIT);
  SetGeneralError(GENERAL_KMA_HIGH_LIMIT, (bKmaEnabled != 0) && (SysVars.KmaPercentage > KMA_HIGHLIMIT), 100, SysVars.KmaPercentage, KMA_HIGHLIMIT);
  //
  // Verify Temperature is within Limits
  //
  SetGeneralError(GENERAL_TEMPERATURE_LOW_LIMIT, (bTemperatureEnabled != 0) && (SysVars.TemperatureCelsius < TEMPERATURE_LOWLIMIT), 10, SysVars.TemperatureCelsius, TEMPERATURE_LOWLIMIT);
  SetGeneralError(GENERAL_TEMPERATURE_HIGH_LIMIT, (bTemperatureEnabled != 0) && (SysVars.TemperatureCelsius > TEMPERATURE_HIGHLIMIT), 10, SysVars.TemperatureCelsius, TEMPERATURE_HIGHLIMIT);
  //
  // Check that Setpoint and Feedback are within limits
  //
  int32_t MinOutput = ((SysVars.RVOutputCurrent * CURRENT_SHUNT_RESISTANCE) / 100000) + 2;

  if (FlashConfiguration->RVOutputMode == CONFIG_RV_OUTPUT_MODE_VOLTAGE) {
    Diff = abs(SysVars.RVSetpoint - SysVars.RVFeedback);
    SetGeneralError(GENERAL_SP_FB_DIFF, (bRvOutputEnabled != 0) && (Diff > 20) && (SysVars.RVFeedback > MinOutput), 500, SysVars.RVSetpoint, SysVars.RVFeedback);
  }
    //
    // Verify CRC's
    //
    bFlashConfigurationValid = (FlashConfiguration->Crc16 == CalculatedCrc.FlashConfiguration);
    if ((bFlashConfigurationValid == 0) && (bDefaultProfileLoaded == 0)) {
      bGetDefaultProfile = 1; // Set flag to load in default Flash Configuration
    }
    bFuzzyPageValid = bUseDefaultFuzzy || (FuzzyPage->Fuzzy2.Crc16 == CalculatedCrc.FuzzyConfiguration);
    bFactoryConfigurationValid = (FactoryConfiguration->Crc16 == CalculatedCrc.FactoryConfiguration);
    bFirmwareValid = (FactoryConfiguration->FirmwareCrc16 == CalculatedCrc.Firmware);

    SetGeneralError(GENERAL_BAD_FIRMWARE_CRC, bFirmwareValid == 0, 10, CalculatedCrc.Firmware, FactoryConfiguration->FirmwareCrc16);
    SetGeneralError(GENERAL_BAD_CONFIGURATION_CRC, bFlashConfigurationValid == 0, 10, CalculatedCrc.FlashConfiguration, FlashConfiguration->Crc16);
    SetGeneralError(GENERAL_BAD_FACTORY_CRC, bFactoryConfigurationValid == 0, 10, CalculatedCrc.FactoryConfiguration, FactoryConfiguration->Crc16);
    SetGeneralError(GENERAL_BAD_FUZZY_CRC, bFuzzyPageValid == 0, 10, CalculatedCrc.FuzzyConfiguration, FuzzyPage->Fuzzy2.Crc16);

ExitHere:
  ExecutionEnd(EXECUTION_PROCESS_SIGNAL_STATUS);
}
