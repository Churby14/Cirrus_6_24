// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: global.h
// Author  : Rick Wright/Charlie Urban
// Revision: 0.2
//
// Description:     Global Data Header File
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Revision Log:
//
//  3.2    CRU     07/26/2022 Integration of released fuel sender software and throttle SW
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
//                               Definitions
// ------------------------------------------------------------------------

#ifndef GLOBAL_H_
#define GLOBAL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STM32F0
#define STM32F0
#define STM32F072CBUx
//#define STM32F072xB Locate initial definition
#define STM32
//#define DEBUG Locate initial definition
#endif

#include "inttypes.h"
#include "limits.h"
#include "string.h"
#include "stdlib.h"
#include <hardware.h>
#include "math.h"

typedef size_t uint32;

#define BOARD_REVISION                      1               // 1=A, 2=B, 3=C, 4=D, 5=E, 6=F, 7=G
// Contains the version of the respective data structure
#define FIRMWARE_VERSION                    0x0202           // xxyy Rev 1 is fuel sender, this is Rev 2
#define FACTORY_VERSION                     0x0202          // xxyy Rev 1 is fuel sender, this is Rev 2
#define FLASH_VERSION                       0x0202          // xxyy Rev 1 is fuel sender, this is Rev 2
#define FUZZY_VERSION                       0x0101          // xxyy

#define FALSE                               0
#define TRUE                                1

#define I2C_MODE_IDLE                       0
#define I2C_MODE_WRITE_EEPROM               1
#define I2C_MODE_READ_EEPROM                2
#define I2C_MODE_READ_ACCEL                 3
#define I2C_MODE_WRITE_ACCEL                4
#define I2C_MODE_WRITE_MCP3421              5
#define I2C_MODE_READ_MCP3421               6
#define I2C_MODE_DONE                       7

#define EEPROM_PAGE_SIZE                    128
#define EEPROM_EVENT_DATA_PARTITION_SIZE    32
#define EEPROM_EVENT_DATA_OFFSET            0

#define PWM_MAX_DUTY_CYCLE                  100000
#define PWM_COUNTS                          40000

#define MIN_INDEX                           400                  // xxxx.xx
#define MAX_INDEX                           102300               // xxxx.xx

#define DIG_IN                              1
#define DIG_OUT                             2
#define DIG_FREQ_IN                         3           // !DIG_IO5
#define DIG_FREQ_OUT                        4           // !DIG_IO5
#define DIG_SERIAL_IN                       6           // DIG_IO5 only
#define DIG_SERIAL_OUT                      7           // DIG_IO2 only
#define DIG_SERIAL_INOUT                    8           // DIG_IO1 only

#define DIG_MODE_POWERUP                    0
#define DIG_MODE_NORMAL                     1

#define FLASH_ADDR                          ((uint16_t*) ((uint32_t) 0x00000000))

#define CONFIG_HARDWARE_ACCELEROMETER       (1 << 4)
#define CONFIG_HARDWARE_RV_OUTPUT           (1 << 5)        // Including MCP3421 and Analog Mux
#define CONFIG_HARDWARE_RS422_DRIVER        (1 << 6)
#define CONFIG_HARDWARE_CAN_DRIVER          (1 << 7)
#define CONFIG_HARDWARE_TEMPERATURE_SENSOR  (1 << 8)

#define CONFIG_RV_OUTPUT_MODE_DISABLED      0
#define CONFIG_RV_OUTPUT_MODE_VOLTAGE       1
#define CONFIG_RV_OUTPUT_MODE_OHMS          2

#define CONFIG_DIG_DISABLED                 0
#define CONFIG_DIG_MASTER_FREQ_OUT          1       // !IO5
#define CONFIG_DIG_TEMP_FREQ_OUT            3       // !IO5
#define CONFIG_DIG_KMA_FREQ_OUT             5       // !IO5
#define CONFIG_DIG_ERROR_CONDITION          6
#define CONFIG_DIG_SERIAL_INOUT             7       // IO1 only
#define CONFIG_DIG_TEST_IN                  10

#define AD_TEMP_Pin                          GPIO_PIN_0
#define AD_TEMP_GPIO_Port                    GPIOA
#define DIG_IO5_Pin                          GPIO_PIN_1
#define DIG_IO5_GPIO_Port                    GPIOA
#define DIG_TX_Pin                           GPIO_PIN_2
#define DIG_TX_GPIO_Port                     GPIOA
#define DIG_RX_Pin                           GPIO_PIN_3
#define DIG_RX_GPIO_Port                     GPIOA
#define DIG_IO1_Pin                          GPIO_PIN_3
#define DIG_IO1_GPIO_Port                    GPIOA
#define PWM_OUTPUT_Pin                       GPIO_PIN_4
#define PWM_OUTPUT_GPIO_Port                 GPIOA
#define AD_VIN_Pin                           GPIO_PIN_5
#define AD_VIN_GPIO_Port                     GPIOA
#define AD_KMA_Pin                           GPIO_PIN_6
#define AD_KMA_GPIO_Port                     GPIOA
#define AD_5VDC_Pin                          GPIO_PIN_7
#define AD_5VDC_GPIO_Port                    GPIOA
#define DIG_IO6_Pin                          GPIO_PIN_8
#define DIG_IO6_GPIO_Port                    GPIOA
#define RS485_TXD_Pin                        GPIO_PIN_9
#define RS485_TXD_GPIO_Port                  GPIOA
#define RS485_RXD_Pin                        GPIO_PIN_10
#define RS485_RXD_GPIO_Port                  GPIOA
#define CANRXD_Pin                           GPIO_PIN_11
#define CANRXD_GPIO_Port                     GPIOA
#define CANTXD_Pin                           GPIO_PIN_12
#define CANTXD_GPIO_Port                     GPIOA
#define MUX_SEL_Pin                          GPIO_PIN_15
#define MUX_SEL_GPIO_Port                    GPIOA

#define DIR4_Pin                             GPIO_PIN_13
#define DIR4_GPIO_Port                       GPIOC
#define EEPROM_WP_Pin                        GPIO_PIN_15
#define EEPROM_WP_GPIO_Port                  GPIOC

