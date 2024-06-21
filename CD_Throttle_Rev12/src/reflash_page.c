// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: reflash_page.c
// Author  : Rick Wright
// Revision: C
//
// Description: Routine to enabling rewritting a single page (2048 bytes)
//              of FLASH Memory
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

#define ACK     0x06
#define NAK     0x15

#define MAX_WAIT_TIME           0xFFFFFF00u
#define FLASH_PAGE_SIZE         2048

// ------------------------------------------------------------------------
//                  Local Conditional Compilation Switches
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//                          Local Variables
// ------------------------------------------------------------------------


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
static void WaitForLastOperation(void) {

    do {
    } while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY);
    if ((FLASH->SR & FLASH_FLAG_EOP) == FLASH_FLAG_EOP) {
        FLASH->SR = FLASH_FLAG_EOP;
    }
}

// ------------------------------------------------------------------------
//
// Flash_Program - This routine contains the functionality to write to FLASH memory
//
// Params:    Address - FLASH memory address being written to
//            WordPtr - Pointer to data being written to FLASH memory address
//
// Returns:   None
//
// Requirement(s):
//
static void Flash_Program(uint32_t Address, uint16_t *WordPtr) {

  uint32_t Index;

  WaitForLastOperation();
  FLASH->CR |= FLASH_CR_PER;
  FLASH->AR = Address;
  FLASH->CR |= FLASH_CR_STRT;
  WaitForLastOperation();
  FLASH->CR &= ~FLASH_CR_PER;
  WaitForLastOperation();
  for (Index = 0; Index < (2048 / 2); Index++) {
      FLASH->CR |= FLASH_CR_PG;
      *(volatile uint16_t *)Address = WordPtr[Index];
      Address += 2;
      WaitForLastOperation();
      FLASH->CR &= ~FLASH_CR_PG;
  }
}

// ------------------------------------------------------------------------
//
// ReflashTuningParameters - Writes to one of the 64 pages of FLASH
//                           up to 2048 bytes of data and writes 0xFF to
//                           any bytes of FLASH within the page not being written to
//
// Params:    PageNumber - Value 0-63 of which 2048 byte page to write to
//            DataPtr - Pointer to the data being written into the page of FLASH
//
// Returns:   None
//
// Requirement(s):
//
void ReflashTuningParameters(uint32_t PageNumber, uint16_t *DataPtr) {

  uint32_t PageAddress;
  volatile uint32_t TmpReg;

  DisableInterrupts();
  //
  // Disable Watchdog Windowing
  //
  IWDG->KR = IWDG_KEY_ENABLE;
  IWDG->KR = IWDG_KEY_WRITE_ACCESS_ENABLE;
  IWDG->PR = IWDG_PRESCALER_256;
  IWDG->RLR = (1250 * 10) / 64;
  while (IWDG->SR != RESET) {}
  IWDG->WINR = 0xFFF;           // Disable Windowing

  IWDG->KR = IWDG_KEY_RELOAD;  // Reset Watchdog

  RCC->AHBENR |= RCC_AHBENR_CRCEN;
  TmpReg = RCC->AHBENR & RCC_AHBENR_CRCEN;
  UNUSED(TmpReg);
  if ((FLASH->CR & FLASH_CR_LOCK) != RESET) {
      FLASH->KEYR = FLASH_KEY1;
      FLASH->KEYR = FLASH_KEY2;
  }
  FLASH->SR = FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR;
  PageAddress = 0x08000000 + (PageNumber * FLASH_PAGE_SIZE);
  WaitForLastOperation();
  FLASH->CR |= FLASH_CR_PER;
  FLASH->AR = PageAddress;
  FLASH->CR |= FLASH_CR_STRT;
  WaitForLastOperation();
  FLASH->CR &= ~FLASH_CR_PER;
  Flash_Program(PageAddress, DataPtr);
  WaitForLastOperation();
  FLASH->CR |= FLASH_CR_LOCK;
  //
  // Restore Watchdog settings
  //
  InitializeWatchDog();

  EnableInterrupts();
}
