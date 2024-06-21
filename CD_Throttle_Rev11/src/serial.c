// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: serial.c
// Author  : Rick Wright/Charlie Urban
// Revision: 3.2
//
// Description:
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
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

typedef struct {
    uint32_t              KeyCode;
    uint32_t              Version : 16;
    uint32_t              Crc : 16;
} stFirmwareUpload;

// ------------------------------------------------------------------------
//                  Local Conditional Compilation Switches
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//                          Local Variables
// ------------------------------------------------------------------------

static int32_t RS485Timer;
static stFirmwareUpload     FirmwareUpload;
static uint8_t RS_Miss; // Counter to make sure that if sensor has not sent an rs485 message in awhile it does so when this hits 15

// ------------------------------------------------------------------------
//                  Function Prototypes
// ------------------------------------------------------------------------

void ProcessSerialMessage(stSerial *SerialStruct);
void SerialSend(stSerial *SerialStruct);

// ------------------------------------------------------------------------
//
// ProcessSerialData - This routine is called for each byte received
//
// Params:    SerialStruct - Data structure containing the configuration of the serial mode used
//
// Returns:   None
//
// Requirement(s):
//
void ProcessSerialData(stSerial *SerialStruct) {

  uint8_t Data;

  ExecutionStart(EXECUTION_PROCESS_SERIAL_DATA);
  //
  // There should never be a 100mS gap between characters coming from Host.  If there is, then reset State Machine
  // back to looking for Sync Characters.
  //
  if (SerialStruct->RxGapTimeout > 100) {           // 100mS
    SerialStruct->State = 0;
  }
  SerialStruct->RxGapTimeout = 0;

  Data = SerialStruct->RxChar;

  switch (SerialStruct->State) {
  case 0:
    if (Data == 0xAA) {
      SerialStruct->SyncCount++;
    } else {
      if ((Data == 0x00) && (SerialStruct->SyncCount >= 3)) {
        SerialStruct->State += 10;                      // Must have received at least 3 of the 5 0xAA's
      }
      SerialStruct->SyncCount = 0;
    }
    break;
  case 10:
    SerialStruct->Command = Data;
    SerialStruct->Crc16 = CalcCrc16(Data, 0);
    SerialStruct->State += 10;
    break;
  case 20:
    SerialStruct->CmdLen = Data;                              // LSB
    SerialStruct->Crc16 = CalcCrc16(Data, SerialStruct->Crc16);
    SerialStruct->State += 10;
    break;
  case 30:
    SerialStruct->CmdLen += (uint16_t)Data << 8;              // MSB
    SerialStruct->Length = SerialStruct->CmdLen;
    SerialStruct->Crc16 = CalcCrc16(Data, SerialStruct->Crc16);
    SerialStruct->State += 10;
    if (SerialStruct->CmdLen == 0) {
      SerialStruct->State += 10;
    }
    if ((SerialStruct->Command == 0x11) || (SerialStruct->Command == 0x12) || (SerialStruct->Command == 0x13)) {
      if (SerialStruct->CmdLen > sizeof(SerialBuffer)) {
        SerialStruct->State = 0;          // Make sure Length is valid
      }
    } else {
      if (SerialStruct->CmdLen > sizeof(SerialStruct->Data)) {
        SerialStruct->State = 0;          // Make sure Length is valid
      }
    }
    SerialStruct->Offset = 0;
    break;
  case 40:
    if ((SerialStruct->Command == 0x11) || (SerialStruct->Command == 0x12) || (SerialStruct->Command == 0x13)) {
            SerialBuffer.ByByte[SerialStruct->Offset++] = Data;
    }
    else if (SerialStruct->Command == 0x72) {
      SerialBuffer.ByByte[SerialStruct->Offset++] = Data;
    }
    else {
      SerialStruct->Data[SerialStruct->Offset++] = Data;
    }

    SerialStruct->Crc16 = CalcCrc16(Data, SerialStruct->Crc16);
    SerialStruct->CmdLen--;
    if (SerialStruct->CmdLen == 0) {
      SerialStruct->State += 10;
    }
    break;
  case 50:
    SerialStruct->CmdCrc = Data;                          // LSB
    SerialStruct->State += 10;
    break;
  case 60:
    SerialStruct->CmdCrc += (uint16_t)Data << 8;          // MSB
    if (SerialStruct->CmdCrc == SerialStruct->Crc16) {
      ProcessSerialMessage(SerialStruct);             // Process Command if CRC's match
    }
    SerialStruct->State = 0;
    break;
  default:
    Error_Handler(FILE_SERIAL, __LINE__);
    break;
  }

  ExecutionEnd(EXECUTION_PROCESS_SERIAL_DATA);
}

