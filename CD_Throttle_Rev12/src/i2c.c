// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: i2c.c
// Author  : Rick Wright
// Revision: C
//
// Description:     This module contains I2C routines:
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Revision Log:
//
//  A           CRU   12/21/2021    Baseline Release
//  B           RLW   06/13/2022    Changes per PR #19 in PACT
//  C           CRU   06/30/2022   Changes per PR #22 in PACT
//
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
// Design Details and Explanations:
// ------------------------------------------------------------------------
//
//  Case 0 of ProcessI2C1 and ProcessI2C2 is for detection of an error on the I2C
//  Bus.  If the STM32 is reset in the middle of an I2C transfer,  The slave device
//  can be left in the middle of a data transfer, which can result in the data line
//  being driven low by the slave device.  Then, with the data line low, the master
//  can not generate a start condition.  Case 0 places the I2C port pins into GPIO
// mode, checks of data line is low.  If low, it will pulse the clock line until the
// data line goes high.  Once high, port pins are returned to I2C mode and program
// goes to case 1.
//
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


// ------------------------------------------------------------------------
//                  Local Conditional Compilation Switches
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//                          Local Variables
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// ProcessI2C1 - This routine is called from Main Loop.
//
// At 400KHz, a character is sent/received every 44.4KHz (400/9) or every 22.5uS.
//
// Currently, Main Loop runs at 40KHz, or 25uS
//
//
// Params:  None
//
// Returns:   None
//
// Requirement(s):
//
void ProcessI2C1(void) {

    ExecutionStart(EXECUTION_PROCESS_I2C1);

    switch (I2C1Control.State) {
    case 0:
        I2C1->CR1 &= ~1;                    // Turn off I2C Interface
        //
        // Configure I2C1_SCL as an output - HIGH.  and I2C_SDA as GPIO Input
        //
        STM32_GPIO_Init(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, 0, SET);
        STM32_GPIO_Init(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, 0, RESET);
        do {
            //
            // Toggle Clock, then check if SDA - Loop if SDA low
            //
            STM32_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_RESET);
            MicroDelay(100);
            STM32_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_SET);
            MicroDelay(100);
        } while (STM32_GPIO_ReadPin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin) == 0);
        //
        // Switch GPIO back to I2C control
        //
        STM32_GPIO_Init(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF1_I2C1, 0);
        STM32_GPIO_Init(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF1_I2C1, 0);
        I2C1->CR1 |= 1;                       // Turn on I2C Interface
        I2C1Control.State++;
        break;

    case 1:
        if (I2C1Control.Mode == I2C_MODE_READ_MCP3421) {
            I2C1Control.MemDevAddr = 0xD0u;
            I2C1Control.State = 10;
        } else if (I2C1Control.Mode == I2C_MODE_WRITE_MCP3421) {
            I2C1Control.MemDevAddr = 0xD0u;
            I2C1Control.State = 20;
        } else {
        }
        break;
        //
        //------------------------------------------------------------------------------------------------------------
        //  Read
        //
    case 10:
        if ((I2C1->ISR & I2C_ISR_BUSY) == 0) {          // Wait for Busy Bit to be reset
            //
            // Send Start Condition, Set NBytes to 2, Device Address in Write Mode
            //
            I2C1->CR2 = I2C1Control.MemDevAddr | I2C1Control.Length << 16 | I2C_AUTOEND_MODE | I2C_GENERATE_START_READ;
            I2C1Control.Offset = 0;
            I2C1Control.State++;
        }
        break;
    case 11:
        if ((I2C1->ISR & I2C_FLAG_RXNE) != 0) {             // Wait for Receive Data
            I2C1Control.Buffer[I2C1Control.Offset++] = (uint8_t)I2C1->RXDR;  // Save Data
            if (--I2C1Control.Length == 0) {                // Proceed to next Case once data has been saved
                I2C1Control.State++;
            }
        }
        break;
    case 12:
        if ((I2C1->ISR & I2C_FLAG_STOPF) != 0) {            // Wait for STOPF to be set
            I2C1->ICR = I2C_FLAG_STOPF;                     /* Clear STOP Flag */
            I2C1->CR2 &= ~(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN);                  /* Clear Configuration Register 2 */
            I2C1Control.Mode = I2C_MODE_DONE;
            I2C1Control.State = 1;
        }
        break;
        //
        //------------------------------------------------------------------------------------------------------------
        //  Write
        //
    case 20:
        if ((I2C1->ISR & I2C_ISR_BUSY) == 0) {          // Wait for Busy Bit to be reset
            //
            // Send Start Condition, Set NBytes to 2, Device Address in Write Mode
            //
            I2C1->CR2 = I2C1Control.MemDevAddr | I2C1Control.Length << 16 | I2C_AUTOEND_MODE | I2C_GENERATE_START_WRITE;
            I2C1Control.Offset = 0;
            I2C1Control.State++;
        }
        break;
    case 21:
        if ((I2C1->ISR & I2C_FLAG_TXIS) != 0) {                       // Wait for Transmit Data
            I2C1->TXDR = I2C1Control.Buffer[I2C1Control.Offset++];    // Send Data
            if (--I2C1Control.Length == 0) {                // Proceed to next Case once data has been sent
                I2C1Control.State++;
            }
        }
        break;
    case 22:
        if ((I2C1->ISR & I2C_FLAG_STOPF) != 0) {            // Wait for STOPF to be set
            I2C1->ICR = I2C_FLAG_STOPF;                     /* Clear STOP Flag */
            I2C1->CR2 &= ~(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN);                  /* Clear Configuration Register 2 */
            I2C1Control.Mode = I2C_MODE_DONE;
            I2C1Control.State = 1;
        }
        break;
    default:
        Error_Handler(FILE_I2C, __LINE__);
        break;
    }
    ExecutionEnd(EXECUTION_PROCESS_I2C1);
}
// ------------------------------------------------------------------------
//
//  ProcessI2C2 - This routine is called from Main Loop and during initialization of EventLog and Accelerometer
//
// Params:  None
//
// Returns:   None
//
// Requirement(s):
//
void ProcessI2C2(void) {

    static uint32_t LastWrite, Timeout;

    ExecutionStart(EXECUTION_PROCESS_I2C2);

    if ((I2C2Control.State > 1) && (STM32_GetTick() > Timeout)) {
        I2C2Control.State = 0;
    }

    switch (I2C2Control.State) {
    case 0:
        I2C2->CR1 &= ~1;                    // Turn off I2C Interface
        //
        // Configure I2C2_SCL as an output - HIGH.  and I2C_SDA as GPIO Input
        //
        STM32_GPIO_Init(I2C2_SCL_GPIO_Port, I2C2_SCL_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, 0, SET);
        STM32_GPIO_Init(I2C2_SDA_GPIO_Port, I2C2_SDA_Pin, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, 0, RESET);
        do {
            //
            // Toggle Clock, then check SDA - Loop if SDA low
            //
            STM32_GPIO_WritePin(I2C2_SCL_GPIO_Port, I2C2_SCL_Pin, GPIO_PIN_RESET);
            MicroDelay(100);
            STM32_GPIO_WritePin(I2C2_SCL_GPIO_Port, I2C2_SCL_Pin, GPIO_PIN_SET);
            MicroDelay(100);
        } while (STM32_GPIO_ReadPin(I2C2_SDA_GPIO_Port, I2C2_SDA_Pin) == 0);
        //
        // Switch GPIO back to I2C control
        //
        STM32_GPIO_Init(I2C2_SCL_GPIO_Port, I2C2_SCL_Pin, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF1_I2C2, 0);
        STM32_GPIO_Init(I2C2_SDA_GPIO_Port, I2C2_SDA_Pin, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF1_I2C2, 0);
        I2C2->CR1 |= 1;                       // Turn on I2C Interface
        I2C2Control.State++;
        break;

    case 1:
        if (I2C2Control.Mode == I2C_MODE_READ_ACCEL) {
            I2C2Control.MemAddSize = 1;
            I2C2Control.MemDevAddr = 0x3Au;
            I2C2Control.State = 10;
        } else if (I2C2Control.Mode == I2C_MODE_WRITE_EEPROM) {
            I2C2Control.MemAddSize = 2;
            I2C2Control.MemDevAddr = 0xA0u;
            I2C2Control.State = 20;
        } else if (I2C2Control.Mode == I2C_MODE_READ_EEPROM) {
            I2C2Control.MemAddSize = 2;
            I2C2Control.MemDevAddr = 0xA0u;
            I2C2Control.State = 10;
        } else if (I2C2Control.Mode == I2C_MODE_WRITE_ACCEL) {
            I2C2Control.MemAddSize = 1;
            I2C2Control.MemDevAddr = 0x3Au;
            I2C2Control.State = 20;
        } else {
        }
        Timeout = STM32_GetTick() + 50;
        break;
        //
        //------------------------------------------------------------------------------------------------------------
        //  Read
        //
    case 10:
        if ((I2C2->ISR & I2C_ISR_BUSY) == 0) {          // Wait for Busy Bit to be reset
            //
            // Send Start Condition, Set NBytes to 2, Device Address in Write Mode
            //
            I2C2->CR2 = I2C2Control.MemDevAddr | I2C2Control.MemAddSize << 16 | I2C_SOFTEND_MODE | I2C_GENERATE_START_WRITE;
            I2C2Control.State++;
            if (I2C2Control.MemAddSize == 1) {
                I2C2Control.State++;
            }
        }
        break;
    case 11:
        if ((I2C2->ISR & I2C_FLAG_TXIS) != 0) {               // Wait for TXIS bit to be set
            I2C2->TXDR = ((uint32_t)(I2C2Control.Address >> 8));            // Send MSB of Address
            I2C2Control.State++;
        }
        break;
    case 12:
        if ((I2C2->ISR & I2C_FLAG_TXIS) != 0) {               // Wait for TXIS bit to be set
            I2C2->TXDR = ((uint32_t)(I2C2Control.Address & 0xFFu));          // Send LSB of Address
            I2C2Control.State++;
        }
        break;
    case 13:
        if ((I2C2->ISR & I2C_FLAG_TC) != 0) {               // Wait for TC bit to be set
            //
            // Send Start Condition, Set NBytes to Size, Device Address in Read Mode
            //
            I2C2->CR2 = I2C2Control.MemDevAddr | I2C2Control.Length << 16 | I2C_AUTOEND_MODE | I2C_GENERATE_START_READ;
            I2C2Control.State++;
            I2C2Control.Offset = 0;
        }
        break;
    case 14:
        if ((I2C2->ISR & I2C_FLAG_RXNE) != 0) {             // Wait for Receive Data
            I2C2Control.Buffer[I2C2Control.Offset++] = (uint8_t)I2C2->RXDR;  // Save Data
            if (--I2C2Control.Length == 0) {                // Proceed to next Case once data has been saved
                I2C2Control.State++;
            }
        }
        break;
    case 15:
        if ((I2C2->ISR & I2C_FLAG_STOPF) != 0) {            // Wait for STOPF to be set
            I2C2->ICR = I2C_FLAG_STOPF;                     /* Clear STOP Flag */
            I2C2->CR2 &= ~(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN);                  /* Clear Configuration Register 2 */
            I2C2Control.Mode = I2C_MODE_DONE;
            I2C2Control.State = 1;
        }
        break;
        //
        //------------------------------------------------------------------------------------------------------------
        //  Write
        //
case 20:
    //
    // Wait at least 10mS from last write before starting a new write to EEPROM
    //
    if (STM32_GetTick() > (LastWrite + 10)) {
        if ((I2C2->ISR & I2C_ISR_BUSY) == 0) {          // Wait for Busy Bit to be reset
            //
            // Send Start Condition, Set NBytes to 2, Device Address in Write Mode
            //
            if (I2C2Control.MemDevAddr == 0x00A0u) {           // Check if writing to EEPROM
                STM32_GPIO_WritePin(EEPROM_WP_GPIO_Port, EEPROM_WP_Pin, GPIO_PIN_RESET);    // Disable Write Protection
            }
            I2C2->CR2 = I2C2Control.MemDevAddr | I2C2Control.MemAddSize << 16 | I2C_RELOAD_MODE | I2C_GENERATE_START_WRITE;
            I2C2Control.State++;
            if (I2C2Control.MemAddSize == 1) {
                I2C2Control.State++;
            }
        }
    }
    break;
  case 21:
    if ((I2C2->ISR & I2C_FLAG_TXIS) != 0) {               // Wait for TXIS bit to be set
      I2C2->TXDR = (uint32_t)I2C2Control.Address >> 8;            // Send MSB of Address
      I2C2Control.State++;
    }
    break;
  case 22:
    if ((I2C2->ISR & I2C_FLAG_TXIS) != 0) {               // Wait for TXIS bit to be set
      I2C2->TXDR = (uint32_t)I2C2Control.Address & 0xFF;          // Send LSB of Address
      I2C2Control.State++;
    }
    break;
  case 23:
    if ((I2C2->ISR & I2C_FLAG_TCR) != 0) {               // Wait for TC bit to be set
        I2C2->CR2 = I2C2Control.MemDevAddr | I2C2Control.Length << 16 | I2C_AUTOEND_MODE | I2C_NO_STARTSTOP;
        I2C2Control.State++;
        I2C2Control.Offset = 0;
    }
    break;
  case 24:
    if ((I2C2->ISR & I2C_FLAG_TXIS) != 0) {                       // Wait for Transmit Data
        I2C2->TXDR = (uint32_t)I2C2Control.Buffer[I2C2Control.Offset++];    // Send Data
        if (--I2C2Control.Length == 0) {                // Proceed to next Case once data has been sent
            I2C2Control.State++;
        }
    }
    break;
  case 25:
    if ((I2C2->ISR & I2C_FLAG_STOPF) != 0) {            // Wait for STOPF to be set
        I2C2->ICR = I2C_FLAG_STOPF;                     /* Clear STOP Flag */
        I2C2->CR2 &= ~(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN);                  /* Clear Configuration Register 2 */
        if (I2C2Control.MemDevAddr == 0xA0u) {
            STM32_GPIO_WritePin(EEPROM_WP_GPIO_Port, EEPROM_WP_Pin, GPIO_PIN_SET);                       // Enable Write Protection
        }
        I2C2Control.Mode = I2C_MODE_DONE;
        I2C2Control.State = 1;
        LastWrite = STM32_GetTick();
    }
    break;
  default:
    Error_Handler(FILE_I2C, __LINE__);
    break;
  }

  ExecutionEnd(EXECUTION_PROCESS_I2C2);
}
