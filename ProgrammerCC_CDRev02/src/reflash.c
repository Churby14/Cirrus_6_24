/*
 * reflash.c
 *
 *  Created on: Nov 14, 2017
 *      Author: Rick
 */
#include "global.h"

HAL_StatusTypeDef WaitForLastOperation(void) __attribute__ ((section (".reflash_text")));
uint16_t CalculateCrcArray(uint8_t *Buffer, int32_t Length) __attribute__ ((section (".reflash_text")));
void ReflashSerialSend(stSerial *SerialStruct) __attribute__ ((section (".reflash_text")));
int32_t GetSerialMessage(stSerial *SerialStruct) __attribute__ ((section (".reflash_text")));
void reflash(stSerial *SerialStruct) __attribute__ ((section (".reflash_text")));
HAL_StatusTypeDef Flash_Program(uint32_t Address, uint16_t *WordPtr) __attribute__ ((section (".reflash_text")));

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
uint32_t MaxTimeout;
#define MAX_WAIT_TIME           0xFFFFFF00

HAL_StatusTypeDef WaitForLastOperation(void) {

  uint32_t TimeoutCounter = 0;

  while ( __HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) ) {                                       // Macro
    if ( ++TimeoutCounter == MAX_WAIT_TIME ) {
      return HAL_TIMEOUT;
    }
  }
  if ( TimeoutCounter > MaxTimeout ) {
    MaxTimeout = TimeoutCounter;
  }

  if ( __HAL_FLASH_GET_FLAG(FLASH_FLAG_EOP) ) {                                       // Macro            /* Check FLASH End of Operation flag  */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);                     // Macro          /* Clear FLASH End of Operation pending bit */
  }
  if ( __HAL_FLASH_GET_FLAG(FLASH_FLAG_WRPERR) || __HAL_FLASH_GET_FLAG(FLASH_FLAG_PGERR) ) {       // Macro's
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);   // Macro - Clear all FLASH flags
    return HAL_ERROR;
  }
  return HAL_OK; /* There is no error flag set */
}

