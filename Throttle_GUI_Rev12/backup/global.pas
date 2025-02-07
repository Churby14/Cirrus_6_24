unit global;

{$mode objfpc}{$H+}

interface

uses
  Windows, Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, ExtCtrls,
  Buttons, profile, ComCtrls, StdCtrls;

const
  ENABLE_POWER = (1 shl 0);
  ENABLE_PULLUP = (1 shl 1);
  ENABLE_COMMUNICATIONS = (1 shl 2);
  ENABLE_MASTER = (1 shl 3);
  ENABLE_SLAVE1 = (1 shl 4);
  ENABLE_SLAVE2 = (1 shl 5);
  ENABLE_SLAVE3 = (1 shl 6);
  ENABLE_TEMPERATURE = (1 shl 7);
  ENABLE_RV_OUTPUT = (1 shl 8);
  ENABLE_REFRESH_CONFIG = (1 shl 9);
  ENABLE_UPDATE_CONFIG = (1 shl 10);
  ENABLE_REFRESH_PROFILE = (1 shl 11);
  ENABLE_MANUAL = (1 shl 12);

  COMM_CC = 1;
  COMM_CD_DIGIO1 = 2;
  COMM_CD_RS485 = 3;
  COMM_CD_RS485_DIRECT = 4;

  FIRMWARE_VERSION = $0101;
  FACTORY_VERSION = $0101;
  FLASH_VERSION = $0101;
  FUZZY_VERSION = $0101;

  DIG_MODE_IN = 1;
  DIG_MODE_OUT = 2;
  DIG_MODE_FREQ_IN = 3;
  DIG_MODE_FREQ_OUT = 4;
  DIG_MODE_PWM_OUT = 5;
  DIG_MODE_SERIAL_IN = 6;
  DIG_MODE_SERIAL_OUT = 7;
  DIG_MODE_SERIAL_INOUT = 8;

  SERIAL_VERIFY_MODE_NORMAL = 0;
  SERIAL_VERIFY_MODE_READ = 1;

  EEPROM_MODE_NORMAL = 0;
  EEPROM_MODE_READ_CONFIG = 1;
  EEPROM_MODE_READ_PROFILE = 2;
  EEPROM_MODE_UPDATE_CONFIG = 3;
  EEPROM_MODE_UPDATE_PROFILE = 4;

const
  ProcessTimingLabels: array [0..26] of string =
    ('Main Loop', 'Process ADC Filtering', 'Process Analog Readings',
    'Process Tick Counters',
    'Process Signal Status', 'Process Master Index', 'Process Slave Index',
    'Process MCP3421',
    'Process Temperature', 'Process KMA Frequency', 'Process Master Frequency',
    'Process Serial Message', 'Process Serial Data',
    'Process Error Condition', 'Calculate Frequency Inputs', 'Process I2C1',
    'Process I2C2', 'Process Rx Serial', 'Process Adc',
    'Process Accelerometer', 'Process Test Input',
    'Process PWM Output', 'Process Clear Event Log', 'Process Event Message Queue', 'Process Serial Slave',
    'Process Throttle Output', 'Process CAN');

const
  GeneralErrors: array [0..17] of string = ('MOSFET Power Limit Excessed',
    '+5VDC Low', '+5VDC High', 'Internal Reference Low', 'Internal Reference High',
    'Power Input Low',
    'Power Input High', 'KMA Percentage Low', 'KMA Percentage High',
    'Temperature Low', 'Temperature High',
    'SP/FB Delta Exceeded', 'CAN Setpoint A Low', 'CAN Setpoint A High',
    'CAN Setpoint B Low', 'CAN Setpoint B High', 'RS485 Not Updating',
    'Sensor Mismatch');

const
  TroubleShootingEngUnitsLabelArray: array [0..15] of string =
    ('DIG_IO4', 'RS485+', 'RS485-', 'CANH',
    'CANL', 'DIG_IO3', 'DIG_IO2', 'DIG_IO1', 'DIG_IO5',
    'DIG_IO6', 'RV_OUT', 'RESERVED', 'RESERVED', 'PWR_IN',
    'PWR_OUT', 'PULLUP+');

type
  stDataArray = record
    NextIndex: integer;
    Y: array [0..1999] of single;
    X: array [0..1999] of single;
  end;