#define DIR3_Pin                             GPIO_PIN_0
#define DIR3_GPIO_Port                       GPIOB
#define DIR2_Pin                             GPIO_PIN_1
#define DIR2_GPIO_Port                       GPIOB
#define DIR1_Pin                             GPIO_PIN_2
#define DIR1_GPIO_Port                       GPIOB
#define DIG_IO4_Pin                          GPIO_PIN_4
#define DIG_IO4_GPIO_Port                    GPIOB
#define EXT_PULLUP_Pin                       GPIO_PIN_5
#define EXT_PULLUP_GPIO_Port                 GPIOB
#define I2C1_SCL_Pin                         GPIO_PIN_6
#define I2C1_SCL_GPIO_Port                   GPIOB
#define I2C1_SDA_Pin                         GPIO_PIN_7
#define I2C1_SDA_GPIO_Port                   GPIOB
#define DIG_IO2_Pin                          GPIO_PIN_8
#define DIG_IO2_GPIO_Port                    GPIOB
#define DIG_IO3_Pin                          GPIO_PIN_9
#define DIG_IO3_GPIO_Port                    GPIOB
#define I2C2_SCL_Pin                         GPIO_PIN_10
#define I2C2_SCL_GPIO_Port                   GPIOB
#define I2C2_SDA_Pin                         GPIO_PIN_11
#define I2C2_SDA_GPIO_Port                   GPIOB
#define DIR6_Pin                             GPIO_PIN_12
#define DIR6_GPIO_Port                       GPIOB
#define DIR5_Pin                             GPIO_PIN_13
#define DIR5_GPIO_Port                       GPIOB
#define RS485_EN_Pin                         GPIO_PIN_14
#define RS485_EN_GPIO_Port                   GPIOB
#define CAN_EN_Pin                           GPIO_PIN_15
#define CAN_EN_GPIO_Port                     GPIOB

#define GENERAL_MOSFET_POWER                    0
#define GENERAL_5VDC_LOW_LIMIT                  1
#define GENERAL_5VDC_HIGH_LIMIT                 2
#define GENERAL_LOW_INTERNAL_REFERENCE_LIMIT    3
#define GENERAL_HIGH_INTERNAL_REFERENCE_LIMIT   4
#define GENERAL_POWER_INPUT_LOW_LIMIT           5
#define GENERAL_POWER_INPUT_HIGH_LIMIT          6
#define GENERAL_KMA_LOW_LIMIT                   7
#define GENERAL_KMA_HIGH_LIMIT                  8
#define GENERAL_TEMPERATURE_LOW_LIMIT           9
#define GENERAL_TEMPERATURE_HIGH_LIMIT          10
#define GENERAL_SP_FB_DIFF                      11
#define GENERAL_CAN_SETPOINT_A_LOW_LIMIT        12
#define GENERAL_CAN_SETPOINT_A_HIGH_LIMIT       13
#define GENERAL_CAN_SETPOINT_B_LOW_LIMIT        14
#define GENERAL_CAN_SETPOINT_B_HIGH_LIMIT       15
#define GENERAL_RS485_NOT_UPDATING              16
#define GENERAL_SENSOR_MISMATCH                 17
#define GENERAL_CAN_RECEPTION_ERROR             18
#define GENERAL_CAN_TRANSMISSION_EEROR          19
#define GENERAL_CAN_ECHO_ERROR                  20
#define GENERAL_CAN_CRC_ERROR                   21
#define GENERAL_BAD_FIRMWARE_CRC                22
#define GENERAL_BAD_CONFIGURATION_CRC           23
#define GENERAL_BAD_FACTORY_CRC                 24
#define GENERAL_BAD_FUZZY_CRC                   25
#define GENERAL_SENSOR_A_QUALITY                26
#define GENERAL_SENSOR_B_QUALITY                27

#define EXECUTION_MAIN_LOOP                         0
#define EXECUTION_PROCESS_ADC_FILTER                1
#define EXECUTION_PROCESS_ANALOG_READINGS           2
#define EXECUTION_PROCESS_TICK_COUNTERS             3
#define EXECUTION_PROCESS_SIGNAL_STATUS             4
#define EXECUTION_PROCESS_MASTER_INDEX              5
#define EXECUTION_PROCESS_MCP3421                   7
#define EXECUTION_PROCESS_FREQUENCY_OUTPUTS         8
#define EXECUTION_PROCESS_MASTER_FREQUENCY_OUTPUT   9
#define EXECUTION_PROCESS_SERIAL_MESSAGE            11
#define EXECUTION_PROCESS_SERIAL_DATA               12
#define EXECUTION_PROCESS_ERROR_CONDITION           13
#define EXECUTION_PROCESS_I2C1                      15
#define EXECUTION_PROCESS_I2C2                      16
#define EXECUTION_PROCESS_RX_SERIAL                 17
#define EXECUTION_PROCESS_ADC                       18
#define EXECUTION_PROCESS_ACCELEROMETER             19

#define EXECUTION_PROCESS_TEST_INPUT                20
#define EXECUTION_PROCESS_PWM_OUT                   21
#define EXECUTION_PROCESS_CLEAR_LOG                 22
#define EXECUTION_PROCESS_MESSAGE_QUEUE             23
#define EXECUTION_PROCESS_THROTTLE                  25
#define EXECUTION_PROCESS_CAN                       26

#define EXECUTION_PROCESS_COUNT                     29            // Increase as needed

#define MISC_STATUS_LOW_ALARM_ACTIVE_BIT            0
#define MISC_STATUS_LOW_ALARM_HIGH_BIT              1
#define MISC_STATUS_ERROR_BIT                       2

#define MISC_STATUS_LOW_ALARM_MASK                  ((1 << MISC_STATUS_LOW_ALARM_ACTIVE_BIT) | (1 << MISC_STATUS_LOW_ALARM_HIGH_BIT))

