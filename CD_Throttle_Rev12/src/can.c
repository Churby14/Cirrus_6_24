// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Filename:      can.c
// Author:        Charlie Urban
// Revision:      1.3
//
// Description:
//
// ------------------------------------------------------------------------
//
// ------------------------------------------------------------------------
//
// Revision Log:
//
//  1.4    CRU     08/03/2023 Built in CAN TEC checking routine and startup delay
//
// ------------------------------------------------------------------------
//
// Design Details and Explanations:
//
// ------------------------------------------------------------------------
//
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
#define MAX_MSG_MISSED      100

// ------------------------------------------------------------------------
//                  Local Conditional Compilation Switches
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//                          Local Variables
// ------------------------------------------------------------------------
static uint8_t Spn_Byte_Count; // Counts number of bytes in the SPN that have been sent
static uint8_t Pgn_Count; // Counts number of SPNs in PGN that have been sent
static uint32_t CanMsgOutTmr[3];
static uint32_t MsgNotReceivedTmr[2];
static uint8_t MissedMsgIn[2];
static uint8_t bMsgReceived[2]; // Flag that is raised when a message is received so the timer can be cleared
static uint8_t bDelayComplete;
static int16_t CanStartupDelay;
static uint8_t TEC;
static uint8_t PrevTEC;
static uint16_t CanWaitTimer;
static uint8_t  bCanReady;


// ------------------------------------------------------------------------
//
// HAL_CAN_MSPInit - Initialize interrupt and pins for CAN Rx and Tx
//
// Params:    CAN Pointer
//
// Returns:   None
//
// Requirement(s):
//
void HAL_CAN_MspInit(CAN_HandleTypeDef * hcan) {

  if (hcan->Instance == CAN) {
    STM32_GPIO_Init(CANRXD_GPIO_Port, CANRXD_Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, GPIO_AF4_CAN, 0);
    STM32_GPIO_Init(CANTXD_GPIO_Port, CANTXD_Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, GPIO_AF4_CAN, 0);
    STM32_NVIC_SetPriority(CEC_CAN_IRQn, 0, 0);
    STM32_NVIC_EnableIRQ(CEC_CAN_IRQn);
  }
}

// ------------------------------------------------------------------------
//
// STM32_CAN_AddTxMessage - Routine to add a message to the avaialble Can Bus Transmit mailbox
//
// Params:    CAN Pointer
//
// Returns:   Status of STM32 CAN Bus
//
// Requirement(s):
//
enumHAL_StatusTypeDef STM32_CAN_AddTxMessage(CAN_HandleTypeDef * hcan, CanTxMsgTypeDef  *pHeader, uint8_t aData[], uint32_t *pTxMailbox) {

  uint32_t transmitmailbox;

  if ((hcan->State == HAL_CAN_STATE_READY) || (hcan->State == HAL_CAN_STATE_LISTENING)) {
    //
    // Check that all the Tx mailboxes are not full
    //
    if (((hcan->Instance->TSR & CAN_TSR_TME0) != 0U) || ((hcan->Instance->TSR & CAN_TSR_TME1) != 0U) ||
        ((hcan->Instance->TSR & CAN_TSR_TME2) != 0U)) {
      //
      // Select an empty transmit mailbox
      //
      transmitmailbox = (hcan->Instance->TSR & CAN_TSR_CODE) >> CAN_TSR_CODE_Pos;
      *pTxMailbox = (uint32_t)1 << transmitmailbox;
      //
      // Set up the Id
      //
      if (pHeader->IDE == CAN_ID_STD) {
        hcan->Instance->sTxMailBox[transmitmailbox].TIR = ((pHeader->StdId << CAN_TI0R_STID_Pos) | pHeader->RTR);
      }
      else {
        hcan->Instance->sTxMailBox[transmitmailbox].TIR = ((pHeader->ExtId << CAN_TI0R_EXID_Pos) |pHeader->IDE | pHeader->RTR);
      }
      //
      // Set data length and data into mailbox
      //
      hcan->Instance->sTxMailBox[transmitmailbox].TDTR = (pHeader->DLC);

      WRITE_REG(hcan->Instance->sTxMailBox[transmitmailbox].TDHR,
                ((uint32_t)aData[7] << CAN_TDH0R_DATA7_Pos) |
                ((uint32_t)aData[6] << CAN_TDH0R_DATA6_Pos) |
                ((uint32_t)aData[5] << CAN_TDH0R_DATA5_Pos) |
                ((uint32_t)aData[4] << CAN_TDH0R_DATA4_Pos));
      WRITE_REG(hcan->Instance->sTxMailBox[transmitmailbox].TDLR,
                ((uint32_t)aData[3] << CAN_TDL0R_DATA3_Pos) |
                ((uint32_t)aData[2] << CAN_TDL0R_DATA2_Pos) |
                ((uint32_t)aData[1] << CAN_TDL0R_DATA1_Pos) |
                ((uint32_t)aData[0] << CAN_TDL0R_DATA0_Pos));
      //
      // Set tranmission request bit
      //
      SET_BIT(hcan->Instance->sTxMailBox[transmitmailbox].TIR, CAN_TI0R_TXRQ);
      CanBus.Status &= ~0x02u;
      return HAL_OK;
    }
    // Set transmission error if more than 3 messages are trying to be sent
    else {
      CanBus.Status |= 0x02u;
    }
  }
  return HAL_OK;
}