type
  stExecutionTimeReport = record
    AverageTime: int32;     // xxxx.xxx uS
    CallsPerSecond: int32;
    MinTime: int32;     // xx.xx uS
    MaxTime: int32;     // xx.xx uS
  end;

type
  stPartition = record
    PartitionData: array [0..511] of byte;
  end;

type
  stOnePoint = record
    Point1: longint;
  end;

type
  stTwoPoint = record
    Point1: longint;
    Point2: longint;
  end;

type
  stOnePointTable = record
    TableLength: longint;
    TablePoint: array [0..31] of stOnePoint;
  end;

type
  stTwoPointTable = record
    TableLength: longint;
    TablePoint: array [0..31] of stTwoPoint;
  end;

type
  stFuzzyInputs = record
    Input0: int32;
    Input1: int32;
    Input2: int32;
  end;

type
  stMembershipFunction = record
    Shape: int32; // 0 = triangle, 1 = trapezoid
    Point0: int32;
    Point1: int32;
    Point2: int32;
    Point3: int32;
  end;

type
  stFuzzyInputRules = record
    InputRules: array [0..14] of stFuzzyInputs;
  end;

type
  stFuzzyOutputRules = record
    OutputRule: array [0..14] of int32;
  end;

type
  stFuzzyRuleInfo = record
    RuleWeight: array [0..14] of int32;
    RuleType: array [0..14] of int32;
    RuleCount: int32;
  end;

type
  stFuzzyInputMFs = record
    MF: array [0..8] of stMembershipFunction;
  end;

type
  stFuzzyOutputMFs = record
    MF: array [0..2] of stMembershipFunction;
  end;

type
  stFuzzyMFCounts = record
    InputCount: array [0..2] of int32;
    OutputCount: int32;
    NumberOfInputs: int32;
  end;

type
  stFuzzyInputInfo = record
    Min: array [0..2] of int32;
    Max: array [0..2] of int32;
  end;

type
  stFuzzyOutputInfo = record
    Min: int32;
    Max: int32;
  end;

type
  stArray16x16 = array [0..15, 0..15] of int32;

type
  stBF1 = bitpacked record
    RVOutputMode: 0..3;
    RVOutputPullup: 0..1;
    ErrorConditionActiveState: 0..1;
    MasterFreqMode: 0..1;
    Reserved: 0..134217727;
  end;

type
  stBF2 = bitpacked record
    DigIo1: 0..31;
    DigIo2: 0..31;
    DigIo3: 0..31;
    DigIo4: 0..31;
    DigIo5: 0..31;
    DigIo6: 0..31;
    Reserved: 0..3;
  end;

type
  stBF3 = bitpacked record
    ReverseIndexKma: 0..1;
    ReverseIndexSlave1: 0..1;
    ReverseIndexSlave2: 0..1;
    ReverseIndexSlave3: 0..1;
    ReverseIndexSlave4: 0..1;
    ReverseIndexSlave5: 0..1;
    ReverseIndexSlave6: 0..1;
    ReverseIndexSlave7: 0..1;
    ReverseIndexSlave8: 0..1;
    ReverseIndexSlave9: 0..1;
    ReverseIndexSlave10: 0..1;
    RS485MessageMode: 0..15;
    RS485MessageTime: 0..2047;
    AccelerometerScaling: 0..3;
    Reserved: 0..15;
  end;

type
  stBF4 = bitpacked record
    ErrorDetect: 0..1;
    RVOutputMode: 0..3;
    Reserved: 0..268435455;
  end;

type
  stBF5 = bitpacked record
    HardwareVersion: 0..15;
    AccelPresent: 0..1;
    RVOutputPresent: 0..1;
    RS485DriverPresent: 0..1;
    CanDriverPresent: 0..1;
    TempSensorPresent: 0..1;
    KmaPresent: 0..1;
    Reserved: 0..4194303;
  end;

type
  stBF6 = bitpacked record
    bManualPowerInputEnable: 0..1;
    bManualKmaEnable: 0..1;
    bManualTemperatureEnable: 0..1;
    bManualRVOutputVoltageEnable: 0..1;
    bManualRVOutputCurrentEnable: 0..1;
    bManualAccelXEnable: 0..1;
    bManualAccelYEnable: 0..1;
    bManualAccelZEnable: 0..1;
    bManualIntRefEnable: 0..1;
    bManual5VDCEnable: 0..1;
    bManualSetpointEnable: 0..1;
    Reserved: 0..2097151;
  end;

