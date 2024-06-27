/**
 ******************************************************************************
 * File Name:       global.h
 *
 * Description:     Global Data Header File
 *
 * Release History:
 *
 *  2019-01-29  RLW     Initial Release
 *
 *
 ******************************************************************************
 **/

#ifndef GLOBAL_H_
#define GLOBAL_H_

#define STM32F072xB                     1

#ifdef __cplusplus
extern "C" {
#endif

#include "inttypes.h"
#include "limits.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"
#include "stm32f0xx_hal_crc.h"
#include "stm32f0xx_hal_dma.h"
#include "stm32f0xx_hal_can.h"
#include "stm32f0xx_hal_i2c.h"
#include "stm32f0xx_hal_tim.h"
#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_uart.h"

#define BOARD_REVISION                      1               // 1=A, 2=B, 3=C, 4=D, 5=E, 6=F, 7=G

typedef size_t uint32;

#define VERSION_MAJOR                       0x02
#define VERSION_MINOR                       0x01

#define FALSE                               0
#define TRUE                                1

#define ACK                                 0x06
#define NAK                                 0x15

#define COMM_CC                 1
#define COMM_CD_DIGIO1          2
#define COMM_CD_RS485           3

#define ENABLE_POWER            (1 << 0)
#define ENABLE_PULLUP           (1 << 1)
#define ENABLE_COMMUNICATIONS   (1 << 2)
#define ENABLE_MASTER           (1 << 3)
#define ENABLE_SLAVE1           (1 << 4)
#define ENABLE_SLAVE2           (1 << 5)
#define ENABLE_SLAVE3           (1 << 6)
#define ENABLE_TEMPERATURE      (1 << 7)
#define ENABLE_RV_OUTPUT        (1 << 8)
#define ENABLE_REFRESH_CONFIG   (1 << 9)
#define ENABLE_UPDATE_CONFIG    (1 << 10)
#define ENABLE_REFRESH_PROFILE  (1 << 11)
#define ENABLE_MANUAL           (1 << 12)

#define EEPROM_MODE_NORMAL                          0
#define EEPROM_MODE_READ_CONFIG                     1
#define EEPROM_MODE_READ_PROFILE                    2
#define EEPROM_MODE_UPDATE_CONFIG                   3
#define EEPROM_MODE_UPDATE_PROFILE                  4
#define EEPROM_MODE_UPDATE_MANUAL                   5

#define SERIAL_VERIFY_MODE_NORMAL                   0
#define SERIAL_VERIFY_MODE_READ                     1


#define DIG_IN                              1
#define DIG_OUT                             2
#define DIG_FREQ_IN                         3
#define DIG_FREQ_OUT                        4
#define DIG_PWM_OUT                         5
#define DIG_SERIAL_IN                       6           // DIG_IO1 only
#define DIG_SERIAL_OUT                      7           // DIG_IO2 only
#define DIG_SERIAL_INOUT                    8           // DIG_IO1 only

#define VREFINT_CAL_ADDR                    ((uint16_t*) ((uint32_t) 0x1FFFF7BA))
#define FLASH_ADDR                          ((uint16_t*) ((uint32_t) 0x00000000))

#define RS485_TXD_Pin                        GPIO_PIN_0
#define RS485_TXD_GPIO_Port                  GPIOA
#define RS485_RXD_Pin                        GPIO_PIN_1
#define RS485_RXD_GPIO_Port                  GPIOA
#define DIG_TX_Pin                           GPIO_PIN_2
#define DIG_TX_GPIO_Port                     GPIOA
#define DIG_IO1_Pin                          GPIO_PIN_3
#define DIG_IO1_GPIO_Port                    GPIOA
#define DIR1_Pin                             GPIO_PIN_4
#define DIR1_GPIO_Port                       GPIOA
#define DIR2_Pin                             GPIO_PIN_5
#define DIR2_GPIO_Port                       GPIOA
#define DIR3_Pin                             GPIO_PIN_6
#define DIR3_GPIO_Port                       GPIOA
#define DIG_IO3_Pin                          GPIO_PIN_7
#define DIG_IO3_GPIO_Port                    GPIOA
#define DIG_IO6_Pin                          GPIO_PIN_8
#define DIG_IO6_GPIO_Port                    GPIOA
#define PC_RS485_TXD_Pin                     GPIO_PIN_9
#define PC_RS485_TXD_GPIO_Port               GPIOA
#define PC_RS485_RXD_Pin                     GPIO_PIN_10
#define PC_RS485_RXD_GPIO_Port               GPIOA
#define CANRXD_Pin                           GPIO_PIN_11
#define CANRXD_GPIO_Port                     GPIOA
#define CANTXD_Pin                           GPIO_PIN_12
#define CANTXD_GPIO_Port                     GPIOA
#define DIG_RX_Pin                           GPIO_PIN_15
#define DIG_RX_GPIO_Port                     GPIOA

#define MUX_A_Pin                            GPIO_PIN_13
#define MUX_A_GPIO_Port                      GPIOC
#define MUX_B_Pin                            GPIO_PIN_14
#define MUX_B_GPIO_Port                      GPIOC
#define MUX_C_Pin                            GPIO_PIN_15
#define MUX_C_GPIO_Port                      GPIOC

#define MUX_D_Pin                            GPIO_PIN_1
#define MUX_D_GPIO_Port                      GPIOF

#define DIR4_Pin                             GPIO_PIN_0
#define DIR4_GPIO_Port                       GPIOB
#define DIG_IO4_Pin                          GPIO_PIN_1
#define DIG_IO4_GPIO_Port                    GPIOB
#define DIR5_Pin                             GPIO_PIN_2
#define DIR5_GPIO_Port                       GPIOB
#define CAN_EN_Pin                           GPIO_PIN_3
#define CAN_EN_GPIO_Port                     GPIOB
#define PWR_EN_Pin                           GPIO_PIN_4
#define PWR_EN_GPIO_Port                     GPIOB
#define RS485_EN_Pin                         GPIO_PIN_5
#define RS485_EN_GPIO_Port                   GPIOB
#define I2C1_SCL_Pin                         GPIO_PIN_6
#define I2C1_SCL_GPIO_Port                   GPIOB
#define I2C1_SDA_Pin                         GPIO_PIN_7
#define I2C1_SDA_GPIO_Port                   GPIOB
#define DIG_IO2_Pin                          GPIO_PIN_8
#define DIG_IO2_GPIO_Port                    GPIOB
#define DIG_IO5_Pin                          GPIO_PIN_10
#define DIG_IO5_GPIO_Port                    GPIOB
#define DIR6_Pin                             GPIO_PIN_11
#define DIR6_GPIO_Port                       GPIOB
#define EXT_PULLUP_Pin                       GPIO_PIN_12
#define EXT_PULLUP_GPIO_Port                 GPIOB
#define PC_RS485_EN_Pin                      GPIO_PIN_14
#define PC_RS485_EN_GPIO_Port                GPIOB


#define NAU7802_RR      (1 << 0)                // Register Reset
#define NAU7802_PUD     (1 << 1)                // Power up Digital Circuit   (0=Power Down, 1=Power up)
#define NAU7802_PUA     (1 << 2)                // Power up Analog Circut       (0=Power Down, 1=Power up)
#define NAU7802_PUR     (1 << 3)                // Power up Ready (read only)   (0=Not ready, 1=Ready)
#define NAU7802_CS      (1 << 4)                // Cycle Start
#define NAU7802_CR      (1 << 5)                // Cycle Ready (read only)
#define NAU7802_OSCS    (1 << 6)                // System Clock Source (0=Internal, 1=External)
#define NAU7802_AVDDS   (1 << 7)                // AVDD source select (0=AVDD input, 1=Internal LDO)

#define NAU7802_CALMOD              (3 << 0)                // Calibration Mode
#define NAU7802_CALMOD_INTERNAL     (0 << 0)                //
#define NAU7802_CALMOD_REVERSED     (1 << 0)                //
#define NAU7802_CALMOD_OFFSET       (2 << 0)                //
#define NAU7802_CALMOD_GAIN         (3 << 0)                //
#define NAU7802_CALS                (1 << 2)                // Start Calibration
#define NAU7802_CAL_ERR             (1 << 3)                // Calibration Result
#define NAU7802_CRS                 (7 << 4)                // Conversion Rate Select
#define NAU7802_CRS_10SPS           (0 << 4)                //
#define NAU7802_CRS_20SPS           (1 << 4)                //
#define NAU7802_CRS_40SPS           (2 << 4)                //
#define NAU7802_CRS_80SPS           (3 << 4)                //
#define NAU7802_CRS_320SPS          (7 << 4)                //
#define NAU7802_CHS                 (1 << 7)                // Input Channel Select

typedef enum {
    RESULT_16, RESULT_32, RESULT_64
} enumMultDivideResult;

typedef struct {
    int32_t             Frequency;                          // xxxx.xx Hz
    int16_t             DutyCycle;                          // xxx.xx %
    uint8_t             Mode;
} DIG_InitTypeDef;

typedef struct {
    DIG_InitTypeDef     DIG_InitStruct;
    TIM_HandleTypeDef   *Handle;
    GPIO_TypeDef        *Dir_Port;
    GPIO_TypeDef        *Dig_IO_Port;
    uint32_t            Prescaler;
    uint32_t            Period;
    uint32_t            Pulse;
    uint32_t            CaptureValue;
    uint32_t            CaptureAccum;
    uint32_t            CaptureAccumCounter;
    uint32_t            Accum;
    uint32_t            AccumCounter;
    int32_t             CaptureFrequency;           // xxxxx.xx Hz
    uint16_t            Timer7Counts;
    uint16_t            Timer6Counts;
    int16_t             Dir_Pin;
    int16_t             Dig_IO_Pin;
    int16_t             TIM_Channel;
    int16_t             bTimerUsed;
    uint16_t            CapturePeriodCounter;
    uint16_t            FreqInputTicks;
} DIG_HandleTypeDef;

typedef struct {
    I2C_HandleTypeDef   *hi2c;
    uint8_t             *Buffer;
    uint16_t            ErrorCounter;
    uint16_t            Address;
    uint16_t            Length;
    uint8_t             bTxDone, bRxDone;
    uint8_t             Mode, State;
} stI2CControl;

typedef struct {                            // This Structure must be 2048 bytes in length
    int32_t Reserved[511];
    uint32_t Crc16;
} stFlashConfiguration;

typedef struct {                // This Structure must be 2048 bytes in length
    int32_t AdcZero[16];
    int32_t AdcSpan[16];
    int32_t Reserved[479];
    uint32_t Crc16;
} stFactoryConfiguration;

typedef struct {
  uint8_t		InstanceNumber;			// 0 = USART1,  1 = USART2
  uint8_t       ReceiveMode;            // 0 = Save Data into Buffer, 1=Decode incoming Data
  uint8_t       Transmitting;           // 0=not transmitting, 1=transmitting (used to ignore serial input when in half-duplex mode)
  uint8_t 		State;
  uint8_t		SyncCount;
  uint16_t 		CmdLen;
  uint16_t		Crc16;
  uint16_t		Offset;
  uint16_t		CmdCrc;
  uint16_t      TxRxBufferSize;
  uint16_t      TxRxBufferCount;
  uint8_t       *TxRxBuffer;
  uint8_t		RxChar;
  uint16_t      RxTimeout;
  uint8_t 		Command;
  int16_t 		Length;
} stSerial;

//typedef struct {
//    union {
//        uint16_t    byWord[1030];
//        uint8_t     byByte[2060];
//    };
//} stSerialBuffer;

typedef struct {
    int32_t FirmwareVersion;           //
    int32_t AdcCounts[16];             //
    int32_t DigIO_InputFrequency[6];    // xxxx.xx
    int32_t RV_Ohms;                    // xxx.xx ohms
    int32_t TestData[16];
    int16_t AdcVolts[16];              // x.xxx V
    int16_t PS_Current;                // xx.xx mA
    int8_t  DigIO_InputLevel;           // Bit 0=DIGIO1, Bit 1=DIGIO2, ... Bit 5=DIGIO6
    int8_t  OverCurrentFlag;            // 0=Current OK, 1=Overcurrent
} stSysVars;

typedef struct {
    int32_t DUT_UpdateCounter;
    int32_t DUT_Status;
    int32_t DUT_Master_Period;
    int32_t DUT_KMA_Period;
    int32_t DUT_Slave_Period[4];
    int32_t DUT_Master_Index;
    int32_t DUT_KMA_Counts;
    int32_t DUT_RV_Volts_Counts;
    int32_t DUT_RV_Amps_Counts;
    int32_t DUT_AD7814_Counts;
    int32_t DUT_Setpoint;
    int32_t DUT_Feedback;
    int32_t DUT_PWM_Counts;
    int32_t DUT_FW_Version;
    int32_t DUT_EEPROM_Config;
    int32_t DUT_EEPROM_Address;
    int32_t EEPROM_Mode;
} stSysVarsCC;

typedef struct {
    uint16_t    OhmsKFactor;            // Add 0
    uint16_t    VoltageKFactor;         // Add 1
    uint16_t    EEPROM_Config;          // Add 2
    uint16_t    Trippoint;              // Add 3
    uint16_t    KmaSamples;             // Add 4
    uint16_t    HoldValue;              // Add 5
    uint16_t    Reserved[12];           // Add 6-17
    uint16_t    Profile[510];           // Add 18-527
} stEEPROMData;

typedef struct {
    int32_t DigIO_FrequencySetpoint[6];
    int32_t DigIO_DutyCycle[6];
    int32_t NewConfigData[6];
    int32_t EnableBits;
    int32_t CommConfig;
    int32_t ManualSetpoint;
    int32_t DigIO_Configuration[6];
    int32_t DigIO_OutputLevel[6];
    int32_t LED_TestMode;
} stSysPars;

typedef struct {
    uint16_t    VerifyAddress;
    uint16_t    VerifyData;
    uint8_t     VerifyMode;
} stSerialVerifyData;

typedef struct {
    uint32_t                Frequency;         // xxxx.xx Hz
    uint32_t                DutyCycle;         // xxx.xxx %
} PWM_InitTypeDef;

typedef struct {
    PWM_InitTypeDef         PWM_InitStruct;
    TIM_HandleTypeDef       *Handle;
    uint32_t                TIM_Channel;
    uint32_t                bTimerUsed;
    GPIO_TypeDef            *Dig_IO_Port;
    uint32_t                Dig_IO_Pin;
    uint32_t                Prescaler;
    uint32_t                Period;
    uint32_t                Pulse;
    uint32_t                OldPulse;
} PWM_HandleTypeDef;

extern stSysVars                SysVars;
extern stSysVarsCC              SysVarsCC;
extern stSysPars                SysPars;
extern stFlashConfiguration     *FlashConfiguration;
extern stFactoryConfiguration   *FactoryConfiguration;
extern stSerial 				SerialUsart1;
extern stSerial                 SerialUsart2;
extern stSerial                 SerialUsart4;
extern uint8_t                  SerialBuffer1[2100];
extern uint8_t                  SerialBuffer2[2100];
extern uint8_t                  SerialBuffer4[2100];
extern stEEPROMData             EEPROMData;
extern stSerialVerifyData       SerialVerifyData;


extern CAN_HandleTypeDef    hcan;
extern CRC_HandleTypeDef    hcrc;
extern I2C_HandleTypeDef    hi2c1;
extern UART_HandleTypeDef   huart1;                   // DIG_IO1
extern UART_HandleTypeDef   huart2;                   // RS422/485
extern UART_HandleTypeDef   huart4;                   // RS422/485
extern TIM_HandleTypeDef    htim1;                     // DIG_IO6
extern TIM_HandleTypeDef    htim2;                     // DIG_IO5
extern TIM_HandleTypeDef    htim3;                     // DIG_IO4
extern TIM_HandleTypeDef    htim6;                     // Execution Timer - Also used for Slave Frequency Inputs
extern TIM_HandleTypeDef    htim7;                     // Execution Timer - Also used for Slave Frequency Inputs
extern TIM_HandleTypeDef    htim15;                    // DIG_IO1
extern TIM_HandleTypeDef    htim16;                    // DIG_IO2
extern TIM_HandleTypeDef    htim17;                    // DIG_IO3

extern DMA_HandleTypeDef    hdma_memtomem_dma1_channel1;
extern DMA_HandleTypeDef    hdma_memtomem_dma1_channel3;
extern DMA_HandleTypeDef    hdma_i2c1_tx;
extern DMA_HandleTypeDef    hdma_usart1_tx;
extern DMA_HandleTypeDef    hdma_usart2_tx;
extern DMA_HandleTypeDef    hdma_usart4_tx;

extern uint8_t              CDTxBuffer[256];
extern uint8_t              CDTxBufferIn, CDTxBufferOut;
extern uint8_t              SendMessageToCD;

extern uint8_t              CDRxBuffer[256];
extern uint8_t              CDRxBufferIn, CDRxBufferOut;
extern uint8_t              SendMessageToPC;

extern DIG_HandleTypeDef    DIG_HandleStruct[6];

extern int16_t              Ticks1ms, Ticks5ms, Ticks1000ms, AccelTicks;
extern uint8_t              bFactoryConfigurationValid;
extern uint8_t              bFlashConfigurationValid, bSerialVerify;
extern uint8_t              bProfileRefresh, bConfigRefresh, bConfigUpdateConfig, bConfigUpdateProfile;

extern stI2CControl         I2C1Control;

extern volatile uint16_t Dummy;
extern volatile uint32_t Dummy32;

void        MX_CAN_Init(void);
void        MX_CRC_Init(void);

void        Error_Handler(void);
void        MX_SPI1_Init(uint16_t BaudRatePrescaler);
void        InitializeDigitalIO(int8_t Mode);
int32_t     DIG_Init(int32_t Ch, DIG_InitTypeDef DIG_InitStruct);
int32_t     DIG_SetFrequency(int32_t Ch, int32_t NewFreq);
int32_t     DIG_SetDutyCycle(int32_t Ch, int32_t NewDutyCycle);
int32_t     DIG_SetPin(int32_t Ch);
int32_t     DIG_ResetPin(int32_t Ch);
int32_t     DIG_ReadPin(int32_t Ch);
void        MX_I2C1_Init(void);
void        HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void        UartClearTx(UART_HandleTypeDef *huart);
void        MX_USART1_UART_Init(void);
void        MX_USART2_UART_Init(void);
void        MX_USART4_UART_Init(void);
void        MX_GPIO_Init(void);
void        MX_DMA_Init(void);
void        CalculuateFrequencyInput(void);
void        I2C_Command(stI2CControl *I2CControl, uint8_t Mode, uint16_t Address, uint8_t *Buffer, uint16_t Length);
uint16_t    CalcCrc(uint8_t Char, uint16_t Crc1);
uint16_t    CalcCrcArray(uint8_t *Buffer, int32_t Length);
void 		ProcessRxSerial(stSerial *SerialStruct);
void        SetSerialMode(int8_t Mode);
void        ProcessSerialData(stSerial *SerialStruct);
void        ProcessTickCounters(void);
int64_t     DivideWithRounding64(int64_t Dividend, int32_t Divisor, enumMultDivideResult MultDivideResult);
int64_t     MultDivideWithRounding64(int32_t Mult1, int32_t Mult2, int32_t Divisor, enumMultDivideResult MultDivideResult);
void        MX_TIM6_Init(void);
void        MX_TIM7_Init(void);
int16_t     mysprintf(void *str, const void *fmt, ...);
void        ProcessI2C1(void);
void        I2C_SetMemoryParameters(stI2CControl *I2CControl, uint16_t Address, uint8_t *Buffer, uint16_t Length);
void        reflash(stSerial *SerialStruct);
void        ReflashTuningParameters(uint32_t PageNumber, uint16_t *DataPtr);
void        VerifyConfigurationData(void);
void        InitializeEventLog(void);
void        ClearEventLog(void);
void        SystemClock_Config(void);
void        InitializeSystem (void);
void        VerifyStack (void);
void        InitializeNAU7802 (void);
void        ProcessAdc (void);
void        ProcessFPGACommunications (void);
void        ProcessMicroCommunications(void);
void        SerialSend(stSerial *SerialStruct);
void        SendMicroCommand(stSerial *SerialStruct);

#ifdef __cplusplus
}
#endif

#endif
