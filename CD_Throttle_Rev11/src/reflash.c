// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: reflash.c
// Author  : Rick Wright
// Revision: C
//
// Description: Routine allows parameter data items to be stored into FLASH
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Revision Log:
//
//  A           CRU   Baseline Release   12/21/2021
//  B           RLW   06/27/2022   Changes per PR #19 in PACT
//  C           CRU   06/30/2022   Changes per PR #22 in PACT
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

#define FLASH_PAGE_SIZE         2048
#define MAX_WAIT_TIME           0xFFFFFF00u

// ------------------------------------------------------------------------
//                  Local Conditional Compilation Switches
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//                          Local Variables
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
//      Functtion Prototypes
//
void WaitForLastOperation(void) __attribute__((section(".reflash_text")));
uint16_t CalculateCrcArray(uint8_t* Buffer, int32_t Length)__attribute__((section(".reflash_text")));
void ReflashSerialSend(stSerial* SerialStruct)__attribute__((section(".reflash_text")));
int32_t GetSerialMessage(stSerial* SerialStruct)__attribute__((section(".reflash_text")));
void reflash(stSerial* SerialStruct, uint16_t Crc, uint16_t Version)__attribute__((section(".reflash_text")));
void Flash_Program(uint32_t Address, uint16_t* WordPtr)__attribute__((section(".reflash_text")));
void MoveMem (uint8_t *dst, uint8_t *src, uint32_t len)__attribute__((section(".reflash_text")));

// ------------------------------------------------------------------------
//
// WaitForLastOperation - While the flash status register is busy
//                        this function disables the MCU from overwriting/reading new data
//                        to/from FLASH
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void WaitForLastOperation(void) {

    do {
    } while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY);
    if ((FLASH->SR & FLASH_FLAG_EOP) == FLASH_FLAG_EOP) {
        FLASH->SR = FLASH_FLAG_EOP;
    }
}

// ------------------------------------------------------------------------
//
// CalculateCrcArray - Calculates the CRC of data array - used to calculate the CRC
//                     of FLASH memory pages
//
// Params:    Buffer - Data array to have the CRC calculated
//            Length - Length of the buffer for looping through the data array
//
// Returns:   None
//
// Requirement(s):
//
uint16_t CalculateCrcArray(uint8_t *Buffer, int32_t Length) {

    int32_t i;

    CRC->INIT = 0x0000;
    CRC->POL = 0x1021;
    CRC->CR = (1u << 3) | (1u << 0);          // 16-bit poly, reset

    for (i = 0; i < Length; i++) {
        *(uint8_t *)(&CRC->DR) = Buffer[i];
    }
    return (CRC->DR & 0xFFFF);
}

// ------------------------------------------------------------------------
//
// ReflashSerialSend - Routine to send serial message containing a page of FLASH memory to host
//
// Params:    SerialStruct - Data structure containing the configuration of the serial mode used
//
// Returns:   None
//
// Requirement(s):
//
void ReflashSerialSend(stSerial *SerialStruct) {

    uint16_t Crc, i;

    SerialBuffer.ByByte[0] = 0xAA;                    // Sync
    SerialBuffer.ByByte[1] = 0xAA;
    SerialBuffer.ByByte[2] = 0xAA;
    SerialBuffer.ByByte[3] = 0xAA;
    SerialBuffer.ByByte[4] = 0xAA;
    SerialBuffer.ByByte[5] = 0x00;
    SerialBuffer.ByByte[6] = SerialStruct->Command;                // Command
    SerialBuffer.ByByte[7] = (uint8_t)(SerialStruct->Length & 0xFF);          // Length
    SerialBuffer.ByByte[8] = (uint8_t)(SerialStruct->Length >> 8);
    //
    //  Calculate CRC
    //
    Crc = CalculateCrcArray(&SerialBuffer.ByByte[6], SerialStruct->Length + 3);
    SerialBuffer.ByByte[SerialStruct->Length + 9] = (uint8_t)(Crc & 0xFF);
    SerialBuffer.ByByte[SerialStruct->Length + 10] = (uint8_t)(Crc >> 8);
    //
    // Send Message via selected SerialMode Mode.
    //
    if (SerialStruct == &SerialUsart1) {
        RS485_EN_GPIO_Port->BSRR = (uint32_t)RS485_EN_Pin;                         // Turn on RS422/485 Driver
    }
    if (SerialStruct == &SerialUsart2) {
        DIR1_GPIO_Port->BSRR = (uint32_t)DIR1_Pin;                         // Set DIG_IO as output
    }

    for (i = 0; i < 20000; i++) {
        Dummy32++;
    }

    for (i = 0; i < SerialStruct->Length + 11; i++) {
        if (SerialStruct == &SerialUsart1) {
            USART1->TDR = SerialBuffer.ByByte[i];                                                      // Send Data
            while ((USART1->ISR & (1u << 6)) == 0) {                                   // Wait for Byte to be sent (Transmission Complete)
            }
        }
        if (SerialStruct == &SerialUsart2) {
            USART2->TDR = SerialBuffer.ByByte[i];                                                     // Send Data
            while ((USART2->ISR & (1u << 6)) == 0) {                               // Transmission Complete
            }
        }
    }
    if (SerialStruct == &SerialUsart1) {
        RS485_EN_GPIO_Port->BRR = (uint32_t)RS485_EN_Pin;                         // Turn off RS422/485 Driver
    }
    if (SerialStruct == &SerialUsart2) {
        DIR1_GPIO_Port->BRR = (uint32_t)DIR1_Pin;                         // Set DIG_IO as input
    }
}