type
  stBF7 = bitpacked record
    Version: 0..65535;
    CRC16: 0..65535;
  end;

type
  stBF8 = bitpacked record
    SensorMode: 0..1;
    CanUse: 0..3;
    SourceAddress: 0..255;
    CanEcho: 0..3;
    CanMessageTime: 0..1023;
    Spacer: 0..511;
    CanTable: stTwoPointTable;
  end;

type
  stFuzzy = record
    FuzzyMFCounts: stFuzzyMFCounts;
    FuzzyInputMFs: stFuzzyInputMFs;
    FuzzyOutputMFs: stFuzzyOutputMFs;
    FuzzyInputRules: stFuzzyInputRules;
    FuzzyOutputRules: stFuzzyOutputRules;
    FuzzyInputInfo: stFuzzyInputInfo;
    FuzzyOutputInfo: stFuzzyOutputInfo;
    FuzzyRuleInfo: stFuzzyRuleInfo;
    Reserved: array [0..90] of int32;
    BF7: stBF7;
  end;

type
  stHostData = record
    BF6:                   stBF6;
    ManualPowerInput:      int32;
    ManualKma:             int32;
    ManualTemperature:     int32;
    ManualRVOutputVoltage: int32;
    ManualRVOutputCurrent: int32;
    ManualAccelX:          int32;
    ManualAccelY:          int32;
    ManualAccelZ:          int32;
    ManualIntRef:          int32;
    Manual5VDCInput:       int32;
    ManualSetpoint:        int32;
  end;

type
  stCD_41 = record
      // NOTE:  This Structure MUST match same structure in STC32 firmware
    case integer of
      0: (SetpointTable: stTwoPointTable;
          MasterFreqTable: stTwoPointTable;
        BF1: stBF1;
        KmaFilterGain: int32;                               // 1 to 16384
        MasterIndexFilteringGain: int32;                    // 1 to 16384
        AccelVoltageXAdcFilteringGain: int32;               // 1 to 16384
        AccelVoltageYAdcFilteringGain: int32;               // 1 to 16384
        AccelVoltageZAdcFilteringGain: int32;               // 1 to 16384
        BF2: stBF2;
        BF8: stBF8;
        BF3: stBF3;
        ProfileName: array [0..39] of char;
        Reserved: array [0..296] of int32;
        BF7: stBF7);
      1: (Partition: array [0..3] of stPartition);
  end;

type
  stConfigData = record
    SetpointTable: stTwoPointTable;
    MasterFreqTable: stTwoPointTable;
    BF1: stBF1;
    KmaFilterGain: int32;
    MasterIndexFilteringGain: int32;
    AccelVoltageXAdcFilteringGain: int32;
    AccelVoltageYAdcFilteringGain: int32;
    AccelVoltageZAdcFilteringGain: int32;
    BF2: stBF2;
    BF8: stBF8;
    BF3: stBF3;
    ProfileName: array [0..39] of char;
  end;

type
  stCD_42 = record
      // NOTE:  This Structure MUST match same structure in STC32 firmware
    case integer of
      0: (Calibration33: int32;
        PowerRail5KFactor: int32;
        KmaKFactor: int32;
        PowerInputKFactor: int32;
        RVOutputVoltageKFactor: int32;
        RVOutputCurrentKFactor: int32;
        PwmOutputKFactor: int32;
        BF5: stBF5;
        DateStr: array [0..15] of char;
        TimeStr: array [0..11] of char;
        NameTesterStr: array [0..39] of char;
        ModelNumberStr: array [0..39] of char;
        SerialNumberStr: array [0..39] of char;

        LocationStr: array [0..39] of char;
        TypeStr: array [0..39] of char;
        ManufacturerStr: array [0..39] of char;
        HardwareIdStr: array [0..39] of char;
        SoftwareIdStr: array [0..39] of char;
        MakeStr: array [0..7] of char;
        ModeStr: array [0..39] of char;
        SerialStr: array [0..39] of char;
        UnitStr: array [0..39] of char;

        PowerInputFilteringGain: int32;
        PowerRail5FilteringGain: int32;
        TemperatureFilteringGain: int32;
        InternalReferenceFilteringGain: int32;
        RVOutputVoltageFilteringGain: int32;
        RVOutputCurrentFilteringGain: int32;
        Reserved: array [0..376] of int32;
        BF7: stBF7);
      1: (Partition: array [0..3] of stPartition);
  end;