#define EVENT_MESSAGE_ID_0                          0           // "Event Log Cleared"
#define EVENT_MESSAGE_ID_1                          1           // "-->Power-up<--"
#define EVENT_MESSAGE_ID_2                          2           // "Watchdog TImeout Reset"
#define EVENT_MESSAGE_ID_4                          4           // "New Flash Configuration Data"
#define EVENT_MESSAGE_ID_5                          5           // "New Factory Configuration Data"
#define EVENT_MESSAGE_ID_6                          6           // "New Fuzzy Data"
#define EVENT_MESSAGE_ID_7                          7           // "MOSFET Power Limit Exceeded"            This is first General error (GENERAL_MOSFET_POWER)
#define EVENT_MESSAGE_ID_8                          8           // "+5VDC Low"
#define EVENT_MESSAGE_ID_9                          9           // "+5VDC High"
#define EVENT_MESSAGE_ID_10                         10          // "Internal Reference Low"
#define EVENT_MESSAGE_ID_11                         11          // "Internal Reference High"
#define EVENT_MESSAGE_ID_12                         12          // "Power Input Low"
#define EVENT_MESSAGE_ID_13                         13          // "Power Input High"
#define EVENT_MESSAGE_ID_14                         14          // "KMA Percentage Low"
#define EVENT_MESSAGE_ID_15                         15          // "KMA Percentage High"
#define EVENT_MESSAGE_ID_16                         16          // "Temperature Low"
#define EVENT_MESSAGE_ID_17                         17          // "Temperature High"
#define EVENT_MESSAGE_ID_18                         18          // "SP/FB Delta Exceeded"
#define EVENT_MESSAGE_ID_19                         19          // "CAN Setpoint A Low"
#define EVENT_MESSAGE_ID_20                         20          // "CAN Setpoint A High"
#define EVENT_MESSAGE_ID_21                         21          // "CAN Setpoint B Low"
#define EVENT_MESSAGE_ID_22                         22          // "CAN Setpoint B High"
#define EVENT_MESSAGE_ID_23                         23          // "RS485 Not Updating "
#define EVENT_MESSAGE_ID_24                         24          // "Setpoint A and Setpoint B Do Not Match"
#define EVENT_MESSAGE_ID_25                         25          // "CAN Reception Error"
#define EVENT_MESSAGE_ID_26                         26          // "CAN Transmission Error"
#define EVENT_MESSAGE_ID_27                         27          // "CAN Echo Error"
#define EVENT_MESSAGE_ID_28                         28          // "CAN CRC Error"
#define EVENT_MESSAGE_ID_29                         29          // "Firmware Upload"
#define EVENT_MESSAGE_ID_30                         30          // "FATAL: Stack Overflow imminent!! - Resetting CPU!!"
#define EVENT_MESSAGE_ID_31                         31          // Bad Fuzzy Logic CRC"
#define EVENT_MESSAGE_ID_32                         32          // "Bad Firmware CRC"
#define EVENT_MESSAGE_ID_33                         33          // "Bad Configuration CRC"
#define EVENT_MESSAGE_ID_34                         34          // "Bad Factory CRC"
#define EVENT_MESSAGE_ID_35                         35          // "Bad Pitch CRC"
#define EVENT_MESSAGE_ID_36                         36          // "Serial Master Error"                This is last General Error (GENERAL_SERIAL_MASTER_ERROR)
#define EVENT_MESSAGE_ID_37                         37          // "Sensor A Quality Error"
#define EVENT_MESSAGE_ID_38                         38          // "Sensor b Quality Error"


//
//  Leave Gap for future General Errors to be added
//
#define EVENT_MESSAGE_ID_100                        100          // "Firmware Upload"
#define EVENT_MESSAGE_ID_101                        101          // "FATAL: Stack Overflow imminent!! - Resetting CPU!!"
#define EVENT_MESSAGE_ID_102                        102          // Error_Handler

// Code files Tracking values
#define FILE_ACCEL                        1
#define FILE_ADC                          2
#define FILE_ANALOG                       3
#define FILE_CAN                          4
#define FILE_CRC8                         5
#define FILE_CRC16                        6
#define FILE_DIG                          7
#define FILE_DMA                          8
#define FILE_ERROR_HANDLER                9
#define FILE_ERRORCONDITION               10
#define FILE_EXECUTION                    11
#define FILE_FREQIN                       12
#define FILE_FREQOUT                      13
#define FILE_FUZZY                        14
#define FILE_GPIO                         15
#define FILE_HARDWARE                     16
#define FILE_I2C                          17
#define FILE_INDEXES                      18
#define FILE_INITSYSTEM                   19
#define FILE_INTERRUPTS                   20
#define FILE_LOGEVENT                     21
#define FILE_MAIN                         22
#define FILE_MATH64                       23
#define FILE_MCP3421                      24
#define FILE_PWM                          25
#define FILE_REFLASH_PAGE                 26
#define FILE_REFLASH                      27
#define FILE_SERIAL                       28
#define FILE_TESTINPUT                    39
#define FILE_TICKS                        30
#define FILE_TIMER                        31
#define FILE_USART                        32
#define FILE_VERIFY                       33
#define FILE_WATCHDOG                     33

#define CAN_TX_0                                    0     // CAN Bus Message Modes
#define CAN_TX_1                                    1
#define CAN_TX_2                                    2
#define CAN_TX_3                                    3
#define CAN_TX_4                                    4
#define CAN_TX_5                                    5

#define CAN_TX_MODES                                        6

#define FUZZY_RESOLUTION                            101

#define UNIQUE_ID_BASE 0x1FFFF7AC // Base address of the 96 bit unique ID



//
// ------------------------------------------------------------------------
//                              Data Struct TypeDef
// ------------------------------------------------------------------------

// Fuzzy logic member functions and shape structures
typedef int32_t(*_FUZZY_MF)(int32_t, int32_t*);
typedef int32_t(*_FUZZY_ARR_OP)(int32_t, int32_t);
typedef int32_t(*_FUZZY_ARR)(int32_t*, int8_t, _FUZZY_ARR_OP);

typedef enum {
  RESULT_U8, RESULT_16, RESULT_32, RESULT_64
} enumMultDivideResult;


typedef struct {
  int32_t             Data[5];
} stSlaveData;