// ------------------------------------------------------------------------
//
// STM32_CAN_ConfigFilter - CAN_FilterTypeDef to configure filters
//
// Params:    CAN Pointer and CAN Filter TypeDef
//
// Returns:   Status of STM32 CAN Bus
//
// Requirement(s):
//
enumHAL_StatusTypeDef STM32_CAN_ConfigFilter(CAN_HandleTypeDef *hcan, CAN_FilterTypeDef *sFilterConfig) {
  uint32_t filternbrbitpos; // bit position of the filter in the FilterBank

  if ((hcan->State == HAL_CAN_STATE_READY) || (hcan->State == HAL_CAN_STATE_LISTENING)) {
    //
    // To activate a filter, we need to enter inizialitation mode, deactivate the filter,
    // Set the size (16 or 32 bit) and mode (Mask/ID or ID) and assign to a FIFO
    //
    SET_BIT(hcan->Instance->FMR, CAN_FMR_FINIT);

    filternbrbitpos = (uint32_t)1 << (sFilterConfig->FilterBank & 0x1FU);

    CLEAR_BIT(hcan->Instance->FA1R, filternbrbitpos);

    if (sFilterConfig->FilterScale == CAN_FILTERSCALE_16BIT) {

      CLEAR_BIT(hcan->Instance->FS1R, filternbrbitpos);

      hcan->Instance->sFilterRegister[sFilterConfig->FilterBank].FR1 =
        ((0x0000FFFFU & (uint32_t)sFilterConfig->FilterMaskIdLow) << 16U) |
        (0x0000FFFFU & (uint32_t)sFilterConfig->FilterIdLow);

      hcan->Instance->sFilterRegister[sFilterConfig->FilterBank].FR2 =
        ((0x0000FFFFU & (uint32_t)sFilterConfig->FilterMaskIdHigh) << 16U) |
        (0x0000FFFFU & (uint32_t)sFilterConfig->FilterIdHigh);
    }

    if (sFilterConfig->FilterScale == CAN_FILTERSCALE_32BIT) {

      SET_BIT(hcan->Instance->FS1R, filternbrbitpos);

      hcan->Instance->sFilterRegister[sFilterConfig->FilterBank].FR1 =
        ((0x0000FFFFU & (uint32_t)sFilterConfig->FilterIdHigh) << 16U) |
        (0x0000FFFFU & (uint32_t)sFilterConfig->FilterIdLow);

      hcan->Instance->sFilterRegister[sFilterConfig->FilterBank].FR2 =
        ((0x0000FFFFU & (uint32_t)sFilterConfig->FilterMaskIdHigh) << 16U) |
        (0x0000FFFFU & (uint32_t)sFilterConfig->FilterMaskIdLow);
    }

    if (sFilterConfig->FilterMode == CAN_FILTERMODE_IDMASK) {
      CLEAR_BIT(hcan->Instance->FM1R, filternbrbitpos);
    }
    else {
      SET_BIT(hcan->Instance->FM1R, filternbrbitpos);
    }

    if (sFilterConfig->FilterFIFOAssignment == CAN_FILTER_FIFO0) {
      CLEAR_BIT(hcan->Instance->FFA1R, filternbrbitpos);
    }
    else {
      SET_BIT(hcan->Instance->FFA1R, filternbrbitpos);
    }

    if (sFilterConfig->FilterActivation == CAN_FILTER_ENABLE) {
      SET_BIT(hcan->Instance->FA1R, filternbrbitpos);
    }
    //
    // Leave the initialisation mode for the filter
    //
    CLEAR_BIT(hcan->Instance->FMR, CAN_FMR_FINIT);

    return HAL_OK;
  }
  else {
    hcan->ErrorCode |= HAL_CAN_ERROR_NOT_INITIALIZED;
    return HAL_ERROR;
  }
}

// ------------------------------------------------------------------------
//
// CAN_Filter_Init - Set receive filters to control which messages are read
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void CAN_Filter_Init(void) {

  //
  // Need to initialize the filter to receive messages that come from
  // CAN ID 0x8B4E09
  //

  CAN_FilterTypeDef CAN_FilterStruct1; // Typedef for Filter 1
  CAN_FilterTypeDef CAN_FilterStruct2; // Typedef for Filter 2
  //
  // Setup CAN Filter1 - Extended ID
  //
  CAN_FilterStruct1.FilterMode = CAN_FILTERMODE_IDMASK;
  CAN_FilterStruct1.FilterScale = CAN_FILTERSCALE_32BIT;

  CAN_FilterStruct1.FilterIdHigh = ((FlashConfiguration->CanAddressIn[0] >> 13) & 0xFFFF); // Ignore the 32nd bit as that should be 0
  CAN_FilterStruct1.FilterIdLow = (FlashConfiguration->CanAddressIn[0] <<3 & 0xFFF8); // Ignore the first 3 bits

  CAN_FilterStruct1.FilterMaskIdHigh = ((FlashConfiguration->CanMaskIn[0] >> 16) & 0xFFFF);
  CAN_FilterStruct1.FilterMaskIdLow = FlashConfiguration->CanMaskIn[0] & 0xFFFF;

  CAN_FilterStruct1.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  CAN_FilterStruct1.FilterBank = 0;
  CAN_FilterStruct1.FilterActivation = ENABLE;
  //
  // Setup CAN Filter2 - Standard ID
  //
  CAN_FilterStruct2.FilterMode = CAN_FILTERMODE_IDMASK; // Filter masked identifiers
  CAN_FilterStruct2.FilterScale = CAN_FILTERSCALE_32BIT; // Use 32 Bit to avoid filtering 2 IDs and Masks

  CAN_FilterStruct2.FilterIdHigh = FlashConfiguration->CanAddressIn[1] << 5;
  CAN_FilterStruct2.FilterIdLow = 0;

  CAN_FilterStruct2.FilterMaskIdHigh = FlashConfiguration->CanMaskIn[1] << 5;
  CAN_FilterStruct2.FilterMaskIdLow = 0;

  CAN_FilterStruct2.FilterFIFOAssignment = CAN_FILTER_FIFO1;
  CAN_FilterStruct2.FilterBank=1;
  CAN_FilterStruct2.FilterActivation = ENABLE;
  // Make sure filter configuration is error free
  if (FlashConfiguration->CanAddressIn[0] != 0) {
    if(STM32_CAN_ConfigFilter(&hcan, &CAN_FilterStruct1) != HAL_OK) {
      Error_Handler(FILE_CAN, __LINE__);
    }
  }
  if (FlashConfiguration->CanAddressIn[1] != 0) {
    if (STM32_CAN_ConfigFilter(&hcan, &CAN_FilterStruct2) != HAL_OK) {
      Error_Handler(FILE_CAN, __LINE__);
    }
  }
}