type
  stCDFactoryData = record
    Calibration33: int32;
    PowerRail5KFactor: int32;
    KmaKFactor: int32;
    PowerInputKFactor: int32;
    RVOutputVoltageKFactor: int32;
    RVOutputCurrentKFactor: int32;
    PwmOutputKFactor: int32;
    BF5: stBF5;
    DateStr: array [0..15] of char;
    TimeStr: array [0..11] of char;
    NameTesterStr: array [0..39] of char;
    ModelNumberStr: array [0..39] of char;
    SerialNumberStr: array [0..39] of char;

    LocationStr: array [0..39] of char;
    TypeStr: array [0..39] of char;
    ManufacturerStr: array [0..39] of char;
    HardwareIdStr: array [0..39] of char;
    SoftwareIdStr: array [0..39] of char;
    MakeStr: array [0..7] of char;
    ModeStr: array [0..39] of char;
    SerialStr: array [0..39] of char;
    UnitStr: array [0..39] of char;

    PowerInputFilteringGain: int32;
    PowerRail5FilteringGain: int32;
    TemperatureFilteringGain: int32;
    InternalReferenceFilteringGain: int32;
    RVOutputVoltageFilteringGain: int32;
    RVOutputCurrentFilteringGain: int32;
  end;

type
  stCD_43 = record
    case integer of
    0: (
        Fuzzy1: stFuzzy;
        Fuzzy2: stFuzzy);
    1: (Partition: array [0..3] of stPartition);

  end;

type
  stProfileData = record
    EEPROM_Config: word;
    Trippoint: word;
    KmaSamples: word;
    HoldValue: word;
    TempOutputMode: word;
    ScalingFactor: single;
    ProfileStr: string;
    Profile: array [0..509] of word;
    ProfileLength: integer;
    ProfileIndex: array [0..99] of single;            // 18 to 527
    ProfileSetpoint: array [0..99] of single;
    // x.xx if Voltage,  xxx.x if Resistance
  end;

type
  stCD_48 = record
    // NOTE:  This Structure MUST match same structure in STC32 firmware
    GeneralErrorTimer: array [0..49] of byte;
    // These timers are incremented every 10mS.
    ErrorPresent: array [0..49] of byte;
  end;

type
  stCD_99 = record
    Crc16: word;
    Version: array [0..20] of char;
  end;

type
  stBFEvent = bitpacked record
    TimeStamp: 0..1048575;
    EventId: 0..4095;
  end;
type
  stCD_44 = record
    EntryIndex: int32;
    stBFEvent:  stBFEvent;
    EventValue1: int32;
    EventValue2: int32;
  end;

type
  stFirmwareInfo = record
    Version: word;
    Crc16: word;
  end;