typedef struct {
    uint16_t        Firmware;
    uint16_t        FlashConfiguration;
    uint16_t        FactoryConfiguration;
    uint16_t        FuzzyConfiguration;
} stCalculatedCrc;

typedef struct {
  uint32_t            AverageTime;                   // xxxx.xxx uS
  uint32_t            CallsPerSecond;
  uint32_t            MinTime;                      // xxxx.xx uS
  uint32_t            MaxTime;                      // xxxx.xx uS
} stExecutionTimeReport;

typedef struct {
  uint32_t            Accum;                             // Accumulate 16Mhz counts
  uint32_t            AccumCount;
  uint32_t            CurrentMinTime;                    // 16Mhz counts 1 = .0625uS)
  uint32_t            CurrentMaxTime;                    // 16Mhz counts 1 = .0625uS)
  uint32_t            StartTime2;
} stExecutionTimeData;

typedef struct {
  uint32_t            Frequency;                          // xxxx.xx Hz
  uint32_t            DutyCycle;                          // xxx.xx %
  uint8_t             Mode;
  uint8_t             Config;
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
  uint32_t            CaptureFrequency;           // xxxxx.xx Hz
  uint32_t            Timer2Counts;
  uint16_t            Dir_Pin;
  uint16_t            Dig_IO_Pin;
  uint16_t            TIM_Channel;
  uint16_t            bTimerUsed;
  uint16_t            CapturePeriodCounter;
  uint32_t            FreqInputTicks;
} DIG_HandleTypeDef;

typedef struct {
  uint8_t           *Buffer;
  uint16_t          Address;
  uint16_t          Length;
  uint8_t           State;
  uint8_t           Mode;
  uint8_t           Offset;
  uint8_t           MemAddSize;
  uint16_t          MemDevAddr;
} stI2CControl;

typedef struct {
  int32_t Point1;
} stOnePoint;

typedef struct {
  int32_t Point1;
  int32_t Point2;
} stTwoPoint;

typedef struct {
  int32_t    TableLength;
  stOnePoint TablePoint[32];
} stOnePointTable;

typedef struct {
  int32_t      TableLength;
  stTwoPoint   TablePoint[32];
} stTwoPointTable;

typedef struct {                            // This Structure must be 2048 bytes in length
    stTwoPointTable SetpointTable;            // Resistance/Voltage Setpoint Table    65 int32's
    stTwoPointTable MasterFreqTable;          // Master Frequency Output table        65 int32's
    uint32_t  RVOutputMode: 2;                // 0=Disabled, 1=Voltage, 2=Resistance
    uint32_t  RVOutputPullup: 1;              // 0=Disabled, 1=Enabled
    uint32_t  ErrorConditionActiveState: 1;   // 0=Active Low, 1=Active High
    uint32_t  MasterFreqMode: 1;              // 0=Freq Table, 1=Compatibility Mode

    int32_t   KmaFilteringGain;               // 1 to 16384
    int32_t   MasterIndexFilteringGain;
    int32_t   AccelXAdcFilteringGain;
    int32_t   AccelYAdcFilteringGain;
    int32_t   AccelZAdcFilteringGain;

    uint32_t  Digio1: 5;                      // 1 to 6,
    uint32_t  Digio2: 5;
    uint32_t  Digio3: 5;
    uint32_t  Digio4: 5;
    uint32_t  Digio5: 5;
    uint32_t  Digio6: 5;
    uint32_t  : 0;             // Causes next bit fields to be at beginning of next uint32_t


    uint32_t  DefaultOutput: 8;     // 0 = freq compatibility, 1 = freq table, 2 = voltage, 3 = resistive, 4 = CAN
    uint32_t  SensorMode: 1;        // 0=A or 1=B
    uint32_t  CanUse: 1;
    uint32_t  SourceAddress: 8;     // 1 byte address to determine source
    uint32_t  CanEcho: 1;           // 0=No Echo, 1=Echo
    uint32_t  CanCrcEnable: 1;      // 0 = No CRC used in CAN messages, 1 = CRCs used in CAN Messages
    uint32_t  CanPrescaler: 9;      // 3, 6, 12, 24, 30, 36, 60, 150 and 300 are allowable values for CAN prescaler to select baud rate
    uint32_t  Endianess: 1;         // 0 = little endian, 1 = big endian

    stTwoPointTable  CanTable;      // Index vs CAN output Setpoint table for CAN bus output
    stTwoPointTable  AngularOutput;  // Angular Output table        65 int32's


    uint32_t  CanAddressOut[6];
    uint32_t  CanMsgOutTime[3]; // Only messages 0-2 are recurring, therefore, 3-5 do not need times

    uint32_t  CanAddressIn[2];
    uint32_t  CanMsgInTime[2]; // Number of 10mSec between messages
    uint32_t  CanMaskIn[2]; // Mask for address filtering

    int32_t   OutputStartupDelay; //xxxx = xx.xx seconds, delay after power up to begin outputting.
    int32_t   CanSamplingDelay; //xxxx = xx.xx seconds, when TEC counter updates, time to wait before sampling again

    uint32_t  ReverseIndexKma: 1;             // 0=Normal, 1=Reversed
    uint32_t  RS485MessageMode: 4;            // 0=Disabled, 1=Mode 1, 2=Mode 2 for Throttle, 3-15=Reserved
    uint32_t  RS485MessageTime: 11;           // 1 to 1000 (0.1 to 100.0 seconds)
    uint32_t  AccelerometerScaling: 2;        // 0=+/-2G, 1=+/-4G, 2=+/-8G,

    char      ProfileName[40];
    int32_t   Reserved[215];                   // Makes the structure 2048 bytes
    uint32_t  Version:16;
    uint32_t  Crc16:16;
} stFlashConfiguration;

typedef struct {
  int32_t TableData[16][16];
} stArray16x16;

