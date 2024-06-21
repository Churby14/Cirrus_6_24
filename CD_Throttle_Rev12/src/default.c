// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: default.c
// Author  : Charlie Urban
// Revision: C
//
//  Description:     Gets and sets the default output profile in the event flash
//                   is corrupt
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Revision Log:
//
//  0.1           CRU   03/03/2023   Initial Creation
//
// ------------------------------------------------------------------------
// Design Details and Explanations:
// ------------------------------------------------------------------------
//
// The first 2024 entries of the EEPROM (16 bytes each) are used for the event log.
// The defaul profile is stored in the next 24 entries (16 bytes each) and is logged
// with the following format:
// Bytes 0-4: 0xAA
// Byte 5: 0x00
// Byte 6: Output Mode - 0 = Freq comp, 1 = freq table, 2 = volt, 3 = resistive, 4 = CAN, 5 = Error
// Bytes 7-266: Setpoint Table, 0's if using freq comp or data is unused
// Bytes 267-268: 0xBB
// Bytes 269-383: 0xFF
// If the data does not follow this format (specifically the 0xAAs and 0xBBs are not found)
// then must resort to another default output

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
static uint8_t TmpBuffer[2304]; // 2048+256 = 18 pages of 128 bytes
// ------------------------------------------------------------------------

void ProcessLogDefaultProfile(void) {

  static uint8_t State, i;
  static uint16_t Idx;

  switch(State) {
  // Set default profile into the buffer
  case 0:
    if ((bLogDefaultProfile != 0) && (I2C2Control.Mode == I2C_MODE_IDLE)) {
      TmpBuffer[0] = 0xAA;
      TmpBuffer[1] = 0xAA;
      TmpBuffer[2] = 0xAA;
      TmpBuffer[3] = 0xAA;
      TmpBuffer[4] = 0xAA;
      TmpBuffer[5] = 0x00;
      memmove(&TmpBuffer[6], FlashConfiguration, sizeof(stFlashConfiguration));

      Idx = sizeof(stFlashConfiguration) + 6;
      TmpBuffer[Idx++] = 0xBB;
      TmpBuffer[Idx++] = 0xBB;
      memset(&TmpBuffer[Idx], 0xFF, sizeof(TmpBuffer) - Idx);
      i = 0; // make sure message counter is reset
      State++;
    }
    break;

  // Start sending Tmp buffer, send the first 128 bytes then the second, then third
  case 1:
    if ((bLogDefaultProfile != 0)) {
      I2C2Control.Address = EEPROM_EVENT_DATA_OFFSET + ((i + 238) * 128);
      Idx = i*128;
      I2C2Control.Buffer = &TmpBuffer[Idx];
      I2C2Control.Length = 128;
      I2C2Control.Mode = I2C_MODE_WRITE_EEPROM;
      State = 2;
    }
    break;
  case 2:
    if (I2C2Control.Mode == I2C_MODE_DONE) {
      i++;
      if (i >= 18) {
        bLogDefaultProfile = 0;
        I2C2Control.Mode = I2C_MODE_IDLE;
        bGetDefaultProfile = 1; // Retrieve profile anytime a new one has attempted to load. That way, if corrupt, we have a valid one
        State = 0;
      }
      else {
        State = 1;
      }
    }
    break;
  }
}