type
  stCD_70 = record
    Version: int32;

    ENG_TemperatureCounts: int32;                // xxxx Counts
    ENG_FilteredTemperatureCounts: int32;        // xxxx.xx Counts
    ENG_TemperatureVoltage: int32;               // x.xxxx V
    ENG_TemperatureCelsius: int32;               // xxx.x C        -55.0 to +130.00
    ENG_TemperatureFrequency: int32;             // xxx.xx Hz      490.18 to 29084.00
    ENG_KmaCounts: int32;                        // xxxx Counts
    ENG_FilteredKmaCounts: int32;                // xxxx.xx counts
    ENG_KmaVoltage: int32;                       // x.xxx V        0.250 to 4.975
    ENG_KmaPercentage: int32;                    // xxx.xxx %      0.00 to 100.000
    ENG_KmaAngle: int32;                         // xxx.xx Deg     0.00 to 180.00
    ENG_KmaIndex: int32;                         // xxxx.xx        4.00 to 1023.00
    ENG_KmaFrequency: int32;                     // xxxxx.xx Hz    64.00 to 16384.00

    ENG_PowerRail5Counts: int32;                 // xxxx counts
    ENG_FilteredPowerRail5Counts: int32;         // xxxx.xx counts
    ENG_PowerRail5VDC: int32;                    // x.xxx V        4.980 nominal

    ENG_PowerInputCounts: int32;                 // xxxx counts
    ENG_FilteredPowerInputCounts: int32;         // xxxx.xx counts
    ENG_PowerInputVoltage: int32;                // xx.xxx V       10.000 to 35.000

    ENG_IntRefCounts: int32;                     // xxxx counts
    ENG_FilteredIntRefCounts: int32;             // xxxx.xx
    ENG_PowerRail33VDC: int32;                   // x.xxx V        3.3000 nominal
    ENG_FactoryIntRefCounts: int32;              // xxxx counts

    ENG_AccelXCounts: int32;                     // xxxx Counts
    ENG_FilteredAccelXCounts: int32;             // xxxx.xx Counts
    ENG_AccelGravityX: int32;                    // x.xxx g        -2.5000 to +2.5000

    ENG_AccelYCounts: int32;                     // xxxx Counts
    ENG_FilteredAccelYCounts: int32;             // xxxx.xx Counts
    ENG_AccelGravityY: int32;                    // x.xxx g        -2.5000 to +2.5000

    ENG_AccelZCounts: int32;                     // xxxx Counts
    ENG_FilteredAccelZCounts: int32;             // xxxx.xx Counts
    ENG_AccelGravityZ: int32;                    // x.xxx g        -2.5000 to +2.5000

    ENG_RVOutputVoltageCounts: int32;
    ENG_FilteredRVOutputVoltageCounts: int32;
    ENG_RVOutputVoltage: int32;                  // xx.xxxx V      0.0000 to 13.3000

    ENG_RVOutputCurrentCounts: int32;
    ENG_FilteredRVOutputCurrentCounts: int32;
    ENG_RVOutputCurrent: int32;                  // xxx.xxx mA     0.000 to 160.000 mA

    ENG_MasterIndex: int32;                      // xxxx.xx        4.00 to 5115.00
    ENG_FilteredMasterIndex: int32;              // xxxx.xx        4.00 to 5115.00
    ENG_MasterFrequency: int32;                  // xxxxx.xx Hz    12.80 to 16384.00

    ENG_RVOutputOhms: int32;                     // xxxx.xx Ohms   0.00 to 10000.00
    ENG_SlaveIndex: array [0..9] of int32;       // xxxx.xx        4.00 to 1023.00
    ENG_SlaveFrequency: array [0..9] of int32;   // xxxx.xx Hz     64.00 to 16384.00
    ENG_SetpointIndex: int32;                    // xxxx.xx        0.00 to 1019.00
    ENG_NumberEventLogEntries: int32;
    ENG_Setpoint: int32;                         // xx.xxx V or xxxx.xx Ohms
    ENG_Feedback: int32;                         // xx.xxx V or xxxx.xx Ohms
    ENG_PwmDutyCycle: int32;                     // xxx.xxx %      0.000 to 100.000
    ENG_PwmDutyCycleFineTune: int32;             // xxx.xx %       -1.00 to +1.00
    ENG_MosfetPower: int32;                      // xx.xxx W
    BF4: stBF4;
    StackSlack: int32;
    StackUsed: int32;
    TestInput: int32;
    CurrentSensePower: int32;                    // xxxx.xxx mW
    Reserved: array [0..24] of int32;            // Reserved for future System Variables
  end;

type
  stSysVars = record
    FirmwareVersion: longint;
    AdcCounts: array [0..15] of longint;              // 0 to
    DigitalFrequency: array [0..5] of longint;          // xxxx.xx Hz
    RV_Ohms: longint;                                  // xxx.xx Ohms
    TestData: array [0..15] of longint;
    AdcVolts: array [0..15] of smallint;                  // x.xxx Volts
    PS_Current: smallint;                               // xx.xx mA
    DigIO_InputLevel: byte;
    OverCurrentFlag: byte;
  end;

type
  stSysVarsCC = record
    DUT_UpdateCounter: longint;
    DUT_Status: longint;
    DUT_Master_Period: longint;
    DUT_KMA_Period: longint;
    DUT_Slave_Period: array [0..3] of longint;
    DUT_Master_Index: longint;
    DUT_KMA_Counts: longint;
    DUT_RV_Volts_Counts: longint;
    DUT_RV_Amps_Counts: longint;
    DUT_AD7814_Counts: longint;
    DUT_Setpoint: longint;
    DUT_Feedback: longint;
    DUT_PWM_Counts: longint;
    DUT_FW_Version: longint;
    DUT_EEPROM_Config: longint;
    DUT_EEPROM_Address: longint;
    EEPROM_Mode: longint;
  end;