// ------------------------------------------------------------------------
//
// GetSerialMessage - Routine to receive serial message containing parameter data items from host
//
// Params:    SerialStruct - Data structure containing the configuration of the serial mode used
//
// Returns:   0 - if data was well received, 1 - if data contains errors or timeout occurred
//
// Requirement(s):
//
int32_t GetSerialMessage(stSerial *SerialStruct) {

    uint32_t  State, Data, SyncCount, InitialCount;
    uint32_t  CmdLen, Offset, CmdCrc;

    State = 0;
    SyncCount = 0;
    CRC->INIT = 0x0000;
    CRC->POL = 0x1021;
    CRC->CR = (1u << 3) | (1u << 0);          // 16-bit poly, reset
    Data = 0;
    CmdCrc = 0;
    Offset = 0;
    CmdLen = 0;
    while (TRUE) {
        //
        // Wait for Data
        //
        if (SerialStruct == &SerialUsart1) {
            USART1->ICR = UART_CLEAR_PEF | UART_CLEAR_FEF | UART_CLEAR_NEF | UART_CLEAR_OREF;
            //
            // Loop Waiting for data from Host.  Return FALSE if we timeout
            //
            InitialCount = 0;
            while ((USART1->ISR & (1u << 5)) == 0) {
                IWDG->KR = IWDG_KEY_RELOAD;                // Reset Watchdog
                InitialCount++;
                if (InitialCount > 2000000) {
                    return 1;
                }
            }
            Data = (uint32_t)USART1->RDR & 0xFFU;
        }
        if (SerialStruct == &SerialUsart2) {
            USART2->ICR = UART_CLEAR_PEF | UART_CLEAR_FEF | UART_CLEAR_NEF | UART_CLEAR_OREF;
            //
            // Loop Waiting for data from Host.  Return FALSE if we timeout
            //
            InitialCount = 0;
            while ((USART2->ISR & (1u << 5)) == 0) {
                IWDG->KR = IWDG_KEY_RELOAD;                // Reset Watchdog
                InitialCount++;
                if (InitialCount > 2000000) {
                    return 1;
                }
            }
            Data = (uint32_t)USART2->RDR & 0xFFu;
        }

        if ((State > 0) && (State < 5)) {
            *(uint8_t *)(&CRC->DR) = Data;
        }

        if (State == 0) {
            if (Data == 0xAA) {
                SyncCount++;
            } else {
                if ((Data == 0x00) && (SyncCount >= 3)) {
                    CRC->CR = (1u << 3) | (1u << 0);          // 16-bit poly, reset - Initialize CRC Generator
                    State++;  // Must have received at least 3 of the 5 0xAA's
                }
                SyncCount = 0;
            }
        } else if (State == 1) {
            SerialStruct->Command = Data;                              // Command
            State++;
        } else if (State == 2) {
            CmdLen = Data;                              // LSB
            State++;
        } else if (State == 3) {
            CmdLen += Data << 8;              // MSB
            State++;
            Offset = 0;
        } else if (State == 4) {
            SerialBuffer.ByByte[Offset++] = (uint8_t)Data;
            CmdLen--;
            if (CmdLen == 0) {
                State++;
            }
        } else if (State == 5) {
            CmdCrc = Data;                          // LSB
            State++;
        } else if (State == 6) {
            State = 0;
            CmdCrc += Data << 8;          // MSB
            if (CmdCrc == (CRC->DR & 0xFFFF)) {
                return 0;
            }
        } else {
            State = 0;
        }
    }
}