// ------------------------------------------------------------------------
//
// PGN_to_CAN_ID - This function converts paramater group numbers (pgn) to
//                 CAN IDs
//
// Params:    pgn: Parameter group number, priority: message priority, source: 1 byte
//                 address of the source
//
// Returns:   CAN ID based on the inputs
//
// Requirement(s):
//
uint32_t PGN_to_CAN_ID(uint32_t pgn, uint8_t priority, uint8_t source) {
  return  priority << 26 | pgn << 8 | source;
}

/*
 * This function convers the can_id (extended only) to pgn
 */
uint32_t CAN_ID_to_PGN(uint32_t can_id) {
  uint32_t pgn; // Parameter group number to be returned
  uint8_t pdu_format; // Protocol Data Unit - message format

  pdu_format = can_id & 0x00FF0000;

  if (pdu_format >= 0xF0) {
      pgn = ((can_id & 0x03FFFF00) >> 8);
  }
  else {
      pgn = ((can_id & 0x3FF0000) >> 8);
  }

  return pgn;
}

// ------------------------------------------------------------------------
//
// CAN_MessageTx - Sets apprpriate patrs of the Can Transmit TypeDef and runs the
//                 STM32_CAN_AddTxMessage function
//
// Params:    pgn: Parameter group number, priority: message priority, source: 1 byte
//            address of the source
//
// Returns:   Transmission mode, CAN Id to send message to, data to send
//
// Requirement(s):
//
void CAN_MessageTx(uint8_t Mode, uint32_t id, uint8_t data_tx[8]) {
  CanTxMsgTypeDef CAN_TxStruct;
  uint32_t tx_mailbox;

  switch(Mode) {
  // J1939 Throttle Output Message
  case CAN_TX_0:
    CAN_TxStruct.IDE = CAN_ID_EXT; // Extended ID for normal operation
    CAN_TxStruct.ExtId = FlashConfiguration->CanAddressOut[0]; // CAN Address extended ID
    CAN_TxStruct.DLC = 8; // Sending 8 bytes
    CAN_TxStruct.RTR = CAN_RTR_DATA; // Always RTR for our purposes
    break;
  // Fault message
  case CAN_TX_1:
    CAN_TxStruct.IDE = CAN_ID_EXT;
    CAN_TxStruct.ExtId = FlashConfiguration->CanAddressOut[1]; // extended ID for fault message
    CAN_TxStruct.DLC = 8;
    CAN_TxStruct.RTR = CAN_RTR_DATA;
    break;
  // Angular output message
  case CAN_TX_2:
    CAN_TxStruct.IDE = CAN_ID_EXT;
    CAN_TxStruct.ExtId = FlashConfiguration->CanAddressOut[2];
    CAN_TxStruct.DLC =8;
    CAN_TxStruct.RTR = CAN_RTR_DATA;
    break;
  // On startup - ECU Info
  case CAN_TX_3:
    CAN_TxStruct.IDE = CAN_ID_EXT;
    CAN_TxStruct.ExtId = FlashConfiguration->CanAddressOut[3];
    CAN_TxStruct.DLC =8;
    CAN_TxStruct.RTR = CAN_RTR_DATA;
    break;
  // On startup - Software Info
  case CAN_TX_4:
    CAN_TxStruct.IDE = CAN_ID_EXT;
    CAN_TxStruct.ExtId = FlashConfiguration->CanAddressOut[4];
    CAN_TxStruct.DLC =8;
    CAN_TxStruct.RTR = CAN_RTR_DATA;
    break;
  // On startup - Component Info
  case CAN_TX_5:
    CAN_TxStruct.IDE = CAN_ID_EXT;
    CAN_TxStruct.ExtId = FlashConfiguration->CanAddressOut[5];
    CAN_TxStruct.DLC =8;
    CAN_TxStruct.RTR = CAN_RTR_DATA;
    break;
  }
  //
  // This will put the message in the outbox and wait for its turn to transmit
  //
  if(STM32_CAN_AddTxMessage(&hcan, &CAN_TxStruct, data_tx, &tx_mailbox) != HAL_OK) {
    Error_Handler(FILE_CAN, __LINE__);
  }
}