typedef struct {                // This Structure must be 2048 bytes in length
    int32_t   Calibration33;                  // x.xxx volts
    int32_t   PowerRail5KFactor;              // xxxxx
    int32_t   KmaKFactor;                     // xxxxx
    int32_t   PowerInputKFactor;              // xxxxx
    int32_t   RVOutputVoltageKFactor;         // xxxxx
    int32_t   RVOutputCurrentKFactor;         // xxxxx
    int32_t   PwmOutputKFactor;               // xxxxx
    uint32_t  HardwareVersion: 4;
    uint32_t  AccelPresent: 1;
    uint32_t  RVOutputPresent: 1;
    uint32_t  RS485DriverPresent: 1;
    uint32_t  CanDriverPresent: 1;
    uint32_t  TempSensorPresent: 1;
    uint32_t  KmaPresent: 1;
    char      DateString[16];
    char      TimeString[12];
    char      NameTester[40];
    char      ModelNumber[40];
    char      SerialNumber[40];
    char      LocationString[40]; // Data used to identify the throttle
    char      TypeString[40];
    char      ManufacturerString[40];
    char      HardwareIdString[40];
    char      SoftwareIdString[40];
    char      MakeString[8];
    char      ModelString[40];
    char      SerialString[40];
    char      UnitString[40];
    int32_t   PowerInputFilteringGain;
    int32_t   PowerRail5FilteringGain;
    int32_t   TemperatureFilteringGain;
    int32_t   InternalReferenceFilteringGain;
    int32_t   RVOutputVoltageFilteringGain;
    int32_t   RVOutputCurrentFilteringGain;
    uint32_t  FirmwareVersion:16;
    uint32_t  FirmwareCrc16:16;               // CRC of pages 0 through 59
    int32_t   Reserved[377];                  // Spacer to make array 2048 bytes
    uint32_t  Version:16;                     // FactoryConfiguration Version
    uint32_t  Crc16:16;                       // CRC16 of first 2046 bytes
} stFactoryConfiguration;

typedef struct {                               // note:  Structure must be 16 bytes
  int32_t   EntryNumber;
  uint32_t  Timestamp:20;
  uint32_t  EventId:12;
  uint32_t  EventValue1;
  uint32_t  EventValue2;
} stEventMessage;

typedef struct {
  uint8_t     State;
  uint8_t     SyncCount;
  uint16_t    CmdLen;
  uint16_t    Crc16;
  uint16_t    Offset;
  uint16_t    CmdCrc;
  uint8_t     DmaRingBuffer[256];
  uint16_t    OldDmaLength;
  uint8_t     RingBufferIn;
  uint8_t     RightBufferOut;
  uint8_t     RxChar;
  uint16_t    RxMessageTimeout;
  uint16_t    RxGapTimeout;
  uint8_t     Command;
  uint16_t    Length;
  uint8_t     Data[530];
} stSerial;

typedef struct {
  union {
    uint32_t    ByDWord[515];
    uint16_t    ByWord[1030];
    uint8_t     ByByte[2060];
  };
} stSerialBuffer;

typedef struct {
  int32_t  NumberOfEntries;
  uint16_t NextEntry;                  // 0 to 2047
} stEventData;

typedef struct {                        // NOTE:  To prevent packing issues, make all data fields 32-bits
  uint32_t  Version;

  int32_t   TemperatureCounts;            // xxxx counts        0 to 4095
  int32_t   FilteredTemperatureCounts;    // xxxx.xx counts     0 to 409500
  int32_t   TemperatureVoltage;           // x.xxxx V           0.0000 to 3.0000
  int32_t   TemperatureCelsius;           // xxx.xx C           -55.00 to 120.00
  int32_t   TemperatureFrequency;         // xxxx.xx Hz         490.00 to 29084.00

  int32_t   KmaCounts;                    // xxxx counts        0 to 4095
  int32_t   FilteredKmaCounts;            // xxxx.xx counts     0 to 409500
  int32_t   KmaVoltage;                   // x.xxxx V           0.0000 to 5.0000
  int32_t   KmaPercentage;                // xxx.xxx %          0 to 100.000
  int32_t   KmaAngle;                     // xxx.xx Deg         0 to 180.00 (Should be)   Possible:  -7.8 to 187.83
  int32_t   KmaIndex;                     // xxxx.xx            4.00 to 1023.00
  int32_t   KmaFrequency;                 // xxxxx.xx Hz        64.06 to 16384.00

  int32_t   PowerRail5Counts;             // xxxx counts        0 to 4095
  int32_t   FilteredPowerRail5Counts;     // xxxx.xx counts     0 to 409500
  int32_t   PowerRail5VDC;                // x.xxx V            0.000 to 5.500

  int32_t   PowerInputCounts;             // xxxx counts        0 to 4095
  int32_t   FilteredPowerInputCounts;     // xxxx.xx counts     0 to 409500
  int32_t   PowerInputVoltage;            // xx.xxx V           0.000 to 47.850 V

  int32_t   IntRefCounts;                 // xxxx counts        1525 typical
  int32_t   FilteredIntRefCounts;         // xxxx.xx counts     1525.00 typical
  int32_t   PowerRail33VDC;               // x.xxx V            3.250 to 3.400
  int32_t   FactoryIntRefCounts;          // xxxx counts

  int32_t   AccelXCounts;                 // xxxx counts        0 to 4095
  int32_t   FilteredAccelXCounts;         // xxxx.xx counts     0 to 409500
  int32_t   AccelGravityX;                // x.xxxx g

  int32_t   AccelYCounts;                 // xxxx counts        0 to 4095
  int32_t   FilteredAccelYCounts;         // xxxx.xx counts     0 to 409500
  int32_t   AccelGravityY;                // x.xxxx g

  int32_t   AccelZCounts;                 // xxxx counts        0 to 4095
  int32_t   FilteredAccelZCounts;         // xxxx.xx counts     0 to 409500
  int32_t   AccelGravityZ;                // x.xxxx g

  int32_t   RVOutputVoltageCounts;          // xxxxx counts     0 to 16383
  int32_t   FilteredRVOutputVoltageCounts;  // xxxxx.xx counts  0 to 1638300
  int32_t   RVOutputVoltage;                // xx.xxxx V        0.0000 to 10.0800

  int32_t   RVOutputCurrentCounts;          // xxxxx counts     0 to 16383
  int32_t   FilteredRVOutputCurrentCounts;  // xxxxx.xx counts  0 to 1638300
  int32_t   RVOutputCurrent;                // xxx.xxx mA       0.000 to 160.000

  int32_t   MasterIndex;                // xxxx.xx                 4.00 to 5115.00
  int32_t   FilteredMasterIndex;        // xxxx.xx                 4.00 to 5115.00
  int32_t   MasterFrequency;            // xxxxx.xx Hz        12.81 to 16384.00
  //
  int32_t   RVOutputOhms;               // xxxx.xx Ohms       0.0 to 9999.99
  int32_t   SetpointIndex;              // xxxx.xx               4.00 to 1023.00
  int32_t   NumberOfEventLogEntries;
  int32_t   RVSetpoint;                   // xx.xxx V or xxx.xxx Ohms
  int32_t   RVFeedback;                   // xx.xxx V or xxx.xxx Ohms
  int32_t   PwmDutyCycle;               // xxx.xxx %                0.000 to 100.000
  int32_t   PwmDutyCycleFineTune;       // xxx.xxx %
  int32_t   MosfetPower;                // xx.xxx W
  uint32_t  ErrorDetected: 1;
  uint32_t  RVOutputMode: 2;
  int32_t   Slack;                      // Number of 32-bit words from end of BSS to last used stack location
  int32_t   StackUsed;                  // Number of 32-bit words used by stack
  int32_t   TestInput;
  int32_t   CurrentSensePower;          // xx.xxx W

  int32_t   CanSetpoint;

  uint32_t  SystemTicks;
  uint32_t  Timer2Counts;

  uint32_t SensorLetter: 1;
  uint32_t ControlMode: 5;
  uint32_t ErrorType: 3;
  uint32_t MatchStatus: 3;
  // 0 = no error, 1 = SysVars.ErrorDetected, 2 = Setpoint A error,
  // 4 = Setpoint B error, 8 = RS485 error, 16 = Setpoint A and B mismatch
  // 32 = CAN not Updating, 64 = Sensor had lower quality score
  uint32_t ErrorStatus: 8;
  uint32_t Space1: 12;

  int32_t  ThrottleSetpoint;

  uint32_t CriticalFault: 16;
  uint32_t MajorFault: 16;
  uint32_t : 0;

  uint32_t MinorFault: 8;
  uint32_t RS485Counter: 8; // Increments every time an RS485 message is received - max value of 15
  uint32_t Space3: 16;

  int32_t  Noise;

  uint32_t SensorAngle: 8;
  uint32_t ThrottleState: 8;
  uint32_t bStartupComplete: 1;
  uint32_t Space4: 15;

  int32_t   Quality1; // Quality value of the sensor for fuzzy 1 (0 = 0% (bad) 10000 = 100.00% (great))
  int32_t   Quality2; // Quality value of sensor for fuzzy 2  (0 = 0% (bad) 10000 = 100.00% (great))

  int32_t   Reserved[14];               // Reserved for future System Variables
} stSysVars;