// ------------------------------------------------------------------------
//
// ProcessRxSerial - This routine is called every 1mS from Main Loop to handle the reception
//                   of serial data (RS485 or USART)
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void ProcessRxSerial(stSerial *SerialStruct) {

    ExecutionStart(EXECUTION_PROCESS_RX_SERIAL);

    if (SerialStruct->RxMessageTimeout < 4000) {        // Prevent Timer from continuing to count and then roll-over back to zero
        SerialStruct->RxMessageTimeout++;
    }
    SerialStruct->RxGapTimeout++;
    //
    // If DIG_Mode is still in Powerup Mode, and USART2 hasn't received a command for 3 seconds, then switch to Normal Mode
    //
    if ((SerialStruct == &SerialUsart2) && (DIG_Mode == DIG_MODE_POWERUP) && (SerialStruct->RxMessageTimeout > 3000)) {
        InitializeDigitalIO(DIG_MODE_NORMAL);
    }
    //
    //  RS485 Message output, but only if no RS485 commands have been received for 2 seconds and is Message mode 1.
    //
    if ((SerialStruct == &SerialUsart1) && (SerialStruct->RxMessageTimeout > 2000) && (FlashConfiguration->RS485MessageMode == 1)) {
        if (RS485Timer != 0) {
            RS485Timer--;
        }
        if ((RS485Timer == 0) && (bFlashConfigurationValid != 0)) {
            RS485Timer = FlashConfiguration->RS485MessageTime * 100;
            if (FlashConfiguration->RS485MessageMode == 1) {
                memmove(&SerialStruct->Data[9], &SysVars.MasterFrequency, 4);
                memmove(&SerialStruct->Data[13], &SysVars.RVOutputVoltage, 4);
                SerialStruct->Length = 8;
                SerialStruct->Command = 0x75;
                SerialSend(SerialStruct);
            }
        }
    }
    //
    // For Throttle, we are using RS485 in Half Duplex
    //
    if (SerialStruct == &SerialUsart1) {
      if (FlashConfiguration->RS485MessageMode == 2) {
        if ((RS485.RSControl == 1) || ((RS_Miss >= 151) && (!(SysVars.SensorLetter))) || ((RS_Miss >= 202) && (SysVars.SensorLetter))){
          // Send data
          memmove(&SerialStruct->Data[9], &SysVars, sizeof(SysVars));
          SerialStruct->Length = sizeof(SysVars);
          SerialStruct->Command = 0x72;
          SerialSend(SerialStruct);
          RS485.RSControl = 0;
          RS_Miss = 0;
        }
        else {
          RS_Miss++;
        }
      }
    }
    //
    // Process any Serial Input that has been received
    //
    if (SerialStruct == &SerialUsart2) {
        if (bSendingMessage != 0) {
            SerialStruct->RightBufferOut = SerialStruct->RingBufferIn;
        }
    }
    while (SerialStruct->RightBufferOut != SerialStruct->RingBufferIn) {
        SerialStruct->RxChar = SerialStruct->DmaRingBuffer[SerialStruct->RightBufferOut++];
        ProcessSerialData(SerialStruct);
    }

    ExecutionEnd(EXECUTION_PROCESS_RX_SERIAL);
}

// ------------------------------------------------------------------------
//
// MicroDelay - This routine is called to create a short delay
//
// Params:    uS - number of microseconds to delay by
//
// Returns:   None
//
// Requirement(s):
//
void MicroDelay(uint32_t uS) {

  uint32_t OldCounts, MicroSecondDelay, DelayCounts, NewCounts;

  OldCounts = TIM2->CNT;                          // This 32-bit Timer is running at 16MHz
  DelayCounts = 0;
  MicroSecondDelay = uS * 16;                   // Calculate number of timer counts to delay
  while (DelayCounts < MicroSecondDelay) {
    NewCounts = TIM2->CNT;
    DelayCounts += NewCounts - OldCounts;
    OldCounts = NewCounts;
  }
}