// ------------------------------------------------------------------------
//
// Can_TxMultipacket - Sends multipacket info about the system
//
// Params:    Mode to run
//
// Returns:   None
//
// Requirement(s):
//
void Can_TxMultipacket(uint8_t Mode) {
  uint8_t pgnlength; // Number of SPNs in a PGN
  uint8_t sizeiter; // Picks the location of the CanBus.InfoSize pointer array to start at
  uint8_t i; // Counter for while loop to fill all 8 data byte frames of CAN message

  // 3 different CAN Multipacket messages
  switch (Mode) {

  case CAN_TX_3:
    pgnlength = sizeof(CanBus.ECUinfo) / 4;
    sizeiter = 0;
    break;

  case CAN_TX_4:
    pgnlength = sizeof(CanBus.SWinfo) / 4;
    sizeiter = sizeof(CanBus.ECUinfo) / 4;
    break;

  case CAN_TX_5:
    pgnlength = sizeof(CanBus.Componentinfo) / 4;
    sizeiter = (sizeof(CanBus.ECUinfo) + sizeof(CanBus.SWinfo)) / 4;
    break;
  }

  if (Pgn_Count < pgnlength) {
    i = 0;
    while (i < 8) {
      //
      // If we have yet to send all bytes in the SPN then we keep sending
      //
      if (Spn_Byte_Count < CanBus.InfoSize[sizeiter + Pgn_Count]) {
        CanBus.Outbox[i] = CanBus.Caninfo[Mode - 3][Pgn_Count][Spn_Byte_Count];
        Spn_Byte_Count++;
        i++; // only increment i if we have filled the outbox[i] with a data bit
      }
      //
      // If we have sent all the bytes of SPN then go onto next SPN (if there is one) and send first byte
      //
      if (Spn_Byte_Count >= CanBus.InfoSize[sizeiter + Pgn_Count]) {
        Spn_Byte_Count = 0;
        Pgn_Count++;
      }
      //
      // Check if any SPNs remain
      //
      if (Pgn_Count >= pgnlength) {
        i = 8;
      }
    }
    CAN_MessageTx(Mode, 123, CanBus.Outbox);
  }
  else {
    //
    // Reset counters for next CAN multipacket message and move onto the next CAN message to be send
    //
    Pgn_Count = 0;
    Spn_Byte_Count = 0;
    CanBus.State++;
  }
}