typedef struct {
  uint16_t               GeneralErrorTimer[56];         // These timers are incremented every 10mS.
  uint16_t               bErrorPresent[56];
} stSignalStatus;

typedef struct {
  int64_t               RunningAverage;
  int32_t               FilterOutput;
} stFilterData;

typedef struct {
  uint32_t              bManualPowerInputEnable: 1;
  uint32_t              bManualKmaEnable: 1;
  uint32_t              bManualTemperatureEnable: 1;
  uint32_t              bManualRVOutputVoltageEnable: 1;
  uint32_t              bManualRVOutputCurrentEnable: 1;
  uint32_t              bManualAccelXEnable: 1;
  uint32_t              bManualAccelYEnable: 1;
  uint32_t              bManualAccelZEnable: 1;
  uint32_t              bManualIntRefEnable: 1;
  uint32_t              bManual5VDCEnable: 1;
  uint32_t              bManualSetpointEnable: 1;
  int32_t               ManualPowerInput;
  int32_t               ManualKma;
  int32_t               ManualTemperature;
  int32_t               ManualRVOutputVoltage;
  int32_t               ManualRVOutputCurrent;
  int32_t               ManualAccelX;
  int32_t               ManualAccelY;
  int32_t               ManualAccelZ;
  int32_t               ManualIntRefEnable;
  int32_t               Manual5VDCInput;
  int32_t               ManualSetpoint;
} stHostData;

typedef struct {
  uint32_t              Frequency;         // xxxx.xx Hz
  int32_t               DutyCycle;         // xxx.xxx %
} PWM_InitTypeDef;

typedef struct {
  PWM_InitTypeDef       PWM_InitStruct;
  TIM_HandleTypeDef     *Handle;
  uint32_t              TIM_Channel;
  uint32_t              Prescaler;
  uint32_t              Period;
  uint32_t              Pulse;
  uint32_t              OldPulse;
} PWM_HandleTypeDef;

typedef struct {
  uint32_t Address;
  uint8_t  Data[8];
} stCanInbox;

typedef struct {
  uint8_t   Outbox[8]; // Transmit outbox
  uint8_t   LastMsg[8]; // Used to validate echo
  stCanInbox  Inbox[2];
  uint8_t   InboxLength[2]; //Corresponds with the length of message in Inbox 1 and 2
  uint32_t  AddressOut[6]; // Bit 0 = TXRQ, Bit 1 = RTR, Bit 2 = IDE and 3-31 ExtID (21-31 are StID as well)
  uint32_t  AddressIn[2]; // Bits 3-31 are ID of incomming messages
  uint32_t  MaskIn[2]; // Mask for filtering incomming messages
  uint8_t   Mode; // 1 = Transmit Mailbox 1, 2 = Transmit Mailbox 2, 3 = Transmit Mailbox 3
  uint8_t   State;
  uint8_t   Status; // 0 = No Error, 01b = Not receiving, 10b = Not transmitting, 100b = Not echoeing, 1000b = Bad CRC
  uint32_t  Timer; // Increments every 10 mSec

  // For multi-packet info
  char      *ECUinfo[7]; // Points to the ECU identification information for AddressOut3
  char      *SWinfo[2];  // Points to the software identification for AddressOut4
  char      *Componentinfo[4]; // Points to the component indentificaiton for AddressOut5
  char      **Caninfo[3]; // double pointer to ECUinfo, SWinfo and Componentinfo
  uint8_t   InfoSize[13]; // lists the size of all spn's within the pgns
  uint8_t   size_buffer[13];
} stCanBus;