// ------------------------------------------------------------------------
//
// SerialSend - This routine is called to send a serial message to Host.
//
// Params:    SerialStruct - Data structure containing the configuration of the serial mode used
//
// Returns:   None
//
// Requirement(s):
//
void SerialSend(stSerial *SerialStruct) {

  uint16_t Crc;

  SerialStruct->Data[0] = 0xAA;                    // Sync
  SerialStruct->Data[1] = 0xAA;
  SerialStruct->Data[2] = 0xAA;
  SerialStruct->Data[3] = 0xAA;
  SerialStruct->Data[4] = 0xAA;
  SerialStruct->Data[5] = 0x00;
  SerialStruct->Data[6] = SerialStruct->Command;                // Command
  SerialStruct->Data[7] = (uint8_t)(SerialStruct->Length & 0xFF);          // Length
  SerialStruct->Data[8] = (uint8_t)(SerialStruct->Length >> 8);
  //
  //  Calculate CRC
  //
  Crc = CalcCrc16Array(&SerialStruct->Data[6], SerialStruct->Length + 3);
  SerialStruct->Data[SerialStruct->Length + 9] = (uint8_t)(Crc & 0xFF);
  SerialStruct->Data[SerialStruct->Length + 10] = (uint8_t)(Crc >> 8);
  //
  // Send Message via selected SerialMode Mode.
  //
  if (SerialStruct == &SerialUsart1) {
    STM32_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET);                              // Turn on RS422/485 Driver
    MicroDelay(500);
    STM32_UART_Transmit_DMA(&huart1, (uint8_t *)&SerialStruct->Data[0], SerialStruct->Length + 11);
  }
  if (SerialStruct == &SerialUsart2) {
    if (DIG_HandleStruct[0].DIG_InitStruct.Mode == DIG_SERIAL_INOUT) {
      //
      //  Switch Bi-Directional Buffer to an Output
      //
      STM32_GPIO_WritePin(DIG_HandleStruct[0].Dir_Port, DIG_HandleStruct[0].Dir_Pin, GPIO_PIN_SET);       // Set DIG_IO1 buffer as output
      MicroDelay(500);
    }
    STM32_UART_Transmit_DMA(&huart2, (uint8_t *)&SerialStruct->Data[0], SerialStruct->Length + 11);
    //
    // As data is transmitted, it will be received also.  This flag prevents the received data from being placed into the Receiver Buffer
    //
    bSendingMessage = TRUE;
  }
}

