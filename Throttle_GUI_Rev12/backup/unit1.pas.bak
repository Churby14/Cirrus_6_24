unit Unit1;

{$mode objfpc}{$H+}

interface

uses
Windows, Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs,
ExtCtrls, eventlog, global, parse, StdCtrls, ComCtrls, Buttons, ValEdit, Grids,
IniFiles, mmsystem, mystr2float, comm, serial, profile, crc16, Types, Unit2,
Unit3;

procedure ProcessCDCommunications;

type
  stCDProfileData = class
  private
    IniConfigData: stConfigData;
    GuiConfigData: stConfigData;
    CurrentConfigData: stConfigData;
    GuiCDFactoryData: stCDFactoryData;
    CurrentCDFactoryData: stCDFactoryData;
    Fuzzy1Data: stFuzzy;
    Fuzzy2Data: stFuzzy;

  public
    procedure LoadFromIniFile (ProfileName: string);
    function GetRVSetpointTable : string;
    function GetCanBusTable: string;
    function GetMasterFreqSetpointTable : string;
    function GetAngularOutputTable: string;
    procedure SetRVSetpointTable (NewData: String);
    procedure SetCanBusTable (NewData: String);
    procedure SetAngularOutputTable (NewData: String);
    procedure SetMasterFreqSetpointTable (NewData: String);
    procedure ClearData;
    procedure UpdateCD_41;
    procedure UpdateCD_42;
    procedure UpdateCD_43;
    function GetFuzzyMFTable(item: integer) : string;
    function GetFuzzyRules(InOutNumber: integer) : string;
    end;

type
  TProfileGraph = class
  private
  public
    BM: TBitMap;
    GraphImage: TImage;
    HorzLowScale: single;
    HorzHighScale: single;
    HorzPrecision: integer;
    HorzHashMarks: integer;
    HorzTitle: string;
    VertLowScale: single;
    VertHighScale: single;
    VertPrecision: integer;
    VertHashMarks: integer;
    VertTitle: string;
    BorderRect: Trect;
    GraphData: stDataArray;
    procedure Redraw;
    procedure AddPoint(X: single; Y: single);
  end;

type

  { TForm1 }

  TForm1 = class(TForm)
    btnDownloadCurrentProfile: TButton;
    btnDown1: TButton;
    btnDown2: TButton;
    btnDown3: TButton;
    btnDown4: TButton;
    btnDown5: TButton;
    btnDown6: TButton;
    btnDown7: TButton;
    btnFuzzy1UploadMFs: TButton;
    btnFuzzy2UploadMFs: TButton;
    btnGenOff: TButton;
    BtnGenOn: TButton;
    btnPower: TButton;
    btnInternalPullup: TButton;
    btnRefreshConfig: TButton;
    btnStart: TButton;
    btnUp1: TButton;
    btnUp2: TButton;
    btnUp3: TButton;
    btnUp4: TButton;
    btnUp5: TButton;
    btnUp6: TButton;
    btnUp7: TButton;
    btnUpdateConfig: TButton;
    btnSaveProfile: TButton;
    Button1: TButton;
    btnVerifyProfile: TButton;
    btnReloadCalibration: TButton;
    btnUploadCalibration: TButton;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    Button5: TButton;
    btnSaveCDProfile: TButton;
    btnUploadCDFirmware: TButton;
    Button6: TButton;
    btnCreateOutputProfile: TButton;
    cbFuzzy2Input0Not: TCheckBox;
    cbFuzzy2Input0RuleValue: TComboBox;
    cbFuzzy2Input1Not: TCheckBox;
    cbFuzzy2Input1RuleValue: TComboBox;
    cbFuzzy2Input2Not: TCheckBox;
    cbFuzzy2Input2RuleValue: TComboBox;
    cbFuzzy2OutputRuleValue: TComboBox;
    cbUploadCanUseMsgOut1: TComboBox;
    cbUploadCanUseMsgOut2: TComboBox;
    cbUploadCanUseMsgOut3: TComboBox;
    cbUploadCanUseMsgOut4: TComboBox;
    cbUploadCanUseMsgOut5: TComboBox;
    cbUploadCanUseMsgOut0: TComboBox;
    cbUploadCanUseMsgIn0: TComboBox;
    cbUploadCanUseMsgIn1: TComboBox;
    cbUploadConfigCanCrcEnable: TComboBox;
    cbUploadConfigCanBaudRate: TComboBox;
    cbUploadConfigEndianess: TComboBox;
    cbUploadConfigCanUse: TComboBox;
    edtOutputStartupDelay: TEdit;
    edtCanSamplingDelay: TEdit;
    edtSensorAAngle: TEdit;
    edtSensorANoise: TEdit;
    edtSensorBNoise: TEdit;
    edtSensorBAngle: TEdit;
    edtSensorAQuality1: TEdit;
    edtSensorBQuality1: TEdit;
    edtSensorAQuality2: TEdit;
    edtSensorBQuality2: TEdit;
    edtUploadConfigAngularOutput: TEdit;
    edtUploadMaskMsgIn0: TEdit;
    edtUploadMaskMsgIn1: TEdit;
    edtUploadAddressMsgOut0: TEdit;
    edtSensorASetpoint: TEdit;
    edtFuzzy1OutputMax: TEdit;
    edtFuzzy2OutputMax: TEdit;
    edtFuzzy1OutputMin: TEdit;
    edtFuzzy1AddRule: TButton;
    Button8: TButton;
    btnUploadProfile: TButton;
    edtFuzzy1Input0Mf0Pt0: TEdit;
    edtFuzzy1Input0Mf0Pt1: TEdit;
    edtFuzzy1Input0Mf0Pt2: TEdit;
    edtFuzzy1Input0Mf0Pt3: TEdit;
    edtFuzzy1Input0Mf0Shape: TComboBox;
    edtFuzzy1Input0Mf1Pt0: TEdit;
    edtFuzzy1Input0Mf1Pt1: TEdit;
    edtFuzzy1Input0Mf1Pt2: TEdit;
    edtFuzzy1Input0Mf1Pt3: TEdit;
    edtFuzzy1Input0Mf1Shape: TComboBox;
    edtFuzzy1Input0Mf2Pt0: TEdit;
    edtFuzzy1Input0Mf2Pt1: TEdit;
    edtFuzzy1Input0Mf2Pt2: TEdit;
    edtFuzzy1Input0Mf2Pt3: TEdit;
    edtFuzzy1Input0Mf2Shape: TComboBox;
    edtFuzzy1Input1Mf0Pt0: TEdit;
    edtFuzzy1Input1Mf0Pt1: TEdit;
    edtFuzzy1Input1Mf0Pt2: TEdit;
    edtFuzzy1Input1Mf0Pt3: TEdit;
    edtFuzzy1Input1Mf0Shape: TComboBox;
    edtFuzzy1Input1Mf1Pt0: TEdit;
    edtFuzzy1Input1Mf1Pt1: TEdit;
    edtFuzzy1Input1Mf1Pt2: TEdit;
    edtFuzzy1Input1Mf1Pt3: TEdit;
    edtFuzzy1Input1Mf1Shape: TComboBox;
    edtFuzzy1Input1Mf2Pt0: TEdit;
    edtFuzzy1Input1Mf2Pt1: TEdit;
    edtFuzzy1Input1Mf2Pt2: TEdit;
    edtFuzzy1Input1Mf2Pt3: TEdit;
    edtFuzzy1Input1Mf2Shape: TComboBox;
    edtFuzzy1Input2Mf0Pt0: TEdit;
    edtFuzzy1Input2Mf0Pt1: TEdit;
    edtFuzzy1Input2Mf0Pt2: TEdit;
    edtFuzzy1Input2Mf0Pt3: TEdit;
    edtFuzzy1Input2Mf0Shape: TComboBox;
    edtFuzzy1Input2Mf1Pt0: TEdit;
    edtFuzzy1Input2Mf1Pt1: TEdit;
    edtFuzzy1Input2Mf1Pt2: TEdit;
    edtFuzzy1Input2Mf1Pt3: TEdit;
    edtFuzzy1Input2Mf1Shape: TComboBox;
    edtFuzzy1Input2Mf2Pt0: TEdit;
    edtFuzzy1Input2Mf2Pt1: TEdit;
    edtFuzzy1Input2Mf2Pt2: TEdit;
    edtFuzzy1Input2Mf2Pt3: TEdit;
    edtFuzzy1Input2Mf2Shape: TComboBox;
    edtFuzzy1OutputMf0Pt0: TEdit;
    edtFuzzy1OutputMf0Pt1: TEdit;
    edtFuzzy1OutputMf0Pt2: TEdit;
    edtFuzzy1OutputMf0Pt3: TEdit;
    edtFuzzy1OutputMf0Shape: TComboBox;
    edtFuzzy1OutputMf1Pt0: TEdit;
    edtFuzzy1OutputMf1Pt1: TEdit;
    edtFuzzy1OutputMf1Pt2: TEdit;
    edtFuzzy1OutputMf1Pt3: TEdit;
    edtFuzzy1OutputMf1Shape: TComboBox;
    edtFuzzy1OutputMf2Pt0: TEdit;
    edtFuzzy1OutputMf2Pt1: TEdit;
    edtFuzzy1OutputMf2Pt2: TEdit;
    edtFuzzy1OutputMf2Pt3: TEdit;
    edtFuzzy1OutputMf2Shape: TComboBox;
    edtFuzzy2OutputMin: TEdit;
    edtFuzzy2AddRule: TButton;
    edtFuzzy1RemoveRule: TButton;
    cbAccelerometer: TCheckBox;
    cbCANBusDriver: TCheckBox;
    cbCascade: TCheckBox;
    cbCDProfileNames: TComboBox;
    cbDigIn1: TComboBox;
    cbDigIn2: TComboBox;
    cbDigIn3: TComboBox;
    cbDigIn4: TComboBox;
    cbDigIn5: TComboBox;
    cbDigIn6: TComboBox;
    cbDigMode1: TComboBox;
    cbDigMode2: TComboBox;
    cbDigMode3: TComboBox;
    cbDigMode4: TComboBox;
    cbDigMode5: TComboBox;
    cbDigMode6: TComboBox;
    cbDigOut1: TComboBox;
    cbDigOut2: TComboBox;
    cbDigOut3: TComboBox;
    cbDigOut4: TComboBox;
    cbDigOut5: TComboBox;
    cbDigOut6: TComboBox;
    cbManual: TCheckBox;
    cbKmaSensor: TCheckBox;
    cbUploadConfigAccelerometerScaling: TComboBox;
    cbUploadConfigReverseKMA: TCheckBox;
    cbUploadConfigDig4: TComboBox;
    cbUploadConfigDig5: TComboBox;
    cbUploadConfigDig6: TComboBox;
    cbUploadConfigDig1: TComboBox;
    cbUploadConfigDig2: TComboBox;
    cbUploadConfigDig3: TComboBox;
    cbUploadConfigErrorActiveLevel: TComboBox;
    cbUploadConfigMasterMode: TComboBox;
    cbUploadConfigOutputMode: TComboBox;
    cbUploadConfigPullUpMode: TComboBox;
    cbFPGAHoldMode: TComboBox;
    cbFPGAKmaDirection: TComboBox;
    cbFPGAProfileType: TComboBox;
    cbFPGATemperatureOutput: TComboBox;
    cbUploadConfigRS485Format: TComboBox;
    cbNoSlave1Input: TCheckBox;
    cbLowFuelActiveHigh: TCheckBox;
    cbLowFuelActiveLow: TCheckBox;
    cbHoldGreater: TCheckBox;
    cbHoldLessThan: TCheckBox;
    cbLowFuelSelected: TCheckBox;
    cbHoldLogicSelected: TCheckBox;
    cbCommunicationsEnable: TCheckBox;
    cbNoSlave2Input: TCheckBox;
    cbNoSlave3Input: TCheckBox;
    cbMasterEnable: TCheckBox;
    cbFPGAProfileName: TComboBox;
    cbPcbRevision: TComboBox;
    cbRS485Driver: TCheckBox;
    cbRVOutput: TCheckBox;
    cbSerialVerify: TCheckBox;
    cbRVVoltage: TCheckBox;
    cbRvResistance: TCheckBox;
    cbKmaReverse: TCheckBox;
    cbLowHoldEnabled: TCheckBox;
    cbTemperatureOutput: TComboBox;
    cbProfileName: TComboBox;
    cbHoldMode: TComboBox;
    cbCalibrateOutput: TCheckBox;
    cbTemperatureSensor: TCheckBox;
    cbManualForm: TCheckBox;
    cbUploadConfigCanEcho: TComboBox;
    cbFuzzy1Input0Not: TCheckBox;
    cbFuzzy1Input1Not: TCheckBox;
    cbFuzzy1Input2Not: TCheckBox;
    cbFuzzy1Input0RuleValue: TComboBox;
    cbFuzzy1Input1RuleValue: TComboBox;
    cbFuzzy1Input2RuleValue: TComboBox;
    cbFuzzy1OutputRuleValue: TComboBox;
    edtFuzzy2RemoveRule: TButton;
    edtFuzzy1RuleWeight: TEdit;
    edtFuzzy2RuleWeight: TEdit;
    edtFuzzy2Input0Mf0Pt0: TEdit;
    edtFuzzy2Input0Mf0Pt1: TEdit;
    edtFuzzy2Input0Mf0Pt2: TEdit;
    edtFuzzy2Input0Mf0Pt3: TEdit;
    edtFuzzy2Input0Mf0Shape: TComboBox;
    edtFuzzy2Input0Mf1Pt0: TEdit;
    edtFuzzy2Input0Mf1Pt1: TEdit;
    edtFuzzy2Input0Mf1Pt2: TEdit;
    edtFuzzy2Input0Mf1Pt3: TEdit;
    edtFuzzy2Input0Mf1Shape: TComboBox;
    edtFuzzy2Input0Mf2Pt0: TEdit;
    edtFuzzy2Input0Mf2Pt1: TEdit;
    edtFuzzy2Input0Mf2Pt2: TEdit;
    edtFuzzy2Input0Mf2Pt3: TEdit;
    edtFuzzy2Input0Mf2Shape: TComboBox;
    edtFuzzy2Input1Mf0Pt0: TEdit;
    edtFuzzy2Input1Mf0Pt1: TEdit;
    edtFuzzy2Input1Mf0Pt2: TEdit;
    edtFuzzy2Input1Mf0Pt3: TEdit;
    edtFuzzy2Input1Mf0Shape: TComboBox;
    edtFuzzy2Input1Mf1Pt0: TEdit;
    edtFuzzy2Input1Mf1Pt1: TEdit;
    edtFuzzy2Input1Mf1Pt2: TEdit;
    edtFuzzy2Input1Mf1Pt3: TEdit;
    edtFuzzy2Input1Mf1Shape: TComboBox;
    edtFuzzy2Input1Mf2Pt0: TEdit;
    edtFuzzy2Input1Mf2Pt1: TEdit;
    edtFuzzy2Input1Mf2Pt2: TEdit;
    edtFuzzy2Input1Mf2Pt3: TEdit;
    edtFuzzy2Input1Mf2Shape: TComboBox;
    edtFuzzy2Input2Mf0Pt0: TEdit;
    edtFuzzy2Input2Mf0Pt1: TEdit;
    edtFuzzy2Input2Mf0Pt2: TEdit;
    edtFuzzy2Input2Mf0Pt3: TEdit;
    edtFuzzy2Input2Mf0Shape: TComboBox;
    edtFuzzy2Input2Mf1Pt0: TEdit;
    edtFuzzy2Input2Mf1Pt1: TEdit;
    edtFuzzy2Input2Mf1Pt2: TEdit;
    edtFuzzy2Input2Mf1Pt3: TEdit;
    edtFuzzy2Input2Mf1Shape: TComboBox;
    edtFuzzy2Input2Mf2Pt0: TEdit;
    edtFuzzy2Input2Mf2Pt1: TEdit;
    edtFuzzy2Input2Mf2Pt2: TEdit;
    edtFuzzy2Input2Mf2Pt3: TEdit;
    edtFuzzy2Input2Mf2Shape: TComboBox;
    edtFuzzy2OutputMf0Pt0: TEdit;
    edtFuzzy2OutputMf0Pt1: TEdit;
    edtFuzzy2OutputMf0Pt2: TEdit;
    edtFuzzy2OutputMf0Pt3: TEdit;
    edtFuzzy2OutputMf0Shape: TComboBox;
    Edit1: TEdit;
    edtFuzzy2OutputMf1Pt0: TEdit;
    edtFuzzy2OutputMf1Pt1: TEdit;
    edtFuzzy2OutputMf1Pt2: TEdit;
    edtFuzzy2OutputMf1Pt3: TEdit;
    edtFuzzy2OutputMf1Shape: TComboBox;
    edtFuzzy2OutputMf2Pt0: TEdit;
    edtFuzzy2OutputMf2Pt1: TEdit;
    edtFuzzy2OutputMf2Pt2: TEdit;
    edtFuzzy2OutputMf2Pt3: TEdit;
    edtFuzzy2OutputMf2Shape: TComboBox;
    edtSensorAKmaIndex: TEdit;
    edtSensorAErrorValue: TEdit;
    edtSensorBErrorValue: TEdit;
    edtSensorBKmaIndex: TEdit;
    edtSensorBSetpoint: TEdit;
    edtUploadAddressMsgIn0: TEdit;
    edtUploadAddressMsgIn1: TEdit;
    edtUploadAddressMsgOut1: TEdit;
    edtUploadAddressMsgOut2: TEdit;
    edtUploadAddressMsgOut3: TEdit;
    edtUploadAddressMsgOut4: TEdit;
    edtUploadAddressMsgOut5: TEdit;
    edtUploadFrequencyMsgOut0: TEdit;
    edtUploadConfigCanBus: TEdit;
    edtUploadConfigSourceAddress: TEdit;
    edtConfigManufacturingUnit: TEdit;
    edtConfigManufacturingSerial: TEdit;
    edtConfigManufacturingModel: TEdit;
    edtConfigManufacturingMake: TEdit;
    edtConfigManufacturingSoftwareId: TEdit;
    edtConfigManufacturingHardwareId: TEdit;
    edtConfigManufacturingManufacturer: TEdit;
    edtConfigManufacturingType: TEdit;
    edtConfigManufacturingLocation: TEdit;
    edtConfig33Calibration: TLabeledEdit;
    edtConfig5VDCCalibration: TLabeledEdit;
    edtConfigPowerInputFilteringGain: TLabeledEdit;
    edtConfig5VDCFilteringGain: TLabeledEdit;
    edtConfigTemperatureFilteringGain: TLabeledEdit;
    edtConfigRVVoltageFilteringGain: TLabeledEdit;
    edtConfigRVCurrentFilteringGain: TLabeledEdit;
    edtConfigInternalReferneceFilteringGain: TLabeledEdit;
    edtDigFreqBlue: TEdit;
    edtDigFreqIn1: TEdit;
    edtDigFreqIn2: TEdit;
    edtDigFreqIn3: TEdit;
    edtDigFreqIn4: TEdit;
    edtDigFreqIn5: TEdit;
    edtDigFreqOut1: TEdit;
    edtDigFreqOut2: TEdit;
    edtDigFreqOut3: TEdit;
    edtDigFreqOut4: TEdit;
    edtDigFreqOut5: TEdit;
    edtDigFreqOut6: TEdit;
    edtDigPwm1: TEdit;
    edtDigPwm2: TEdit;
    edtDigPwm3: TEdit;
    edtDigPwm4: TEdit;
    edtDigPwm5: TEdit;
    edtDigPwm6: TEdit;
    edtGenDigit1: TEdit;
    edtGenDigit2: TEdit;
    edtGenDigit3: TEdit;
    edtGenDigit4: TEdit;
    edtGenDigit5: TEdit;
    edtGenDigit6: TEdit;
    edtGenDigit7: TEdit;
    edtIndex: TEdit;
    edtManual: TEdit;
    edtUploadConfigMasterSetpoint: TEdit;
    edtUploadConfigRVsetpoint: TEdit;
    edtUploadConfigAccelXGain: TLabeledEdit;
    edtUploadConfigAccelYGain: TLabeledEdit;
    edtUploadConfigAccelZGain: TLabeledEdit;
    edtConfigKmaCalibration: TLabeledEdit;
    edtUploadConfigKmaGain: TLabeledEdit;
    edtConfigManufacturingDate: TEdit;
    edtConfigManufacturingModelNumber: TEdit;
    edtConfigManufacturingNameTester: TEdit;
    edtConfigManufacturingSerialNumber: TEdit;
    edtConfigManufacturingTime: TEdit;
    edtUploadConfigMasterGain: TLabeledEdit;
    edtConfigPowerInputVoltageCalibration: TLabeledEdit;
    edtConfigPwmOutput: TLabeledEdit;
    edtConfigRVCurrentScaleCalibration: TLabeledEdit;
    edtUploadConfigRS485Time: TEdit;
    edtConfigRVVoltageCalibration: TLabeledEdit;
    edtFPGAHoldValue: TEdit;
    edtFPGAKmaSamples: TEdit;
    edtFPGASetpointProfile: TEdit;
    edtFPGATripPoint: TEdit;
    edtTripPoint: TEdit;
    edtHoldValue: TEdit;
    edtKmaSamples: TEdit;
    edtSetpointProfile: TEdit;
    edtUploadFrequencyMsgOut1: TEdit;
    edtUploadFrequencyMsgOut2: TEdit;
    edtUploadFrequencyMsgIn0: TEdit;
    edtUploadFrequencyMsgIn1: TEdit;
    edtVoltsGreen: TEdit;
    Fuzzy2RuleMemo: TMemo;
    gbAccelerometer2: TGroupBox;
    gbFuzzy1Rule1: TGroupBox;
    gbMaster2: TGroupBox;
    gbRVOutput2: TGroupBox;
    GroupBox1: TGroupBox;
    GroupBox10: TGroupBox;
    GroupBox11: TGroupBox;
    GroupBox12: TGroupBox;
    GroupBox13: TGroupBox;
    GroupBox14: TGroupBox;
    GroupBox15: TGroupBox;
    GroupBox16: TGroupBox;
    GroupBox17: TGroupBox;
    GroupBox18: TGroupBox;
    GroupBox19: TGroupBox;
    GroupBox2: TGroupBox;
    GroupBox20: TGroupBox;
    GroupBox21: TGroupBox;
    GroupBox22: TGroupBox;
    GroupBox24: TGroupBox;
    GroupBox29: TGroupBox;
    Image15: TImage;
    Label101: TLabel;
    Label102: TLabel;
    Label118: TLabel;
    Label119: TLabel;
    Label120: TLabel;
    Label121: TLabel;
    Label122: TLabel;
    Label123: TLabel;
    Label124: TLabel;
    Label125: TLabel;
    Label18: TLabel;
    Label24: TLabel;
    Label25: TLabel;
    Label26: TLabel;
    Label263: TLabel;
    Label27: TLabel;
    Label29: TLabel;
    Label30: TLabel;
    Label31: TLabel;
    Label32: TLabel;
    Label34: TLabel;
    Label35: TLabel;
    Label36: TLabel;
    Label37: TLabel;
    Label39: TLabel;
    Label40: TLabel;
    Label41: TLabel;
    Label42: TLabel;
    Label44: TLabel;
    Label45: TLabel;
    Label46: TLabel;
    Label47: TLabel;
    Label49: TLabel;
    Label50: TLabel;
    Label53: TLabel;
    Label68: TLabel;
    Label76: TLabel;
    Label77: TLabel;
    Label83: TLabel;
    Label84: TLabel;
    Label86: TLabel;
    Label87: TLabel;
    Label88: TLabel;
    Label89: TLabel;
    Label91: TLabel;
    Label92: TLabel;
    Label93: TLabel;
    Label94: TLabel;
    Label96: TLabel;
    Label97: TLabel;
    Label98: TLabel;
    Label99: TLabel;
    leCanSetpoint: TLabeledEdit;
    Power_Lever_Info: TGroupBox;
    GroupBox23: TGroupBox;
    GroupBox26: TGroupBox;
    GroupBox27: TGroupBox;
    GroupBox28: TGroupBox;
    GroupBox3: TGroupBox;
    GroupBox30: TGroupBox;
    GroupBox31: TGroupBox;
    GroupBox32: TGroupBox;
    GroupBox39: TGroupBox;
    GroupBox4: TGroupBox;
    GroupBox40: TGroupBox;
    GroupBox41: TGroupBox;
    GroupBox42: TGroupBox;
    GroupBox43: TGroupBox;
    GroupBox44: TGroupBox;
    GroupBox45: TGroupBox;
    GroupBox46: TGroupBox;
    GroupBox47: TGroupBox;
    GroupBox48: TGroupBox;
    GroupBox49: TGroupBox;
    GroupBox5: TGroupBox;
    GroupBox50: TGroupBox;
    gbFuzzy1Rule: TGroupBox;
    GroupBox51: TGroupBox;
    GroupBox52: TGroupBox;
    GroupBox53: TGroupBox;
    GroupBox54: TGroupBox;
    GroupBox55: TGroupBox;
    GroupBox56: TGroupBox;
    GroupBox57: TGroupBox;
    GroupBox58: TGroupBox;
    GroupBox59: TGroupBox;
    GroupBox6: TGroupBox;
    GroupBox60: TGroupBox;
    GroupBox61: TGroupBox;
    GroupBox62: TGroupBox;
    GroupBox7: TGroupBox;
    GroupBox8: TGroupBox;
    GroupBox9: TGroupBox;
    Image11: TImage;
    Image12: TImage;
    Image2: TImage;
    Image3: TImage;
    Image14: TImage;
    Label10: TLabel;
    Label103: TLabel;
    Label104: TLabel;
    Label105: TLabel;
    Label106: TLabel;
    Label107: TLabel;
    Label108: TLabel;
    Label109: TLabel;
    Label11: TLabel;
    Label110: TLabel;
    Label111: TLabel;
    Label112: TLabel;
    Label113: TLabel;
    Label114: TLabel;
    Label115: TLabel;
    Label116: TLabel;
    Label117: TLabel;
    Label12: TLabel;
    Label13: TLabel;
    Label131: TLabel;
    Label132: TLabel;
    Label133: TLabel;
    Label136: TLabel;
    Label137: TLabel;
    Label138: TLabel;
    Label139: TLabel;
    Label14: TLabel;
    Label140: TLabel;
    Label141: TLabel;
    Label142: TLabel;
    Label143: TLabel;
    Label144: TLabel;
    Label145: TLabel;
    Label146: TLabel;
    Label147: TLabel;
    Label148: TLabel;
    Label149: TLabel;
    Label15: TLabel;
    Label150: TLabel;
    Label151: TLabel;
    Label152: TLabel;
    Label153: TLabel;
    Label154: TLabel;
    Label155: TLabel;
    Label156: TLabel;
    Label157: TLabel;
    Label158: TLabel;
    Label159: TLabel;
    Label16: TLabel;
    Label160: TLabel;
    Label161: TLabel;
    Label162: TLabel;
    Label163: TLabel;
    Label164: TLabel;
    Label165: TLabel;
    Label166: TLabel;
    Label17: TLabel;
    Label171: TLabel;
    Label176: TLabel;
    Label177: TLabel;
    Label179: TLabel;
    Label180: TLabel;
    Label181: TLabel;
    Label182: TLabel;
    Label183: TLabel;
    Label184: TLabel;
    Label185: TLabel;
    Label186: TLabel;
    Label187: TLabel;
    Label188: TLabel;
    Label189: TLabel;
    Label19: TLabel;
    Label190: TLabel;
    Label192: TLabel;
    Label194: TLabel;
    Label195: TLabel;
    Label196: TLabel;
    Label197: TLabel;
    Label198: TLabel;
    Label199: TLabel;
    Label20: TLabel;
    Label200: TLabel;
    Label201: TLabel;
    Label202: TLabel;
    Label203: TLabel;
    Label204: TLabel;
    Label205: TLabel;
    Label206: TLabel;
    Label207: TLabel;
    Label208: TLabel;
    Label209: TLabel;
    Label21: TLabel;
    Label210: TLabel;
    Label211: TLabel;
    Label212: TLabel;
    Label213: TLabel;
    Label22: TLabel;
    Label220: TLabel;
    Label221: TLabel;
    Label222: TLabel;
    Label225: TLabel;
    Label226: TLabel;
    Label227: TLabel;
    Label241: TLabel;
    Label242: TLabel;
    Label243: TLabel;
    Label244: TLabel;
    Label23: TLabel;
    Label252: TLabel;
    Label254: TLabel;
    Label255: TLabel;
    Label256: TLabel;
    Label28: TLabel;
    Label258: TLabel;
    Label259: TLabel;
    Label261: TLabel;
    Label262: TLabel;
    Label310: TLabel;
    Label33: TLabel;
    Label264: TLabel;
    Label265: TLabel;
    Label266: TLabel;
    Label267: TLabel;
    Label38: TLabel;
    Label269: TLabel;
    Label270: TLabel;
    Label271: TLabel;
    Label272: TLabel;
    Label75: TLabel;
    Label274: TLabel;
    Label275: TLabel;
    Label276: TLabel;
    Label277: TLabel;
    Label95: TLabel;
    Label279: TLabel;
    Label280: TLabel;
    Label281: TLabel;
    Label282: TLabel;
    Label43: TLabel;
    Label284: TLabel;
    Label285: TLabel;
    Label286: TLabel;
    Label287: TLabel;
    Label85: TLabel;
    Label289: TLabel;
    Label290: TLabel;
    Label291: TLabel;
    Label292: TLabel;
    Label100: TLabel;
    Label294: TLabel;
    Label295: TLabel;
    Label296: TLabel;
    Label297: TLabel;
    Label48: TLabel;
    Label299: TLabel;
    Label300: TLabel;
    Label301: TLabel;
    Label302: TLabel;
    Label90: TLabel;
    Label304: TLabel;
    Label305: TLabel;
    Label306: TLabel;
    Label307: TLabel;
    Label135: TLabel;
    Label309: TLabel;
    lblFuzzy2NumberRules: TLabel;
    lblFuzzy1NumberRules: TLabel;
    rgDefaultOutput: TRadioGroup;
    rgUploadFuzzyYN: TRadioGroup;
    rgFuzzy2RuleType: TRadioGroup;
    TabSheet6: TTabSheet;
    TabSheetFuzzy1MF: TTabSheet;
    Weight: TLabel;
    Label260: TLabel;
    Label167: TLabel;
    Label168: TLabel;
    Label169: TLabel;
    Label170: TLabel;
    Label172: TLabel;
    Label173: TLabel;
    Label174: TLabel;
    Label175: TLabel;
    Label178: TLabel;
    Label191: TLabel;
    Label193: TLabel;
    Label214: TLabel;
    Label215: TLabel;
    Label216: TLabel;
    Label217: TLabel;
    Label218: TLabel;
    Label219: TLabel;
    Label223: TLabel;
    Label224: TLabel;
    Label229: TLabel;
    Label230: TLabel;
    Label235: TLabel;
    Label236: TLabel;
    Label237: TLabel;
    Label245: TLabel;
    Label246: TLabel;
    Label247: TLabel;
    Label248: TLabel;
    Label249: TLabel;
    Label250: TLabel;
    Label253: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label51: TLabel;
    Label57: TLabel;
    Label58: TLabel;
    Label59: TLabel;
    HardwareId: TLabel;
    Label6: TLabel;
    Label60: TLabel;
    Label61: TLabel;
    Label62: TLabel;
    Label69: TLabel;
    Label7: TLabel;
    Label70: TLabel;
    Label71: TLabel;
    Label72: TLabel;
    Label73: TLabel;
    Label74: TLabel;
    Label8: TLabel;
    Label9: TLabel;
    lblEventsAvaliable: TLabel;
    Label65: TLabel;
    Label66: TLabel;
    Label78: TLabel;
    Label79: TLabel;
    Label80: TLabel;
    Label81: TLabel;
    Label82: TLabel;
    leCD44_Events: TLabeledEdit;
    lblHoldMode1: TLabel;
    lblHoldValue1: TLabel;
    lblInternalPullup: TLabel;
    Label52: TLabel;
    lblMessage: TLabel;
    Label54: TLabel;
    Label55: TLabel;
    Label56: TLabel;
    Label63: TLabel;
    Label64: TLabel;
    lblHoldMode: TLabel;
    lblTripPoint: TLabel;
    lblHoldValue: TLabel;
    Label67: TLabel;
    lblTripPoint1: TLabel;
    le33PwrRail: TLabeledEdit;
    le33PwrRail5: TLabeledEdit;
    le5PwrRail: TEdit;
    le5PwrRail1: TEdit;
    le5PwrRail2: TEdit;
    leAccelXg: TEdit;
    leAccelXg1: TEdit;
    leAccelXg2: TEdit;
    leAccelYg: TEdit;
    leAccelYg1: TEdit;
    leAccelYg2: TEdit;
    leAccelZg: TEdit;
    leAccelZg1: TEdit;
    leAccelZg2: TEdit;
    leFeedback1: TLabeledEdit;
    leFirmwareBuild: TLabeledEdit;
    leFirmwareCRC: TLabeledEdit;
    leFirmwareStackSlack: TLabeledEdit;
    leFirmwareStackUsed: TLabeledEdit;
    leIntRef10: TEdit;
    leIntRef9: TEdit;
    leKMA: TEdit;
    leKMA1: TEdit;
    leKMA2: TEdit;
    leKmaDeg: TLabeledEdit;
    leKmaFreq1: TLabeledEdit;
    leKmaIndex: TLabeledEdit;
    leKmaPercent: TLabeledEdit;
    leMasterFrequency: TLabeledEdit;
    leMasterIndex1: TLabeledEdit;
    leMasterIndex2: TLabeledEdit;
    leMosfetPower: TLabeledEdit;
    lePowerInputVoltage: TEdit;
    lePowerInputVoltage1: TEdit;
    lePowerInputVoltage2: TEdit;
    lePwmDutyCycle: TLabeledEdit;
    lePwmDutyCycle1: TLabeledEdit;
    leRVOutputCurrent: TEdit;
    leRVOutputCurrent1: TEdit;
    leRVOutputCurrent2: TEdit;
    leRVOutputOhms: TLabeledEdit;
    leRVOutputVoltage: TEdit;
    leRVOutputVoltage1: TEdit;
    leRVOutputVoltage2: TEdit;
    leRVResistance: TLabeledEdit;
    leSetpoint1: TLabeledEdit;
    leSetpointIndex: TLabeledEdit;
    leTemp: TEdit;
    leTemp1: TEdit;
    leTemp2: TEdit;
    leTempFreq: TLabeledEdit;
    leTempHz: TLabeledEdit;
    leKmaPeriod: TLabeledEdit;
    leRVVoltsCounts: TLabeledEdit;
    leRVCurrentCounts: TLabeledEdit;
    leSetpoint: TLabeledEdit;
    leFeedback: TLabeledEdit;
    lePwmCounts: TLabeledEdit;
    leEepromAddress: TLabeledEdit;
    leFirmwareVersion: TLabeledEdit;
    leEEPROMConfig: TLabeledEdit;
    leOhmsKFactor: TLabeledEdit;
    leVoltsKFactor: TLabeledEdit;
    leSlave1Period: TLabeledEdit;
    leTrippoint: TLabeledEdit;
    leKmaSamples: TLabeledEdit;
    leHoldValue: TLabeledEdit;
    leTempDegC: TLabeledEdit;
    leDac1Counts24: TLabeledEdit;
    lePwmVolts: TLabeledEdit;
    leSlave2Freq: TLabeledEdit;
    leSlave1Freq: TLabeledEdit;
    leKmaFreq: TLabeledEdit;
    leMasterFreq: TLabeledEdit;
    leSlave2Period: TLabeledEdit;
    leRvVolts: TLabeledEdit;
    leRVCurrent: TLabeledEdit;
    leSlave3Period: TLabeledEdit;
    leSlave3Freq: TLabeledEdit;
    leMasterPeriod: TLabeledEdit;
    leKmaCounts: TLabeledEdit;
    leMasterIndex: TLabeledEdit;
    leAD7814Counts: TLabeledEdit;
    lePowerCurrent: TLabeledEdit;
    lePowerVoltage: TLabeledEdit;
    leRVvoltage: TLabeledEdit;
    Memo2: TMemo;
    Memo3: TMemo;
    Fuzzy1RuleMemo: TMemo;
    OpenDialog1: TOpenDialog;
    PageControl1: TPageControl;
    PageControl2: TPageControl;
    rgFuzzy1RuleType: TRadioGroup;
    TabSheetFuzzy2Rules: TTabSheet;
    TabSheetFuzzy1Rules: TTabSheet;
    TabSheetFuzzy: TPageControl;
    PageControl6: TPageControl;
    pnlReadConfig: TPanel;
    ProgressBar1: TProgressBar;
    ProgressBar2: TProgressBar;
    rgUploadConfigSensorMode: TRadioGroup;
    rgMode: TRadioGroup;
    rgProfileType: TRadioGroup;
    rgKmaDirection: TRadioGroup;
    rgSlaves: TRadioGroup;
    ScrollBox1: TScrollBox;
    ScrollBox3: TScrollBox;
    shtMasterFreqProfile2: TTabSheet;
    shtRVProfile2: TTabSheet;
    StatusBar1: TStatusBar;
    TabSheet1: TTabSheet;
    TabSheet10: TTabSheet;
    TabSheet11: TTabSheet;
    TabSheet14: TTabSheet;
    TabSheet15: TTabSheet;
    TabSheet16: TTabSheet;
    TabSheet17: TTabSheet;
    TabSheet2: TTabSheet;
    shtCANBusOutput: TTabSheet;
    TabSheet3: TTabSheet;
    TabSheet4: TTabSheet;
    TabSheet5: TTabSheet;
    TabSheet7: TTabSheet;
    TabSheet8: TTabSheet;
    TabSheetFuzzy2MF: TTabSheet;
    Timer1: TTimer;
    Memo1: TMemo;
    imgIcon: TImage;
    StaticText2: TStaticText;
    StaticText3: TStaticText;
    StaticText4: TStaticText;
    StaticText5: TStaticText;
    edtNameofTester: TEdit;
    edtSerialNumber: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    Weight1: TLabel;
    procedure btnCreateOutputProfileClick(Sender: TObject);
    procedure btnDownloadCurrentProfileClick(Sender: TObject);
    procedure btnDown1Click(Sender: TObject);
    procedure btnDown2Click(Sender: TObject);
    procedure btnDown3Click(Sender: TObject);
    procedure btnDown4Click(Sender: TObject);
    procedure btnDown5Click(Sender: TObject);
    procedure btnDown6Click(Sender: TObject);
    procedure btnDown7Click(Sender: TObject);
    procedure btnGenOffClick(Sender: TObject);
    procedure BtnGenOnClick(Sender: TObject);
    procedure btnStartClick(Sender: TObject);
    procedure btnPowerClick(Sender: TObject);
    procedure btnInternalPullupClick(Sender: TObject);
    procedure btnRefreshConfigClick(Sender: TObject);
    procedure btnUp1Click(Sender: TObject);
    procedure btnUp2Click(Sender: TObject);
    procedure btnUp3Click(Sender: TObject);
    procedure btnUp4Click(Sender: TObject);
    procedure btnUp5Click(Sender: TObject);
    procedure btnUp6Click(Sender: TObject);
    procedure btnUp7Click(Sender: TObject);
    procedure btnUpdateConfigClick(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure btnSaveProfileClick(Sender: TObject);
    procedure btnVerifyProfileClick(Sender: TObject);
    procedure btnReloadCalibrationClick(Sender: TObject);
    procedure btnUploadCalibrationClick(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure btnSaveCDProfileClick(Sender: TObject);
    procedure btnUploadCDFirmwareClick(Sender: TObject);
    procedure Button6Click(Sender: TObject);
    procedure Button8Click(Sender: TObject);
    procedure btnUploadProfileClick(Sender: TObject);
    procedure cbCDProfileNamesChange(Sender: TObject);
    procedure cbCDProfileNamesCloseUp(Sender: TObject);
    procedure cbFPGAProfileNameChange(Sender: TObject);
    procedure cbManualChange(Sender: TObject);
    procedure cbManualFormChange(Sender: TObject);
    procedure cbProfileNameCloseUp(Sender: TObject);
    procedure cbUploadCanUseMsgChange(Sender: TObject);
    procedure cbUploadConfigDig1Change(Sender: TObject);
    procedure cbUploadConfigDig1CloseUp(Sender: TObject);
    procedure cbUploadConfigDig2Change(Sender: TObject);
    procedure cbUploadConfigDig6Change(Sender: TObject);
    procedure cbUploadConfigErrorRVChange(Sender: TObject);
    procedure cbUploadConfigMasterErrorChange(Sender: TObject);
    procedure cbUploadConfigMasterModeChange(Sender: TObject);
    procedure cbUploadConfigOutputModeChange(Sender: TObject);
    procedure cbUploadConfigRS485FormatChange(Sender: TObject);
    procedure changeMfShape(Sender: TObject);
    procedure DrawGrid1Click(Sender: TObject);
    procedure edtFuzzy1RemoveRuleClick(Sender: TObject);
    procedure edtFuzzy2Input0Mf0ShapeChange(Sender: TObject);
    procedure edtFuzzy2Input1Mf2Pt0Change(Sender: TObject);
    procedure edtSetpointProfileChange(Sender: TObject);
    procedure edtUploadConfigCanBusChange(Sender: TObject);
    procedure edtUploadConfigMasterSetpointChange(Sender: TObject);
    procedure edtUploadConfigRVsetpointChange(Sender: TObject);
    procedure edtConfigManufacturingTypeChange(Sender: TObject);
    procedure edtUploadConfigSensorALowChange(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FuzzyAddRule(Sender: TObject);
    procedure FuzzyRemoveRule(Sender: TObject);
    procedure GroupBox20Click(Sender: TObject);
    procedure GroupBox24Click(Sender: TObject);
    procedure gbFuzzy1RuleClick(Sender: TObject);
    procedure imgIconDblClick(Sender: TObject);
    procedure Label124Click(Sender: TObject);
    procedure Label172Click(Sender: TObject);
    procedure Label17Click(Sender: TObject);
    procedure Label18Click(Sender: TObject);
    procedure Label309Click(Sender: TObject);
    procedure PageControl1Change(Sender: TObject);
    procedure PageControl6Change(Sender: TObject);
    procedure rgModeClick(Sender: TObject);
    procedure ShowHideMF();
    procedure rgUploadConfigSensorModeClick(Sender: TObject);
    procedure TabSheet14ContextPopup(Sender: TObject; MousePos: TPoint;
      var Handled: Boolean);
    procedure TabSheetFuzzy1MFContextPopup(Sender: TObject; MousePos: TPoint;
      var Handled: Boolean);
    procedure TabSheetFuzzy1RulesContextPopup(Sender: TObject;
      MousePos: TPoint; var Handled: Boolean);
    procedure TabSheetFuzzyChange(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure ReloadIniFile;
    procedure InitializeTest;
    procedure ProcessSerialReceive;
    procedure UpdateProfileInformation(ProfileData: TProfileData);
    procedure UpdateFPGAProfileInformation(ProfileData: TProfileData);
    procedure UpdateProfileData (ProfileData: TProfileData);
    procedure ProcessEEPROMState;
    procedure PowerControl (PowerState: boolean);
    procedure FindProfile (var EEPROMData: stEEPROMData; ProfileResults: TStringList);
    Function GeneratorFrequency : single;
    procedure AdjustFrequency (AdjAmount: single);
    procedure UpdateProfile(ProfileStr: string; ScaleFactor: single; ErrorLabel: Tlabel; ProfileGraph: TProfileGraph; var NewTable: stTwoPointTable);
    procedure SyncControls;
    procedure SaveToIniFile;
    procedure LoadFromGui;
    procedure UpdateGui;
    procedure LoadFactoryFromGui;
    procedure ProcessManual;
    procedure UploadFuzzyMFs(Sender: TObject);
    procedure ShowHideCanMsgOptions;


  private
    { private declarations }
  public
    { public declarations }
  end;

//**************************************************************************************************************************************

var
  Form1: TForm1;
  bVerifyProfile: boolean;
  SenseResistance: single;
  CDProfileData: stCDProfileData;
  qOld, qCurrent: int64;
  Fuzzy1RuleCount, Fuzzy2RuleCount: int32;

procedure TestMessage(MsgStr: string; Location: integer);

implementation

uses
    siglent;

{$R *.lfm}

{ TForm1 }

var
  bNext: boolean;
  bCalibrationComm: boolean;
  UploadVoltageResistanceProfileGraph: TProfileGraph;
  UploadMasterFreqProfileGraph: TProfileGraph;
  UploadCanBusProfileGraph: TProfileGraph;
  UploadAngularOutputProfileGraph: TProfileGraph;
const
  IndextoDIG1: array [0..7] of int32 = (0,1,3,4,5,6,7,10);
  IndextoDIG2: array [0..7] of int32 = (0,1,3,4,5,6,9,10);
  IndextoDIG3: array [0..6] of int32 = (0,1,3,4,5,6,10);
  IndextoDIG4: array [0..6] of int32 = (0,1,3,4,5,6,10);
  IndextoDIG5: array [0..4] of int32 = (0,4,6,8,10);
  IndextoDIG6: array [0..6] of int32 = (0,1,3,4,5,6,10);

  DIG1toIndex: array [0..12] of int32 = (0,1,0,2,3,4,5,6,0,0,7,0,0);
  DIG2toIndex: array [0..12] of int32 = (0,1,0,2,3,4,5,0,0,6,7,0,0);
  DIG3toIndex: array [0..12] of int32 = (0,1,0,2,3,4,5,0,0,0,6,0,0);
  DIG4toIndex: array [0..12] of int32 = (0,1,0,2,3,4,5,0,0,0,6,0,0);
  DIG5toIndex: array [0..12] of int32 = (0,0,0,0,1,0,2,0,3,0,4,0,0);
  DIG6toIndex: array [0..12] of int32 = (0,1,0,2,3,4,5,0,0,0,6,0,0);

  // Convert indices of baud rate to prescaler values. Only the prescaler
  // need to change to change the CAN Baud rate
  CanBaudtoPrescaler: array [0..8] of int32 = (300,150,60,36,30,24,12,6,3);

//******************************************************************************************************************

procedure TestMessage(MsgStr: string; Location: integer);
//var
//  qcounter: int64;
begin
//  QueryPerformanceCounter(qcounter);
//  MessageQueue[NextIn] := format('%5.3f: %s', [(qcounter - qstart) / qFreq, MsgStr]);

  MessageQueue[NextIn].MsgStr := MsgStr;
  MessageQueue[NextIn].Location := Location;
  NextIn := (NextIn + 1) and $FF;
end;

function GetIndex (item: integer; buffer: Pointer; len: integer) : integer;
var
  i: integer;
  DataPtr: ^int32;

begin

  DataPtr := buffer;

  if (len > 101) then Result := -1; // will cause errors which is the output if the length is greater than the pointer length

  for i := 0 to len do
    begin
    if Dataptr[i] = item then
      begin
      Result := i;
      end;
    end;
end;

//******************************************************************************************************************
//
//
function BooleanToInteger (value: boolean) : integer;
begin
  if value then Result := 1 else Result := 0;
end;

//******************************************************************************************************************
//
//
function IntegerToBoolean(int: integer) : boolean;
begin
  if int > 0 then Result := true else Result := false;
end;

//******************************************************************************************************************
//
//
function HexTextToInteger (const str: string) : integer;
begin
  // If string is empty, the hex output is 0
  if (str = '') then
    begin
    Result := 0;
    end
  else
    begin
    Result := StrToInt('$' + str);
    end;
end;

//******************************************************************************************************************
// Shows or hides CAN message options based on whether the message is enabled or disabled
//
procedure TForm1.ShowHideCanMsgOptions;
begin
  if (cbUploadCanUseMsgOut0.ItemIndex = 0) then
    begin
    edtUploadAddressMsgOut0.Enabled:=False;
    edtUploadFrequencyMsgOut0.Enabled:=False;
    end
  else
    begin
    edtUploadAddressMsgOut0.Enabled:=True;
    edtUploadFrequencyMsgOut0.Enabled:=True;
    end;
  if (cbUploadCanUseMsgOut1.ItemIndex = 0) then
    begin
    edtUploadAddressMsgOut1.Enabled:=False;
    edtUploadFrequencyMsgOut1.Enabled:=False;
    end
  else
    begin
    edtUploadAddressMsgOut1.Enabled:=True;
    edtUploadFrequencyMsgOut1.Enabled:=True;
    end;
  if (cbUploadCanUseMsgOut2.ItemIndex = 0) then
    begin
    edtUploadAddressMsgOut2.Enabled:=False;
    edtUploadFrequencyMsgOut2.Enabled:=False;
    edtUploadConfigAngularOutput.Enabled := False;
    end
  else
    begin
    edtUploadAddressMsgOut2.Enabled:=True;
    edtUploadFrequencyMsgOut2.Enabled:=True;
    edtUploadConfigAngularOutput.Enabled := True;
    end;
  if (cbUploadCanUseMsgOut3.ItemIndex = 0) then
    begin
    edtUploadAddressMsgOut3.Enabled:=False;
    end
  else
    begin
    edtUploadAddressMsgOut3.Enabled:=True;
    end;
  if (cbUploadCanUseMsgOut4.ItemIndex = 0) then
    begin
    edtUploadAddressMsgOut4.Enabled:=False;
    end
  else
    begin
    edtUploadAddressMsgOut4.Enabled:=True;
    end;
  if (cbUploadCanUseMsgOut5.ItemIndex = 0) then
    begin
    edtUploadAddressMsgOut5.Enabled:=False;
    end
  else
    begin
    edtUploadAddressMsgOut5.Enabled:=True;
    end;
  if (cbUploadCanUseMsgIn0.ItemIndex = 0) then
    begin
    edtUploadAddressMsgIn0.Enabled:=False;
    edtUploadFrequencyMsgIn0.Enabled:=False;
    edtUploadMaskMsgIn0.Enabled:=False;
    end
  else
    begin
    edtUploadAddressMsgIn0.Enabled:=True;
    edtUploadFrequencyMsgIn0.Enabled:=True;
    edtUploadMaskMsgIn0.Enabled:=True;
    end;
  if (cbUploadCanUseMsgIn1.ItemIndex = 0) then
    begin
    edtUploadAddressMsgIn1.Enabled:=False;
    edtUploadFrequencyMsgIn1.Enabled:=False;
    edtUploadMaskMsgIn1.Enabled:=False;
    end
  else
    begin
    edtUploadAddressMsgIn1.Enabled:=True;
    edtUploadFrequencyMsgIn1.Enabled:=True;
    edtUploadMaskMsgIn1.Enabled:=True;
    end;

end;

//**************************************************************************************************************************************

procedure TForm1.ReloadIniFile;
var
  IniFile: TIniFile;
begin
  IniFile := TIniFile.Create(AppPath + 'PROFILES_CC.INI');
  IniFile.ReadSection('PROFILES', cbProfileName.Items);
  IniFile.ReadSection('PROFILES', cbFPGAProfileName.Items);
  cbProfileName.ItemIndex := -1;
  cbFPGAProfileName.ItemIndex := -1;
  IniFile.Free;

  IniFile := TIniFile.Create(AppPath + 'PROFILES_CD.INI');
  IniFile.ReadSections(cbCDProfileNames.Items);
  IniFile.Free;
end;

//**************************************************************************************************************************************
//
// CC SERIES
//
// This code generates config and profile data based on the settings of the Profile Tab
//
procedure TForm1.UpdateProfileData (ProfileData: TProfileData);
var
   StrIndex: integer;
   TmpFloat: single;
begin
  TmpFloat := 0.0;
  ProfileData.EEPROM_Config := 0;
  if rgProfileType.ItemIndex = 0 then ProfileData.EEPROM_Config := ProfileData.EEPROM_Config or 1;
  if rgKmaDirection.ItemIndex = 1 then ProfileData.EEPROM_Config := ProfileData.EEPROM_Config or 2;
  if (cbTemperatureOutput.ItemIndex <> 0) or (cbHoldMode.ItemIndex <> 0) then ProfileData.EEPROM_Config := ProfileData.EEPROM_Config or 4;
  if (cbTemperatureOutput.ItemIndex = 2) or (cbHoldMode.ItemIndex = 2) then ProfileData.EEPROM_Config := ProfileData.EEPROM_Config or 8;
  if (cbHoldMode.ItemIndex <> 0) then ProfileData.EEPROM_Config := ProfileData.EEPROM_Config or $10;

  StrIndex := 1;
  MyStrToFloat(edtTripPoint.Text, StrIndex, TmpFloat);
  ProfileData.Trippoint := round(TmpFloat);

  StrIndex := 1;
  MyStrToFloat(edtHoldValue.Text, StrIndex, TmpFloat);
  ProfileData.HoldValue := round(TmpFloat);

  StrIndex := 1;
  MyStrToFloat(edtKmaSamples.Text, StrIndex, TmpFloat);
  ProfileData.KmaSamples := round(TmpFloat);
  //
  // Update Label to display any parsing errors
  //
  Label63.Caption := ProfileData.ParseProfileStr(edtSetpointProfile.Text);

end;

//**************************************************************************************************************************************

procedure TForm1.FormCreate(Sender: TObject);
var
IniFile: TIniFile;
i, j: integer;
TableResults: stTwoPointTable;
begin
  timeBeginPeriod(1);
  QueryPerformanceFrequency(qFreq);
  QueryPerformanceCounter(qStart);
  AppPath := ExtractFilePath(Application.ExeName);
  EventLogger := TEventLogger.Create('test.log');
  DataLog := TEventLogger.Create('data.log');

  SiglentThread := TSiglentThread.Create(false);
  SiglentThread.FreeOnTerminate := true;

  CDProfileData := stCDProfileData.Create;

  CurrentProfileData := TProfileData.create;
  CurrentProfileData.GraphImage := nil;

  NewProfileData := TProfileData.create;
  NewProfileData.GraphImage := Image2;

  FPGAProfileData := TProfileData.create;
  FPGAProfileData.GraphImage := Image3;

  IniFile := TIniFile.Create(AppPath + 'LIMITS.INI');
  edtNameOfTester.Text := IniFile.ReadString('SETUP', 'TESTER', '');
  edtSerialNumber.Text := IniFile.ReadString('SETUP', 'SN', '');
  IniFile.Free;

  UploadVoltageResistanceProfileGraph := TProfileGraph.Create;
  UploadVoltageResistanceProfileGraph.GraphImage := Image11;
  UploadVoltageResistanceProfileGraph.HorzHashMarks := 10;
  UploadVoltageResistanceProfileGraph.VertHashMarks := 10;
  UploadVoltageResistanceProfileGraph.VertTitle := 'INDEX';
  UploadVoltageResistanceProfileGraph.VertLowScale := 4;
  UploadVoltageResistanceProfileGraph.VertHighScale := 1023;
  UploadVoltageResistanceProfileGraph.VertPrecision := 1;
  UploadVoltageResistanceProfileGraph.HorzTitle := 'Setpoint (Ohms)';
  UploadVoltageResistanceProfileGraph.HorzLowScale := 0;
  UploadVoltageResistanceProfileGraph.HorzHighScale := 1000;
  UploadVoltageResistanceProfileGraph.HorzPrecision := 2;

  UploadMasterFreqProfileGraph := TProfileGraph.Create;
  UploadMasterFreqProfileGraph.GraphImage := Image12;
  UploadMasterFreqProfileGraph.HorzHashMarks := 10;
  UploadMasterFreqProfileGraph.VertHashMarks := 10;
  UploadMasterFreqProfileGraph.VertTitle := 'INDEX';
  UploadMasterFreqProfileGraph.VertLowScale := 4;
  UploadMasterFreqProfileGraph.VertHighScale := 1023;
  UploadMasterFreqProfileGraph.VertPrecision := 1;
  UploadMasterFreqProfileGraph.HorzTitle := 'FREQ (Hz)';
  UploadMasterFreqProfileGraph.HorzLowScale := 0;
  UploadMasterFreqProfileGraph.HorzHighScale := 1000;
  UploadMasterFreqProfileGraph.HorzPrecision := 2;

  UploadCanBusProfileGraph := TProfileGraph.Create;
  UploadCanBusProfileGraph.GraphImage := Image14;
  UploadCanBusProfileGraph.HorzHashMarks := 10;
  UploadCanBusProfileGraph.VertHashMarks := 10;
  UploadCanBusProfileGraph.VertTitle := 'INDEX';
  UploadCanBusProfileGraph.VertLowScale := 4;
  UploadCanBusProfileGraph.VertHighScale := 1023;
  UploadCanBusProfileGraph.VertPrecision := 1;
  UploadCanBusProfileGraph.HorzTitle := 'CanBus Output Value';
  UploadCanBusProfileGraph.HorzLowScale := 0;
  UploadCanBusProfileGraph.HorzHighScale := 255;
  UploadCanBusProfileGraph.HorzPrecision := 0;

  UploadAngularOutputProfileGraph := TProfileGraph.Create;
  UploadAngularOutputProfileGraph.GraphImage := Image15;
  UploadAngularOutputProfileGraph.HorzHashMarks := 10;
  UploadAngularOutputProfileGraph.VertHashMarks := 10;
  UploadAngularOutputProfileGraph.VertTitle := 'INDEX';
  UploadAngularOutputProfileGraph.VertLowScale := 4;
  UploadAngularOutputProfileGraph.VertHighScale := 1023;
  UploadAngularOutputProfileGraph.VertPrecision := 1;
  UploadAngularOutputProfileGraph.HorzTitle := 'Angular Output Value';
  UploadAngularOutputProfileGraph.HorzLowScale := 0;
  UploadAngularOutputProfileGraph.HorzHighScale := 255;
  UploadAngularOutputProfileGraph.HorzPrecision := 0;

  for i := 0 to 23 do
    begin
    ProcessTimingAverageTime[i] := TEdit.Create(Self);
    ProcessTimingAverageTime[i].Parent := ScrollBox1;
    ProcessTimingAverageTime[i].Width := 52;
    ProcessTimingAverageTime[i].Left := 260;
    ProcessTimingAverageTime[i].Top := 38 + (I * 22);
    ProcessTimingAverageTime[i].Font.Size := 8;
    ProcessTimingAverageTime[i].Alignment := taCenter;

    ProcessTimingMinTime[i] := TEdit.Create(Self);
    ProcessTimingMinTime[i].Parent := ScrollBox1;
    ProcessTimingMinTime[i].Width := 52;
    ProcessTimingMinTime[i].Left := 196;
    ProcessTimingMinTime[i].Top := 38 + (I * 22);
    ProcessTimingMinTime[i].Font.Size := 8;
    ProcessTimingMinTime[i].Alignment := taCenter;

    ProcessTimingMaxTime[i] := TEdit.Create(Self);
    ProcessTimingMaxTime[i].Parent := ScrollBox1;
    ProcessTimingMaxTime[i].Width := 52;
    ProcessTimingMaxTime[i].Left := 324;
    ProcessTimingMaxTime[i].Top := 38 + (I * 22);
    ProcessTimingMaxTime[i].Font.Size := 8;
    ProcessTimingMaxTime[i].Alignment := taCenter;

    ProcessTimingCalls[i] := TEdit.Create(Self);
    ProcessTimingCalls[i].Parent := ScrollBox1;
    ProcessTimingCalls[i].Width := 52;
    ProcessTimingCalls[i].Left := 388;
    ProcessTimingCalls[i].Top := 38 + (I * 22);
    ProcessTimingCalls[i].Font.Size := 8;
    ProcessTimingCalls[i].Alignment := taCenter;

    ProcessTimingLabel[i] := TLabel.Create(Self);
    ProcessTimingLabel[i].Parent := ScrollBox1;
    ProcessTimingLabel[i].Width := 180;
    ProcessTimingLabel[i].Left := 10;
    ProcessTimingLabel[i].Top := 40 + (I * 22);
    ProcessTimingLabel[i].Font.Size := 8;
    ProcessTimingLabel[i].Alignment := taRightJustify;
    ProcessTimingLabel[i].Caption := ProcessTimingLabels[i];
    ProcessTimingLabel[i].AutoSize := false;
    ProcessTimingLabel[i].Font.Style := [fsBold];

    end;

  for i := 0 to 27 do
    begin
    CheckBoxErrorCondition[i] := TCheckbox.Create(Self);
    CheckBoxErrorCondition[i].Parent := TabSheet17;
    CheckBoxErrorCondition[i].Left := 20 + ((i div 25) * 300);
    CheckBoxErrorCondition[i].Top := 20 + ((i mod 25) * 20);
    CheckBoxErrorCondition[i].Caption := GeneralErrors[i];
    end;

  for i := 0 to 15 do
    begin
    TroubleShootingTestDataLabel[i] := TLabel.Create(Self);
    TroubleShootingTestDataLabel[i].Parent := GroupBox2;
    TroubleShootingTestDataLabel[i].Width := 25;
    TroubleShootingTestDataLabel[i].Left := 2;
    TroubleShootingTestDataLabel[i].Top := 2 + (i * 20);
    TroubleShootingTestDataLabel[i].Font.Size := 8;
    TroubleShootingTestDataLabel[i].Alignment := taRightJustify;
    TroubleShootingTestDataLabel[i].Caption := format('[%d]', [i]);
    TroubleShootingTestDataLabel[i].AutoSize := false;
    TroubleShootingTestDataLabel[i].Font.Style := [fsBold];

    TroubleShootingTestDataEdit[i] := TEdit.Create(Self);
    TroubleShootingTestDataEdit[i].Parent := GroupBox2;
    TroubleShootingTestDataEdit[i].Width := 75;
    TroubleShootingTestDataEdit[i].Left := 35;
    TroubleShootingTestDataEdit[i].Top := 2 + (i * 20);
    TroubleShootingTestDataEdit[i].Font.Size := 8;
    TroubleShootingTestDataEdit[i].Alignment := taLeftJustify;
    TroubleShootingTestDataEdit[i].Font.Style := [];

    TroubleShootingAdcCountsLabel[i] := TLabel.Create(Self);
    TroubleShootingAdcCountsLabel[i].Parent := GroupBox3;
    TroubleShootingAdcCountsLabel[i].Width := 25;
    TroubleShootingAdcCountsLabel[i].Left := 2;
    TroubleShootingAdcCountsLabel[i].Top := 2 + (i * 20);
    TroubleShootingAdcCountsLabel[i].Font.Size := 8;
    TroubleShootingAdcCountsLabel[i].Alignment := taRightJustify;
    TroubleShootingAdcCountsLabel[i].Caption := format('[%d]', [i]);
    TroubleShootingAdcCountsLabel[i].AutoSize := false;
    TroubleShootingAdcCountsLabel[i].Font.Style := [fsBold];

    TroubleShootingAdcCountsEdit[i] := TEdit.Create(Self);
    TroubleShootingAdcCountsEdit[i].Parent := GroupBox3;
    TroubleShootingAdcCountsEdit[i].Width := 50;
    TroubleShootingAdcCountsEdit[i].Left := 35;
    TroubleShootingAdcCountsEdit[i].Top := 2 + (i * 20);
    TroubleShootingAdcCountsEdit[i].Font.Size := 8;
    TroubleShootingAdcCountsEdit[i].Alignment := taLeftJustify;
    TroubleShootingAdcCountsEdit[i].Font.Style := [];

    TroubleShootingEngUnitsLabel[i] := TLabel.Create(Self);
    TroubleShootingEngUnitsLabel[i].Parent := GroupBox4;
    TroubleShootingEngUnitsLabel[i].Width := 55;
    TroubleShootingEngUnitsLabel[i].Left := 2;
    TroubleShootingEngUnitsLabel[i].Top := 2 + (i * 20);
    TroubleShootingEngUnitsLabel[i].Font.Size := 8;
    TroubleShootingEngUnitsLabel[i].Alignment := taRightJustify;
    TroubleShootingEngUnitsLabel[i].Caption := TroubleShootingEngUnitsLabelArray[i];
    TroubleShootingEngUnitsLabel[i].AutoSize := false;
    TroubleShootingEngUnitsLabel[i].Font.Style := [fsBold];

    TroubleShootingEngUnitsEdit[i] := TEdit.Create(Self);
    TroubleShootingEngUnitsEdit[i].Parent := GroupBox4;
    TroubleShootingEngUnitsEdit[i].Width := 60;
    TroubleShootingEngUnitsEdit[i].Left := 61;
    TroubleShootingEngUnitsEdit[i].Top := 2 + (i * 20);
    TroubleShootingEngUnitsEdit[i].Font.Size := 8;
    TroubleShootingEngUnitsEdit[i].Alignment := taLeftJustify;
    TroubleShootingEngUnitsEdit[i].Font.Style := [];

    CalibrationAdcZeroLabel[i] := TLabel.Create(Self);
    CalibrationAdcZeroLabel[i].Parent := GroupBox6;
    CalibrationAdcZeroLabel[i].Width := 55;
    CalibrationAdcZeroLabel[i].Left := 2;
    CalibrationAdcZeroLabel[i].Top := 2 + (i * 20);
    CalibrationAdcZeroLabel[i].Font.Size := 8;
    CalibrationAdcZeroLabel[i].Alignment := taRightJustify;
    CalibrationAdcZeroLabel[i].Caption := TroubleShootingEngUnitsLabelArray[i];
    CalibrationAdcZeroLabel[i].AutoSize := false;
    CalibrationAdcZeroLabel[i].Font.Style := [fsBold];

    CalibrationAdcZeroEdit[i] := TEdit.Create(Self);
    CalibrationAdcZeroEdit[i].Parent := GroupBox6;
    CalibrationAdcZeroEdit[i].Width := 60;
    CalibrationAdcZeroEdit[i].Left := 61;
    CalibrationAdcZeroEdit[i].Top := 2 + (i * 20);
    CalibrationAdcZeroEdit[i].Font.Size := 8;
    CalibrationAdcZeroEdit[i].Alignment := taLeftJustify;
    CalibrationAdcZeroEdit[i].Font.Style := [];

    CalibrationAdcSpanLabel[i] := TLabel.Create(Self);
    CalibrationAdcSpanLabel[i].Parent := GroupBox7;
    CalibrationAdcSpanLabel[i].Width := 55;
    CalibrationAdcSpanLabel[i].Left := 2;
    CalibrationAdcSpanLabel[i].Top := 2 + (i * 20);
    CalibrationAdcSpanLabel[i].Font.Size := 8;
    CalibrationAdcSpanLabel[i].Alignment := taRightJustify;
    CalibrationAdcSpanLabel[i].Caption := TroubleShootingEngUnitsLabelArray[i];
    CalibrationAdcSpanLabel[i].AutoSize := false;
    CalibrationAdcSpanLabel[i].Font.Style := [fsBold];

    CalibrationAdcSpanEdit[i] := TEdit.Create(Self);
    CalibrationAdcSpanEdit[i].Parent := GroupBox7;
    CalibrationAdcSpanEdit[i].Width := 60;
    CalibrationAdcSpanEdit[i].Left := 61;
    CalibrationAdcSpanEdit[i].Top := 2 + (i * 20);
    CalibrationAdcSpanEdit[i].Font.Size := 8;
    CalibrationAdcSpanEdit[i].Alignment := taLeftJustify;
    CalibrationAdcSpanEdit[i].Font.Style := [];
    end;


  memo1.Clear;

  EventLogger.WriteStr(' ', False);
  EventLogger.WriteStr('=================================================================================================', False);
  EventLogger.WriteStr('--------------------------  JET FUEL TRANSDUCER PROGRAMMER STARTED  ---------------------------', False);
  EventLogger.WriteStr('=================================================================================================', False);
  EventLogger.WriteStr(' ', False);

  EventLogger.WriteStr('Reading INI Files', True);
  ReloadIniFile();

  GenerateCommPortList();
  CommPort := TCommPort.Create;
  SerialThread := TSerialThread.Create(False);

  Form1.Top := 0;
  Form1.Left := 0;
  InitializeTest();

  UpdateProfileData(NewProfileData);
  NewProfileData.Redraw;

  Label1.Visible := false;
  Label2.Visible := false;
  edtNameofTester.Visible := false;
  edtSerialNumber.Visible := false;

  TabSheet1.TabVisible := true;
  TabSheet2.TabVisible := false;
  TabSheet4.TabVisible := false;
  TabSheet5.TabVisible := true;
  TabSheet7.TabVisible := false;
  TabSheet8.TabVisible:= false;
  TabSheet10.TabVisible:= false;
  TabSheet3.TabVisible := false;

  SetpointData.CommConfig := COMM_CC;

  ScrollBox3.HorzScrollBar.Position := 0;

  PageControl1.TabIndex := 0;

  UpdateProfile(edtUploadConfigMasterSetpoint.Text, 100.0, Label250, UploadMasterFreqProfileGraph, TableResults);
  UpdateProfile(edtUploadConfigRVsetpoint.Text, 1000.0, Label245, UploadVoltageResistanceProfileGraph, TableResults);
  UpdateProfile(edtUploadConfigCanBus.Text, 100.0, Label260, UploadCanBusProfileGraph, TableResults);
  UpdateProfile(edtUploadConfigAngularOutput.Text, 1.0, Label263, UploadAngularOutputProfileGraph, TableResults);

  shtMasterFreqProfile2.TabVisible := false;

  // Format CAN Message options
  ShowHideCanMsgOptions;

end;

//**************************************************************************************************************************************

procedure TForm1.FormDestroy(Sender: TObject);
var
  IniFile: TIniFile;
begin
  timeBeginPeriod(1);
  EventLogger.WriteStr(' ', False);
  EventLogger.WriteStr('=================================================================================================', False);
  EventLogger.WriteStr('--------------------------  JET FUEL TRANSDUCER TEST PROGRAM CLOSED  ----------------------------', False);
  EventLogger.WriteStr('=================================================================================================', False);
  EventLogger.WriteStr(' ', False);
  EventLogger.Refresh;
  EventLogger.Free;
  DataLog.Free;
  SerialThread.Terminate;
  Sleep(100);
  SerialThread.Free;

  IniFile := TIniFile.Create(AppPath + 'LIMITS.INI');
  IniFile.WriteString('SETUP', 'TESTER', edtNameOfTester.Text);
  IniFile.WriteString('SETUP', 'SN', edtSerialNumber.Text);
  IniFile.Free;

end;

//******************************************************************************************************************

Function ConvertToFloat (FloatStr: string; ScaleFactor: single) : LongInt;
var
  StrIndex: integer;
  TmpFloat: single;
begin
  StrIndex := 1;
  MyStrToFloat(FloatStr, StrIndex, TmpFloat);
  Result := round(TmpFloat * ScaleFactor);
end;


Procedure TForm1.ShowHideMF ();
begin

  if edtFuzzy1Input0Mf0Shape.ItemIndex = 0 then edtFuzzy1Input0Mf0Pt3.Visible := false else edtFuzzy1Input0Mf0Pt3.Visible := true;
  if edtFuzzy1Input0Mf1Shape.ItemIndex = 0 then edtFuzzy1Input0Mf1Pt3.Visible := false else edtFuzzy1Input0Mf1Pt3.Visible := true;
  if edtFuzzy1Input0Mf2Shape.ItemIndex = 0 then edtFuzzy1Input0Mf2Pt3.Visible := false else edtFuzzy1Input0Mf2Pt3.Visible := true;

  if edtFuzzy1Input1Mf0Shape.ItemIndex = 0 then edtFuzzy1Input1Mf0Pt3.Visible := false else edtFuzzy1Input1Mf0Pt3.Visible := true;
  if edtFuzzy1Input1Mf1Shape.ItemIndex = 0 then edtFuzzy1Input1Mf1Pt3.Visible := false else edtFuzzy1Input1Mf1Pt3.Visible := true;
  if edtFuzzy1Input1Mf2Shape.ItemIndex = 0 then edtFuzzy1Input1Mf2Pt3.Visible := false else edtFuzzy1Input1Mf2Pt3.Visible := true;

  if edtFuzzy1Input2Mf0Shape.ItemIndex = 0 then edtFuzzy1Input2Mf0Pt3.Visible := false else edtFuzzy1Input2Mf0Pt3.Visible := true;
  if edtFuzzy1Input2Mf1Shape.ItemIndex = 0 then edtFuzzy1Input2Mf1Pt3.Visible := false else edtFuzzy1Input2Mf1Pt3.Visible := true;
  if edtFuzzy1Input2Mf2Shape.ItemIndex = 0 then edtFuzzy1Input2Mf2Pt3.Visible := false else edtFuzzy1Input2Mf2Pt3.Visible := true;

  if edtFuzzy1OutputMf0Shape.ItemIndex = 0 then edtFuzzy1OutputMf0Pt3.Visible := false else edtFuzzy1OutputMf0Pt3.Visible := true;
  if edtFuzzy1OutputMf1Shape.ItemIndex = 0 then edtFuzzy1OutputMf1Pt3.Visible := false else edtFuzzy1OutputMf1Pt3.Visible := true;
  if edtFuzzy1OutputMf2Shape.ItemIndex = 0 then edtFuzzy1OutputMf2Pt3.Visible := false else edtFuzzy1OutputMf2Pt3.Visible := true;

  if edtFuzzy1Input0Mf0Shape.ItemIndex = 0 then Label23.Visible := false else Label23.Visible := true;
  if edtFuzzy1Input0Mf1Shape.ItemIndex = 0 then Label28.Visible := false else Label28.Visible := true;
  if edtFuzzy1Input0Mf2Shape.ItemIndex = 0 then Label33.Visible := false else Label33.Visible := true;

  if edtFuzzy1Input1Mf0Shape.ItemIndex = 0 then Label38.Visible := false else Label38.Visible := true;
  if edtFuzzy1Input1Mf1Shape.ItemIndex = 0 then Label75.Visible := false else Label75.Visible := true;
  if edtFuzzy1Input1Mf2Shape.ItemIndex = 0 then Label95.Visible := false else Label95.Visible := true;

  if edtFuzzy1Input2Mf0Shape.ItemIndex = 0 then Label43.Visible := false else Label43.Visible := true;
  if edtFuzzy1Input2Mf1Shape.ItemIndex = 0 then Label85.Visible := false else Label85.Visible := true;
  if edtFuzzy1Input2Mf2Shape.ItemIndex = 0 then Label100.Visible := false else Label100.Visible := true;

  if edtFuzzy1OutputMf0Shape.ItemIndex = 0 then Label48.Visible := false else Label48.Visible := true;
  if edtFuzzy1OutputMf1Shape.ItemIndex = 0 then Label90.Visible := false else Label90.Visible := true;
  if edtFuzzy1OutputMf2Shape.ItemIndex = 0 then Label135.Visible := false else Label135.Visible := true;


  if edtFuzzy2Input0Mf0Shape.ItemIndex = 0 then edtFuzzy2Input0Mf0Pt3.Visible := false else edtFuzzy2Input0Mf0Pt3.Visible := true;
  if edtFuzzy2Input0Mf1Shape.ItemIndex = 0 then edtFuzzy2Input0Mf1Pt3.Visible := false else edtFuzzy2Input0Mf1Pt3.Visible := true;
  if edtFuzzy2Input0Mf2Shape.ItemIndex = 0 then edtFuzzy2Input0Mf2Pt3.Visible := false else edtFuzzy2Input0Mf2Pt3.Visible := true;

  if edtFuzzy2Input1Mf0Shape.ItemIndex = 0 then edtFuzzy2Input1Mf0Pt3.Visible := false else edtFuzzy2Input1Mf0Pt3.Visible := true;
  if edtFuzzy2Input1Mf1Shape.ItemIndex = 0 then edtFuzzy2Input1Mf1Pt3.Visible := false else edtFuzzy2Input1Mf1Pt3.Visible := true;
  if edtFuzzy2Input1Mf2Shape.ItemIndex = 0 then edtFuzzy2Input1Mf2Pt3.Visible := false else edtFuzzy2Input1Mf2Pt3.Visible := true;

  if edtFuzzy2Input2Mf0Shape.ItemIndex = 0 then edtFuzzy2Input2Mf0Pt3.Visible := false else edtFuzzy2Input2Mf0Pt3.Visible := true;
  if edtFuzzy2Input2Mf1Shape.ItemIndex = 0 then edtFuzzy2Input2Mf1Pt3.Visible := false else edtFuzzy2Input2Mf1Pt3.Visible := true;
  if edtFuzzy2Input2Mf2Shape.ItemIndex = 0 then edtFuzzy2Input2Mf2Pt3.Visible := false else edtFuzzy2Input2Mf2Pt3.Visible := true;

  if edtFuzzy2OutputMf0Shape.ItemIndex = 0 then edtFuzzy2OutputMf0Pt3.Visible := false else edtFuzzy2OutputMf0Pt3.Visible := true;
  if edtFuzzy2OutputMf1Shape.ItemIndex = 0 then edtFuzzy2OutputMf1Pt3.Visible := false else edtFuzzy2OutputMf1Pt3.Visible := true;
  if edtFuzzy2OutputMf2Shape.ItemIndex = 0 then edtFuzzy2OutputMf2Pt3.Visible := false else edtFuzzy2OutputMf2Pt3.Visible := true;

  if edtFuzzy2Input0Mf0Shape.ItemIndex = 0 then Label143.Visible := false else Label143.Visible := true;
  if edtFuzzy2Input0Mf1Shape.ItemIndex = 0 then Label148.Visible := false else Label148.Visible := true;
  if edtFuzzy2Input0Mf2Shape.ItemIndex = 0 then Label153.Visible := false else Label153.Visible := true;

  if edtFuzzy2Input1Mf0Shape.ItemIndex = 0 then Label158.Visible := false else Label158.Visible := true;
  if edtFuzzy2Input1Mf1Shape.ItemIndex = 0 then Label163.Visible := false else Label163.Visible := true;
  if edtFuzzy2Input1Mf2Shape.ItemIndex = 0 then Label176.Visible := false else Label176.Visible := true;

  if edtFuzzy2Input2Mf0Shape.ItemIndex = 0 then Label182.Visible := false else Label182.Visible := true;
  if edtFuzzy2Input2Mf1Shape.ItemIndex = 0 then Label187.Visible := false else Label187.Visible := true;
  if edtFuzzy2Input2Mf2Shape.ItemIndex = 0 then Label194.Visible := false else Label194.Visible := true;

  if edtFuzzy2OutputMf0Shape.ItemIndex = 0 then Label199.Visible := false else Label199.Visible := true;
  if edtFuzzy2OutputMf1Shape.ItemIndex = 0 then Label204.Visible := false else Label204.Visible := true;
  if edtFuzzy2OutputMf2Shape.ItemIndex = 0 then Label209.Visible := false else Label209.Visible := true;


end;

//
// Outputs the membership function selected as a text to be used in FuzzyRulesToText
//
Function InputsToMFText (Input: int32) : string;
var
  TmpStr: string;
  NotStr: string;
begin
  if Input = 0 then
    begin
      TmpStr := '';
    end
  else
    begin
      if Abs(Input) = 1 then
        begin
          TmpStr := 'MF0';
        end
      else
        begin
          if Abs(Input) = 2 then
            begin
              TmpStr := 'MF1';
            end
          else
            begin
              TmpStr := 'MF2';
            end;
        end;
    end;

  if Input < 0 then
    begin
      NotStr := 'not '
    end
  else
    begin
      NotStr := '';
    end;

  Result := NotStr + TmpStr;
end;

//
// Converts Rules selected into text for the memo box
//
Function FuzzyRulesToText (RuleCount: int32; Input0: int32; Input1: int32; Input2: int32; Output: int32; Connection: int32; Weight: int32) : string;
var
  TmpStr0: string;
  TmpStr1: string;
  TmpStr2: string;
  andor: string;
begin
  TmpStr0 := InputsToMFText(Input0);
  TmpStr1 := InputsToMFText(Input1);
  TmpStr2 := InputsToMFText(Input2);

  if Connection = 1 then
    begin
      andor := 'and';
    end
  else
    begin
      andor:= 'or';
    end;

  Result := format('%d', [RuleCount]) + ') ' + 'If ' + 'Input 1 is: ' + TmpStr0 + ' ' + andor + ' Input 2 is: ' + TmpStr1 + ' ' + andor +
  ' Input 3 is: ' + TmpStr2 + ' then the output is: ' + InputsToMFText(Output) + ' (' + format('%d', [Weight]) + ')';
end;

//
// Compares InX and CompX to and returns 1 if X = 0 to 5 match and 0 if one or more do not match
//
Function RuleMatch(In0: int32; Comp0: int32; In1: int32; Comp1: int32; In2: int32; Comp2: int32; In3: int32; Comp3: int32; In4: int32; Comp4: int32; In5: int32; Comp5: int32) : int32;
begin

  if (In0 = Comp0) and (In1 = Comp1) and (In2 = Comp2) and (In3 = Comp3) and (In4 = Comp4) and (In5 = Comp5) then
    begin
      Result := 1;
    end
  else
    begin
      Result := 0;
    end;

end;

//
// Depending on whether the Fuzzy1Rules or Fuzzy2 Rules tabsheet is open,
// This function will store data from the added rules
//
procedure TForm1.FuzzyAddRule(Sender: TObject);
var
  i: integer;
begin
  if TabSheetFuzzy.TabIndex = 2 then
    begin
      // If not is clicked then it becomes the negative ItemIndex value
      Fuzzy1RuleCount := Fuzzy1RuleCount + 1;
      if Fuzzy1RuleCount = 16 then
        begin
          Fuzzy1RuleCount := 15;
          Fuzzy1RuleMemo.Lines.Delete(14);
        end;

      CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[Fuzzy1RuleCount - 1].Input0 := cbFuzzy1Input0RuleValue.ItemIndex + (-2 * BooleanToInteger(cbFuzzy1Input0Not.Checked) * cbFuzzy1Input0RuleValue.ItemIndex);
      CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[Fuzzy1RuleCount - 1].Input1 := cbFuzzy1Input1RuleValue.ItemIndex + (-2 * BooleanToInteger(cbFuzzy1Input1Not.Checked) * cbFuzzy1Input1RuleValue.ItemIndex);
      CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[Fuzzy1RuleCount - 1].Input2 := cbFuzzy1Input2RuleValue.ItemIndex + (-2 * BooleanToInteger(cbFuzzy1Input2Not.Checked) * cbFuzzy1Input2RuleValue.ItemIndex);
      CDProfileData.Fuzzy1Data.FuzzyOutputRules.OutputRule[Fuzzy1RuleCount - 1] := cbFuzzy1OutputRuleValue.ItemIndex;
      CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleType[Fuzzy1RuleCount - 1] := rgFuzzy1RuleType.ItemIndex;
      CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleWeight[Fuzzy1RuleCount - 1] := ConvertToFloat(edtFuzzy1RuleWeight.Text, 1.0);
      CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleCount := Fuzzy1RuleCount;

      Fuzzy1RuleMemo.Lines.Clear;

      for i := 0 to Fuzzy1RuleCount - 1 do
        begin
          Fuzzy1RuleMemo.Lines.Add(FuzzyRulesToText(i+1, CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[i].Input0, CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[i].Input1, CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[i].Input2,
          CDProfileData.Fuzzy1Data.FuzzyOutputRules.OutputRule[i], CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleType[i], CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleWeight[i]));
        end;

      lblFuzzy1NumberRules.Caption:= format('%d', [Fuzzy1RuleCount]);

    end
  else
    begin
      if TabSheetFuzzy.TabIndex = 3 then
        begin
          // If not is clicked then it becomes the negative ItemIndex value
          Fuzzy2RuleCount := Fuzzy2RuleCount + 1;
          if Fuzzy2RuleCount = 16 then
            begin
              Fuzzy2RuleCount := 15;
              Fuzzy2RuleMemo.Lines.Delete(14);
            end;
          CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[Fuzzy2RuleCount - 1].Input0 := cbFuzzy2Input0RuleValue.ItemIndex + (-2 * BooleanToInteger(cbFuzzy2Input0Not.Checked) * cbFuzzy2Input0RuleValue.ItemIndex);
          CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[Fuzzy2RuleCount - 1].Input1 := cbFuzzy2Input1RuleValue.ItemIndex + (-2 * BooleanToInteger(cbFuzzy2Input1Not.Checked) * cbFuzzy2Input1RuleValue.ItemIndex);
          CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[Fuzzy2RuleCount - 1].Input2 := cbFuzzy2Input2RuleValue.ItemIndex + (-2 * BooleanToInteger(cbFuzzy2Input2Not.Checked) * cbFuzzy2Input2RuleValue.ItemIndex);
          CDProfileData.Fuzzy2Data.FuzzyOutputRules.OutputRule[Fuzzy2RuleCount - 1] := cbFuzzy2OutputRuleValue.ItemIndex;
          CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleType[Fuzzy2RuleCount - 1] := rgFuzzy2RuleType.ItemIndex;
          CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleWeight[Fuzzy2RuleCount - 1] := ConvertToFloat(edtFuzzy2RuleWeight.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleCount := Fuzzy2RuleCount;

          Fuzzy2RuleMemo.Lines.Clear;

          for i := 0 to Fuzzy2RuleCount - 1 do
            begin
              Fuzzy2RuleMemo.Lines.Add(FuzzyRulesToText(i+1, CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[i].Input0, CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[i].Input1, CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[i].Input2,
              CDProfileData.Fuzzy2Data.FuzzyOutputRules.OutputRule[i], CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleType[i], CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleWeight[i]));
            end;
          lblFuzzy2NumberRules.Caption:= format('%d', [Fuzzy2RuleCount]);
        end;
    end;
end;
//
// Runs if the Remove Rule btn is clicked:
// Changes the Fuzzy Rule data structures if rules are removed
// and prints out the updated rule set in the memo
//
procedure TForm1.FuzzyRemoveRule(Sender: TObject);
 var
  Input0: int32;
  Input1: int32;
  Input2: int32;
  Output: int32;
  RuleType: int32;
  RuleWeight: int32;
  i,j : integer;
  LinesKept: array [0..14] of int32 = (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

begin
  if TabSheetFuzzy.TabIndex = 2 then
    begin
      Input0 := cbFuzzy1Input0RuleValue.ItemIndex + (-2 * BooleanToInteger(cbFuzzy1Input0Not.Checked) * cbFuzzy1Input0RuleValue.ItemIndex);
      Input1 := cbFuzzy1Input1RuleValue.ItemIndex + (-2 * BooleanToInteger(cbFuzzy1Input1Not.Checked) * cbFuzzy1Input1RuleValue.ItemIndex);
      Input2 := cbFuzzy1Input2RuleValue.ItemIndex + (-2 * BooleanToInteger(cbFuzzy1Input2Not.Checked) * cbFuzzy1Input2RuleValue.ItemIndex);
      Output := cbFuzzy1OutputRuleValue.ItemIndex;
      RuleWeight := ConvertToFloat(edtFuzzy1RuleWeight.Text, 1.0);
      RuleType := rgFuzzy1RuleType.ItemIndex;

      for i := Fuzzy1RuleMemo.Lines.Count downto 0 do
        begin
          if IntegerToBoolean(RuleMatch(Input0, CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[i].Input0, Input1, CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[i].Input1,
          Input2, CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[i].Input2, Output, CDProfileData.Fuzzy1Data.FuzzyOutputRules.OutputRule[i], RuleType, CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleType[i], RuleWeight, CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleWeight[i])) then
            begin
              if (Fuzzy1RuleCount >0) then Fuzzy1RuleCount := Fuzzy1RuleCount -1 else Fuzzy1RuleCount := 0;
            end
          else
            begin
              LinesKept[i] := 1;
            end;
        end;
      j := 0;
      for i := 0 to 14 do
        begin
          if LinesKept[i] = 1 then
            begin
              CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[j].Input0 := CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[i].Input0;
              CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[j].Input1 := CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[i].Input1;
              CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[j].Input2 := CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[i].Input2;
              CDProfileData.Fuzzy1Data.FuzzyOutputRules.OutputRule[j] := CDProfileData.Fuzzy1Data.FuzzyOutputRules.OutputRule[i];
              CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleWeight[j] := CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleWeight[i];
              CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleType[j] := CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleType[i];
              j := j + 1;
            end;
        end;
      CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleCount := Fuzzy1RuleCount;

      Fuzzy1RuleMemo.Lines.Clear;

      for i := 0 to Fuzzy1RuleCount - 1 do
        begin
          Fuzzy1RuleMemo.Lines.Add(FuzzyRulesToText(i+1, CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[i].Input0, CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[i].Input1, CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[i].Input2,
          CDProfileData.Fuzzy1Data.FuzzyOutputRules.OutputRule[i], CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleType[i], CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleWeight[i]));
        end;
      lblFuzzy1NumberRules.Caption:= format('%d', [Fuzzy1RuleCount]);

    end
  else
    begin
      if TabSheetFuzzy.TabIndex = 3 then
        begin
          Input0 := cbFuzzy2Input0RuleValue.ItemIndex + (-2 * BooleanToInteger(cbFuzzy2Input0Not.Checked) * cbFuzzy2Input0RuleValue.ItemIndex);
          Input1 := cbFuzzy2Input1RuleValue.ItemIndex + (-2 * BooleanToInteger(cbFuzzy2Input1Not.Checked) * cbFuzzy2Input1RuleValue.ItemIndex);
          Input2 := cbFuzzy2Input2RuleValue.ItemIndex + (-2 * BooleanToInteger(cbFuzzy2Input2Not.Checked) * cbFuzzy2Input2RuleValue.ItemIndex);
          Output := cbFuzzy2OutputRuleValue.ItemIndex;
          RuleWeight := ConvertToFloat(edtFuzzy2RuleWeight.Text, 1.0);
          RuleType := rgFuzzy2RuleType.ItemIndex;

          for i := Fuzzy2RuleMemo.Lines.Count downto 0 do
            begin
              if IntegerToBoolean(RuleMatch(Input0, CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[i].Input0, Input1, CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[i].Input1,
              Input2, CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[i].Input2, Output, CDProfileData.Fuzzy2Data.FuzzyOutputRules.OutputRule[i], RuleType, CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleType[i], RuleWeight, CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleWeight[i])) then
                begin
                  if (Fuzzy2RuleCount >0) then Fuzzy2RuleCount := Fuzzy2RuleCount -1 else Fuzzy2RuleCount := 0;
                end
              else
                begin
                  LinesKept[i] := 1;
                end;
            end;
          j := 0;
          for i := 0 to 14 do
            begin
              if LinesKept[i] = 1 then
                begin
                  CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[j].Input0 := CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[i].Input0;
                  CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[j].Input1 := CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[i].Input1;
                  CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[j].Input2 := CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[i].Input2;
                  CDProfileData.Fuzzy2Data.FuzzyOutputRules.OutputRule[j] := CDProfileData.Fuzzy2Data.FuzzyOutputRules.OutputRule[i];
                  CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleWeight[j] := CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleWeight[i];
                  CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleType[j] := CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleType[i];
                  j := j + 1;
                end;
            end;
          CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleCount := Fuzzy2RuleCount;
          Fuzzy2RuleMemo.Lines.Clear;

          for i := 0 to Fuzzy2RuleCount - 1 do
            begin
              Fuzzy2RuleMemo.Lines.Add(FuzzyRulesToText(i+1, CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[i].Input0, CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[i].Input1, CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[i].Input2,
              CDProfileData.Fuzzy2Data.FuzzyOutputRules.OutputRule[i], CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleType[i], CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleWeight[i]));
            end;
          lblFuzzy2NumberRules.Caption:= format('%d', [Fuzzy1RuleCount]);

          end;
    end;
end;

procedure TForm1.GroupBox20Click(Sender: TObject);
begin

end;

procedure TForm1.GroupBox24Click(Sender: TObject);
begin

end;

procedure TForm1.gbFuzzy1RuleClick(Sender: TObject);
begin

end;

//******************************************************************************************************
//******************************************************************************************************
//
// CC SERIES
//
// This routine steps through all of the known Profiles in PROFILES_CC.INI, looking for matches to profile
// data just downloaded from Sender.
//
procedure TForm1.FindProfile (var EEPROMData: stEEPROMData; ProfileResults: TStringList);
var
  k, i: integer;
  ProfileList: TStringList;
  IniFile: TIniFile;
  bMatch: boolean;
  ProfileData: TProfileData;
begin
  ProfileResults.Clear;
  ProfileList := TStringList.Create;
  ProfileData := TProfileData.create;

  IniFile := TIniFile.Create(AppPath + 'PROFILES_CC.INI');
  IniFile.ReadSection('PROFILES', ProfileList);
  IniFile.free;

  for k := 0 to ProfileList.Count - 1 do
    begin
    ProfileData.ParseCompleteProfileStr(ProfileList.Strings[k]);

    if ProfileData.EEPROM_Config <> EEPROMData.EEPROM_Config then continue;
    //
    // $14 = Hold Mode.  When Hold Mode is enabled, the Trip Point and Hold Values can be +/-1 count off from
    //    the values stored in the Profile due to rounding errors.
    //
    if (EEPROMData.EEPROM_Config and $14) = $14 then
      begin
      if abs(ProfileData.Trippoint - EEPROMData.Trippoint) > 1 then continue;
      if abs(ProfileData.HoldValue - EEPROMData.HoldValue) > 1 then continue;
      end
    else
      begin
      if ProfileData.Trippoint <> EEPROMData.Trippoint then continue;
      if ProfileData.HoldValue <> EEPROMData.HoldValue then continue;
      end;
    if ProfileData.KmaSamples <> EEPROMData.KmaSamples then continue;

    bMatch := TRUE;
    for i := 0 to 509 do
      begin
      if ProfileData.Profile[i] <> EEPROMData.Profile[i] then
        begin
        bMatch := false;
        break;
        end;
      end;
    if bMatch then
      ProfileResults.Add(ProfileList.Strings[k]);
    end;
  ProfileList.Free;
  ProfileData.free;
end;

//******************************************************************************************************
//******************************************************************************************************

procedure TForm1.ProcessSerialReceive;
var
  Entries, StartingIndex, i: integer;
  Temp, TmpFreq: single;
  TmpStr: string;
  ProfileList: TStringList;
label
  ExitHere;
begin

  if bSysVarsUpdate then
    begin
    bSysVarsUpdate := false;

    cbDigIn1.ItemIndex := (SysVars.DigIO_InputLevel shr 0) and 1;
    cbDigIn2.ItemIndex := (SysVars.DigIO_InputLevel shr 1) and 1;
    cbDigIn3.ItemIndex := (SysVars.DigIO_InputLevel shr 2) and 1;
    cbDigIn4.ItemIndex := (SysVars.DigIO_InputLevel shr 3) and 1;
    cbDigIn5.ItemIndex := (SysVars.DigIO_InputLevel shr 4) and 1;
    cbDigIn6.ItemIndex := (SysVars.DigIO_InputLevel shr 5) and 1;

    for i := 0 to 15 do
      begin
      TroubleShootingTestDataEdit[i].Text := format('%d', [SysVars.TestData[i]]);
      TroubleShootingAdcCountsEdit[i].Text := format('%d', [SysVars.AdcCounts[i]]);
      TroubleShootingEngUnitsEdit[i].Text := format('%1.3f', [SysVars.AdcVolts[i] / 1000.0]);
      end;

    edtVoltsGreen.text := format('%1.3f', [SysVars.AdcVolts[10] / 1000.0]);

    lePowerVoltage.text := format('%1.2f', [SysVars.AdcVolts[14] / 1000.0]);
    lePowerCurrent.text := format('%1.2f', [SysVars.PS_Current / 100.0]);    // Power Supply Current
    leRVvoltage.text := format('%1.3f', [SysVars.AdcVolts[10] / 1000.0]);
    leRVResistance.text := format('%1.2f', [SysVars.RV_Ohms / 100.0]);

    if DUT_InvalidCounter <= 20 then
      begin
      cbMasterEnable.Checked := (SysVarsCC.DUT_Status and (1 shl 0)) <> 0;
      cbNoSlave1Input.Checked := (SysVarsCC.DUT_Status and (1 shl 1)) <> 0;
      cbNoSlave2Input.Checked := (SysVarsCC.DUT_Status and (1 shl 2)) <> 0;
      cbNoSlave3Input.Checked := (SysVarsCC.DUT_Status and (1 shl 3)) <> 0;
      cbSerialVerify.Checked := (SysVarsCC.DUT_Status and (1 shl 5)) <> 0;
      cbRVVoltage.Checked := (SysVarsCC.DUT_Status and (1 shl 7)) <> 0;
      cbRvResistance.Checked := (SysVarsCC.DUT_Status and (1 shl 7)) = 0;
      cbKmaReverse.Checked := (SysVarsCC.DUT_Status and (1 shl 8)) <> 0;
      cbLowHoldEnabled.Checked := (SysVarsCC.DUT_Status and (1 shl 9)) <> 0;
      if not cbLowHoldEnabled.Checked then
        begin
        cbLowFuelSelected.Checked := FALSE;
        cbLowFuelSelected.Enabled := FALSE;
        cbHoldLogicSelected.Checked := FALSE;
        cbHoldLogicSelected.Enabled := FALSE;
        cbLowFuelActiveHigh.Checked := FALSE;
        cbLowFuelActiveHigh.Enabled := FALSE;
        cbLowFuelActiveLow.Checked := FALSE;
        cbLowFuelActiveLow.Enabled := FALSE;
        cbHoldGreater.Checked := FALSE;
        cbHoldGreater.Enabled := FALSE;
        cbHoldLessThan.Checked := FALSE;
        cbHoldLessThan.Enabled := FALSE;
        end
      else
        begin
        cbLowFuelSelected.Checked := (SysVarsCC.DUT_Status and (1 shl 11)) = 0;
        cbLowFuelSelected.Enabled := TRUE;

        cbHoldLogicSelected.Checked := (SysVarsCC.DUT_Status and (1 shl 11)) <> 0;
        cbHoldLogicSelected.Enabled := TRUE;

        if not cbLowFuelSelected.Checked then
          begin
          cbLowFuelActiveHigh.Checked := FALSE;
          cbLowFuelActiveHigh.Enabled := FALSE;
          cbLowFuelActiveLow.Checked := FALSE;
          cbLowFuelActiveLow.Enabled := FALSE;
          end
        else
          begin
          cbLowFuelActiveHigh.Checked := (SysVarsCC.DUT_Status and (1 shl 10)) = 0;
          cbLowFuelActiveHigh.Enabled := TRUE;
          cbLowFuelActiveLow.Checked := (SysVarsCC.DUT_Status and (1 shl 10)) <> 0;
          cbLowFuelActiveLow.Enabled := TRUE;
          end;

        if not cbHoldLogicSelected.Checked then
          begin
          cbHoldGreater.Checked := FALSE;
          cbHoldGreater.Enabled := FALSE;
          cbHoldLessThan.Checked := FALSE;
          cbHoldLessThan.Enabled := FALSE;
          end
        else
          begin
          cbHoldGreater.Checked := (SysVarsCC.DUT_Status and (1 shl 10)) = 0;
          cbHoldGreater.Enabled := TRUE;
          cbHoldLessThan.Checked := (SysVarsCC.DUT_Status and (1 shl 10)) <> 0;
          cbHoldLessThan.Enabled := TRUE;
          end;
        end;

      leKmaCounts.Text := format('%u', [SysVarsCC.DUT_KMA_Counts]);

      Temp := 251 - ((SysVarsCC.DUT_AD7814_Counts xor $200) shr 2);
      TmpFreq := 174504.0 / ((Temp + 1) * 2);
      leAD7814Counts.Text := format('%u', [SysVarsCC.DUT_AD7814_Counts]);
      leTempDegC.Text := format('%1.2f', [-128 + ((SysVarsCC.DUT_AD7814_Counts xor $200) * 0.25)]);
      leTempHz.Text := format('%1.2f', [TmpFreq]);

      DUT_RVVolts := SysVarsCC.DUT_RV_Volts_Counts * 0.002428;
      leRVVoltsCounts.Text := format('%u', [SysVarsCC.DUT_RV_Volts_Counts]);
      leRvVolts.Text := format('%1.2f', [DUT_RVVolts]);

      leRVCurrentCounts.Text := format('%u', [SysVarsCC.DUT_RV_Amps_Counts]);
      leRvCurrent.Text := format('%1.2f', [SysVarsCC.DUT_RV_Amps_Counts * 0.01506]);

      leKmaPeriod.Text := format('%u', [SysVarsCC.DUT_KMA_Period]);
      if SysVarsCC.DUT_KMA_Period = 0 then
        leKmaFreq.Text := '0.00'
      else
        leKmaFreq.Text := format('%1.2f', [65536.0 / SysVarsCC.DUT_KMA_Period]);

      leSlave1Period.Text := format('%u', [SysVarsCC.DUT_Slave_Period[0]]);
      if SysVarsCC.DUT_Slave_Period[0] = 0 then
        leSlave1Freq.Text := '0.00'
      else
        leSlave1Freq.Text := format('%1.2f', [65536.0 / SysVarsCC.DUT_Slave_Period[0]]);

      leSlave2Period.Text := format('%u', [SysVarsCC.DUT_Slave_Period[1]]);
      if SysVarsCC.DUT_Slave_Period[1] = 0 then
        leSlave2Freq.Text := '0.00'
      else
        leSlave2Freq.Text := format('%1.2f', [65536.0 / SysVarsCC.DUT_Slave_Period[1]]);

      leSlave3Period.Text := format('%u', [SysVarsCC.DUT_Slave_Period[2]]);
      if SysVarsCC.DUT_Slave_Period[2] = 0 then
        leSlave3Freq.Text := '0.00'
      else
        leSlave3Freq.Text := format('%1.2f', [65536.0 / SysVarsCC.DUT_Slave_Period[2]]);

      leMasterPeriod.Text := format('%u', [SysVarsCC.DUT_Master_Period]);
      if SysVarsCC.DUT_Master_Period = 0 then
        leMasterFreq.Text := '0.00'
      else
        leMasterFreq.Text := format('%1.2f', [65536.0 / SysVarsCC.DUT_Master_Period]);

      leMasterIndex.Text := format('%u', [SysVarsCC.DUT_Master_Index]);
      leEepromAddress.Text := format('%u', [SysVarsCC.DUT_EEPROM_Address]);
      if cbRVVoltage.Checked then
        begin
        leSetpoint.Text := format('%1.2f', [SysVarsCC.DUT_Setpoint / 100.0]);
        leFeedback.Text := format('%1.2f', [SysVarsCC.DUT_Feedback / 100.0]);
        end
      else
        begin
        leSetpoint.Text := format('%1.1f', [SysVarsCC.DUT_Setpoint / 10.0]);
        leFeedback.Text := format('%1.1f', [SysVarsCC.DUT_Feedback / 10.0]);
        end;

      lePwmCounts.Text := format('%u', [SysVarsCC.DUT_PWM_Counts]);
      lePwmVolts.Text := format('%1.3f', [SysVarsCC.DUT_PWM_Counts * 0.002377]);

      leFirmwareVersion.Text := format('%u', [SysVarsCC.DUT_FW_Version]);

      end;
    DUT_InvalidCounter := DUT_InvalidCounter + 1;
    if SysVarsCC.DUT_UpdateCounter <> OldUpdateCounter then
      begin
      if not bCommunicating then
        begin
        TestMessage('Communicating w/ Sender', 0);
        bCommunicating := true;
        end;
      OldUpdateCounter := SysVarsCC.DUT_UpdateCounter;
      DUT_InvalidCounter := 0;
      end;

    if (DUT_InvalidCounter = 5) and bCommunicating then
      begin
      TestMessage('Stopped Communicating with Sender', 0);
      bCommunicating := false;
      end;

    edtDigFreqIn1.Text := format('%1.2f', [SysVars.DigitalFrequency[0] /100.0]);
    edtDigFreqIn2.Text := format('%1.2f', [SysVars.DigitalFrequency[1] /100.0]);
    edtDigFreqIn3.Text := format('%1.2f', [SysVars.DigitalFrequency[2] /100.0]);
    edtDigFreqIn4.Text := format('%1.2f', [SysVars.DigitalFrequency[3] /100.0]);
    edtDigFreqIn5.Text := format('%1.2f', [SysVars.DigitalFrequency[4] /100.0]);
    edtDigFreqBlue.Text := format('%1.2f', [SysVars.DigitalFrequency[0] / 100.0]);

    if SysVars.DigitalFrequency[0] = 0 then
      edtIndex.Text := '---'
    else
      begin
      if rgSlaves.ItemIndex = 0 then edtIndex.Text := format('%1.0f', [(3276800 / SysVars.DigitalFrequency[0]) + 16]);
      if rgSlaves.ItemIndex = 1 then edtIndex.Text := format('%1.0f', [(1638400 / SysVars.DigitalFrequency[0]) + 16]);
      if rgSlaves.ItemIndex = 2 then edtIndex.Text := format('%1.0f', [(10922.67 / SysVars.DigitalFrequency[0]) + 16]);
      if rgSlaves.ItemIndex = 3 then edtIndex.Text := format('%1.0f', [(819200 / SysVars.DigitalFrequency[0]) + 16]);
      end;

    end;

  //----------------------------------------------------------

    if bFactoryUpdate then
      begin
      bFactoryUpdate := false;
      for i := 0 to 15 do
        begin
        CalibrationAdcZeroEdit[i].Text := format('%d', [FactoryData.AdcZero[i]]);
        CalibrationAdcSpanEdit[i].Text := format('%d', [FactoryData.AdcSpan[i]]);
        end;
      end;

    //----------------------------------------------------------

    if bSerialEEPROMDataUpdate then
      begin
      bSerialEEPROMDataUpdate := false;
      leOhmsKFactor.Text := format('%u', [CurrentEEPROMData.OhmsKFactor]);
      leVoltsKFactor.Text := format('%u', [CurrentEEPROMData.VoltageKFactor]);
      leEEPROMConfig.Text := format('%u', [CurrentEEPROMData.EEPROM_Config]);
      leKmaSamples.Text := format('%u', [CurrentEEPROMData.KmaSamples]);
      leTrippoint.Text := format('%u', [CurrentEEPROMData.Trippoint]);
      leHoldValue.Text := format('%u', [CurrentEEPROMData.HoldValue]);

      CurrentProfileData.EEPROM_Config := CurrentEEPROMData.EEPROM_Config;
      CurrentProfileData.Trippoint := CurrentEEPROMData.Trippoint;
      CurrentProfileData.KmaSamples := CurrentEEPROMData.KmaSamples;
      CurrentProfileData.HoldValue := CurrentEEPROMData.HoldValue;
      end;

    //----------------------------------------------------------


  if bSerialGetEEPROMDataUpdate then
    begin
    bSerialGetEEPROMDataUpdate := false;
    //
    // Calculate the Calibration Scaling factor.  2976 is the nominal calibration of a Sender.  The following logic will round the
    // Scaling Factor to the nearest 0.05 (i.e., 1.00, 0.95, 0.90, 0.85, etc)
    //
    CurrentProfileData.CalibrationScaling := (round((CurrentEEPROMData.VoltageKFactor / 2976.0) * 20.0) * 5) / 100.0;
    //
    // Now, using the Calibration Scaling Factor, convert EEPROM profile data back into engineering values
    //
    for i := 0 to 509 do CurrentProfileData.Profile[i] := round(CurrentEEPROMData.Profile[i] * CurrentProfileData.CalibrationScaling);
    //
    // Now, check if Config/Profile data in EEPROM matches any of the Profiles stored in PROFILES_CC.INI
    //  For now, if a match is found, add name of profile to Memo1
    //
    ProfileList := TStringList.Create;
    FindProfile(CurrentEEPROMData, ProfileList);
    if ProfileList.Count > 0 then
      begin
      for i := 0 to ProfileList.Count - 1 do memo1.lines.add(ProfileList.Strings[i]);
      end;
    ProfileList.Free;
    bCurrentEEPROMDataValid := true;
    end;

  //----------------------------------------------------------

  if bCD_48_Update then
    begin
    bCD_48_Update := false;
    for i := 0 to 27 do
      begin
      CheckboxErrorCondition[i].Checked := CD_48.ErrorPresent[i] <> 0;
      end;
    end;

  //----------------------------------------------------------

    if bCD_99_Update then
      begin
      bCD_99_Update := false;
      leFirmwareCrc.Text := format('%4X', [CD_99.Crc16]);
      leFirmwareBuild.Text := StrPas(CD_99.Version);
      end;

  //----------------------------------------------------------

    if bCD_71_Update then
      begin
      bCD_71_Update := false;
      for i := 0 to 23 do
        begin
        ProcessTimingAverageTime[i].Text := format('%1.2f', [CD_71[i].AverageTime / 1000.0]);
        ProcessTimingMinTime[i].Text := format('%1.2f', [CD_71[i].MinTime / 100.0]);
        ProcessTimingMaxTime[i].Text := format('%1.2f', [CD_71[i].MaxTime / 100.0]);
        ProcessTimingCalls[i].Text := format('%d', [CD_71[i].CallsPerSecond]);
        end;
      end;

  //----------------------------------------------------------

    if bCD_41_Update then          // Flash Configuration
      begin
      bCD_41_Update := false;
      bDownloadDone := true;
      move(CD_41_Download.ProfileName, CDProfileData.CurrentConfigData.ProfileName, Sizeof(CDProfileData.CurrentConfigData.ProfileName));
      CDProfileData.CurrentConfigData.BF2.DigIo1 := CD_41_Download.BF2.DigIo1;
      CDProfileData.CurrentConfigData.BF2.DigIo2 := CD_41_Download.BF2.DigIo2;
      CDProfileData.CurrentConfigData.BF2.DigIo3 := CD_41_Download.BF2.DigIo3;
      CDProfileData.CurrentConfigData.BF2.DigIo4 := CD_41_Download.BF2.DigIo4;
      CDProfileData.CurrentConfigData.BF2.DigIo5 := CD_41_Download.BF2.DigIo5;
      CDProfileData.CurrentConfigData.BF2.DigIo6 := CD_41_Download.BF2.DigIo6;

      CDProfileData.CurrentConfigData.BF8.DefaultOutput:= CD_41_Download.BF8.DefaultOutput;
      CDProfileData.CurrentConfigData.BF8.SensorMode := CD_41_Download.BF8.SensorMode;
      CDProfileData.CurrentConfigData.BF8.CanUse := CD_41_Download.BF8.CanUse;
      CDProfileData.CurrentConfigData.BF8.SourceAddress := CD_41_Download.BF8.SourceAddress;
      CDProfileData.CurrentConfigData.BF8.CanEcho := CD_41_Download.BF8.CanEcho;
      CDProfileData.CurrentConfigData.BF8.CanCrcEnable := CD_41_Download.BF8.CanCrcEnable;
      CDProfileData.CurrentConfigData.BF8.CanPrescaler := CD_41_Download.BF8.CanPrescaler;
      CDProfileData.CurrentConfigData.BF8.Endianess := CD_41_Download.BF8.Endianess;
      move(CD_41_Download.CanTable, CDProfileData.CurrentConfigData.CanTable, sizeof(CDProfileData.CurrentConfigData.CanTable));
      move(CD_41_Download.AngularOutputTable, CDProfileData.CurrentConfigData.AngularOutputTable, sizeof(CDProfileData.CurrentConfigData.AngularOutputTable));

      CDProfileData.CurrentConfigData.CanAddressOut[0]:= CD_41_Download.CanAddressOut[0];
      CDProfileData.CurrentConfigData.CanAddressOut[1]:= CD_41_Download.CanAddressOut[1];
      CDProfileData.CurrentConfigData.CanAddressOut[2]:= CD_41_Download.CanAddressOut[2];
      CDProfileData.CurrentConfigData.CanAddressOut[3]:= CD_41_Download.CanAddressOut[3];
      CDProfileData.CurrentConfigData.CanAddressOut[4]:= CD_41_Download.CanAddressOut[4];
      CDProfileData.CurrentConfigData.CanAddressOut[5]:= CD_41_Download.CanAddressOut[5];
      CDProfileData.CurrentConfigData.CanAddressIn[0]:= CD_41_Download.CanAddressIn[0];
      CDProfileData.CurrentConfigData.CanAddressIn[1]:= CD_41_Download.CanAddressIn[1];

      CDProfileData.CurrentConfigData.CanMsgOutTime[0]:= CD_41_Download.CanMsgOutTime[0];
      CDProfileData.CurrentConfigData.CanMsgOutTime[1]:= CD_41_Download.CanMsgOutTime[1];
      CDProfileData.CurrentConfigData.CanMsgOutTime[2]:= CD_41_Download.CanMsgOutTime[2];
      CDProfileData.CurrentConfigData.CanMsgInTime[0]:= CD_41_Download.CanMsgInTime[0];
      CDProfileData.CurrentConfigData.CanMsgInTime[1]:= CD_41_Download.CanMsgInTime[1];

      CDProfileData.CurrentConfigData.CanMaskIn[0] := CD_41_Download.CanMaskIn[0];
      CDProfileData.CurrentConfigData.CanMaskIn[1] := CD_41_Download.CanMaskIn[1];

      CDProfileData.CurrentConfigData.CanSamplingDelay:= CD_41_Download.CanSamplingDelay;
      CDProfileData.CurrentConfigData.OutputStartupdelay:= CD_41_Download.OutputStartupdelay;

      CDProfileData.CurrentConfigData.KmaFilterGain := CD_41_Download.KmaFilterGain;
      CDProfileData.CurrentConfigData.MasterIndexFilteringGain := CD_41_Download.MasterIndexFilteringGain;
      CDProfileData.CurrentConfigData.AccelVoltageXAdcFilteringGain := CD_41_Download.AccelVoltageXAdcFilteringGain;
      CDProfileData.CurrentConfigData.AccelVoltageYAdcFilteringGain := CD_41_Download.AccelVoltageYAdcFilteringGain;
      CDProfileData.CurrentConfigData.AccelVoltageZAdcFilteringGain := CD_41_Download.AccelVoltageZAdcFilteringGain;
      CDProfileData.CurrentConfigData.BF3.ReverseIndexKma := CD_41_Download.BF3.ReverseIndexKma;
      CDProfileData.CurrentConfigData.BF1.ErrorConditionActiveState := CD_41_Download.BF1.ErrorConditionActiveState;
      CDProfileData.CurrentConfigData.BF1.RVOutputMode := CD_41_Download.BF1.RVOutputMode;
      CDProfileData.CurrentConfigData.BF1.RVOutputPullup := CD_41_Download.BF1.RVOutputPullup;
      CDProfileData.CurrentConfigData.BF1.MasterFreqMode := CD_41_Download.BF1.MasterFreqMode;
      CDProfileData.CurrentConfigData.BF3.RS485MessageMode := CD_41_Download.BF3.RS485MessageMode;
      CDProfileData.CurrentConfigData.BF3.RS485MessageTime := CD_41_Download.BF3.RS485MessageTime;
      CDProfileData.CurrentConfigData.BF3.AccelerometerScaling := CD_41_Download.BF3.AccelerometerScaling;


      move(CD_41_Download.SetpointTable, CDProfileData.CurrentConfigData.SetpointTable, sizeof(CDProfileData.CurrentConfigData.SetpointTable));
      move(CD_41_Download.MasterFreqTable, CDProfileData.CurrentConfigData.MasterFreqTable, sizeof(CDProfileData.CurrentConfigData.MasterFreqTable));

      end;

  //----------------------------------------------------------

    if bCD_42_Update then          // Factory Configuration
      begin
      bCD_42_Update := false;
      bDownloadDone := true;
      move(CD_42_Download.DateStr, CDProfileData.CurrentCDFactoryData.DateStr, Sizeof(CDProfileData.CurrentCDFactoryData.DateStr));
      move(CD_42_Download.TimeStr, CDProfileData.CurrentCDFactoryData.TimeStr, Sizeof(CDProfileData.CurrentCDFactoryData.TimeStr));
      move(CD_42_Download.NameTesterStr, CDProfileData.CurrentCDFactoryData.NameTesterStr, Sizeof(CDProfileData.CurrentCDFactoryData.NameTesterStr));
      move(CD_42_Download.ModelNumberStr, CDProfileData.CurrentCDFactoryData.ModelNumberStr, Sizeof(CDProfileData.CurrentCDFactoryData.ModelNumberStr));
      move(CD_42_Download.SerialNumberStr, CDProfileData.CurrentCDFactoryData.SerialNumberStr, Sizeof(CDProfileData.CurrentCDFactoryData.SerialNumberStr));

      move(CD_42_Download.LocationStr, CDProfileData.CurrentCDFactoryData.LocationStr, Sizeof(CDProfileData.CurrentCDFactoryData.LocationStr));
      move(CD_42_Download.TypeStr, CDProfileData.CurrentCDFactoryData.TypeStr, Sizeof(CDProfileData.CurrentCDFactoryData.TypeStr));
      move(CD_42_Download.ManufacturerStr, CDProfileData.CurrentCDFactoryData.ManufacturerStr, Sizeof(CDProfileData.CurrentCDFactoryData.ManufacturerStr));
      move(CD_42_Download.HardwareIdStr, CDProfileData.CurrentCDFactoryData.HardwareIdStr, Sizeof(CDProfileData.CurrentCDFactoryData.HardwareIdStr));
      move(CD_42_Download.SoftwareIdStr, CDProfileData.CurrentCDFactoryData.SoftwareIdStr, Sizeof(CDProfileData.CurrentCDFactoryData.SoftwareIdStr));
      move(CD_42_Download.MakeStr, CDProfileData.CurrentCDFactoryData.MakeStr, Sizeof(CDProfileData.CurrentCDFactoryData.MakeStr));
      move(CD_42_Download.ModeStr, CDProfileData.CurrentCDFactoryData.ModeStr, Sizeof(CDProfileData.CurrentCDFactoryData.ModeStr));
      move(CD_42_Download.SerialStr, CDProfileData.CurrentCDFactoryData.SerialStr, Sizeof(CDProfileData.CurrentCDFactoryData.SerialStr));
      move(CD_42_Download.UnitStr, CDProfileData.CurrentCDFactoryData.UnitStr, Sizeof(CDProfileData.CurrentCDFactoryData.UnitStr));

      CDProfileData.CurrentCDFactoryData.Calibration33 := CD_42_Download.Calibration33;
      CDProfileData.CurrentCDFactoryData.PowerRail5KFactor := CD_42_Download.PowerRail5KFactor;
      CDProfileData.CurrentCDFactoryData.KmaKFactor := CD_42_Download.KmaKFactor;
      CDProfileData.CurrentCDFactoryData.PowerInputKFactor := CD_42_Download.PowerInputKFactor;
      CDProfileData.CurrentCDFactoryData.RVOutputVoltageKFactor := CD_42_Download.RVOutputVoltageKFactor;
      CDProfileData.CurrentCDFactoryData.RVOutputCurrentKFactor := CD_42_Download.RVOutputCurrentKFactor;
      CDProfileData.CurrentCDFactoryData.PwmOutputKFactor := CD_42_Download.PwmOutputKFactor;
      CDProfileData.CurrentCDFactoryData.BF5 := CD_42_Download.BF5;

      CDProfileData.CurrentCDFactoryData.PowerInputFilteringGain := CD_42_Download.PowerInputFilteringGain;
      CDProfileData.CurrentCDFactoryData.PowerRail5FilteringGain := CD_42_Download.PowerRail5FilteringGain;
      CDProfileData.CurrentCDFactoryData.TemperatureFilteringGain := CD_42_Download.TemperatureFilteringGain;
      CDProfileData.CurrentCDFactoryData.InternalReferenceFilteringGain := CD_42_Download.InternalReferenceFilteringGain;
      CDProfileData.CurrentCDFactoryData.RVOutputVoltageFilteringGain := CD_42_Download.RVOutputVoltageFilteringGain;
      CDProfileData.CurrentCDFactoryData.RVOutputCurrentFilteringGain := CD_42_Download.RVOutputCurrentFilteringGain;

      cbPcbRevision.ItemIndex := CD_42_Download.BF5.HardwareVersion + 1;
      cbAccelerometer.Checked := (CD_42_Download.BF5.AccelPresent) <> 0;
      cbRVOutput.Checked := (CD_42_Download.BF5.RVOutputPresent) <> 0;
      cbRS485Driver.Checked := (CD_42_Download.BF5.RS485DriverPresent) <> 0;
      cbCANBusDriver.Checked := (CD_42_Download.BF5.CanDriverPresent) <> 0;
      cbTemperatureSensor.Checked := (CD_42_Download.BF5.TempSensorPresent) <> 0;
      cbKmaSensor.Checked := (CD_42_Download.BF5.KmaPresent) <> 0;

      edtConfig33Calibration.Text := format('%1.3f', [CD_42_Download.Calibration33 / 1000.0]);
      edtConfig5VDCCalibration.Text := format('%1.0f', [CD_42_Download.PowerRail5KFactor / 1.0]);
      edtConfigKmaCalibration.Text := format('%1.0f', [CD_42_Download.KmaKFactor / 1.0]);
      edtConfigPowerInputVoltageCalibration.Text := format('%1.0f', [CD_42_Download.PowerInputKFactor / 1.0]);
      edtConfigRVVoltageCalibration.Text := format('%1.0f', [CD_42_Download.RVOutputVoltageKFactor / 1.0]);
      edtConfigRVCurrentScaleCalibration.Text := format('%1.0f', [CD_42_Download.RVOutputCurrentKFactor / 1.0]);
      edtConfigPwmOutput.Text := format('%1.0f', [CD_42_Download.PwmOutputKFactor / 1.0]);

      edtConfigPowerInputFilteringGain.Text := format('%d', [CD_42_Download.PowerInputFilteringGain]);
      edtConfig5VDCFilteringGain.Text := format('%d', [CD_42_Download.PowerRail5FilteringGain]);
      edtConfigTemperatureFilteringGain.Text := format('%d', [CD_42_Download.TemperatureFilteringGain]);
      edtConfigInternalReferneceFilteringGain.Text := format('%d', [CD_42_Download.InternalReferenceFilteringGain]);
      edtConfigRVVoltageFilteringGain.Text := format('%d', [CD_42_Download.RVOutputVoltageFilteringGain]);
      edtConfigRVCurrentFilteringGain.Text := format('%d', [CD_42_Download.RVOutputCurrentFilteringGain]);

      CD_42_Download.DateStr[15] := #0;
      CD_42_Download.TimeStr[11] := #0;
      CD_42_Download.NameTesterStr[39] := #0;
      CD_42_Download.ModelNumberStr[39] := #0;
      CD_42_Download.SerialNumberStr[39] := #0;

      CD_42_Download.LocationStr[39] := #0;
      CD_42_Download.TypeStr[39] := #0;
      CD_42_Download.ManufacturerStr[39] := #0;
      CD_42_Download.HardwareIdStr[39] := #0;
      CD_42_Download.SoftwareIdStr[39] := #0;
      CD_42_Download.MakeStr[7] := #0;
      CD_42_Download.ModeStr[39] := #0;
      CD_42_Download.SerialStr[39] := #0;
      CD_42_Download.UnitStr[39] := #0;

      edtConfigManufacturingDate.Text := StrPas(CD_42_Download.DateStr);
      edtConfigManufacturingTime.Text := StrPas(CD_42_Download.TimeStr);
      edtConfigManufacturingNameTester.Text := StrPas(CD_42_Download.NameTesterStr);
      edtConfigManufacturingModelNumber.Text := StrPas(CD_42_Download.ModelNumberStr);
      edtConfigManufacturingSerialNumber.Text := StrPas(CD_42_Download.SerialNumberStr);

      edtConfigManufacturingLocation.text := StrPas(CD_42_Download.LocationStr);
      edtConfigManufacturingType.text := StrPas(CD_42_Download.TypeStr);
      edtConfigManufacturingManufacturer.text := StrPas(CD_42_Download.ManufacturerStr);
      edtConfigManufacturingHardwareId.text := StrPas(CD_42_Download.HardwareIdStr);
      edtConfigManufacturingSoftwareId.text := StrPas(CD_42_Download.SoftwareIdStr);
      edtConfigManufacturingMake.text := StrPas(CD_42_Download.MakeStr);
      edtConfigManufacturingModel.text := StrPas(CD_42_Download.ModeStr);
      edtConfigManufacturingSerial.text := StrPas(CD_42_Download.SerialStr);
      edtConfigManufacturingUnit.text := StrPas(CD_42_Download.UnitStr);

      end;

  //----------------------------------------------------------

  if bCD_43_Update then            // Fuzzy Logic Configuration
    begin
    bCD_43_Update := false;
    bDownloadDone := true;
    move(CD_43_Download.Fuzzy1, CDProfileData.Fuzzy1Data, sizeof(CDProfileData.Fuzzy1Data));
    move(CD_43_Download.Fuzzy2, CDProfileData.Fuzzy2Data, sizeof(CDProfileData.Fuzzy2Data));

    end;
  //----------------------------------------------------------


    if bCD_44_Update then          // Event Log
      begin
      bCD_44_Update := false;
      pnlReadConfig.Visible:=false;
      StartingIndex := -1;
      Entries := 0;
      for i := 0 to 511 do
        begin
        if CD_44[i].EntryIndex = -1 then
          begin
          StartingIndex := -1;
          break;
          end;
        if CD_44[i].EntryIndex > Entries then
          begin
          Entries := CD_44[i].EntryIndex;
          StartingIndex := i;
          end;
        end;
      if StartingIndex = -1 then
        StartingIndex := 0
      else
        begin
        StartingIndex := StartingIndex + 1;
        if StartingIndex = 512 then
          StartingIndex := 0;
        end;
      leCD44_Events.Text:=format('%u', [Entries]);

      memo2.Lines.BeginUpdate;
      memo2.Clear;
      for i := 0 to 2047 do
        begin
        if CD_44[StartingIndex].EntryIndex = -1 then
          break;
        case CD_44[StartingIndex].stBFEvent.EventId of
            0:  TmpStr := 'Event Log Cleared';
            1:  TmpStr := '-->Power-up<--';
            2:  TmpStr := 'FATAL: Factory Configuration CRC Invalid';
            3:  TmpStr := 'FATAL: User Configuration CRC Invalid';
            4:  TmpStr := 'New Flash Configuration Data';
            5:  TmpStr := 'New Factory Configuration Data';
            6:  TmpStr := 'New Fuzzy Logic Data';
            7:  TmpStr := 'MOSFET Power Limit Exceeded';
            8:  TmpStr := '+5VDC Low';
            9:  TmpStr := '+5VDC High';
            10:  TmpStr := 'Internal Reference Low';
            11:  TmpStr := 'Internal Reference High';
            12:  TmpStr := 'Power Input Low';
            13:  TmpStr := 'Power Input High';
            14:  TmpStr := 'KMA Percentage Low';
            15:  TmpStr := 'KMA Percentage High';
            16:  TmpStr := 'Temperature Low';
            17:  TmpStr := 'Temperature High';
            18:  TmpStr := 'SP/FB Delta Exceeded';
            19:  TmpStr := 'CAN Setpoint A Low';
            20:  TmpStr := 'CAN Setpoint A High';
            21:  TmpStr := 'CAN Setpoint B Low';
            22:  TmpStr := 'CAN Setpoint B High';
            23:  TmpStr := 'RS485 Not Updating';
            24:  TmpStr := 'Setpoint A and Setpoint B Do Not Match';
            25:  TmpStr := 'CAN Reception Error';
            26:  TmpStr := 'CAN Transmission Error';
            27:  TmpStr := 'CAN Echo Error';
            28:  TmpStr := 'CAN CRC Error';
            29:  TmpStr := 'Firmware Upload';
            30:  TmpStr := 'FATAL: Stack Overflow imminent!! - Resetting CPU!!';
            31:  TmpStr := 'FATAL: Fuzzy Logic Configuration CRC Invalid';
            32:  TmpStr := 'FATAL: Firmware CRC Invalid';
            33:  TmpStr := 'FATAL: Configuration CRC Invalid';
            34:  TmpStr := 'FATAL: Factory CRC Invalid';
            35:  TmpStr := 'FATAL: Pitch CRC Invalid';
            36:  TmpStr := 'Serial Master Error';
            37:  TmpStr := 'Sensor A Quality Error';
            38:  TmpStr := 'Sensor B Quality Error';
            end;

        TmpStr := format('%5u %6u %s', [CD_44[StartingIndex].EntryIndex, CD_44[StartingIndex].stBFEvent.TimeStamp, TmpStr]);
        memo2.Lines.add(TmpStr);
        StartingIndex := StartingIndex + 1;
        if StartingIndex = 2048 then
          StartingIndex := 0;
        end;
      memo2.Lines.EndUpdate;
      OldNumberOfEvents := CD_70.ENG_NumberEventLogEntries;
      end;



  //----------------------------------------------------------

    if bCD_70_Update then                         // System Variables
      begin
      bCD_70_Update := false;
      bCalibrationComm := true;
      CD_70_UpdateCounter := 2;

      leFirmwareStackSlack.Text := format('%d', [CD_70.StackSlack * 4]);
      leFirmwareStackUsed.Text := format('%d', [CD_70.StackUsed * 4]);

      lePowerInputVoltage1.Text := format('%d', [CD_70.ENG_PowerInputCounts]);
      leKma1.Text := format('%d', [CD_70.ENG_KmaCounts]);
      leTemp1.Text := format('%d', [CD_70.ENG_TemperatureCounts]);
      leRVOutputVoltage1.Text := format('%d', [CD_70.ENG_RVOutputVoltageCounts]);
      leRVOutputCurrent1.Text := format('%d', [CD_70.ENG_RVOutputCurrentCounts]);
      leAccelXg1.Text := format('%d', [CD_70.ENG_AccelXCounts]);
      leAccelYg1.Text := format('%d', [CD_70.ENG_AccelYCounts]);
      leAccelZg1.Text := format('%d', [CD_70.ENG_AccelZCounts]);
      leIntRef9.Text := format('%d', [CD_70.ENG_IntRefCounts]);
      le5PwrRail1.Text := format('%d', [CD_70.ENG_PowerRail5Counts]);
      if (CD_70.BF9.SensorLetter <> 0) then
        begin
        edtSensorBKmaIndex.Text := format('%1.2f', [CD_70.ENG_KmaIndex / 100.0]);
        edtSensorBErrorValue.Text := format('%d', [CD_70.BF9.ErrorStatus]);
        edtSensorBQuality1.Text := format('%1.2f', [CD_70.Quality1 / 100.0]);
        edtSensorBQuality2.Text := format('%1.2f', [CD_70.Quality2 / 100.0]);
        edtSensorBSetpoint.Text := format('%1.2f', [CD_70.ThrottleSetpoint/100]);
        edtSensorBAngle.Text := format('%d', [CD_70.BF11.SensorAngle]);
        edtSensorBNoise.Text := format('%1.1f', [CD_70.Noise / 10.0]);

        end
      else
        begin
        edtSensorAKmaIndex.Text := format('%1.2f', [CD_70.ENG_KmaIndex / 100.0]);
        edtSensorAErrorValue.Text := format('%d', [CD_70.BF9.ErrorStatus]);
        edtSensorAQuality1.Text := format('%1.2f', [CD_70.Quality1 / 100.0]);
        edtSensorAQuality2.Text := format('%1.2f', [CD_70.Quality2 / 100.0]);
        edtSensorASetpoint.Text := format('%1.2f', [CD_70.ThrottleSetpoint/100]);
        edtSensorAAngle.Text := format('%d', [CD_70.BF11.SensorAngle]);
        edtSensorANoise.Text := format('%1.1f', [CD_70.Noise / 10.0]);
        end;

      Form3.leSensorAIndex.Text:= edtSensorAKmaIndex.Text;
      Form3.leSensorBIndex.Text:= edtSensorBKmaIndex.Text;

      leCanSetpoint.Text := format('%1.2f', [CD_70.CanSetpoint / 100.0]);


      lePowerInputVoltage2.Text := format('%1.1f', [CD_70.ENG_FilteredPowerInputCounts / 100.0]);
      leKma2.Text := format('%1.1f', [CD_70.ENG_FilteredKmaCounts / 100.0]);
      leTemp2.Text := format('%1.1f', [CD_70.ENG_FilteredTemperatureCounts / 100.0]);
      leRVOutputVoltage2.Text := format('%1.1f', [CD_70.ENG_FilteredRVOutputVoltageCounts / 100.0]);
      leRVOutputCurrent2.Text := format('%1.1f', [CD_70.ENG_FilteredRVOutputCurrentCounts / 100.0]);
      leAccelXg2.Text := format('%1.1f', [CD_70.ENG_FilteredAccelXCounts / 100.0]);
      leAccelYg2.Text := format('%1.1f', [CD_70.ENG_FilteredAccelYCounts / 100.0]);
      leAccelZg2.Text := format('%1.1f', [CD_70.ENG_FilteredAccelZCounts / 100.0]);
      leIntRef10.Text := format('%1.1f', [CD_70.ENG_FilteredIntRefCounts / 100.0]);
      le5PwrRail2.Text := format('%1.1f', [CD_70.ENG_FilteredPowerRail5Counts / 100.0]);

      lePowerInputVoltage.Text := format('%1.3f', [CD_70.ENG_PowerInputVoltage / 1000.0]);
      leKma.Text := format('%1.4f', [CD_70.ENG_KmaVoltage / 10000.0]);
      leTemp.Text := format('%1.2f', [CD_70.ENG_TemperatureCelsius / 100.0]);
      leRVOutputVoltage.Text := format('%1.4f', [CD_70.ENG_RVOutputVoltage / 10000.0]);
      leRVOutputCurrent.Text := format('%1.3f', [CD_70.ENG_RVOutputCurrent / 1000.0]);
      leAccelXg.Text := format('%1.4f', [CD_70.ENG_AccelGravityX / 10000.0]);
      leAccelYg.Text := format('%1.4f', [CD_70.ENG_AccelGravityY / 10000.0]);
      leAccelZg.Text := format('%1.4f', [CD_70.ENG_AccelGravityZ / 10000.0]);
      le5PwrRail.Text := format('%1.3f', [CD_70.ENG_PowerRail5VDC / 1000.0]);

      leKmaPercent.Text := format('%1.3f', [CD_70.ENG_KmaPercentage / 1000.0]);
      leKmaDeg.Text := format('%1.2f', [CD_70.ENG_KmaAngle / 100.0]);
      leKmaIndex.Text := format('%1.2f', [CD_70.ENG_KmaIndex / 100.0]);
      leKmaFreq1.Text := format('%1.2f', [CD_70.ENG_KmaFrequency / 100.0]);

      le33PwrRail5.Text := format('%1.0f', [CD_70.ENG_FactoryIntRefCounts / 1.0]);
      le33PwrRail.Text := format('%1.3f', [CD_70.ENG_PowerRail33VDC / 1000.0]);
      leTempFreq.Text := format('%1.2f', [CD_70.ENG_TemperatureFrequency / 100.0]);

      leMasterIndex1.Text := format('%1.2f', [CD_70.ENG_MasterIndex / 100.00]);
      leMasterIndex2.Text := format('%1.2f', [CD_70.ENG_FilteredMasterIndex / 100.0]);
      leMasterFrequency.Text := format('%1.2f', [CD_70.ENG_MasterFrequency / 100.0]);

      leSetpointIndex.Text := format('%1.2f', [CD_70.ENG_SetpointIndex / 100.0]);
      lePwmDutyCycle.Text := format('%1.3f', [CD_70.ENG_PwmDutyCycle / 1000.0]);
      lePwmDutyCycle1.Text := format('%1.3f', [CD_70.ENG_PwmDutyCycleFineTune / 1000.0]);

      leSetpoint1.Text := format('%1.3f', [CD_70.ENG_Setpoint / 1000.0]);
      leFeedback1.Text := format('%1.3f', [CD_70.ENG_Feedback / 1000.0]);

      leRVOutputOhms.Text := format('%1.2f', [CD_70.ENG_RVOutputOhms / 100.0]);
      leMosfetPower.Text := format('%1.3f', [CD_70.ENG_MosfetPower / 1000.0]);

      end;

    //----------------------------------------------------------

    if bCD_73_Update then                         // System Variables
      begin
      bCD_73_Update := false;
;
      if (CD_70.BF9.SensorLetter <> 0) then
        begin
        edtSensorAKmaIndex.Text := format('%1.2f', [CD_73.ENG_KmaIndex / 100.0]);
        edtSensorAErrorValue.Text := format('%d', [CD_73.BF9.ErrorStatus]);
        edtSensorAQuality1.Text := format('%1.2f', [CD_73.Quality1 / 100.0]);
        edtSensorAQuality2.Text := format('%1.2f', [CD_73.Quality2 / 100.0]);
        edtSensorASetpoint.Text := format('%1.2f', [CD_73.ThrottleSetpoint/100]);
        edtSensorAAngle.Text := format('%d', [CD_73.BF11.SensorAngle]);
        edtSensorANoise.Text := format('%1.1f', [CD_73.Noise / 10.0]);
        end
      else
        begin
        edtSensorBKmaIndex.Text := format('%1.2f', [CD_73.ENG_KmaIndex / 100.0]);
        edtSensorBErrorValue.Text := format('%d', [CD_73.BF9.ErrorStatus]);
        edtSensorBQuality1.Text := format('%1.2f', [CD_73.Quality1 / 100.0]);
        edtSensorBQuality2.Text := format('%1.2f', [CD_73.Quality2 / 100.0]);
        edtSensorBSetpoint.Text := format('%1.2f', [CD_73.ThrottleSetpoint/100]);
        edtSensorBAngle.Text := format('%d', [CD_73.BF11.SensorAngle]);
        edtSensorBNoise.Text := format('%1.1f', [CD_73.Noise / 10.0]);
        end;

      Form3.leSensorAIndex.Text:= edtSensorAKmaIndex.Text;
      Form3.leSensorBIndex.Text:= edtSensorBKmaIndex.Text;

      end;

ExitHere:
end;

//******************************************************************************************************************
var
  EEPROMState: integer;

procedure TForm1.ProcessEEPROMState;
begin

  if WaitTime > 0 then WaitTime := WaitTime - 1;
  case (EEPROMState) of
  0:
    begin
    if (SysVarsCC.EEPROM_Mode = EEPROM_MODE_READ_PROFILE) then EEPROMState := 100;
    if (SysVarsCC.EEPROM_Mode = EEPROM_MODE_UPDATE_PROFILE) then EEPROMState := 200;
    end;

  //----------------------------------------------------------------------------------

  100:
    begin
    TestMessage('Reading Config and Profile Data', 0);
    lblMessage.Caption:= 'Reading Configuration and Profile Information';
    pnlReadConfig.Visible := true;
    ProgressBar1.Position:= SysVarsCC.DUT_EEPROM_Address shr 1;
    EEPROMState := 110;
    end;
  110:
    begin
    ProgressBar1.Position:= SysVarsCC.DUT_EEPROM_Address shr 1;
    if (SysVarsCC.EEPROM_Mode <> EEPROM_MODE_READ_PROFILE) then
      begin
      pnlReadConfig.Visible := FALSE;
      EEPROMState := 0;
      end;
    end;

  //----------------------------------------------------------------------------------

  200:
    begin
    TestMessage('Writing Config and Profile Data', 0);
    lblMessage.Caption:= 'Writing Profile Information';
    pnlReadConfig.Visible := true;
    ProgressBar1.Position:= SysVarsCC.DUT_EEPROM_Address shr 1;
    EEPROMState := 210;
    end;
  210:
    begin
    ProgressBar1.Position:= SysVarsCC.DUT_EEPROM_Address shr 1;
    if (SysVarsCC.EEPROM_Mode <> EEPROM_MODE_UPDATE_PROFILE) then
      begin
      EEPROMState := 0;
      end;
    end;
  end;
end;

//******************************************************************************************************************

var
  ExecutionTimer: integer;
  StatusTimer: integer;
  ManualDelay: integer;
  ForeignSensorTimer: integer;

procedure ProcessCDCommunications;
var
  TmpFloat: single;
  ManualSP, StrIndex: integer;
  TMP: integer;

begin
  if (Form1.rgMode.ItemIndex = 0) or (((SetpointData.EnableBits and ENABLE_POWER) = 0) and (Form1.rgMode.ItemIndex <> 3)) then
    begin
    CDCommState := 0;
    CDCommTimer := 20;
    CDFlashIndex := 0;
    bDownloadFirmware := true;
    bDownloadEventLog := false;
    bDownloadTiming := true;
    bDownloadStatus := true;
    bDownloadForeignSensor:= true;
    CD_70_UpdateCounter := 0;
    exit;
    end;

  if CDCommTimer <> 0 then
    begin
    dec(CDCommTimer);
    exit;
    end;

  CDCommTimer := 0;
  case (CDCommState) of
  0: begin
  if bUploadCDFirmware then
      begin
      bUploadCDFirmware := false;
      bUploadCDFirmwareStarted := true;
      CDCommBuffer[0] := $CC;                     // Issue Reprogramming Command

      move(&FirmwareInfo, &CDCommBuffer[1], sizeof(FirmwareInfo));
      CDCommBufferLength := 9;
      CDCommState := 10;
      end
    else if bDownloadFlashConfig then
      begin
      CDFlashIndex := 1;
      CDCommBuffer[0] := $41;                     // Read Flash Config
      CDCommBuffer[1] := 0;
      CDCommBufferLength := 2;
      CDCommState := 1;
      end
    else if bUploadFlashConfiguration  then
      begin
      bUploadFlashConfiguration := false;
      CDCommBuffer[0] := $11;                     // Write Flash Configuration
      move(&CD_41_Upload, &CDCommBuffer[1], SizeOf(CD_41_Upload));
      CDCommBufferLength := SizeOf(CD_41_Upload) + 1;
      end
    else if bUploadFactoryConfiguration  then
      begin
      bUploadFactoryConfiguration := false;
      CDCommBuffer[0] := $12;                     // Write Factory Configuration
      move(&CD_42_Upload, &CDCommBuffer[1], SizeOf(CD_42_Upload));
      CDCommBufferLength := SizeOf(CD_42_Upload) + 1;
      end
    else if bUploadFuzzyConfiguration then
      begin
      bUploadFuzzyConfiguration := false;
      CDCommBuffer[0] := $13;                    // Write Fuzzy Configuration
      move(&CD_43_Upload, &CDCommBuffer[1], SizeOf(CD_43_Upload));
      CDCommBufferLength := SizeOf(CD_43_Upload) + 1;
      end
    else if bDownloadFuzzyConfig then     // Read Fuzzy Config
      begin
      CDFlashIndex := 1;
      CDCommBuffer[0] := $43;
      CDCommBuffer[1] := 0;
      CDCommBufferLength := 2;
      CDCommState := 3;
      end
    else if bDownloadFactoryConfig then
      begin
      CDFlashIndex := 1;
      CDCommBuffer[0] := $42;                     // Read Factory Config
      CDCommBuffer[1] := 0;
      CDCommBufferLength := 2;
      CDCommState := 2;
      end
    else if bDownloadFirmware then
      begin
      CDCommBuffer[0] := $99;                     // Read Firmware Information
      CDCommBufferLength := 1;
      bDownloadFirmware := false;
      end
    else if bClearEventLog then
      begin
      CDCommBuffer[0] := $14;                     // Clear Event Log
      CDCommBufferLength := 1;
      bClearEventLog := false;
      end
    else if bDownloadEventLog then
      begin
      CDFlashIndex := 0;
      CDCommBuffer[0] := $44;                     // Read EventLog
      CDCommBuffer[1] := 0;
      CDCommBuffer[2] := 0;
      CDCommBufferLength := 3;
      CDCommState := 4;
      end
    else if bDownloadTiming then
      begin
      CDCommBuffer[0] := $71;                     // Read Execution Timing Report
      CDCommBufferLength := 1;
      bDownloadTiming := false;
      end
    else if bDownloadStatus then
      begin
      CDCommBuffer[0] := $48;                     // Read Signal Status
      CDCommBufferLength := 1;
      bDownloadStatus := false;
      end
    else if bDownloadForeignSensor then
      begin
      CDCommBuffer[0] := $73;                    // Read foreign sensor data
      CDCommBufferLength := 1;
      bDownloadForeignSensor := false;
      end
    else
      begin
      queryPerformanceCounter(qCurrent);
      qDelta := qCurrent - qOld;
      qOld := qCurrent;

      CDCommBuffer[0] := $70;                     // Read System Variables
      move(HostData, CDCommBuffer[1], SizeOf(HostData));
      CDCommBufferLength := SizeOf(HostData) + 1;
      if (CD_70_UpdateCounter > 0) then CD_70_UpdateCounter := CD_70_UpdateCounter - 1;
      ExecutionTimer := ExecutionTimer + 1;
      StatusTimer := StatusTimer + 1;
      ForeignSensorTimer := ForeignSensorTimer + 1;
      // Get foreign sensor data every other time
      if ForeignSensorTimer > 1 then
        begin
        ForeignSensorTimer := 0;
        bDownloadForeignSensor := true;
        end
      else if ExecutionTimer > 5 then
        begin
        ExecutionTimer := 0;
        bDownloadTiming := true;
        end
      else if StatusTimer > 8 then
        begin
        StatusTimer := 0;
        bDownloadStatus := true;
        end;
      end;
      bSerialRelayMessage := true;
    end;
  1: begin
    CDCommBuffer[0] := $41;                     // Read Flash Config
    CDCommBuffer[1] := CDFlashIndex;
    CDCommBufferLength := 2;
    inc(CDFlashIndex);
    bSerialRelayMessage := true;
    if CDFlashIndex = 4 then
      begin
      bDownloadFlashConfig := false;
      CDCommState := 0;
      end;
    end;
  2: begin
    CDCommBuffer[0] := $42;                // Read Factory Config
    CDCommBuffer[1] := CDFlashIndex;
    CDCommBufferLength := 2;
    inc(CDFlashIndex);
    bSerialRelayMessage := true;
    if CDFlashIndex = 4 then
      begin
      bDownloadFactoryConfig := false;
      CDCommState := 0;
      end;
    end;
  3: begin
    CDCommBuffer[0] := $43;                // Ready Fuzzy Logic Config
    CDCommBuffer[1] := CDFlashIndex;
    CDCommBufferLength := 2;
    inc(CDFlashIndex);
    bSerialRelayMessage := true;
    if CDFlashIndex = 4 then
      begin
      bDownloadFuzzyConfig := false;
      CDCommState := 0;
      end;
    end;
  4: begin
    if CDFlashIndex = 128 then
      begin
      bDownloadEventLog := false;
      CDCommState := 0;
      end
    else
      begin
      CDCommBuffer[0] := $44;                   // Read Event Log
      CDCommBuffer[1] := CDFlashIndex and $FF;
      CDCommBuffer[2] := (CDFlashIndex shr 8) and $FF;
      CDCommBufferLength := 3;
      bSerialRelayMessage := true;
      end;
    end;
  10: begin
      if bCDSendProgramingData then
        begin
        bCDSendProgramingData := false;
        if FirmwarePageNumber <> 99 then
          begin
          CDCommBuffer[0] := $EE;                     // Send Firmware Data
          move(&FirmwarePageNumber, &CDCommBuffer[0], sizeof(FirmwarePageNumber));
          move(&FirmwarePageNumber, &CDCommBuffer[1], sizeof(FirmwarePageNumber));
          move(&FirmwareBinaryBuffer[FirmwarePageNumber * 2048], &CDCommBuffer[5], 2048);
          CDCommBufferLength := 2053;
          bSerialRelayMessage := true;
          end
        else
            begin
            bUploadCDFirmwareStarted := false;
            CDCommState := 0;
            end;
        end;
      end;

  end;

end;

//******************************************************************************************************************
//******************************************************************************************************************

procedure TForm1.ProcessManual;
var
  StrIndex: integer;
  TmpFloat: single;
  begin
    HostData.BF6.bManualPowerInputEnable := BooleanToInteger(Form2.cbManualPowerInput.Checked);
    HostData.BF6.bManualKmaEnable := BooleanToInteger(Form2.cbManualKma.Checked);
    HostData.BF6.bManualTemperatureEnable := BooleanToInteger(Form2.cbManualTemperature.Checked);
    HostData.BF6.bManualRVOutputVoltageEnable := BooleanToInteger(Form2.cbManualRVOutputVoltage.Checked);
    HostData.BF6.bManualRVOutputCurrentEnable := BooleanToInteger(Form2.cbManualRVOutputCurrent.Checked);
    HostData.BF6.bManualAccelXEnable := BooleanToInteger(Form2.cbManualAccelX.Checked);
    HostData.BF6.bManualAccelYEnable := BooleanToInteger(Form2.cbManualAccelY.Checked);
    HostData.BF6.bManualAccelZEnable := BooleanToInteger(Form2.cbManualAccelZ.Checked);
    HostData.BF6.bManualIntRefEnable := BooleanToInteger(Form2.cbManualIntRef.Checked);
    HostData.BF6.bManual5VDCEnable := BooleanToInteger(Form2.cbManual5VDC.Checked);
    HostData.BF6.bManualSetpointEnable := BooleanToInteger(Form2.cbManualSetpoint.Checked or cbManual.Checked);
    StrIndex := 1;
    MyStrToFloat(Form2.edtManualPowerInput.Text, StrIndex, TmpFloat);
    HostData.ManualPowerInput := round(TmpFloat);
    StrIndex := 1;
    MyStrToFloat(Form2.edtManualKma.Text, StrIndex, TmpFloat);
    HostData.ManualKma := round(TmpFloat);
    StrIndex := 1;
    MyStrToFloat(Form2.edtManualTemperature.Text, StrIndex, TmpFloat);
    HostData.ManualTemperature := round(TmpFloat);
    StrIndex := 1;
    MyStrToFloat(Form2.edtManualRVOutputVoltage.Text, StrIndex, TmpFloat);
    HostData.ManualRVOutputVoltage := round(TmpFloat);
    StrIndex := 1;
    MyStrToFloat(Form2.edtManualRVOutputCurrent.Text, StrIndex, TmpFloat);
    HostData.ManualRVOutputCurrent := round(TmpFloat);
    StrIndex := 1;
    MyStrToFloat(Form2.edtManualAccelX.Text, StrIndex, TmpFloat);
    HostData.ManualAccelX := round(TmpFloat);
    StrIndex := 1;
    MyStrToFloat(Form2.edtManualAccelY.Text, StrIndex, TmpFloat);
    HostData.ManualAccelY := round(TmpFloat);
    StrIndex := 1;
    MyStrToFloat(Form2.edtManualAccelZ.Text, StrIndex, TmpFloat);
    HostData.ManualAccelZ := round(TmpFloat);
    StrIndex := 1;
    MyStrToFloat(Form2.edtManualIntRef.Text, StrIndex, TmpFloat);
    HostData.ManualIntRef := round(TmpFloat);
    StrIndex := 1;
    MyStrToFloat(Form2.edtManual5VDC.Text, StrIndex, TmpFloat);
    HostData.Manual5VDCInput := round(TmpFloat);
    StrIndex := 1;
    if cbManual.checked then
      begin
      MyStrToFloat(edtManual.Text, StrIndex, TmpFloat);
      end
    else
      begin
      MyStrToFloat(Form2.edtManualSetpoint.Text, StrIndex, TmpFloat);
      end;
    HostData.ManualSetpoint := round(TmpFloat * 1000);
    StrIndex := 1;
  end;

//-------------------------------------------------------------------------------------
type
  PFuzzy = ^stFuzzy;

//-------------------------------------------------------------------------------------
//
// Calculates various values to complete the data structure for each fuzzy logic
//
procedure CalculateFuzzyVals(DataPointer: Pointer);
var
  FuzzyDataPtr: PFuzzy;
  i: integer;
begin
  FuzzyDataPtr := DataPointer;

  FuzzyDataPtr^.FuzzyMFCounts.InputCount[0] := 0;
  FuzzyDataPtr^.FuzzyMFCounts.InputCount[1] := 0;
  FuzzyDataPtr^.FuzzyMFCounts.InputCount[2] := 0;
  FuzzyDataPtr^.FuzzyMFCounts.OutputCount := 0;

  for i := 0 to 8 do
    begin
    if FuzzyDataPtr^.FuzzyInputMFs.MF[i].Shape = 0 then
      begin
      if (FuzzyDataPtr^.FuzzyInputMFs.MF[i].Point0 <> 0) or (FuzzyDataPtr^.FuzzyInputMFs.MF[i].Point1 <> 0) or (FuzzyDataPtr^.FuzzyInputMFs.MF[i].Point2 <> 0) then
        begin
        if (i < 3) then FuzzyDataPtr^.FuzzyMFCounts.InputCount[0] := FuzzyDataPtr^.FuzzyMFCounts.InputCount[0] + 1 ;
        if (i > 2) and (i < 6) then FuzzyDataPtr^.FuzzyMFCounts.InputCount[1] := FuzzyDataPtr^.FuzzyMFCounts.InputCount[1] + 1;
        if (i > 5) and (i < 9) then FuzzyDataPtr^.FuzzyMFCounts.InputCount[2] := FuzzyDataPtr^.FuzzyMFCounts.InputCount[2] + 1;
        end;
      end
    else
      begin
      if (FuzzyDataPtr^.FuzzyInputMFs.MF[i].Point0 <> 0) or (FuzzyDataPtr^.FuzzyInputMFs.MF[i].Point1 <> 0) or (FuzzyDataPtr^.FuzzyInputMFs.MF[i].Point2 <> 0) or (FuzzyDataPtr^.FuzzyInputMFs.MF[i].Point3 <> 0) then
        begin
        if (i < 3) then FuzzyDataPtr^.FuzzyMFCounts.InputCount[0] := FuzzyDataPtr^.FuzzyMFCounts.InputCount[0] + 1 ;
        if (i > 2) and (i < 6) then FuzzyDataPtr^.FuzzyMFCounts.InputCount[1] := FuzzyDataPtr^.FuzzyMFCounts.InputCount[1] + 1;
        if (i > 5) and (i < 9) then FuzzyDataPtr^.FuzzyMFCounts.InputCount[2] := FuzzyDataPtr^.FuzzyMFCounts.InputCount[2] + 1;
        end;
      end;
    end;

  for i := 0 to 2 do
    begin
      if FuzzyDataPtr^.FuzzyOutputMFs.MF[i].Shape = 0 then
        begin
          if (FuzzyDataPtr^.FuzzyOutputMFs.MF[i].Point0 <> 0) or (FuzzyDataPtr^.FuzzyOutputMFs.MF[i].Point1 <> 0) or (FuzzyDataPtr^.FuzzyOutputMFs.MF[i].Point2 <> 0) then
            begin
            FuzzyDataPtr^.FuzzyMFCounts.OutputCount := FuzzyDataPtr^.FuzzyMFCounts.OutputCount + 1;
            end;
        end
      else
        begin
        if (FuzzyDataPtr^.FuzzyOutputMFs.MF[i].Point0 <> 0) or (FuzzyDataPtr^.FuzzyOutputMFs.MF[i].Point1 <> 0) or (FuzzyDataPtr^.FuzzyOutputMFs.MF[i].Point2 <> 0) or (FuzzyDataPtr^.FuzzyOutputMFs.MF[i].Point3 <> 0) then
          begin
          FuzzyDataPtr^.FuzzyMFCounts.OutputCount := FuzzyDataPtr^.FuzzyMFCounts.OutputCount + 1;
          end;
        end;
    end;

  FuzzyDataPtr^.FuzzyMFCounts.NumberOfInputs := 0;

  for i := 0 to 2 do
    begin
    if FuzzyDataPtr^.FuzzyMFCounts.InputCount[i]<> 0 then FuzzyDataPtr^.FuzzyMFCounts.NumberOfInputs := FuzzyDataPtr^.FuzzyMFCounts.NumberOfInputs + 1;
    end;


end;

procedure TForm1.UploadFuzzyMFs(Sender: TObject);
begin
  if TabSheetFuzzy.TabIndex = 0 then
    begin
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[0].Shape := edtFuzzy1Input0Mf0Shape.ItemIndex;
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[0].Point0 := ConvertToFloat(edtFuzzy1Input0Mf0Pt0.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[0].Point1 := ConvertToFloat(edtFuzzy1Input0Mf0Pt1.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[0].Point2 := ConvertToFloat(edtFuzzy1Input0Mf0Pt2.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[0].Point3 := ConvertToFloat(edtFuzzy1Input0Mf0Pt3.Text, 1.0);

    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[1].Shape := edtFuzzy1Input0Mf1Shape.ItemIndex;
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[1].Point0 := ConvertToFloat(edtFuzzy1Input0Mf1Pt0.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[1].Point1 := ConvertToFloat(edtFuzzy1Input0Mf1Pt1.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[1].Point2 := ConvertToFloat(edtFuzzy1Input0Mf1Pt2.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[1].Point3 := ConvertToFloat(edtFuzzy1Input0Mf1Pt3.Text, 1.0);

    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[2].Shape := edtFuzzy1Input0Mf2Shape.ItemIndex;
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[2].Point0 := ConvertToFloat(edtFuzzy1Input0Mf2Pt0.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[2].Point1 := ConvertToFloat(edtFuzzy1Input0Mf2Pt1.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[2].Point2 := ConvertToFloat(edtFuzzy1Input0Mf2Pt2.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[2].Point3 := ConvertToFloat(edtFuzzy1Input0Mf2Pt3.Text, 1.0);

    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[3].Shape := edtFuzzy1Input1Mf0Shape.ItemIndex;
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[3].Point0 := ConvertToFloat(edtFuzzy1Input1Mf0Pt0.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[3].Point1 := ConvertToFloat(edtFuzzy1Input1Mf0Pt1.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[3].Point2 := ConvertToFloat(edtFuzzy1Input1Mf0Pt2.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[3].Point3 := ConvertToFloat(edtFuzzy1Input1Mf0Pt3.Text, 1.0);

    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[4].Shape := edtFuzzy1Input1Mf1Shape.ItemIndex;
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[4].Point0 := ConvertToFloat(edtFuzzy1Input1Mf1Pt0.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[4].Point1 := ConvertToFloat(edtFuzzy1Input1Mf1Pt1.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[4].Point2 := ConvertToFloat(edtFuzzy1Input1Mf1Pt2.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[4].Point3 := ConvertToFloat(edtFuzzy1Input1Mf1Pt3.Text, 1.0);

    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[5].Shape := edtFuzzy1Input1Mf2Shape.ItemIndex;
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[5].Point0 := ConvertToFloat(edtFuzzy1Input1Mf2Pt0.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[5].Point1 := ConvertToFloat(edtFuzzy1Input1Mf2Pt1.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[5].Point2 := ConvertToFloat(edtFuzzy1Input1Mf2Pt2.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[5].Point3 := ConvertToFloat(edtFuzzy1Input1Mf2Pt3.Text, 1.0);

    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[6].Shape := edtFuzzy1Input2Mf0Shape.ItemIndex;
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[6].Point0 := ConvertToFloat(edtFuzzy1Input2Mf0Pt0.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[6].Point1 := ConvertToFloat(edtFuzzy1Input2Mf0Pt1.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[6].Point2 := ConvertToFloat(edtFuzzy1Input2Mf0Pt2.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[6].Point3 := ConvertToFloat(edtFuzzy1Input2Mf0Pt3.Text, 1.0);

    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[7].Shape := edtFuzzy1Input2Mf1Shape.ItemIndex;
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[7].Point0 := ConvertToFloat(edtFuzzy1Input2Mf1Pt0.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[7].Point1 := ConvertToFloat(edtFuzzy1Input2Mf1Pt1.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[7].Point2 := ConvertToFloat(edtFuzzy1Input2Mf1Pt2.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[7].Point3 := ConvertToFloat(edtFuzzy1Input2Mf1Pt3.Text, 1.0);

    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[8].Shape := edtFuzzy1Input2Mf2Shape.ItemIndex;
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[8].Point0 := ConvertToFloat(edtFuzzy1Input2Mf2Pt0.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[8].Point1 := ConvertToFloat(edtFuzzy1Input2Mf2Pt1.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[8].Point2 := ConvertToFloat(edtFuzzy1Input2Mf2Pt2.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[8].Point3 := ConvertToFloat(edtFuzzy1Input2Mf2Pt3.Text, 1.0);

    CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[0].Shape := edtFuzzy1OutputMf0Shape.ItemIndex;
    CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[0].Point0 := ConvertToFloat(edtFuzzy1OutputMf0Pt0.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[0].Point1 := ConvertToFloat(edtFuzzy1OutputMf0Pt1.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[0].Point2 := ConvertToFloat(edtFuzzy1OutputMf0Pt2.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[0].Point3 := ConvertToFloat(edtFuzzy1OutputMf0Pt3.Text, 1.0);

    CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[1].Shape := edtFuzzy1OutputMf1Shape.ItemIndex;
    CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[1].Point0 := ConvertToFloat(edtFuzzy1OutputMf1Pt0.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[1].Point1 := ConvertToFloat(edtFuzzy1OutputMf1Pt1.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[1].Point2 := ConvertToFloat(edtFuzzy1OutputMf1Pt2.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[1].Point3 := ConvertToFloat(edtFuzzy1OutputMf1Pt3.Text, 1.0);

    CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[2].Shape := edtFuzzy1OutputMf2Shape.ItemIndex;
    CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[2].Point0 := ConvertToFloat(edtFuzzy1OutputMf2Pt0.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[2].Point1 := ConvertToFloat(edtFuzzy1OutputMf2Pt1.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[2].Point2 := ConvertToFloat(edtFuzzy1OutputMf2Pt2.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[2].Point3 := ConvertToFloat(edtFuzzy1OutputMf2Pt3.Text, 1.0);

    CDProfileData.Fuzzy1Data.FuzzyOutputInfo.Min := ConvertToFloat(edtFuzzy1OutputMin.Text, 1.0);
    CDProfileData.Fuzzy1Data.FuzzyOutputInfo.Max := ConvertToFloat(edtFuzzy1OutputMax.Text, 1.0);

    CalculateFuzzyVals(@CDProfileData.Fuzzy1Data);
    end
  else
    begin
      if TabSheetFuzzy.TabIndex = 1 then
        begin
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[0].Shape := edtFuzzy2Input0Mf0Shape.ItemIndex;
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[0].Point0 := ConvertToFloat(edtFuzzy2Input0Mf0Pt0.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[0].Point1 := ConvertToFloat(edtFuzzy2Input0Mf0Pt1.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[0].Point2 := ConvertToFloat(edtFuzzy2Input0Mf0Pt2.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[0].Point3 := ConvertToFloat(edtFuzzy2Input0Mf0Pt3.Text, 1.0);

          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[1].Shape := edtFuzzy2Input0Mf1Shape.ItemIndex;
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[1].Point0 := ConvertToFloat(edtFuzzy2Input0Mf1Pt0.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[1].Point1 := ConvertToFloat(edtFuzzy2Input0Mf1Pt1.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[1].Point2 := ConvertToFloat(edtFuzzy2Input0Mf1Pt2.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[1].Point3 := ConvertToFloat(edtFuzzy2Input0Mf1Pt3.Text, 1.0);

          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[2].Shape := edtFuzzy2Input0Mf2Shape.ItemIndex;
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[2].Point0 := ConvertToFloat(edtFuzzy2Input0Mf2Pt0.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[2].Point1 := ConvertToFloat(edtFuzzy2Input0Mf2Pt1.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[2].Point2 := ConvertToFloat(edtFuzzy2Input0Mf2Pt2.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[2].Point3 := ConvertToFloat(edtFuzzy2Input0Mf2Pt3.Text, 1.0);

          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[3].Shape := edtFuzzy2Input1Mf0Shape.ItemIndex;
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[3].Point0 := ConvertToFloat(edtFuzzy2Input1Mf0Pt0.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[3].Point1 := ConvertToFloat(edtFuzzy2Input1Mf0Pt1.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[3].Point2 := ConvertToFloat(edtFuzzy2Input1Mf0Pt2.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[3].Point3 := ConvertToFloat(edtFuzzy2Input1Mf0Pt3.Text, 1.0);

          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[4].Shape := edtFuzzy2Input1Mf1Shape.ItemIndex;
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[4].Point0 := ConvertToFloat(edtFuzzy2Input1Mf1Pt0.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[4].Point1 := ConvertToFloat(edtFuzzy2Input1Mf1Pt1.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[4].Point2 := ConvertToFloat(edtFuzzy2Input1Mf1Pt2.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[4].Point3 := ConvertToFloat(edtFuzzy2Input1Mf1Pt3.Text, 1.0);

          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[5].Shape := edtFuzzy2Input1Mf2Shape.ItemIndex;
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[5].Point0 := ConvertToFloat(edtFuzzy2Input1Mf2Pt0.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[5].Point1 := ConvertToFloat(edtFuzzy2Input1Mf2Pt1.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[5].Point2 := ConvertToFloat(edtFuzzy2Input1Mf2Pt2.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[5].Point3 := ConvertToFloat(edtFuzzy2Input1Mf2Pt3.Text, 1.0);

          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[6].Shape := edtFuzzy2Input2Mf0Shape.ItemIndex;
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[6].Point0 := ConvertToFloat(edtFuzzy2Input2Mf0Pt0.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[6].Point1 := ConvertToFloat(edtFuzzy2Input2Mf0Pt1.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[6].Point2 := ConvertToFloat(edtFuzzy2Input2Mf0Pt2.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[6].Point3 := ConvertToFloat(edtFuzzy2Input2Mf0Pt3.Text, 1.0);

          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[7].Shape := edtFuzzy2Input2Mf1Shape.ItemIndex;
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[7].Point0 := ConvertToFloat(edtFuzzy2Input2Mf1Pt0.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[7].Point1 := ConvertToFloat(edtFuzzy2Input2Mf1Pt1.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[7].Point2 := ConvertToFloat(edtFuzzy2Input2Mf1Pt2.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[7].Point3 := ConvertToFloat(edtFuzzy2Input2Mf1Pt3.Text, 1.0);

          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[8].Shape := edtFuzzy2Input2Mf2Shape.ItemIndex;
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[8].Point0 := ConvertToFloat(edtFuzzy2Input2Mf2Pt0.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[8].Point1 := ConvertToFloat(edtFuzzy2Input2Mf2Pt1.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[8].Point2 := ConvertToFloat(edtFuzzy2Input2Mf2Pt2.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[8].Point3 := ConvertToFloat(edtFuzzy2Input2Mf2Pt3.Text, 1.0);

          CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[0].Shape := edtFuzzy1OutputMf0Shape.ItemIndex;
          CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[0].Point0 := ConvertToFloat(edtFuzzy1OutputMf0Pt0.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[0].Point1 := ConvertToFloat(edtFuzzy1OutputMf0Pt1.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[0].Point2 := ConvertToFloat(edtFuzzy1OutputMf0Pt2.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[0].Point3 := ConvertToFloat(edtFuzzy1OutputMf0Pt3.Text, 1.0);

          CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[1].Shape := edtFuzzy1OutputMf1Shape.ItemIndex;
          CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[1].Point0 := ConvertToFloat(edtFuzzy1OutputMf1Pt0.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[1].Point1 := ConvertToFloat(edtFuzzy1OutputMf1Pt1.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[1].Point2 := ConvertToFloat(edtFuzzy1OutputMf1Pt2.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[1].Point3 := ConvertToFloat(edtFuzzy1OutputMf1Pt3.Text, 1.0);

          CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[2].Shape := edtFuzzy2OutputMf2Shape.ItemIndex;
          CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[2].Point0 := ConvertToFloat(edtFuzzy2OutputMf2Pt0.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[2].Point1 := ConvertToFloat(edtFuzzy2OutputMf2Pt1.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[2].Point2 := ConvertToFloat(edtFuzzy2OutputMf2Pt2.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[2].Point3 := ConvertToFloat(edtFuzzy2OutputMf2Pt3.Text, 1.0);

          CDProfileData.Fuzzy2Data.FuzzyOutputInfo.Min := ConvertToFloat(edtFuzzy2OutputMin.Text, 1.0);
          CDProfileData.Fuzzy2Data.FuzzyOutputInfo.Max := ConvertToFloat(edtFuzzy2OutputMax.Text, 1.0);

          CalculateFuzzyVals(@CDProfileData.Fuzzy2Data);
        end;
    end;
end;

//******************************************************************************************************************
//******************************************************************************************************************


  var
   Delay1: integer;

procedure TForm1.Timer1Timer(Sender: TObject);
var
  Config_CC, CalibrationScaling, StrIndex: integer;
  TmpStr: string;
  VoltageKFactor, TmpFloat: single;
begin
  Timer1.Enabled := False;
  TmpFloat := 0.0;

  ProcessManual();

  if bReprogramming then
    begin
    ProgressBar1.Position := FirmwarePageNumber;
    if ProgressBar1.Position >= ProgressBar1.Max then
      pnlReadConfig.Visible := FALSE;
    end;
  if bUploadCDFirmwareStarted then
    begin
    ProgressBar2.Position := FirmwarePageNumber;
    if ProgressBar2.Position >= ProgressBar2.Max then
      ProgressBar2.Position := 0;
    end;

  ProcessEEPROMState();

  if (SysVars.OverCurrentFlag <> 0) then
     begin
     lePowerCurrent.Font.Color := clWhite;
     lePowerCurrent.Color := clRed;
     SetpointData.EnableBits := SetpointData.EnableBits and not ENABLE_POWER;
     btnPower.Caption := 'Turn ON';
     cbManual.Checked := false;
     end;

  bGetCCData:= (rgMode.ItemIndex = 0) and ((SetpointData.EnableBits and ENABLE_POWER) <> 0);

  lblEventsAvaliable.visible := CD_70.ENG_NumberEventLogEntries <> OldNumberOfEvents;

  if (Delay1 <> 0) then Delay1 := Delay1 - 1;
  if (Delay1 = 0) then
    begin
    Delay1 := 3;

    if (cbTemperatureOutput.ItemIndex <> 0) then cbHoldMode.ItemIndex := 0;

    lblTripPoint.Enabled := (cbTemperatureOutput.ItemIndex <> 0) or (cbHoldMode.ItemIndex <> 0);
    edtTripPoint.Enabled := (cbTemperatureOutput.ItemIndex <> 0) or (cbHoldMode.ItemIndex <> 0);

    lblInternalPullup.Enabled := (SetpointData.EnableBits and ENABLE_POWER) <> 0;
    btnInternalPullup.Enabled := (SetpointData.EnableBits and ENABLE_POWER) <> 0;

    lblHoldValue.Enabled := (cbHoldMode.ItemIndex <> 0);
    edtHoldValue.Enabled := (cbHoldMode.ItemIndex <> 0);

    lblHoldMode.Enabled := (cbTemperatureOutput.ItemIndex  = 0);
    cbHoldMode.Enabled := (cbTemperatureOutput.ItemIndex  = 0);

    TmpStr := cbProfileName.Text;
    TmpStr := stringreplace(TmpStr, 'V3-', '', [rfReplaceAll, rfIgnoreCase]);
    TmpStr := stringreplace(TmpStr, 'R3-', '', [rfReplaceAll, rfIgnoreCase]);
    if rgProfileType.ItemIndex = 0 then       // Profile Type = Voltage
      begin
      cbProfileName.Text := 'V3-' + TmpStr;
      end
    else
      begin
      cbProfileName.Text := 'R3-' + TmpStr;
      end;

    cbDigOut1.Enabled := cbDigMode1.ItemIndex = 1;
    edtDigFreqIn1.Enabled := cbDigMode1.ItemIndex = 2;
    edtDigFreqOut1.Enabled := (cbDigMode1.ItemIndex = 3) or (cbDigMode1.ItemIndex = 4);
    edtDigPwm1.Enabled := cbDigMode1.ItemIndex = 4;

    cbDigOut2.Enabled := cbDigMode2.ItemIndex = 1;
    edtDigFreqIn2.Enabled := cbDigMode2.ItemIndex = 2;
    edtDigFreqOut2.Enabled := (cbDigMode2.ItemIndex = 3) or (cbDigMode2.ItemIndex = 4);
    edtDigPwm2.Enabled := cbDigMode2.ItemIndex = 4;

    cbDigOut3.Enabled := cbDigMode3.ItemIndex = 1;
    edtDigFreqIn3.Enabled := cbDigMode3.ItemIndex = 2;
    edtDigFreqOut3.Enabled := (cbDigMode3.ItemIndex = 3) or (cbDigMode3.ItemIndex = 4);
    edtDigPwm3.Enabled := cbDigMode3.ItemIndex = 4;

    cbDigOut4.Enabled := cbDigMode4.ItemIndex = 1;
    edtDigFreqIn4.Enabled := cbDigMode4.ItemIndex = 2;
    edtDigFreqOut4.Enabled := (cbDigMode4.ItemIndex = 3) or (cbDigMode4.ItemIndex = 4);
    edtDigPwm4.Enabled := cbDigMode4.ItemIndex = 4;

    cbDigOut5.Enabled := cbDigMode5.ItemIndex = 1;
    edtDigFreqIn5.Enabled := cbDigMode5.ItemIndex = 2;
    edtDigFreqOut5.Enabled := (cbDigMode5.ItemIndex = 3) or (cbDigMode5.ItemIndex = 4);
    edtDigPwm5.Enabled := cbDigMode5.ItemIndex = 4;

    cbDigOut6.Enabled := cbDigMode6.ItemIndex = 1;
    edtDigFreqOut6.Enabled := (cbDigMode6.ItemIndex = 2) or (cbDigMode6.ItemIndex = 3);
    edtDigPwm6.Enabled := cbDigMode6.ItemIndex = 3;

    if (DUT_InvalidCounter > 20) and (TabSheet4.Enabled) then
      begin
      TabSheet4.Enabled := false;
      leKmaCounts.Text := '';
      leAD7814Counts.Text := '';
      leTempDegC.Text := '';
      leTempHz.Text := '';
      leRVVoltsCounts.Text := '';
      leRvVolts.Text := '';
      leRVCurrentCounts.Text := '';
      leRvCurrent.Text := '';
      leKmaPeriod.Text := '';
      leKmaFreq.Text := '';
      leSlave1Period.Text := '';
      leSlave1Freq.Text := '';
      leSlave2Period.Text := '';
      leSlave2Freq.Text := '';
      leSlave3Period.Text := '';
      leSlave3Freq.Text := '';
      leMasterPeriod.Text := '';
      leMasterFreq.Text := '';
      leMasterIndex.Text := '';
      leEepromAddress.Text := '';
      leSetpoint.Text := '';
      leFeedback.Text := '';
      lePwmCounts.Text := '';
      lePwmVolts.Text := '';
      leFirmwareVersion.Text := '';
      leOhmsKFactor.Text := '';
      leVoltsKFactor.Text := '';
      leEEPROMConfig.Text := '';
      leTrippoint.Text := '';
      leKmaSamples.Text := '';
      leHoldValue.Text := '';
      cbMasterEnable.Checked := FALSE;
      cbNoSlave1Input.Checked := FALSE;
      cbNoSlave2Input.Checked := FALSE;
      cbNoSlave3Input.Checked := FALSE;
      cbSerialVerify.Checked := FALSE;
      cbRVVoltage.Checked := FALSE;
      cbRvResistance.Checked := FALSE;
      cbKmaReverse.Checked := FALSE;
      cbLowHoldEnabled.Checked := FALSE;
      cbLowFuelSelected.Checked := FALSE;
      cbLowFuelSelected.Enabled := FALSE;
      cbHoldLogicSelected.Checked := FALSE;
      cbHoldLogicSelected.Enabled := FALSE;
      cbLowFuelActiveHigh.Checked := FALSE;
      cbLowFuelActiveHigh.Enabled := FALSE;
      cbLowFuelActiveLow.Checked := FALSE;
      cbLowFuelActiveLow.Enabled := FALSE;
      cbHoldGreater.Checked := FALSE;
      cbHoldGreater.Enabled := FALSE;
      cbHoldLessThan.Checked := FALSE;
      cbHoldLessThan.Enabled := FALSE;
      end;
    if (DUT_InvalidCounter < 20) and (not TabSheet4.Enabled) then
      begin
      TabSheet4.Enabled := true;
      end;

    StrIndex := 1;
    MyStrToFloat(edtDigFreqOut1.Text, StrIndex, TmpFloat);
    SetpointData.DIGIO_Frequency_Setpoint[0] := round(TmpFloat * 100);
    StrIndex := 1;
    MyStrToFloat(edtDigFreqOut2.Text, StrIndex, TmpFloat);
    SetpointData.DIGIO_Frequency_Setpoint[1] := round(TmpFloat * 100);
    StrIndex := 1;
    MyStrToFloat(edtDigFreqOut3.Text, StrIndex, TmpFloat);
    SetpointData.DIGIO_Frequency_Setpoint[2] := round(TmpFloat * 100);
    StrIndex := 1;
    MyStrToFloat(edtDigFreqOut4.Text, StrIndex, TmpFloat);
    SetpointData.DIGIO_Frequency_Setpoint[3] := round(TmpFloat * 100);
    StrIndex := 1;
    MyStrToFloat(edtDigFreqOut5.Text, StrIndex, TmpFloat);
    SetpointData.DIGIO_Frequency_Setpoint[4] := round(TmpFloat * 100);
    StrIndex := 1;
    MyStrToFloat(edtDigFreqOut6.Text, StrIndex, TmpFloat);
    SetpointData.DIGIO_Frequency_Setpoint[5] := round(TmpFloat * 100);

    StrIndex := 1;
    MyStrToFloat(edtDigPwm1.Text, StrIndex, TmpFloat);
    SetpointData.DIGIO_DutyCycle[0] := round(TmpFloat * 100);
    StrIndex := 1;
    MyStrToFloat(edtDigPwm2.Text, StrIndex, TmpFloat);
    SetpointData.DIGIO_DutyCycle[1] := round(TmpFloat * 100);
    StrIndex := 1;
    MyStrToFloat(edtDigPwm3.Text, StrIndex, TmpFloat);
    SetpointData.DIGIO_DutyCycle[2] := round(TmpFloat * 100);
    StrIndex := 1;
    MyStrToFloat(edtDigPwm4.Text, StrIndex, TmpFloat);
    SetpointData.DIGIO_DutyCycle[3] := round(TmpFloat * 100);
    StrIndex := 1;
    MyStrToFloat(edtDigPwm5.Text, StrIndex, TmpFloat);
    SetpointData.DIGIO_DutyCycle[4] := round(TmpFloat * 100);
    StrIndex := 1;
    MyStrToFloat(edtDigPwm6.Text, StrIndex, TmpFloat);
    SetpointData.DIGIO_DutyCycle[5] := round(TmpFloat * 100);

    btnStart.Enabled := cbFPGAProfileName.ItemIndex >= 0;

    StrIndex := 1;
    MyStrToFloat(leEEPROMConfig.Text, StrIndex, TmpFloat);
    Config_CC := round(TmpFloat);

    StrIndex := 1;
    MyStrToFloat(edtManual.Text, StrIndex, TmpFloat);

    if (Config_CC and 1) = 0 then
      SetpointData.ManualSetpoint := round(TmpFloat * 10)
    else
      begin
      StrIndex := 1;
      MyStrToFloat(leVoltsKFactor.Text, StrIndex, VoltageKFactor);
      CalibrationScaling := 10000 div (round((VoltageKFactor / 2976.0) * 20.0) * 5);
      SetpointData.ManualSetpoint := round(TmpFloat * CalibrationScaling);
      end;

    SetpointData.DIGIO_OutputLevel[0] := cbDigOut1.ItemIndex;
    SetpointData.DIGIO_OutputLevel[1] := cbDigOut2.ItemIndex;
    SetpointData.DIGIO_OutputLevel[2] := cbDigOut3.ItemIndex;
    SetpointData.DIGIO_OutputLevel[3] := cbDigOut4.ItemIndex;
    SetpointData.DIGIO_OutputLevel[4] := cbDigOut5.ItemIndex;
    SetpointData.DIGIO_OutputLevel[5] := cbDigOut6.ItemIndex;

    end;

  SetpointData.DIGIO_Configuration[0] := cbDigMode1.ItemIndex + 1;
  if SetpointData.DIGIO_Configuration[0] = 7 then SetpointData.DIGIO_Configuration[0] := 8;
  SetpointData.DIGIO_Configuration[1] := cbDigMode2.ItemIndex + 1;
  if SetpointData.DIGIO_Configuration[1] = 6 then SetpointData.DIGIO_Configuration[1] := 7;
  SetpointData.DIGIO_Configuration[2] := cbDigMode3.ItemIndex + 1;
  SetpointData.DIGIO_Configuration[3] := cbDigMode4.ItemIndex + 1;
  SetpointData.DIGIO_Configuration[4] := cbDigMode5.ItemIndex + 1;
  SetpointData.DIGIO_Configuration[5] := cbDigMode6.ItemIndex + 1;
  if SetpointData.DIGIO_Configuration[5] >= 3 then SetpointData.DIGIO_Configuration[5] := SetpointData.DIGIO_Configuration[5] + 1;

  if cbCommunicationsEnable.Checked then SetpointData.EnableBits := SetpointData.EnableBits or ENABLE_COMMUNICATIONS;
  if not cbCommunicationsEnable.Checked then SetpointData.EnableBits := SetpointData.EnableBits and not ENABLE_COMMUNICATIONS;

  if cbManual.Checked then SetpointData.EnableBits := SetpointData.EnableBits or ENABLE_MANUAL;
  if not cbManual.Checked then SetpointData.EnableBits := SetpointData.EnableBits and not ENABLE_MANUAL;

  ProcessSerialReceive;

  // Serial_Timeout is reset when TestData is received

  ProcessSerialTimeout();

  StatusBar1.Panels[2].Text := format('%u %u %u %u', [WaitTime, ThreadCounter, SerialCounter, SysVarsCC.DUT_UpdateCounter]);

  // Display any messages from Arduino

  while NextIn <> NextOut do
    begin
      if MessageQueue[NextOut].Location = 0 then
        memo1.Lines.Add(MessageQueue[NextOut].MsgStr);
      if MessageQueue[NextOut].Location = 1 then
        StatusBar1.Panels[0].Text := MessageQueue[NextOut].MsgStr;
      if MessageQueue[NextOut].Location = 3 then
        memo3.Lines.Add(MessageQueue[NextOut].MsgStr);
    NextOut := (NextOut + 1) and $FF;
    end;

  Timer1.Enabled := True;
end;

//**************************************************************************************************************************************

procedure TForm1.InitializeTest;
begin
  bAbort := False;
  FillChar(SysVars, sizeof(SysVars), 0);
end;

//******************************************************************************************************************

procedure TForm1.btnStartClick(Sender: TObject);
var
  OldUpdateCounter, CurrentUpdateCounter, TestCase, i: integer;
  VerifyRetryCounter, RetryCounter, MaxSetpoint, MaxIndex, NewVoltageKFactor, NewOhmsKFactor: word;
  CurrentVolts: single;
label
  ExitNow, ExitToTop;
begin
  if btnStart.Caption = 'ABORT' then
    begin
    bAbort := True;
    end;
  btnStart.Caption := 'ABORT';

  memo1.Clear;
  EventLogger.WriteStr(' ', False);
  EventLogger.WriteStr('=================================================================================================', False);
  EventLogger.WriteStr('---------------------------------------  START PROGRAMMING  -------------------------------------', False);
  EventLogger.WriteStr('=================================================================================================', False);
  EventLogger.WriteStr(format('Name of Tester: %s', [edtNameOfTester.Text]), True);
  EventLogger.WriteStr(format('Product Serial Number:  %s', [edtSerialNumber.Text]), True);
  EventLogger.WriteStr(format('Name of Profile: %s', [cbFPGAProfileName.Text]), True);
  EventLogger.WriteStr(' ', False);

  TestCase := 0;
  bEndTest := False;
  WaitTime := 0;
  while (not bEndTest) do
    begin
    ExitToTop:
      if bAbort then
        begin
        InitializeTest();
        TestMessage('PROGRAMMING ABORTED BY USER!!', 0);
        EventLogger.WriteStr('------>> PROGRAMMING ABORTED BY USER <------', True);
        goto ExitNow;
        end;

    // If WaitTime <> 0, then Add a 10mS delay into loop time.

    if WaitTime <> 0 then
      WaitTime := WaitTime - 1;
    Sleep(10);
    Application.ProcessMessages;

    // This Case statement controls each test we are running

    case (TestCase) of
      0:
        begin
        EventLogger.WriteStr('Initializing Programming', True);
        StatusBar1.Panels[0].Text := 'Initialize Unit for programming';
        InitializeTest();
        WaitTime := 50;
        TestCase := 100;
        end;

      //-----------------------------------------------------------------------------------------------------------

      100:
        begin
        if WaitTime <> 0 then
          goto ExitToTop;
        EventLogger.WriteStr('Turn on Power', True);
        TestMessage('Turning On Power', 0);
        cbCommunicationsEnable.Checked := True;
        PowerControl(TRUE);
        CurrentUpdateCounter := 0;
        OldUpdateCounter := 0;
        WaitTime := 1000;
        TestCase := 200;
        end;

      200:
        begin
        //
        // Once we see DUT_UpdateCounter change at least 10 times, we can safely assume that the Programmer is communicating with
        // Sender, therefore reduce WaitTime
        //
        if SysVarsCC.DUT_UpdateCounter <> OldUpdateCounter then
           begin
           OldUpdateCounter := SysVarsCC.DUT_UpdateCounter;
           CurrentUpdateCounter := CurrentUpdateCounter + 1;
           end;
        if (WaitTime > 20) and (CurrentUpdateCounter > 10) then
          WaitTime := 20;
        //
        // If ENABLE_POWER bit resets, then excess current has been detected and power was automatically turned off, therefore abort test
        //
        if (SetpointData.EnableBits and ENABLE_POWER) = 0 then
          begin
          EventLogger.WriteStr('ERROR: Excess Current Detected, Programming Aborted', True);
          TestMessage('ERROR: Excess Current Detected', 0);
          TestCase := 10000;
          goto ExitToTop;
          end;
        //
        // Wait for WaitTime to reach zero
        //
        if WaitTime <> 0 then
          goto ExitToTop;
        //
        // If DUT_UpdateCounter hasn't changed, then we aren't communicating with Sender, therefore abort test.
        //
        if (CurrentUpdateCounter < 10) then
          begin
          EventLogger.WriteStr('ERROR: Unable to Communicate with Fuel Sender, Programming Aborted', True);
          TestMessage('ERROR: Unable to Communicate with Sender', 0);
          TestCase := 10000;
          goto ExitToTop;
          end;
        RetryCounter := 3;
        WaitTime := 50;
        TestCase := 300;
        if bVerifyProfile then
           begin
           TestCase := 750;
           end;
        goto ExitToTop;
        end;

      300:
        begin
        if WaitTime <> 0 then
          goto ExitToTop;
        //
        // Load NewEEPROMData structure with Profile Data.  Setting InitialCount to 30 will cause NewEEPROMData to be sent to Programmer.
        // Programmer will then upload profile data to Sender.
        //
        NewEEPROMData.EEPROM_Config := FPGAProfileData.EEPROM_Config;
        NewEEPROMData.HoldValue := FPGAProfileData.HoldValue;
        NewEEPROMData.KmaSamples := FPGAProfileData.KmaSamples;
        NewEEPROMData.Trippoint := FPGAProfileData.Trippoint;
        FillChar(NewEEPROMData.Reserved, Sizeof(NewEEPROMData.Reserved), 0);
        for i := 0 to 509 do NewEEPROMData.Profile[i] := FPGAProfileData.Profile[i];
        bSerialWriteProfile := true;
        WaitTime := 50;
        TestCase := 400;
        goto ExitToTop;
        end;

      400:
        begin
        //
        // Need Delay here to make sure Serial Communication routine has started the upload process
        //
        if WaitTime <> 0 then
          goto ExitToTop;
        //
        // Wait for EEPROM_MODE to reset, indicating that Profile Data has been uploaded to Sender
        //
        if (SysVarsCC.EEPROM_Mode <> EEPROM_MODE_UPDATE_PROFILE) then
          begin
          TestMessage('Done Writing Profile Information', 0);
          CurrentEEPROMData.EEPROM_Config := FPGAProfileData.EEPROM_Config;
          CurrentEEPROMData.Trippoint := FPGAProfileData.Trippoint;
          CurrentEEPROMData.KmaSamples := FPGAProfileData.KmaSamples;
          CurrentEEPROMData.HoldValue := FPGAProfileData.HoldValue;
          for i := 0 to 509 do CurrentEEPROMData.Profile[i] := FPGAProfileData.Profile[i];
          leEEPROMConfig.Text := format('%u', [CurrentEEPROMData.EEPROM_Config]);
          leKmaSamples.Text := format('%u', [CurrentEEPROMData.KmaSamples]);
          leTrippoint.Text := format('%u', [CurrentEEPROMData.Trippoint]);
          leHoldValue.Text := format('%u', [CurrentEEPROMData.HoldValue]);
          pnlReadConfig.Visible := FALSE;
          //
          // Default to not running Calibration Routine
          //
          TestCase := 750;
          //
          // Only run calibration if Checkbox is checked
          //
          if cbCalibrateOutput.Checked then
            begin
            if (CurrentEEPROMData.EEPROM_Config and 1) <> 0 then
              begin
              TestMessage('Calibrating Voltage Output', 0);
              TestCase := 500;
              end
            else
              begin
              TestMessage('Calibrating Resistive Output', 0);
              TestCase := 2000;
              end;
            end;
          goto ExitToTop;
          end;
        end;

      500:
        begin
        //
        // Start of Voltage Calibration Logic
        //
        MaxSetpoint := 0;
        MaxIndex := 0;
        //
        // Look for maximum setpoint value that is less than or equal to 5.00 volts
        //
        for i := 0 to 509 do
          begin
          if (NewEEPROMData.Profile[i] > MaxSetpoint) and (NewEEPROMData.Profile[i] <= 500) then
            begin
            MaxSetpoint := NewEEPROMData.Profile[i];
            MaxIndex := i;
            end;
          end;
        //
        // Cause Sender to enter Verify Mode.  This will override MasterIndex driving Setpoint lookup table.
        //
        SerialVerifyData.VerifyMode := SERIAL_VERIFY_MODE_READ;
        SerialVerifyData.VerifyAddress := MaxIndex + 18;
        SerialVerifyData.VerifyData := 0;
        bSerialVerifyMode := true;
        WaitTime := 10000;
        TestCase := 550;
        end;

      550:
        begin
        //
        // At this point, we are waiting for Feedback to reach the Setpoint.  However, if PWM counts reaches 4095 while waiting, this would
        //  indicate that there is no current source connected to the RV output (i.e., divider board), therefore to provide a current
        //  source, turn on the internal Pull-up.
        //
        if (SysVarsCC.DUT_PWM_Counts = 4095) and ((SetpointData.EnableBits and ENABLE_PULLUP) = 0) then
          begin
          TestMessage('Info: Turning on Internal Pull-up', 0);
          btnInternalPullupClick(Self);
          end;
        //
        // Normally, the logic below this IF statement will cause WaitTime to be set to 500, thereby skipping over 2000.  However,
        // should the Feedback never equal the Setpoint, then WaitTime would count down to 2000, causing the Calibration routine to
        // abort.
        //
        if (WaitTime = 2000) then
          begin
          TestMessage('Error: SP <> FB', 0);
          TestCase := 1050;
          goto ExitToTop;
          end;
        //
        // Wait for Feedback to Equal Setpoint,  Once they do, reset WaitTime down to 500
        //
        if (SysVarsCC.DUT_Setpoint = SysVarsCC.DUT_Feedback) and (WaitTime > 500) then WaitTime := 500;
        //
        // Once WaitTime reaches zero and SP == FB, calculate a new Voltage K Factor
        //
        if (SysVarsCC.DUT_Setpoint = SysVarsCC.DUT_Feedback) and (WaitTime = 0) then
          begin
          //
          //  Abort calibration if RV voltage measured by Tester isn't within .15 volts of voltage being measured by Sender.
          //
          if abs(DUT_RVVolts - RVVoltage) > 0.15 then
            begin
            TestMessage(format('Error: Tester RV Voltage (%1.2f) and Sender RV Voltage (%1.2f) differ by more than 0.15 volts', [RVVoltage, DUT_RVVolts]), 0);
            TestCase := 1050;
            goto ExitToTop;
            end;
          //
          // If CurrentVolts is < 0.1, then set to 0.1.  This is to prevent a divide by zero from happening
          //
          CurrentVolts := RVVoltage;
          if CurrentVolts < 0.1 then CurrentVolts := 0.1;
          //
          // Calculate a new Voltage K Factor value
          //
          NewVoltageKFactor := round(CurrentEEPROMData.VoltageKFactor * (SysVarsCC.DUT_Setpoint / 100.0) /  CurrentVolts) ;
          //
          // If difference bewteen current K factor and new K factor is greater than 5, then upload new value to Sender
          //
          if abs(CurrentEEPROMData.VoltageKFactor - NewVoltageKFactor) > 5 then
            begin
            //
            // Limit to 3500, nominal is normally around 2600
            //
            if NewVoltageKFactor > 3500 then NewVoltageKFactor := 3500;
            TestMessage(format('Voltage K Factor: Current: %u  New: %u', [CurrentEEPROMData.VoltageKFactor, NewVoltageKFactor]), 0);
            CurrentEEPROMData.VoltageKFactor := NewVoltageKFactor;
            leVoltsKFactor.Text := format('%u', [NewVoltageKFactor]);
            btnUpdateConfigClick(self);
            WaitTime := 75;
            TestCase := 600;
            end
          else
            begin
            WaitTime := 75;
            TestCase := 650;
            end;
          end;
        end;

      600:
        begin;
        if WaitTime <> 0 then
          goto ExitToTop;
        //
        // Once the new Config Data has been written to EEPROM, cbSerialVerify checkbox will automatically be cleared.
        //  Once checkbox as cleared, go back and run Calibration logic again.  Continue running calibration logic until
        //  difference between new and old is 5 counts or less.
        //
        if  not cbSerialVerify.Checked then TestCase := 500;
        end;

      650:
        begin
        //
        // This point is reached once we have a good voltage calibration value
        //
        if WaitTime <> 0 then
          goto ExitToTop;
        //
        //Now that we have a good Calibration value, adjust it based on Scale Factor
        //
        NewVoltageKFactor := round(NewVoltageKFactor * FPGAProfileData.CalibrationScaling);
        TestMessage(format('Scaling K Factor: Current: %u  Final: %u', [CurrentEEPROMData.VoltageKFactor, NewVoltageKFactor]), 0);
        leVoltsKFactor.Text := format('%u', [NewVoltageKFactor]);
        btnUpdateConfigClick(self);
        WaitTime := 75;
        TestCase := 700;
        end;

      700:
        begin;
        if WaitTime <> 0 then
          goto ExitToTop;
        //
        // Wait for New Config Data to be written
        //
        if  not cbSerialVerify.Checked then
          begin
          TestMessage('Done calibrating voltage output..', 0);
          VerifyRetryCounter := 3;
          TestCase := 750;
          end;
        end;

      750:
        begin
        //
        // Now that the profile has been written and the Sender calibrated, time to verify everything.
        //
        TestMessage('Verify Config and Profile Data', 0);
        lblMessage.Caption:= 'Verify Configuration and Profile Information';
        pnlReadConfig.Visible := TRUE;
        bRefreshProfile := TRUE;
        bCurrentEEPROMDataValid := false;
        TestCase := 800;
        end;

      800:
        begin
        //
        // This state basically makes sure Serial Communications routine has started reading the config and profile data
        //
        ProgressBar1.Position:= SysVarsCC.DUT_EEPROM_Address shr 1;
        if (SysVarsCC.EEPROM_Mode = EEPROM_MODE_READ_PROFILE) then TestCase := 850;
        end;

      850:
        begin
        //
        // Now wait for all of the config and profile data to be downloaded from Sender to Programmer, and from Programmer to PC
        //
        ProgressBar1.Position:= SysVarsCC.DUT_EEPROM_Address shr 1;
        if (SysVarsCC.EEPROM_Mode <> EEPROM_MODE_READ_PROFILE) then
          begin
          pnlReadConfig.Visible := FALSE;
          WaitTime := 100;
          TestCase := 900;
          end;
        end;

      900:
        begin
        //
        // Wait for Serial Message to finish downloading new profile data from Programmer
        //
        if not bCurrentEEPROMDataValid then
          goto ExitToTop;
        bCurrentEEPROMDataValid := false;
        //
        // Now, verify profile data just downloaded matches profile that was programmed into Sender
        //
        TestCase := 1000;
        if NewEEPROMData.EEPROM_Config <> CurrentEEPROMData.EEPROM_Config then TestCase := 950;
        if NewEEPROMData.KmaSamples <> CurrentEEPROMData.KmaSamples then TestCase := 950;
        if NewEEPROMData.HoldValue <> CurrentEEPROMData.HoldValue then TestCase := 950;
        if NewEEPROMData.Trippoint <> CurrentEEPROMData.Trippoint then TestCase := 950;
        MaxSetpoint := 0;
        MaxIndex := 0;
        for i := 0 to 509 do
          begin
          if NewEEPROMData.Profile[i] <> CurrentEEPROMData.Profile[i] then TestCase := 950;
          if (NewEEPROMData.Profile[i] > MaxSetpoint) and (NewEEPROMData.Profile[i] < 500) then
            begin
            MaxSetpoint := NewEEPROMData.Profile[i];
            MaxIndex := i + 18;
            end;
          end;
        if bVerifyProfile then
          TestCase := 1000;
        end;

      950:
        begin;
        TestMessage('ERROR: Config/Profile Data does not match', 0);
        VerifyRetryCounter := VerifyRetryCounter - 1;
        if VerifyRetryCounter <> 0 then
          begin
          TestCase := 750;
          end
        else
          begin
          TestCase := 1050;
          RetryCounter := RetryCounter - 1;
          if RetryCounter <> 0 then
            begin
            TestCase := 300;
            end;
          end;
        end;

      1000:
        begin;
        if not bVerifyProfile then
                TestMessage('Config and Profile Data Verified!', 0);
        TestCase := 10000;
        end;

      1050:
        begin;
        TestMessage('ERROR: CALIBRATION FAILED!!!', 0);
        TestCase := 10000;
        end;
//----------------------------------------------------------------------------
//
// Resistive Calibration
//
      2000:
        begin
        if (SetpointData.EnableBits and ENABLE_PULLUP) = 0 then
          begin
          TestMessage('Info: Turning on Internal Pull-up', 0);
          btnInternalPullupClick(Self);
          end;
        WaitTime := 100;
        TestCase := 2100;
        end;

      2100:
        begin
        if WaitTime <> 0 then
          goto ExitToTop;
        //
        // Start of Ohms Calibration Logic
        //
        MaxSetpoint := 0;
        MaxIndex := 0;
        //
        // Look for maximum setpoint value that is less than or equal to 50.0 ohms
        //
        for i := 0 to 509 do
          begin
          if (NewEEPROMData.Profile[i] > MaxSetpoint) and (NewEEPROMData.Profile[i] <= 500) then
            begin
            MaxSetpoint := NewEEPROMData.Profile[i];
            MaxIndex := i;
            end;
          end;
        //
        // Cause Sender to enter Verify Mode.  This will override MasterIndex driving Setpoint lookup table.
        //
        SerialVerifyData.VerifyMode := SERIAL_VERIFY_MODE_READ;
        SerialVerifyData.VerifyAddress := MaxIndex + 18;
        SerialVerifyData.VerifyData := 0;
        bSerialVerifyMode := true;
        WaitTime := 3000;
        TestCase := 2200;
        end;

      2200:
        begin
        //
        // Normally, the logic below this IF statement will cause WaitTime to be set to 100, thereby skipping over 200.  However, should the Feedback
        // never equal the Setpoint, then WaitTime would count down to 200, causing the Calibration routine to abort.
        //
        if (WaitTime < 200) then
          begin
          TestMessage('Error: SP <> FB', 0);
          TestCase := 1050;
          goto ExitToTop;
          end;
        //
        // Wait for Feedback to Equal Setpoint,  Once they do, reset WaitTime down to 100
        //
        if abs(SysVarsCC.DUT_Setpoint - SysVarsCC.DUT_Feedback) <= 3 then
          begin
          WaitTime := 1000;
          TestCase := 2300;
          goto ExitToTop;
          end;
        if WaitTime <> 0 then
          goto ExitToTop;
        end;

      2300:
        begin
        if WaitTime <> 0 then
          goto ExitToTop;
        //
        // Calculate a new Ohms K Factor value
        //
        NewOhmsKFactor := round(CurrentEEPROMData.OhmsKFactor * (SysVars.RV_Ohms / 100.0) / (SysVarsCC.DUT_Setpoint / 10.0)) ;
        if NewOhmsKFactor < 300 then
           NewOhmsKFactor := 300;
        //
        // If difference bewteen current K factor and new K factor is greater than 5, then upload new value to Sender
        //
        if abs(CurrentEEPROMData.OhmsKFactor - NewOhmsKFactor) > 10 then
          begin
          //
          // Limit to 2500, nominal is normally around 1660
          //
          if NewOhmsKFactor > 2500 then NewOhmsKFactor := 2500;
          TestMessage(format('Ohms K Factor: Current: %u  New: %u', [CurrentEEPROMData.OhmsKFactor, NewOhmsKFactor]), 0);
          CurrentEEPROMData.OhmsKFactor := NewOhmsKFactor;
          leOhmsKFactor.Text := format('%u', [NewOhmsKFactor]);
          btnUpdateConfigClick(self);
          WaitTime := 75;
          TestCase := 2400;
          end
        else
          begin
          WaitTime := 75;
          TestCase := 750;
          end;
        end;

      2400:
        begin;
        if WaitTime <> 0 then
          goto ExitToTop;
        //
        // Once the new Config Data has been written to EEPROM, cbSerialVerify checkbox will automatically be cleared.
        //  Once checkbox as cleared, go back and run Calibration logic again.  Continue running calibration logic until
        //  difference between new and old is 5 counts or less.
        //
        if  not cbSerialVerify.Checked then TestCase := 2100;
        end;


      //-----------------------------------------------------------------------------------------------------------

      10000:
        begin
        if (SetpointData.EnableBits and ENABLE_POWER) <> 0 then
          begin
          EventLogger.WriteStr('Turning off Power', True);
          TestMessage('Turning Off Power', 0);
          StatusBar1.Panels[0].Text := '';
          btnPowerClick(Sender);
          end;
        goto ExitNow;
        end;
      end;
    end;

  ExitNow:
  bVerifyProfile := false;
  bEndTest := True;
  StatusBar1.Panels[0].Text := 'END-OF-TEST';
  TestMessage('End of Programming', 0);
  btnStart.Caption := 'Upload Profile';
  EventLogger.WriteStr('=================================================================================================', False);
  EventLogger.WriteStr('----------------------------------------  END TEST  ---------------------------------------------', False);
  EventLogger.WriteStr('=================================================================================================', False);
  EventLogger.WriteStr(' ', False);
  EventLogger.WriteStr(' ', False);
  EventLogger.Refresh;
end;

//******************************************************************************************************************

procedure TForm1.PowerControl (PowerState: boolean);
begin
  if PowerState then SetpointData.EnableBits := SetpointData.EnableBits or ENABLE_POWER;
  if not PowerState then SetpointData.EnableBits := SetpointData.EnableBits and not ENABLE_POWER;

  cbDigMode1.ItemIndex := 2;
  cbDigMode2.ItemIndex := 0;
  cbDigMode3.ItemIndex := 0;
  cbDigMode4.ItemIndex := 0;
  cbDigMode5.ItemIndex := 0;
  cbDigMode6.ItemIndex := 0;

  if not PowerState then
    begin
    btnPower.Caption := 'Turn ON';
    end;
  if PowerState then
    begin
    btnPower.Caption := 'Turn OFF';
    //
    // Default state
    //
    if cbCommunicationsEnable.Checked and (rgMode.ItemIndex = 0) then
      begin
      cbDigMode1.ItemIndex := 5;          // Serial Input
      cbDigMode2.ItemIndex := 5;          // Serial Output
      end;
    if cbCommunicationsEnable.Checked and (rgMode.ItemIndex = 1) then
      begin
      cbDigMode1.ItemIndex := 6;          // Serial In/Out
      end;
    end;
  lePowerCurrent.Font.Color := clBlack;
  lePowerCurrent.Color := clWhite;
end;

//******************************************************************************************************************

procedure TForm1.btnPowerClick(Sender: TObject);
var
  CurrentState: boolean;
  qnow: int64;
begin
  CurrentState := (SetpointData.EnableBits and ENABLE_POWER) <> 0;
  PowerControl(not CurrentState);
  CurrentState := (SetpointData.EnableBits and ENABLE_POWER) <> 0;
  if (not CurrentState) and ((SetpointData.EnableBits and ENABLE_PULLUP) <> 0) then btnInternalPullupClick(Sender);
  TimeGap := 0;
  queryPerformanceCounter(qnow);
  qlast := qnow;
end;

//**************************************************************************************************************************************

procedure TForm1.btnInternalPullupClick(Sender: TObject);
begin
  SetpointData.EnableBits := SetpointData.EnableBits xor ENABLE_PULLUP;
  if (SetpointData.EnableBits and ENABLE_PULLUP) = 0 then btnInternalPullup.Caption := 'Turn ON';
  if (SetpointData.EnableBits and ENABLE_PULLUP) <> 0 then btnInternalPullup.Caption := 'Turn OFF';
end;

//******************************************************************************************************************

procedure TForm1.btnRefreshConfigClick(Sender: TObject);
begin
  bRefreshConfig := TRUE;
end;

//******************************************************************************************************************

procedure TForm1.btnUp1Click(Sender: TObject);
var
  StrIndex: integer;
  Freq, TmpFloat: single;
begin
  if cbCascade.Checked then
    begin
    AdjustFrequency(0.01);
    end
  else
    begin
    StrIndex := 1;
    TmpFloat := 0;
    MyStrToFloat(edtGenDigit1.Text, StrIndex, TmpFloat);
    edtGenDigit1.Text := format('%u', [(round(TmpFloat) + 1) mod 10]);
    end;
  Freq := GeneratorFrequency();
  edtDigFreqOut6.Text:=format('%1.2f', [Freq]);
end;

//******************************************************************************************************************

procedure TForm1.btnUp2Click(Sender: TObject);
var
  StrIndex: integer;
  Freq, TmpFloat: single;
begin
  if cbCascade.Checked then
    begin
    AdjustFrequency(0.1);
    end
  else
    begin
    StrIndex := 1;
    TmpFloat := 0;
    MyStrToFloat(edtGenDigit2.Text, StrIndex, TmpFloat);
    edtGenDigit2.Text := format('%u', [(round(TmpFloat) + 1) mod 10]);
    end;
  Freq := GeneratorFrequency();
  edtDigFreqOut6.Text:=format('%1.2f', [Freq]);
end;

//******************************************************************************************************************

procedure TForm1.btnUp3Click(Sender: TObject);
var
  StrIndex: integer;
  Freq, TmpFloat: single;
begin
  if cbCascade.Checked then
    begin
    AdjustFrequency(1.0);
    end
  else
    begin
    StrIndex := 1;
    TmpFloat := 0;
    MyStrToFloat(edtGenDigit3.Text, StrIndex, TmpFloat);
    edtGenDigit3.Text := format('%u', [(round(TmpFloat) + 1) mod 10]);
    end;
  Freq := GeneratorFrequency();
  edtDigFreqOut6.Text:=format('%1.2f', [Freq]);
end;

//******************************************************************************************************************

procedure TForm1.btnUp4Click(Sender: TObject);
var
  StrIndex: integer;
  Freq, TmpFloat: single;
begin
  if cbCascade.Checked then
    begin
    AdjustFrequency(10.0);
    end
  else
    begin
    StrIndex := 1;
    TmpFloat := 0;
    MyStrToFloat(edtGenDigit4.Text, StrIndex, TmpFloat);
    edtGenDigit4.Text := format('%u', [(round(TmpFloat) + 1) mod 10]);
    end;
  Freq := GeneratorFrequency();
  edtDigFreqOut6.Text:=format('%1.2f', [Freq]);
end;

//******************************************************************************************************************

procedure TForm1.btnUp5Click(Sender: TObject);
var
  StrIndex: integer;
  Freq, TmpFloat: single;
begin
  if cbCascade.Checked then
    begin
    AdjustFrequency(100.0);
    end
  else
    begin
    StrIndex := 1;
    TmpFloat := 0;
    MyStrToFloat(edtGenDigit5.Text, StrIndex, TmpFloat);
    edtGenDigit5.Text := format('%u', [(round(TmpFloat) + 1) mod 10]);
    end;
  Freq := GeneratorFrequency();
  edtDigFreqOut6.Text:=format('%1.2f', [Freq]);
end;

//******************************************************************************************************************

procedure TForm1.btnUp6Click(Sender: TObject);
var
  StrIndex: integer;
  Freq, TmpFloat: single;
begin
  if cbCascade.Checked then
    begin
    AdjustFrequency(1000.0);
    end
  else
    begin
    StrIndex := 1;
    TmpFloat := 0;
    MyStrToFloat(edtGenDigit6.Text, StrIndex, TmpFloat);
    edtGenDigit6.Text := format('%u', [(round(TmpFloat) + 1) mod 10]);
    end;
    Freq := GeneratorFrequency();
    edtDigFreqOut6.Text:=format('%1.2f', [Freq]);
end;

//******************************************************************************************************************

procedure TForm1.btnUp7Click(Sender: TObject);
var
  StrIndex: integer;
  Freq, TmpFloat: single;
begin
  if cbCascade.Checked then
    begin
    AdjustFrequency(10000.0);
    end
  else
    begin
    StrIndex := 1;
    TmpFloat := 0;
    MyStrToFloat(edtGenDigit7.Text, StrIndex, TmpFloat);
    edtGenDigit7.Text := format('%u', [(round(TmpFloat) + 1) mod 10]);
    end;
  Freq := GeneratorFrequency();
  edtDigFreqOut6.Text:=format('%1.2f', [Freq]);
end;

//******************************************************************************************************************

procedure TForm1.btnUpdateConfigClick(Sender: TObject);
var
  StrIndex: integer;
  TmpFloat: single;
begin
  TmpFloat := 0.0;
  StrIndex := 1;
  TmpFloat := 0;
  MyStrToFloat(leOhmsKFactor.Text, StrIndex, TmpFloat);
  SetpointData.NewConfigData[0] := round(TmpFloat);

  StrIndex := 1;
  MyStrToFloat(leVoltsKFactor.Text, StrIndex, TmpFloat);
  SetpointData.NewConfigData[1] := round(TmpFloat);

  StrIndex := 1;
  MyStrToFloat(leEEPROMConfig.Text, StrIndex, TmpFloat);
  SetpointData.NewConfigData[2] := round(TmpFloat);

  StrIndex := 1;
  MyStrToFloat(leTrippoint.Text, StrIndex, TmpFloat);
  SetpointData.NewConfigData[3] := round(TmpFloat);

  StrIndex := 1;
  MyStrToFloat(leKmaSamples.Text, StrIndex, TmpFloat);
  SetpointData.NewConfigData[4] := round(TmpFloat);

  StrIndex := 1;
  MyStrToFloat(leHoldValue.Text, StrIndex, TmpFloat);
  SetpointData.NewConfigData[5] := round(TmpFloat);

  if (SetpointData.NewConfigData[2] and $14) = $14 then
     begin
     SetpointData.NewConfigData[3] := round((SetpointData.NewConfigData[3] - 4) / 1.125) + 50;
     SetpointData.NewConfigData[5] := round((SetpointData.NewConfigData[5] - 4) / 1.125) + 50;
     end;

  bUpdateConfig := TRUE;
end;

//******************************************************************************************************************

procedure TForm1.Button1Click(Sender: TObject);
var
  F1:   file;
  dwFileSize: dword;
  Filename: string;
begin
  if not OpenDialog1.Execute then exit;
  Filename := OpenDialog1.FileName;
  if not FileExists(Filename) then exit;
  AssignFile(F1, Filename);
  Reset(F1, 1);
  dwFileSize := FileSize(F1);
  FillChar(FirmwareBinaryBuffer, $20000, $FF);
  BlockRead(F1, FirmwareBinaryBuffer, dwFileSize);
  CloseFile(F1);
  FirmwarePageNumber := 0;
  FirmwarePageSize := (dwFileSize div 2048);
  if (dwFileSize mod 2048) <> 0 then FirmwarePageSize := FirmwarePageSize + 1;
  bIssueReprogrammingCommand := true;
  lblMessage.Caption:='Uploading New Firmware';
  ProgressBar1.Max := FirmwarePageSize - 1;
  ProgressBar1.Position := 0;
  pnlReadConfig.Visible := TRUE;
end;

//******************************************************************************************************************

procedure TForm1.btnSaveProfileClick(Sender: TObject);
var
   IniFile: TIniFile;
   ConfigData: array [0..4] of integer;
   ProfileCompleteString: string;
   StrIndex: integer;
   TmpFloat: single;
begin
  IniFile := TIniFile.Create(AppPath + 'PROFILES_CC.INI');
  if rgKmaDirection.ItemIndex = 0 then ConfigData[0] := 0 else ConfigData[0] := 1;

  ConfigData[1] := 0;
  if cbHoldMode.ItemIndex > 0 then ConfigData[1] := cbHoldMode.ItemIndex + 2;
  if cbTemperatureOutput.ItemIndex > 0 then ConfigData[1] := cbTemperatureOutput.ItemIndex;

  StrIndex := 1;
  TmpFloat := 0;
  MyStrToFloat(edtTripPoint.Text, StrIndex, TmpFloat);
  ConfigData[2] :=round(TmpFloat);

  StrIndex := 1;
  MyStrToFloat(edtHoldValue.Text, StrIndex, TmpFloat);
  ConfigData[3] :=round(TmpFloat);

  StrIndex := 1;
  MyStrToFloat(edtKmaSamples.Text, StrIndex, TmpFloat);
  ConfigData[4] :=round(TmpFloat);

  ProfileCompleteString := format('[%u,%u,%u,%u,%u], %s', [ConfigData[0],ConfigData[1],ConfigData[2],ConfigData[3],ConfigData[4], edtSetpointProfile.Text]);

  IniFile.WriteString('PROFILES', cbProfileName.Text, ProfileCompleteString);
  IniFile.free;
  ReloadIniFile();
end;

//******************************************************************************************************************

procedure TForm1.btnVerifyProfileClick(Sender: TObject);
begin
  bVerifyProfile := true;
  btnStartClick(self);
end;

//******************************************************************************************************************

procedure TForm1.btnReloadCalibrationClick(Sender: TObject);
begin
  FlashIndex := 0;
  bSerialGetFactory := true;
end;

//******************************************************************************************************************

procedure TForm1.btnUploadCalibrationClick(Sender: TObject);
var
  TmpFloat: single;
  i, StrIndex: longint;
begin
  TmpFloat := 0;                                   // to keep compiler happy

  for i := 0 to 15 do
    begin
    StrIndex := 1;
    MyStrToFloat(CalibrationAdcZeroEdit[i].Text, StrIndex, TmpFloat);
    FactoryData.AdcZero[i] := round(TmpFloat * 1.0);
    StrIndex := 1;
    MyStrToFloat(CalibrationAdcSpanEdit[i].Text, StrIndex, TmpFloat);
    FactoryData.AdcSpan[i] := round(TmpFloat * 1.0);
    end;

  FactoryData.CRC16 := CalcCrcArray(@FactoryData, 2046);

  bSerialSetFactory := true;
end;

procedure TForm1.Button2Click(Sender: TObject);
var
  TestCase: integer;
  bPowerAlreadyOn: boolean;
label
  ExitNow, ExitToTop;
begin
    TestCase := 0;
    bEndTest := False;
    bPowerAlreadyOn := false;
    WaitTime := 0;
    while (not bEndTest) do
      begin
  ExitToTop:
      if WaitTime <> 0 then
        WaitTime := WaitTime - 1;
      Sleep(1);
      Application.ProcessMessages;

      // This Case statement controls each test we are running

      case (TestCase) of
          0:
            begin
            if CD_70_UpdateCounter <> 0 then       // Power On and communicating
              begin
              bPowerAlreadyOn := true;
              WaitTime := 10;
              TestCase := 300;
              end
            else
              begin
              TestMessage('Turning On Power', 0);
              PowerControl(TRUE);
              WaitTime := 2000;
              TestCase := 200;
              end;
            end;

          200:
            begin
            //
            // Once we see DUT_UpdateCounter change at least 10 times, we can safely assume that the Programmer is communicating with
            // Sender, therefore reduce WaitTime
            //
            if (WaitTime > 20) and (CD_70_UpdateCounter <> 0) then
              WaitTime := 20;
            //
            // If ENABLE_POWER bit resets, then excess current has been detected and power was automatically turned off, therefore abort test
            //
            if (SetpointData.EnableBits and ENABLE_POWER) = 0 then
              begin
              TestMessage('ERROR: Excess Current Detected', 0);
              TestCase := 10000;
              goto ExitToTop;
              end;
            //
            // Wait for WaitTime to reach zero
            //
            if WaitTime <> 0 then
              goto ExitToTop;
            //
            // If DUT_UpdateCounter hasn't changed, then we aren't communicating with Sender, therefore abort test.
            //
            if (CD_70_UpdateCounter = 0) then
              begin
              TestMessage('ERROR: Unable to Communicate with Sender', 0);
              TestCase := 10000;
              goto ExitToTop;
              end;

            WaitTime := 50;
            TestCase := 300;
            goto ExitToTop;
            end;

          300:
            begin
            if WaitTime <> 0 then
              goto ExitToTop;
            //
            // Write Flash Configuration Data to Sender
            //
            TestMessage('Download Factory Configuration Data', 0);
            bDownloadDone := false;
            bDownloadFactoryConfig := true;
            WaitTime := 2000;
            TestCase := 400;
            goto ExitToTop;
            end;

          400:
            begin
            if WaitTime = 0 then
              begin
              TestMessage('ERROR:  Unable to Download Factory Configuration Data', 0);
              TestCase := 10000;
              goto ExitToTop;
              end;
            if not bDownloadDone then
              begin
              goto ExitToTop;
              end;
            TestMessage('Download Done..', 0);
            TestCase := 10000;
            goto ExitToTop;
            end;

          10000:
            begin
            if ((SetpointData.EnableBits and ENABLE_POWER) <> 0) and not bPowerAlreadyOn then
              begin
              TestMessage('Turning Off Power', 0);
              StatusBar1.Panels[0].Text := '';
              btnPowerClick(Sender);
              end;
            goto ExitNow;
            end;
          end;
        end;

      ExitNow:
end;

procedure TForm1.Button3Click(Sender: TObject);
var
  TestCase: integer;
  bPowerAlreadyOn, CompareStatus: boolean;

label
  ExitNow, ExitToTop;
begin
  //
  // Store GUI values into CDProfileData.GUIConfigData
  //
  LoadFactoryFromGui();
  //
  // Copy CDProfileData.GUIFactoryData values into CD_42_Upload
  //
  CDProfileData.UpdateCD_42();

  TestCase := 0;
  bEndTest := False;
  bPowerAlreadyOn := false;
  WaitTime := 0;
  while (not bEndTest) do
    begin
ExitToTop:

    // If WaitTime <> 0, then Add a 10mS delay into loop time.

    if WaitTime <> 0 then
      WaitTime := WaitTime - 1;
    Sleep(1);
    Application.ProcessMessages;

    // This Case statement controls each test we are running

    case (TestCase) of
        0:
          begin
          if CD_70_UpdateCounter <> 0 then       // Power On and communicating
            begin
            bPowerAlreadyOn := true;
            WaitTime := 10;
            TestCase := 300;
            end
          else
            begin
            EventLogger.WriteStr('Turn on Power', True);
            TestMessage('Turning On Power', 0);
            PowerControl(TRUE);
            bEnableLogging := true;
            WaitTime := 2000;
            TestCase := 200;
            end;
          end;

        200:
          begin
          //
          // Once we see DUT_UpdateCounter change at least 10 times, we can safely assume that the Programmer is communicating with
          // Sender, therefore reduce WaitTime
          //
          if (WaitTime > 20) and (CD_70_UpdateCounter <> 0) then
            WaitTime := 20;
          //
          // If ENABLE_POWER bit resets, then excess current has been detected and power was automatically turned off, therefore abort test
          //
          if (SetpointData.EnableBits and ENABLE_POWER) = 0 then
            begin
            EventLogger.WriteStr('ERROR: Excess Current Detected, Programming Aborted', True);
            TestMessage('ERROR: Excess Current Detected', 0);
            TestCase := 10000;
            goto ExitToTop;
            end;
          //
          // Wait for WaitTime to reach zero
          //
          if WaitTime <> 0 then
            goto ExitToTop;
          //
          // If DUT_UpdateCounter hasn't changed, then we aren't communicating with Sender, therefore abort test.
          //
          if (CD_70_UpdateCounter = 0) then
            begin
            EventLogger.WriteStr('ERROR: Unable to Communicate with Fuel Sender, Programming Aborted', True);
            TestMessage('ERROR: Unable to Communicate with Sender', 0);
            TestCase := 10000;
            goto ExitToTop;
            end;

          WaitTime := 20;
          TestCase := 300;
          goto ExitToTop;
          end;

        300:
          begin
          if WaitTime <> 0 then
            goto ExitToTop;
          //
          // Write Factory Configuration Data to Sender
          //
          TestMessage('Uploading Factory Configuration Data', 0);
          bUploadFactoryConfiguration := true;
          bUploadDone := false;
          WaitTime := 5000;
          TestCase := 400;
          goto ExitToTop;
          end;

        400:
          begin
          if WaitTime = 0 then
            begin
            TestMessage('ERROR:  Unable to Upload Factory Configuration Data', 0);
            TestCase := 10000;
            goto ExitToTop;
            end;
          if not bUploadDone then
            begin
            goto ExitToTop;
            end;
          bUploadDone := false;
          WaitTime := 20;
          TestCase := 750;
          goto ExitToTop;
          end;

        750:
          begin
          TestMessage('Verify Factory Configuration Data', 0);
          bDownloadFactoryConfig := true;
          bDownloadDone := false;
          WaitTime := 5000;
          TestCase := 800;
          end;

        800:
          begin
          if WaitTime = 0 then
            begin
            TestMessage('ERROR:  Unable to Download Factory Configuration Data', 0);
            TestCase := 10000;
            goto ExitToTop;
            end;
          if not bDownloadDone then
            begin
            goto ExitToTop;
            end;
          bDownloadDone := false;
          CompareStatus := CompareMem(@CDProfileData.CurrentCDFactoryData, @CDProfileData.GuiCDFactoryData, sizeof(CDProfileData.GuiCDFactoryData));
          if CompareStatus then
            begin
            TestMessage('Factory Data is Valid', 0);
            end
          else
            begin
            TestMessage('-->>FACTORY DATA IS INVALID!!!!', 0);
            end;
          TestCase := 10000;
          goto ExitToTop;
          end;

        //-----------------------------------------------------------------------------------------------------------

        10000:
          begin
          if ((SetpointData.EnableBits and ENABLE_POWER) <> 0) and not bPowerAlreadyOn then
            begin
            EventLogger.WriteStr('Turning off Power', True);
            TestMessage('Turning Off Power', 0);
            StatusBar1.Panels[0].Text := '';
            btnPowerClick(Sender);
            end;
          goto ExitNow;
          end;
        end;
      end;

    ExitNow:
    bEnableLogging := false;
    bVerifyProfile := false;
    bEndTest := True;
    TestMessage('End of Programming', 0);
    btnStart.Caption := 'Upload Profile';
    EventLogger.WriteStr('=================================================================================================', False);
    EventLogger.WriteStr('----------------------------------------  END TEST  ---------------------------------------------', False);
    EventLogger.WriteStr('=================================================================================================', False);
    EventLogger.WriteStr(' ', False);
    EventLogger.WriteStr(' ', False);
    EventLogger.Refresh;
end;

procedure TForm1.Button4Click(Sender: TObject);
var
  FineTune, NewCal, Old5, ConnectTimer, State: integer;
  Voltage: double;
begin
  memo3.clear;
  memo3.lines.add('CD Factory Calibration..Started');

  Old5 := 0;
  NewCal := 0;
  State := 0;
  FineTune := 0;
  ConnectTimer := 0;
  while true do
    begin
    Application.ProcessMessages;
    Sleep(1);
    ConnectTimer := ConnectTimer + 1;
    Edit1.Text := format('%1.3f', [SiglentValue]);
    case (State) of
        0: begin
           memo3.lines.add('Connecting to Siglent DVM');
           bSiglentThreadRunning := true;
           ConnectTimer := 0;
           bNewSiglentValue := false;
           State := 10;
           end;
        10: begin
           if bNewSiglentValue then
             begin
             bNewSiglentValue := false;
             memo3.lines.add(format('ConnectTimer = %d', [ConnectTimer]));
             State := 20;
             end;
           if ConnectTimer > 4000 then
             begin
             memo3.lines.add('Unable to connect to Siglent DVM...Calibration Aborted!!');
             bSiglentThreadRunning := false;
             exit;
             end;
           end;
        20: begin
           memo3.lines.add('Turning on Power to CD2840');
           btnPowerClick(Self);
           ConnectTimer := 0;
           State := 30;
           end;
        30: begin
           if ConnectTimer > 500 then
             begin
             Voltage := SysVars.AdcVolts[14] / 1000.0;
             if (Voltage > 10) and (Voltage < 11) then
               begin
               memo3.lines.add(format('Power is On and within limits (Voltage = %1.2f)', [Voltage]));
               State := 40;
               end;
             end;
           if ConnectTimer > 3000 then
             begin
             memo3.lines.add(format('Power is not within limits (Voltage = %1.2f) - Calibration Aborted!!', [Voltage]));
             bSiglentThreadRunning := false;
             exit;
             end;
           end;
        40: begin
           memo3.lines.add('Download Firmware w/ ST-Link Utility, then press NEXT');
           bNext := false;
           State := 42;
           end;
        42: begin
           if bNext then
             begin
             bNext := false;
             bCalibrationComm := false;
             ConnectTimer := 0;
             State := 50;
             end;
        end;
        50: begin
             if bCalibrationComm then
               begin
               State := 60;
               end;
             if ConnectTimer > 10000 then
               begin
               memo3.lines.add('Not Communicating with CD2840 - Calibration Aborted!!');
               bSiglentThreadRunning := false;
               bNext := false;
               exit;
               end;
           end;
        60: begin
            memo3.lines.add('Uploading FactoryProfile to Sender');
            cbCDProfileNames.Text := 'FactoryProfile';
            cbCDProfileNamesCloseUp(Self);
            btnUploadProfileClick(self);
            State := 64;
            end;
        64: begin
           memo3.lines.add('Connect Siglent to +3.3V Test Point, then press NEXT');
           bNext := false;
           bNewSiglentValue := false;
           State := 70;
           end;
        70: begin
           if bNewSiglentValue then
             begin
             bNewSiglentValue := false;
             if bNext then
               begin
               bNext := false;
               edtConfig33Calibration.Text:= format('%1.3f', [SiglentValue]);
               edtConfig5VDCCalibration.Text := '10000';
               edtConfigKmaCalibration.Text := '10000';
               edtConfigPowerInputVoltageCalibration.Text := '10000';
               edtConfigRVVoltageCalibration.Text := '10000';
               edtConfigRVCurrentScaleCalibration.Text := '10000';
               edtConfigPwmOutput.Text := '10000';

               edtConfigPowerInputFilteringGain.Text := '200';
               edtConfig5VDCFilteringGain.Text := '200';
               edtConfigTemperatureFilteringGain.Text := '200';
               edtConfigInternalReferneceFilteringGain.Text := '200';
               edtConfigRVVoltageFilteringGain.Text := '200';
               edtConfigRVCurrentFilteringGain.Text := '200';

               edtConfigManufacturingTime.Text := formatDateTime('hh:nn', now);
               edtConfigManufacturingDate.Text := formatDateTime('YYYY/MM/DD', now);
               edtConfigManufacturingNameTester.Text := 'Charlie Urban';
               edtConfigManufacturingModelNumber.Text := 'CD7603';
               edtConfigManufacturingSerialNumber.Text := 'X' + formatDateTime('YYYYMMDD_hhnn', now);

               cbPcbRevision.ItemIndex := 1;
               cbRVOutput.Checked := true;
               cbRS485Driver.Checked := true;
               cbCANBusDriver.Checked := true;
               cbTemperatureSensor.Checked := true;
               cbKmaSensor.Checked := true;
               memo3.lines.add('Uploading Factory Calibration Data to Sender');
               Button3Click(Self);
               State := 80;
               end;
             end;
           end;
        80: begin
            memo3.lines.add('Connect Siglent to +5V Test Point, then press NEXT');
            bNext := false;
            State := 90;
            end;
        90: begin
           if bNewSiglentValue then
             begin
             bNewSiglentValue := false;
             if bNext then
               begin
               bNext := false;
               memo3.lines.add('Waiting for +5V to Stabilize');
               memo3.Lines.Add(format('+5VDC=%1.3f', [CD_70.ENG_PowerRail5VDC /1000.0]));
               memo3.lines.add('');
               Voltage := SiglentValue;
               ConnectTimer := 0;
               Old5 := 0;
               State := 100;
               end;
             end;
            end;
        100: begin
            memo3.Lines.Strings[memo3.Lines.Count - 2] := format('+5VDC=%1.3f', [CD_70.ENG_PowerRail5VDC /1000.0]);
            if abs(CD_70.ENG_PowerRail5VDC- Old5) > 5 then
              begin
              ConnectTimer := 0;
              Old5 := CD_70.ENG_PowerRail5VDC;
              end;
            if ConnectTimer > 500 then
              begin
              NewCal := round(((CD_70.ENG_PowerRail5VDC /1000.0) / Voltage) * 10000);
              edtConfig5VDCCalibration.Text := format('%d', [NewCal]);
              memo3.Lines.Add(format('+5VDC K Factor = %d', [NewCal]));
              Button3Click(Self);
              State := 110;
              end;
           end;
        110: begin
             memo3.lines.add('Connect Siglent to KMA Test Point, then press NEXT');
             bNext := false;
             State := 120;
            end;
        120: begin
            if bNewSiglentValue then
              begin
              bNewSiglentValue := false;
              if bNext then
                  begin
                   bNext := false;
                   memo3.lines.add('Setting KMA Filter Gain to 10');
                   edtUploadConfigKmaGain.Text := '10';
                   btnUploadProfileClick(self);
                   memo3.lines.add('Waiting for KMA Voltage to Stabilize');
                   memo3.Lines.Add(format('KMA=%1.4f', [CD_70.ENG_KmaVoltage /10000.0]));
                   memo3.lines.add('');
                   Old5 := 0;
                   ConnectTimer :=0;
                   State := 122;
                   end;
              end;
            end;
        122: begin
             memo3.Lines.Strings[memo3.Lines.Count - 2] := format('KMA=%1.4f', [CD_70.ENG_KmaVoltage /10000.0]);
             if abs(CD_70.ENG_KmaVoltage - Old5) > 5 then
               begin
               Old5 := CD_70.ENG_KmaVoltage;
               ConnectTimer := 0;
               end;
             if ConnectTimer > 500 then
               begin
               bNewSiglentValue := false;
               State := 124;
               end;
             end;
        124: begin
             if bNewSiglentValue then
               begin
               bNewSiglentValue := false;
               memo3.lines.add('KMA Stabilized, taking Siglent Reading');
               Voltage := SiglentValue;
               NewCal := round(((CD_70.ENG_KmaVoltage /10000.0) / SiglentValue) * 10000);
               edtConfigKmaCalibration.Text := format('%d', [NewCal]);
               memo3.Lines.Add(format('KMA K Factor = %d', [NewCal]));
               Button3Click(Self);
               State := 130;
               end;
              end;
        130: begin
              memo3.lines.add('Connect Siglent to Power Input, then press NEXT');
              bNext := false;
              State := 140;
             end;
        140: begin
            if bNewSiglentValue then
              begin
              bNewSiglentValue := false;
              if bNext then
                  begin
                   bNext := false;
                   Voltage := SiglentValue;
                   NewCal := round(((CD_70.ENG_PowerInputVoltage /1000.0) / SiglentValue) * 10000);
                   edtConfigPowerInputVoltageCalibration.Text := format('%d', [NewCal]);
                   memo3.Lines.Add(format('Power Input K Factor = %d', [NewCal]));
                   Button3Click(Self);
                   State := 142;
                   end;
                end;
             end;
        142: begin
             memo3.lines.add('Connect 332 Ohm Load to R/V Output, then Press NEXT');
             State := 144;
             end;
        144: begin
            if bNext then
                begin
                 bNext := false;
                 State := 146;
                 end;
             end;
        146: begin
             memo3.lines.add('Enable Setpoint Manual Mode = 5.00V');
             Form2.cbManualSetpoint.Checked := true;
             Form2.edtManualSetpoint.Text := '5.00';
             memo3.lines.add('Waiting for R/V Output to stabilize');
             memo3.Lines.Add(format('R/V Ouput=%1.4f  FineTune=%1.3f', [CD_70.ENG_RVOutputVoltage /10000.0, CD_70.ENG_PwmDutyCycleFineTune /1000.0]));
             memo3.lines.add('');
             Old5 := 0;
             ConnectTimer := 0;
             State := 148;
             end;
        148: begin
             memo3.Lines.Strings[memo3.Lines.Count - 2] := format('R/V Ouput=%1.4f  FineTune=%1.3f', [CD_70.ENG_RVOutputVoltage /10000.0, CD_70.ENG_PwmDutyCycleFineTune /1000.0]);
            if bNewSiglentValue then
              begin
              bNewSiglentValue := false;
              end;
             if abs(CD_70.ENG_PwmDutyCycleFineTune - Old5) > 10 then
               begin
               Old5 := CD_70.ENG_PwmDutyCycleFineTune;
               ConnectTimer := 0;
               end;
             if ConnectTimer > 500 then
               begin
               State := 150;
               end;
             end;
        150: begin
             memo3.lines.add('Connect Siglent to R/V Output, then press NEXT');
             bNext := false;
             State := 160;
             end;
        160: begin
             if bNewSiglentValue then
               begin
               bNewSiglentValue := false;
               if bNext then
                  begin
                   bNext := false;
                   Voltage := SiglentValue;
                   NewCal := round(((CD_70.ENG_RVOutputVoltage /10000.0) / SiglentValue) * 10000);
                   edtConfigRVVoltageCalibration.Text := format('%d', [NewCal]);
                   memo3.Lines.Add(format('R/V Voltage K Factor = %d', [NewCal]));
                   Button3Click(Self);
                   FineTune := 10000;
                   memo3.lines.add('Adjusting R/V PWM K Factor');
                   memo3.lines.add(format('R/V PWM K Factor = %d', [FineTune]));
                   memo3.Lines.Add(format('FineTune=%1.3f', [CD_70.ENG_PwmDutyCycleFineTune /1000.0]));
                   memo3.lines.add('');
                   Old5 := CD_70.ENG_PwmDutyCycleFineTune;
                   ConnectTimer := 501;
                   State := 170;
                   end;
               end;
             end;
        170: begin
             memo3.Lines.Strings[memo3.Lines.Count - 2] := format('FineTune=%1.3f', [CD_70.ENG_PwmDutyCycleFineTune /1000.0]);
             if abs(CD_70.ENG_PwmDutyCycleFineTune - Old5) > 5 then
               begin
               Old5 := CD_70.ENG_PwmDutyCycleFineTune;
               ConnectTimer := 0;
               end;
             if ConnectTimer > 500 then
               begin
               if abs(CD_70.ENG_PwmDutyCycleFineTune) < 20 then
                 begin
                 memo3.lines.add('Done..Setting Manual Setpoint = 0.00V');
                 Form2.edtManualSetpoint.Text := '0.00';
                 State := 180;
                 end
               else
                 begin
                 FineTune := FineTune - (CD_70.ENG_PwmDutyCycleFineTune div 10);
                 edtConfigPwmOutput.Text := format('%d', [FineTune]);
                 memo3.Lines.Strings[memo3.Lines.Count - 3] := format('R/V PWM K Factor = %d', [FineTune]);
                 Button3Click(Self);
                 ConnectTimer := 0;
                 Old5 := 0;
                 end;
               end;
             end;
        180: begin
             memo3.lines.add('Connect Siglent across 332 Ohm Load Resistor, then Press NEXT');
             State := 190;
             end;
        190: begin
             if bNewSiglentValue then
               begin
               bNewSiglentValue := false;
               if bNext then
                 begin
                 bNext := false;
                 Voltage := abs(SiglentValue);
                 NewCal := round(((CD_70.ENG_RVOutputCurrent /1000.0) / (Voltage * 1000 / 332.0)) * 10000);
                 edtConfigRVCurrentScaleCalibration.Text := format('%d', [NewCal]);
                 memo3.Lines.Add(format('R/V Current K Factor = %d', [NewCal]));
                 Button3Click(Self);
                 State := 200;
                 end;
               end;
             end;
         200: begin
              bSiglentThreadRunning := false;
              btnPowerClick(Self);
              memo3.Lines.Add('--> Calibration Done <--');
              exit;
              end;
        end;
    end;

end;

//******************************************************************************************************************

procedure TForm1.Button5Click(Sender: TObject);
begin
  bClearEventLog := true;
end;

//******************************************************************************************************************
//
procedure TForm1.btnSaveCDProfileClick(Sender: TObject);
begin
  LoadFromGUI();
  SaveToIniFile();
  TestMessage('Profile Saved..', 0)
end;

procedure TForm1.btnUploadCDFirmwareClick(Sender: TObject);
var
  TestCase: integer;
  bPowerAlreadyOn: boolean;
  F1: file;
  i, dwFileSize: dword;
  Filename: string;
label
  ExitNow, ExitToTop;
begin
  if not OpenDialog1.Execute then exit;
  Filename := OpenDialog1.FileName;
  if not FileExists(Filename) then exit;
  AssignFile(F1, Filename);
  Reset(F1, 1);
  dwFileSize := FileSize(F1);
  if (dwFileSize > 122880) then
    begin
    exit;
    end;
  for i := 0 to $1FFFF do
     begin
     FirmwareBinaryBuffer[i] := $FF;
     end;
  BlockRead(F1, FirmwareBinaryBuffer, dwFileSize);
  CloseFile(F1);

  move(&FirmwareBinaryBuffer, &FirmwareCrcBuffer, sizeof(FirmwareCrcBuffer));

  FirmwareInfo.BF7.CRC16 := CalcCrcArray(@FirmwareCrcBuffer, (2048 * 60));     // Only run CRC on first 60 pages
  FirmwareInfo.BF7.Version := $0202;
  FirmwareInfo.Keycode:= $78563412;
  TestMessage(format('CRC16 = %X', [FirmwareInfo.BF7.CRC16]), 0);

  move(&FirmwareCrcBuffer, &FirmwareBinaryBuffer, sizeof(FirmwareCrcBuffer));

  FirmwarePageNumber := 0;
  ProgressBar2.Max := 56;
  ProgressBar2.Position := 0;
  ProgressBar2.Visible := True;

  TestCase := 0;
  bEndTest := False;
  bPowerAlreadyOn := false;
  WaitTime := 0;
  while (not bEndTest) do
    begin
  ExitToTop:
    if WaitTime <> 0 then
      WaitTime := WaitTime - 1;
    Sleep(1);
    Application.ProcessMessages;

      // This Case statement controls each test we are running

      case (TestCase) of
          0:
            begin
            if CD_70_UpdateCounter <> 0 then       // Power On and communicating
              begin
              bPowerAlreadyOn := true;
              WaitTime := 10;
              TestCase := 300;
              end
            else
              begin
              TestMessage('Turning On Power', 0);
              PowerControl(TRUE);
              WaitTime := 2000;
              TestCase := 200;
              end;
            end;

          200:
            begin
            //
            // Once we see DUT_UpdateCounter change at least 10 times, we can safely assume that the Programmer is communicating with
            // Sender, therefore reduce WaitTime
            //
            if (WaitTime > 20) and (CD_70_UpdateCounter <> 0) then
              WaitTime := 20;
            //
            // If ENABLE_POWER bit resets, then excess current has been detected and power was automatically turned off, therefore abort test
            //
            if (SetpointData.EnableBits and ENABLE_POWER) = 0 then
              begin
              TestMessage('ERROR: Excess Current Detected', 0);
              TestCase := 10000;
              goto ExitToTop;
              end;
            //
            // Wait for WaitTime to reach zero
            //
            if WaitTime <> 0 then
              goto ExitToTop;
            //
            // If DUT_UpdateCounter hasn't changed, then we aren't communicating with Sender, therefore abort test.
            //
            if (CD_70_UpdateCounter = 0) then
              begin
              TestMessage('ERROR: Unable to Communicate with Sender', 0);
              TestCase := 10000;
              goto ExitToTop;
              end;

            WaitTime := 50;
            TestCase := 300;
            goto ExitToTop;
            end;

          300:
            begin
            if WaitTime <> 0 then
              goto ExitToTop;
            //
            //
            //
            TestMessage('Starting Firmware Upload', 0);
            bDownloadDone := false;
            bUploadCDFirmware := true;
            WaitTime := 20000;
            TestCase := 400;
            goto ExitToTop;
            end;

          400:
            begin
            if WaitTime = 0 then
              begin
              TestMessage('ERROR:  Unable to Start Firmware Upload', 0);
              TestCase := 10000;
              goto ExitToTop;
              end;
            if not bUploadCDFirmware then
              begin
              WaitTime := 30000;
              TestCase := 500;
              goto ExitToTop;
              end;
            end;

          500:
            begin
            if WaitTime = 0 then
              begin
              TestMessage('ERROR:  Unable to reprogram Sender', 0);
              TestCase := 10000;
              goto ExitToTop;
              end;
            if not bUploadCDFirmwareStarted then
              begin
              TestMessage('Done uploading Firmware..', 0);
              TestCase := 10000;
              goto ExitToTop;
              end;
            end;

          10000:
            begin
            if ((SetpointData.EnableBits and ENABLE_POWER) <> 0) and not bPowerAlreadyOn then
              begin
              TestMessage('Turning Off Power', 0);
              StatusBar1.Panels[0].Text := '';
              btnPowerClick(Sender);
              bUploadCDFirmwareStarted := false;
              bReprogramming := false;
              end;
            goto ExitNow;
            end;
          end;
        end;

      ExitNow:
end;

procedure TForm1.Button6Click(Sender: TObject);
begin
  bNext := true;
end;

//******************************************************************************************************************
//
//  CD SERIES
//
//  Update GUI values
//
procedure TForm1.UpdateGui;
var
  i, j: integer;
  TableResults: stTwoPointTable;
  bSaveLoading: boolean;
begin
  bSaveLoading := bLoading;
  bLoading := true;

  cbCDProfileNames.Text := StrPas(CDProfileData.GuiConfigData.ProfileName);
  cbUploadConfigDig1.ItemIndex := DIG1toIndex[CDProfileData.GuiConfigData.BF2.DigIo1];
  cbUploadConfigDig2.ItemIndex := DIG2toIndex[CDProfileData.GuiConfigData.BF2.DigIo2];
  cbUploadConfigDig3.ItemIndex := DIG3toIndex[CDProfileData.GuiConfigData.BF2.DigIo3];
  cbUploadConfigDig4.ItemIndex := DIG4toIndex[CDProfileData.GuiConfigData.BF2.DigIo4];
  cbUploadConfigDig5.ItemIndex := DIG5toIndex[CDProfileData.GuiConfigData.BF2.DigIo5];
  cbUploadConfigDig6.ItemIndex := DIG6toIndex[CDProfileData.GuiConfigData.BF2.DigIo6];
  cbUploadConfigReverseKMA.Checked := CDProfileData.GuiConfigData.BF3.ReverseIndexKma <> 0;
  cbUploadConfigErrorActiveLevel.ItemIndex := CDProfileData.GuiConfigData.BF1.ErrorConditionActiveState;
  edtUploadConfigKmaGain.Text := format('%d', [CDProfileData.GuiConfigData.KmaFilterGain]);
  edtUploadConfigMasterGain.Text := format('%d', [CDProfileData.GuiConfigData.MasterIndexFilteringGain]);
  edtUploadConfigAccelXGain.Text := format('%d', [CDProfileData.GuiConfigData.AccelVoltageXAdcFilteringGain]);
  edtUploadConfigAccelYGain.Text := format('%d', [CDProfileData.GuiConfigData.AccelVoltageYAdcFilteringGain]);
  edtUploadConfigAccelZGain.Text := format('%d', [CDProfileData.GuiConfigData.AccelVoltageZAdcFilteringGain]);
  cbUploadConfigRS485Format.ItemIndex := CDProfileData.GuiConfigData.BF3.RS485MessageMode;
  edtUploadConfigRS485Time.Text := format('%1.1f', [CDProfileData.GuiConfigData.BF3.RS485MessageTime / 10.0]);
  cbUploadConfigAccelerometerScaling.ItemIndex := CDProfileData.GuiConfigData.BF3.AccelerometerScaling;
  cbUploadConfigOutputMode.ItemIndex := CDProfileData.GuiConfigData.BF1.RVOutputMode;
  cbUploadConfigPullUpMode.ItemIndex := CDProfileData.GuiConfigData.BF1.RVOutputPullup;
  cbUploadConfigMasterMode.ItemIndex := CDProfileData.GuiConfigData.BF1.MasterFreqMode;
  edtUploadConfigRVsetpoint.Text := CDProfileData.GetRVSetpointTable();
  edtUploadConfigMasterSetpoint.Text := CDProfileData.GetMasterFreqSetpointTable;

  rgDefaultOutput.ItemIndex := CDProfileData.GuiConfigData.BF8.DefaultOutput;
  rgUploadConfigSensorMode.ItemIndex := CDProfileData.GuiConfigData.BF8.SensorMode;
  cbUploadConfigCanUse.ItemIndex := CDProfileData.GuiConfigData.BF8.CanUse;
  edtUploadConfigSourceAddress.Text := format('%d', [CDProfileData.GuiConfigData.BF8.SourceAddress]);
  cbUploadConfigCanEcho.ItemIndex := CDProfileData.GuiConfigData.BF8.CanEcho;
  cbUploadConfigCanCrcEnable.ItemIndex := CDProfileData.GuiConfigData.BF8.CanCrcEnable;
  cbUploadConfigCanBaudRate.ItemIndex := GetIndex(CDProfileData.GuiConfigData.BF8.CanPrescaler, @CanBaudtoPrescaler, length(CanBaudtoPrescaler));
  cbUploadConfigEndianess.ItemIndex := CDProfileData.GuiConfigData.BF8.Endianess;
  edtUploadConfigCanBus.Text:= CDProfileData.GetCanBusTable;
  edtUploadConfigAngularOutput.Text:=CDProfileData.GetAngularOutputTable;

  edtCanSamplingDelay.Text := format('%1.2f', [CDProfileData.GuiConfigData.CanSamplingDelay / 100.0]);
  edtOutputStartupDelay.Text := format('%1.2f', [CDProfileData.GuiConfigData.OutputStartupdelay / 100.0]);

  // If no CAN Address is provided, disable the CAN Message options
  if (CDProfileData.GuiConfigData.CanAddressOut[0] <> 0) then
    begin
    cbUploadCanUseMsgOut0.ItemIndex := 1;
    edtUploadAddressMsgOut0.Text := IntToHex(CDProfileData.GuiConfigData.CanAddressOut[0]);
    edtUploadFrequencyMsgOut0.Text := format('%d', [CDProfileData.GuiConfigData.CanMsgOutTime[0]]);
    end
  else
    begin
    cbUploadCanUseMsgOut0.ItemIndex := 0;
    edtUploadAddressMsgOut0.Enabled := False;
    edtUploadFrequencyMsgOut0.Enabled := False;
    end;

  if (CDProfileData.GuiConfigData.CanAddressOut[1] <> 0) then
    begin
    cbUploadCanUseMsgOut1.ItemIndex := 1;
    edtUploadAddressMsgOut1.Text := IntToHex(CDProfileData.GuiConfigData.CanAddressOut[1]);
    edtUploadFrequencyMsgOut1.Text := format('%d', [CDProfileData.GuiConfigData.CanMsgOutTime[1]]);
    end
  else
    begin
    cbUploadCanUseMsgOut1.ItemIndex := 0;
    edtUploadAddressMsgOut1.Enabled := False;
    edtUploadFrequencyMsgOut1.Enabled := False;
    end;

  if (CDProfileData.GuiConfigData.CanAddressOut[2] <> 0) then
    begin
    cbUploadCanUseMsgOut2.ItemIndex := 1;
    edtUploadAddressMsgOut2.Text := IntToHex(CDProfileData.GuiConfigData.CanAddressOut[2]);
    edtUploadFrequencyMsgOut2.Text := format('%d', [CDProfileData.GuiConfigData.CanMsgOutTime[2]]);
    end
  else
    begin
    cbUploadCanUseMsgOut2.ItemIndex := 0;
    edtUploadAddressMsgOut2.Enabled := False;
    edtUploadFrequencyMsgOut2.Enabled := False;
    end;

  if (CDProfileData.GuiConfigData.CanAddressOut[3] <> 0) then
    begin
    cbUploadCanUseMsgOut3.ItemIndex := 1;
    edtUploadAddressMsgOut3.Text := IntToHex(CDProfileData.GuiConfigData.CanAddressOut[3]);
    end
  else
    begin
    cbUploadCanUseMsgOut3.ItemIndex := 0;
    edtUploadAddressMsgOut3.Enabled := False;
    end;

  if (CDProfileData.GuiConfigData.CanAddressOut[4] <> 0) then
    begin
    cbUploadCanUseMsgOut4.ItemIndex := 1;
    edtUploadAddressMsgOut4.Text := IntToHex(CDProfileData.GuiConfigData.CanAddressOut[4]);
    end
  else
    begin
    cbUploadCanUseMsgOut4.ItemIndex := 0;
    edtUploadAddressMsgOut4.Enabled := False;
    end;

  if (CDProfileData.GuiConfigData.CanAddressOut[5] <> 0) then
    begin
    cbUploadCanUseMsgOut5.ItemIndex := 1;
    edtUploadAddressMsgOut5.Text := IntToHex(CDProfileData.GuiConfigData.CanAddressOut[5]);
    end
  else
    begin
    cbUploadCanUseMsgOut5.ItemIndex := 0;
    edtUploadAddressMsgOut5.Enabled := False;
    end;

  if (CDProfileData.GuiConfigData.CanAddressIn[0] <> 0) then
    begin
    cbUploadCanUseMsgIn0.ItemIndex := 1;
    edtUploadAddressMsgIn0.Text := IntToHex(CDProfileData.GuiConfigData.CanAddressIn[0]);
    edtUploadFrequencyMsgIn0.Text := format('%d', [CDProfileData.GuiConfigData.CanMsgInTime[0]]);
    edtUploadMaskMsgIn0.Text := IntToHex(CDProfileData.GuiConfigData.CanMaskIn[0]);
    end
  else
    begin
    cbUploadCanUseMsgIn0.ItemIndex := 0;
    edtUploadAddressMsgIn0.Enabled := False;
    edtUploadFrequencyMsgIn0.Enabled := False;
    edtUploadMaskMsgIn0.Enabled:=False;
    end;

  if (CDProfileData.GuiConfigData.CanAddressIn[1] <> 0) then
    begin
    cbUploadCanUseMsgIn1.ItemIndex := 1;
    edtUploadAddressMsgIn1.Text := IntToHex(CDProfileData.GuiConfigData.CanAddressIn[1]);
    edtUploadFrequencyMsgIn1.Text := format('%d', [CDProfileData.GuiConfigData.CanMsgInTime[1]]);
    edtUploadMaskMsgIn1.Text:= IntToHex(CDProfileData.GuiConfigData.CanMaskIn[1]);
    end
  else
    begin
    cbUploadCanUseMsgIn1.ItemIndex := 0;
    edtUploadAddressMsgIn1.Enabled := False;
    edtUploadFrequencyMsgIn1.Enabled := False;
    edtUploadMaskMsgIn1.Enabled:=False;
    end;



  if cbUploadConfigOutputMode.ItemIndex = 1 then
    begin
    UploadVoltageResistanceProfileGraph.HorzTitle := 'Setpoint (Volts)';
    end
  else
    begin
    UploadVoltageResistanceProfileGraph.HorzTitle := 'Setpoint (Ohms)';
    end;
  UpdateProfile(edtUploadConfigMasterSetpoint.Text, 100.0, Label250, UploadMasterFreqProfileGraph, TableResults);
  UpdateProfile(edtUploadConfigRVsetpoint.Text, 1000.0, Label245, UploadVoltageResistanceProfileGraph, TableResults);
  UpdateProfile(edtUploadConfigCanBus.Text, 100.0, Label260, UploadCanBusProfileGraph, TableResults);
  UpdateProfile(edtUploadConfigAngularOutput.Text, 1.0, Label263, UploadAngularOutputProfileGraph, TableResults);

  edtFuzzy1Input0Mf0Shape.ItemIndex:= CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[0].Shape;
  edtFuzzy1Input0Mf0Pt0.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[0].Point0]);
  edtFuzzy1Input0Mf0Pt1.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[0].Point1]);
  edtFuzzy1Input0Mf0Pt2.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[0].Point2]);
  edtFuzzy1Input0Mf0Pt3.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[0].Point3]);

  edtFuzzy1Input0Mf1Shape.ItemIndex:= CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[1].Shape;
  edtFuzzy1Input0Mf1Pt0.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[1].Point0]);
  edtFuzzy1Input0Mf1Pt1.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[1].Point1]);
  edtFuzzy1Input0Mf1Pt2.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[1].Point2]);
  edtFuzzy1Input0Mf1Pt3.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[1].Point3]);

  edtFuzzy1Input0Mf2Shape.ItemIndex:= CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[2].Shape;
  edtFuzzy1Input0Mf2Pt0.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[2].Point0]);
  edtFuzzy1Input0Mf2Pt1.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[2].Point1]);
  edtFuzzy1Input0Mf2Pt2.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[2].Point2]);
  edtFuzzy1Input0Mf2Pt3.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[2].Point3]);

  edtFuzzy1Input1Mf0Shape.ItemIndex:= CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[3].Shape;
  edtFuzzy1Input1Mf0Pt0.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[3].Point0]);
  edtFuzzy1Input1Mf0Pt1.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[3].Point1]);
  edtFuzzy1Input1Mf0Pt2.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[3].Point2]);
  edtFuzzy1Input1Mf0Pt3.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[3].Point3]);

  edtFuzzy1Input1Mf1Shape.ItemIndex:= CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[4].Shape;
  edtFuzzy1Input1Mf1Pt0.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[4].Point0]);
  edtFuzzy1Input1Mf1Pt1.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[4].Point1]);
  edtFuzzy1Input1Mf1Pt2.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[4].Point2]);
  edtFuzzy1Input1Mf1Pt3.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[4].Point3]);

  edtFuzzy1Input1Mf2Shape.ItemIndex:= CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[5].Shape;
  edtFuzzy1Input1Mf2Pt0.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[5].Point0]);
  edtFuzzy1Input1Mf2Pt1.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[5].Point1]);
  edtFuzzy1Input1Mf2Pt2.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[5].Point2]);
  edtFuzzy1Input1Mf2Pt3.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[5].Point3]);

  edtFuzzy1Input2Mf0Shape.ItemIndex:= CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[6].Shape;
  edtFuzzy1Input2Mf0Pt0.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[6].Point0]);
  edtFuzzy1Input2Mf0Pt1.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[6].Point1]);
  edtFuzzy1Input2Mf0Pt2.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[6].Point2]);
  edtFuzzy1Input2Mf0Pt3.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[6].Point3]);

  edtFuzzy1Input2Mf1Shape.ItemIndex:= CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[7].Shape;
  edtFuzzy1Input2Mf1Pt0.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[7].Point0]);
  edtFuzzy1Input2Mf1Pt1.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[7].Point1]);
  edtFuzzy1Input2Mf1Pt2.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[7].Point2]);
  edtFuzzy1Input2Mf1Pt3.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[7].Point3]);

  edtFuzzy1Input2Mf2Shape.ItemIndex:= CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[8].Shape;
  edtFuzzy1Input2Mf2Pt0.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[8].Point0]);
  edtFuzzy1Input2Mf2Pt1.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[8].Point1]);
  edtFuzzy1Input2Mf2Pt2.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[8].Point2]);
  edtFuzzy1Input2Mf2Pt3.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyInputMFs.MF[8].Point3]);

  edtFuzzy1OutputMf0Shape.ItemIndex:= CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[0].Shape;
  edtFuzzy1OutputMf0Pt0.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[0].Point0]);
  edtFuzzy1OutputMf0Pt1.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[0].Point1]);
  edtFuzzy1OutputMf0Pt2.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[0].Point2]);
  edtFuzzy1OutputMf0Pt3.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[0].Point3]);

  edtFuzzy1OutputMf1Shape.ItemIndex:= CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[1].Shape;
  edtFuzzy1OutputMf1Pt0.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[1].Point0]);
  edtFuzzy1OutputMf1Pt1.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[1].Point1]);
  edtFuzzy1OutputMf1Pt2.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[1].Point2]);
  edtFuzzy1OutputMf1Pt3.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[1].Point3]);

  edtFuzzy1OutputMf2Shape.ItemIndex:= CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[2].Shape;
  edtFuzzy1OutputMf2Pt0.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[2].Point0]);
  edtFuzzy1OutputMf2Pt1.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[2].Point1]);
  edtFuzzy1OutputMf2Pt2.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[2].Point2]);
  edtFuzzy1OutputMf2Pt3.Text:= format('%d', [CDProfileData.Fuzzy1Data.FuzzyOutputMFs.MF[2].Point3]);

  edtFuzzy2Input0Mf0Shape.ItemIndex:= CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[0].Shape;
  edtFuzzy2Input0Mf0Pt0.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[0].Point0]);
  edtFuzzy2Input0Mf0Pt1.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[0].Point1]);
  edtFuzzy2Input0Mf0Pt2.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[0].Point2]);
  edtFuzzy2Input0Mf0Pt3.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[0].Point3]);

  edtFuzzy2Input0Mf1Shape.ItemIndex:= CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[1].Shape;
  edtFuzzy2Input0Mf1Pt0.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[1].Point0]);
  edtFuzzy2Input0Mf1Pt1.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[1].Point1]);
  edtFuzzy2Input0Mf1Pt2.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[1].Point2]);
  edtFuzzy2Input0Mf1Pt3.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[1].Point3]);

  edtFuzzy2Input0Mf2Shape.ItemIndex:= CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[2].Shape;
  edtFuzzy2Input0Mf2Pt0.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[2].Point0]);
  edtFuzzy2Input0Mf2Pt1.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[2].Point1]);
  edtFuzzy2Input0Mf2Pt2.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[2].Point2]);
  edtFuzzy2Input0Mf2Pt3.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[2].Point3]);

  edtFuzzy2Input1Mf0Shape.ItemIndex:= CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[3].Shape;
  edtFuzzy2Input1Mf0Pt0.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[3].Point0]);
  edtFuzzy2Input1Mf0Pt1.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[3].Point1]);
  edtFuzzy2Input1Mf0Pt2.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[3].Point2]);
  edtFuzzy2Input1Mf0Pt3.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[3].Point3]);

  edtFuzzy2Input1Mf1Shape.ItemIndex:= CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[4].Shape;
  edtFuzzy2Input1Mf1Pt0.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[4].Point0]);
  edtFuzzy2Input1Mf1Pt1.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[4].Point1]);
  edtFuzzy2Input1Mf1Pt2.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[4].Point2]);
  edtFuzzy2Input1Mf1Pt3.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[4].Point3]);

  edtFuzzy2Input1Mf2Shape.ItemIndex:= CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[5].Shape;
  edtFuzzy2Input1Mf2Pt0.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[5].Point0]);
  edtFuzzy2Input1Mf2Pt1.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[5].Point1]);
  edtFuzzy2Input1Mf2Pt2.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[5].Point2]);
  edtFuzzy2Input1Mf2Pt3.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[5].Point3]);

  edtFuzzy2Input2Mf0Shape.ItemIndex:= CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[6].Shape;
  edtFuzzy2Input2Mf0Pt0.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[6].Point0]);
  edtFuzzy2Input2Mf0Pt1.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[6].Point1]);
  edtFuzzy2Input2Mf0Pt2.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[6].Point2]);
  edtFuzzy2Input2Mf0Pt3.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[6].Point3]);

  edtFuzzy2Input2Mf1Shape.ItemIndex:= CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[7].Shape;
  edtFuzzy2Input2Mf1Pt0.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[7].Point0]);
  edtFuzzy2Input2Mf1Pt1.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[7].Point1]);
  edtFuzzy2Input2Mf1Pt2.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[7].Point2]);
  edtFuzzy2Input2Mf1Pt3.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[7].Point3]);

  edtFuzzy2Input2Mf2Shape.ItemIndex:= CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[8].Shape;
  edtFuzzy2Input2Mf2Pt0.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[8].Point0]);
  edtFuzzy2Input2Mf2Pt1.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[8].Point1]);
  edtFuzzy2Input2Mf2Pt2.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[8].Point2]);
  edtFuzzy2Input2Mf2Pt3.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyInputMFs.MF[8].Point3]);

  edtFuzzy2OutputMf0Shape.ItemIndex:= CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[0].Shape;
  edtFuzzy2OutputMf0Pt0.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[0].Point0]);
  edtFuzzy2OutputMf0Pt1.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[0].Point1]);
  edtFuzzy2OutputMf0Pt2.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[0].Point2]);
  edtFuzzy2OutputMf0Pt3.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[0].Point3]);

  edtFuzzy2OutputMf1Shape.ItemIndex:= CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[1].Shape;
  edtFuzzy2OutputMf1Pt0.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[1].Point0]);
  edtFuzzy2OutputMf1Pt1.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[1].Point1]);
  edtFuzzy2OutputMf1Pt2.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[1].Point2]);
  edtFuzzy2OutputMf1Pt3.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[1].Point3]);

  edtFuzzy2OutputMf2Shape.ItemIndex:= CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[2].Shape;
  edtFuzzy2OutputMf2Pt0.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[2].Point0]);
  edtFuzzy2OutputMf2Pt1.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[2].Point1]);
  edtFuzzy2OutputMf2Pt2.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[2].Point2]);
  edtFuzzy2OutputMf2Pt3.Text:= format('%d', [CDProfileData.Fuzzy2Data.FuzzyOutputMFs.MF[2].Point3]);

  Fuzzy1RuleMemo.Lines.Clear;

  for i := 0 to CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleCount - 1 do
    begin
    Fuzzy1RuleMemo.Lines.Add(FuzzyRulesToText(i+1, CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[i].Input0, CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[i].Input1, CDProfileData.Fuzzy1Data.FuzzyInputRules.InputRules[i].Input2,
    CDProfileData.Fuzzy1Data.FuzzyOutputRules.OutputRule[i], CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleType[i], CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleWeight[i]));
    end;

  Fuzzy2RuleMemo.Lines.Clear;

  for i := 0 to CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleCount - 1 do
    begin
    Fuzzy2RuleMemo.Lines.Add(FuzzyRulesToText(i+1, CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[i].Input0, CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[i].Input1, CDProfileData.Fuzzy2Data.FuzzyInputRules.InputRules[i].Input2,
    CDProfileData.Fuzzy2Data.FuzzyOutputRules.OutputRule[i], CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleType[i], CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleWeight[i]));
    end;

  Fuzzy1RuleCount := CDProfileData.Fuzzy1Data.FuzzyRuleInfo.RuleCount;
  Fuzzy2RuleCount := CDProfileData.Fuzzy2Data.FuzzyRuleInfo.RuleCount;

  lblFuzzy1NumberRules.Caption:= format('%d', [Fuzzy1RuleCount]);
  lblFuzzy2NumberRules.Caption:= format('%d', [Fuzzy2RuleCount]);

  bLoading := bSaveLoading;
end;

//******************************************************************************************************************

procedure TForm1.SaveToIniFile;
var
  NumberAngles, NumberIndexes, i, j, StrIndex, AlarmMode, ConfigFlags: integer;
  TmpStr: string;
  FloatTmp1: single;
  IniFile: TIniFile;
  ProfileStrKey: array [0..99] of string;
  ProfileStrValue: array [0..99] of string;
  ProfileStrIndex: integer;
begin
  if cbCDProfileNames.Text = '' then
    begin
    TestMessage('ERROR: Profile Name Missing', 0);
    exit;
    end;

  ProfileStrIndex := 0;
  ProfileStrKey[ProfileStrIndex] := 'DIGIO1';
  ProfileStrValue[ProfileStrIndex] :=  format('%u', [CDProfileData.GuiConfigData.BF2.DigIo1]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'DIGIO2';
  ProfileStrValue[ProfileStrIndex] :=  format('%u', [CDProfileData.GuiConfigData.BF2.DigIo2]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'DIGIO3';
  ProfileStrValue[ProfileStrIndex] :=  format('%u', [CDProfileData.GuiConfigData.BF2.DigIo3]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'DIGIO4';
  ProfileStrValue[ProfileStrIndex] :=  format('%u', [CDProfileData.GuiConfigData.BF2.DigIo4]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'DIGIO5';
  ProfileStrValue[ProfileStrIndex] :=  format('%u', [CDProfileData.GuiConfigData.BF2.DigIo5]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'DIGIO6';
  ProfileStrValue[ProfileStrIndex] :=  format('%u', [CDProfileData.GuiConfigData.BF2.DigIo6]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'DEFAULT_OUTPUT';
  ProfileStrValue[ProfileStrIndex] :=  format('%u', [CDProfileData.GuiConfigData.BF8.DefaultOutput]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'SENSOR_MODE';
  ProfileStrValue[ProfileStrIndex] :=  format('%u', [CDProfileData.GuiConfigData.BF8.SensorMode]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_USE';
  ProfileStrValue[ProfileStrIndex] :=  format('%u', [CDProfileData.GuiConfigData.BF8.CanUse]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'SOURCE_ADDRESS';
  ProfileStrValue[ProfileStrIndex] :=  format('%u', [CDProfileData.GuiConfigData.BF8.SourceAddress]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_ECHO';
  ProfileStrValue[ProfileStrIndex] :=  format('%u', [CDProfileData.GuiConfigData.BF8.CanEcho]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_CRC_ENABLE';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.GuiConfigData.BF8.CanCrcEnable]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_PRESCALER';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.BF8.CanPrescaler]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_ENDIANESS';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.BF8.Endianess]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_ADDRESS_OUT_0';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.GuiConfigData.CanAddressOut[0]]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_FREQUENCY_OUT_0';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.GuiConfigData.CanMsgOutTime[0]]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_ADDRESS_OUT_1';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.GuiConfigData.CanAddressOut[1]]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_FREQUENCY_OUT_1';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.GuiConfigData.CanMsgOutTime[1]]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_ADDRESS_OUT_2';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.GuiConfigData.CanAddressOut[2]]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_FREQUENCY_OUT_2';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.GuiConfigData.CanMsgOutTime[2]]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_ADDRESS_OUT_3';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.GuiConfigData.CanAddressOut[3]]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_ADDRESS_OUT_4';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.GuiConfigData.CanAddressOut[4]]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_ADDRESS_OUT_5';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.GuiConfigData.CanAddressOut[5]]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_ADDRESS_IN_0';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.GuiConfigData.CanAddressIn[0]]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_FREQUENCY_IN_0';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.GuiConfigData.CanMsgInTime[0]]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_MASK_IN_0';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.GuiConfigData.CanMaskIn[0]]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_ADDRESS_IN_1';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.GuiConfigData.CanAddressIn[1]]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_FREQUENCY_IN_1';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.GuiConfigData.CanMsgInTime[1]]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_MASK_IN_1';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.GuiConfigData.CanMaskIn[1]]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_SAMPLING_DELAY';
  ProfileStrValue[ProfileStrIndex] := format('%1.2f', [CDProfileData.GuiConfigData.CanSamplingDelay/100.0]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'OUTPUT_STARTUP_DELAY';
  ProfileStrValue[ProfileStrIndex] := format('%1.2f', [CDProfileData.GuiConfigData.OutputStartupdelay/100.0]);
  ProfileStrIndex := ProfileStrIndex + 1;


  ProfileStrKey[ProfileStrIndex] := 'KMA_FILTER_GAIN';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.KmaFilterGain]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'MASTER_FILTER_GAIN';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.MasterIndexFilteringGain]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'ACCEL_X_FILTER_GAIN';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.AccelVoltageXAdcFilteringGain]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'ACCEL_Y_FILTER_GAIN';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.AccelVoltageYAdcFilteringGain]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'ACCEL_Z_FILTER_GAIN';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.AccelVoltageZAdcFilteringGain]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'KMA_REV';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.BF3.ReverseIndexKma]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'ERROR_OUTPUT_ACTIVE_LEVEL';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.BF1.ErrorConditionActiveState]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'RV_OUTPUT_MODE';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.BF1.RVOutputMode]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'RV_OUTPUT_PULLUP';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.BF1.RVOutputPullup]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'MASTER_FREQ_MODE';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.BF1.MasterFreqMode]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'RS485_MODE';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.BF3.RS485MessageMode]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'RS485_TIME';
  ProfileStrValue[ProfileStrIndex] := format('%1.1f', [CDProfileData.GuiConfigData.BF3.RS485MessageTime / 10.0]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'ACCELEROMETER_SCALING';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.BF3.AccelerometerScaling]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'MASTER_FREQUENCY_TABLE';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetMasterFreqSetpointTable]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'CAN_BUS_TABLE';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetCanBusTable]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'ANGULAR_OUTPUT_TABLE';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetAngularOutputTable]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'RV_SETPOINT_TABLE';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetRVSetpointTable]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY1_MF_SHAPE_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyMFTable(0)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY1_MF_POINT0_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyMFTable(1)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY1_MF_POINT1_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyMFTable(2)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY1_MF_POINT2_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyMFTable(3)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY1_MF_POINT3_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyMFTable(4)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY2_MF_SHAPE_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyMFTable(10)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY2_MF_POINT0_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyMFTable(11)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY2_MF_POINT1_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyMFTable(12)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY2_MF_POINT2_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyMFTable(13)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY2_MF_POINT3_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyMFTable(14)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY1_INPUT0_RULE_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyRules(0)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY1_INPUT1_RULE_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyRules(1)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY1_INPUT2_RULE_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyRules(2)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY1_OUTPUT_RULE_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyRules(3)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY1_OUTPUT_MIN';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.Fuzzy1Data.FuzzyOutputInfo.Min]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY1_OUTPUT_MAX';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.Fuzzy1Data.FuzzyOutputInfo.Max]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY1_RULE_WEIGHT_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyRules(4)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY1_RULE_CONNECTION_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyRules(5)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY2_INPUT0_RULE_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyRules(10)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY2_INPUT1_RULE_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyRules(11)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY2_INPUT2_RULE_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyRules(12)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY2_OUTPUT_RULE_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyRules(13)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY2_OUTPUT_MIN';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.Fuzzy2Data.FuzzyOutputInfo.Min]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY2_OUTPUT_MAX';
  ProfileStrValue[ProfileStrIndex] := format('%d', [CDProfileData.Fuzzy2Data.FuzzyOutputInfo.Max]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY2_RULE_WEIGHT_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyRules(14)]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'FUZZY2_RULE_CONNECTION_ARRAY';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetFuzzyRules(15)]);
  ProfileStrIndex := ProfileStrIndex + 1;


  IniFile := TIniFile.Create(AppPath + 'PROFILES_CD.INI');
  for i := 0 to ProfileStrIndex - 1 do
    begin
    IniFile.WriteString(cbCDProfileNames.Text, ProfileStrKey[i], ProfileStrValue[i]);
    end;
  IniFile.Free;

  ReloadIniFile();

end;

//******************************************************************************************************************

procedure TForm1.Button8Click(Sender: TObject);
begin
  pnlReadConfig.Visible := true;
  lblMessage.Caption := 'Reading Event Log Data...';
  ProgressBar1.Max := 127;
  ProgressBar1.Position := 0;
  bDownloadEventLog := true;
end;

//******************************************************************************************************************
procedure ConvertRuleMemoToRulesStrutcure;
begin

end;


//******************************************************************************************************************
//
//
procedure TForm1.LoadFromGui;
begin
  CDProfileData.GuiConfigData.ProfileName := cbCDProfileNames.Text;
  CDProfileData.GuiConfigData.BF2.DigIo1 := IndextoDIG1[cbUploadConfigDig1.ItemIndex];
  CDProfileData.GuiConfigData.BF2.DigIo2 := IndextoDIG2[cbUploadConfigDig2.ItemIndex];
  CDProfileData.GuiConfigData.BF2.DigIo3 := IndextoDIG3[cbUploadConfigDig3.ItemIndex];
  CDProfileData.GuiConfigData.BF2.DigIo4 := IndextoDIG4[cbUploadConfigDig4.ItemIndex];
  CDProfileData.GuiConfigData.BF2.DigIo5 := IndextoDIG5[cbUploadConfigDig5.ItemIndex];
  CDProfileData.GuiConfigData.BF2.DigIo6 := IndextoDIG6[cbUploadConfigDig6.ItemIndex];

  CDProfileData.GuiConfigData.BF8.DefaultOutput := rgDefaultOutput.ItemIndex;
  CDProfileData.GuiConfigData.BF8.SensorMode := rgUploadConfigSensorMode.ItemIndex;
  CDProfileData.GuiConfigData.BF8.CanUse := cbUploadConfigCanUse.ItemIndex;
  CDProfileData.GuiConfigData.BF8.SourceAddress := ConvertToFloat(edtUploadConfigSourceAddress.Text, 1.0);
  CDProfileData.GuiConfigData.BF8.CanEcho := cbUploadConfigCanEcho.ItemIndex;
  CDProfileData.GuiConfigData.BF8.CanCrcEnable := cbUploadConfigCanCrcEnable.ItemIndex;
  CDProfileData.GuiConfigData.BF8.CanPrescaler:= CanBaudtoPrescaler[cbUploadConfigCanBaudRate.ItemIndex];
  CDProfileData.GuiConfigData.BF8.Endianess := cbUploadConfigEndianess.ItemIndex;

  if (cbUploadCanUseMsgOut0.ItemIndex <> 0) then
    begin
    CDProfileData.GuiConfigData.CanAddressOut[0] := ConvertToFloat(edtUploadAddressMsgOut0.Text, 1.0);
    CDProfileData.GuiConfigData.CanMsgOutTime[0] := ConvertToFloat(edtUploadFrequencyMsgOut0.Text, 1.0);
    end
  else
    begin
    CDProfileData.GuiConfigData.CanAddressOut[0] := 0;
    CDProfileData.GuiConfigData.CanMsgOutTime[0] := 0;
    end;

  if (cbUploadCanUseMsgOut1.ItemIndex <> 0) then
    begin
    CDProfileData.GuiConfigData.CanAddressOut[1] := ConvertToFloat(edtUploadAddressMsgOut1.Text, 1.0);
    CDProfileData.GuiConfigData.CanMsgOutTime[1] := ConvertToFloat(edtUploadFrequencyMsgOut1.Text, 1.0);
    end
  else
    begin
    CDProfileData.GuiConfigData.CanAddressOut[1] := 0;
    CDProfileData.GuiConfigData.CanMsgOutTime[1] := 0;
    end;

  if (cbUploadCanUseMsgOut2.ItemIndex <> 0) then
    begin
    CDProfileData.GuiConfigData.CanAddressOut[2] := ConvertToFloat(edtUploadAddressMsgOut2.Text, 1.0);
    CDProfileData.GuiConfigData.CanMsgOutTime[2] := ConvertToFloat(edtUploadFrequencyMsgOut2.Text, 1.0);
    end
  else
    begin
    CDProfileData.GuiConfigData.CanAddressOut[2] := 0;
    CDProfileData.GuiConfigData.CanMsgOutTime[2] := 0;
    end;

  if (cbUploadCanUseMsgOut3.ItemIndex <> 0) then
    begin
    CDProfileData.GuiConfigData.CanAddressOut[3] := ConvertToFloat(edtUploadAddressMsgOut3.Text, 1.0);
    end
  else
    begin
    CDProfileData.GuiConfigData.CanAddressOut[3] := 0;
    end;

  if (cbUploadCanUseMsgOut4.ItemIndex <> 0) then
    begin
    CDProfileData.GuiConfigData.CanAddressOut[4] := ConvertToFloat(edtUploadAddressMsgOut4.Text, 1.0);
    end
  else
    begin
    CDProfileData.GuiConfigData.CanAddressOut[4] := 0;
    end;

  if (cbUploadCanUseMsgOut5.ItemIndex <> 0) then
    begin
    CDProfileData.GuiConfigData.CanAddressOut[5] := ConvertToFloat(edtUploadAddressMsgOut5.Text, 1.0);
    end
  else
    begin
    CDProfileData.GuiConfigData.CanAddressOut[5] := 0;
    end;

  if (cbUploadCanUseMsgIn0.ItemIndex <> 0) then
    begin
    CDProfileData.GuiConfigData.CanAddressIn[0] := ConvertToFloat(edtUploadAddressMsgIn0.Text, 1.0);
    CDProfileData.GuiConfigData.CanMsgInTime[0] := ConvertToFloat(edtUploadFrequencyMsgIn0.Text, 1.0);
    CDProfileData.GuiConfigData.CanMaskIn[0]:= ConvertToFloat(edtUploadMaskMsgIn0.Text, 1.0);
    end
  else
    begin
    CDProfileData.GuiConfigData.CanAddressIn[0] := 0;
    CDProfileData.GuiConfigData.CanMsgInTime[0] := 0;
    CDProfileData.GuiConfigData.CanMaskIn[0] := 0;
    end;

  if (cbUploadCanUseMsgOut0.ItemIndex <> 0) then
    begin
    CDProfileData.GuiConfigData.CanAddressIn[1] := ConvertToFloat(edtUploadAddressMsgIn1.Text, 1.0);
    CDProfileData.GuiConfigData.CanMsgInTime[1] := ConvertToFloat(edtUploadFrequencyMsgIn1.Text, 1.0);
    CDProfileData.GuiConfigData.CanMaskIn[1] := ConvertToFloat(edtUploadMaskMsgIn1.Text, 1.0);
    end
  else
    begin
    CDProfileData.GuiConfigData.CanAddressIn[1] := 0;
    CDProfileData.GuiConfigData.CanMsgInTime[1] := 0;
    CDProfileData.GuiConfigData.CanMaskIn[1] := 0;
    end;

  CDProfileData.GuiConfigData.KmaFilterGain := ConvertToFloat(edtUploadConfigKmaGain.Text, 1.0);
  CDProfileData.GuiConfigData.MasterIndexFilteringGain := ConvertToFloat(edtUploadConfigMasterGain.Text, 1.0);
  CDProfileData.GuiConfigData.AccelVoltageXAdcFilteringGain := ConvertToFloat(edtUploadConfigAccelXGain.Text, 1.0);
  CDProfileData.GuiConfigData.AccelVoltageYAdcFilteringGain := ConvertToFloat(edtUploadConfigAccelYGain.Text, 1.0);
  CDProfileData.GuiConfigData.AccelVoltageZAdcFilteringGain := ConvertToFloat(edtUploadConfigAccelZGain.Text, 1.0);
  CDProfileData.GuiConfigData.BF3.ReverseIndexKma := BooleanToInteger(cbUploadConfigReverseKma.Checked);
  CDProfileData.GuiConfigData.BF1.ErrorConditionActiveState := cbUploadConfigErrorActiveLevel.ItemIndex;
  CDProfileData.GuiConfigData.BF1.RVOutputMode := cbUploadConfigOutputMode.ItemIndex;
  CDProfileData.GuiConfigData.BF1.RVOutputPullup := cbUploadConfigPullUpMode.ItemIndex;
  CDProfileData.GuiConfigData.BF1.MasterFreqMode := cbUploadConfigMasterMode.ItemIndex;
  CDProfileData.GuiConfigData.BF3.RS485MessageMode := cbUploadConfigRS485Format.ItemIndex;
  CDProfileData.GuiConfigData.BF3.RS485MessageTime := ConvertToFloat(edtUploadConfigRS485Time.Text, 10.0);
  CDProfileData.GuiConfigData.BF3.AccelerometerScaling := cbUploadConfigAccelerometerScaling.ItemIndex;

  CDProfileData.GuiConfigData.CanSamplingDelay := ConvertToFloat(edtCanSamplingDelay.Text, 100.0);
  CDProfileData.GuiConfigData.OutputStartupdelay := ConvertToFloat(edtOutputStartupdelay.Text, 100.0);



  if cbUploadConfigOutputMode.ItemIndex = 1 then
    begin
    UploadVoltageResistanceProfileGraph.HorzTitle := 'Setpoint (Volts)';
    end;
  if cbUploadConfigOutputMode.ItemIndex = 2 then
    begin
    UploadVoltageResistanceProfileGraph.HorzTitle := 'Setpoint (Ohms)';
    end;
  UpdateProfile(edtUploadConfigMasterSetpoint.Text, 100.0, Label250, UploadMasterFreqProfileGraph, CDProfileData.GuiConfigData.MasterFreqTable);
  UpdateProfile(edtUploadConfigRVsetpoint.Text, 1000.0, Label245, UploadVoltageResistanceProfileGraph, CDProfileData.GuiConfigData.SetpointTable);
  UpdateProfile(edtUploadConfigCanBus.Text, 100.0, Label260, UploadCanBusProfileGraph, CDProfileData.GuiConfigData.CanTable);
  UpdateProfile(edtUploadConfigAngularOutput.Text, 1.0, Label263, UploadAngularOutputProfileGraph, CDProfileData.GuiConfigData.AngularOutputTable);

  if (cbUploadCanUseMsgOut0.ItemIndex = 0) then
    begin
    CDProfileData.GuiConfigData.CanAddressOut[0]:=0;
    CDProfileData.GuiConfigData.CanMsgOutTime[0]:=0;
    end
  else
    begin
    CDProfileData.GuiConfigData.CanAddressOut[0]:=HexTextToInteger(edtUploadAddressMsgOut0.Text);
    CDProfileData.GuiConfigData.CanMsgOutTime[0]:=ConvertToFloat(edtUploadFrequencyMsgOut0.Text, 1.0);
    end;

  if (cbUploadCanUseMsgOut1.ItemIndex = 0) then
    begin
    CDProfileData.GuiConfigData.CanAddressOut[1]:=0;
    CDProfileData.GuiConfigData.CanMsgOutTime[1]:=0;
    end
  else
    begin
    CDProfileData.GuiConfigData.CanAddressOut[1]:=HexTextToInteger(edtUploadAddressMsgOut1.Text);
    CDProfileData.GuiConfigData.CanMsgOutTime[1]:=ConvertToFloat(edtUploadFrequencyMsgOut1.Text, 1.0);
    end;

  if (cbUploadCanUseMsgOut2.ItemIndex = 0) then
    begin
    CDProfileData.GuiConfigData.CanAddressOut[2]:=0;
    CDProfileData.GuiConfigData.CanMsgOutTime[2]:=0;

    end
  else
    begin
    CDProfileData.GuiConfigData.CanAddressOut[2]:=HexTextToInteger(edtUploadAddressMsgOut2.Text);
    CDProfileData.GuiConfigData.CanMsgOutTime[2]:=ConvertToFloat(edtUploadFrequencyMsgOut2.Text, 1.0);
    end;

  if (cbUploadCanUseMsgOut3.ItemIndex = 0) then
    begin
    CDProfileData.GuiConfigData.CanAddressOut[3]:=0;
    end
  else
    begin
    CDProfileData.GuiConfigData.CanAddressOut[3]:=HexTextToInteger(edtUploadAddressMsgOut3.Text);
    end;

  if (cbUploadCanUseMsgOut4.ItemIndex = 0) then
    begin
    CDProfileData.GuiConfigData.CanAddressOut[4]:=0;
    end
  else
    begin
    CDProfileData.GuiConfigData.CanAddressOut[4]:=HexTextToInteger(edtUploadAddressMsgOut4.Text);
    end;

  if (cbUploadCanUseMsgOut5.ItemIndex = 0) then
    begin
    CDProfileData.GuiConfigData.CanAddressOut[5]:=0;
    end
  else
    begin
    CDProfileData.GuiConfigData.CanAddressOut[5]:=HexTextToInteger(edtUploadAddressMsgOut5.Text);
    end;

  if (cbUploadCanUseMsgIn0.ItemIndex = 0) then
    begin
    CDProfileData.GuiConfigData.CanAddressIn[0]:=0;
    CDProfileData.GuiConfigData.CanMsgInTime[0]:=0;
    CDProfileData.GuiConfigData.CanMaskIn[0]:=0;
    end
  else
    begin
    CDProfileData.GuiConfigData.CanAddressIn[0]:=HexTextToInteger(edtUploadAddressMsgIn0.Text);
    CDProfileData.GuiConfigData.CanMsgInTime[0]:=ConvertToFloat(edtUploadFrequencyMsgIn0.Text, 1.0);
    CDProfileData.GuiConfigData.CanMaskIn[0]:=HexTextToInteger(edtUploadMaskMsgIn0.Text);
    end;

  if (cbUploadCanUseMsgIn1.ItemIndex = 0) then
    begin
    CDProfileData.GuiConfigData.CanAddressIn[1]:=0;
    CDProfileData.GuiConfigData.CanMsgInTime[1]:=0;
    CDProfileData.GuiConfigData.CanMaskIn[1]:=0;
    end
  else
    begin
    CDProfileData.GuiConfigData.CanAddressIn[1]:=HexTextToInteger(edtUploadAddressMsgIn1.Text);
    CDProfileData.GuiConfigData.CanMsgInTime[1]:=ConvertToFloat(edtUploadFrequencyMsgIn1.Text, 1.0);
    CDProfileData.GuiConfigData.CanMaskIn[1]:=HexTextToInteger(edtUploadMaskMsgIn1.Text);
    end;

end;

//******************************************************************************************************************
//
//
procedure TForm1.LoadFactoryFromGui;
begin
  CDProfileData.GuiCDFactoryData.BF5.HardwareVersion := cbPcbRevision.ItemIndex - 1;
  CDProfileData.GuiCDFactoryData.BF5.AccelPresent := BooleanToInteger(cbAccelerometer.Checked);
  CDProfileData.GuiCDFactoryData.BF5.RVOutputPresent := BooleanToInteger(cbRVOutput.Checked);
  CDProfileData.GuiCDFactoryData.BF5.RS485DriverPresent := BooleanToInteger(cbRS485Driver.Checked);
  CDProfileData.GuiCDFactoryData.BF5.CanDriverPresent := BooleanToInteger(cbCANBusDriver.Checked);
  CDProfileData.GuiCDFactoryData.BF5.TempSensorPresent := BooleanToInteger(cbTemperatureSensor.Checked);
  CDProfileData.GuiCDFactoryData.BF5.KmaPresent := BooleanToInteger(cbKmaSensor.Checked);

  CDProfileData.GuiCDFactoryData.Calibration33 := ConvertToFloat(edtConfig33Calibration.Text, 1000.0);
  CDProfileData.GuiCDFactoryData.PowerRail5KFactor := ConvertToFloat(edtConfig5VDCCalibration.Text, 1.0);
  CDProfileData.GuiCDFactoryData.KmaKFactor := ConvertToFloat(edtConfigKmaCalibration.Text, 1.0);
  CDProfileData.GuiCDFactoryData.PowerInputKFactor := ConvertToFloat(edtConfigPowerInputVoltageCalibration.Text, 1.0);
  CDProfileData.GuiCDFactoryData.RVOutputVoltageKFactor := ConvertToFloat(edtConfigRVVoltageCalibration.Text, 1.0);
  CDProfileData.GuiCDFactoryData.RVOutputCurrentKFactor := ConvertToFloat(edtConfigRVCurrentScaleCalibration.Text, 1.0);
  CDProfileData.GuiCDFactoryData.PwmOutputKFactor := ConvertToFloat(edtConfigPwmOutput.Text, 1.0);

  CDProfileData.GuiCDFactoryData.PowerInputFilteringGain := ConvertToFloat(edtConfigPowerInputFilteringGain.Text, 1.0);
  CDProfileData.GuiCDFactoryData.PowerRail5FilteringGain := ConvertToFloat(edtConfig5VDCFilteringGain.Text, 1.0);
  CDProfileData.GuiCDFactoryData.TemperatureFilteringGain := ConvertToFloat(edtConfigTemperatureFilteringGain.Text, 1.0);
  CDProfileData.GuiCDFactoryData.InternalReferenceFilteringGain := ConvertToFloat(edtConfigInternalReferneceFilteringGain.Text, 1.0);
  CDProfileData.GuiCDFactoryData.RVOutputVoltageFilteringGain := ConvertToFloat(edtConfigRVVoltageFilteringGain.Text, 1.0);
  CDProfileData.GuiCDFactoryData.RVOutputCurrentFilteringGain := ConvertToFloat(edtConfigRVCurrentFilteringGain.Text, 1.0);

  CDProfileData.GuiCDFactoryData.TimeStr := edtConfigManufacturingTime.Text;
  CDProfileData.GuiCDFactoryData.DateStr := edtConfigManufacturingDate.Text;
  CDProfileData.GuiCDFactoryData.NameTesterStr := edtConfigManufacturingNameTester.Text;
  CDProfileData.GuiCDFactoryData.ModelNumberStr := edtConfigManufacturingModelNumber.Text;
  CDProfileData.GuiCDFactoryData.SerialNumberStr := edtConfigManufacturingSerialNumber.Text;

  CDProfileData.GuiCDFactoryData.LocationStr := edtConfigManufacturingLocation.Text;
  CDProfileData.GuiCDFactoryData.TypeStr := edtConfigManufacturingType.Text;
  CDProfileData.GuiCDFactoryData.ManufacturerStr := edtConfigManufacturingManufacturer.Text;
  CDProfileData.GuiCDFactoryData.HardwareIdStr := edtConfigManufacturingHardwareId.Text;
  CDProfileData.GuiCDFactoryData.SoftwareIdStr := edtConfigManufacturingSoftwareId.Text;
  CDProfileData.GuiCDFactoryData.MakeStr := edtConfigManufacturingMake.Text;
  CDProfileData.GuiCDFactoryData.ModeStr := edtConfigManufacturingModel.Text;
  CDProfileData.GuiCDFactoryData.SerialStr := edtConfigManufacturingSerial.Text;
  CDProfileData.GuiCDFactoryData.UnitStr := edtConfigManufacturingUnit.Text;

  // Set delimeter for Bye Aerospace

  CDProfileData.GuiCDFactoryData.LocationStr[37] := '"';
  CDProfileData.GuiCDFactoryData.LocationStr[38] := '*';
  CDProfileData.GuiCDFactoryData.LocationStr[39] := '"';

  CDProfileData.GuiCDFactoryData.TypeStr[37] := '"';
  CDProfileData.GuiCDFactoryData.TypeStr[38] := '*';
  CDProfileData.GuiCDFactoryData.TypeStr[39] := '"';

  CDProfileData.GuiCDFactoryData.ManufacturerStr[37] := '"';
  CDProfileData.GuiCDFactoryData.ManufacturerStr[38] := '*';
  CDProfileData.GuiCDFactoryData.ManufacturerStr[39] := '"';

  CDProfileData.GuiCDFactoryData.HardwareIdStr[37] := '"';
  CDProfileData.GuiCDFactoryData.HardwareIdStr[38] := '*';
  CDProfileData.GuiCDFactoryData.HardwareIdStr[39] := '"';

  CDProfileData.GuiCDFactoryData.SoftwareIdStr[37] := '"';
  CDProfileData.GuiCDFactoryData.SoftwareIdStr[38] := '*';
  CDProfileData.GuiCDFactoryData.SoftwareIdStr[39] := '"';

  CDProfileData.GuiCDFactoryData.ModeStr[37] := '"';
  CDProfileData.GuiCDFactoryData.ModeStr[38] := '*';
  CDProfileData.GuiCDFactoryData.ModeStr[39] := '"';

  CDProfileData.GuiCDFactoryData.MakeStr[5] := '"';
  CDProfileData.GuiCDFactoryData.MakeStr[6] := '*';
  CDProfileData.GuiCDFactoryData.MakeStr[7] := '"';

  CDProfileData.GuiCDFactoryData.SerialStr[37] := '"';
  CDProfileData.GuiCDFactoryData.SerialStr[38] := '*';
  CDProfileData.GuiCDFactoryData.SerialStr[39] := '"';

  CDProfileData.GuiCDFactoryData.UnitStr[37] := '"';
  CDProfileData.GuiCDFactoryData.UnitStr[38] := '*';
  CDProfileData.GuiCDFactoryData.UnitStr[39] := '"';

  CDProfileData.GuiCDFactoryData.DateStr[13] := '"';
  CDProfileData.GuiCDFactoryData.DateStr[14] := '*';
  CDProfileData.GuiCDFactoryData.DateStr[15] := '"';

  CDProfileData.GuiCDFactoryData.ModelNumberStr[37] := '"';
  CDProfileData.GuiCDFactoryData.ModelNumberStr[38] := '*';
  CDProfileData.GuiCDFactoryData.ModelNumberStr[39] := '"';

  CDProfileData.GuiCDFactoryData.SerialNumberStr[37] := '"';
  CDProfileData.GuiCDFactoryData.SerialNumberStr[38] := '*';
  CDProfileData.GuiCDFactoryData.SerialNumberStr[39] := '"';


end;

//******************************************************************************************************************

procedure TForm1.btnUploadProfileClick(Sender: TObject);
var
  TestCase: integer;
  bPowerAlreadyOn, CompareStatus: boolean;

label
  ExitNow, ExitToTop;
begin
  //
  // Store GUI values into CDProfileData.GUIConfigData
  //
  LoadFromGui();
  move(CDProfileData.Fuzzy1Data, CD_43_Upload.Fuzzy1, sizeof(CD_43_Upload.Fuzzy1));
  move(CDProfileData.Fuzzy2Data, CD_43_Upload.Fuzzy2, sizeof(CD_43_Upload.Fuzzy2));
  //
  // Copy CDProfileData.GUIConfigData values into CD_41_Upload and CD_43_Upload
  //
  CDProfileData.UpdateCD_41();
  if rgUploadFuzzyYN.ItemIndex = 1 then
    begin
      CDProfileData.UpdateCD_43();
    end;

  if btnStart.Caption = 'ABORT' then
    begin
    bAbort := True;
    end;
  btnStart.Caption := 'ABORT';

  memo1.Clear;
  EventLogger.WriteStr(' ', False);
  EventLogger.WriteStr('=================================================================================================', False);
  EventLogger.WriteStr('---------------------------------------  START PROGRAMMING  -------------------------------------', False);
  EventLogger.WriteStr('=================================================================================================', False);
  EventLogger.WriteStr(format('Name of Tester: %s', [edtNameOfTester.Text]), True);
  EventLogger.WriteStr(format('Product Serial Number:  %s', [edtSerialNumber.Text]), True);
  EventLogger.WriteStr(format('Name of Profile: %s', [cbCDProfileNames.Text]), True);
  EventLogger.WriteStr(' ', False);

  TestCase := 0;
  bEndTest := False;
  bPowerAlreadyOn := false;
  WaitTime := 0;
  while (not bEndTest) do
    begin
ExitToTop:
    if bAbort then
      begin
      InitializeTest();
      TestMessage('PROGRAMMING ABORTED BY USER!!', 0);
      EventLogger.WriteStr('------>> PROGRAMMING ABORTED BY USER <------', True);
      goto ExitNow;
      end;

    // If WaitTime <> 0, then Add a 10mS delay into loop time.

    if WaitTime <> 0 then
      WaitTime := WaitTime - 1;
    Sleep(1);
    Application.ProcessMessages;

    // This Case statement controls each test we are running

    case (TestCase) of
        0:
          begin
          if CD_70_UpdateCounter <> 0 then       // Power On and communicating
            begin
            bPowerAlreadyOn := true;
            WaitTime := 10;
            TestCase := 300;
            end
          else
            begin
            EventLogger.WriteStr('Turn on Power', True);
            TestMessage('Turning On Power', 0);
            PowerControl(TRUE);
            bEnableLogging := true;
            WaitTime := 2000;
            TestCase := 200;
            end;
          end;

        200:
          begin
          //
          // Once we see DUT_UpdateCounter change at least 10 times, we can safely assume that the Programmer is communicating with
          // Sender, therefore reduce WaitTime
          //
          if (WaitTime > 20) and (CD_70_UpdateCounter <> 0) then
            WaitTime := 20;
          //
          // If ENABLE_POWER bit resets, then excess current has been detected and power was automatically turned off, therefore abort test
          //
          if (SetpointData.EnableBits and ENABLE_POWER) = 0 then
            begin
            EventLogger.WriteStr('ERROR: Excess Current Detected, Programming Aborted', True);
            TestMessage('ERROR: Excess Current Detected', 0);
            TestCase := 10000;
            goto ExitToTop;
            end;
          //
          // Wait for WaitTime to reach zero
          //
          if WaitTime <> 0 then
            goto ExitToTop;
          //
          // If DUT_UpdateCounter hasn't changed, then we aren't communicating with Sender, therefore abort test.
          //
          if (CD_70_UpdateCounter = 0) then
            begin
            EventLogger.WriteStr('ERROR: Unable to Communicate with Fuel Sender, Programming Aborted', True);
            TestMessage('ERROR: Unable to Communicate with Sender', 0);
            TestCase := 10000;
            goto ExitToTop;
            end;

          WaitTime := 20;
          TestCase := 300;
          goto ExitToTop;
          end;

        300:
          begin
          if WaitTime <> 0 then
            goto ExitToTop;
          //
          // Write Flash Configuration Data to Sender
          //
          EventLogger.WriteStr('Uploading Flash Configuration Data', True);
          TestMessage('Uploading Flash Configuration Data', 0);
          bUploadFlashConfiguration := true;
          bUploadDone := false;
          CDVerifyResults := -1;
          WaitTime := 1000;
          TestCase := 400;
          goto ExitToTop;
          end;

        400:
          begin
          if WaitTime = 0 then
            begin
            EventLogger.WriteStr('ERROR:  Unable to Upload Flash Configuration Data', True);
            TestMessage('ERROR:  Unable to Upload Flash Configuration Data', 0);
            TestCase := 10000;
            goto ExitToTop;
            end;
          if not bUploadDone then
            begin
            goto ExitToTop;
            end;
          bUploadDone := false;
          if CDVerifyResults <> 0 then
            begin
            TestMessage(format('ERROR:  Invalid Flash Configuration Parameter: %d', [CDVerifyResults]), 0);
            TestCase := 10000;
            goto ExitToTop;
            end;

          if rgUploadFuzzyYN.ItemIndex = 1 then
            begin
            WaitTime := 20;
            TestCase := 450;
            goto ExitToTop;
            end
          else
            begin
            bUploadDone := false;
            WaitTime := 20;
            TestCase := 750;
            goto ExitToTop;
            end;
          end;

        450:
          begin
          EventLogger.WriteStr('Uploading Fuzzy Logic Configuration Data', True);
          TestMessage('Uploading Fuzzy Logic Configuration Data', 0);
          bUploadFuzzyConfiguration := true;
          bUploadDone := false;
          WaitTime := 2000;
          TestCase := 500;
          goto ExitToTop;
          end;

        500:
          begin
          if WaitTime = 0 then
            begin
            EventLogger.WriteStr('ERROR: Unable to Upload Fuzzy Logic Configuration Data', True);
            TestMessage('ERROR: Unable to Upload Fuzzy Logic Configuration Data', 0);
            TestCase := 10000;
            goto ExitToTop;
            end;
          if not bUploadDone then
            begin
            goto ExitToTop;
            end;
          bUploadDone := false;
          WaitTime := 20;
          TestCase := 750;
          goto ExitToTop;
          end;

        750:
          begin
          //
          // Now that the profile has been written, time to verify everything.
          //
          TestMessage('Verify Flash Config Data', 0);
          bDownloadFlashConfig := true;
          TestMessage('Download Flash Configuration Data', 0);
          WaitTime := 2000;
          TestCase := 800;
          end;

        800:
          begin
          if WaitTime = 0 then
            begin
            TestMessage('ERROR:  Unable to Download Flash Configuration Data', 0);
            TestCase := 10000;
            goto ExitToTop;
            end;
          if not bDownloadDone then
            begin
            goto ExitToTop;
            end;
          TestMessage('Download Done..', 0);
          TestCase := 900;
          goto ExitToTop;
          end;
        900:
          begin
          if not bDownloadDone then
            begin
            goto ExitToTop;
            end;
          bDownloadDone := false;
          CompareStatus := CompareMem(@CDProfileData.CurrentConfigData, @CDProfileData.GuiConfigData, sizeof(CDProfileData.GuiConfigData));
          if CompareStatus then
            begin
            TestMessage('Profile Data is Valid', 0);
            end
          else
            begin
            TestMessage('-->>PROFILE DATA IS INVALID!!!!', 0);
            end;
          TestCase := 10000;
          goto ExitToTop;
          end;

        //-----------------------------------------------------------------------------------------------------------

        10000:
          begin
          if ((SetpointData.EnableBits and ENABLE_POWER) <> 0) and not bPowerAlreadyOn then
            begin
            EventLogger.WriteStr('Turning off Power', True);
            TestMessage('Turning Off Power', 0);
            StatusBar1.Panels[0].Text := '';
            btnPowerClick(Sender);
            end;
          goto ExitNow;
          end;
        end;
      end;

    ExitNow:
    bEnableLogging := false;
    bVerifyProfile := false;
    bEndTest := True;
    TestMessage('End of Programming', 0);
    btnStart.Caption := 'Upload Profile';
    EventLogger.WriteStr('=================================================================================================', False);
    EventLogger.WriteStr('----------------------------------------  END TEST  ---------------------------------------------', False);
    EventLogger.WriteStr('=================================================================================================', False);
    EventLogger.WriteStr(' ', False);
    EventLogger.WriteStr(' ', False);
    EventLogger.Refresh;
end;

procedure TForm1.cbCDProfileNamesChange(Sender: TObject);
begin
  if bLoading then exit;
  SyncControls();
end;

//******************************************************************************************************************

procedure TForm1.cbCDProfileNamesCloseUp(Sender: TObject);
begin
  CDProfileData.LoadFromIniFile(cbCDProfileNames.Text);
  move(CDProfileData.IniConfigData, CDProfileData.GuiConfigData, sizeof(CDProfileData.GuiConfigData));
  UpdateGui();
  SyncControls();
end;

//******************************************************************************************************************

procedure TForm1.cbFPGAProfileNameChange(Sender: TObject);
var
   TmpStr: string;
begin
  TmpStr := cbFPGAProfileName.Text;
  FPGAProfileData.ParseCompleteProfileStr(TmpStr);
  UpdateFPGAProfileInformation(FPGAProfileData);
  FPGAProfileData.Redraw;
end;

//******************************************************************************************************************

procedure TForm1.cbManualChange(Sender: TObject);
begin
  if cbManual.Checked and ((SetpointData.EnableBits and ENABLE_POWER) = 0) then
    begin
    btnPowerClick(Sender);
    end;
  if not cbManual.Checked and ((SetpointData.EnableBits and ENABLE_POWER) <> 0) then
    begin
    btnPowerClick(Sender);
    end;
end;

procedure TForm1.cbManualFormChange(Sender: TObject);
begin
  if cbManualForm.Checked then
    begin
    if CD_70.BF4.RVOutputMode = 1 then
      begin
      Form2.cbManualSetpoint.Caption:='Setpoint (0.000 to 9.999 Volts)';
      Form2.tbSetpoint.Min := 0;
      Form2.tbSetpoint.Max := 9999;
      end;
    if CD_70.BF4.RVOutputMode = 2 then
      begin
      Form2.cbManualSetpoint.Caption:='Setpoint (1.0 to 999.9 Ohms)';
      Form2.tbSetpoint.Min := 10;
      Form2.tbSetpoint.Max := 9999;
      end;
    Form2.Show;

    end
  else
    Form2.Hide;
end;

//******************************************************************************************************************

procedure TForm1.UpdateProfileInformation(ProfileData: TProfileData);
begin
  if (ProfileData.EEPROM_Config and $01) <> 0 then rgProfileType.ItemIndex := 0 else rgProfileType.ItemIndex := 1;
  if (ProfileData.EEPROM_Config and $02) = 0 then rgKmaDirection.ItemIndex := 0 else rgKmaDirection.ItemIndex := 1;
  if (ProfileData.EEPROM_Config and $04) = 0 then
    begin
    cbTemperatureOutput.ItemIndex := 0;
    cbHoldMode.ItemIndex := 0;
    end
  else if (ProfileData.EEPROM_Config and $10) = 0 then
    begin
    if (ProfileData.EEPROM_Config and $08) = 0 then cbTemperatureOutput.ItemIndex := 1 else cbTemperatureOutput.ItemIndex := 2;
    cbHoldMode.ItemIndex := 0;
    end
  else
    begin
    if (ProfileData.EEPROM_Config and $08) = 0 then cbHoldMode.ItemIndex := 1 else cbHoldMode.ItemIndex := 2;
    cbTemperatureOutput.ItemIndex := 0;
    end;

  edtTripPoint.Text := format('%u', [ProfileData.Trippoint]);
  edtHoldValue.Text := format('%u', [ProfileData.HoldValue]);
  edtKmaSamples.Text := format('%u', [ProfileData.KmaSamples]);
  edtSetpointProfile.Text := ProfileData.ProfileStr;

end;

//******************************************************************************************************************

procedure TForm1.UpdateFPGAProfileInformation(ProfileData: TProfileData);
begin
  if (ProfileData.EEPROM_Config and $01) <> 0 then cbFPGAProfileType.ItemIndex := 0 else cbFPGAProfileType.ItemIndex := 1;
  if (ProfileData.EEPROM_Config and $02) = 0 then cbFPGAKmaDirection.ItemIndex := 0 else cbFPGAKmaDirection.ItemIndex := 1;
  if (ProfileData.EEPROM_Config and $04) = 0 then
    begin
    cbFPGATemperatureOutput.ItemIndex := 0;
    cbFPGAHoldMode.ItemIndex := 0;
    end
  else if (ProfileData.EEPROM_Config and $10) = 0 then
    begin
    if (ProfileData.EEPROM_Config and $08) = 0 then cbFPGATemperatureOutput.ItemIndex := 1 else cbFPGATemperatureOutput.ItemIndex := 2;
    cbHoldMode.ItemIndex := 0;
    end
  else
    begin
    if (ProfileData.EEPROM_Config and $08) = 0 then cbFPGAHoldMode.ItemIndex := 1 else cbFPGAHoldMode.ItemIndex := 2;
    cbTemperatureOutput.ItemIndex := 0;
    end;

  edtFPGATripPoint.Text := format('%u', [ProfileData.Trippoint]);
  edtFPGAHoldValue.Text := format('%u', [ProfileData.HoldValue]);
  edtFPGAKmaSamples.Text := format('%u', [ProfileData.KmaSamples]);
  edtFPGASetpointProfile.Text := ProfileData.ProfileStr;

end;

//******************************************************************************************************************
var
   bProfileChanging: boolean;

procedure TForm1.cbProfileNameCloseUp(Sender: TObject);
var
   TmpStr: string;
begin
  bProfileChanging := true;
  TmpStr := cbProfileName.Text;
  NewProfileData.ParseCompleteProfileStr(TmpStr);
  UpdateProfileInformation(NewProfileData);
  NewProfileData.Redraw;
  edtSetpointProfile.SetFocus;
  bProfileChanging := false;
end;

procedure TForm1.cbUploadCanUseMsgChange(Sender: TObject);
begin
  if bLoading then exit;
  SyncControls();
end;

procedure TForm1.cbUploadConfigDig1Change(Sender: TObject);
begin
  if (cbUploadConfigDig1.ItemIndex <> 8) and (cbUploadConfigDig2.ItemIndex = 7) then cbUploadConfigDig2.ItemIndex := 0;
  if (cbUploadConfigDig1.ItemIndex <> 9) and (cbUploadConfigDig2.ItemIndex = 8) then cbUploadConfigDig2.ItemIndex := 0;
  if (cbUploadConfigDig1.ItemIndex = 7) and (cbUploadConfigDig2.ItemIndex = 7) then cbUploadConfigDig2.ItemIndex := 0;
  if (cbUploadConfigDig1.ItemIndex = 7) and (cbUploadConfigDig2.ItemIndex = 8) then cbUploadConfigDig2.ItemIndex := 0;
  if (cbUploadConfigDig1.ItemIndex = 8) and (cbUploadConfigDig2.ItemIndex <> 7) then cbUploadConfigDig2.ItemIndex := 7;
  if (cbUploadConfigDig1.ItemIndex = 9) and (cbUploadConfigDig2.ItemIndex <> 8) then cbUploadConfigDig2.ItemIndex := 8;
  SyncControls();
end;

procedure TForm1.cbUploadConfigDig1CloseUp(Sender: TObject);
begin

end;

procedure TForm1.cbUploadConfigDig2Change(Sender: TObject);
begin
  if (cbUploadConfigDig2.ItemIndex <> 7) and (cbUploadConfigDig1.ItemIndex = 8) then cbUploadConfigDig1.ItemIndex := 0;
  if (cbUploadConfigDig2.ItemIndex <> 8) and (cbUploadConfigDig1.ItemIndex = 9) then cbUploadConfigDig1.ItemIndex := 0;
  if (cbUploadConfigDig2.ItemIndex = 7) and (cbUploadConfigDig1.ItemIndex <> 8) then cbUploadConfigDig1.ItemIndex := 8;
  if (cbUploadConfigDig2.ItemIndex = 8) and (cbUploadConfigDig1.ItemIndex <> 9) then cbUploadConfigDig1.ItemIndex := 9;
  SyncControls();
end;

procedure TForm1.cbUploadConfigDig6Change(Sender: TObject);
begin

end;

//**************************************************************************************************************************************

procedure TForm1.cbUploadConfigErrorRVChange(Sender: TObject);
begin
  if bLoading then exit;
  SyncControls();
end;

//**************************************************************************************************************************************

procedure TForm1.cbUploadConfigMasterErrorChange(Sender: TObject);
begin
  if bLoading then exit;
  SyncControls();
end;

//**************************************************************************************************************************************

procedure TForm1.cbUploadConfigMasterModeChange(Sender: TObject);
begin
  if bLoading then exit;
  SyncControls();
end;

//**************************************************************************************************************************************

procedure TForm1.cbUploadConfigOutputModeChange(Sender: TObject);
begin
  if bLoading then exit;
  SyncControls();
end;

//**************************************************************************************************************************************

procedure TForm1.cbUploadConfigRS485FormatChange(Sender: TObject);
begin
  if bLoading then exit;
  SyncControls();
end;

procedure TForm1.changeMfShape(Sender: TObject);
begin
  ShowHideMF();
end;

procedure TForm1.DrawGrid1Click(Sender: TObject);
begin

end;

procedure TForm1.edtFuzzy1RemoveRuleClick(Sender: TObject);
begin

end;

procedure TForm1.edtFuzzy2Input0Mf0ShapeChange(Sender: TObject);
begin

end;

procedure TForm1.edtFuzzy2Input1Mf2Pt0Change(Sender: TObject);
begin

end;

//**************************************************************************************************************************************

procedure TForm1.SyncControls;
var
   ParseStatus, i, j: integer;
   FirstValue: single;
   ParseResults: stParseResults;
   FieldResults: stFieldResults;
begin

  LoadFromGui;

  ShowHideCanMsgOptions;

  btnUploadProfile.Enabled := cbCDProfileNames.Text <> '';



  if cbUploadConfigRS485Format.ItemIndex = 0 then
    begin
    edtUploadConfigRS485Time.Visible := false;
    edtUploadConfigRS485Time.Text := '0.0' ;
    Label236.Visible := false;
    end;

  if cbUploadConfigRS485Format.ItemIndex > 0 then
    begin
    edtUploadConfigRS485Time.Visible := true;
    Label236.Visible := true;
    end;

  if cbUploadConfigOutputMode.ItemIndex = 0 then
    begin
    cbUploadConfigPullUpMode.ItemIndex := 0;
    cbUploadConfigPullUpMode.Visible := false;
    Label224.Visible := false;
    shtRVProfile2.TabVisible := false;
    end;

  if cbUploadConfigOutputMode.ItemIndex = 1 then
    begin
    cbUploadConfigPullUpMode.Visible := true;
    Label224.Visible := true;
    shtRVProfile2.TabVisible := true;
    end;

  if cbUploadConfigOutputMode.ItemIndex = 2 then
    begin
    CDProfileData.GuiConfigData.BF1.RVOutputMode := 2;
    cbUploadConfigPullUpMode.Visible := true;
    Label224.Visible := true;
    shtRVProfile2.TabVisible := true;
    end;

  if cbUploadConfigMasterMode.ItemIndex = 0 then
    begin
    shtMasterFreqProfile2.TabVisible := true;
    end;

  if cbUploadConfigMasterMode.ItemIndex = 1 then
    begin
    shtMasterFreqProfile2.TabVisible := false;
    end;
end;

//**************************************************************************************************************************************

procedure TForm1.edtSetpointProfileChange(Sender: TObject);
begin
  if bProfileChanging then exit;
  bProfileChanging := true;
  UpdateProfileData(NewProfileData);
  NewProfileData.Redraw;
  bProfileChanging := false;
end;

procedure TForm1.edtUploadConfigCanBusChange(Sender: TObject);
begin
  if bLoading then exit;
  SyncControls();
end;

//**************************************************************************************************************************************

procedure TForm1.edtUploadConfigMasterSetpointChange(Sender: TObject);
begin
  if bLoading then exit;
  SyncControls();
end;

//**************************************************************************************************************************************

procedure TForm1.edtUploadConfigRVsetpointChange(Sender: TObject);
begin
  if bLoading then exit;
  SyncControls();
end;

procedure TForm1.edtConfigManufacturingTypeChange(Sender: TObject);
begin

end;

procedure TForm1.edtUploadConfigSensorALowChange(Sender: TObject);
begin

end;

//**************************************************************************************************************************************

procedure TForm1.btnDown1Click(Sender: TObject);
var
  Digit, StrIndex: integer;
  Freq, TmpFloat: single;
begin
  if cbCascade.Checked then
    begin
    AdjustFrequency(-0.01);
    end
  else
    begin
    StrIndex := 1;
    TmpFloat := 0;
    MyStrToFloat(edtGenDigit1.Text, StrIndex, TmpFloat);
    Digit := round(TmpFloat);
    if Digit = 0 then Digit := 9 else dec(Digit);
    edtGenDigit1.Text := format('%u', [Digit]);
    end;
  Freq := GeneratorFrequency();
  edtDigFreqOut6.Text:=format('%1.2f', [Freq]);
end;

//**************************************************************************************************************************************

procedure TForm1.btnDownloadCurrentProfileClick(Sender: TObject);
var
  TestCase: integer;
  bPowerAlreadyOn: boolean;
label
  ExitNow, ExitToTop;
begin
    TestCase := 0;
    bEndTest := False;
    bPowerAlreadyOn := false;
    WaitTime := 0;
    while (not bEndTest) do
      begin
  ExitToTop:
      if WaitTime <> 0 then
        WaitTime := WaitTime - 1;
      Sleep(1);
      Application.ProcessMessages;

      // This Case statement controls each test we are running

      case (TestCase) of
          0:
            begin
            if CD_70_UpdateCounter <> 0 then       // Power On and communicating
              begin
              bPowerAlreadyOn := true;
              WaitTime := 10;
              TestCase := 300;
              end
            else
              begin
              TestMessage('Turning On Power', 0);
              PowerControl(TRUE);
              WaitTime := 2000;
              TestCase := 200;
              end;
            end;

          200:
            begin
            //
            // Once we see DUT_UpdateCounter change at least 10 times, we can safely assume that the Programmer is communicating with
            // Sender, therefore reduce WaitTime
            //
            if (WaitTime > 20) and (CD_70_UpdateCounter <> 0) then
              WaitTime := 20;
            //
            // If ENABLE_POWER bit resets, then excess current has been detected and power was automatically turned off, therefore abort test
            //
            if (SetpointData.EnableBits and ENABLE_POWER) = 0 then
              begin
              TestMessage('ERROR: Excess Current Detected', 0);
              TestCase := 10000;
              goto ExitToTop;
              end;
            //
            // Wait for WaitTime to reach zero
            //
            if WaitTime <> 0 then
              goto ExitToTop;
            //
            // If DUT_UpdateCounter hasn't changed, then we aren't communicating with Sender, therefore abort test.
            //
            if (CD_70_UpdateCounter = 0) then
              begin
              TestMessage('ERROR: Unable to Communicate with Sender', 0);
              TestCase := 10000;
              goto ExitToTop;
              end;

            WaitTime := 50;
            TestCase := 300;
            goto ExitToTop;
            end;

          300:
            begin
            if WaitTime <> 0 then
              goto ExitToTop;
            //
            // Write Flash Configuration Data to Sender
            //
            TestMessage('Download Flash Configuration Data', 0);
            bDownloadDone := false;
            bDownloadFlashConfig := true;
            WaitTime := 2000;
            TestCase := 400;
            goto ExitToTop;
            end;

          400:
            begin
            if WaitTime = 0 then
              begin
              TestMessage('ERROR:  Unable to Download Flash Configuration Data', 0);
              TestCase := 10000;
              goto ExitToTop;
              end;
            if not bDownloadDone then
              begin
              goto ExitToTop;
              end;
            TestMessage('Download Done..', 0);
            TestMessage('Download Fuzzy Logic Configuration Data', 0);
            bDownloadDone := false;
            move(CDProfileData.CurrentConfigData, CDProfileData.GuiConfigData, sizeof(CDProfileData.GuiConfigData));
            UpdateGui();
            SyncControls();
            bDownloadFuzzyConfig := true;
            WaitTime := 2000;
            TestCase := 500;
            goto ExitToTop;
            end;

          500:
            begin
            if WaitTime = 0 then
              begin
              TestMessage('ERROR:  Unable to Download Fuzzy Configuration Data', 0);
              TestCase := 10000;
              goto ExitToTop;
              end;
            if not bDownloadDone then
              begin
              goto ExitToTop;
              end;
            TestMessage('Download Done..', 0);
            bDownloadDone := false;
            TestCase := 10000;
            move(CDProfileData.CurrentConfigData, CDProfileData.GuiConfigData, sizeof(CDProfileData.GuiConfigData));
            UpdateGui();
            SyncControls();
            goto ExitToTop;
            end;

          10000:
            begin
            if ((SetpointData.EnableBits and ENABLE_POWER) <> 0) and not bPowerAlreadyOn then
              begin
              TestMessage('Turning Off Power', 0);
              StatusBar1.Panels[0].Text := '';
              btnPowerClick(Sender);
              end;
            goto ExitNow;
            end;
          end;
        end;

      ExitNow:
end;
//**************************************************************************************************************************************
// Creates profiles for Sensor A and Sensor B, assumes that local sensor is Sensor A
procedure TForm1.btnCreateOutputProfileClick(Sender: TObject);
begin
  Form3.InitializeForm();
  Form3.Show;

end;

//**************************************************************************************************************************************

procedure TForm1.btnDown2Click(Sender: TObject);
var
  Digit, StrIndex: integer;
  Freq, TmpFloat: single;
begin
  if cbCascade.Checked then
    begin
    AdjustFrequency(-0.1);
    end
  else
    begin
    StrIndex := 1;
    TmpFloat := 0;
    MyStrToFloat(edtGenDigit2.Text, StrIndex, TmpFloat);
    Digit := round(TmpFloat);
    if Digit = 0 then Digit := 9 else dec(Digit);
    edtGenDigit2.Text := format('%u', [Digit]);
    end;
  Freq := GeneratorFrequency();
  edtDigFreqOut6.Text:=format('%1.2f', [Freq]);
end;

//**************************************************************************************************************************************

procedure TForm1.btnDown3Click(Sender: TObject);
var
  Digit, StrIndex: integer;
  Freq, TmpFloat: single;
begin
  if cbCascade.Checked then
    begin
    AdjustFrequency(-1.0);
    end
  else
    begin
    StrIndex := 1;
    TmpFloat := 0;
    MyStrToFloat(edtGenDigit3.Text, StrIndex, TmpFloat);
    Digit := round(TmpFloat);
    if Digit = 0 then Digit := 9 else dec(Digit);
    edtGenDigit3.Text := format('%u', [Digit]);
    end;
  Freq := GeneratorFrequency();
  edtDigFreqOut6.Text:=format('%1.2f', [Freq]);
end;

//**************************************************************************************************************************************

procedure TForm1.btnDown4Click(Sender: TObject);
var
  Digit, StrIndex: integer;
  Freq, TmpFloat: single;
begin
  if cbCascade.Checked then
    begin
    AdjustFrequency(-10.0);
    end
  else
    begin
    StrIndex := 1;
    TmpFloat := 0;
    MyStrToFloat(edtGenDigit4.Text, StrIndex, TmpFloat);
    Digit := round(TmpFloat);
    if Digit = 0 then Digit := 9 else dec(Digit);
    edtGenDigit4.Text := format('%u', [Digit]);
    end;
  Freq := GeneratorFrequency();
  edtDigFreqOut6.Text:=format('%1.2f', [Freq]);
end;

//**************************************************************************************************************************************

procedure TForm1.btnDown5Click(Sender: TObject);
var
  Digit, StrIndex: integer;
  Freq, TmpFloat: single;
begin
  if cbCascade.Checked then
    begin
    AdjustFrequency(-100.0);
    end
  else
    begin
    StrIndex := 1;
    TmpFloat := 0;
    MyStrToFloat(edtGenDigit5.Text, StrIndex, TmpFloat);
    Digit := round(TmpFloat);
    if Digit = 0 then Digit := 9 else dec(Digit);
    edtGenDigit5.Text := format('%u', [Digit]);
    end;
  Freq := GeneratorFrequency();
  edtDigFreqOut6.Text:=format('%1.2f', [Freq]);
end;

//**************************************************************************************************************************************

procedure TForm1.btnDown6Click(Sender: TObject);
var
  Digit, StrIndex: integer;
  Freq, TmpFloat: single;
begin
  if cbCascade.Checked then
    begin
    AdjustFrequency(-1000.0);
    end
  else
    begin
    StrIndex := 1;
    TmpFloat := 0;
    MyStrToFloat(edtGenDigit6.Text, StrIndex, TmpFloat);
    Digit := round(TmpFloat);
    if Digit = 0 then Digit := 9 else dec(Digit);
    edtGenDigit6.Text := format('%u', [Digit]);
    end;
  Freq := GeneratorFrequency();
  edtDigFreqOut6.Text:=format('%1.2f', [Freq]);
end;

//**************************************************************************************************************************************

procedure TForm1.btnDown7Click(Sender: TObject);
var
  Digit, StrIndex: integer;
  Freq, TmpFloat: single;
begin
  if cbCascade.Checked then
    begin
    AdjustFrequency(-10000.0);
    end
  else
    begin
    StrIndex := 1;
    TmpFloat := 0;
    MyStrToFloat(edtGenDigit7.Text, StrIndex, TmpFloat);
    Digit := round(TmpFloat);
    if Digit = 0 then Digit := 9 else dec(Digit);
    edtGenDigit7.Text := format('%u', [Digit]);
    end;
  Freq := GeneratorFrequency();
  edtDigFreqOut6.Text:=format('%1.2f', [Freq]);
end;

//**************************************************************************************************************************************

procedure TForm1.btnGenOffClick(Sender: TObject);
begin
  cbDigMode6.ItemIndex := 0;
end;

//**************************************************************************************************************************************

Function TForm1.GeneratorFrequency : single;
var
   StrIndex: integer;
   TmpFloat: single;
begin
  Result := 0;

  StrIndex := 1;
  TmpFloat := 0;
  MyStrToFloat(edtGenDigit7.Text, StrIndex, TmpFloat);
  Result := Result + (TmpFloat * 10000.0);

  StrIndex := 1;
  MyStrToFloat(edtGenDigit6.Text, StrIndex, TmpFloat);
  Result := Result + (TmpFloat * 1000.0);

  StrIndex := 1;
  MyStrToFloat(edtGenDigit5.Text, StrIndex, TmpFloat);
  Result := Result + (TmpFloat * 100.0);

  StrIndex := 1;
  MyStrToFloat(edtGenDigit4.Text, StrIndex, TmpFloat);
  Result := Result + (TmpFloat * 10.0);

  StrIndex := 1;
  MyStrToFloat(edtGenDigit3.Text, StrIndex, TmpFloat);
  Result := Result + (TmpFloat * 1.0);

  StrIndex := 1;
  MyStrToFloat(edtGenDigit2.Text, StrIndex, TmpFloat);
  Result := Result + (TmpFloat * 0.1);

  StrIndex := 1;
  MyStrToFloat(edtGenDigit1.Text, StrIndex, TmpFloat);
  Result := Result + (TmpFloat * 0.01);

end;

//**************************************************************************************************************************************

procedure TForm1.AdjustFrequency (AdjAmount: single);
var
   Freq: single;
   TmpStr: string;
begin
  Freq := GeneratorFrequency() + AdjAmount;
  if Freq < 1.0 then Freq := 1.0;
  if Freq > 29999.99 then Freq := 29999.99;
  TmpStr := format('%8.2f', [Freq]);
  TmpStr := stringreplace(TmpStr, ' ', '0', [rfReplaceAll, rfIgnoreCase]);
  edtGenDigit1.Text := TmpStr[8];
  edtGenDigit2.Text := TmpStr[7];
  edtGenDigit3.Text := TmpStr[5];
  edtGenDigit4.Text := TmpStr[4];
  edtGenDigit5.Text := TmpStr[3];
  edtGenDigit6.Text := TmpStr[2];
  edtGenDigit7.Text := TmpStr[1];
end;


//**************************************************************************************************************************************

procedure TForm1.BtnGenOnClick(Sender: TObject);
var
   Freq: single;
begin
  cbDigMode6.ItemIndex := 2;
  Freq := GeneratorFrequency();
  edtDigFreqOut6.Text:=format('%1.2f', [Freq]);
end;

//**************************************************************************************************************************************

procedure TForm1.imgIconDblClick(Sender: TObject);
begin

  if rgMode.ItemIndex = 0 then
    begin
    TabSheet1.TabVisible := true;
    TabSheet4.TabVisible := true;
    TabSheet5.TabVisible := true;
    TabSheet8.TabVisible := false;
    TabSheet10.TabVisible := false;
    TabSheet3.TabVisible := false;
    end
  else
    begin
    TabSheet8.TabVisible := true;
    TabSheet10.TabVisible := true;
    TabSheet1.TabVisible := false;
    TabSheet4.TabVisible := false;
    TabSheet5.TabVisible := false;
    TabSheet3.TabVisible := true;
    end;

    TabSheet2.TabVisible := true;
    TabSheet7.TabVisible := true;

  Label1.Visible := true;
  Label2.Visible := true;
  edtNameofTester.Visible := true;
  edtSerialNumber.Visible := true;
end;

procedure TForm1.Label124Click(Sender: TObject);
begin

end;

procedure TForm1.Label172Click(Sender: TObject);
begin

end;

procedure TForm1.Label17Click(Sender: TObject);
begin

end;

procedure TForm1.Label18Click(Sender: TObject);
begin

end;

procedure TForm1.Label309Click(Sender: TObject);
begin

end;

procedure TForm1.PageControl1Change(Sender: TObject);
begin

end;

procedure TForm1.PageControl6Change(Sender: TObject);
begin

end;

//**************************************************************************************************************************************

procedure TForm1.rgModeClick(Sender: TObject);
begin
  if rgMode.ItemIndex = 0 then
    begin
    TabSheet1.TabVisible := true;
    TabSheet4.TabVisible := false;
    TabSheet5.TabVisible := true;
    TabSheet8.TabVisible := false;
    TabSheet10.TabVisible := false;
    TabSheet3.TabVisible := false;;
    SetpointData.CommConfig := COMM_CC;
    PageControl1.TabIndex := 0;
    end;
  if rgMode.ItemIndex = 1 then
    begin
    TabSheet1.TabVisible := false;
    TabSheet4.TabVisible := false;
    TabSheet5.TabVisible := false;
    TabSheet8.TabVisible := true;
    TabSheet10.TabVisible := false;
    TabSheet3.TabVisible := true;
    ShowHideMF();
    SetpointData.CommConfig := COMM_CD_DIGIO1;
    PageControl1.TabIndex := 7;
    end;
  if rgMode.ItemIndex = 2 then
    begin
    TabSheet1.TabVisible := false;
    TabSheet4.TabVisible := false;
    TabSheet5.TabVisible := false;
    TabSheet8.TabVisible := true;
    TabSheet10.TabVisible := false;
    TabSheet3.TabVisible := true;
    ShowHideMF();
    SetpointData.CommConfig := COMM_CD_RS485;
    PageControl1.TabIndex := 7;
    end;
  if rgMode.ItemIndex = 3 then
    begin
    TabSheet1.TabVisible := false;
    TabSheet4.TabVisible := false;
    TabSheet5.TabVisible := false;
    TabSheet8.TabVisible := true;
    TabSheet10.TabVisible := false;
    TabSheet3.TabVisible := true;
    ShowHideMF();
    SetpointData.CommConfig := COMM_CD_RS485_DIRECT;
    PageControl1.TabIndex := 7;
    end;

end;

procedure TForm1.rgUploadConfigSensorModeClick(Sender: TObject);
begin

end;

procedure TForm1.TabSheet14ContextPopup(Sender: TObject; MousePos: TPoint;
  var Handled: Boolean);
begin

end;

procedure TForm1.TabSheetFuzzy1MFContextPopup(Sender: TObject;
  MousePos: TPoint; var Handled: Boolean);
begin

end;

procedure TForm1.TabSheetFuzzy1RulesContextPopup(Sender: TObject;
  MousePos: TPoint; var Handled: Boolean);
begin

end;

procedure TForm1.TabSheetFuzzyChange(Sender: TObject);
begin

end;

//******************************************************************************************************
//******************************************************************************************************

procedure TForm1.UpdateProfile(ProfileStr: string; ScaleFactor: single; ErrorLabel: Tlabel; ProfileGraph: TProfileGraph; var NewTable: stTwoPointTable);
var
  CI, Status, i: integer;
  MaxSetpoint, MinSetpoint, SP: single;
  ParseResults: stParseResults;
begin
  for i := 0 to 31 do
    begin
    NewTable.TablePoint[i].Point1 := 0;
    NewTable.TablePoint[i].Point2 := 0;
    end;
  Status := ParseRecords(ProfileStr, ParseResults);
  if ParseResults.ParseLength > 32 then ParseResults.ParseLength := 32;
  for i := 0 to ParseResults.ParseLength - 1 do
    begin
    NewTable.TablePoint[i].Point1 := round(ParseResults.ParsePoint[i].Point1 * 100.0);
    NewTable.TablePoint[i].Point2 := round(ParseResults.ParsePoint[i].Point2 * ScaleFactor);
    end;
  NewTable.TableLength := ParseResults.ParseLength;

  if (Status = 0) then
    begin
    if NewTable.TableLength < 2 then
      Status := -15;
    if (Status = 0) and (NewTable.TablePoint[0].Point1 <> 400) then
      Status := -10;
    if (Status = 0) and (NewTable.TablePoint[NewTable.TableLength - 1].Point1 <> 102300) then
      Status := -11;
    if (Status = 0) then
      begin
      CI := -1;
      for i := 0 to NewTable.TableLength - 1 do
        begin
        if (NewTable.TablePoint[i].Point1 <= CI) then
          begin
          Status := -14;
          break;
          end;
        CI := NewTable.TablePoint[i].Point1;
        SP := NewTable.TablePoint[i].Point2 / ScaleFactor;
        if SP < 0.0 then
          begin
          Status := -12;
          break;
          end;
        ProfileGraph.AddPoint(NewTable.TablePoint[i].Point1 div 10, SP);
        end;
      end;
    end;

  if Status <> 0 then
    begin
    if Status = -1 then
      ErrorLabel.Caption := 'Error: Missing Left Bracket';
    if Status = -2 then
      ErrorLabel.Caption := 'Error: Missing Right Bracket';
    if Status = -3 then
      ErrorLabel.Caption := 'Error: Missing Comma';
    if Status = -4 then
      ErrorLabel.Caption := 'Error: Too many Pairs, Maximum is 63';
    if Status = -5 then
      ErrorLabel.Caption := 'Error: Missing Index Value';
    if Status = -6 then
      ErrorLabel.Caption := 'Error: Missing Setpoint Value';
    if Status = -15 then
      ErrorLabel.Caption := 'Error: Too few Pairs, Minimum is 2';
    if Status = -10 then
      ErrorLabel.Caption := 'Error: Index of first pair MUST be 4';
    if Status = -11 then
      ErrorLabel.Caption := 'Error: Index of last pair MUST be 1023';
    if Status = -12 then
      ErrorLabel.Caption := 'Error: Setpoint must be greater than 0.0';
    if Status = -14 then
      ErrorLabel.Caption := 'Error: Each Index must be bigger than the previous Index';
    ProfileGraph.GraphData.NextIndex := 0;
    end
  else
    begin
    ProfileGraph.GraphData.NextIndex := 0;
    MaxSetpoint := NewTable.TablePoint[0].Point2;
    MinSetpoint := NewTable.TablePoint[0].Point2;
    for i := 0 to NewTable.TableLength - 1 do
      begin
      ProfileGraph.AddPoint(NewTable.TablePoint[i].Point1 div 100, NewTable.TablePoint[i].Point2 / ScaleFactor);
      if NewTable.TablePoint[i].Point2 > MaxSetpoint then
        MaxSetpoint := NewTable.TablePoint[i].Point2;
      if NewTable.TablePoint[i].Point2 < MinSetpoint then
        MinSetpoint := NewTable.TablePoint[i].Point2;
      end;
    ProfileGraph.HorzHighScale := ((MaxSetpoint * 1.05) / ScaleFactor);
    ProfileGraph.HorzLowScale := ((MinSetpoint * 0.95) / ScaleFactor);
    ErrorLabel.Caption := '';
    end;
  ProfileGraph.Redraw;
end;


//******************************************************************************************************************

procedure TProfileGraph.AddPoint(X: single; Y: single);
begin
  GraphData.Y[GraphData.NextIndex] := Y;
  GraphData.X[GraphData.NextIndex] := X;
  GraphData.NextIndex := GraphData.NextIndex + 1;
end;

//******************************************************************************************************************

procedure TProfileGraph.Redraw;
var
  X, Y, i, Yoffset, Xoffset: integer;
  XRange, YRange: single;
  R: TRect;
  TmpStr: string;
begin
  BM := TBitMap.Create;
  BM.Width := GraphImage.Width;
  BM.Height := GraphImage.Height;

  // Clear Canvas

  R.Top := 0;
  R.Left := 0;
  R.Right := BM.Width;
  R.Bottom := BM.Height;
  BM.Canvas.Brush.Color := clBlack;
  BM.Canvas.FillRect(R);
  BM.Canvas.Pen.Color := clWhite;

  // Initial Position of Graphing Area

  BorderRect.Top := 10;
  BorderRect.Left := 10;
  BorderRect.Right := BM.Width - 15;
  BorderRect.Bottom := BM.Height - 40;

  // Draw scales

  BM.Canvas.Font.Color := clYellow;
  for i := 0 to HorzHashMarks do
    begin
    TmpStr := format('%5.*f', [HorzPrecision, (((HorzHighScale - HorzLowScale) * i) / HorzHashMarks) + HorzLowScale]);
    Yoffset := BorderRect.Bottom - (((BorderRect.Bottom - BorderRect.Top - 2) * i) div HorzHashMarks) - 1;
    XOffset := BM.Canvas.TextWidth(TmpStr);
    BM.Canvas.TextOut(80 - 3 - Xoffset, Yoffset - 6, TmpStr);
    end;
  if HorzTitle <> '' then
    begin
    BM.Canvas.Font.Orientation := 900;
    XOffset := BM.Canvas.TextWidth(HorzTitle);
    BM.Canvas.TextOut(5, (BorderRect.Bottom + BorderRect.Top + Xoffset) div 2, HorzTitle);
    BM.Canvas.Font.Orientation := 0;
    end;

  // Final Position of Graphing Area

  BorderRect.Top := 10;
  BorderRect.Left := 80;
  BorderRect.Right := BM.Width - 15;
  BorderRect.Bottom := BM.Height - 40;

  BM.Canvas.MoveTo(BorderRect.Left, BorderRect.Top);
  BM.Canvas.LineTo(BorderRect.Right, BorderRect.Top);
  BM.Canvas.LineTo(BorderRect.Right, BorderRect.Bottom);
  BM.Canvas.LineTo(BorderRect.Left, BorderRect.Bottom);
  BM.Canvas.LineTo(BorderRect.Left, BorderRect.Top);

  for i := 0 to HorzHashMarks do
    begin
    Yoffset := BorderRect.Bottom - (((BorderRect.Bottom - BorderRect.Top - 2) * i) div HorzHashMarks) - 1;
    BM.Canvas.MoveTo(BorderRect.Left - 5, Yoffset);
    BM.Canvas.LineTo(BorderRect.Left, Yoffset);
    if (i > 0) and (i < HorzHashMarks) then
      begin
      BM.Canvas.Pen.Style := psDot;
      BM.Canvas.LineTo(BorderRect.Right, Yoffset);
      BM.Canvas.Pen.Style := psSolid;
      end;
    end;

  for i := 0 to VertHashMarks do
    begin
    TmpStr := format('%4.*f', [VertPrecision, (((VertHighScale - VertLowScale) * i) / VertHashMarks) + VertLowScale]);
    Xoffset := BorderRect.Left + (((BorderRect.Right - BorderRect.Left - 2) * i) div VertHashMarks) + 1;
    YOffset := BM.Canvas.TextWidth(TmpStr);
    BM.Canvas.Font.Color := clWhite;
    BM.Canvas.TextOut(Xoffset - (Yoffset div 2), BorderRect.Bottom + 6, TmpStr);
    BM.Canvas.MoveTo(Xoffset, BorderRect.Bottom + 5);
    BM.Canvas.LineTo(Xoffset, BorderRect.Bottom);
    if (i > 0) and (i < VertHashMarks) then
      begin
      BM.Canvas.Pen.Style := psDot;
      BM.Canvas.LineTo(Xoffset, BorderRect.Top);
      BM.Canvas.Pen.Style := psSolid;
      end;
    end;
  XOffset := BM.Canvas.TextWidth(VertTitle);
  BM.Canvas.TextOut((BorderRect.Right + BorderRect.Left - Xoffset) div 2, BorderRect.Bottom + 20, VertTitle);

  XRange := VertHighScale - VertLowScale;
  if XRange = 0 then XRange := 1;
  X := BorderRect.Right - BorderRect.Left;
  YRange := HorzHighScale - HorzLowScale;
  if (YRange = 0) then
    YRange := 1;
  Y := BorderRect.Bottom - BorderRect.Top;

  BM.Canvas.Pen.Color := clYellow;
  for i := 0 to GraphData.NextIndex - 1 do
    begin
    Xoffset := BorderRect.Left + round(((GraphData.X[i] - VertLowScale) / XRange) * X);
    Yoffset := BorderRect.Bottom - round(((GraphData.Y[i] - HorzLowScale) / YRange) * Y);
    if i = 0 then
      begin
      BM.Canvas.MoveTo(Xoffset, Yoffset);
      end
    else
      begin
      BM.Canvas.LineTo(Xoffset, Yoffset);
      end;
    end;
  BM.Canvas.Pen.Width := 1;

  GraphImage.Picture.Graphic := BM;
  BM.Free;
end;

//-------------------------------------------------------------------------------------

type
  PTwoPointTable = ^stTwoPointTable;
  POnePointTable = ^stOnePointTable;
  PArray16x16 = ^stArray16x16;
  PBool = ^boolean;


//-------------------------------------------------------------------------------------

procedure SetIniData (IniFile: TIniFile; ProfileName: string; IniName: string; DataType: integer; DataPointer: Pointer; ScaleFactor: single);
var
  IntPtr: PInteger;
  BooleanPtr: PBool;
  j, i: integer;
  TmpInt: single;
  TwoPointTablePtr: PTwoPointTable;
  OnePointTablePtr: POnePointTable;
  Array16x16Ptr: PArray16x16;
  ParseResults: stParseResults;
begin
  if DataType = 0 then
    begin
    TmpInt := IniFile.ReadFloat(ProfileName, IniName, 0.0);
    IntPtr := DataPointer;
    IntPtr^ := round(TmpInt * ScaleFactor);
    end;
  if DataType = 1 then
    begin
    TmpInt := IniFile.ReadFloat(ProfileName, IniName, 0.0);
    IntPtr := DataPointer;
//    ScaleFactor := 1.0;
    IntPtr^ := round(TmpInt * 1000.0);
    end;
  if DataType = 2 then
    begin
    TwoPointTablePtr := DataPointer;
    ParseRecords(IniFile.ReadString(ProfileName, IniName, ''), ParseResults);
    TwoPointTablePtr^.TableLength := ParseResults.ParseLength;
    for i := 0 to 31 do
      begin
      TwoPointTablePtr^.TablePoint[i].Point1 := 0;
      TwoPointTablePtr^.TablePoint[i].Point2 := 0;
      if ParseResults.ParseLength > i then
        begin
        TwoPointTablePtr^.TablePoint[i].Point1 := round(ParseResults.ParsePoint[i].Point1 * 100.0);
        TwoPointTablePtr^.TablePoint[i].Point2 := round(ParseResults.ParsePoint[i].Point2 * 1000.0);
        end;
      end;
    end;
  if DataType = 3 then
    begin
    TwoPointTablePtr := DataPointer;
    ParseRecords(IniFile.ReadString(ProfileName, IniName, ''), ParseResults);
    TwoPointTablePtr^.TableLength := ParseResults.ParseLength;
    for i := 0 to 31 do
      begin
      TwoPointTablePtr^.TablePoint[i].Point1 := 0;
      TwoPointTablePtr^.TablePoint[i].Point2 := 0;
      if ParseResults.ParseLength > i then
        begin
        TwoPointTablePtr^.TablePoint[i].Point1 := round(ParseResults.ParsePoint[i].Point1 * 100.0);
        TwoPointTablePtr^.TablePoint[i].Point2 := round(ParseResults.ParsePoint[i].Point2 * 100.0);
        end;
      end;
    end;
  if DataType = 4 then
    begin
    TwoPointTablePtr := DataPointer;
    ParseRecords(IniFile.ReadString(ProfileName, IniName, ''), ParseResults);
    TwoPointTablePtr^.TableLength := ParseResults.ParseLength;
    for i := 0 to 31 do
      begin
      TwoPointTablePtr^.TablePoint[i].Point1 := 0;
      TwoPointTablePtr^.TablePoint[i].Point2 := 0;
      if ParseResults.ParseLength > i then
        begin
        TwoPointTablePtr^.TablePoint[i].Point1 := round(ParseResults.ParsePoint[i].Point1 * 10000.0);
        TwoPointTablePtr^.TablePoint[i].Point2 := round(ParseResults.ParsePoint[i].Point2 * 100.0);
        end;
      end;
    end;
  if DataType = 5 then
    begin
    OnePointTablePtr := DataPointer;
    ParseFields(IniFile.ReadString(ProfileName, IniName, ''), FieldResults);
    OnePointTablePtr^.TableLength := FieldResults.FieldCount;
    for i := 0 to 31 do
      begin
      OnePointTablePtr^.TablePoint[i].Point1 := 0;
      if FieldResults.FieldCount > i then
        begin
        OnePointTablePtr^.TablePoint[i].Point1 := round(FieldResults.FieldValue[i] * ScaleFactor);
        end;
      end;
    end;
  if DataType = 6 then
    begin
    Array16x16Ptr := DataPointer;
    for i := 0 to 15 do
      begin
      ParseFields(IniFile.ReadString(ProfileName, format('%s_%d', [IniName, i]), ''), FieldResults);
      for j := 0 to 15 do
        begin
        Array16x16Ptr^[i, j] := round(FieldResults.FieldValue[j] * ScaleFactor);
        end;
      end;
    end;
  if DataType = 7 then
    begin
    TmpInt := IniFile.ReadFloat(ProfileName, IniName, 0.0);
    BooleanPtr := DataPointer;
    BooleanPtr^ := TmpInt > 0;
    end;
  if DataType = 8 then
    begin
    TwoPointTablePtr := DataPointer;
    ParseRecords(IniFile.ReadString(ProfileName, IniName, ''), ParseResults);
    TwoPointTablePtr^.TableLength := ParseResults.ParseLength;
    for i := 0 to 31 do
      begin
      TwoPointTablePtr^.TablePoint[i].Point1 := 0;
      TwoPointTablePtr^.TablePoint[i].Point2 := 0;
      if ParseResults.ParseLength > i then
        begin
        TwoPointTablePtr^.TablePoint[i].Point1 := round(ParseResults.ParsePoint[i].Point1 * 100.0);
        TwoPointTablePtr^.TablePoint[i].Point2 := round(ParseResults.ParsePoint[i].Point2 * 1.0);
        end;
      end;
    end;


end;

//-------------------------------------------------------------------------------------

type
  PFuzzyInputMFs = ^stFuzzyInputMFs;
  PFuzzyOutputMFs = ^stFuzzyOutputMFs;
  PFuzzyInputRules = ^stFuzzyInputRules;
  PFuzzyOutputRules = ^stFuzzyOutputRules;
  PFuzzyOutputInfo = ^stFuzzyOutputInfo;
  PFuzzyRuleInfo = ^stFuzzyRuleInfo;


//-------------------------------------------------------------------------------------

Procedure SetFuzzyIniData(IniFile: TIniFile; ProfileName: string; IniName: string; DataType: integer; DataPointer1: Pointer; DataPointer2: Pointer; ScaleFactor: single);
var
  IntPtr: PInteger;
  BooleanPtr: PBool;
  j, i: integer;
  TmpInt: single;
  FuzzyInputMFsPtr: PFuzzyInputMFs;
  FuzzyOutputMFsPtr: PFuzzyOutputMFs;
  FuzzyInputRulesPtr: PFuzzyInputRules;
  FuzzyOutputRulesPtr: PFuzzyOutputRules;
  FuzzyOutputInfoPtr: PFuzzyOutputInfo;
  FuzzyRuleInfoPtr: PFuzzyRuleInfo;

  ParseResults: stParseResults;
begin
    if DataType = 0 then                               // Fuzzy MF Shape
      begin
      FuzzyInputMFsPtr := DataPointer1;
      FuzzyOutputMFsPtr := DataPointer2;
      ParseFields(IniFile.ReadString(ProfileName, IniName, ''), FieldResults);
      for i := 0 to 8 do
        begin
        FuzzyInputMFsPtr^.MF[i].Shape := round(FieldResults.FieldValue[i] * ScaleFactor);
        end;
      for i := 0 to 2 do
        begin
        FuzzyOutputMFsPtr^.MF[i].Shape := round(FieldResults.FieldValue[i+9] * ScaleFactor);
        end;
      end;
    if DataType = 1 then                               // Fuzzy MF Point0
      begin
      FuzzyInputMFsPtr := DataPointer1;
      FuzzyOutputMFsPtr := DataPointer2;
      ParseFields(IniFile.ReadString(ProfileName, IniName, ''), FieldResults);
      for i := 0 to 8 do
        begin
        FuzzyInputMFsPtr^.MF[i].Point0 := round(FieldResults.FieldValue[i] * ScaleFactor);
        end;
      for i := 0 to 2 do
        begin
        FuzzyOutputMFsPtr^.MF[i].Point0 := round(FieldResults.FieldValue[i+9] * ScaleFactor);
        end;
      end;
    if DataType = 2 then                               // Fuzzy MF Point1
      begin
      FuzzyInputMFsPtr := DataPointer1;
      FuzzyOutputMFsPtr := DataPointer2;
      ParseFields(IniFile.ReadString(ProfileName, IniName, ''), FieldResults);
      for i := 0 to 8 do
        begin
        FuzzyInputMFsPtr^.MF[i].Point1 := round(FieldResults.FieldValue[i] * ScaleFactor);
        end;
      for i := 0 to 2 do
        begin
        FuzzyOutputMFsPtr^.MF[i].Point1 := round(FieldResults.FieldValue[i+9] * ScaleFactor);
        end;
      end;
    if DataType = 3 then                               // Fuzzy MF Point2
      begin
      FuzzyInputMFsPtr := DataPointer1;
      FuzzyOutputMFsPtr := DataPointer2;
      ParseFields(IniFile.ReadString(ProfileName, IniName, ''), FieldResults);
      for i := 0 to 8 do
        begin
        FuzzyInputMFsPtr^.MF[i].Point2 := round(FieldResults.FieldValue[i] * ScaleFactor);
        end;
      for i := 0 to 2 do
        begin
        FuzzyOutputMFsPtr^.MF[i].Point2 := round(FieldResults.FieldValue[i+9] * ScaleFactor);
        end;
      end;
    if DataType = 4 then                               // Fuzzy MF Point3
      begin
      FuzzyInputMFsPtr := DataPointer1;
      FuzzyOutputMFsPtr := DataPointer2;
      ParseFields(IniFile.ReadString(ProfileName, IniName, ''), FieldResults);
      for i := 0 to 8 do
        begin
        FuzzyInputMFsPtr^.MF[i].Point3 := round(FieldResults.FieldValue[i] * ScaleFactor);
        end;
      for i := 0 to 2 do
        begin
        FuzzyOutputMFsPtr^.MF[i].Point3 := round(FieldResults.FieldValue[i+9] * ScaleFactor);
        end;
      end;
    if DataType = 5 then                               // Fuzzy Rule Input0
      begin
      FuzzyInputRulesPtr := DataPointer1;
      ParseFields(IniFile.ReadString(ProfileName, IniName, ''), FieldResults);
      for i := 0 to FieldResults.FieldCount-1 do
        begin
        FuzzyInputRulesPtr^.InputRules[i].Input0 := round(FieldResults.FieldValue[i] * ScaleFactor);
        end;
      end;
    if DataType = 6 then                               // Fuzzy Rule Input1
      begin
      FuzzyInputRulesPtr := DataPointer1;
      ParseFields(IniFile.ReadString(ProfileName, IniName, ''), FieldResults);
      for i := 0 to FieldResults.FieldCount-1 do
        begin
        FuzzyInputRulesPtr^.InputRules[i].Input1 := round(FieldResults.FieldValue[i] * ScaleFactor);
        end;
      end;
    if DataType = 7 then                               // Fuzzy Rule Input2
      begin
      FuzzyInputRulesPtr := DataPointer1;
      ParseFields(IniFile.ReadString(ProfileName, IniName, ''), FieldResults);
      for i := 0 to FieldResults.FieldCount-1 do
        begin
        FuzzyInputRulesPtr^.InputRules[i].Input2 := round(FieldResults.FieldValue[i] * ScaleFactor);
        end;
      end;
    if DataType = 8 then                               // Fuzzy Rule Output
      begin
      FuzzyOutputRulesPtr := DataPointer2;
      ParseFields(IniFile.ReadString(ProfileName, IniName, ''), FieldResults);
      for i := 0 to FieldResults.FieldCount-1 do
        begin
        FuzzyOutputRulesPtr^.OutputRule[i] := round(FieldResults.FieldValue[i] * ScaleFactor);
        end;
      end;
    if DataType = 9 then                               // Fuzzy Rule Connection - and/or
      begin
      FuzzyRuleInfoPtr := DataPointer1;
      ParseFields(IniFile.ReadString(ProfileName, IniName, ''), FieldResults);
      for i := 0 to FieldResults.FieldCount-1 do
        begin
        FuzzyRuleInfoPtr^.RuleType[i] := round(FieldResults.FieldValue[i] * ScaleFactor);
        end;
      end;
    if DataType = 10 then                               // Fuzzy Rule Weight
      begin
      FuzzyRuleInfoPtr := DataPointer1;
      ParseFields(IniFile.ReadString(ProfileName, IniName, ''), FieldResults);
      for i := 0 to FieldResults.FieldCount-1 do
        begin
        FuzzyRuleInfoPtr^.RuleWeight[i] := round(FieldResults.FieldValue[i] * ScaleFactor);
        end;
      FuzzyRuleInfoPtr^.RuleCount := FieldResults.FieldCount;
      end;
    if DataType = 11 then                               // Fuzzy Min Output
      begin
      FuzzyOutputInfoPtr := DataPointer1;
      ParseFields(IniFile.ReadString(ProfileName, IniName, ''), FieldResults);
      FuzzyOutputInfoPtr^.Min := round(FieldResults.FieldValue[i] * ScaleFactor);
      end;
    if DataType = 12 then                               // Fuzzy Max Output
      begin
      FuzzyOutputInfoPtr := DataPointer1;
      ParseFields(IniFile.ReadString(ProfileName, IniName, ''), FieldResults);
      FuzzyOutputInfoPtr^.Max := round(FieldResults.FieldValue[i] * ScaleFactor);
      end;
end;

//-------------------------------------------------------------------------------------

procedure stCDProfileData.LoadFromIniFile (ProfileName: string);
var
  IniFile: TIniFile;
  bLoadingSave: boolean;
  TmpInt: integer;
  TmpBool: boolean;
begin
  bLoadingSave := bLoading;
  bLoading := true;
  IniFile := TIniFile.Create(AppPath + 'PROFILES_CD.INI');

  IniConfigData.ProfileName := ProfileName;

  SetIniData(IniFile, ProfileName, 'DIGIO1', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF2.DigIo1 := TmpInt;
  SetIniData(IniFile, ProfileName, 'DIGIO2', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF2.DigIo2 := TmpInt;
  SetIniData(IniFile, ProfileName, 'DIGIO3', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF2.DigIo3 := TmpInt;
  SetIniData(IniFile, ProfileName, 'DIGIO4', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF2.DigIo4 := TmpInt;
  SetIniData(IniFile, ProfileName, 'DIGIO5', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF2.DigIo5 := TmpInt;
  SetIniData(IniFile, ProfileName, 'DIGIO6', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF2.DigIo6 := TmpInt;

  SetIniData(IniFile, ProfileName, 'DEFAULT_OUTPUT', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF8.DefaultOutput := TmpInt;
  SetIniData(IniFile, ProfileName, 'SENSOR_MODE', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF8.SensorMode := TmpInt;
  SetIniData(IniFile, ProfileName, 'CAN_USE', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF8.CanUse := TmpInt;
  SetIniData(IniFile, ProfileName, 'SOURCE_ADDRESS', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF8.SourceAddress := TmpInt;
  SetIniData(IniFile, ProfileName, 'CAN_ECHO', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF8.CanEcho := TmpInt;
  SetIniData(IniFile, ProfileName, 'CAN_CRC_ENABLE', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF8.CanCrcEnable := TmpInt;
  SetIniData(IniFile, ProfileName, 'CAN_PRESCALER', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF8.CanPrescaler := TmpInt;
  SetIniData(IniFile, ProfileName, 'CAN_ENDIANESS', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF8.Endianess := TmpInt;


  SetIniData(IniFile, ProfileName, 'CAN_ADDRESS_OUT_0', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.CanAddressOut[0]:= TmpInt;
  SetIniData(IniFile, ProfileName, 'CAN_FREQUENCY_OUT_0', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.CanMsgOutTime[0]:= TmpInt;

  SetIniData(IniFile, ProfileName, 'CAN_ADDRESS_OUT_1', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.CanAddressOut[1]:= TmpInt;
  SetIniData(IniFile, ProfileName, 'CAN_FREQUENCY_OUT_1', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.CanMsgOutTime[1]:= TmpInt;

  SetIniData(IniFile, ProfileName, 'CAN_ADDRESS_OUT_2', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.CanAddressOut[2]:= TmpInt;
  SetIniData(IniFile, ProfileName, 'CAN_FREQUENCY_OUT_2', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.CanMsgOutTime[2]:= TmpInt;

  SetIniData(IniFile, ProfileName, 'CAN_ADDRESS_OUT_3', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.CanAddressOut[3]:= TmpInt;

  SetIniData(IniFile, ProfileName, 'CAN_ADDRESS_OUT_4', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.CanAddressOut[4]:= TmpInt;

  SetIniData(IniFile, ProfileName, 'CAN_ADDRESS_OUT_5', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.CanAddressOut[5]:= TmpInt;

  SetIniData(IniFile, ProfileName, 'CAN_ADDRESS_IN_0', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.CanAddressIn[0]:= TmpInt;
  SetIniData(IniFile, ProfileName, 'CAN_FREQUENCY_IN_0', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.CanMsgInTime[0]:= TmpInt;
  SetIniData(IniFile, ProfileName, 'CAN_MASK_IN_0', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.CanMaskIn[0]:= TmpInt;

  SetIniData(IniFile, ProfileName, 'CAN_ADDRESS_In_1', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.CanAddressIn[1]:= TmpInt;
  SetIniData(IniFile, ProfileName, 'CAN_FREQUENCY_IN_1', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.CanMsgInTime[0]:= TmpInt;
  SetIniData(IniFile, ProfileName, 'CAN_MASK_IN_1', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.CanMaskIn[1]:= TmpInt;

  SetIniData(IniFile, ProfileName, 'CAN_SAMPLING_DELAY', 0, @TmpInt, 100.0);
  CDProfileData.IniConfigData.CanSamplingDelay:= TmpInt;

  SetIniData(IniFile, ProfileName, 'OUTPUT_STARTUP_DELAY', 0, @TmpInt, 100.0);
  CDProfileData.IniConfigData.OutputStartupdelay:= TmpInt;

  SetIniData(IniFile, ProfileName, 'KMA_FILTER_GAIN', 0, @CDProfileData.IniConfigData.KmaFilterGain, 1.0);
  SetIniData(IniFile, ProfileName, 'MASTER_FILTER_GAIN', 0, @CDProfileData.IniConfigData.MasterIndexFilteringGain, 1.0);
  SetIniData(IniFile, ProfileName, 'ACCEL_X_FILTER_GAIN', 0, @CDProfileData.IniConfigData.AccelVoltageXAdcFilteringGain, 1.0);
  SetIniData(IniFile, ProfileName, 'ACCEL_Y_FILTER_GAIN', 0, @CDProfileData.IniConfigData.AccelVoltageYAdcFilteringGain, 1.0);
  SetIniData(IniFile, ProfileName, 'ACCEL_Z_FILTER_GAIN', 0, @CDProfileData.IniConfigData.AccelVoltageZAdcFilteringGain, 1.0);

  SetIniData(IniFile, ProfileName, 'KMA_REV', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF3.ReverseIndexKma := TmpInt;
  SetIniData(IniFile, ProfileName, 'ERROR_OUTPUT_ACTIVE_LEVEL', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF1.ErrorConditionActiveState := TmpInt;
  SetIniData(IniFile, ProfileName, 'MASTER_FREQ_MODE', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF1.MasterFreqMode := TmpInt;
  SetIniData(IniFile, ProfileName, 'RS485_MODE', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF3.RS485MessageMode := TmpInt;
  SetIniData(IniFile, ProfileName, 'RS485_TIME', 0, @TmpInt, 10.0);
  CDProfileData.IniConfigData.BF3.RS485MessageTime := TmpInt;
  SetIniData(IniFile, ProfileName, 'ACCELEROMETER_SCALING', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF3.AccelerometerScaling := TmpInt;

  SetIniData(IniFile, ProfileName, 'RV_OUTPUT_MODE', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF1.RVOutputMode := TmpInt;
  SetIniData(IniFile, ProfileName, 'RV_OUTPUT_PULLUP', 0, @TmpInt, 1.0);
  CDProfileData.IniConfigData.BF1.RVOutputPullup := TmpInt;

  SetIniData(IniFile, ProfileName, 'RV_SETPOINT_TABLE', 2, @CDProfileData.IniConfigData.SetpointTable, 0.0);
  SetIniData(IniFile, ProfileName, 'MASTER_FREQUENCY_TABLE', 3, @CDProfileData.IniConfigData.MasterFreqTable, 0.0);
  SetIniData(IniFile, ProfileName, 'CAN_BUS_TABLE', 3, @CDProfileData.IniConfigData.CanTable, 0.0);
  SetIniData(IniFile, ProfileName, 'ANGULAR_OUTPUT_TABLE', 3, @CDProfileData.IniConfigData.AngularOutputTable, 0.0);

  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY1_MF_SHAPE_ARRAY', 0, @CDProfileData.Fuzzy1Data.FuzzyInputMFs, @CDProfileData.Fuzzy1Data.FuzzyOutputMFs, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY1_MF_POINT0_ARRAY', 1, @CDProfileData.Fuzzy1Data.FuzzyInputMFs, @CDProfileData.Fuzzy1Data.FuzzyOutputMFs, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY1_MF_POINT1_ARRAY', 2, @CDProfileData.Fuzzy1Data.FuzzyInputMFs, @CDProfileData.Fuzzy1Data.FuzzyOutputMFs, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY1_MF_POINT2_ARRAY', 3, @CDProfileData.Fuzzy1Data.FuzzyInputMFs, @CDProfileData.Fuzzy1Data.FuzzyOutputMFs, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY1_MF_POINT3_ARRAY', 4, @CDProfileData.Fuzzy1Data.FuzzyInputMFs, @CDProfileData.Fuzzy1Data.FuzzyOutputMFs, 1.0);

  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY2_MF_SHAPE_ARRAY', 0, @CDProfileData.Fuzzy2Data.FuzzyInputMFs, @CDProfileData.Fuzzy2Data.FuzzyOutputMFs, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY2_MF_POINT0_ARRAY', 1, @CDProfileData.Fuzzy2Data.FuzzyInputMFs, @CDProfileData.Fuzzy2Data.FuzzyOutputMFs, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY2_MF_POINT1_ARRAY', 2, @CDProfileData.Fuzzy2Data.FuzzyInputMFs, @CDProfileData.Fuzzy2Data.FuzzyOutputMFs, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY2_MF_POINT2_ARRAY', 3, @CDProfileData.Fuzzy2Data.FuzzyInputMFs, @CDProfileData.Fuzzy2Data.FuzzyOutputMFs, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY2_MF_POINT3_ARRAY', 4, @CDProfileData.Fuzzy2Data.FuzzyInputMFs, @CDProfileData.Fuzzy2Data.FuzzyOutputMFs, 1.0);

  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY1_INPUT0_RULE_ARRAY', 5, @CDProfileData.Fuzzy1Data.FuzzyInputRules, @CDProfileData.Fuzzy1Data.FuzzyOutputRules, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY1_INPUT1_RULE_ARRAY', 6, @CDProfileData.Fuzzy1Data.FuzzyInputRules, @CDProfileData.Fuzzy1Data.FuzzyOutputRules, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY1_INPUT2_RULE_ARRAY', 7, @CDProfileData.Fuzzy1Data.FuzzyInputRules, @CDProfileData.Fuzzy1Data.FuzzyOutputRules, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY1_OUTPUT_RULE_ARRAY', 8, @CDProfileData.Fuzzy1Data.FuzzyInputRules, @CDProfileData.Fuzzy1Data.FuzzyOutputRules, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY1_RULE_CONNECTION_ARRAY', 9, @CDProfileData.Fuzzy1Data.FuzzyRuleInfo, @CDProfileData.Fuzzy1Data.FuzzyOutputRules, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY1_RULE_WEIGHT_ARRAY', 10, @CDProfileData.Fuzzy1Data.FuzzyRuleInfo, @CDProfileData.Fuzzy1Data.FuzzyOutputRules, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY1_OUTPUT_MIN', 11, @CDProfileData.Fuzzy1Data.FuzzyOutputInfo, @CDProfileData.Fuzzy1Data.FuzzyOutputRules, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY1_OUTPUT_MAX', 12, @CDProfileData.Fuzzy1Data.FuzzyOutputInfo, @CDProfileData.Fuzzy1Data.FuzzyOutputRules, 1.0);
  CalculateFuzzyVals(@CDProfileData.Fuzzy1Data);

  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY2_INPUT0_RULE_ARRAY', 5, @CDProfileData.Fuzzy2Data.FuzzyInputRules, @CDProfileData.Fuzzy2Data.FuzzyOutputRules, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY2_INPUT1_RULE_ARRAY', 6, @CDProfileData.Fuzzy2Data.FuzzyInputRules, @CDProfileData.Fuzzy2Data.FuzzyOutputRules, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY2_INPUT2_RULE_ARRAY', 7, @CDProfileData.Fuzzy2Data.FuzzyInputRules, @CDProfileData.Fuzzy2Data.FuzzyOutputRules, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY2_OUTPUT_RULE_ARRAY', 8, @CDProfileData.Fuzzy2Data.FuzzyInputRules, @CDProfileData.Fuzzy2Data.FuzzyOutputRules, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY2_RULE_CONNECTION_ARRAY', 9, @CDProfileData.Fuzzy2Data.FuzzyRuleInfo, @CDProfileData.Fuzzy2Data.FuzzyOutputRules, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY2_RULE_WEIGHT_ARRAY', 10, @CDProfileData.Fuzzy2Data.FuzzyRuleInfo, @CDProfileData.Fuzzy2Data.FuzzyOutputRules, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY2_OUTPUT_MIN', 11, @CDProfileData.Fuzzy2Data.FuzzyOutputInfo, @CDProfileData.Fuzzy2Data.FuzzyOutputRules, 1.0);
  SetFuzzyIniData(IniFile, ProfileName, 'FUZZY2_OUTPUT_MAX', 12, @CDProfileData.Fuzzy1Data.FuzzyOutputInfo, @CDProfileData.Fuzzy2Data.FuzzyOutputRules, 1.0);
  CalculateFuzzyVals(@CDProfileData.Fuzzy2Data);

  IniFile.Free;
  bLoading := bLoadingSave;
end;

//-------------------------------------------------------------------------------------

function stCDProfileData.GetMasterFreqSetpointTable : string;
var
  i: integer;
begin
  Result := '';
  for i := 0 to GuiConfigData.MasterFreqTable.TableLength - 1 do
    begin
    if i <> 0 then Result := Result + ',';
    Result := Result + format('[%1.2f,%1.2f]', [GuiConfigData.MasterFreqTable.TablePoint[i].Point1 / 100.0, GuiConfigData.MasterFreqTable.TablePoint[i].Point2 / 100.0]);
    end;
end;

//-------------------------------------------------------------------------------------

procedure stCDProfileData.SetMasterFreqSetpointTable (NewData: String);
var
  ParseResults: stParseResults;
  i: integer;
begin
  ParseRecords(NewData, ParseResults);
  GuiConfigData.MasterFreqTable.TableLength := ParseResults.ParseLength;
  for i := 0 to 31 do
    begin
    GuiConfigData.MasterFreqTable.TablePoint[i].Point1 := 0;
    GuiConfigData.MasterFreqTable.TablePoint[i].Point2 := 0;
    if ParseResults.ParseLength > i then
      begin
      GuiConfigData.MasterFreqTable.TablePoint[i].Point1 := round(ParseResults.ParsePoint[i].Point1 * 100.0);
      GuiConfigData.MasterFreqTable.TablePoint[i].Point2 := round(ParseResults.ParsePoint[i].Point2 * 100.0);
      end;
    end;
end;

//-------------------------------------------------------------------------------------

function stCDProfileData.GetRVSetpointTable : string;
var
  i: integer;
begin
  Result := '';
  for i := 0 to GuiConfigData.SetpointTable.TableLength - 1 do
    begin
    if i <> 0 then Result := Result + ',';
    Result := Result + format('[%1.2f,%1.3f]', [GuiConfigData.SetpointTable.TablePoint[i].Point1 / 100.0, GuiConfigData.SetpointTable.TablePoint[i].Point2 / 1000.0]);
    end;
end;

//-------------------------------------------------------------------------------------

procedure stCDProfileData.SetRVSetpointTable (NewData: String);
var
  ParseResults: stParseResults;
  i: integer;
begin
  ParseRecords(NewData, ParseResults);
  GuiConfigData.SetpointTable.TableLength := ParseResults.ParseLength;
  for i := 0 to 31 do
    begin
    GuiConfigData.SetpointTable.TablePoint[i].Point1 := 0;
    GuiConfigData.SetpointTable.TablePoint[i].Point2 := 0;
    if ParseResults.ParseLength > i then
      begin
      GuiConfigData.SetpointTable.TablePoint[i].Point1 := round(ParseResults.ParsePoint[i].Point1 * 100.0);
      GuiConfigData.SetpointTable.TablePoint[i].Point2 := round(ParseResults.ParsePoint[i].Point2 * 1000.0);
      end;
    end;
end;

//-------------------------------------------------------------------------------------

function stCDProfileData.GetCanBusTable : string;
var
  i: integer;
begin
  Result := '';
  for i := 0 to GuiConfigData.CanTable.TableLength - 1 do
    begin
    if i <> 0 then Result := Result + ',';
    Result := Result + format('[%1.2f,%1.0f]', [GuiConfigData.CanTable.TablePoint[i].Point1 / 100.0, GuiConfigData.CanTable.TablePoint[i].Point2 / 100.0]);
    end;
end;

//-------------------------------------------------------------------------------------

procedure stCDProfileData.SetCanBusTable (NewData: String);
var
  ParseResults: stParseResults;
  i: integer;
begin
  ParseRecords(NewData, ParseResults);
  GuiConfigData.CanTable.TableLength := ParseResults.ParseLength;
  for i := 0 to 31 do
    begin
    GuiConfigData.CanTable.TablePoint[i].Point1 := 0;
    GuiConfigData.CanTable.TablePoint[i].Point2 := 0;
    if ParseResults.ParseLength > i then
      begin
      GuiConfigData.CanTable.TablePoint[i].Point1 := round(ParseResults.ParsePoint[i].Point1 * 100.0);
      GuiConfigData.CanTable.TablePoint[i].Point2 := round(ParseResults.ParsePoint[i].Point2 * 100.0);
      end;
    end;
end;

//-------------------------------------------------------------------------------------

function stCDProfileData.GetAngularOutputTable : string;
var
  i: integer;
begin
  Result := '';
  for i := 0 to GuiConfigData.AngularOutputTable.TableLength - 1 do
    begin
    if i <> 0 then Result := Result + ',';
    Result := Result + format('[%1.2f,%d]', [GuiConfigData.AngularOutputTable.TablePoint[i].Point1 / 100.0, GuiConfigData.AngularOutputTable.TablePoint[i].Point2]);

    end;
end;

//-------------------------------------------------------------------------------------

procedure stCDProfileData.SetAngularOutputTable (NewData: String);
var
  ParseResults: stParseResults;
  i: integer;
begin
  ParseRecords(NewData, ParseResults);
  GuiConfigData.AngularOutputTable.TableLength := ParseResults.ParseLength;
  for i := 0 to 31 do
    begin
    GuiConfigData.AngularOutputTable.TablePoint[i].Point1 := 0;
    GuiConfigData.AngularOutputTable.TablePoint[i].Point2 := 0;
    if ParseResults.ParseLength > i then
      begin
      GuiConfigData.AngularOutputTable.TablePoint[i].Point1 := round(ParseResults.ParsePoint[i].Point1 * 100.0);
      GuiConfigData.AngularOutputTable.TablePoint[i].Point2 := round(ParseResults.ParsePoint[i].Point2 * 1.0);
      end;
    end;
end;

//-------------------------------------------------------------------------------------
function stCDProfileData.GetFuzzyMFTable(item: integer) : string;
var
  i: integer;
begin
  Result := '';
  for i := 0 to 11 do
    begin
    if i<> 0 then Result := Result + ',';
    if item = 0 then                               // return Fuzzy 1 string of shapes
      begin
      if i < 9 then
        begin
        Result := Result + format('%d', [Fuzzy1Data.FuzzyInputMFs.MF[i].Shape]);
        end
      else
        begin
        Result := Result + format('%d', [Fuzzy1Data.FuzzyOutputMFs.MF[i-9].Shape]);
        end;
      end

    else if item = 1 then                        // return Fuzzy 1 string of point0
      begin
      if i < 9 then
        begin
        Result := Result + format('%d', [Fuzzy1Data.FuzzyInputMFs.MF[i].Point0]);
        end
      else
        begin
        Result := Result + format('%d', [Fuzzy1Data.FuzzyOutputMFs.MF[i-9].Point0]);
        end;
      end

    else if item = 2 then                      // return Fuzzy 1 string of point1
      begin
      if i < 9 then
        begin
        Result := Result + format('%d', [Fuzzy1Data.FuzzyInputMFs.MF[i].Point1]);
        end
      else
        begin
        Result := Result + format('%d', [Fuzzy1Data.FuzzyOutputMFs.MF[i-9].Point1]);
        end;
      end

    else if item = 3 then                     // return Fuzzy 1 string of point2
      begin
      if i < 9 then
        begin
        Result := Result + format('%d', [Fuzzy1Data.FuzzyInputMFs.MF[i].Point2]);
        end
      else
        begin
        Result := Result + format('%d', [Fuzzy1Data.FuzzyOutputMFs.MF[i-9].Point2]);
        end;
      end

    else if item = 4 then                    // return Fuzzy 1 string of point3
      begin
      if i < 9 then
        begin
        Result := Result + format('%d', [Fuzzy1Data.FuzzyInputMFs.MF[i].Point3]);
        end
      else
        begin
        Result := Result + format('%d', [Fuzzy1Data.FuzzyOutputMFs.MF[i-9].Point3]);
        end;
      end

    else if item = 10 then                               // return Fuzzy 2 string of shapes
      begin
      if i < 9 then
        begin
        Result := Result + format('%d', [Fuzzy2Data.FuzzyInputMFs.MF[i].Shape]);
        end
      else
        begin
        Result := Result + format('%d', [Fuzzy2Data.FuzzyOutputMFs.MF[i-9].Shape]);
        end;
      end

    else if item = 11 then                        // return Fuzzy 2 string of point0
      begin
      if i < 9 then
        begin
        Result := Result + format('%d', [Fuzzy2Data.FuzzyInputMFs.MF[i].Point0]);
        end
      else
        begin
        Result := Result + format('%d', [Fuzzy2Data.FuzzyOutputMFs.MF[i-9].Point0]);
        end;
      end

    else if item = 12 then                      // return Fuzzy 2 string of point1
      begin
      if i < 9 then
        begin
        Result := Result + format('%d', [Fuzzy2Data.FuzzyInputMFs.MF[i].Point1]);
        end
      else
        begin
        Result := Result + format('%d', [Fuzzy2Data.FuzzyOutputMFs.MF[i-9].Point1]);
        end;
      end

    else if item = 13 then                     // return Fuzzy 2 string of point2
      begin
      if i < 9 then
        begin
        Result := Result + format('%d', [Fuzzy2Data.FuzzyInputMFs.MF[i].Point2]);
        end
      else
        begin
        Result := Result + format('%d', [Fuzzy2Data.FuzzyOutputMFs.MF[i-9].Point2]);
        end;
      end

    else if item = 14 then                    // return Fuzzy 2 string of point3
      begin
      if i < 9 then
        begin
        Result := Result + format('%d', [Fuzzy2Data.FuzzyInputMFs.MF[i].Point3]);
        end
      else
        begin
        Result := Result + format('%d', [Fuzzy2Data.FuzzyOutputMFs.MF[i-9].Point3]);
        end;
      end;


    end;
end;

//-------------------------------------------------------------------------------------
function stCDProfileData.GetFuzzyRules(InOutNumber: integer) : string;
var
  i: integer;
begin
  Result := '';
  if InOutNumber < 10 then
    begin
    for i := 0 to Fuzzy1Data.FuzzyRuleInfo.RuleCount - 1 do
      begin
      if i<> 0 then Result := Result + ',';
      if InOutNumber = 0 then
        begin
        Result := Result + format('%d', [Fuzzy1Data.FuzzyInputRules.InputRules[i].Input0]);
        end
      else if InOutNumber = 1 then
        begin
        Result := Result + format('%d', [Fuzzy1Data.FuzzyInputRules.InputRules[i].Input1]);
        end
      else if InOutNumber = 2 then
        begin
        Result := Result + format('%d', [Fuzzy1Data.FuzzyInputRules.InputRules[i].Input2]);
        end
      else if InOutNumber = 3 then
        begin
        Result := Result + format('%d', [Fuzzy1Data.FuzzyOutputRules.OutputRule[i]]);
        end
      else if InOutNumber = 4 then
        begin
        Result := Result + format('%d', [Fuzzy1Data.FuzzyRuleInfo.RuleWeight[i]]);
        end
      else if InOutNumber = 5 then
        begin
        Result := Result + format('%d', [Fuzzy1Data.FuzzyRuleInfo.RuleType[i]]);
        end;
      end;
    end
  else
    begin
    for i := 0 to Fuzzy2Data.FuzzyRuleInfo.RuleCount - 1 do
      begin
      if i<> 0 then Result := Result + ',';
      if InOutNumber = 10 then
        begin
        Result := Result + format('%d', [Fuzzy2Data.FuzzyInputRules.InputRules[i].Input0]);
        end
      else if InOutNumber = 11 then
        begin
        Result := Result + format('%d', [Fuzzy2Data.FuzzyInputRules.InputRules[i].Input1]);
        end
      else if InOutNumber = 12 then
        begin
        Result := Result + format('%d', [Fuzzy2Data.FuzzyInputRules.InputRules[i].Input2]);
        end
      else if InOutNumber = 13 then
        begin
        Result := Result + format('%d', [Fuzzy2Data.FuzzyOutputRules.OutputRule[i]]);
        end
      else if InOutNumber = 14 then
        begin
        Result := Result + format('%d', [Fuzzy2Data.FuzzyRuleInfo.RuleWeight[i]]);
        end
      else if InOutNumber = 15 then
        begin
        Result := Result + format('%d', [Fuzzy2Data.FuzzyRuleInfo.RuleType[i]]);
        end;
      end;
    end;

end;



//-------------------------------------------------------------------------------------

procedure stCDProfileData.ClearData;
begin
  ZeroMemory(@GuiConfigData, sizeof(GuiConfigData));
  GuiConfigData.BF1.RVOutputMode := 1;
  GuiConfigData.MasterFreqTable.TableLength:=2;
  GuiConfigData.MasterFreqTable.TablePoint[0].Point1 := 400;
  GuiConfigData.MasterFreqTable.TablePoint[0].Point2 := 200000;
  GuiConfigData.MasterFreqTable.TablePoint[1].Point1 := 102300;
  GuiConfigData.MasterFreqTable.TablePoint[1].Point2 := 100000;
  GuiConfigData.SetpointTable.TableLength := 2;
  GuiConfigData.SetpointTable.TablePoint[0].Point1 := 400;
  GuiConfigData.SetpointTable.TablePoint[0].Point2 := 5000;
  GuiConfigData.SetpointTable.TablePoint[1].Point1 := 102300;
  GuiConfigData.SetpointTable.TablePoint[1].Point2 := 0;
  GuiConfigData.CanTable.TableLength := 2;
  GuiConfigData.CanTable.TablePoint[0].Point1 := 400;
  GuiConfigData.CanTable.TablePoint[0].Point2 := 320;
  GuiConfigData.CanTable.TablePoint[1].Point1 := 102300;
  GuiConfigData.CanTable.TablePoint[1].Point2 := 576;
  GuiConfigData.AngularOutputTable.TableLength := 2;
  GuiConfigData.AngularOutputTable.TablePoint[0].Point1 := 400;
  GuiConfigData.AngularOutputTable.TablePoint[0].Point2 := 0;
  GuiConfigData.AngularOutputTable.TablePoint[1].Point1 := 102300;
  GuiConfigData.AngularOutputTable.TablePoint[1].Point2 := 180;


end;

//-------------------------------------------------------------------------------------

procedure stCDProfileData.UpdateCD_41;
var
  Tmp: integer;
begin
  Tmp := sizeof(CD_41_Upload);
  move(GuiConfigData.ProfileName, CD_41_Upload.ProfileName, Sizeof(CD_41_Upload.ProfileName));
  CD_41_Upload.BF2.DigIo1 := GuiConfigData.BF2.DigIo1;
  CD_41_Upload.BF2.DigIo2 := GuiConfigData.BF2.DigIo2;
  CD_41_Upload.BF2.DigIo3 := GuiConfigData.BF2.DigIo3;
  CD_41_Upload.BF2.DigIo4 := GuiConfigData.BF2.DigIo4;
  CD_41_Upload.BF2.DigIo5 := GuiConfigData.BF2.DigIo5;
  CD_41_Upload.BF2.DigIo6 := GuiConfigData.BF2.DigIo6;
  CD_41_Upload.KmaFilterGain := GuiConfigData.KmaFilterGain;
  CD_41_Upload.MasterIndexFilteringGain := GuiConfigData.MasterIndexFilteringGain;
  CD_41_Upload.AccelVoltageXAdcFilteringGain := GuiConfigData.AccelVoltageXAdcFilteringGain;
  CD_41_Upload.AccelVoltageYAdcFilteringGain := GuiConfigData.AccelVoltageYAdcFilteringGain;
  CD_41_Upload.AccelVoltageZAdcFilteringGain := GuiConfigData.AccelVoltageZAdcFilteringGain;

  CD_41_Upload.BF3.ReverseIndexKma := GuiConfigData.BF3.ReverseIndexKma;

  CD_41_Upload.BF8.DefaultOutput := GuiConfigData.BF8.DefaultOutput;
  CD_41_Upload.BF8.SensorMode := GuiConfigData.BF8.SensorMode;
  CD_41_Upload.BF8.CanUse := GuiConfigData.BF8.CanUse;
  CD_41_Upload.BF8.SourceAddress := GuiConfigData.BF8.SourceAddress;
  CD_41_Upload.BF8.CanEcho := GuiConfigData.BF8.CanEcho;
  CD_41_Upload.BF8.CanCrcEnable := GuiConfigData.BF8.CanCrcEnable;
  CD_41_Upload.BF8.CanPrescaler := GuiConfigData.BF8.CanPrescaler;
  CD_41_Upload.BF8.Endianess := GuiConfigData.BF8.Endianess;

  move(GuiConfigData.CanTable, CD_41_Upload.CanTable, sizeof(CD_41_Upload.CanTable));
  move(GuiConfigData.AngularOutputTable, CD_41_Upload.AngularOutputTable, sizeof(CD_41_Upload.AngularOutputTable));

  CD_41_Upload.CanAddressOut[0]:= GuiConfigData.CanAddressOut[0];
  CD_41_Upload.CanMsgOutTime[0]:= GuiConfigData.CanMsgOutTime[0];

  CD_41_Upload.CanAddressOut[1]:= GuiConfigData.CanAddressOut[1];
  CD_41_Upload.CanMsgOutTime[1]:= GuiConfigData.CanMsgOutTime[1];

  CD_41_Upload.CanAddressOut[2]:= GuiConfigData.CanAddressOut[2];
  CD_41_Upload.CanMsgOutTime[2]:= GuiConfigData.CanMsgOutTime[2];

  CD_41_Upload.CanAddressOut[3]:= GuiConfigData.CanAddressOut[3];

  CD_41_Upload.CanAddressOut[4]:= GuiConfigData.CanAddressOut[4];

  CD_41_Upload.CanAddressOut[5]:= GuiConfigData.CanAddressOut[5];

  CD_41_Upload.CanAddressIn[0]:= GuiConfigData.CanAddressIn[0];
  CD_41_Upload.CanMsgInTime[0]:= GuiConfigData.CanMsgInTime[0];
  CD_41_Upload.CanMaskIn[0]:= GuiConfigData.CanMaskIn[0];

  CD_41_Upload.CanAddressIn[1]:= GuiConfigData.CanAddressIn[1];
  CD_41_Upload.CanMsgInTime[1]:= GuiConfigData.CanMsgInTime[1];
  CD_41_Upload.CanMaskIn[1]:= GuiConfigData.CanMaskIn[1];

  CD_41_Upload.CanSamplingDelay:=GuiConfigData.CanSamplingDelay;
  CD_41_Upload.OutputStartupdelay:=GuiConfigData.OutputStartupdelay;

  CD_41_Upload.BF1.ErrorConditionActiveState := GuiConfigData.BF1.ErrorConditionActiveState;
  CD_41_Upload.BF1.RVOutputMode := GuiConfigData.BF1.RVOutputMode;
  CD_41_Upload.BF1.RVOutputPullup := GuiConfigData.BF1.RVOutputPullup;
  CD_41_Upload.BF1.MasterFreqMode := GuiConfigData.BF1.MasterFreqMode;
  CD_41_Upload.BF3.RS485MessageMode := GuiConfigData.BF3.RS485MessageMode;
  CD_41_Upload.BF3.RS485MessageTime := GuiConfigData.BF3.RS485MessageTime;
  CD_41_Upload.BF3.AccelerometerScaling := GuiConfigData.BF3.AccelerometerScaling;

  move(GuiConfigData.SetpointTable, CD_41_Upload.SetpointTable, sizeof(CD_41_Upload.SetpointTable));
  move(GuiConfigData.MasterFreqTable, CD_41_Upload.MasterFreqTable, sizeof(CD_41_Upload.MasterFreqTable));

  CD_41_Upload.BF7.Version := FLASH_VERSION;
  CD_41_Upload.BF7.CRC16 := CalcCrcArray(@CD_41_Upload, 2046);

end;

//-------------------------------------------------------------------------------------

procedure stCDProfileData.UpdateCD_42;
begin
  move(GuiCDFactoryData.TimeStr, CD_42_Upload.TimeStr, Sizeof(CD_42_Upload.TimeStr));
  move(GuiCDFactoryData.DateStr, CD_42_Upload.DateStr, Sizeof(CD_42_Upload.DateStr));
  move(GuiCDFactoryData.NameTesterStr, CD_42_Upload.NameTesterStr, Sizeof(CD_42_Upload.NameTesterStr));
  move(GuiCDFactoryData.ModelNumberStr, CD_42_Upload.ModelNumberStr, Sizeof(CD_42_Upload.ModelNumberStr));
  move(GuiCDFactoryData.SerialNumberStr, CD_42_Upload.SerialNumberStr, Sizeof(CD_42_Upload.SerialNumberStr));

  move(GuiCDFactoryData.LocationStr, CD_42_Upload.LocationStr, Sizeof(CD_42_Upload.LocationStr));
  move(GuiCDFactoryData.TypeStr, CD_42_Upload.TypeStr, Sizeof(CD_42_Upload.TypeStr));
  move(GuiCDFactoryData.ManufacturerStr, CD_42_Upload.ManufacturerStr, Sizeof(CD_42_Upload.ManufacturerStr));
  move(GuiCDFactoryData.HardwareIdStr, CD_42_Upload.HardwareIdStr, Sizeof(CD_42_Upload.HardwareIdStr));
  move(GuiCDFactoryData.SoftwareIdStr, CD_42_Upload.SoftwareIdStr, Sizeof(CD_42_Upload.SoftwareIdStr));
  move(GuiCDFactoryData.MakeStr, CD_42_Upload.MakeStr, Sizeof(CD_42_Upload.MakeStr));
  move(GuiCDFactoryData.ModeStr, CD_42_Upload.ModeStr, Sizeof(CD_42_Upload.ModeStr));
  move(GuiCDFactoryData.SerialStr, CD_42_Upload.SerialStr, Sizeof(CD_42_Upload.SerialStr));
  move(GuiCDFactoryData.UnitStr, CD_42_Upload.UnitStr, Sizeof(CD_42_Upload.UnitStr));

  CD_42_Upload.Calibration33 := GuiCDFactoryData.Calibration33;
  CD_42_Upload.PowerRail5KFactor := GuiCDFactoryData.PowerRail5KFactor;
  CD_42_Upload.KmaKFactor := GuiCDFactoryData.KmaKFactor;
  CD_42_Upload.PowerInputKFactor := GuiCDFactoryData.PowerInputKFactor;
  CD_42_Upload.RVOutputVoltageKFactor := GuiCDFactoryData.RVOutputVoltageKFactor;
  CD_42_Upload.RVOutputCurrentKFactor := GuiCDFactoryData.RVOutputCurrentKFactor;
  CD_42_Upload.PwmOutputKFactor := GuiCDFactoryData.PwmOutputKFactor;
  CD_42_Upload.BF5 := GuiCDFactoryData.BF5;
  CD_42_Upload.PowerInputFilteringGain := GuiCDFactoryData.PowerInputFilteringGain;
  CD_42_Upload.PowerRail5FilteringGain := GuiCDFactoryData.PowerRail5FilteringGain;
  CD_42_Upload.TemperatureFilteringGain := GuiCDFactoryData.TemperatureFilteringGain;
  CD_42_Upload.InternalReferenceFilteringGain := GuiCDFactoryData.InternalReferenceFilteringGain;
  CD_42_Upload.RVOutputVoltageFilteringGain := GuiCDFactoryData.RVOutputVoltageFilteringGain;
  CD_42_Upload.RVOutputCurrentFilteringGain := GuiCDFactoryData.RVOutputCurrentFilteringGain;

  CD_42_Upload.FirmwareBF7.Version := FIRMWARE_VERSION;
  CD_42_Upload.FirmwareBF7.CRC16 := CD_99.Crc16;

  CD_42_Upload.BF7.Version := FACTORY_VERSION;
  CD_42_Upload.BF7.CRC16 := CalcCrcArray(@CD_42_Upload, 2046);
end;

//-------------------------------------------------------------------------------------

procedure stCDProfileData.UpdateCD_43;
begin
  CD_43_Upload.Fuzzy1.BF7.Version := FUZZY_VERSION;
  CD_43_Upload.Fuzzy1.BF7.CRC16 := CalcCrcArray(@CD_43_Upload, 2046);
  CD_43_Upload.Fuzzy2.BF7.Version := FUZZY_VERSION;
  CD_43_Upload.Fuzzy2.BF7.CRC16 := CalcCrcArray(@CD_43_Upload, 2046);
end;

end.