// ------------------------------------------------------------------------
//
// CAN_Tx - Transmits messages onto the CAN outbox
//
// Params:    Mode to run in
//
// Returns:   None
//
// Requirement(s):
//
void CAN_Tx(uint8_t Mode) {
  static uint8_t tx0counter, tx1counter, tx2counter; // Number of transmissions sent for each CAN Mode
  uint8_t crc_buffer[11]; // Data for Crc8 calculation
  uint32_t pgn; // Parameter group number
  uint32_t address; // Address to send CAN message
  //
  // Need a counter from 0 to 15 for CAN messages - goes into msg[1]
  //
  if (tx0counter > 15) {
      tx0counter = 0;
  }
  if (tx1counter > 15) {
      tx1counter = 0;
  }
  if (tx2counter > 15) {
      tx2counter = 0;
  }
  //
  // We need to create the messages for CAN Communication. To select the mode we
  // take an input (Mode) which will be determined by the code. If all data entries
  // are valid we will use CAN_NORMAL_TX, however, if entries are invalid, we will
  // need to select CAN_TH_FAULT.
  //
  switch(Mode) {
    case CAN_TX_0:
      address = FlashConfiguration->CanAddressOut[0];
      pgn = CAN_ID_to_PGN(FlashConfiguration->CanAddressOut[0]);
      CanBus.Outbox[1] = (0xF0) | tx0counter;
      CanBus.Outbox[2] = (0xFC) | SysVars.MatchStatus;
      if (FlashConfiguration->SensorMode == 0){
        if (FlashConfiguration->Endianess != 0) {
          CanBus.Outbox[4] = ForeignSysVars.CanSetpoint & 0x00FF;
          CanBus.Outbox[3] = (ForeignSysVars.CanSetpoint & 0xFF00) >> 8;
        }
        else {
          CanBus.Outbox[3] = ForeignSysVars.CanSetpoint & 0x00FF;
          CanBus.Outbox[4] = (ForeignSysVars.CanSetpoint & 0xFF00) >> 8;
        }
      }
      else {
        if (FlashConfiguration->Endianess != 0) {
          CanBus.Outbox[4] = SysVars.CanSetpoint & 0x00FF;
          CanBus.Outbox[3] = (SysVars.CanSetpoint & 0xFF00) >> 8;
        }
        else {
          CanBus.Outbox[3] = SysVars.CanSetpoint & 0x00FF;
          CanBus.Outbox[4] = (SysVars.CanSetpoint & 0xFF00) >> 8;
        }
      }
      CanBus.Outbox[5] = 7 << 3; // Always 0x38 for torque mode %
      if (FlashConfiguration->SensorMode == 0){
        if (FlashConfiguration->Endianess != 0) {
          CanBus.Outbox[7] = SysVars.CanSetpoint & 0x00FF;
          CanBus.Outbox[6] = (SysVars.CanSetpoint & 0xFF00) >> 8;
        }
        else {
          CanBus.Outbox[6] = SysVars.CanSetpoint & 0x00FF;
          CanBus.Outbox[7] = (SysVars.CanSetpoint & 0xFF00) >> 8;
        }
      }
      else {
        if (FlashConfiguration->Endianess != 0) {
          CanBus.Outbox[7] = ForeignSysVars.CanSetpoint & 0x00FF;
          CanBus.Outbox[6] = (ForeignSysVars.CanSetpoint & 0xFF00) >> 8;
        }
        else {
          CanBus.Outbox[6] = ForeignSysVars.CanSetpoint & 0x00FF;
          CanBus.Outbox[7] = (ForeignSysVars.CanSetpoint & 0xFF00) >> 8;
        }
      }
      tx0counter++;
      break;
    case CAN_TX_1:
      address = FlashConfiguration->CanAddressOut[1];
      pgn = CAN_ID_to_PGN(FlashConfiguration->CanAddressOut[1]);
      CanBus.Outbox[1] = (0xF0) | tx1counter;
      CanBus.Outbox[2] = (0xF8) | SysVars.ErrorType;
      if (FlashConfiguration->Endianess != 0) {
        CanBus.Outbox[4] = SysVars.CriticalFault & 0xFF;
        CanBus.Outbox[3] = SysVars.CriticalFault >> 8;
      }
      else {
        CanBus.Outbox[3] = SysVars.CriticalFault & 0xFF;
        CanBus.Outbox[4] = SysVars.CriticalFault >> 8;
      }
      if (FlashConfiguration->Endianess != 0) {
        CanBus.Outbox[6] = SysVars.MajorFault & 0xFF;
        CanBus.Outbox[5] = SysVars.MajorFault >> 8;
      }
      else {
        CanBus.Outbox[5] = SysVars.MajorFault & 0xFF;
        CanBus.Outbox[6] = SysVars.MajorFault >> 8;
      }
      CanBus.Outbox[7] = SysVars.MinorFault;
      tx1counter++;
      break;
    case CAN_TX_2:
      address = FlashConfiguration->CanAddressOut[2];
      pgn = CAN_ID_to_PGN(FlashConfiguration->CanAddressOut[2]);
      if (FlashConfiguration->SensorMode == 0) {
        CanBus.Outbox[0] = SysVars.SensorAngle;
        CanBus.Outbox[1] = ForeignSysVars.SensorAngle;
      }
      else {
        CanBus.Outbox[0] = ForeignSysVars.SensorAngle;
        CanBus.Outbox[1] = SysVars.SensorAngle;
      }
      CanBus.Outbox[3] = 0xFF;
      CanBus.Outbox[4] = 0xFF;
      CanBus.Outbox[5] = 0xFF;
      CanBus.Outbox[6] = 0xFF;
      CanBus.Outbox[7] = 0xFF;
      tx2counter++;
      break;
  }
  if (FlashConfiguration->CanCrcEnable != 0) {
    //
    // Setup crc data
    //
    crc_buffer[0] = CanBus.Outbox[1];
    crc_buffer[1] = CanBus.Outbox[2];
    crc_buffer[2] = CanBus.Outbox[3];
    crc_buffer[3] = CanBus.Outbox[4];
    crc_buffer[4] = CanBus.Outbox[5];
    crc_buffer[5] = CanBus.Outbox[6];
    crc_buffer[6] = CanBus.Outbox[7];
    crc_buffer[7] = (address & 0xFF);
    crc_buffer[8] = pgn & 0xFF;
    crc_buffer[9] = ((pgn & 0xFF00) >> 8);
    crc_buffer[10] = ((pgn & 0xFF0000) >> 16);
    //
    // Calculate crc8
    //
    if (Mode == CAN_TX_2) {
      CanBus.Outbox[2] = CalcCrc8(crc_buffer, sizeof(crc_buffer));
    }
    else {
      CanBus.Outbox[0] = CalcCrc8(crc_buffer, sizeof(crc_buffer));
    }
  }
  else {
    if (Mode == CAN_TX_2) {
      CanBus.Outbox[2] = 0xFF;
    }
    else {
      CanBus.Outbox[0] = 0xFF; // Fill Crc with 1s if disabled
    }
  }

  // If echo mode is enabled, store the last message send to validate echoing
  if ((FlashConfiguration->CanEcho != 0) && (Mode == CAN_TX_0)) {
    memmove(&CanBus.LastMsg, &CanBus.Outbox, sizeof(CanBus.Outbox));
  }

  //
  // Output message via CANBus as long as all 3 mailboxes are not full
  //
  CAN_MessageTx(Mode, 123, CanBus.Outbox);
}