// ------------------------------------------------------------------------
//
// VerifyFlashData - Verify all Parameter Data Items are within range.  This routine is called when the
//                   Write FlashConfiguration Data serial command is received
//
//
// Params:    NewData - Buffer containing the new FlashConfiguration data written from the host to the MCU
//
// Returns:   Value 0-255 where 0 indicates no errors with new FlashConfiguration data and > 0 is an error
//
// Requirement(s):
//
uint8_t VerifyFlashData(stFlashConfiguration *NewData) {

  int32_t TmpIndex, i, TestInputCount;
  uint32_t Dig = 0;

  //
  // Only Modes 0 through 2 are supported at this time
  //
  if ((NewData->RS485MessageMode < 0) || (NewData->RS485MessageMode > 2)) {
    return 5;
  }
  //
  // If Mode isn't Disabled, then Time must be greater than 0 and less than 1001, or 0.1 to 100.0 seconds
  //
  if (NewData->RS485MessageMode > 0) {
    if ((NewData->RS485MessageTime < 1) || (NewData->RS485MessageTime > 1000)) {          // 0.1 to 100.0 seconds
      return 10;
    }
  }
  //
  // Verify all FilteringGains are within the range of 1 to 16384
  //
  if ((NewData->KmaFilteringGain < 1) || (NewData->KmaFilteringGain > 16384)) {
    return 15;
  }
  if ((NewData->MasterIndexFilteringGain < 1) || (NewData->MasterIndexFilteringGain > 16384)) {
    return 20;
  }
  if ((NewData->AccelXAdcFilteringGain < 1) || (NewData->AccelXAdcFilteringGain > 16384)) {
    return 25;
  }
  if ((NewData->AccelYAdcFilteringGain < 1) || (NewData->AccelYAdcFilteringGain > 16384)) {
    return 30;
  }
  if ((NewData->AccelZAdcFilteringGain < 1) || (NewData->AccelZAdcFilteringGain > 16384)) {
    return 35;
  }
  //
  // Verify Mode is within the range of 0 to 2
  // TESTING!! Update these values
  //
  if ((NewData->RVOutputMode < 0) || (NewData->RVOutputMode > 2)) {
    return 50;
  }
  //
  // Verify DIGIO1 0 to 7, or 10 to 12
  //
  if (((NewData->Digio1 > 7) && (NewData->Digio1 < 10)) || (NewData->Digio1 > 12)) {
    return 55;
  }
  //
  // Verify DIGIO2 0 to 6, or 9 to 12
  //
  if (((NewData->Digio2 > 6) && (NewData->Digio2 < 9)) || (NewData->Digio2 > 12)) {
    return 60;
  }
  //
  // Verify DIGIO3 0 to 6, or 10 to 12
  //
  if (((NewData->Digio3 > 6) && (NewData->Digio3 < 10)) || (NewData->Digio3 > 12)) {
    return 65;
  }
  //
  // Verify DIGIO4 0 to 6, or 10 to 12
  //
  if (((NewData->Digio4 > 6) && (NewData->Digio4 < 10)) || (NewData->Digio4 > 12)) {
    return 70;
  }
  //
  // Verify DIGIO5 is 0,2,4,6,8,10,11,12
  //
  if ((NewData->Digio5 == 1) || (NewData->Digio5 == 3) || (NewData->Digio5 == 5) || (NewData->Digio5 == 7) || (NewData->Digio5 == 9) || (NewData->Digio5 > 12)) {
    return 75;
  }
  //
  // Verify DIGIO6 0 to 6, or 10 to 12
  //
  if (((NewData->Digio6 > 6) && (NewData->Digio6 < 10)) || (NewData->Digio6 > 12)) {
    return 80;
  }

  //
  // DIGIO2 and DIGIO6 can not both be configured as SLAVE_FREQ_INPUTS
  //
  if ((NewData->Digio2 == 2) && (NewData->Digio6 == 2)) {
    return 100;
  }
  //
  // If RVOutput is configured in Resistance Mode, then RV Pullup must be disabled
  //
  if ((NewData->RVOutputMode == 2) && (NewData->RVOutputPullup != 0)) {      // if RV Output Mode is Resistance, then Pullup can not be enabled
    return 110;
  }
  //
  // If MasterFreqMode is configured for FreqTableMode, then verify MasterFreqTable is correct.
  //
  if (NewData->MasterFreqMode == 0) {
    //
    // There must be at least 2 entries in table
    //
    if (NewData->MasterFreqTable.TableLength < 2) {
      return 115;
    }
    //
    // Index of First Entry must be 400 (4.00)
    //
    if (NewData->MasterFreqTable.TablePoint[0].Point1 != 400) {
      return 120;
    }
    //
    // Index of Last Entry must be 102300 (1023.00)
    //
    if (NewData->MasterFreqTable.TablePoint[NewData->MasterFreqTable.TableLength - 1].Point1 != 102300) {
      return 125;
    }
    //
    // Indexes must be increasing
    //
    TmpIndex = NewData->MasterFreqTable.TablePoint[0].Point1;
    for (i = 1; i < NewData->MasterFreqTable.TableLength; i++) {
      if (NewData->MasterFreqTable.TablePoint[i].Point1 <= TmpIndex) {
        return 130;
      }
      TmpIndex = NewData->MasterFreqTable.TablePoint[i].Point1;
    }
    //
    // Frequenies must be >= 10Hz and <= 20KHz
    //
    for (i = 0; i < NewData->MasterFreqTable.TableLength; i++) {
      if ((NewData->MasterFreqTable.TablePoint[i].Point2 < 1000) || (NewData->MasterFreqTable.TablePoint[i].Point2 > 2000000)) {
        return 135;
      }
    }
  }
  //
  // IF RVOutput is enabled (voltage or resistance), then verify SetpointTable
  //
  if (NewData->RVOutputMode != 0) {
    //
    // There must be >= 2 entries in table and <= 32
    //
    if ((NewData->SetpointTable.TableLength < 2) || (NewData->SetpointTable.TableLength > 32)) {     // Must have at least two entries
      return 140;
    }
    //
    // Index of First Entry must be 400 (4.00)
    //
    if (NewData->SetpointTable.TablePoint[0].Point1 != 400) {     // Index of first Entry must be 4.00
      return 145;
    }
    //
    // Index of Last Entry must be 102300 (1023.00)
    //
    if (NewData->SetpointTable.TablePoint[NewData->SetpointTable.TableLength - 1].Point1 != 102300) {  // Index of last entry must be 1023.00
      return 150;
    }
    //
    // Indexes must be increasing
    //
    TmpIndex = NewData->SetpointTable.TablePoint[0].Point1;
    for (i = 1; i < NewData->SetpointTable.TableLength; i++) {
      if (NewData->SetpointTable.TablePoint[i].Point1 <= TmpIndex) {
        return 155;
      }
      TmpIndex = NewData->SetpointTable.TablePoint[i].Point1;
    }
    //
    // If RV Output Mode is Voltage, then Setpoint must be from 0 to 9999 (9.999) volts
    //
    if (NewData->RVOutputMode == 1) {
      for (i = 0; i < NewData->SetpointTable.TableLength; i++) {
        if ((NewData->SetpointTable.TablePoint[i].Point2 < 0) || (NewData->SetpointTable.TablePoint[i].Point2 > 9999)) {
          return 160;
        }
      }
    }
    //
    // If RV Output Mode is Resistance, then Setpoint must be from 1000 to 999999 (1.000 to 999.999 ohms)
    //
    if (NewData->RVOutputMode == 2) {              // RV Output mode is Resistance
      for (i = 0; i < NewData->SetpointTable.TableLength; i++) {
        if ((NewData->SetpointTable.TablePoint[i].Point2 < 1000) || (NewData->SetpointTable.TablePoint[i].Point2 > 999999)) {        // 0.5 to 999.999 Ohms
          return 165;
        }
      }
    }
  }
  //
  // Only one DIGIO can be configured as a TEST_IN
  // Only one DIGIO can be configured as a KINGAIR_CLOCK
  // Only one DIGIO can be configured as a KINGAIR_DATA
  //
  TestInputCount = 0;
  for (i = 0; i < 6; i++) {
    switch (i) {
    case 0:
      Dig = NewData->Digio1;
      break;
    case 1:
      Dig = NewData->Digio2;
      break;
    case 2:
      Dig = NewData->Digio3;
      break;
    case 3:
      Dig = NewData->Digio4;
      break;
    case 4:
      Dig = NewData->Digio5;
      break;
    case 5:
      Dig = NewData->Digio6;
      break;
    default:
      Error_Handler(FILE_SERIAL, __LINE__);
      break;
    }
    if (Dig == CONFIG_DIG_TEST_IN) {
      TestInputCount++;
    }
  }
  if (TestInputCount > 1) {               // Only one DIG_IO can be configured as a TEST_INPUT
    return 170;
  }
  //
  // Scaling must be 0, 1 or 2
  //
  if (NewData->AccelerometerScaling > 2) {
    return 195;
  }
  if (NewData->DefaultOutput > 4) {
    return 200;
  }
  if (NewData->SensorMode > 1) {
    return 201;
  }
  if (NewData->CanUse != 0 && NewData->CanUse != 1) {
    return 202;
  }
  if (NewData->CanEcho != 0 && NewData->CanEcho != 1) {
    return 203;
  }
  if (NewData->CanCrcEnable != 0 && NewData->CanCrcEnable != 1) {
    return 204;
  }
  if (NewData->CanPrescaler != 3 && NewData->CanPrescaler != 6 && NewData->CanPrescaler != 12 &&
      NewData->CanPrescaler != 24 && NewData->CanPrescaler != 30 && NewData->CanPrescaler != 36 &&
      NewData->CanPrescaler != 60 && NewData->CanPrescaler != 150 && NewData->CanPrescaler != 300) {
    return 205;
  }
  //
  // There must be >= 2 entries in table and <= 32
  //
  if ((NewData->CanTable.TableLength < 2) || (NewData->CanTable.TableLength > 32)) {     // Must have at least two entries
    return 206;
  }
  //
  // Index of First Entry must be 400 (4.00)
  //
  if (NewData->CanTable.TablePoint[0].Point1 != 400) {     // Index of first Entry must be 4.00
    return 207;
  }
  //
  // Index of Last Entry must be 102300 (1023.00)
  //
  if (NewData->CanTable.TablePoint[NewData->CanTable.TableLength - 1].Point1 != 102300) {  // Index of last entry must be 1023.00
    return 208;
  }
  //
  // Indexes must be increasing
  //
  TmpIndex = NewData->CanTable.TablePoint[0].Point1;
  for (i = 1; i < NewData->CanTable.TableLength; i++) {
    if (NewData->CanTable.TablePoint[i].Point1 <= TmpIndex) {
      return 209;
    }
    TmpIndex = NewData->CanTable.TablePoint[i].Point1;
  }

  for (i = 0; i < 3; i++) {
    if (NewData->CanMsgOutTime[i] != 0) {
      if (NewData->CanMsgOutTime[i] < 10) {
        return 211;
      }
    }
  }
  for (i = 0; i < 2; i++) {
    if (NewData->CanAddressIn[i] != 0) {
      if (NewData->CanMsgInTime[i] < 10 && NewData->CanMsgInTime[i] > 0) {
        return 212;
      }
      if (NewData->CanMaskIn[i] == 0) {
        return 213;
      }
    }
  }
  if (NewData->Endianess != 0 && NewData->Endianess != 1) {
    return 214;
  }

  //
  // There must be >= 2 entries in table and <= 32
  //
  if ((NewData->AngularOutput.TableLength < 2) || (NewData->AngularOutput.TableLength > 32)) {     // Must have at least two entries
    return 215;
  }
  //
  // Index of First Entry must be 400 (4.00)
  //
  if (NewData->AngularOutput.TablePoint[0].Point1 != 400) {     // Index of first Entry must be 4.00
    return 216;
  }
  //
  // Index of Last Entry must be 102300 (1023.00)
  //
  if (NewData->AngularOutput.TablePoint[NewData->AngularOutput.TableLength - 1].Point1 != 102300) {  // Index of last entry must be 1023.00
    return 217;
  }
  //
  // Indexes must be increasing
  //
  TmpIndex = NewData->AngularOutput.TablePoint[0].Point1;
  for (i = 1; i < NewData->AngularOutput.TableLength; i++) {
    if (NewData->AngularOutput.TablePoint[i].Point1 <= TmpIndex) {
      return 218;
    }
    TmpIndex = NewData->AngularOutput.TablePoint[i].Point1;
  }

  NewData->Version = FLASH_VERSION;
  NewData->Crc16 = CalcCrc16Array((uint8_t *)NewData, 2046);
  return 0;           // No Errors Detected
}

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// VerifyFactoryData - Verify all Factory Parameter Data Items are within range.
//                     This routine is called when Write FactoryConfiguration Data serial command is received.
//
// Params:    NewData - Buffer containing the new FactoryConfiguration data written from the host to the MCU
//
// Returns:   Value 0-255 where 0 indicates no errors with new FactoryConfiguration data and > 0 is an error
//
uint8_t VerifyFactoryData(stFactoryConfiguration *NewData) {

    if ((NewData->Calibration33 < 3250) || (NewData->Calibration33 > 3350)) {
        return 1;
    }
    if ((NewData->PowerInputFilteringGain < 1) || (NewData->PowerInputFilteringGain > 16384)) {
        return 2;
    }
    if ((NewData->PowerRail5FilteringGain < 1) || (NewData->PowerRail5FilteringGain > 16384)) {
        return 3;
    }
    if ((NewData->TemperatureFilteringGain < 1) || (NewData->TemperatureFilteringGain > 16384)) {
        return 4;
    }
    if ((NewData->InternalReferenceFilteringGain < 1) || (NewData->InternalReferenceFilteringGain > 16384)) {
        return 5;
    }
    if ((NewData->RVOutputVoltageFilteringGain < 1) || (NewData->RVOutputVoltageFilteringGain > 16384)) {
        return 6;
    }
    if ((NewData->RVOutputCurrentFilteringGain < 1) || (NewData->RVOutputCurrentFilteringGain > 16384)) {
        return 7;
    }
    if ((NewData->KmaKFactor < 1) || (NewData->KmaKFactor > 100000)) {
        return 8;
    }
    if ((NewData->PowerInputKFactor < 1) || (NewData->PowerInputKFactor > 100000)) {
        return 9;
    }
    if ((NewData->PowerRail5KFactor < 1) || (NewData->PowerRail5KFactor > 100000)) {
        return 10;
    }
    if ((NewData->PwmOutputKFactor < 1) || (NewData->PwmOutputKFactor > 100000)) {
        return 11;
    }
    if ((NewData->RVOutputCurrentKFactor < 1) || (NewData->RVOutputCurrentKFactor > 100000)) {
        return 12;
    }
    if ((NewData->RVOutputVoltageKFactor < 1) || (NewData->RVOutputVoltageKFactor > 100000)) {
        return 13;
    }
    return 0;           // No Errors Detected
}