typedef struct {
  uint8_t Moving; // Tracks whether the throttle arm is moving based on sensor noise
  uint8_t Moved; // Tracks whether the throttle has moved and now settled
} stThrottleInfo;

typedef struct {
  uint8_t   Buffer[23];
  uint8_t   RSControl; // 0 while waiting to receive data, 1 if data received and ready to send
} stRS485;

typedef struct {
  int32_t  Input0;
  int32_t  Input1;
  int32_t  Input2;
} stFuzzyInputs;

typedef struct {
  int32_t  Shape; // 0 = triangle, 1 = trapezoid
  int32_t  Point0;
  int32_t  Point1;
  int32_t  Point2;
  int32_t  Point3;
} stMembershipFunction;

typedef struct {
  stFuzzyInputs  InputRules[15];
} stFuzzyInputRules;

typedef struct {
  int32_t OutputRule[15];
} stFuzzyOutputRules;

typedef struct {
  int32_t  RuleWeight[15];
  int32_t  RuleType[15];
  int32_t  RuleCount;
} stFuzzyRuleInfo;

typedef struct {
  stMembershipFunction  MF[9];
} stFuzzyInputMFs;

typedef struct {
  stMembershipFunction  MF[3];
} stFuzzyOutputMFs;

typedef struct {
  int32_t  InputCount[3];
  int32_t  OutputCount;
  int32_t  NumberOfInputs;
} stFuzzyMFCounts;

typedef struct {
  int32_t  Min[3];
  int32_t  Max[3];
} stFuzzyInputInfo;

typedef struct {
  int32_t  Min;
  int32_t  Max;
} stFuzzyOutputInfo;


typedef struct {                         // Must be 1024 bytes long (2 of these are used for a page)
  stFuzzyMFCounts      FuzzyMFCounts;    // length = 16 bytes
  stFuzzyInputMFs      FuzzyInputMFs;    // length = 180 bytes
  stFuzzyOutputMFs     FuzzyOutputMFs;   // length = 60 bytes
  stFuzzyInputRules    FuzzyInputRules;  // length = 180 bytes
  stFuzzyOutputRules   FuzzyOutputRules; // length = 60 bytes
  stFuzzyOutputInfo    FuzzyOutputInfo;  // length = 8 bytes
  stFuzzyRuleInfo      FuzzyRuleInfo;    // length = 120 bytes
  int32_t              Reserved[97];
  uint32_t             Version: 16;
  uint32_t             Crc16: 16;
} stFuzzyConfiguration;

typedef struct {
  stFuzzyConfiguration Fuzzy1;
  stFuzzyConfiguration Fuzzy2;
} stFuzzyPage;

typedef struct {
  uint8_t WaferId;
  uint32_t XYCoordniates;
  uint8_t Lot1;
  uint16_t Lot2;
  uint32_t Lot3;
} stUniqueId;

extern stCanBus                 CanBus;
extern stThrottleInfo           ThrottleInfo;
extern stRS485                  RS485;

extern stFlashConfiguration     DefaultData;
extern stFactoryConfiguration   DefaultFactory;

extern stSysVars                SysVars;
extern stSysVars                ForeignSysVars;

extern stHostData               HostData;
extern stFlashConfiguration     *FlashConfiguration;
extern stFactoryConfiguration   *FactoryConfiguration;

extern stFuzzyPage              *FuzzyPage;
extern stFuzzyConfiguration     *Fuzzy1Configuration;
extern stFuzzyConfiguration     *Fuzzy2Configuration;

extern stSerial                 SerialUsart1;       // RS485
extern stSerial                 SerialUsart2;       // Digital I/O

extern stEventData              EventData;
extern stSignalStatus           SignalStatus;
extern stExecutionTimeData      ExecutionTimeData[EXECUTION_PROCESS_COUNT];
extern stFilterData             MasterIndexFiltering;
extern stExecutionTimeReport    ExecutionTimeReport[EXECUTION_PROCESS_COUNT];

extern ADC_HandleTypeDef    hadc;
extern CAN_HandleTypeDef    hcan;
extern DIG_HandleTypeDef    DIG_HandleStruct[6];
extern PWM_HandleTypeDef    PWM_HandleStruct;
extern TIM_HandleTypeDef    htim1;                     // DIG_IO6
extern TIM_HandleTypeDef    htim3;                     // DIG_IO4
extern TIM_HandleTypeDef    htim14;                    // PWM_OUT
extern TIM_HandleTypeDef    htim15;                    // DIG_IO1
extern TIM_HandleTypeDef    htim16;                    // DIG_IO2
extern TIM_HandleTypeDef    htim17;                    // DIG_IO3
extern UART_HandleTypeDef   huart1;                    // DIG_IO1
extern UART_HandleTypeDef   huart2;                    // RS422/485
extern UART_HandleTypeDef   huart4;                    // DIG_IO5 - Receive Only
extern DMA_HandleTypeDef    hdma_adc;
extern DMA_HandleTypeDef    hdma_usart1_tx;
extern DMA_HandleTypeDef    hdma_usart1_rx;
extern DMA_HandleTypeDef    hdma_usart2_tx;
extern DMA_HandleTypeDef    hdma_usart2_rx;

extern stSerialBuffer       SerialBuffer;

extern stUniqueId           UniqueId;

extern int16_t              Ticks1ms, Ticks5ms, Ticks10ms, Ticks20ms, Ticks1000ms;
extern int8_t               SlaveCount;
extern uint8_t              bClearEventLog;
extern uint8_t              bLogDefaultProfile;
extern uint8_t              bGetDefaultProfile;
extern uint8_t              bDefaultProfileLoaded;
extern uint8_t              bRVUpdate;
extern uint8_t              bFuzzyPageValid;
extern uint8_t              bFactoryConfigurationValid;
extern uint8_t              bFuzzyConfigurationValid;
extern uint8_t              bFlashConfigurationValid;