type
  stEEPROMPartition = record
    PartitionData: array [0..263] of byte;
  end;

type
  stFactoryData = record
      // NOTE:  This Structure MUST match same structure in STC32 firmware
    case integer of
      0: (AdcZero: array [0..15] of int32;
        AdcSpan: array [0..15] of int32;
        Reserved: array [0..478] of int32;
        CRC16: dword);
      1: (Partition: array [0..3] of stPartition);
  end;

type
  stEEPROMData = record
    case integer of
      0: (OhmsKFactor: word;                    // Add 0
        VoltageKFactor: word;                    // Add 1
        EEPROM_Config: word;                    // Add 2
        Trippoint: word;              // Add 3
        KmaSamples: word;              // Add 4
        HoldValue: word;              // Add 5
        Reserved: array [0..11] of word;          // Add 6-17
        Profile: array [0..509] of word);                // Add 18-527
      1: (Partition: array [0..3] of stEEPROMPartition);
  end;

type
  stSerialVerifyData = record
    VerifyAddress: word;
    VerifyData: word;
    VerifyMode: byte;
  end;

type
  stSetpointData = record
    DIGIO_Frequency_Setpoint: array [0..5] of dword;        // xxxx.xx Hz
    DIGIO_DutyCycle: array [0..5] of dword;                  // xxx.xx %
    NewConfigData: array [0..5] of dword;
    EnableBits: dword;
    //Bit0=Power, Bit1=Pullup, Bit2=Communications, Bit3=Master, Bit 4=Slave1, Bit 5=Slave2
    // Bit 6=Slave3, Bit 7=Temperature, Bit 8=R/V Output, bit9=Refresh Config, bit10=update Config
    // bit11=Refresh Profile bit12=Manual Output
    CommConfig: dword;
    // 1=CC, 2=CD DIGIO1, 3=CD RS485
    ManualSetpoint: dword;
    DIGIO_Configuration: array [0..5] of dword;
    DIGIO_OutputLevel: array [0..5] of dword;
    LED_TestMode: dword;
  end;

type
  stMessageQueue = record
    MsgStr: string;
    Location: integer;
  end;


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

var
  CDCommState, CDCommTimer: integer;