// ------------------------------------------------------------------------
//
// VerifyFuzzyData - This routine is called when to validate a serial message containing
//                   new fuzzy logic data
//
// Params:    Pointer to page 61 where the fuzzy logic data is stored
//
// Returns:   None
//
// Requirement(s):
//

int32_t VerifyFuzzyData(stFuzzyPage *NewData) {
  // TESTING - In development
  NewData->Fuzzy2.Version = FUZZY_VERSION;
  NewData->Fuzzy2.Crc16 = CalcCrc16Array((uint8_t *) NewData, 2046);
  return 0;
}

// ------------------------------------------------------------------------
//
// This routine is called when a valid serial message has been received.
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void ProcessSerialMessage(stSerial *SerialStruct) {

  uint16_t BlockNo, FlashIndex;
  uint8_t *BytePointer, VerifyResults;

  ExecutionStart(EXECUTION_PROCESS_SERIAL_MESSAGE); //
  //
  // If a Message is received, then reset  RS485Timer to prevent messages from being sent.
  //
  if (SerialStruct == &SerialUsart1) {
    RS485Timer = FlashConfiguration->RS485MessageTime * 100;
  }
  //
  // If a Message is received, then reset RxMEssageTimeout.  This will prevent DIGIO1 from switching to NormalMode
  //
  if (SerialStruct == &SerialUsart2) {
      SerialStruct->RxMessageTimeout = 0;
  }
  //
  //  Write FlashConfiguration Data
  //
  if (SerialStruct->Command == 0x11) {
    if (SerialStruct->Length == 2048) {
      VerifyResults = VerifyFlashData((stFlashConfiguration *)&SerialBuffer.ByWord[0]);
      if (VerifyResults == 0) {
        LogEvent(EVENT_MESSAGE_ID_4, 0, 0);
        ReflashTuningParameters(62, &SerialBuffer.ByWord[0]);
        CalculatedCrc.FlashConfiguration = CalcCrc16Array((uint8_t *)FlashConfiguration, 2046);
        SysVars.SensorLetter = FlashConfiguration->SensorMode;
        bFlashConfigurationValid = (FlashConfiguration->Crc16 == CalculatedCrc.FlashConfiguration);
        bLogDefaultProfile = bFlashConfigurationValid; //Only log default profile if flash is valid
        InitializeDigitalIO(DIG_Mode);
        InitializeAccel();
        MX_CAN_Init(); // Initalize CAN in case addresses or prescaler changes



      }
      SerialStruct->Command = 0x11;
      SerialStruct->Length = 1;
      SerialStruct->Data[9] = VerifyResults;
      SerialSend(SerialStruct);
    }
  }
  //
  // Write FactoryConfiguration Data
  //
  if (SerialStruct->Command == 0x12) {
    if (SerialStruct->Length == 2048) {
      VerifyResults = VerifyFactoryData((stFactoryConfiguration *)&SerialBuffer.ByWord[0]);
      if (VerifyResults == 0) {
        LogEvent(EVENT_MESSAGE_ID_5, 0, 0);
        ReflashTuningParameters(63, &SerialBuffer.ByWord[0]);
        CalculatedCrc.FactoryConfiguration = CalcCrc16Array((uint8_t *)FactoryConfiguration, 2046);
        bFactoryConfigurationValid = (FactoryConfiguration->Crc16 == CalculatedCrc.FactoryConfiguration);
      }
      SerialStruct->Command = 0x12;
      SerialStruct->Length = 1;
      SerialStruct->Data[9] = VerifyResults;
      SerialSend(SerialStruct);
    }
  }
  //
  // Write Fuzzy Logic Compensation Data
  //
  if (SerialStruct->Command == 0x13) {
    if (SerialStruct->Length == 2048) {
      VerifyResults = VerifyFuzzyData((stFuzzyPage *)&SerialBuffer.ByWord[0]);
      if (VerifyResults == 0) {
        LogEvent(EVENT_MESSAGE_ID_6, 0, 0);
        ReflashTuningParameters(61, &SerialBuffer.ByWord[0]);
        bFuzzyPageValid = (FuzzyPage->Fuzzy2.Crc16 == CalcCrc16Array((uint8_t *)FuzzyPage, 2046));

      }
      SerialStruct->Command = 0x13;
      SerialStruct->Length = 1;
      SerialStruct->Data[9] = VerifyResults;
      SerialSend(SerialStruct);
    }
  }
  //
  // Clear Event Log
  //
  if (SerialStruct->Command == 0x14) {
    if (SerialStruct->Length == 0) {
      bClearEventLog = 1;
      SerialStruct->Length = 0;
      SerialSend(SerialStruct);
    }
  }
  //
  // Read FlashConfiguration Data
  //
  if (SerialStruct->Command == 0x41) {
    if (SerialStruct->Length == 1) {
      FlashIndex = SerialStruct->Data[0];
      SerialStruct->Data[9] = FlashIndex;
      BytePointer = (uint8_t *)FlashConfiguration;
      memmove(&SerialStruct->Data[10], &BytePointer[FlashIndex * 512], 512);
      SerialStruct->Length = 513;
      SerialSend(SerialStruct);
    }
  }
  //
  // Read FactoryConfiguration Data
  //
  if (SerialStruct->Command == 0x42) {
    if (SerialStruct->Length == 1) {
      FlashIndex = SerialStruct->Data[0];
      SerialStruct->Data[9] = FlashIndex;
      BytePointer = (uint8_t *)FactoryConfiguration;
      memmove(&SerialStruct->Data[10], &BytePointer[FlashIndex * 512], 512);
      SerialStruct->Length = 513;
      SerialSend(SerialStruct);
    }
  }
  //
  // Read Fuzzy Logic Data
  //
  if (SerialStruct->Command == 0x43) {
    if (SerialStruct->Length == 1) {
      FlashIndex = SerialStruct->Data[0];
      SerialStruct->Data[9] = FlashIndex;
      BytePointer = (uint8_t *)FuzzyPage;
      memmove(&SerialStruct->Data[10], &BytePointer[FlashIndex * 512], 512);
      SerialStruct->Length = 513;
      SerialSend(SerialStruct);
    }
  }
    //
    // Read Event Log Partition  32K - 110 blocks of 256 bytes each
    //
  if (SerialStruct->Command == 0x44) {
    if ((SerialStruct->Length == 2) && (!bClearEventLog) && (I2C2Control.Mode == I2C_MODE_IDLE)) {
      BlockNo = SerialStruct->Data[0] + ((uint16_t)SerialStruct->Data[1] << 8);
      SerialStruct->Data[9] = SerialStruct->Data[0];                               // Block Count, 0 to 127
      SerialStruct->Data[10] = SerialStruct->Data[1];                               // Block Count, 0 to 127
      I2C2Control.Address = EEPROM_EVENT_DATA_OFFSET + (256 * BlockNo);           // Read 128 bytes from EEPROM
      I2C2Control.Length = 128;
      I2C2Control.Buffer = &SerialStruct->Data[11];
      I2C2Control.Mode = I2C_MODE_READ_EEPROM;
      do {
        ProcessI2C2();
      } while (I2C2Control.Mode != I2C_MODE_DONE);
      I2C2Control.Address = EEPROM_EVENT_DATA_OFFSET + (256 * BlockNo) + 128;     // Read another 128 bytes from EEPROM
      I2C2Control.Length = 128;
      I2C2Control.Buffer = &SerialStruct->Data[11 + 128];
      I2C2Control.Mode = I2C_MODE_READ_EEPROM;
      do {
          ProcessI2C2();
      } while (I2C2Control.Mode != I2C_MODE_DONE);
      I2C2Control.Mode = I2C_MODE_IDLE;
      SerialStruct->Length = 256 + 2;
      SerialSend(SerialStruct);
    }
    else {
      SerialStruct->Length = 0;
      SerialSend(SerialStruct);
    }
  }
  //
  // Read Signal Status Information
  //
  if (SerialStruct->Command == 0x48) {
    if (SerialStruct->Length == 0) {
      memmove(&SerialStruct->Data[9], &SignalStatus, sizeof(SignalStatus));
      SerialStruct->Length = sizeof(SignalStatus);
      SerialSend(SerialStruct);
    }
  }
  //
  // Read System Variables and Throttle Variables
  //
  if (SerialStruct->Command == 0x70) {
    if (SerialStruct->Length == sizeof(HostData)) {
      memmove(&HostData, &SerialStruct->Data[0], sizeof(HostData));
      if (bFlashConfigurationValid != 0) {
        SysVars.RVOutputMode = FlashConfiguration->RVOutputMode;
      }
      SysVars.SystemTicks = STM32_GetTick();
      SysVars.Timer2Counts = TIM2->CNT;
      memmove(&SerialStruct->Data[9], &SysVars, sizeof(SysVars));
      SerialStruct->Length = sizeof(SysVars);
      SerialSend(SerialStruct);
    }
  }
  //
  // Read Execution Timing Report
  //
  if (SerialStruct->Command == 0x71) {
    if (SerialStruct->Length == 0) {
      memmove(&SerialStruct->Data[9], &ExecutionTimeReport, sizeof(stExecutionTimeReport) * EXECUTION_PROCESS_COUNT);
      SerialStruct->Length = sizeof(stExecutionTimeReport) * EXECUTION_PROCESS_COUNT;
      SerialSend(SerialStruct);
    }
  }
  //
  // Store Throttle Data
  //
  if (SerialStruct->Command == 0x72) {
    if (SerialStruct->Length == sizeof(SysVars)) {
      if (SerialStruct == &SerialUsart1) {
        memmove(&ForeignSysVars, &SerialBuffer.ByByte[0], sizeof(ForeignSysVars));
        // Used to ensure one sensor is always A and the other is B
        if (SysVars.SensorLetter == ForeignSysVars.SensorLetter) {
          SysVars.SensorLetter = !ForeignSysVars.SensorLetter;
        }

        SysVars.RS485Counter++;
        if (SysVars.RS485Counter >= 15) {
          SysVars.RS485Counter = 0;
        }
        RS485.RSControl = 1;
      }
    }
  }
  //
  // Send Foreign Throttle Data
  //
  if (SerialStruct->Command == 0x73) {
    if (SerialStruct->Length == 0) {
      memmove(&SerialStruct->Data[9], &ForeignSysVars, sizeof(ForeignSysVars));
      SerialStruct->Length = sizeof(SysVars);
      SerialSend(SerialStruct);
    }
  }


#if INSTRUMENT_CODE==1
  //
  // Read CMark Array
  //        NOTE: Serial Data Buffer is 530 bytes, allowing upto 4240 Bit Marks
  //
  if (SerialStruct->Command == 0x72) {
    memmove(&SerialStruct->Data[9], &CmarkArray, sizeof(CmarkArray));
    SerialStruct->Length = sizeof(CmarkArray);
    SerialSend(SerialStruct);
  }
#endif
  //
  // Return Firmware CRC Information
  //
  if (SerialStruct->Command == 0x99) {
    if (SerialStruct->Length == 0) {
      //
      // Calculate Firmware Checksum - Pages 0 to 59
      //
      uint16_t Crc = CalcCrc16Array((uint8_t *)0x08000000, (2048 * 60));  //
      SerialStruct->Data[9] = (uint8_t)(Crc & 0xFF);
      SerialStruct->Data[10] = (uint8_t)(Crc >> 8);
      SerialStruct->Length = 2;
      SerialSend(SerialStruct);
    }
  }
  //
  // Flash new Firmware
  //
  if (SerialStruct->Command == 0xCC) {
    if (SerialStruct->Length == sizeof(FirmwareUpload)) {
      memmove(&FirmwareUpload, &SerialStruct->Data[0], sizeof(FirmwareUpload));
      if (FirmwareUpload.KeyCode == 0x78563412) {
        reflash(SerialStruct, FirmwareUpload.Crc, FirmwareUpload.Version);
      }
    }
  }

  ExecutionEnd(EXECUTION_PROCESS_SERIAL_MESSAGE);
}