void ProcessDefaultProfile(void) {

  uint8_t j, VerifyResults;
  static int8_t SyncCount = 0;
  uint32_t *FirmwarePointer;

  // Load in Flash Configuration Profile
  if (bFlashConfigurationValid == 0 && bGetDefaultProfile != 0 && I2C2Control.Mode == I2C_MODE_IDLE) {
    for (j = 0; j< 18; j++) {
      I2C2Control.Address = EEPROM_EVENT_DATA_OFFSET + ((238 + j) * 128); // read  128 bytes from EEPROM at a time
      I2C2Control.Length = 128;
      I2C2Control.Buffer = &TmpBuffer[128 * j];
      I2C2Control.Mode = I2C_MODE_READ_EEPROM;
      do {
        ProcessI2C2();
      } while (I2C2Control.Mode != I2C_MODE_DONE);
    }
    I2C2Control.Mode = I2C_MODE_IDLE;
    bGetDefaultProfile = 0;
    bDefaultProfileLoaded = 0;
  }

  //
  // Load in default profile. Try first to get it from the EEPROM but if the backup
  // is bad (EEPROM) then use hard coded backup
  //
  if (bFlashConfigurationValid == 0 && bDefaultProfileLoaded == 0 && bGetDefaultProfile == 0) {
    // Check if EEPROM data is valid
    for (j = 0; j < 5; j++) {
      if (TmpBuffer[j] == 0xAA) {
        SyncCount++;
      }
    }
    if (TmpBuffer[5] == 0x00) {
      SyncCount++;
    }
    if (TmpBuffer[sizeof(stFlashConfiguration) + 6] == 0xBB) {
      SyncCount++;
    }
    if (TmpBuffer[sizeof(stFlashConfiguration) + 7] == 0xBB) {
      SyncCount++;
    }

    // If data retrieved is valid, then store into default data structure
    if (SyncCount >= 8) {
      memmove(&DefaultData, &TmpBuffer[6], sizeof(stFlashConfiguration));
      bGetDefaultProfile = 0;
    }
    VerifyResults = VerifyFlashData(&DefaultData);
    if (SyncCount < 8 || VerifyResults != 0) {
      // Stop trying to get a default profile and use the default hard coded settings
      memset(&DefaultData, 0x00, sizeof(DefaultData));

      DefaultData.SetpointTable.TableLength = 2;
      DefaultData.SetpointTable.TablePoint[0].Point1 = 400;
      DefaultData.SetpointTable.TablePoint[0].Point2 = 0; // 0 V output
      DefaultData.SetpointTable.TablePoint[1].Point1 = 102300;
      DefaultData.SetpointTable.TablePoint[1].Point2 = 5000; // 5V output


      DefaultData.MasterFreqTable.TableLength = 2;
      DefaultData.MasterFreqTable.TablePoint[0].Point1 = 400;
      DefaultData.MasterFreqTable.TablePoint[0].Point2 = 0; // 0 Hz output
      DefaultData.MasterFreqTable.TablePoint[1].Point1 = 102300;
      DefaultData.MasterFreqTable.TablePoint[1].Point2 = 1000000; // 10 kHz output

      DefaultData.RVOutputMode = 1; // Default is voltage
      DefaultData.RVOutputPullup = 1; // Default is pull-up enabled
      DefaultData.ErrorConditionActiveState = 0;  // Default is active low
      DefaultData.MasterFreqMode = 0;  // Default is frequency table

      DefaultData.KmaFilteringGain = 500;
      DefaultData.MasterIndexFilteringGain = 500;
      DefaultData.AccelXAdcFilteringGain = 200;
      DefaultData.AccelYAdcFilteringGain = 200;
      DefaultData.AccelZAdcFilteringGain = 200;

      DefaultData.Digio1 = CONFIG_DIG_MASTER_FREQ_OUT;
      DefaultData.Digio2 = CONFIG_DIG_DISABLED;
      DefaultData.Digio3 = CONFIG_DIG_DISABLED;
      DefaultData.Digio4 = CONFIG_DIG_DISABLED;
      DefaultData.Digio5 = CONFIG_DIG_DISABLED;
      DefaultData.Digio6 = CONFIG_DIG_DISABLED;

      DefaultData.DefaultOutput = 4;    // CAN Bus is default output
      DefaultData.SensorMode = 0;       // Default is A
      DefaultData.CanUse = 1;           // Default is CAN Enabled and used
      DefaultData.SourceAddress = 0;
      DefaultData.CanEcho = 0;
      DefaultData.CanCrcEnable = 0;
      DefaultData.CanPrescaler = 3; // 1 MBit baud rate
      DefaultData.Endianess = 1;    // Big endian is default

      DefaultData.CanTable.TableLength = 2;
      DefaultData.CanTable.TablePoint[0].Point1 = 400;
      DefaultData.CanTable.TablePoint[0].Point2 = 32000; // 0% output per J1939
      DefaultData.CanTable.TablePoint[1].Point1 = 102300;
      DefaultData.CanTable.TablePoint[1].Point2 = 57600; // 100% Output per J1939


      DefaultData.AngularOutput.TableLength = 2;
      DefaultData.AngularOutput.TablePoint[0].Point1 = 400;
      DefaultData.AngularOutput.TablePoint[0].Point2 = 0; // 0% output per J1939
      DefaultData.AngularOutput.TablePoint[1].Point1 = 102300;
      DefaultData.AngularOutput.TablePoint[1].Point2 = 180; // 100% Output per J1939


      DefaultData.CanAddressOut[0] = 0x00260000;
      DefaultData.CanAddressOut[1] = 0x08B4E020;
      DefaultData.CanAddressOut[2] = 0x08b4E200;
      DefaultData.CanAddressOut[3] = 0;
      DefaultData.CanAddressOut[4] = 0;
      DefaultData.CanAddressOut[5] = 0;

      DefaultData.CanMsgOutTime[0] = 10;
      DefaultData.CanMsgOutTime[1] = 100;
      DefaultData.CanMsgOutTime[2] = 1000;

      DefaultData.CanAddressIn[0] = 0;
      DefaultData.CanAddressIn[1] = 0;

      DefaultData.CanMsgInTime[0] = 0;
      DefaultData.CanMsgInTime[1] = 0;

      DefaultData.CanMaskIn[0] = 0;
      DefaultData.CanMaskIn[1] = 0;

      DefaultData.ReverseIndexKma = 0;
      DefaultData.RS485MessageMode = 2;
      DefaultData.RS485MessageTime = 10; // Default is send a message every 1 second
      DefaultData.AccelerometerScaling = 0;

      // ProfileName = Default in ASCII
      DefaultData.ProfileName[0] = 0x44u;
      DefaultData.ProfileName[1] = 0x65u;
      DefaultData.ProfileName[2] = 0x66u;
      DefaultData.ProfileName[3] = 0x61u;
      DefaultData.ProfileName[4] = 0x75u;
      DefaultData.ProfileName[5] = 0x6Cu;
      DefaultData.ProfileName[6] = 0x74u;

      DefaultData.Version = FLASH_VERSION;
      DefaultData.Crc16 = CalcCrc16Array((uint8_t *)&DefaultData, 2046);

    }


    LogEvent(EVENT_MESSAGE_ID_4, 0, 0);
    ReflashTuningParameters(62, (uint16_t *)&DefaultData);
    CalculatedCrc.FlashConfiguration = CalcCrc16Array((uint8_t *)FlashConfiguration, 2046);
    bFlashConfigurationValid = (FlashConfiguration->Crc16 == CalculatedCrc.FlashConfiguration);
    SysVars.SensorLetter = FlashConfiguration->SensorMode;
    InitializeDigitalIO(DIG_Mode);
    InitializeAccel();
    MX_CAN_Init();
    bUseDefaultFlash = 1;
    bDefaultProfileLoaded = 1;
    // Give a low quality score given this may trigger a sensor mismatch
    SysVars.Quality1 = 1000;

  }

  // Set default values for Factory Configuration
  if (bFactoryConfigurationValid == 0) {
    bUseDefaultFactory = 1;
    DefaultFactory.Calibration33 = 3300;                  // x.xxx volts
    DefaultFactory.PowerRail5KFactor = 10000;              // xxxxx
    DefaultFactory.KmaKFactor = 10000;                     // xxxxx
    DefaultFactory.PowerInputKFactor = 10000;              // xxxxx
    DefaultFactory.RVOutputVoltageKFactor = 10000;         // xxxxx
    DefaultFactory.RVOutputCurrentKFactor = 10000;         // xxxxx
    DefaultFactory.PwmOutputKFactor = 10000;               // xxxxx
    DefaultFactory.HardwareVersion = 0;
    DefaultFactory.AccelPresent = 1;
    DefaultFactory.RVOutputPresent = 1;
    DefaultFactory.RS485DriverPresent = 1;
    DefaultFactory.CanDriverPresent = 1;
    DefaultFactory.TempSensorPresent = 1;
    DefaultFactory.KmaPresent = 1;
    DefaultFactory.PowerInputFilteringGain = 200;
    DefaultFactory.PowerRail5FilteringGain = 200;
    DefaultFactory.TemperatureFilteringGain = 200;
    DefaultFactory.InternalReferenceFilteringGain = 200;
    DefaultFactory.RVOutputVoltageFilteringGain = 200;
    DefaultFactory.RVOutputCurrentFilteringGain = 200;
    DefaultFactory.FirmwareVersion = FIRMWARE_VERSION;
    FirmwarePointer = (uint32_t *)0x08000000u;
    DefaultFactory.FirmwareCrc16 = CalcCrc16Array((uint8_t *)FirmwarePointer, (60 * 2048));               // CRC of pages 0 through 59
    DefaultFactory.Version = FACTORY_VERSION;                     // FactoryConfiguration Version
    DefaultFactory.Crc16 = CalcCrc16Array((uint8_t *)&DefaultFactory, 2046);                       // CRC16 of first 2046 bytes

    LogEvent(EVENT_MESSAGE_ID_5, 0, 0);
    ReflashTuningParameters(63, (uint16_t *)&DefaultFactory);
    CalculatedCrc.FactoryConfiguration = CalcCrc16Array((uint8_t *)FactoryConfiguration, 2046);
    bFactoryConfigurationValid = (FactoryConfiguration->Crc16 == CalculatedCrc.FactoryConfiguration);
    // Give a low quality score given this may trigger a sensor mismatch
    SysVars.Quality1 = 1000;

  }
  else {
    bUseDefaultFactory = 0;
  }

  // Set default values for Fuzzy Configuration
  if (bFuzzyPageValid == 0) {
    bUseDefaultFuzzy = 1;
    SysVars.Quality1 = 1000;
  }
  else {
    bUseDefaultFuzzy = 0;
  }
}




