// ------------------------------------------------------------------------
//
// VerifyCanTxTimes - Validates messages are received at the appropriate increments
//
// Params:    Tolerance - Number of mSec a message can be late and be valid (must be multiple of 10)
//            AddressIdx - Which Address In timer to compare against
//
// Returns:   0 if no issues, 1 if message is too late
//
// Requirement(s):
//
void VerifyCanRxTimes(uint8_t AddressIdx, uint8_t Tolerance) {
  if (FlashConfiguration->CanMsgInTime[AddressIdx] != 0) {
    // When a message is received, ensure it is received within the allotted amount of time
    if (bMsgReceived[AddressIdx] != 0) {
      if (MsgNotReceivedTmr[AddressIdx] > (FlashConfiguration->CanMsgInTime[AddressIdx] + Tolerance)) {
        MissedMsgIn[AddressIdx]++;
        MsgNotReceivedTmr[AddressIdx] = 0;
      }
      else {
        MissedMsgIn[AddressIdx] = 0;
        MsgNotReceivedTmr[AddressIdx] = 0;
      }
      bMsgReceived[AddressIdx] = 0; // Clear received flag
    }
    // If no message is received, then see if the allotted amount of time has passed between the last message
    else {
      if (MsgNotReceivedTmr[AddressIdx] > (FlashConfiguration->CanMsgInTime[AddressIdx] + Tolerance)) {
        MissedMsgIn[AddressIdx]++;
      }
    }
  }
  else {
    MissedMsgIn[AddressIdx] = 0;
    MsgNotReceivedTmr[AddressIdx] = 0;
  }
}

// ------------------------------------------------------------------------
//
// CanEchoError - Determines whether CAN echo matches message send
//                based on the CRC8
//
// Params:    Which inbox to check for echoes (0 or 1)
//
// Returns:   1 if messages are being properly echoes, 0 if the
//            echoes message is wrong or did not updated
//
// Requirement(s):
//
void VerifyCanRx (stCanInbox CanData) {
  uint8_t crc_buffer[11]; // Data for CRC8 calculation
  uint32_t pgn; // Paramter group number
  uint32_t addressin; // Address of message to calculate pgn

  // Check if echoes messages match the previous message sent
  if (FlashConfiguration->CanEcho != 0) {
    for (uint8_t i=0; i<8; i++) {
      if (CanBus.LastMsg[i] != CanBus.Inbox[0].Data[i]) {
        CanBus.Status |= 0x04u;
        break;
      }
      else {
        CanBus.Status &= ~0x04u;
      }
    }
  }
  else {
    CanBus.Status &= ~0x04u;
  }
  // If echoing is not available but CRC is, then
  // validate CRC in messages vs calculated CRC
  if (FlashConfiguration->CanCrcEnable != 0) {
    addressin = CanData.Address;
    pgn = CAN_ID_to_PGN(addressin);
    crc_buffer[0] = CanData.Data[1];
    crc_buffer[1] = CanData.Data[2];
    crc_buffer[2] = CanData.Data[3];
    crc_buffer[3] = CanData.Data[4];
    crc_buffer[4] = CanData.Data[5];
    crc_buffer[5] = CanData.Data[6];
    crc_buffer[6] = CanData.Data[7];
    crc_buffer[7] = (addressin & 0xFF);
    crc_buffer[8] = pgn & 0xFF;
    crc_buffer[9] = ((pgn & 0xFF00) >> 8);
    crc_buffer[10] = ((pgn & 0xFF0000) >> 16);

    if (CalcCrc8(crc_buffer, sizeof(crc_buffer)) != CanData.Data[0]) {
      CanBus.Status |= 0x08u;
    }
    else {
      CanBus.Status &= ~0x08u;
    }
  }
  // If echo and crc are not available, then no additional message
  // validation is required
  else {
    // Ensure Echo and CRC invalid bits are not set
    CanBus.Status &= ~0x08u;
  }
}

// ------------------------------------------------------------------------
//
// CAN_Rx - Routine to retrieve infomation from inbox1 (FIFO0) and inbox2 (FIFO1), store it
//          in the CanBus data structure and then clear the mailbox so it can receive
//          future messages
//
// Params:    CAN Pointer
//            AddressIdx - Even Indices are assigned to Fifo 0, odd indices are assigned to Fifo 1
//
// Returns:   None
//
// Requirement(s):
//
void CAN_Rx (CAN_HandleTypeDef *hcan, uint8_t AddressIdx) {

  uint8_t FIFO; // Even AddressIdx, FIFO = 0, Odd FIFO = 1

  //
  // Routine to receive can messages
  //
  // If RX FIFO 0 is not empty read the data
  if (AddressIdx % 2 == 0) {
    FIFO = 0;
  }
  else {
    FIFO = 1;
  }
  // Read the FIFO for corresponding AddressIdx out if a message is pending
  if (((FIFO == 1) && (hcan->Instance->RF1R & 0x03)) || ((FIFO == 0) && (hcan->Instance->RF0R & 0x03))) {
    CanBus.Inbox[FIFO].Address = (FlashConfiguration->CanMaskIn[AddressIdx] & CAN->sFIFOMailBox[FIFO].RIR) >> 3u;;
    CanBus.Inbox[FIFO].Data[0] = ((CAN_RDL0R_DATA0 & hcan->Instance->sFIFOMailBox[FIFO].RDLR) >> CAN_RDL0R_DATA0_Pos);
    CanBus.Inbox[FIFO].Data[1] = ((CAN_RDL0R_DATA1 & hcan->Instance->sFIFOMailBox[FIFO].RDLR) >> CAN_RDL0R_DATA1_Pos);
    CanBus.Inbox[FIFO].Data[2] = ((CAN_RDL0R_DATA2 & hcan->Instance->sFIFOMailBox[FIFO].RDLR) >> CAN_RDL0R_DATA2_Pos);
    CanBus.Inbox[FIFO].Data[3] = ((CAN_RDL0R_DATA3 & hcan->Instance->sFIFOMailBox[FIFO].RDLR) >> CAN_RDL0R_DATA3_Pos);
    CanBus.Inbox[FIFO].Data[4] = ((CAN_RDH0R_DATA4 & hcan->Instance->sFIFOMailBox[FIFO].RDHR) >> CAN_RDH0R_DATA4_Pos);
    CanBus.Inbox[FIFO].Data[5] = ((CAN_RDH0R_DATA5 & hcan->Instance->sFIFOMailBox[FIFO].RDHR) >> CAN_RDH0R_DATA5_Pos);
    CanBus.Inbox[FIFO].Data[6] = ((CAN_RDH0R_DATA6 & hcan->Instance->sFIFOMailBox[FIFO].RDHR) >> CAN_RDH0R_DATA6_Pos);
    CanBus.Inbox[FIFO].Data[7] = ((CAN_RDH0R_DATA7 & hcan->Instance->sFIFOMailBox[FIFO].RDHR) >> CAN_RDH0R_DATA7_Pos);
    //Clear the appropriate FIFO
    if (FIFO == 1) {
      hcan->Instance->RF1R |= CAN_RF1R_RFOM1; // Release the receive mailbox
    }
    else {
      hcan->Instance->RF0R |= CAN_RF0R_RFOM0; // Release the receive mailbox
    }
    bMsgReceived[AddressIdx] = 1;
    VerifyCanRx (CanBus.Inbox[FIFO]);
  }


}

