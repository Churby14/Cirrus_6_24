/**
 ******************************************************************************
 * File Name:       serial.c
 *
 * Description:     Serial Routines
 *
 * Release History:
 *
 *  Ver 2017-02-26  RLW     Initial Release
 *
 *
 ******************************************************************************
 **/

#include "global.h"

void ProcessSerialMessage(stSerial *SerialStruct);


// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
//  This routine is called for each byte received
//
// HLR-xxxx
// LLR-xxxx
//
void ProcessSerialData(stSerial *SerialStruct) {

  uint8_t   Data;

  //
  // There should never be a 50mS gap between characters coming from Host.  If there is, then reset State Machine
  // back to looking for Sync Characters.
  //
  if (SerialStruct->RxTimeout > 50) {           // 50mS
      if (SerialStruct->InstanceNumber == 1) SendMessageToCD = 0;
      if (SerialStruct->InstanceNumber == 4) SendMessageToPC = 0;
      SerialStruct->State = 0;
  }
  SerialStruct->RxTimeout = 0;

  Data = SerialStruct->RxChar;
  if ((SendMessageToCD == 2) && (SerialStruct->InstanceNumber == 1)) {
      CDTxBuffer[CDTxBufferIn++] = Data;
  }
  if ((SendMessageToPC == 2) && (SerialStruct->InstanceNumber == 4)) {
      CDRxBuffer[CDRxBufferIn++] = Data;
  }

  switch ( SerialStruct->State ) {
  case 0:
      if ((Data == 0xBB) && (SerialStruct->InstanceNumber == 1)) {
        CDTxBuffer[CDTxBufferIn++] = 0xAA;
        SendMessageToCD = 1;
        SerialStruct->SyncCount++;
    } else if ( Data == 0xAA ) {
        if (SerialStruct->InstanceNumber == 1) SendMessageToCD = 0;
        if ((SendMessageToPC == 1) && (SerialStruct->InstanceNumber == 4)) {
            CDRxBuffer[CDRxBufferIn++] = 0xBB;
        }
        SerialStruct->SyncCount++;
    } else {
       if (Data == 0x00) {
          if (SerialStruct->SyncCount >= 3) {
              if ((SendMessageToCD == 1) && (SerialStruct->InstanceNumber == 1)) {
                  CDTxBuffer[CDTxBufferIn++] = 0x00;
                  SendMessageToCD = 2;
              }
              if ((SendMessageToPC == 1) && (SerialStruct->InstanceNumber == 4)) {
                  CDRxBuffer[CDRxBufferIn++] = 0x00;
                  SendMessageToPC = 2;
              }
              SerialStruct->State += 10;                      // Must have received at least 3 of the 5 0xAA's

          }
      } else {
          if (SerialStruct->InstanceNumber == 1) SendMessageToCD = 0;
          if (SerialStruct->InstanceNumber == 4) SendMessageToPC = 0;
      }
      SerialStruct->SyncCount = 0;
    }
    break;
  case 10:
    SerialStruct->Command = Data;
    SerialStruct->Crc16 = CalcCrc(Data, 0x0);
    SerialStruct->State += 10;
    break;
  case 20:
    SerialStruct->CmdLen = Data;                              // LSB
    SerialStruct->Crc16 = CalcCrc(Data, SerialStruct->Crc16);
    SerialStruct->State += 10;
    break;
  case 30:
      SerialStruct->CmdLen += (uint16_t) Data << 8;              // MSB
      SerialStruct->Length = SerialStruct->CmdLen;
      SerialStruct->Crc16 = CalcCrc(Data, SerialStruct->Crc16);
      SerialStruct->State += 10;
      if ( SerialStruct->CmdLen == 0 ) {
          SerialStruct->State += 10;
      }
      if ( SerialStruct->CmdLen > SerialStruct->TxRxBufferSize ) {
          SerialStruct->State = 0;          // Make sure Length is valid
      }
      SerialStruct->Offset = 0;
      break;
  case 40:
      SerialStruct->TxRxBuffer[SerialStruct->Offset++] = Data;
      SerialStruct->Crc16 = CalcCrc(Data, SerialStruct->Crc16);
      SerialStruct->CmdLen--;
      if ( SerialStruct->CmdLen == 0 ) {
          SerialStruct->State += 10;
      }
      break;
  case 50:
    SerialStruct->CmdCrc = Data;                          // LSB
    SerialStruct->State += 10;
    break;
  case 60:
    SerialStruct->CmdCrc += (uint16_t) Data << 8;          // MSB
    if (SerialStruct->InstanceNumber == 2) {    // TESTING
      SerialStruct->SyncCount++;
    }
    if (( SerialStruct->CmdCrc == SerialStruct->Crc16 ) &&
            (((SerialStruct->InstanceNumber == 2)) ||
            ((SerialStruct->InstanceNumber == 1) && (SendMessageToCD == 0)) ||
            ((SerialStruct->InstanceNumber == 4) && (SendMessageToPC == 0))) ){
      ProcessSerialMessage(SerialStruct);             // Process Command if CRC's match
    }
    SerialStruct->State = 0;
    break;
  }
}
// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// This routine is called on TX Complete
//
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if ( huart->Instance == USART1 ) {
        HAL_GPIO_WritePin( PC_RS485_EN_GPIO_Port, PC_RS485_EN_Pin, GPIO_PIN_RESET);            // Turn off RS422/485 Driver
        SerialUsart1.Transmitting = FALSE;
    }
  if ( huart->Instance == USART2 ) {
      SerialUsart2.Transmitting = FALSE;
      if (SysPars.CommConfig == COMM_CD_DIGIO1) {
          HAL_GPIO_WritePin(DIG_HandleStruct[0].Dir_Port, DIG_HandleStruct[0].Dir_Pin, GPIO_PIN_RESET);   //Config DIGIO1 Buffer as an input
      }
      HAL_UART_Receive_IT(&huart2, &SerialUsart2.RxChar, 1);
  }
  if ( huart->Instance == USART4 ) {
      HAL_GPIO_WritePin( RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET);            // Turn off RS422/485 Driver
      SerialUsart4.Transmitting = FALSE;
  }
}

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// This routine is called on RX Complete
//
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if ( huart->Instance == USART1 ) {                                  // To/From PC
      if (!SerialUsart1.Transmitting) {
        ProcessSerialData(&SerialUsart1);
        }
      HAL_UART_Receive_IT(&huart1, &SerialUsart1.RxChar, 1);
  }
  if ( huart->Instance == USART2 ) {                                // To/From Sender TTL (can be either half duplex or full)
      if (!SerialUsart2.Transmitting) {
              if (SerialUsart2.ReceiveMode == 1) {
                  ProcessSerialData(&SerialUsart2);
              } else {
                  SerialUsart2.TxRxBuffer[SerialUsart2.TxRxBufferCount++] = SerialUsart2.RxChar;
              }
      }
      HAL_UART_Receive_IT(&huart2, &SerialUsart2.RxChar, 1);
  }
  if ( huart->Instance == USART4 ) {                                // To/From Sender RS485
          if (!SerialUsart4.Transmitting) {
              ProcessSerialData(&SerialUsart4);
          }
      HAL_UART_Receive_IT(&huart4, &SerialUsart4.RxChar, 1);
  }
}

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// HLR-xxxx
//
void SerialSend(stSerial *SerialStruct) {

  uint16_t Crc;
  uint16_t Counter7, TxDelay;

  Counter7 = __HAL_TIM_GET_COUNTER(&htim7);
  TxDelay = Counter7 + 250;                                                 // 500uS
  while (Counter7 < TxDelay) {
      Counter7 = __HAL_TIM_GET_COUNTER(&htim7);
  }

  SerialStruct->TxRxBuffer[0] = 0xAA;                    // Sync
  SerialStruct->TxRxBuffer[1] = 0xAA;
  SerialStruct->TxRxBuffer[2] = 0xAA;
  SerialStruct->TxRxBuffer[3] = 0xAA;
  SerialStruct->TxRxBuffer[4] = 0xAA;
  SerialStruct->TxRxBuffer[5] = 0x00;
  SerialStruct->TxRxBuffer[6] = SerialStruct->Command;                // Command
  SerialStruct->TxRxBuffer[7] = SerialStruct->Length & 0xFF;          // Length
  SerialStruct->TxRxBuffer[8] = SerialStruct->Length >> 8;
  //
  //  Calculate CRC
  //
  Crc = CalcCrcArray(&SerialStruct->TxRxBuffer[6], SerialStruct->Length + 3);
  SerialStruct->TxRxBuffer[SerialStruct->Length + 9] = Crc & 0xFF;
  SerialStruct->TxRxBuffer[SerialStruct->Length + 10] = Crc >> 8;
  //
  // Send Message via selected SerialMode Mode.
  //
  if (SerialStruct->InstanceNumber == 1) {
      HAL_GPIO_WritePin( PC_RS485_EN_GPIO_Port, PC_RS485_EN_Pin, GPIO_PIN_SET);                              // Turn on RS422/485 Driver
      SerialUsart1.Transmitting = TRUE;
      HAL_UART_Transmit_DMA(&huart1, (uint8_t *) &SerialStruct->TxRxBuffer[0], SerialStruct->Length + 11);
  }
  if (SerialStruct->InstanceNumber == 2) {
      SerialUsart2.Transmitting = TRUE;
    HAL_UART_Transmit_IT(&huart2, (uint8_t *) &SerialStruct->TxRxBuffer[0], SerialStruct->Length + 11);
  }
  if (SerialStruct->InstanceNumber == 4) {
      HAL_GPIO_WritePin( RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET);                              // Turn on RS422/485 Driver
      SerialUsart4.Transmitting = TRUE;
      HAL_UART_Transmit_DMA(&huart4, (uint8_t *) &SerialStruct->TxRxBuffer[0], SerialStruct->Length + 11);
  }
}

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// This routine is called when a valid message has been received
//
void ProcessSerialMessage(stSerial *SerialStruct) {

  uint16_t  FlashIndex, i;
  uint8_t   *BytePointer;
  static int32_t OldConfig[6], OldFreq[6], OldDuty[6], OldLevel[6];
  static int32_t    OldEnable;
  DIG_InitTypeDef   DIG_InitStruct;

  //
  // If Comm Mode is RS485 and
  //
  if ((SysPars.CommConfig == COMM_CD_RS485) && (SerialStruct->InstanceNumber == 4)) {
        SerialUsart1.TxRxBuffer[9] = SerialStruct->Command;
        memmove(&SerialUsart1.TxRxBuffer[10], &SerialStruct->TxRxBuffer[0], SerialStruct->Length);
        SerialUsart1.Length = SerialStruct->Length + 1;
        SerialUsart1.Command = 0xBC;
        SerialSend(&SerialUsart1);
        return;
    }
  if ((SysPars.CommConfig == COMM_CD_DIGIO1) && (SerialStruct->InstanceNumber == 2)) {
        SerialUsart1.TxRxBuffer[9] = SerialStruct->Command;
        if (SerialStruct->Length < (SerialStruct->TxRxBufferSize - 15)) {
            memmove(&SerialUsart1.TxRxBuffer[10], &SerialStruct->TxRxBuffer[0], SerialStruct->Length);
        }
        SerialUsart1.Length = SerialStruct->Length + 1;
        SerialUsart1.Command = 0xBC;
        SerialSend(&SerialUsart1);
        return;
    }
  //
  // Read FlashConfiguration Data from Programmer Memory
  //
  if (SerialStruct->Command == 0xB1) {
      if (SerialStruct->Length == 1) {
          FlashIndex = SerialStruct->TxRxBuffer[0];
          SerialStruct->TxRxBuffer[9] = FlashIndex;
          BytePointer = (uint8_t *)FlashConfiguration;
          memmove(&SerialStruct->TxRxBuffer[10], &BytePointer[FlashIndex * 512], 512);
          SerialStruct->Length = 513;
      } else {
          SerialStruct->Length = 0;
      }
      SerialSend(SerialStruct);
  }
  //
  // Read FactoryConfiguration Data from Programmer Memory
  //
  if (SerialStruct->Command == 0xB2) {
      if (SerialStruct->Length == 1) {
          FlashIndex = SerialStruct->TxRxBuffer[0];
          SerialStruct->TxRxBuffer[9] = FlashIndex;
          BytePointer = (uint8_t *)FactoryConfiguration;
          memmove(&SerialStruct->TxRxBuffer[10], &BytePointer[FlashIndex * 512], 512);
          SerialStruct->Length = 513;
      } else {
          SerialStruct->Length = 0;
      }
      SerialSend(SerialStruct);
  }
  //
  //  Write FlashConfiguration Data to Programmer Memory
  //
  if (SerialStruct->Command == 0xB3) {
      if (SerialStruct->Length == 2048)  {
          ReflashTuningParameters(63, (uint16_t *)&SerialStruct->TxRxBuffer[0]);
      }
      SerialStruct->Command = 0xB3;
      SerialStruct->Length = 0;
      SerialSend(SerialStruct);
  }
  //
  // Write FactoryConfiguration Data to Programmer Memory
  //
  if (SerialStruct->Command == 0xB4) {
      if (SerialStruct->Length == 2048) {
          ReflashTuningParameters(62, (uint16_t *)&SerialStruct->TxRxBuffer[0]);
      }
      SerialStruct->Command = 0xB4;
      SerialStruct->Length = 0;
      SerialSend(SerialStruct);
  }
  //
  // Read CC Series SysVars Data
  //
  if (SerialStruct->Command == 0xB5) {
      memmove(&SerialStruct->TxRxBuffer[9], &SysVarsCC, sizeof(SysVarsCC));
      SerialStruct->Length = sizeof(SysVarsCC);
      SerialSend(SerialStruct);
  }
  //
  // Return Firmware Version Information of Programmer
  //
  if (SerialStruct->Command == 0xB6) {
    //
    // Calculate Firmware Checksum - excluding last 2 pages of flash from calculation
    //
//      uint16_t Crc = CalcCrcArray((uint8_t *) 0x08000000, 0x20000 - (2048 * 2));  // Last two pages are Calibration/Configuration
      uint16_t Crc = 0;

    SerialStruct->TxRxBuffer[9] = Crc & 0xFF;
    SerialStruct->TxRxBuffer[10] = Crc >> 8;
//    memmove(&SerialStruct->Data[11], Version, sizeof(Version));
//    SerialStruct->Length = sizeof(Version) + 2;
    SerialSend(SerialStruct);
  }
  //
  // Get CC Series EEPROM Config Data
  //
  if ( SerialStruct->Command == 0xB7 )  {
      memmove(&SerialStruct->TxRxBuffer[9], &EEPROMData, 12);
      SerialStruct->Command = 0xB7;
      SerialStruct->Length = 12;
      SerialSend(SerialStruct);
  }
  //
  // Read CC Series EEPROM Config/Profile Data
  //
  if (SerialStruct->Command == 0xB8) {
      if (SerialStruct->Length == 1) {
          FlashIndex = SerialStruct->TxRxBuffer[0];
          SerialStruct->TxRxBuffer[9] = FlashIndex;
          BytePointer = (uint8_t *)&EEPROMData;
          memmove(&SerialStruct->TxRxBuffer[10], &BytePointer[FlashIndex * 264], 264);
          SerialStruct->Length = 265;
      } else {
          SerialStruct->Length = 0;
      }
      SerialStruct->Command = 0xB8;
      SerialSend(SerialStruct);
  }
  //
  // Write to CC Series EEPROM
  //
  if ( SerialStruct->Command == 0xB9 ) {
      if ( SerialStruct->Length == sizeof(EEPROMData) )  {              // Set EEPROM Data
          memmove(&EEPROMData, &SerialStruct->TxRxBuffer[0], sizeof( EEPROMData ));
          bConfigUpdateProfile = TRUE;
      }
      SerialStruct->Command = 0xB9;
      SerialStruct->Length = 0;
      SerialSend(SerialStruct);
  }
  //
  // Set SerialVerifyData Mode for CC Series
  //
  if ( SerialStruct->Command == 0xBA ) {
      if ( SerialStruct->Length == sizeof(SerialVerifyData) ) {      // Set SerialVerifyData
          memmove(&SerialVerifyData, &SerialStruct->TxRxBuffer[0], sizeof( SerialVerifyData ));
          bSerialVerify = TRUE;
      }
      SerialStruct->Command = 0xBA;
      SerialStruct->Length = 0;
      SerialSend(SerialStruct);
  }
  //
  // Flash new Firmware into Programmer
  //
  if ( (SerialStruct->Command == 0xBB) && (SerialStruct->Length == 4) ) {
    if ( (SerialStruct->TxRxBuffer[0] == 0x12) && (SerialStruct->TxRxBuffer[1] == 0x34) && (SerialStruct->TxRxBuffer[2] == 0x56) && (SerialStruct->TxRxBuffer[3] == 0x78) ) {
      reflash(SerialStruct);
    }
  }
  //
  // Read Programming Unit System Variables
  //
  if (SerialStruct->Command == 0x70) {
      if (SerialStruct->Length == sizeof(SysPars))  {
          memmove(&SysPars, &SerialStruct->TxRxBuffer[0], sizeof(SysPars));
      }
	  SysVars.DigIO_InputLevel = 0;
      for (i = 0 ; i < 6 ; i++) {
          SysVars.DigIO_InputLevel |= DIG_ReadPin(i) << i;
      }
      SysVars.FirmwareVersion = 0x55AA0000 + (VERSION_MAJOR << 8) + VERSION_MINOR;
      memmove(&SerialStruct->TxRxBuffer[9], &SysVars, sizeof(SysVars));
      SerialStruct->Length = sizeof(SysVars);
      SerialSend(SerialStruct);

      if (((SysPars.EnableBits & 0x01) != 0) && (SysVars.OverCurrentFlag == 0)) {
          HAL_GPIO_WritePin( PWR_EN_GPIO_Port, PWR_EN_Pin, GPIO_PIN_SET);
      } else {
          HAL_GPIO_WritePin( PWR_EN_GPIO_Port, PWR_EN_Pin, GPIO_PIN_RESET);
      }
      if ((SysPars.EnableBits & 0x02) != 0) {
          HAL_GPIO_WritePin( EXT_PULLUP_GPIO_Port, EXT_PULLUP_Pin, GPIO_PIN_SET);
      } else {
          HAL_GPIO_WritePin( EXT_PULLUP_GPIO_Port, EXT_PULLUP_Pin, GPIO_PIN_RESET);
      }

      if (((SysPars.EnableBits & ENABLE_POWER) != 0) && ((OldEnable & ENABLE_POWER) == 0)) {
          bConfigRefresh = TRUE;
      }
      if (((SysPars.EnableBits & ENABLE_REFRESH_CONFIG) != 0) && ((OldEnable & ENABLE_REFRESH_CONFIG) == 0)) {
          bConfigRefresh = TRUE;
      }
      if (((SysPars.EnableBits & ENABLE_REFRESH_PROFILE) != 0) && ((OldEnable & ENABLE_REFRESH_PROFILE) == 0)) {
          bProfileRefresh = TRUE;
      }
      if (((SysPars.EnableBits & ENABLE_UPDATE_CONFIG) != 0) && ((OldEnable & ENABLE_UPDATE_CONFIG) == 0)) {
          bConfigUpdateConfig = TRUE;
      }
      OldEnable = SysPars.EnableBits;

      for ( i = 0 ; i < 6 ; i++) {
          if ((SysPars.DigIO_Configuration[i] != OldConfig[i]) || (SysPars.DigIO_FrequencySetpoint[i] != OldFreq[i]) || (SysPars.DigIO_DutyCycle[i] != OldDuty[i])) {
              OldConfig[i] = SysPars.DigIO_Configuration[i];
              OldFreq[i] = SysPars.DigIO_FrequencySetpoint[i];
              OldDuty[i] = SysPars.DigIO_DutyCycle[i];
              DIG_InitStruct.Mode = OldConfig[i];
              DIG_InitStruct.Frequency = SysPars.DigIO_FrequencySetpoint[i];
              DIG_InitStruct.DutyCycle = SysPars.DigIO_DutyCycle[i];
              DIG_Init(i, DIG_InitStruct);
          }
          if (SysPars.DigIO_Configuration[i] == DIG_OUT) {
              if (SysPars.DigIO_OutputLevel[i] != OldLevel[i]) {
                  OldLevel[i] = SysPars.DigIO_OutputLevel[i];
                  if (OldLevel[i] != 0) {
                      DIG_SetPin(i);
                  } else {
                      DIG_ResetPin(i);
                  }
              }
          }
      }
  }
  //
  // Relay Message to CD Series Sender
  //
  if ( SerialStruct->Command == 0xBC) {
      if (SysPars.CommConfig == COMM_CD_RS485) {
          SerialUsart4.Command = SerialStruct->TxRxBuffer[0];
          SerialUsart4.Length = SerialStruct->Length - 1;
          if (SerialUsart4.Length != 0) {
              memmove(&SerialUsart4.TxRxBuffer[9], &SerialStruct->TxRxBuffer[1], SerialUsart4.Length);
          }
          SerialUsart4.ReceiveMode = 1;
          SerialSend(&SerialUsart4);
      }
      if (SysPars.CommConfig == COMM_CD_DIGIO1) {
          SerialUsart2.Command = SerialStruct->TxRxBuffer[0];
          SerialUsart2.Length = SerialStruct->Length - 1;
          if (SerialUsart2.Length != 0) {
              memmove(&SerialUsart2.TxRxBuffer[9], &SerialStruct->TxRxBuffer[1], SerialUsart2.Length);
          }
          HAL_GPIO_WritePin(DIG_HandleStruct[0].Dir_Port, DIG_HandleStruct[0].Dir_Pin, GPIO_PIN_SET);   //Config DIGIO1 as an output driven by USART2_TX
          SerialUsart2.ReceiveMode = 1;
          SerialSend(&SerialUsart2);
      }
  }
}

