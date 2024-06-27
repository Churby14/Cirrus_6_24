//
// ******************************************************************************
//
// Description:     Global Variables
//
// Release History:
//
//  2018-10-12  RLW     Initial Release
//
// ******************************************************************************

#include "inttypes.h"
#include "global.h"

DIG_HandleTypeDef       DIG_HandleStruct[6];
CAN_HandleTypeDef       hcan;
CRC_HandleTypeDef       hcrc;
I2C_HandleTypeDef       hi2c1;
UART_HandleTypeDef      huart1;
UART_HandleTypeDef      huart2;
UART_HandleTypeDef      huart4;
TIM_HandleTypeDef       htim1;                // DIG_IO6
TIM_HandleTypeDef       htim2;                // DIG_IO5
TIM_HandleTypeDef       htim3;                // DIG_IO4
TIM_HandleTypeDef       htim6;                // Execution Timer - Also used for to measure Slave Frequency Inputs
TIM_HandleTypeDef       htim7;                // Execution Timer - Also used for to measure Slave Frequency Inputs
TIM_HandleTypeDef       htim15;               // DIG_IO1
TIM_HandleTypeDef       htim16;               // DIG_IO2
TIM_HandleTypeDef       htim17;               // DIG_IO3

DMA_HandleTypeDef       hdma_memtomem_dma1_channel1;
DMA_HandleTypeDef       hdma_memtomem_dma1_channel3;
DMA_HandleTypeDef       hdma_i2c1_tx;
DMA_HandleTypeDef       hdma_usart1_tx;
DMA_HandleTypeDef       hdma_usart2_tx;
DMA_HandleTypeDef       hdma_usart4_tx;


stSysVars                   SysVars;
stSysVarsCC                 SysVarsCC;
stSysPars                   SysPars;
stFlashConfiguration        *FlashConfiguration;
stFactoryConfiguration      *FactoryConfiguration;
stSerial 					SerialUsart1;				// RS485 to PC
stSerial                    SerialUsart2;               // TTL to Sender
stSerial                    SerialUsart4;               // RS485 to Sender
uint8_t                     SerialBuffer1[2100];
uint8_t                     SerialBuffer2[2100];
uint8_t                     SerialBuffer4[2100];

uint8_t                     CDTxBuffer[256];
uint8_t                     CDTxBufferIn, CDTxBufferOut;
uint8_t                     SendMessageToCD;

uint8_t                     CDRxBuffer[256];
uint8_t                     CDRxBufferIn, CDRxBufferOut;
uint8_t                     SendMessageToPC;

stEEPROMData                EEPROMData;
stSerialVerifyData          SerialVerifyData;

int16_t             Ticks1ms, Ticks5ms, Ticks1000ms;
int16_t             AccelTicks;
uint8_t             I2C_Mode;
uint8_t             bRVUpdate;
uint8_t             bFlashConfigurationValid;
uint8_t             bFactoryConfigurationValid, bSerialVerify;
uint8_t             bProfileRefresh, bConfigRefresh, bConfigUpdateConfig, bConfigUpdateProfile;

stI2CControl        I2C1Control;

volatile uint16_t Dummy;
volatile uint32_t Dummy32;