// ------------------------------------------------------------------------
//
// GetCanTxStatus - Routine checks if Transmissions are going well and if not
//                  sets a delay before retransmitting messages
//
// Params:    CAN Pointer
//
// Returns:   None
//
// Requirement(s):
//
void GetCanTxStatus(CAN_HandleTypeDef * hcan) {
  // Get error counters from the ESR register
  TEC = (hcan->Instance->ESR & CAN_ESR_TEC_Msk) >> CAN_ESR_TEC_Pos;
  // If the TEC counter has increased by 16, then temporarily stop messaging
  if (TEC >= (PrevTEC + 16)) {
    bCanReady = 0;
  }
  // If TEC Counter is decreasing, so should the PrevTEC as TEC
  // decreases on successful transmissions
  else {
    bCanReady = 1;
    PrevTEC = TEC;
  }
  // after CanSamplingDelay, try sending messages again
  if (bCanReady == 0) {
    if (CanWaitTimer > FlashConfiguration->CanSamplingDelay) {
      bCanReady = 1;
      CanWaitTimer = 0;
      PrevTEC = TEC;
    }
    else {
      CanWaitTimer++;
    }
  }
}

// ------------------------------------------------------------------------
//
// ProcessCan - Runs the CAN Bus operations for the Power Lever
//
// Params:    Which mode to run in
//
// Returns:   None
//
// Requirement(s):
//
void ProcessCan (void) {

  ExecutionStart(EXECUTION_PROCESS_CAN);

  // Implement a time delay before sending or receiving messages
  // Note that if DIGIO1 is sending and receiving serial messages
  // then there is a delay if the CAN message timing matches the
  // time delay timing.
  if (CanStartupDelay >= FlashConfiguration->OutputStartupDelay) {
    bDelayComplete = 1;
  }
  else {
    bDelayComplete = 0;
    CanStartupDelay++;
  }

  if ((bDelayComplete != 0) && (FlashConfiguration->CanUse != 0)) {

    // Routine increments the following timers (if enabled) every 10 mSec
    if (FlashConfiguration->CanMsgInTime[0] != 0){
      MsgNotReceivedTmr[0] += 10;
    }
    if (FlashConfiguration->CanMsgInTime[1] != 0){
      MsgNotReceivedTmr[1] += 10;
    }

    // Check that CAN Transmissions are working properly
    GetCanTxStatus(&hcan);

    // Transmit messages if enabled
    switch (CanBus.State) {
    // Cases 0-2 are only sent on startup if enabled
    case 0:
      if (FlashConfiguration->CanAddressOut[3] != 0) {
        Can_TxMultipacket(CAN_TX_3);
      }
      else {
        CanBus.State++;
      }
      break;

    case 1:
      if (FlashConfiguration->CanAddressOut[4] != 0) {
        Can_TxMultipacket(CAN_TX_4);
      }
      else {
        CanBus.State++;
      }
      break;

    case 2:
      if (FlashConfiguration->CanAddressOut[5] != 0) {
        Can_TxMultipacket(CAN_TX_5);
      }
      else {
        CanBus.State++;
      }
      break;

    // Cases 3-5 are sent at regular intervals based on the
    // FlashConfiguration->CanMsgOutTime value
    case 3:
      for (uint8_t i = 0; i<CAN_TX_MODES-3; i++) {
        if (FlashConfiguration->CanAddressOut[i] != 0) {
          CanMsgOutTmr[i] += 10;
          if (CanMsgOutTmr[i] >= FlashConfiguration->CanMsgOutTime[i]) {
            CAN_Tx(i);
            CanMsgOutTmr[i] = 0;
          }
        }
      }

      // Receive messages with addresses
      if (FlashConfiguration->CanAddressIn[0] != 0) {
        while(hcan.Instance->RF0R != 0) {
          CAN_Rx(&hcan, 0);
        }
        VerifyCanRxTimes(0, 10);
      }
      // Needed if receive mode is updated after being on
      else {
        MissedMsgIn[0] = 0;
      }

      if (FlashConfiguration->CanAddressIn[1] != 0) {
        while(hcan.Instance->RF1R != 0) {
          CAN_Rx(&hcan, 1);
        }
        VerifyCanRxTimes(1, 10);
      }
      else {
        MissedMsgIn[1] = 0;
      }

      if ((MissedMsgIn[0] >= MAX_MSG_MISSED) || (MissedMsgIn[1] >= MAX_MSG_MISSED)) {
        CanBus.Status |= 0x01u;
        // Keep Error flag at max value so no overflow occurs
        if (MissedMsgIn[0] >= MAX_MSG_MISSED) {
          MissedMsgIn[0] = MAX_MSG_MISSED;
        }
        else {
          MissedMsgIn[1] = MAX_MSG_MISSED;
        }
      }
      else {
        CanBus.Status &= ~0x01u;
      }
      break;
    }
  }
  else {
    CanBus.Status = 0;
  }

  ExecutionEnd(EXECUTION_PROCESS_CAN);
}