var
  bNetworkThreadRunning: boolean;
  bClearEventLog: boolean;
  SysVars: stSysVars;
  SysVarsCC: stSysVarsCC;
  SetpointData: stSetpointData;
  FactoryData: stFactoryData;
  NewEEPROMData: stEEPROMData;
  CurrentEEPROMData: stEEPROMData;
  SerialVerifyData: stSerialVerifyData;
  FirmwareInfo: stFirmwareInfo;
  ProfileData: TProfileData;
  NewProfileData: TProfileData;
  FPGAProfileData: TProfileData;
  CurrentProfileData: TProfileData;

  bSerialGetVersion: boolean;
  bSerialGetEEPROMData: boolean;
  bSerialGetEEPROMConfigProfileData: boolean;
  bSerialWriteProfile: boolean;
  bSerialVerifyMode: boolean;
  bSerialRelayMessage: boolean;
  bSerialSetFactory: boolean;
  bSerialGetFactory: boolean;
  bIssueReprogrammingCommand: boolean;
  bReprogramming: boolean;
  bGetSysVars: boolean;
  bSysVarsUpdate: boolean;
  bFirmwareUpdate: boolean;
  bFactoryUpdate: boolean;
  bSerialEEPROMDataUpdate: boolean;
  bSerialGetEEPROMDataUpdate: boolean;
  bUploadFlashConfiguration: boolean;
  bUploadFactoryConfiguration: boolean;
  bUploadFuzzyConfiguration: boolean;
  bDownloadFlashConfig: boolean;
  bDownloadFactoryConfig: boolean;
  bDownloadFuzzyConfig: boolean;
  bDownloadEventLog: boolean;
  bDownloadFirmware: boolean;
  bDownloadTiming: boolean;
  bDownloadStatus: boolean;
  bStartCDFirmwareUpload: boolean;

  bCD_14_Update: boolean;
  bCD_41_Update: boolean;
  bCD_42_Update: boolean;
  bCD_43_Update: boolean;
  bCD_44_Update: boolean;
  bCD_48_Update: boolean;
  bCD_70_Update: boolean;
  bCD_71_Update: boolean;
  bCD_99_Update: boolean;

  FlashIndex: word;
  FactoryIndex: word;
  CD_44_Index: word;
  CDFlashIndex: word;
  NextFlashIndex: word;
  EEPROMConfigIndex: word;

  CDCommBufferLength: word;
  CDCommBuffer: array [0..4000] of byte;

  bCommunicating: boolean;

  PullupInputVoltage, RVVoltage, RVOhm: single;
  DUT_RVVolts: single;

  Serial_Timeout: integer;
  bLockCommPort: boolean;
  SyncCount: integer;
  bResetSerial: boolean;
  bCurrentEEPROMDataValid: boolean;
  bUpdateTrend: boolean;
  bRefreshConfig: boolean;
  bRefreshProfile: boolean;
  bUpdateConfig: boolean;
  bCommandPending: boolean;
  bUploadDone: boolean;
  bUploadCDFirmware: boolean;
  bUploadCDFirmwareStarted: boolean;
  bCDSendProgramingData: boolean;

  SerialCounter: integer;

  DUT_InvalidCounter: integer;
  OldUpdateCounter: longint;
  CD_70_UpdateCounter: integer;


  AppPath: string;
  CmdLen, CmdCrc, Crc16Result: word;
  SerialOffset, SerialCase: integer;
  bSendFirmwareData, bEndTest, bAbort: boolean;
  ThreadCounter: integer;
  WaitTime: integer;

  ReceiveBuffer: array [0..$FFF] of byte;
  ReceiveBufferIn, ReceiveBufferOut: dword;
  ReceiveBufferCount: integer;

  FirmwarePageNumber: longint;
  FirmwarePageSize: word;
  FirmwareBinaryBuffer: array [0..$1FFFF] of byte;     // 128K max

  CD_41_Download: stCD_41;
  CD_41_Upload: stCD_41;
  CD_42_Download: stCD_42;
  CD_42_Upload: stCD_42;
  CD_43_Download: stCD_43;
  CD_43_Upload: stCD_43;
  CD_44: array [0..2047] of stCD_44;
  CD_48: stCD_48;
  CD_70: stCD_70;
  CD_71: array [0..39] of stExecutionTimeReport;
  CD_99: stCD_99;
  HostData: stHostData;

  UploadMasterFrequencyTable: stTwoPointTable;
  UploadRVTable: stTwoPointTable;

  CheckBoxErrorCondition: array [0..99] of TCheckBox;

  bEnableLogging: boolean;

  bLoading: boolean;
  bDownloadDone: boolean;
  bGetCCData: boolean;

  ProfileDelayTimer: integer;
  CDVerifyResults: integer;

  ProcessTimingAverageTime: array [0..41] of TEdit;
  ProcessTimingMinTime: array [0..41] of TEdit;
  ProcessTimingMaxTime: array [0..41] of TEdit;
  ProcessTimingCalls: array [0..41] of TEdit;
  ProcessTimingLabel: array [0..41] of TLabel;

  TroubleShootingTestDataLabel: array [0..15] of TLabel;
  TroubleShootingTestDataEdit: array [0..15] of TEdit;
  TroubleShootingAdcCountsLabel: array [0..15] of TLabel;
  TroubleShootingAdcCountsEdit: array [0..15] of TEdit;
  TroubleShootingEngUnitsLabel: array [0..15] of TLabel;
  TroubleShootingEngUnitsEdit: array [0..15] of TEdit;

  CalibrationAdcZeroLabel: array [0..15] of TLabel;
  CalibrationAdcZeroEdit: array [0..15] of TEdit;
  CalibrationAdcSpanLabel: array [0..15] of TLabel;
  CalibrationAdcSpanEdit: array [0..15] of TEdit;

var
  qStart, qFreq, qlast: int64;
  qDelta, qFirstByte, qProcessMessage, qSendCommand: int64;
  TimeGap: single;
  OldNumberOfEvents: integer;

var
  MessageQueue: array [0..$FF] of stMessageQueue;
  NextIn, NextOut: byte;


implementation

end.