// ******************************************************************************
// ******************************************************************************
//
uint16_t CalculateCrcArray(uint8_t *Buffer, int32_t Length) {

    int32_t i;

    CRC->INIT = 0x0000;
    CRC->POL = 0x1021;
    CRC->CR = (1 << 3) | (1 << 0);          // 16-bit poly, reset

    for ( i = 0; i < Length; i++ ) {
        *(uint8_t*) (&CRC->DR) = Buffer[i];
    }
    return (CRC->DR & 0xFFFF);
}

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// HLR-xxxx
//
void ReflashSerialSend(stSerial *SerialStruct) {

  uint16_t Crc, i;

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
  Crc = CalculateCrcArray(&SerialStruct->TxRxBuffer[6], SerialStruct->Length + 3);
  SerialStruct->TxRxBuffer[SerialStruct->Length + 9] = Crc & 0xFF;
  SerialStruct->TxRxBuffer[SerialStruct->Length + 10] = Crc >> 8;
  //
  // Send Message via selected SerialMode Mode.
  //
  PC_RS485_EN_GPIO_Port->BSRR = (uint32_t) PC_RS485_EN_Pin;               // Turn on RS422/485 Driver

  for (i = 0 ; i < 20000  ; i++) {
      Dummy32++;
  }

  for (i = 0 ; i < SerialStruct->Length + 11 ; i++) {
      USART1->TDR = SerialStruct->TxRxBuffer[i];                          // Send Data
      while ( (USART1->ISR & (1 << 6)) == 0 ) {                           // Wait for Byte to be sent (Transmission Complete)
      }
  }
  PC_RS485_EN_GPIO_Port->BRR = (uint32_t) PC_RS485_EN_Pin;               // Turn off RS422/485 Driver
}

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
int32_t GetSerialMessage(stSerial *SerialStruct) {

  uint32_t  State, Data, SyncCount, InitialCount;
  uint32_t  CmdLen, Offset, CmdCrc;

  State = 0;
  SyncCount = 0;

  CRC->INIT = 0x0000;
  CRC->POL = 0x1021;
  CRC->CR = (1 << 3) | (1 << 0);          // 16-bit poly, reset

  Data = 0;
  CmdCrc = 0;
  Offset = 0;
  CmdLen = 0;
  while ( TRUE ) {
    //
    // Wait for Data
    //
    USART1->ICR = UART_CLEAR_PEF | UART_CLEAR_FEF | UART_CLEAR_NEF | UART_CLEAR_OREF;
    InitialCount = 0;
    while ((USART1->ISR & (1 << 5)) == 0 ) {
        InitialCount++;
        if (InitialCount > 700000) {
            return FALSE;
        }
    }
    Data = USART1->RDR & 0xFF;
    //
    // NOTE:  Do not use a Switch statement, as the addresses of the cases are stored in a lookup table that is
    //  being placed in the area of flash that gets erased, causing the switch statement to blow-up!
    //
    if (State == 0) {
        if ( Data == 0xAA ) {
          SyncCount++;
        } else {
          if ( (Data == 0x00) && (SyncCount >= 3) ) {
            State++;  // Must have received at least 3 of the 5 0xAA's
          }
          SyncCount = 0;
         }
    } else if (State == 1) {
        CRC->CR = (1 << 3) | (1 << 0);          // 16-bit poly, reset - Initialize CRC Generator
        *(uint8_t*) (&CRC->DR) = Data;
        State++;
        if ( Data != 0xEE ) {
          State = 0;                        // Command Must be 0xEE
        }
    } else if (State == 2) {
        CmdLen = Data;                              // LSB
        *(uint8_t*) (&CRC->DR) = Data;
        State++;
    } else if (State == 3) {
        CmdLen += (uint16_t) Data << 8;              // MSB
        *(uint8_t*) (&CRC->DR) = Data;
        State++;
        if ( CmdLen != 2052 ) {
          State = 0;                            // Data Length must be 2052, 4 bytes for Page Number, 2048 bytes of page Data
        }
        Offset = 0;
    } else if (State == 4) {
        SerialStruct->TxRxBuffer[Offset++] = Data;
        *(uint8_t*) (&CRC->DR) = Data;
        CmdLen--;
        if ( CmdLen == 0 ) {
          State++;
        }
    } else if (State == 5) {
        CmdCrc = Data;                          // LSB
        State++;
    } else if (State == 6) {
        State = 0;
        CmdCrc += (uint16_t) Data << 8;          // MSB
        if ( CmdCrc == (CRC->DR & 0xFFFF) ) {
          return TRUE;
        }
    }
  }
}

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
void reflash(stSerial *SerialStruct) {

  uint16_t  PageNumber, i;
  int32_t   Response, FirmwarePageNumber;

  __disable_irq();
  __HAL_RCC_CRC_CLK_ENABLE();
  //
  // Unlock FLASH
  //
  if ( HAL_IS_BIT_SET(FLASH->CR, FLASH_CR_LOCK) ) {     // Macro
    WRITE_REG(FLASH->KEYR, FLASH_KEY1);                 // Macro
    WRITE_REG(FLASH->KEYR, FLASH_KEY2);                 // Macro
  }
  //
  // Clear Error Flags
  //
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);// Macro - Clear all FLASH flags

  for (i = 0 ; i < 56 ; i++) {
      while (1) {
          SerialStruct->TxRxBuffer[9] = i;
          SerialStruct->Command = 0xEE;
          SerialStruct->Length = 1;
          ReflashSerialSend(SerialStruct);
          Response = GetSerialMessage(SerialStruct);
          FirmwarePageNumber = SerialStruct->TxRxBuffer[0];
           if (Response && (i == FirmwarePageNumber)) {
              PageNumber = i;
              if (PageNumber == 55) {
                  PageNumber = 56;
                  SerialStruct->TxRxBuffer[2050] = 0x55;
                  SerialStruct->TxRxBuffer[2051] = 0xAA;
              }
              Response = Flash_Program(0x08000000 + (PageNumber * FLASH_PAGE_SIZE), (uint16_t *)&SerialStruct->TxRxBuffer[4]);
              break;
          }
      }
  }

  SerialStruct->TxRxBuffer[9] = 99;
  SerialStruct->Command = 0xEE;
  SerialStruct->Length = 1;
  ReflashSerialSend(SerialStruct);
  //
  // Lock Flash
  //
  SET_BIT(FLASH->CR, FLASH_CR_LOCK);// Macro
  //
  // Issue System Reset Command
  //
  __DSB(); /* Ensure all outstanding memory accesses included buffered write are completed before reset */
  SCB->AIRCR = ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk);
  __DSB(); /* Ensure completion of memory access */
  //
  // Configure Watchdog
  //
  WRITE_REG(IWDG->KR, IWDG_KEY_ENABLE);
  WRITE_REG(IWDG->KR, IWDG_KEY_WRITE_ACCESS_ENABLE);
  IWDG->PR = IWDG_PRESCALER_256;
  IWDG->RLR = (500 * 10) / 64;
  while ( IWDG->SR != RESET ) {
  }
  IWDG->WINR = (250 * 10) / 64;
  for ( ; ; ) {
    __NOP();            // Wait for Watchdog to timeout
  }
}
//
// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
HAL_StatusTypeDef Flash_Program(uint32_t Address, uint16_t *WordPtr) {

  HAL_StatusTypeDef status;

  status = WaitForLastOperation();
  if ( status == HAL_OK ) {
    SET_BIT(FLASH->CR, FLASH_CR_PER);           // Erase current page
    WRITE_REG(FLASH->AR, Address);
    SET_BIT(FLASH->CR, FLASH_CR_STRT);
    status = WaitForLastOperation(); /* Wait for last operation to be completed */
    CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
  }
  if ( status != HAL_OK ) {
    return status;
  }
  status = WaitForLastOperation(); /* Wait for last operation to be completed */
  if ( status == HAL_OK ) {
    for ( uint32_t index = 0 ; index < (2048 / 2) ; index++ ) {
      SET_BIT(FLASH->CR, FLASH_CR_PG); /* Proceed to program the new data */
      *(__IO uint16_t*) Address = WordPtr[index]; /* Write data in the address */
      Address += 2;
      status = WaitForLastOperation(); /* Wait for last operation to be completed */
      CLEAR_BIT(FLASH->CR, FLASH_CR_PG); /* If the program operation is completed, disable the PG Bit */
      if ( status != HAL_OK ) {
        break;
      }
    }
  }
  return status;
}