extern uint8_t              bUseDefaultFlash;
extern uint8_t              bUseDefaultFactory;
extern uint8_t              bUseDefaultFuzzy;

extern uint8_t              bFirmwareValid;
extern stI2CControl         I2C1Control;
extern stI2CControl         I2C2Control;
extern int8_t               DIG_Mode;
extern uint32_t             uTicks;
extern stCalculatedCrc      CalculatedCrc;

extern uint32_t             _ebss;
extern uint32_t             _estack;

extern volatile uint16_t Dummy;
extern volatile uint32_t Dummy32;

extern stEventMessage       MessageQueue[16];
extern int8_t               MessageIndexIn, MessageIndexOut;

extern volatile uint8_t bSendingMessage;

void        InitializeSystem(void);
void        InitializeExecutionTimers(void);
void        InitializeMcp3421(void);
void        InitializeDigitalIO(int8_t Mode);
void        InitializePWM(void);
void        InitializeAccel(void);
void        InitializeEventLog(void);

void        ProcessPwmOutput(void);
void        ProcessSerialData(stSerial *SerialStruct);
void        ProcessAnalogReadings(void);
void        ProcessAdcFilter(void);
void        ProcessTemperatureFrequencyOutput(void);
void        ProcessMasterIndex(void);
void        ProcessMasterFrequencyOutput(void);
void        ProcessFrequencyOutputs(void);
void        ProcessTickCounters(void);
void        ProcessSignalStatus(void);
void        ProcessErrorCondition(void);
void        ProcessClearEventLog(void);
void        ProcessAccelerometer(void);
void        ProcessReadAdc(void);
void        ProcessSerialSlave(void);
void        ProcessVerifyStack(void);
void        ProcessTestInput(void);
void        ProcessEventMessageQueue(void);
void        ProcessSlaveIndexes(void);
void        ProcessI2C1(void);
void        ProcessI2C2(void);
void        ProcessRxSerial(stSerial *SerialStruct);
void        ProcessMcp3421(void);

void        MX_ADC_Init(void);
void        MX_SPI1_Init(uint16_t BaudRatePrescaler);
void        MX_CAN_Init(void);
void        MX_I2C1_Init(void);
void        MX_I2C2_Init(void);
void        MX_USART1_UART_Init(void);
void        MX_USART2_UART_Init(void);
void        MX_GPIO_Init(void);
void        MX_DMA_Init(void);
void        MX_TIM2_Init(void);

void        DIG_Init(int32_t Ch, DIG_InitTypeDef DIG_InitStruct);
void        DIG_SetFrequency(int32_t Ch, int32_t NewFreq);
void        DIG_SetPin(int32_t Ch);
void        DIG_ResetPin(int32_t Ch);
int32_t     DIG_ReadPin(int32_t Ch);

void        LogEvent(uint32_t EventID, int32_t EventValue1, int32_t EventValue2);

void        ProcessLogDefaultProfile(void);
void        ProcessDefaultProfile(void);


void        Error_Handler(int32_t FileCode, int32_t LineNumber);
uint16_t    CalcCrc16(uint8_t Val, uint16_t Crc1);
uint16_t    CalcCrc16Array(uint8_t *Buffer, int32_t Length);
int64_t     MultDivideWithRounding64(int32_t Mult1, int32_t Mult2, int32_t Divisor, enumMultDivideResult MultDivideResult);
void        ExecutionStart(int16_t Process);
void        ExecutionEnd(int16_t Process);
void        ExecutionAverage(void);
void        reflash(stSerial *SerialStruct, uint16_t Crc, uint16_t Version);
void        ReflashTuningParameters(uint32_t PageNumber, uint16_t *DataPtr);
void        IWDG_TriggerWatchdog(void);
int32_t     GetDigIo(int32_t Ch);
enumHAL_StatusTypeDef STM32_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
void        MicroDelay(uint32_t uS);
void        InitializeWatchDog(void);

uint8_t VerifyFlashData(stFlashConfiguration *NewData);

//
// Throttle routines
//
void        CAN_Tx(uint8_t);
void        CAN_Rx (CAN_HandleTypeDef *hcan, uint8_t AddressIdx);
void        CAN_Filter_Init(void);
uint32_t    PGN_to_CAN_ID(uint32_t pgn, uint8_t priority, uint8_t source);
uint32_t    CAN_ID_to_PGN(uint32_t can_id);
void        ProcessCan(void);
void        Can_TxMultipacket(uint8_t Mode);
void        CAN_MessageTx(uint8_t Mode, uint32_t id, uint8_t data_tx[8]);
uint8_t     CalcCrc8(uint8_t *data, uint8_t Length);
void        VerifyCanRx(stCanInbox CanData);

void        ThrottleStatus(void);
void        Noise(void);
int32_t     ExpAverage(int32_t avg, int32_t sample);
void        ThrottleMisMatch(void);
void        ThrottlePilotAction(int32_t old_index);
void        AngularConversion(void);
void        ProcessThrottle(void);

void        Fuzzy_Evaluate(stFuzzyConfiguration *FuzzyData);
int32_t     Fuzzy_Defuzz_Centroid(int32_t** fuzzyRuleSet, int8_t o, stFuzzyConfiguration *FuzzyData);
int32_t     Fuzzy_MF_Out(int32_t** fuzzyRuleSet, int32_t x, int8_t o, stFuzzyConfiguration *FuzzyData);

int32_t     Fuzzy_Trimf(int32_t x, int32_t* p);
int32_t     Fuzzy_Trapmf(int32_t x, int32_t* p);
int32_t     Min(int32_t a, int32_t b);
int32_t     Max(int32_t a, int32_t b);
int32_t     Fuzzy_Array_Operation(int32_t *array, int8_t size, _FUZZY_ARR_OP pfnOp);
void        Qual_1(void);
void        Qual_2(void);

#ifdef __cplusplus
#endif

#endif
