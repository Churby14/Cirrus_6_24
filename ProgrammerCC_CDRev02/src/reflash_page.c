/*
 * reflash_page.c
 *
 *  Created on: Aug 20, 2019
 *      Author: Rick
 */

#include "global.h"

#define ACK     0x06
#define NAK     0x15

#define MAX_WAIT_TIME           0xFFFFFF00

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
static void WaitForLastOperation(void) {

  while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) {
  }
  if ((FLASH->SR & FLASH_FLAG_EOP) == FLASH_FLAG_EOP) {
      FLASH->SR = FLASH_FLAG_EOP;
  }
}

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// HLR-xxxx
// LLR-xxxx
//
static void Flash_Program(uint32_t Address, uint16_t *WordPtr) {

  uint32_t index;

  WaitForLastOperation();
  FLASH->CR |= FLASH_CR_PER;
  FLASH->AR = Address;
  FLASH->CR |= FLASH_CR_STRT;
  WaitForLastOperation();
  FLASH->CR &= ~FLASH_CR_PER;
  WaitForLastOperation();
  for ( index = 0 ; index < (2048 / 2) ; index++ ) {
    FLASH->CR |= FLASH_CR_PG;
    *(__IO uint16_t*) Address = WordPtr[index];
    Address += 2;
    WaitForLastOperation();
    FLASH->CR &= ~FLASH_CR_PG;
  }
}

// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// HLR-xxxx
// LLR-xxxx
//
void ReflashTuningParameters(uint32_t PageNumber, uint16_t *DataPtr) {

  uint32_t PageAddress;
  volatile uint32_t tmpreg;

  __disable_irq();
  RCC->AHBENR |= RCC_AHBENR_CRCEN;
  tmpreg = RCC->AHBENR & RCC_AHBENR_CRCEN;
  UNUSED(tmpreg);
  if ((FLASH->CR & FLASH_CR_LOCK) != RESET ) {
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
  __enable_irq();
}