// ------------------------------------------------------------------------
//
// MoveMem - Copies data from a source to a destination
//
// Params:    *dst - Pointer to destination starting address to store data.
//            *src - Pointer to source starting address to send data from.
//            len - Length of data to be sent.
//
// Returns:   None
//
// Requirement(s):
//
void MoveMem (uint8_t *dst, uint8_t *src, uint32_t len) {
    while (len--) {
        *dst++ = *src++;
    }
}

// ------------------------------------------------------------------------
//
// reflash - Function that stores the parameter data items into FLASH and communicated with the host
//           reprogramming to unit that flashing has been complete
//
// Params:    SerialStruct - Data structure containing the configuration of the serial mode used
//            Crc - Crc of firmware
//            Version - Firmware version number
//
// Returns:   None
//
// Requirement(s):
//
void reflash(stSerial *SerialStruct, uint16_t Crc, uint16_t Version) {

    uint16_t  PageNumber;
    int32_t   ResponseCode;
    uint8_t   i, FirmwarePageNumber;

    DisableInterrupts();
    IWDG->KR = IWDG_KEY_ENABLE;
    IWDG->KR = IWDG_KEY_WRITE_ACCESS_ENABLE;
    IWDG->PR = IWDG_PRESCALER_256;
    IWDG->RLR = (2000 * 10) / 64;
    do {
    } while (IWDG->SR != RESET);
    IWDG->WINR = 0xFFF;           // Disable Windowing
    //
    // Unlock FLASH
    //
    if ((FLASH->CR & FLASH_CR_LOCK) != 0) {     // Macro
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
    }
    //
    // Clear Error Flags
    //
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR); // Macro - Clear all FLASH flags
    for (i = 0; i < 56; i++) {

        //
        // Loop Here sending request for Firmware until we get a response with the correct page number
        //
        while (1) {
            IWDG->KR = IWDG_KEY_RELOAD;                // Reset Watchdog
            //
            // Send command to Host requesting a Page of Data
            //
            SerialBuffer.ByByte[9] = i;
            SerialStruct->Command = 0xEE;
            SerialStruct->Length = 1;
            ReflashSerialSend(SerialStruct);
            ResponseCode = GetSerialMessage(SerialStruct);
            FirmwarePageNumber = SerialBuffer.ByByte[0];
            if ((ResponseCode == 0) && (i == FirmwarePageNumber)) {
                PageNumber = i;
                //
                // As we are executing from Page 55, Page 55 can not be reprogrammed, so move page 55 code to page 56.
                // In initsystem, there is code that will check if page 56 should be moved back to page 55 on the next power-up
                //
                if (PageNumber == 55) {
                    PageNumber = 56;
                }
                Flash_Program(0x08000000u + (PageNumber * FLASH_PAGE_SIZE), (uint16_t *)&SerialBuffer.ByByte[4]);
                break;
            }
        }
    }
    //
    // Send command to Host that we are done
    //
    SerialBuffer.ByByte[9] = 99;
    SerialStruct->Command = 0xEE;
    SerialStruct->Length = 3;
    ReflashSerialSend(SerialStruct);

    MoveMem((uint8_t *)&SerialBuffer.ByByte[0], (uint8_t *)(0x08000000u + (63 * FLASH_PAGE_SIZE)), 2048);
    FactoryConfiguration = (stFactoryConfiguration *)&SerialBuffer.ByByte[0];
    FactoryConfiguration->FirmwareVersion = Version;
    FactoryConfiguration->FirmwareCrc16 = Crc;
    FactoryConfiguration->Crc16 = CalcCrc16Array((uint8_t *)FactoryConfiguration, 2046);
    Flash_Program(0x08000000u + (63 * FLASH_PAGE_SIZE), (uint16_t *)&SerialBuffer.ByByte[0]);

    while (1) {
        IWDG->KR = IWDG_KEY_RELOAD;                // Reset Watchdog
    }
}

// ------------------------------------------------------------------------
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void Flash_Program(uint32_t Address, uint16_t *WordPtr) {

    WaitForLastOperation();
    FLASH->CR |= FLASH_CR_PER;           // Erase current page
    FLASH->AR = Address;
    FLASH->CR |= FLASH_CR_STRT;
    WaitForLastOperation(); /* Wait for last operation to be completed */
    FLASH->CR &= ~FLASH_CR_PER;
    WaitForLastOperation(); /* Wait for last operation to be completed */
    for (uint32_t Index = 0; Index < (2048 / 2); Index++) {
        FLASH->CR |= FLASH_CR_PG; /* Proceed to program the new data */
        *(volatile uint16_t *)Address = WordPtr[Index]; /* Write data in the address */
        Address += 2;
        WaitForLastOperation(); /* Wait for last operation to be completed */
        FLASH->CR &= ~FLASH_CR_PG; /* If the program operation is completed, disable the PG Bit */
        }
}