// ------------------------------------------------------------------------
//
// STM32_CAN_Init - Routine to initialize the CAN Bus based on the
//                  parameters set in the hcan->Instance pointer
//
// Params:    CAN Pointer
//
// Returns:   Status - HAL_OK: No error
//
// Requirement(s):
//
enumHAL_StatusTypeDef STM32_CAN_Init(CAN_HandleTypeDef * hcan) {

	/* Init the low level hardware: CLOCK, NVIC */
  HAL_CAN_MspInit(hcan);


    /* Exit from sleep mode */
  CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_SLEEP);


    /* Check Sleep mode leave acknowledge */
  while ((hcan->Instance->MSR & CAN_MSR_SLAK) != 0U) {}

    /* Request initialisation */
  SET_BIT(hcan->Instance->MCR, CAN_MCR_INRQ);

    /* Wait initialisation acknowledge */
  while ((hcan->Instance->MSR & CAN_MSR_INAK) == 0U) {}

    /* Set the time triggered communication mode */
  if (hcan->Init.TTCM == ENABLE) {
      SET_BIT(hcan->Instance->MCR, CAN_MCR_TTCM);
  }
  else {
    CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_TTCM);
  }
    /* Set the automatic bus-off management */
  if (hcan->Init.ABOM == ENABLE) {
    SET_BIT(hcan->Instance->MCR, CAN_MCR_ABOM);
  }
  else {
    CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_ABOM);
  }

    /* Set the automatic wake-up mode */
  if (hcan->Init.AWUM == ENABLE) {
    SET_BIT(hcan->Instance->MCR, CAN_MCR_AWUM);
  }
  else {
    CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_AWUM);
  }

    /* Set the automatic retransmission */
  if (hcan->Init.NART == ENABLE) {
    CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_NART);
  }
  else {
    SET_BIT(hcan->Instance->MCR, CAN_MCR_NART);
  }

    /* Set the receive FIFO not locked mode */
  if (hcan->Init.RFLM == ENABLE) {
    SET_BIT(hcan->Instance->MCR, CAN_MCR_RFLM);
  }
  else {
    CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_RFLM);
  }

    /* Set the transmit FIFO priority */
  if (hcan->Init.TXFP == ENABLE) {
    SET_BIT(hcan->Instance->MCR, CAN_MCR_TXFP);
  }
  else {
    CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_TXFP);
  }

    /* Set the bit timing register */
  WRITE_REG(hcan->Instance->BTR, (uint32_t)(hcan->Init.Mode | hcan->Init.SJW  | hcan->Init.BS1 | hcan->Init.BS2 | (hcan->Init.Prescaler - 1U)));
   /* Initialize the error code */
  hcan->ErrorCode = HAL_CAN_ERROR_NONE;

    /* Initialize the CAN state */
  hcan->State = HAL_CAN_STATE_READY;

    /* Return function status */
  return HAL_OK;
}

// ------------------------------------------------------------------------
//
// MX_CAN_Init - Fills CAN TypeDef with values to properly initialize the
//               CAN Bus
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void MX_CAN_Init(void) {

  if ((bFlashConfigurationValid != 0) && FlashConfiguration->CanUse != 0) {
    // Setup can initialization parameters
    hcan.Instance = CAN;
    if (FlashConfiguration->CanPrescaler != 0) {
      hcan.Init.Prescaler = FlashConfiguration->CanPrescaler;
    }
    // As a default, set the baud rate to 1000
    else {
      hcan.Init.Prescaler = 3;
    }
    hcan.Init.Mode = CAN_MODE_NORMAL;
    hcan.Init.SJW = CAN_SJW_1TQ;
    hcan.Init.BS1 = CAN_BS1_13TQ;
    hcan.Init.BS2 = CAN_BS2_2TQ;
    hcan.Init.TTCM = DISABLE;
    hcan.Init.ABOM = DISABLE;
    hcan.Init.AWUM = DISABLE;
    hcan.Init.NART = ENABLE;
    hcan.Init.RFLM = DISABLE;
    hcan.Init.TXFP = DISABLE;
    STM32_CAN_Init(&hcan);

    CAN_Filter_Init();

    // Exit Initialization mode
    CLEAR_BIT(hcan.Instance->MCR, CAN_MCR_INRQ);
    while((hcan.Instance->MSR & CAN_MSR_INAK) != 0U) {}
  }
}


