// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: freqout.c
// Author  : Rick Wright/Charlie Urban
// Revision: 3.2
//
// Description:     Global Variables
//
// ------------------------------------------------------------------------
//
// ------------------------------------------------------------------------
//
// Revision Log:
//
//  3.2    CRU     07/26/2022 Integration of released fuel sender software and throttle SW
//
//
// ------------------------------------------------------------------------
//
// Design Details and Explanations:
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Implementation Notes:
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//                              Include Files
// ------------------------------------------------------------------------

#include "inttypes.h"
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

ADC_HandleTypeDef   hadc;
CAN_HandleTypeDef   hcan;
DIG_HandleTypeDef   DIG_HandleStruct[6];
PWM_HandleTypeDef   PWM_HandleStruct;
TIM_HandleTypeDef   htim1;                // DIG_IO6
TIM_HandleTypeDef   htim3;                // DIG_IO4
TIM_HandleTypeDef   htim14;               // PWM
TIM_HandleTypeDef   htim15;               // DIG_IO1
TIM_HandleTypeDef   htim16;               // DIG_IO2
TIM_HandleTypeDef   htim17;               // DIG_IO3
UART_HandleTypeDef  huart1;
UART_HandleTypeDef  huart2;
UART_HandleTypeDef  huart4;
DMA_HandleTypeDef   hdma_adc;
UART_HandleTypeDef  huart1;
UART_HandleTypeDef  huart2;
DMA_HandleTypeDef   hdma_usart1_tx;
DMA_HandleTypeDef   hdma_usart1_rx;
DMA_HandleTypeDef   hdma_usart2_tx;
DMA_HandleTypeDef   hdma_usart2_rx;

stCanBus                    CanBus;
stCanInbox                  CanInbox[2];
stRS485                     RS485;
stThrottleInfo              ThrottleInfo;

stFlashConfiguration     DefaultData;
stFactoryConfiguration   DefaultFactory;

stSysVars                   SysVars;
stSysVars                   ForeignSysVars;


stHostData                  HostData;
stFilterData                MasterIndexFiltering;
stFlashConfiguration        *FlashConfiguration;
stFactoryConfiguration      *FactoryConfiguration;
stFuzzyPage                 *FuzzyPage;
stFuzzyConfiguration        *Fuzzy1Configuration;
stFuzzyConfiguration        *Fuzzy2Configuration;

stSerial                    SerialUsart1;               // RS485
stSerial                    SerialUsart2;               // Digital I/O
stEventData                 EventData;
stSignalStatus              SignalStatus;
stExecutionTimeData         ExecutionTimeData[EXECUTION_PROCESS_COUNT];
stExecutionTimeReport       ExecutionTimeReport[EXECUTION_PROCESS_COUNT];

stSerialBuffer              SerialBuffer __attribute__((aligned(4)));          // 2060 bytes - dword aligned

stUniqueId                  UniqueId;

int16_t             Ticks1ms, Ticks5ms, Ticks10ms, Ticks20ms, Ticks1000ms;
int8_t              SlaveCount;                                 // Number of Slave Inputs
uint8_t             bClearEventLog;
uint8_t             bLogDefaultProfile;
uint8_t             bGetDefaultProfile;
uint8_t             bDefaultProfileLoaded;
uint8_t             bRVUpdate;
uint8_t             bFuzzyPageValid;
uint8_t             bFlashConfigurationValid;
uint8_t             bFactoryConfigurationValid;
uint8_t             bFirmwareValid;

uint8_t              bUseDefaultFlash;
uint8_t              bUseDefaultFactory;
uint8_t              bUseDefaultFuzzy;

stI2CControl        I2C1Control;
stI2CControl        I2C2Control;
int8_t              DIG_Mode;
uint32_t            uTicks;
stCalculatedCrc     CalculatedCrc;

volatile uint16_t Dummy;
volatile uint32_t Dummy32;

stEventMessage      MessageQueue[16];
int8_t              MessageIndexIn, MessageIndexOut;

volatile uint8_t bSendingMessage;

