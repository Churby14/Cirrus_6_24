unit Unit1;

{$mode objfpc}{$H+}

interface

uses
Windows, Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, ExtCtrls, eventlog, global, parse,
StdCtrls, ComCtrls, Buttons, IniFiles, mmsystem, mystr2float, comm, serial, profile, crc16, Types;

type
  stPitchAngleLabelArray = array [0..15] of TLabel;
  stPitchIndexLabelArray = array [0..15] of TLabel;
  stPitchKFactorTable = array [0..15,0..15] of TEdit;

type
  stCDProfileData = class
  private
    IniConfigData: stConfigData;
    GuiConfigData: stConfigData;
    CurrentConfigData: stConfigData;
    GuiCDFactoryData: stCDFactoryData;
    CurrentCDFactoryData: stCDFactoryData;
    PitchAngleLabels: ^stPitchAngleLabelArray;
    PitchIndexLabels: ^stPitchIndexLabelArray;
    PitchKFactorTable: ^stPitchKFactorTable;
//    PitchAngles: TEdit;
//    PitchIndexes: TEdit;
  public
    procedure LoadFromIniFile (ProfileName: string);
    function GetRVSetpointTable : string;
    function GetMasterFreqSetpointTable : string;
    function GetPitchCalibration : string;
    function GetPitchAngles : string;
    function GetPitchIndexes : string;
    procedure SetPitchAngleLabel (AngleOffset: integer; AngleLabel: TLabel);
    procedure SetPitchIndexLabel (IndexOffset: integer; IndexLabel: TLabel);
    procedure SetPitchKFactorEdit (AngleOffset, IndexOffset: integer; KFactorEdit: TEdit);
    procedure SetRVSetpointTable (NewData: String);
    procedure SetMasterFreqSetpointTable (NewData: String);
    procedure SetPitchCalibration (NewData: String);
    procedure SetPitchAngles (NewData: String);
    procedure SetPitchIndexes (NewData: String);
    procedure SetPitchTable;
    procedure UpdatePitchTable;
    procedure ClearData;
    procedure UpdateCD_41;
    procedure UpdateCD_42;
    procedure UpdateCD_43;
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
    Button8: TButton;
    btnUploadProfile: TButton;
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
    cbUploadConfigReverseKMA: TCheckBox;
    cbUploadPitchControlAxis: TComboBox;
    cbUploadConfigDig4: TComboBox;
    cbUploadConfigDig5: TComboBox;
    cbUploadConfigDig6: TComboBox;
    cbUploadConfigDig1: TComboBox;
    cbUploadConfigDig2: TComboBox;
    cbUploadConfigDig3: TComboBox;
    cbUploadConfigErrorActiveLevel: TComboBox;
    cbUploadConfigErrorRV: TComboBox;
    cbUploadConfigLowFuelInvert: TCheckBox;
    cbUploadConfigLowFuelOutputLevel: TComboBox;
    cbUploadConfigMasterError: TComboBox;
    cbUploadConfigMasterMode: TComboBox;
    cbUploadConfigOutputMode: TComboBox;
    cbUploadConfigPullUpMode: TComboBox;
    cbUploadConfigReverseSlave1: TCheckBox;
    cbUploadConfigReverseSlave2: TCheckBox;
    cbUploadConfigReverseSlave3: TCheckBox;
    cbUploadConfigReverseSlave4: TCheckBox;
    cbUploadConfigReverseSlave5: TCheckBox;
    cbUploadConfigReverseSlave6: TCheckBox;
    cbUploadConfigReverseSlave7: TCheckBox;
    cbUploadConfigReverseSlave8: TCheckBox;
    cbUploadConfigReverseSlave9: TCheckBox;
    cbUploadConfigReverseSlave10: TCheckBox;
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
    edtConfig33Calibration: TLabeledEdit;
    edtConfig5VDCCalibration: TLabeledEdit;
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
    edtUploadPitchAngles: TEdit;
    edtUploadPitchAxisOffset: TEdit;
    edtUploadConfigAccelXGain: TLabeledEdit;
    edtUploadConfigAccelYGain: TLabeledEdit;
    edtUploadConfigAccelZGain: TLabeledEdit;
    edtConfigKmaCalibration: TLabeledEdit;
    edtUploadConfigFreq1: TEdit;
    edtUploadConfigFreq2: TEdit;
    edtUploadConfigKmaGain: TLabeledEdit;
    edtConfigManufacturingDate: TEdit;
    edtConfigManufacturingModelNumber: TEdit;
    edtConfigManufacturingNameTester: TEdit;
    edtConfigManufacturingSerialNumber: TEdit;
    edtConfigManufacturingTime: TEdit;
    edtUploadConfigLowFuelTripPoint: TEdit;
    edtUploadConfigMasterGain: TLabeledEdit;
    edtUploadConfigMasterToggleTime: TEdit;
    edtConfigPowerInputVoltageCalibration: TLabeledEdit;
    edtConfigPwmOutput: TLabeledEdit;
    edtUploadConfigOut1: TEdit;
    edtUploadConfigOut2: TEdit;
    edtConfigRVCurrentScaleCalibration: TLabeledEdit;
    edtUploadConfigRS485Time: TEdit;
    edtConfigRVVoltageCalibration: TLabeledEdit;
    edtUploadConfigRVToggleTime: TEdit;
    edtUploadPitchCalibration: TEdit;
    edtUploadPitchIndexes: TEdit;
    edtFPGAHoldValue: TEdit;
    edtFPGAKmaSamples: TEdit;
    edtFPGASetpointProfile: TEdit;
    edtFPGATripPoint: TEdit;
    edtTripPoint: TEdit;
    edtHoldValue: TEdit;
    edtKmaSamples: TEdit;
    edtSetpointProfile: TEdit;
    edtVoltsGreen: TEdit;
    gbAccelerometer2: TGroupBox;
    gbLowFuel2: TGroupBox;
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
    GroupBox2: TGroupBox;
    GroupBox23: TGroupBox;
    GroupBox26: TGroupBox;
    GroupBox27: TGroupBox;
    GroupBox28: TGroupBox;
    GroupBox29: TGroupBox;
    GroupBox3: TGroupBox;
    GroupBox30: TGroupBox;
    GroupBox31: TGroupBox;
    GroupBox4: TGroupBox;
    GroupBox6: TGroupBox;
    GroupBox7: TGroupBox;
    GroupBox8: TGroupBox;
    GroupBox9: TGroupBox;
    Image1: TImage;
    Image11: TImage;
    Image12: TImage;
    Image2: TImage;
    Image3: TImage;
    Label103: TLabel;
    Label104: TLabel;
    Label105: TLabel;
    Label106: TLabel;
    Label107: TLabel;
    Label108: TLabel;
    Label109: TLabel;
    Label110: TLabel;
    Label111: TLabel;
    Label112: TLabel;
    Label113: TLabel;
    Label114: TLabel;
    Label115: TLabel;
    Label116: TLabel;
    Label117: TLabel;
    Label119: TLabel;
    Label120: TLabel;
    Label121: TLabel;
    Label122: TLabel;
    Label123: TLabel;
    Label124: TLabel;
    Label125: TLabel;
    Label126: TLabel;
    Label127: TLabel;
    Label128: TLabel;
    Label129: TLabel;
    Label130: TLabel;
    Label131: TLabel;
    Label132: TLabel;
    Label133: TLabel;
    Label136: TLabel;
    Label138: TLabel;
    Label139: TLabel;
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
    Label221: TLabel;
    Label222: TLabel;
    Label223: TLabel;
    Label224: TLabel;
    Label225: TLabel;
    Label226: TLabel;
    Label227: TLabel;
    Label228: TLabel;
    Label229: TLabel;
    Label230: TLabel;
    Label231: TLabel;
    Label232: TLabel;
    Label233: TLabel;
    Label234: TLabel;
    Label235: TLabel;
    Label236: TLabel;
    Label237: TLabel;
    Label238: TLabel;
    Label239: TLabel;
    Label240: TLabel;
    Label241: TLabel;
    Label242: TLabel;
    Label243: TLabel;
    Label244: TLabel;
    Label245: TLabel;
    Label246: TLabel;
    Label247: TLabel;
    Label248: TLabel;
    Label249: TLabel;
    Label250: TLabel;
    Label251: TLabel;
    Label51: TLabel;
    Label57: TLabel;
    Label58: TLabel;
    Label59: TLabel;
    Label60: TLabel;
    Label61: TLabel;
    Label62: TLabel;
    Label69: TLabel;
    Label70: TLabel;
    Label71: TLabel;
    Label72: TLabel;
    Label73: TLabel;
    Label74: TLabel;
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
    leAlarm: TLabeledEdit;
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
    lePitchAngle: TLabeledEdit;
    lePitchKFactor: TLabeledEdit;
    lePitchMasterIndex: TLabeledEdit;
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
    leSlave10Freq: TEdit;
    leSlave10Index: TEdit;
    leSlave1Freq1: TEdit;
    leSlave1Index: TEdit;
    leSlave2Freq1: TEdit;
    leSlave2Index: TEdit;
    leSlave3Freq1: TEdit;
    leSlave3Index: TEdit;
    leSlave4Freq: TEdit;
    leSlave4Index: TEdit;
    leSlave5Freq: TEdit;
    leSlave5Index: TEdit;
    leSlave6Freq: TEdit;
    leSlave6Index: TEdit;
    leSlave7Freq: TEdit;
    leSlave7Index: TEdit;
    leSlave8Freq: TEdit;
    leSlave8Index: TEdit;
    leSlave9Freq: TEdit;
    leSlave9Index: TEdit;
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
    OpenDialog1: TOpenDialog;
    PageControl1: TPageControl;
    PageControl2: TPageControl;
    PageControl6: TPageControl;
    pnlReadConfig: TPanel;
    ProgressBar1: TProgressBar;
    ProgressBar2: TProgressBar;
    rgMode: TRadioGroup;
    rgProfileType: TRadioGroup;
    rgKmaDirection: TRadioGroup;
    rgSlaves: TRadioGroup;
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
    shtAngleCalibration: TTabSheet;
    TabSheet4: TTabSheet;
    TabSheet5: TTabSheet;
    TabSheet7: TTabSheet;
    TabSheet8: TTabSheet;
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
    procedure Button8Click(Sender: TObject);
    procedure btnUploadProfileClick(Sender: TObject);
    procedure cbCDProfileNamesChange(Sender: TObject);
    procedure cbCDProfileNamesCloseUp(Sender: TObject);
    procedure cbFPGAProfileNameChange(Sender: TObject);
    procedure cbManualChange(Sender: TObject);
    procedure cbProfileNameCloseUp(Sender: TObject);
    procedure cbUploadConfigDig1Change(Sender: TObject);
    procedure cbUploadConfigDig1CloseUp(Sender: TObject);
    procedure cbUploadConfigDig2Change(Sender: TObject);
    procedure cbUploadConfigErrorRVChange(Sender: TObject);
    procedure cbUploadConfigMasterErrorChange(Sender: TObject);
    procedure cbUploadConfigMasterModeChange(Sender: TObject);
    procedure cbUploadConfigOutputModeChange(Sender: TObject);
    procedure cbUploadConfigRS485FormatChange(Sender: TObject);
    procedure cbUploadPitchControlAxisChange(Sender: TObject);
    procedure edtSetpointProfileChange(Sender: TObject);
    procedure edtUploadConfigMasterSetpointChange(Sender: TObject);
    procedure edtUploadConfigRVsetpointChange(Sender: TObject);
    procedure edtUploadPitchAnglesChange(Sender: TObject);
    procedure edtUploadPitchCalibrationChange(Sender: TObject);
    procedure edtUploadPitchIndexesChange(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure imgIconDblClick(Sender: TObject);
    procedure rgModeClick(Sender: TObject);
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
    procedure ProcessCDCommunications();
    procedure UpdateProfile(ProfileStr: string; ScaleFactor: single; ErrorLabel: Tlabel; ProfileGraph: TProfileGraph; var NewTable: stTwoPointTable);
    procedure SyncControls;
    procedure SaveToIniFile;
    procedure LoadFromGui;
    procedure UpdateGui;
    procedure UpdateAngleCalibrationProfile;
    procedure LoadFactoryFromGui;


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


procedure TestMessage(MsgStr: string; Location: integer);

implementation


{$R *.lfm}

{ TForm1 }

var
  UploadVoltageResistanceProfileGraph: TProfileGraph;
  UploadMasterFreqProfileGraph: TProfileGraph;
  AngleCalibrationProfileGraph: TProfileGraph;

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

  CDProfileData := stCDProfileData.Create;
  CDProfileData.PitchIndexLabels := @PitchKFactorIndexLabel;
  CDProfileData.PitchAngleLabels := @PitchKFactorAngleLabel;
  CDProfileData.PitchKFactorTable := @PitchKFactorEdit;

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

  AngleCalibrationProfileGraph := TProfileGraph.Create;
  AngleCalibrationProfileGraph.GraphImage := Image1;
  AngleCalibrationProfileGraph.HorzHashMarks := 10;
  AngleCalibrationProfileGraph.VertHashMarks := 10;
  AngleCalibrationProfileGraph.VertTitle := 'ACCELEROMETER (g)';
  AngleCalibrationProfileGraph.VertLowScale := -1;
  AngleCalibrationProfileGraph.VertHighScale := 1;
  AngleCalibrationProfileGraph.VertPrecision := 3;
  AngleCalibrationProfileGraph.HorzTitle := 'ANGLE (Degs)';
  AngleCalibrationProfileGraph.HorzLowScale := -90;
  AngleCalibrationProfileGraph.HorzHighScale := 90;
  AngleCalibrationProfileGraph.HorzPrecision := 2;

  for i := 0 to 15 do                           // Angle
    begin
    PitchKFactorAngleLabel[i] := TLabel.Create(Self);
    PitchKFactorAngleLabel[i].Parent := GroupBox15;
    PitchKFactorAngleLabel[i].Font.Size := 7;
    PitchKFactorAngleLabel[i].Font.Style := [fsBold];
    PitchKFactorAngleLabel[i].Caption := 'Angle';
    PitchKFactorAngleLabel[i].Left := 5;
    PitchKFactorAngleLabel[i].Top := 28 + (i * 22);

    PitchKFactorIndexLabel[i] := TLabel.Create(Self);
    PitchKFactorIndexLabel[i].Parent := GroupBox15;
    PitchKFactorIndexLabel[i].Font.Size := 7;
    PitchKFactorIndexLabel[i].Font.Style := [fsBold];
    PitchKFactorIndexLabel[i].Caption := 'Index';
    PitchKFactorIndexLabel[i].Left := 45 + (i * 50);
    PitchKFactorIndexLabel[i].Top := 7;
    PitchKFactorIndexLabel[i].Alignment := taCenter;
    PitchKFactorIndexLabel[i].Width := 45;
    PitchKFactorIndexLabel[i].AutoSize := false;

    for j := 0 to 15 do                         // Index
      begin
      PitchKFactorEdit[i,j] := TEdit.Create(Self);
      PitchKFactorEdit[i,j].Parent := GroupBox15;
      PitchKFactorEdit[i,j].Width := 45;
      PitchKFactorEdit[i,j].Left := 45 + (j * 50);
      PitchKFactorEdit[i,j].Top := 25 + (i * 22);
      PitchKFactorEdit[i,j].Font.Size := 7;
      PitchKFactorEdit[i,j].Alignment := taCenter;
      PitchKFactorEdit[i,j].Text := '1.00';
      end;
    end;

  for i := 0 to 21 do
    begin
    ProcessTimingAverageTime[i] := TEdit.Create(Self);
    ProcessTimingAverageTime[i].Parent := TabSheet16;
    ProcessTimingAverageTime[i].Width := 52;
    ProcessTimingAverageTime[i].Left := 176;
    ProcessTimingAverageTime[i].Top := 38 + (I * 22);
    ProcessTimingAverageTime[i].Font.Size := 8;
    ProcessTimingAverageTime[i].Alignment := taCenter;

    ProcessTimingMinTime[i] := TEdit.Create(Self);
    ProcessTimingMinTime[i].Parent := TabSheet16;
    ProcessTimingMinTime[i].Width := 52;
    ProcessTimingMinTime[i].Left := 240;
    ProcessTimingMinTime[i].Top := 38 + (I * 22);
    ProcessTimingMinTime[i].Font.Size := 8;
    ProcessTimingMinTime[i].Alignment := taCenter;

    ProcessTimingMaxTime[i] := TEdit.Create(Self);
    ProcessTimingMaxTime[i].Parent := TabSheet16;
    ProcessTimingMaxTime[i].Width := 52;
    ProcessTimingMaxTime[i].Left := 304;
    ProcessTimingMaxTime[i].Top := 38 + (I * 22);
    ProcessTimingMaxTime[i].Font.Size := 8;
    ProcessTimingMaxTime[i].Alignment := taCenter;

    ProcessTimingCalls[i] := TEdit.Create(Self);
    ProcessTimingCalls[i].Parent := TabSheet16;
    ProcessTimingCalls[i].Width := 52;
    ProcessTimingCalls[i].Left := 368;
    ProcessTimingCalls[i].Top := 38 + (I * 22);
    ProcessTimingCalls[i].Font.Size := 8;
    ProcessTimingCalls[i].Alignment := taCenter;

    ProcessTimingLabel[i] := TLabel.Create(Self);
    ProcessTimingLabel[i].Parent := TabSheet16;
    ProcessTimingLabel[i].Width := 165;
    ProcessTimingLabel[i].Left := 5;
    ProcessTimingLabel[i].Top := 40 + (I * 22);
    ProcessTimingLabel[i].Font.Size := 8;
    ProcessTimingLabel[i].Alignment := taRightJustify;
    ProcessTimingLabel[i].Caption := ProcessTimingLabels[i];
    ProcessTimingLabel[i].AutoSize := false;
    ProcessTimingLabel[i].Font.Style := [fsBold];

    end;

  for i := 0 to 39 do
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

  SetpointData.CommConfig := COMM_CC;

  ScrollBox3.HorzScrollBar.Position := 0;

  PageControl1.TabIndex := 0;

  UpdateProfile(edtUploadConfigMasterSetpoint.Text, 100.0, Label250, UploadMasterFreqProfileGraph, TableResults);
  UpdateProfile(edtUploadConfigRVsetpoint.Text, 1000.0, Label245, UploadVoltageResistanceProfileGraph, TableResults);
  UpdateAngleCalibrationProfile();

  shtMasterFreqProfile2.TabVisible := false;
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
    lePowerCurrent.text := format('%1.3f', [SysVars.PS_Current / 1000.0]);    // Power Supply Current
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
    for i := 0 to 39 do
      begin
      CheckboxErrorCondition[i].Checked := CD_48.GeneralErrorTimer[i] <> 0;
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
      for i := 0 to 21 do
        begin
        ProcessTimingAverageTime[i].Text := format('%1.1f', [CD_71[i].AverageTime / 10.0]);
        ProcessTimingMinTime[i].Text := format('%d', [CD_71[i].MinTime]);
        ProcessTimingMaxTime[i].Text := format('%d', [CD_71[i].MaxTime]);
        ProcessTimingCalls[i].Text := format('%d', [CD_71[i].CallsPerSecond]);
        end;
      end;

  //----------------------------------------------------------

    if bCD_41_Update then          // Flash Configuration
      begin
      bCD_41_Update := false;
      bDownloadDone := true;
      move(CD_41_Download.ProfileName, CDProfileData.CurrentConfigData.ProfileName, Sizeof(CDProfileData.CurrentConfigData.ProfileName));
      CDProfileData.CurrentConfigData.DIG0 := CD_41_Download.DIG0;
      CDProfileData.CurrentConfigData.DIG1 := CD_41_Download.DIG1;
      CDProfileData.CurrentConfigData.DIG2 := CD_41_Download.DIG2;
      CDProfileData.CurrentConfigData.DIG3 := CD_41_Download.DIG3;
      CDProfileData.CurrentConfigData.DIG4 := CD_41_Download.DIG4;
      CDProfileData.CurrentConfigData.DIG5 := CD_41_Download.DIG5;
      CDProfileData.CurrentConfigData.KmaFilterGain := CD_41_Download.KmaFilterGain;
      CDProfileData.CurrentConfigData.MasterIndexFilteringGain := CD_41_Download.MasterIndexFilteringGain;
      CDProfileData.CurrentConfigData.AccelVoltageXAdcFilteringGain := CD_41_Download.AccelVoltageXAdcFilteringGain;
      CDProfileData.CurrentConfigData.AccelVoltageYAdcFilteringGain := CD_41_Download.AccelVoltageYAdcFilteringGain;
      CDProfileData.CurrentConfigData.AccelVoltageZAdcFilteringGain := CD_41_Download.AccelVoltageZAdcFilteringGain;
      CDProfileData.CurrentConfigData.KMAReverseFlag := (CD_41_Download.ReverseFlags and 1) <> 0;
      CDProfileData.CurrentConfigData.Slave1ReverseFlag := (CD_41_Download.ReverseFlags and 2) <> 0;
      CDProfileData.CurrentConfigData.Slave2ReverseFlag := (CD_41_Download.ReverseFlags and 4) <> 0;
      CDProfileData.CurrentConfigData.Slave3ReverseFlag := (CD_41_Download.ReverseFlags and 8) <> 0;
      CDProfileData.CurrentConfigData.Slave4ReverseFlag := (CD_41_Download.ReverseFlags and $10) <> 0;
      CDProfileData.CurrentConfigData.Slave5ReverseFlag := (CD_41_Download.ReverseFlags and $20) <> 0;
      CDProfileData.CurrentConfigData.Slave6ReverseFlag := (CD_41_Download.ReverseFlags and $40) <> 0;
      CDProfileData.CurrentConfigData.Slave7ReverseFlag := (CD_41_Download.ReverseFlags and $80) <> 0;
      CDProfileData.CurrentConfigData.Slave8ReverseFlag := (CD_41_Download.ReverseFlags and $100) <> 0;
      CDProfileData.CurrentConfigData.Slave9ReverseFlag := (CD_41_Download.ReverseFlags and $200) <> 0;
      CDProfileData.CurrentConfigData.Slave10ReverseFlag := (CD_41_Download.ReverseFlags and $400) <> 0;
      CDProfileData.CurrentConfigData.LowFuelLevelOutputLevel :=  (CD_41_Download.LowFuelLevelAlarmMode and 1);
      CDProfileData.CurrentConfigData.LowFuelLevelInvertFlag := (CD_41_Download.LowFuelLevelAlarmMode and 2) <> 0;
      CDProfileData.CurrentConfigData.LowFuelLevelTripPoint := CD_41_Download.LowFuelLevelTripPoint;
      CDProfileData.CurrentConfigData.ErrorOutputActiveLevel := (CD_41_Download.ErrorCondition and 1);
      CDProfileData.CurrentConfigData.RVOutputErrorMode := (CD_41_Download.ErrorCondition shr 3) and 3;
      CDProfileData.CurrentConfigData.MasterFreqErrorMode := (CD_41_Download.ErrorCondition shr 1) and 3;
      CDProfileData.CurrentConfigData.RVOutputMode := CD_41_Download.RVOutputMode and 3;
      CDProfileData.CurrentConfigData.RVPullupMode := (CD_41_Download.RVOutputMode shr 2) and 1;
      CDProfileData.CurrentConfigData.ErrorRVOutput1 := CD_41_Download.ErrorRVOutput1;
      CDProfileData.CurrentConfigData.ErrorRVOutput2 := CD_41_Download.ErrorRVOutput2;
      CDProfileData.CurrentConfigData.ErrorRvOutputToggleTime := CD_41_Download.ErrorRvOutputToggleTime;
      CDProfileData.CurrentConfigData.MasterFreqMode := CD_41_Download.MasterFreqMode and 1;
      CDProfileData.CurrentConfigData.ErrorMasterFreq1 := CD_41_Download.ErrorMasterFreq1;
      CDProfileData.CurrentConfigData.ErrorMasterFreq2 := CD_41_Download.ErrorMasterFreq2;
      CDProfileData.CurrentConfigData.ErrorMasterToggleTime := CD_41_Download.ErrorMasterToggleTime;
      CDProfileData.CurrentConfigData.RS485_MessageMode := CD_41_Download.RS485_MessageMode;
      CDProfileData.CurrentConfigData.RS485_MessageTime := CD_41_Download.RS485_MessageTime;
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
      CDProfileData.CurrentCDFactoryData.Calibration33 := CD_42_Download.Calibration33;
      CDProfileData.CurrentCDFactoryData.PowerRail5KFactor := CD_42_Download.PowerRail5KFactor;
      CDProfileData.CurrentCDFactoryData.KmaKFactor := CD_42_Download.KmaKFactor;
      CDProfileData.CurrentCDFactoryData.PowerInputKFactor := CD_42_Download.PowerInputKFactor;
      CDProfileData.CurrentCDFactoryData.RVOutputVoltageKFactor := CD_42_Download.RVOutputVoltageKFactor;
      CDProfileData.CurrentCDFactoryData.RVOutputCurrentKFactor := CD_42_Download.RVOutputCurrentKFactor;
      CDProfileData.CurrentCDFactoryData.PwmOutputKFactor := CD_42_Download.PwmOutputKFactor;
      CDProfileData.CurrentCDFactoryData.Hardware := CD_42_Download.Hardware;

      cbPcbRevision.ItemIndex := (CD_42_Download.Hardware and $F) + 1;
      cbAccelerometer.Checked := (CD_42_Download.Hardware and $10) <> 0;
      cbRVOutput.Checked := (CD_42_Download.Hardware and $20) <> 0;
      cbRS485Driver.Checked := (CD_42_Download.Hardware and $40) <> 0;
      cbCANBusDriver.Checked := (CD_42_Download.Hardware and $80) <> 0;
      cbTemperatureSensor.Checked := (CD_42_Download.Hardware and $100) <> 0;

      edtConfig33Calibration.Text := format('%1.3f', [CD_42_Download.Calibration33 / 1000.0]);
      edtConfig5VDCCalibration.Text := format('%1.0f', [CD_42_Download.PowerRail5KFactor / 1.0]);
      edtConfigKmaCalibration.Text := format('%1.0f', [CD_42_Download.KmaKFactor / 1.0]);
      edtConfigPowerInputVoltageCalibration.Text := format('%1.0f', [CD_42_Download.PowerInputKFactor / 1.0]);
      edtConfigRVVoltageCalibration.Text := format('%1.0f', [CD_42_Download.RVOutputVoltageKFactor / 1.0]);
      edtConfigRVCurrentScaleCalibration.Text := format('%1.0f', [CD_42_Download.RVOutputCurrentKFactor / 1.0]);
      edtConfigPwmOutput.Text := format('%1.0f', [CD_42_Download.PwmOutputKFactor / 1.0]);


      CD_42_Download.DateStr[11] := #0;
      CD_42_Download.TimeStr[11] := #0;
      CD_42_Download.NameTesterStr[39] := #0;
      CD_42_Download.ModelNumberStr[39] := #0;
      CD_42_Download.SerialNumberStr[39] := #0;
      edtConfigManufacturingDate.Text := StrPas(CD_42_Download.DateStr);
      edtConfigManufacturingTime.Text := StrPas(CD_42_Download.TimeStr);
      edtConfigManufacturingNameTester.Text := StrPas(CD_42_Download.NameTesterStr);
      edtConfigManufacturingModelNumber.Text := StrPas(CD_42_Download.ModelNumberStr);
      edtConfigManufacturingSerialNumber.Text := StrPas(CD_42_Download.SerialNumberStr);

      end;

  //----------------------------------------------------------

    if bCD_43_Update then          // Pitch Configuration
      begin
      bCD_43_Update := false;
      bDownloadDone := true;
      CDProfileData.CurrentConfigData.PitchAxis := CD_43_Download.ControlAxis;
      CDProfileData.CurrentConfigData.PitchAxisOffset := CD_43_Download.ControlAxisOffset;
      move(CD_43_Download.ControlAxisCalibration, CDProfileData.CurrentConfigData.PitchAxisCalibration, sizeof(CDProfileData.CurrentConfigData.PitchAxisCalibration));
      move(CD_43_Download.ControlAngleTable, CDProfileData.CurrentConfigData.PitchAngleTable, sizeof(CDProfileData.CurrentConfigData.PitchAngleTable));
      move(CD_43_Download.ControlIndexTable, CDProfileData.CurrentConfigData.PitchIndexTable, sizeof(CDProfileData.CurrentConfigData.PitchIndexTable));
      move(CD_43_Download.ControlKTable, CDProfileData.CurrentConfigData.PitchKFactorTable, sizeof(CDProfileData.CurrentConfigData.PitchKFactorTable));
      end;

  //----------------------------------------------------------

    if bCD_44_Update then          // Event Log
      begin
      bCD_44_Update := false;
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
      for i := 0 to 511 do
        begin
        if CD_44[StartingIndex].EntryIndex = -1 then
          break;
        TmpStr := format('%u [%1.2f]:  ', [CD_44[StartingIndex].EntryIndex, CD_44[StartingIndex].TimeStamp / 1000.0]);
        TmpStr := TmpStr + StrPas(CD_44[StartingIndex].MessageData);
        memo2.Lines.add(TmpStr);
        StartingIndex := StartingIndex + 1;
        if StartingIndex = 512 then
          StartingIndex := 0;
        end;
      memo2.Lines.EndUpdate;
      OldNumberOfEvents := CD_70.ENG_NumberEventLogEntries;
      end;

  //----------------------------------------------------------

    if bCD_70_Update then                         // System Variables
      begin
      bCD_70_Update := false;
      CD_70_UpdateCounter := 2;

      leFirmwareStackSlack.Text := format('%d', [CD_70.StackSlack * 4]);
      leFirmwareStackUsed.Text := format('%d', [CD_70.StackUsed * 4]);

      lePowerInputVoltage1.Text := format('%d', [CD_70.ADC_RawCounts[1]]);
      leKma1.Text := format('%d', [CD_70.ADC_RawCounts[2]]);
      leTemp1.Text := format('%d', [CD_70.ADC_RawCounts[4]]);
      leRVOutputVoltage1.Text := format('%d', [CD_70.ADC_RawCounts[0]]);
      leRVOutputCurrent1.Text := format('%d', [CD_70.ADC_RawCounts[5]]);
      leAccelXg1.Text := format('%d', [CD_70.ADC_RawCounts[7]]);
      leAccelYg1.Text := format('%d', [CD_70.ADC_RawCounts[8]]);
      leAccelZg1.Text := format('%d', [CD_70.ADC_RawCounts[9]]);
      leIntRef9.Text := format('%d', [CD_70.ADC_RawCounts[6]]);
      le5PwrRail1.Text := format('%d', [CD_70.ADC_RawCounts[3]]);

      lePowerInputVoltage2.Text := format('%1.1f', [CD_70.ADC_FilteredCounts[1] / 100.0]);
      leKma2.Text := format('%1.1f', [CD_70.ADC_FilteredCounts[2] / 100.0]);
      leTemp2.Text := format('%1.1f', [CD_70.ADC_FilteredCounts[4] / 100.0]);
      leRVOutputVoltage2.Text := format('%1.1f', [CD_70.ADC_FilteredCounts[0] / 100.0]);
      leRVOutputCurrent2.Text := format('%1.1f', [CD_70.ADC_FilteredCounts[5] / 100.0]);
      leAccelXg2.Text := format('%1.1f', [CD_70.ADC_FilteredCounts[7] / 100.0]);
      leAccelYg2.Text := format('%1.1f', [CD_70.ADC_FilteredCounts[8] / 100.0]);
      leAccelZg2.Text := format('%1.1f', [CD_70.ADC_FilteredCounts[9] / 100.0]);
      leIntRef10.Text := format('%1.1f', [CD_70.ADC_FilteredCounts[6] / 100.0]);
      le5PwrRail2.Text := format('%1.1f', [CD_70.ADC_FilteredCounts[3] / 100.0]);

      lePowerInputVoltage.Text := format('%1.3f', [CD_70.ENG_PowerInputVoltage / 1000.0]);
      leKma.Text := format('%1.4f', [CD_70.ENG_KmaVoltage / 10000.0]);
      leTemp.Text := format('%1.2f', [CD_70.ENG_TemperatureCelsius / 100.0]);
      leRVOutputVoltage.Text := format('%1.4f', [CD_70.ENG_RVOutputVoltage / 10000.0]);
      leRVOutputCurrent.Text := format('%1.3f', [CD_70.ENG_RVOutputCurrent / 1000.0]);
      leAccelXg.Text := format('%1.4f', [CD_70.ENG_AccelGravityX / 10000.0]);
      leAccelYg.Text := format('%1.4f', [CD_70.ENG_AccelGravityY / 10000.0]);
      leAccelZg.Text := format('%1.4f', [CD_70.ENG_AccelGravityZ / 10000.0]);
      le5PwrRail.Text := format('%1.3f', [CD_70.ENG_PowerRail5VDC / 1000.0]);

      leSlave1Freq1.Text := format('%1.2f', [CD_70.ENG_SlaveFrequency[0] / 100.0]);
      leSlave2Freq1.Text := format('%1.2f', [CD_70.ENG_SlaveFrequency[1] / 100.0]);
      leSlave3Freq1.Text := format('%1.2f', [CD_70.ENG_SlaveFrequency[2] / 100.0]);
      leSlave4Freq.Text := format('%1.2f', [CD_70.ENG_SlaveFrequency[3] / 100.0]);
      leSlave5Freq.Text := format('%1.2f', [CD_70.ENG_SlaveFrequency[4] / 100.0]);
      leSlave6Freq.Text := format('%1.2f', [CD_70.ENG_SlaveFrequency[5] / 100.0]);
      leSlave7Freq.Text := format('%1.2f', [CD_70.ENG_SlaveFrequency[6] / 100.0]);
      leSlave8Freq.Text := format('%1.2f', [CD_70.ENG_SlaveFrequency[7] / 100.0]);
      leSlave9Freq.Text := format('%1.2f', [CD_70.ENG_SlaveFrequency[8] / 100.0]);
      leSlave10Freq.Text := format('%1.2f', [CD_70.ENG_SlaveFrequency[9] / 100.0]);

      leSlave1Index.Text := format('%1.2f', [CD_70.ENG_SlaveIndex[0] / 100.0]);
      leSlave2Index.Text := format('%1.2f', [CD_70.ENG_SlaveIndex[1] / 100.0]);
      leSlave3Index.Text := format('%1.2f', [CD_70.ENG_SlaveIndex[2] / 100.0]);
      leSlave4Index.Text := format('%1.2f', [CD_70.ENG_SlaveIndex[3] / 100.0]);
      leSlave5Index.Text := format('%1.2f', [CD_70.ENG_SlaveIndex[4] / 100.0]);
      leSlave6Index.Text := format('%1.2f', [CD_70.ENG_SlaveIndex[5] / 100.0]);
      leSlave6Index.Text := format('%1.2f', [CD_70.ENG_SlaveIndex[5] / 100.0]);
      leSlave7Index.Text := format('%1.2f', [CD_70.ENG_SlaveIndex[6] / 100.0]);
      leSlave8Index.Text := format('%1.2f', [CD_70.ENG_SlaveIndex[7] / 100.0]);
      leSlave9Index.Text := format('%1.2f', [CD_70.ENG_SlaveIndex[8] / 100.0]);
      leSlave10Index.Text := format('%1.2f', [CD_70.ENG_SlaveIndex[9] / 100.0]);

      lePitchAngle.Text := format('%1.2f', [CD_70.PitchAngle / 100.0]);
      lePitchKFactor.Text := format('%1.3f', [CD_70.PitchKFactor / 1000.0]);

      leKmaPercent.Text := format('%1.3f', [CD_70.ENG_KmaPercentage / 1000.0]);
      leKmaDeg.Text := format('%1.2f', [CD_70.ENG_KmaAngle / 100.0]);
      leKmaIndex.Text := format('%1.2f', [CD_70.ENG_KmaIndex / 100.0]);
      leKmaFreq1.Text := format('%1.2f', [CD_70.ENG_KmaFrequency / 100.0]);

      le33PwrRail5.Text := format('%1.0f', [CD_70.ENG_InternalReferenceCounts / 1.0]);
      le33PwrRail.Text := format('%1.3f', [CD_70.ENG_PowerRail33VDC / 1000.0]);
      leTempFreq.Text := format('%1.2f', [CD_70.ENG_TemperatureFrequency / 100.0]);

      TmpStr := 'Inactive ';
      if (CD_70.ENG_AlarmHoldStatus and 1) <> 0 then
        begin
        TmpStr := 'Active ';
        if (CD_70.ENG_AlarmHoldStatus and 2) <> 0 then
          TmpStr := TmpStr + 'High'
        else
          TmpStr := TmpStr + 'Low';
        end;
      leAlarm.Text := TmpStr;

      leMasterIndex1.Text := format('%1.2f', [CD_70.ENG_MasterIndex / 100.00]);
      leMasterIndex2.Text := format('%1.2f', [CD_70.ENG_FilteredMasterIndex / 100.0]);
      lePitchMasterIndex.Text := format('%1.2f', [CD_70.PitchCompensatedMasterIndex / 100.0]);
      leMasterFrequency.Text := format('%1.2f', [CD_70.ENG_MasterFrequency / 100.0]);

      leSetpointIndex.Text := format('%1.2f', [CD_70.ENG_SetpointIndex / 100.0]);
      lePwmDutyCycle.Text := format('%1.3f', [CD_70.ENG_PwmDutyCycle / 1000.0]);
      lePwmDutyCycle1.Text := format('%1.3f', [CD_70.ENG_PwmDutyCycleFineTune / 1000.0]);

      leSetpoint1.Text := format('%1.3f', [CD_70.ENG_Setpoint / 1000.0]);
      leFeedback1.Text := format('%1.3f', [CD_70.ENG_Feedback / 1000.0]);

      leRVOutputOhms.Text := format('%1.2f', [CD_70.ENG_RVOutputOhms / 100.0]);
      leMosfetPower.Text := format('%1.3f', [CD_70.ENG_MosfetPower / 1000.0]);

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

procedure TForm1.ProcessCDCommunications;
var
  TmpFloat: single;
  ManualSP, StrIndex: integer;

begin
  if (rgMode.ItemIndex = 0) or ((SetpointData.EnableBits and ENABLE_POWER) = 0) then
    begin
    CDCommState := 0;
    CDCommTimer := 20;
    CDFlashIndex := 0;
    bDownloadFirmware := true;
    bDownloadEventLog := false;
    bDownloadTiming := true;
    bDownloadStatus := true;
    CD_70_UpdateCounter := 0;
    exit;
    end;

  if CDCommTimer <> 0 then
    begin
    dec(CDCommTimer);
    exit;
    end;

  CDCommTimer := 50;
  case (CDCommState) of
  0: begin
  if bUploadCDFirmware then
      begin
      bUploadCDFirmware := false;
      bUploadCDFirmwareStarted := true;
      CDCommBuffer[0] := $CC;                     // Issue Reprogramming Command
      CDCommBuffer[1] := $12;
      CDCommBuffer[2] := $34;
      CDCommBuffer[3] := $56;
      CDCommBuffer[4] := $78;
      CDCommBufferLength := 5;
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
    else if bUploadPitchConfiguration  then
      begin
      bUploadPitchConfiguration := false;
      CDCommBuffer[0] := $13;                     // Write Pitch Configuration
      move(&CD_43_Upload, &CDCommBuffer[1], SizeOf(CD_43_Upload));
      CDCommBufferLength := SizeOf(CD_43_Upload) + 1;
      end
    else if bDownloadFactoryConfig then
      begin
      CDFlashIndex := 1;
      CDCommBuffer[0] := $42;                     // Read Factory Config
      CDCommBuffer[1] := 0;
      CDCommBufferLength := 2;
      CDCommState := 2;
      end
    else if bDownloadPitchConfig then
      begin
      CDFlashIndex := 1;
      CDCommBuffer[0] := $43;                     // Read Pitch Config
      CDCommBuffer[1] := 0;
      CDCommBufferLength := 2;
      CDCommState := 3;
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
      CDFlashIndex := 1;
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
    else if cbManual.Checked and (ManualDelay > 20) then
      begin
      ManualDelay := 0;
      CDCommBuffer[0] := $85;                     // Enter Manual Setpoint Mode
      StrIndex := 1;
      MyStrToFloat(edtManual.Text, StrIndex, TmpFloat);
      ManualSP := round(TmpFloat * 1000);
      move(&ManualSP, &CDCommBuffer[1], 4);
      CDCommBufferLength := 5;
      end
    else
      begin
      CDCommBuffer[0] := $70;                     // Read System Variables
      CDCommBufferLength := 1;
      if (CD_70_UpdateCounter > 0) then CD_70_UpdateCounter := CD_70_UpdateCounter - 1;
      ManualDelay := ManualDelay + 1;
      ExecutionTimer := ExecutionTimer + 1;
      StatusTimer := StatusTimer + 1;
      if ExecutionTimer > 5 then
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
    CDCommBuffer[0] := $43;                // Read Pitch Config
    CDCommBuffer[1] := CDFlashIndex;
    CDCommBufferLength := 2;
    inc(CDFlashIndex);
    bSerialRelayMessage := true;
    if CDFlashIndex = 4 then
      begin
      bDownloadPitchConfig := false;
      CDCommState := 0;
      end;
    end;
  4: begin
    CDCommBuffer[0] := $44;                   // Read Event Log
    CDCommBuffer[1] := CDFlashIndex and $FF;
    CDCommBuffer[2] := (CDFlashIndex shr 8) and $FF;
    CDCommBufferLength := 3;
    inc(CDFlashIndex);
    bSerialRelayMessage := true;
    if CDFlashIndex = 128 then
      begin
      bDownloadEventLog := false;
      CDCommState := 0;
      end;
    end;
  10: begin
      if bCDSendProgramingData then
        begin
        bCDSendProgramingData := false;
        if FirmwarePageNumber <> 99 then
          begin
          CDCommBuffer[0] := $EE;                     // Read EventLog
          move(&FirmwarePageNumber, &CDCommBuffer[1], 4);
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
  ProcessCDCommunications();

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

    if SetpointData.LED_TestMode <> 0 then
      begin
      SetpointData.DIGIO_OutputLevel[0] := cbDigOut1.ItemIndex;
      SetpointData.DIGIO_OutputLevel[1] := cbDigOut2.ItemIndex;
      SetpointData.DIGIO_OutputLevel[2] := cbDigOut3.ItemIndex;
      SetpointData.DIGIO_OutputLevel[3] := cbDigOut4.ItemIndex;
      SetpointData.DIGIO_OutputLevel[4] := cbDigOut5.ItemIndex;
      SetpointData.DIGIO_OutputLevel[5] := cbDigOut6.ItemIndex;
      end;

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

  FactoryData.CRC16 := CalcCrcArray(@FactoryData, 2044);

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
  LedCase, LedCounter, LedBitCounter: integer;
  LedColor: dword;
begin
  if SetpointData.LED_TestMode = 0 then
    begin
    cbDigMode1.ItemIndex:= 1;
    cbDigMode2.ItemIndex:= 1;
    cbDigMode3.ItemIndex:= 1;
    cbDigMode4.ItemIndex:= 1;
    cbDigMode5.ItemIndex:= 1;
    cbDigMode6.ItemIndex:= 1;
    SetpointData.DIGIO_OutputLevel[0] := 0;  // Clk
    SetpointData.DIGIO_OutputLevel[1] := 0;  // DAta1
    SetpointData.DIGIO_OutputLevel[2] := 0;  // DAta2
    SetpointData.DIGIO_OutputLevel[3] := 0;  // DAta3
    SetpointData.DIGIO_OutputLevel[4] := 0;  // DAta4
    SetpointData.DIGIO_OutputLevel[5] := 0;  // DAta5
    SetpointData.LED_TestMode := 1;
    end
  else
      begin
      SetpointData.LED_TestMode := 0;
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
  dwFileSize: dword;
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
  FillChar(FirmwareBinaryBuffer, $20000, $FF);
  BlockRead(F1, FirmwareBinaryBuffer, dwFileSize);
  CloseFile(F1);
  FirmwarePageNumber := 0;
//  FirmwarePageSize := (dwFileSize div 2048);
//  if (dwFileSize mod 2048) <> 0 then FirmwarePageSize := FirmwarePageSize + 1;
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
            WaitTime := 2000;
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
              WaitTime := 2000000;
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
              end;
            goto ExitNow;
            end;
          end;
        end;

      ExitNow:
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
  cbUploadConfigDig1.ItemIndex := CDProfileData.GuiConfigData.DIG0;
  cbUploadConfigDig2.ItemIndex := CDProfileData.GuiConfigData.DIG1;
  cbUploadConfigDig3.ItemIndex := CDProfileData.GuiConfigData.DIG2;
  cbUploadConfigDig4.ItemIndex := CDProfileData.GuiConfigData.DIG3;
  cbUploadConfigDig5.ItemIndex := CDProfileData.GuiConfigData.DIG4;
  cbUploadConfigDig6.ItemIndex := CDProfileData.GuiConfigData.DIG5;
  cbUploadConfigReverseKMA.Checked := CDProfileData.GuiConfigData.KMAReverseFlag;
  cbUploadConfigReverseSlave1.Checked := CDProfileData.GuiConfigData.Slave1ReverseFlag;
  cbUploadConfigReverseSlave2.Checked := CDProfileData.GuiConfigData.Slave2ReverseFlag;
  cbUploadConfigReverseSlave3.Checked := CDProfileData.GuiConfigData.Slave3ReverseFlag;
  cbUploadConfigReverseSlave4.Checked := CDProfileData.GuiConfigData.Slave4ReverseFlag;
  cbUploadConfigReverseSlave5.Checked := CDProfileData.GuiConfigData.Slave5ReverseFlag;
  cbUploadConfigReverseSlave6.Checked := CDProfileData.GuiConfigData.Slave6ReverseFlag;
  cbUploadConfigReverseSlave7.Checked := CDProfileData.GuiConfigData.Slave7ReverseFlag;
  cbUploadConfigReverseSlave8.Checked := CDProfileData.GuiConfigData.Slave8ReverseFlag;
  cbUploadConfigReverseSlave9.Checked := CDProfileData.GuiConfigData.Slave9ReverseFlag;
  cbUploadConfigReverseSlave10.Checked := CDProfileData.GuiConfigData.Slave10ReverseFlag;
  cbUploadConfigLowFuelOutputLevel.ItemIndex := CDProfileData.GuiConfigData.LowFuelLevelOutputLevel;
  cbUploadConfigErrorActiveLevel.ItemIndex := CDProfileData.GuiConfigData.ErrorOutputActiveLevel;
  edtUploadConfigLowFuelTripPoint.Text := format('%1.2f', [CDProfileData.GuiConfigData.LowFuelLevelTripPoint / 100.0]);
  cbUploadConfigLowFuelInvert.Checked := CDProfileData.GuiConfigData.LowFuelLevelInvertFlag;
  edtUploadConfigKmaGain.Text := format('%d', [CDProfileData.GuiConfigData.KmaFilterGain]);
  edtUploadConfigMasterGain.Text := format('%d', [CDProfileData.GuiConfigData.MasterIndexFilteringGain]);
  edtUploadConfigAccelXGain.Text := format('%d', [CDProfileData.GuiConfigData.AccelVoltageXAdcFilteringGain]);
  edtUploadConfigAccelYGain.Text := format('%d', [CDProfileData.GuiConfigData.AccelVoltageYAdcFilteringGain]);
  edtUploadConfigAccelZGain.Text := format('%d', [CDProfileData.GuiConfigData.AccelVoltageZAdcFilteringGain]);
  cbUploadConfigRS485Format.ItemIndex := CDProfileData.GuiConfigData.RS485_MessageMode;
  edtUploadConfigRS485Time.Text := format('%1.1f', [CDProfileData.GuiConfigData.RS485_MessageTime / 10.0]);
  cbUploadConfigOutputMode.ItemIndex := CDProfileData.GuiConfigData.RVOutputMode;
  cbUploadConfigPullUpMode.ItemIndex := CDProfileData.GuiConfigData.RVPullupMode;
  cbUploadConfigErrorRV.ItemIndex := CDProfileData.GuiConfigData.RVOutputErrorMode;
  edtUploadConfigOut1.Text := format('%1.3f', [CDProfileData.GuiConfigData.ErrorRVOutput1 / 1000.0]);
  edtUploadConfigOut2.Text := format('%1.3f', [CDProfileData.GuiConfigData.ErrorRVOutput2 / 1000.0]);
  edtUploadConfigRVToggleTime.Text := format('%1.1f', [CDProfileData.GuiConfigData.ErrorRvOutputToggleTime / 10.0]);
  cbUploadConfigMasterMode.ItemIndex := CDProfileData.GuiConfigData.MasterFreqMode;
  cbUploadConfigMasterError.ItemIndex := CDProfileData.GuiConfigData.MasterFreqErrorMode;
  edtUploadConfigFreq1.Text := format('%1.2f', [CDProfileData.GuiConfigData.ErrorMasterFreq1 / 100.0]);
  edtUploadConfigFreq2.Text := format('%1.2f', [CDProfileData.GuiConfigData.ErrorMasterFreq2 / 100.0]);
  edtUploadConfigMasterToggleTime.Text := format('%1.1f', [CDProfileData.GuiConfigData.ErrorMasterToggleTime / 10.0]);
  edtUploadConfigRVsetpoint.Text := CDProfileData.GetRVSetpointTable();
  edtUploadConfigMasterSetpoint.Text := CDProfileData.GetMasterFreqSetpointTable;
  cbUploadPitchControlAxis.ItemIndex := CDProfileData.GuiConfigData.PitchAxis;
  edtUploadPitchAxisOffset.Text :=  format('%1.4f', [CDProfileData.GuiConfigData.PitchAxisOffset / 10000.0]);
  edtUploadPitchCalibration.Text := CDProfileData.GetPitchCalibration();
  edtUploadPitchAngles.Text := CDProfileData.GetPitchAngles;
  edtUploadPitchIndexes.Text := CDProfileData.GetPitchIndexes;
  for i := 0 to 15 do
    begin
    CDProfileData.SetPitchAngleLabel(i, PitchKFactorAngleLabel[i]);
    CDProfileData.SetPitchIndexLabel(i, PitchKFactorIndexLabel[i]);
    for j := 0 to 15 do
      begin
      CDProfileData.SetPitchKFactorEdit(i, j, PitchKFactorEdit[i,j]);
      end;
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
  UpdateAngleCalibrationProfile();
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
  ProfileStrValue[ProfileStrIndex] :=  format('%u', [CDProfileData.GuiConfigData.DIG0]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'DIGIO2';
  ProfileStrValue[ProfileStrIndex] :=  format('%u', [CDProfileData.GuiConfigData.DIG1]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'DIGIO3';
  ProfileStrValue[ProfileStrIndex] :=  format('%u', [CDProfileData.GuiConfigData.DIG2]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'DIGIO4';
  ProfileStrValue[ProfileStrIndex] :=  format('%u', [CDProfileData.GuiConfigData.DIG3]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'DIGIO5';
  ProfileStrValue[ProfileStrIndex] :=  format('%u', [CDProfileData.GuiConfigData.DIG4]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'DIGIO6';
  ProfileStrValue[ProfileStrIndex] :=  format('%u', [CDProfileData.GuiConfigData.DIG5]);
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

  ConfigFlags := 0;
  if (CDProfileData.GuiConfigData.KMAReverseFlag) then ConfigFlags := 1;
  ProfileStrKey[ProfileStrIndex] := 'KMA_REV';
  ProfileStrValue[ProfileStrIndex] := format('%u', [ConfigFlags]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ConfigFlags := 0;
  if (CDProfileData.GuiConfigData.Slave1ReverseFlag) then ConfigFlags := 1;
  ProfileStrKey[ProfileStrIndex] := 'SLAVE1_REV';
  ProfileStrValue[ProfileStrIndex] := format('%u', [ConfigFlags]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ConfigFlags := 0;
  if (CDProfileData.GuiConfigData.Slave2ReverseFlag) then ConfigFlags := 1;
  ProfileStrKey[ProfileStrIndex] := 'SLAVE2_REV';
  ProfileStrValue[ProfileStrIndex] := format('%u', [ConfigFlags]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ConfigFlags := 0;
  if (CDProfileData.GuiConfigData.Slave3ReverseFlag) then ConfigFlags := 1;
  ProfileStrKey[ProfileStrIndex] := 'SLAVE3_REV';
  ProfileStrValue[ProfileStrIndex] := format('%u', [ConfigFlags]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ConfigFlags := 0;
  if (CDProfileData.GuiConfigData.Slave4ReverseFlag) then ConfigFlags := 1;
  ProfileStrKey[ProfileStrIndex] := 'SLAVE4_REV';
  ProfileStrValue[ProfileStrIndex] := format('%u', [ConfigFlags]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ConfigFlags := 0;
  if (CDProfileData.GuiConfigData.Slave5ReverseFlag) then ConfigFlags := 1;
  ProfileStrKey[ProfileStrIndex] := 'SLAVE5_REV';
  ProfileStrValue[ProfileStrIndex] := format('%u', [ConfigFlags]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ConfigFlags := 0;
  if (CDProfileData.GuiConfigData.Slave6ReverseFlag) then ConfigFlags := 1;
  ProfileStrKey[ProfileStrIndex] := 'SLAVE6_REV';
  ProfileStrValue[ProfileStrIndex] := format('%u', [ConfigFlags]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ConfigFlags := 0;
  if (CDProfileData.GuiConfigData.Slave7ReverseFlag) then ConfigFlags := 1;
  ProfileStrKey[ProfileStrIndex] := 'SLAVE7_REV';
  ProfileStrValue[ProfileStrIndex] := format('%u', [ConfigFlags]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ConfigFlags := 0;
  if (CDProfileData.GuiConfigData.Slave8ReverseFlag) then ConfigFlags := 1;
  ProfileStrKey[ProfileStrIndex] := 'SLAVE8_REV';
  ProfileStrValue[ProfileStrIndex] := format('%u', [ConfigFlags]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ConfigFlags := 0;
  if (CDProfileData.GuiConfigData.Slave9ReverseFlag) then ConfigFlags := 1;
  ProfileStrKey[ProfileStrIndex] := 'SLAVE9_REV';
  ProfileStrValue[ProfileStrIndex] := format('%u', [ConfigFlags]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ConfigFlags := 0;
  if (CDProfileData.GuiConfigData.Slave10ReverseFlag) then ConfigFlags := 1;
  ProfileStrKey[ProfileStrIndex] := 'SLAVE10_REV';
  ProfileStrValue[ProfileStrIndex] := format('%u', [ConfigFlags]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'LOW_FUEL_ACTIVE_LEVEL';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.LowFuelLevelOutputLevel]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'LOW_FUEL_TRIPPOINT';
  ProfileStrValue[ProfileStrIndex] := format('%1.2f', [CDProfileData.GuiConfigData.LowFuelLevelTripPoint / 100.0]);
  ProfileStrIndex := ProfileStrIndex + 1;

  AlarmMode := 0;
  if (CDProfileData.GuiConfigData.LowFuelLevelInvertFlag) then AlarmMode := 1;
  ProfileStrKey[ProfileStrIndex] := 'LOW_FUEL_SOURCE_INVERT';
  ProfileStrValue[ProfileStrIndex] := format('%u', [AlarmMode]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'ERROR_OUTPUT_ACTIVE_LEVEL';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.ErrorOutputActiveLevel]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'RV_OUTPUT_MODE';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.RVOutputMode]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'RV_OUTPUT_PULLUP';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.RVPullupMode]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'RV_OUTPUT_ERROR_MODE';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.RVOutputErrorMode]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'RV_OUTPUT_ERROR_OUT1';
  ProfileStrValue[ProfileStrIndex] := format('%1.3f', [CDProfileData.GuiConfigData.ErrorRVOutput1 / 1000.0]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'RV_OUTPUT_ERROR_OUT2';
  ProfileStrValue[ProfileStrIndex] := format('%1.3f', [CDProfileData.GuiConfigData.ErrorRVOutput2 / 1000.0]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'RV_OUTPUT_ERROR_TOGGLE_TIME';
  ProfileStrValue[ProfileStrIndex] := format('%1.1f', [CDProfileData.GuiConfigData.ErrorRvOutputToggleTime / 10.0]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'MASTER_FREQ_MODE';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.MasterFreqMode]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'MASTER_FREQ_ERROR_MODE';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.MasterFreqErrorMode]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'MASTER_FREQ_ERROR_OUT1';
  ProfileStrValue[ProfileStrIndex] := format('%1.2f', [CDProfileData.GuiConfigData.ErrorMasterFreq1 / 100.0]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'MASTER_FREQ_ERROR_OUT2';
  ProfileStrValue[ProfileStrIndex] := format('%1.2f', [CDProfileData.GuiConfigData.ErrorMasterFreq2 / 100.0]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'MASTER_FREQ_ERROR_TOGGLE_TIME';
  ProfileStrValue[ProfileStrIndex] := format('%1.1f', [CDProfileData.GuiConfigData.ErrorMasterToggleTime / 10.0]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'RS485_MODE';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.RS485_MessageMode]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'RS485_TIME';
  ProfileStrValue[ProfileStrIndex] := format('%1.1f', [CDProfileData.GuiConfigData.RS485_MessageTime / 10.0]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'MASTER_FREQUENCY_TABLE';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetMasterFreqSetpointTable]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'RV_SETPOINT_TABLE';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetRVSetpointTable]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'PITCH_CONTROL_AXIS';
  ProfileStrValue[ProfileStrIndex] := format('%u', [CDProfileData.GuiConfigData.PitchAxis]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'PITCH_AXIS_OFFSET';
  ProfileStrValue[ProfileStrIndex] := format('%1.4f', [CDProfileData.GuiConfigData.PitchAxisOffset / 10000.0]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'PITCH_G_ANGLE_CALIBRATION';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetPitchCalibration]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'PITCH_ANGLE_TABLE';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetPitchAngles]);
  ProfileStrIndex := ProfileStrIndex + 1;

  ProfileStrKey[ProfileStrIndex] := 'PITCH_INDEX_TABLE';
  ProfileStrValue[ProfileStrIndex] := format('%s', [CDProfileData.GetPitchIndexes]);
  ProfileStrIndex := ProfileStrIndex + 1;

  for i := 0 to 15 do
    begin
    TmpStr := '';
    for j := 0 to 15 do
      begin
      if (j = CDProfileData.GuiConfigData.PitchIndexTable.TableLength) or (i >= CDProfileData.GuiConfigData.PitchAngleTable.TableLength) then break;
      if j <> 0 then TmpStr := TmpStr + ',';
      TmpStr := TmpStr + format('%1.3f', [CDProfileData.GuiConfigData.PitchKFactorTable[i,j] / 1000.0]);
      end;
    ProfileStrKey[ProfileStrIndex] := format('PITCH_K_FACTOR_%d', [i]);
    ProfileStrValue[ProfileStrIndex] := format('%s', [TmpStr]);
    ProfileStrIndex := ProfileStrIndex + 1;
    end;

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
  bDownloadEventLog := true;
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

//******************************************************************************************************************
//
//
procedure TForm1.LoadFromGui;
begin
  CDProfileData.GuiConfigData.ProfileName := cbCDProfileNames.Text;
  CDProfileData.GuiConfigData.DIG0 := cbUploadConfigDig1.ItemIndex;
  CDProfileData.GuiConfigData.DIG1 := cbUploadConfigDig2.ItemIndex;
  CDProfileData.GuiConfigData.DIG2 := cbUploadConfigDig3.ItemIndex;
  CDProfileData.GuiConfigData.DIG3 := cbUploadConfigDig4.ItemIndex;
  CDProfileData.GuiConfigData.DIG4 := cbUploadConfigDig5.ItemIndex;
  CDProfileData.GuiConfigData.DIG5 := cbUploadConfigDig6.ItemIndex;
  CDProfileData.GuiConfigData.KmaFilterGain := ConvertToFloat(edtUploadConfigKmaGain.Text, 1.0);
  CDProfileData.GuiConfigData.MasterIndexFilteringGain := ConvertToFloat(edtUploadConfigMasterGain.Text, 1.0);
  CDProfileData.GuiConfigData.AccelVoltageXAdcFilteringGain := ConvertToFloat(edtUploadConfigAccelXGain.Text, 1.0);
  CDProfileData.GuiConfigData.AccelVoltageYAdcFilteringGain := ConvertToFloat(edtUploadConfigAccelYGain.Text, 1.0);
  CDProfileData.GuiConfigData.AccelVoltageZAdcFilteringGain := ConvertToFloat(edtUploadConfigAccelZGain.Text, 1.0);
  CDProfileData.GuiConfigData.KMAReverseFlag := cbUploadConfigReverseKma.Checked;
  CDProfileData.GuiConfigData.Slave1ReverseFlag := cbUploadConfigReverseSlave1.Checked;
  CDProfileData.GuiConfigData.Slave2ReverseFlag := cbUploadConfigReverseSlave2.Checked;
  CDProfileData.GuiConfigData.Slave3ReverseFlag := cbUploadConfigReverseSlave3.Checked;
  CDProfileData.GuiConfigData.Slave4ReverseFlag := cbUploadConfigReverseSlave4.Checked;
  CDProfileData.GuiConfigData.Slave5ReverseFlag := cbUploadConfigReverseSlave5.Checked;
  CDProfileData.GuiConfigData.Slave6ReverseFlag := cbUploadConfigReverseSlave6.Checked;
  CDProfileData.GuiConfigData.Slave7ReverseFlag := cbUploadConfigReverseSlave7.Checked;
  CDProfileData.GuiConfigData.Slave8ReverseFlag := cbUploadConfigReverseSlave8.Checked;
  CDProfileData.GuiConfigData.Slave9ReverseFlag := cbUploadConfigReverseSlave9.Checked;
  CDProfileData.GuiConfigData.Slave10ReverseFlag := cbUploadConfigReverseSlave10.Checked;
  CDProfileData.GuiConfigData.LowFuelLevelOutputLevel := cbUploadConfigLowFuelOutputLevel.ItemIndex;
  CDProfileData.GuiConfigData.LowFuelLevelInvertFlag := cbUploadConfigLowFuelInvert.Checked;
  CDProfileData.GuiConfigData.LowFuelLevelTripPoint := ConvertToFloat(edtUploadConfigLowFuelTripPoint.Text, 100.0);
  CDProfileData.GuiConfigData.ErrorOutputActiveLevel := cbUploadConfigErrorActiveLevel.ItemIndex;
  CDProfileData.GuiConfigData.RVOutputMode := cbUploadConfigOutputMode.ItemIndex;
  CDProfileData.GuiConfigData.RVPullupMode := cbUploadConfigPullUpMode.ItemIndex;
  CDProfileData.GuiConfigData.RVOutputErrorMode := cbUploadConfigErrorRV.ItemIndex;
  CDProfileData.GuiConfigData.ErrorRVOutput1 := ConvertToFloat(edtUploadConfigOut1.Text, 1000.0);
  CDProfileData.GuiConfigData.ErrorRVOutput2 := ConvertToFloat(edtUploadConfigOut2.Text, 1000.0);
  CDProfileData.GuiConfigData.ErrorRvOutputToggleTime := ConvertToFloat(edtUploadConfigRVToggleTime.Text, 10.0);
  CDProfileData.GuiConfigData.MasterFreqMode := cbUploadConfigMasterMode.ItemIndex;
  CDProfileData.GuiConfigData.MasterFreqErrorMode := cbUploadConfigMasterError.ItemIndex;
  CDProfileData.GuiConfigData.ErrorMasterFreq1 := ConvertToFloat(edtUploadConfigFreq1.Text, 100.0);
  CDProfileData.GuiConfigData.ErrorMasterFreq2 := ConvertToFloat(edtUploadConfigFreq2.Text, 100.0);
  CDProfileData.GuiConfigData.ErrorMasterToggleTime := ConvertToFloat(edtUploadConfigMasterToggleTime.Text, 10.0);
  CDProfileData.GuiConfigData.RS485_MessageMode := cbUploadConfigRS485Format.ItemIndex;
  CDProfileData.GuiConfigData.RS485_MessageTime := ConvertToFloat(edtUploadConfigRS485Time.Text, 10.0);
  CDProfileData.GuiConfigData.PitchAxis := cbUploadPitchControlAxis.ItemIndex;
  CDProfileData.GuiConfigData.PitchAxisOffset := ConvertToFloat(edtUploadPitchAxisOffset.Text, 10000.0);
  CDProfileData.SetPitchCalibration(edtUploadPitchCalibration.Text);
  CDProfileData.SetPitchAngles(edtUploadPitchAngles.Text);
  CDProfileData.SetPitchIndexes(edtUploadPitchIndexes.Text);
  CDProfileData.SetPitchTable();

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
  UpdateAngleCalibrationProfile();

end;

//******************************************************************************************************************
//
//
procedure TForm1.LoadFactoryFromGui;
begin
  CDProfileData.GuiCDFactoryData.Hardware := cbPcbRevision.ItemIndex - 1;
  if cbAccelerometer.Checked then
     CDProfileData.GuiCDFactoryData.Hardware := CDProfileData.GuiCDFactoryData.Hardware or $10;
  if cbRVOutput.Checked then
     CDProfileData.GuiCDFactoryData.Hardware := CDProfileData.GuiCDFactoryData.Hardware or $20;
  if cbRS485Driver.Checked then
     CDProfileData.GuiCDFactoryData.Hardware := CDProfileData.GuiCDFactoryData.Hardware or $40;
  if cbCANBusDriver.Checked then
     CDProfileData.GuiCDFactoryData.Hardware := CDProfileData.GuiCDFactoryData.Hardware or $80;
  if cbTemperatureSensor.Checked then
     CDProfileData.GuiCDFactoryData.Hardware := CDProfileData.GuiCDFactoryData.Hardware or $100;

  CDProfileData.GuiCDFactoryData.Calibration33 := ConvertToFloat(edtConfig33Calibration.Text, 1000.0);
  CDProfileData.GuiCDFactoryData.PowerRail5KFactor := ConvertToFloat(edtConfig5VDCCalibration.Text, 1.0);
  CDProfileData.GuiCDFactoryData.KmaKFactor := ConvertToFloat(edtConfigKmaCalibration.Text, 1.0);
  CDProfileData.GuiCDFactoryData.PowerInputKFactor := ConvertToFloat(edtConfigPowerInputVoltageCalibration.Text, 1.0);
  CDProfileData.GuiCDFactoryData.RVOutputVoltageKFactor := ConvertToFloat(edtConfigRVVoltageCalibration.Text, 1.0);
  CDProfileData.GuiCDFactoryData.RVOutputCurrentKFactor := ConvertToFloat(edtConfigRVCurrentScaleCalibration.Text, 1.0);
  CDProfileData.GuiCDFactoryData.PwmOutputKFactor := ConvertToFloat(edtConfigPwmOutput.Text, 1.0);

  CDProfileData.GuiCDFactoryData.TimeStr := edtConfigManufacturingTime.Text;
  CDProfileData.GuiCDFactoryData.DateStr := edtConfigManufacturingDate.Text;
  CDProfileData.GuiCDFactoryData.NameTesterStr := edtConfigManufacturingNameTester.Text;
  CDProfileData.GuiCDFactoryData.ModelNumberStr := edtConfigManufacturingModelNumber.Text;
  CDProfileData.GuiCDFactoryData.SerialNumberStr := edtConfigManufacturingSerialNumber.Text;
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
  //
  // Copy CDProfileData.GUIConfigData values into CD_41_Upload  and CD_43_Upload
  //
  CDProfileData.UpdateCD_41();
  CDProfileData.UpdateCD_43();

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

          EventLogger.WriteStr('Uploading Pitch Configuration Data', True);
          TestMessage('Uploading Pitch Configuration Data', 0);
          bUploadPitchConfiguration := true;
          WaitTime := 2000;
          TestCase := 500;
          goto ExitToTop;
          end;

        500:
          begin
          if WaitTime = 0 then
            begin
            EventLogger.WriteStr('ERROR:  Unable to Upload Pitch Configuration Data', True);
            TestMessage('ERROR:  Unable to Upload Pitch Configuration Data', 0);
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
          TestMessage('Verify Flash Config and Pitch Config Data', 0);
          bDownloadFlashConfig := true;
          TestMessage('Download Pitch Configuration Data', 0);
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
          TestMessage('Download Pitch Configuration Data', 0);
          bDownloadDone := false;
          bDownloadPitchConfig := true;
          WaitTime := 2000;
          TestCase := 900;
          goto ExitToTop;
          end;
        900:
          begin
          if WaitTime = 0 then
            begin
            TestMessage('ERROR:  Unable to Download Pitch Configuration Data', 0);
            TestCase := 10000;
            goto ExitToTop;
            end;
          if not bDownloadDone then
            begin
            goto ExitToTop;
            end;
          TestMessage('Download Done..', 0);
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

procedure TForm1.cbUploadConfigDig1Change(Sender: TObject);
begin
  if (cbUploadConfigDig1.ItemIndex <> 8) and (cbUploadConfigDig2.ItemIndex = 7) then cbUploadConfigDig2.ItemIndex := 0;
  if (cbUploadConfigDig1.ItemIndex <> 9) and (cbUploadConfigDig2.ItemIndex = 8) then cbUploadConfigDig2.ItemIndex := 0;
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

//**************************************************************************************************************************************

procedure TForm1.SyncControls;
var
   ParseStatus, i, j: integer;
   FirstValue: single;
   ParseResults: stParseResults;
   FieldResults: stFieldResults;
begin

  LoadFromGui;

  btnUploadProfile.Enabled := cbCDProfileNames.Text <> '';

  if cbUploadPitchControlAxis.ItemIndex = 0 then
    begin
    edtUploadPitchAxisOffset.Visible := false;
    edtUploadPitchAxisOffset.Text := '0.0000';
    Label239.Visible := false;
    GroupBox15.Visible := false;
    edtUploadPitchCalibration.Text := '';
    edtUploadPitchAngles.Text := '';
    edtUploadPitchIndexes.Text := '';
    for i := 0 to 15 do
      begin
      for j := 0 to 15 do
        begin
        PitchKFactorEdit[i,j].Text := '1.00';
        end;
      end;
    GroupBox16.Visible := false;
    shtAngleCalibration.TabVisible := false;
    end;
  if cbUploadPitchControlAxis.ItemIndex > 0 then
    begin
    edtUploadPitchAxisOffset.Visible := true;
    Label239.Visible := true;
    GroupBox15.Visible := true;
    GroupBox16.Visible := true;
    shtAngleCalibration.TabVisible := true;
    end;
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

    cbUploadConfigErrorRV.ItemIndex := 0;
    cbUploadConfigErrorRV.Visible := false;
    Label225.Visible := false;

    shtRVProfile2.TabVisible := false;
    end;
  if cbUploadConfigOutputMode.ItemIndex = 1 then
    begin

    cbUploadConfigPullUpMode.Visible := true;
    Label224.Visible := true;

    cbUploadConfigErrorRV.Visible := true;
    Label225.Visible := true;

    shtRVProfile2.TabVisible := true;

    end;
  if cbUploadConfigOutputMode.ItemIndex = 2 then
    begin
    CDProfileData.GuiConfigData.RVOutputMode := 2;

    cbUploadConfigPullUpMode.Visible := true;
    Label224.Visible := true;

    cbUploadConfigErrorRV.Visible := true;
    Label225.Visible := true;

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
  if (cbUploadConfigMasterError.ItemIndex = 0) then
    begin
    edtUploadConfigFreq1.Visible := false;
    edtUploadConfigFreq1.Text := '0.0';
    Label232.Visible := false;

    edtUploadConfigFreq2.Visible := false;
    edtUploadConfigFreq2.Text := '0.0';
    Label233.Visible := false;

    edtUploadConfigMasterToggleTime.Visible := false;
    edtUploadConfigMasterToggleTime.Text := '0.0';
    Label234.Visible := false;
    end;
  if (cbUploadConfigMasterError.ItemIndex = 1) then
    begin
    edtUploadConfigFreq1.Visible := true;
    Label232.Visible := true;

    edtUploadConfigFreq2.Visible := false;
    edtUploadConfigFreq2.Text := '0.0';
    Label233.Visible := false;

    edtUploadConfigMasterToggleTime.Visible := false;
    edtUploadConfigMasterToggleTime.Text := '0.0';
    Label234.Visible := false;
    end;
  if (cbUploadConfigMasterError.ItemIndex = 2) then
    begin
    edtUploadConfigFreq1.Visible := true;
    Label232.Visible := true;

    edtUploadConfigFreq2.Visible := true;
    Label233.Visible := true;

    edtUploadConfigMasterToggleTime.Visible := true;
    Label234.Visible := true;
    end;
  if (cbUploadConfigErrorRV.ItemIndex = 0) then
    begin
    edtUploadConfigOut1.Text := '0.0';
    edtUploadConfigOut1.Visible := false;
    Label226.Visible := false;

    edtUploadConfigOut2.Text := '0.0';
    edtUploadConfigOut2.Visible := false;
    Label227.Visible := false;

    edtUploadConfigRVToggleTime.Text := '0.0';
    edtUploadConfigRVToggleTime.Visible := false;
    Label228.Visible := false;
    end;
  if (cbUploadConfigErrorRV.ItemIndex = 1) then
    begin
    edtUploadConfigOut1.Visible := true;
    Label226.Visible := true;

    edtUploadConfigOut2.Text := '0.0';
    edtUploadConfigOut2.Visible := false;
    Label227.Visible := false;

    edtUploadConfigRVToggleTime.Text := '0.0';
    edtUploadConfigRVToggleTime.Visible := false;
    Label228.Visible := false;
    end;
  if (cbUploadConfigErrorRV.ItemIndex = 2) then
    begin
    edtUploadConfigOut1.Visible := true;
    Label226.Visible := true;

    edtUploadConfigOut2.Visible := true;
    Label227.Visible := true;

    edtUploadConfigRVToggleTime.Visible := true;
    Label228.Visible := true;
    end;

  ParseStatus := ParseRecords(edtUploadPitchCalibration.Text, ParseResults);
  if (ParseStatus = 0) and (ParseResults.ParseLength < 2) then ParseStatus := 100;
  if (ParseStatus = 0) and (ParseResults.ParseLength > 32) then ParseStatus := 101;
  case (ParseStatus) of
      0: Label251.Caption := '';
      -1: Label251.Caption := 'Missing Left Bracket';
      -2: Label251.Caption := 'Missing Right Bracket';
      -3: Label251.Caption := 'Missing Comma';
      -4: Label251.Caption := 'Maximum number of pairs exceeeded';
      -5: Label251.Caption := 'Missing Value';
      100: Label251.Caption := 'Must be a minumim of two pairs';
      101: Label251.Caption := 'Maximum number of pairs exceeeded';
  end;

  CDProfileData.UpdatePitchTable;
  ParseStatus := ParseFields(edtUploadPitchAngles.Text, FieldResults);
  if (ParseStatus = 0) and (FieldResults.FieldCount < 2) then ParseStatus := 100;
  if (ParseStatus = 0) and (FieldResults.FieldCount > 16) then ParseStatus := 101;
  if (ParseStatus = 0) then
    begin
    FirstValue := 0;
    for i := 0 to FieldResults.FieldCount - 1 do
      begin
      if (FieldResults.FieldValue[i] < FirstValue) and (i > 0) then
        begin
        ParseStatus := 102;
        break;
        end;
      FirstValue := FieldResults.FieldValue[i];
      if FirstValue > 90 then
        begin
        ParseStatus := 103;
        break;
        end;
      if FirstValue < -90 then
        begin
        ParseStatus := 104;
        break;
        end;
      end;
    end;
  case (ParseStatus) of
      0: Label244.Caption := '';
      -3: Label244.Caption := 'Missing Comma';
      -4: Label244.Caption := 'Maximum number of pairs exceeeded';
      -5: Label244.Caption := 'Missing Value';
      100: Label244.Caption := 'Must be a minumim of two pairs';
      101: Label244.Caption := 'Maximum number of pairs exceeeded';
      102: Label244.Caption := 'Angles must get larger';
      103: Label244.Caption := 'Angle must be less than 90 degrees';
      104: Label244.Caption := 'Angle must be greater than 90 degrees';
  end;

  ParseStatus := ParseFields(edtUploadPitchIndexes.Text, FieldResults);
  if (ParseStatus = 0) and (FieldResults.FieldCount < 2) then ParseStatus := 100;
  if (ParseStatus = 0) and (FieldResults.FieldCount > 16) then ParseStatus := 101;
  if (ParseStatus = 0) then
    begin
    FirstValue := 0;
    for i := 0 to FieldResults.FieldCount - 1 do
      begin
      if (FieldResults.FieldValue[i] < FirstValue) and (i > 0) then
        begin
        ParseStatus := 102;
        break;
        end;
      FirstValue := FieldResults.FieldValue[i];
      if FirstValue > 1023 then
        begin
        ParseStatus := 103;
        break;
        end;
      if FirstValue < 4 then
        begin
        ParseStatus := 104;
        break;
        end;
      end;
    end;
  case (ParseStatus) of
      0: Label243.Caption := '';
      -3: Label243.Caption := 'Missing Comma';
      -4: Label243.Caption := 'Maximum number of pairs exceeeded';
      -5: Label243.Caption := 'Missing Value';
      100: Label243.Caption := 'Must be a minumim of two pairs';
      101: Label243.Caption := 'Maximum number of pairs exceeeded';
      102: Label243.Caption := 'Indexes must get larger';
      103: Label243.Caption := 'Index must be less than 1023';
      104: Label243.Caption := 'Index must be greater than 4';
  end;
end;

//**************************************************************************************************************************************

procedure TForm1.cbUploadPitchControlAxisChange(Sender: TObject);
begin
  if bLoading then exit;
  SyncControls();
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

//**************************************************************************************************************************************

procedure TForm1.edtUploadPitchAnglesChange(Sender: TObject);
begin
  if bLoading then exit;
  SyncControls();
end;

procedure TForm1.edtUploadPitchCalibrationChange(Sender: TObject);
begin
  if bLoading then exit;
  SyncControls();
end;

//**************************************************************************************************************************************

procedure TForm1.edtUploadPitchIndexesChange(Sender: TObject);
begin
  if bLoading then exit;
  SyncControls();
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
            TestMessage('Download Pitch Configuration Data', 0);
            bDownloadDone := false;
            move(CDProfileData.CurrentConfigData, CDProfileData.GuiConfigData, sizeof(CDProfileData.GuiConfigData));
            UpdateGui();
            SyncControls();
            bDownloadPitchConfig := true;
            WaitTime := 2000;
            TestCase := 500;
            goto ExitToTop;
            end;

          500:
            begin
            if WaitTime = 0 then
              begin
              TestMessage('ERROR:  Unable to Download Pitch Configuration Data', 0);
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
    end
  else
    begin
    TabSheet8.TabVisible := true;
    TabSheet10.TabVisible := true;
    TabSheet1.TabVisible := false;
    TabSheet4.TabVisible := false;
    TabSheet5.TabVisible := false;
    end;

    TabSheet2.TabVisible := true;
    TabSheet7.TabVisible := true;

  Label1.Visible := true;
  Label2.Visible := true;
  edtNameofTester.Visible := true;
  edtSerialNumber.Visible := true;
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
    SetpointData.CommConfig := COMM_CD_RS485;
    PageControl1.TabIndex := 7;
    end;

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

//******************************************************************************************************
//******************************************************************************************************

procedure TForm1.UpdateAngleCalibrationProfile;
var
  Status, i: integer;
  CI, MaxSetpoint, MinSetpoint, SP: single;
begin
  AngleCalibrationProfileGraph.GraphData.NextIndex := 0;
  for i := 0 to CDProfileData.GuiConfigData.PitchAxisCalibration.TableLength - 1 do
    begin
    CI := CDProfileData.GuiConfigData.PitchAxisCalibration.TablePoint[i].Point1 / 10000.0;
    SP := CDProfileData.GuiConfigData.PitchAxisCalibration.TablePoint[i].Point2 / 100.0;
    AngleCalibrationProfileGraph.AddPoint(CI, SP);
    end;

  MaxSetpoint := CDProfileData.GuiConfigData.PitchAxisCalibration.TablePoint[0].Point2;
  MinSetpoint := CDProfileData.GuiConfigData.PitchAxisCalibration.TablePoint[0].Point2;
  for i := 0 to CDProfileData.GuiConfigData.PitchAxisCalibration.TableLength - 1 do
    begin
    if CDProfileData.GuiConfigData.PitchAxisCalibration.TablePoint[i].Point2 > MaxSetpoint then
      MaxSetpoint := CDProfileData.GuiConfigData.PitchAxisCalibration.TablePoint[i].Point2;
    if CDProfileData.GuiConfigData.PitchAxisCalibration.TablePoint[i].Point2 < MinSetpoint then
      MinSetpoint := CDProfileData.GuiConfigData.PitchAxisCalibration.TablePoint[i].Point2;
    end;
  AngleCalibrationProfileGraph.HorzHighScale := (MaxSetpoint + ((MaxSetpoint - MinSetpoint) * 0.05)) / 100.0;
  AngleCalibrationProfileGraph.HorzLowScale := (MinSetpoint - ((MaxSetpoint - MinSetpoint) * 0.05)) / 100.0;

  MaxSetpoint := CDProfileData.GuiConfigData.PitchAxisCalibration.TablePoint[0].Point1;
  MinSetpoint := CDProfileData.GuiConfigData.PitchAxisCalibration.TablePoint[0].Point1;
  for i := 0 to CDProfileData.GuiConfigData.PitchAxisCalibration.TableLength - 1 do
    begin
    if CDProfileData.GuiConfigData.PitchAxisCalibration.TablePoint[i].Point1 > MaxSetpoint then
      MaxSetpoint := CDProfileData.GuiConfigData.PitchAxisCalibration.TablePoint[i].Point1;
    if CDProfileData.GuiConfigData.PitchAxisCalibration.TablePoint[i].Point1 < MinSetpoint then
      MinSetpoint := CDProfileData.GuiConfigData.PitchAxisCalibration.TablePoint[i].Point1;
    end;
  AngleCalibrationProfileGraph.VertHighScale := (MaxSetpoint + ((MaxSetpoint - MinSetpoint) * 0.05)) / 10000.0;
  AngleCalibrationProfileGraph.VertLowScale := (MinSetpoint - ((MaxSetpoint - MinSetpoint) * 0.05)) / 10000.0;

  AngleCalibrationProfileGraph.Redraw;
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
    ScaleFactor := 1.0;
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

end;

//-------------------------------------------------------------------------------------

procedure stCDProfileData.LoadFromIniFile (ProfileName: string);
var
  IniFile: TIniFile;
  bLoadingSave: boolean;
begin
  bLoadingSave := bLoading;
  bLoading := true;
  IniFile := TIniFile.Create(AppPath + 'PROFILES_CD.INI');

  IniConfigData.ProfileName := ProfileName;

  SetIniData(IniFile, ProfileName, 'DIGIO1', 0, @CDProfileData.IniConfigData.DIG0, 1.0);
  SetIniData(IniFile, ProfileName, 'DIGIO2', 0, @CDProfileData.IniConfigData.DIG1, 1.0);
  SetIniData(IniFile, ProfileName, 'DIGIO3', 0, @CDProfileData.IniConfigData.DIG2, 1.0);
  SetIniData(IniFile, ProfileName, 'DIGIO4', 0, @CDProfileData.IniConfigData.DIG3, 1.0);
  SetIniData(IniFile, ProfileName, 'DIGIO5', 0, @CDProfileData.IniConfigData.DIG4, 1.0);
  SetIniData(IniFile, ProfileName, 'DIGIO6', 0, @CDProfileData.IniConfigData.DIG5, 1.0);
  SetIniData(IniFile, ProfileName, 'KMA_FILTER_GAIN', 0, @CDProfileData.IniConfigData.KmaFilterGain, 1.0);
  SetIniData(IniFile, ProfileName, 'MASTER_FILTER_GAIN', 0, @CDProfileData.IniConfigData.MasterIndexFilteringGain, 1.0);
  SetIniData(IniFile, ProfileName, 'ACCEL_X_FILTER_GAIN', 0, @CDProfileData.IniConfigData.AccelVoltageXAdcFilteringGain, 1.0);
  SetIniData(IniFile, ProfileName, 'ACCEL_Y_FILTER_GAIN', 0, @CDProfileData.IniConfigData.AccelVoltageYAdcFilteringGain, 1.0);
  SetIniData(IniFile, ProfileName, 'ACCEL_Z_FILTER_GAIN', 0, @CDProfileData.IniConfigData.AccelVoltageZAdcFilteringGain, 1.0);
  SetIniData(IniFile, ProfileName, 'KMA_REV', 7, @CDProfileData.IniConfigData.KMAReverseFlag, 1.0);
  SetIniData(IniFile, ProfileName, 'SLAVE1_REV', 7, @CDProfileData.IniConfigData.Slave1ReverseFlag, 1.0);
  SetIniData(IniFile, ProfileName, 'SLAVE2_REV', 7, @CDProfileData.IniConfigData.Slave2ReverseFlag, 1.0);
  SetIniData(IniFile, ProfileName, 'SLAVE3_REV', 7, @CDProfileData.IniConfigData.Slave3ReverseFlag, 1.0);
  SetIniData(IniFile, ProfileName, 'SLAVE4_REV', 7, @CDProfileData.IniConfigData.Slave4ReverseFlag, 1.0);
  SetIniData(IniFile, ProfileName, 'SLAVE5_REV', 7, @CDProfileData.IniConfigData.Slave5ReverseFlag, 1.0);
  SetIniData(IniFile, ProfileName, 'SLAVE6_REV', 7, @CDProfileData.IniConfigData.Slave6ReverseFlag, 1.0);
  SetIniData(IniFile, ProfileName, 'SLAVE7_REV', 7, @CDProfileData.IniConfigData.Slave7ReverseFlag, 1.0);
  SetIniData(IniFile, ProfileName, 'SLAVE8_REV', 7, @CDProfileData.IniConfigData.Slave8ReverseFlag, 1.0);
  SetIniData(IniFile, ProfileName, 'SLAVE9_REV', 7, @CDProfileData.IniConfigData.Slave9ReverseFlag, 1.0);
  SetIniData(IniFile, ProfileName, 'SLAVE10_REV', 7, @CDProfileData.IniConfigData.Slave10ReverseFlag, 1.0);
  SetIniData(IniFile, ProfileName, 'LOW_FUEL_ACTIVE_LEVEL', 0, @CDProfileData.IniConfigData.LowFuelLevelOutputLevel, 1.0);
  SetIniData(IniFile, ProfileName, 'LOW_FUEL_TRIPPOINT', 0, @CDProfileData.IniConfigData.LowFuelLevelTripPoint, 100.0);
  SetIniData(IniFile, ProfileName, 'LOW_FUEL_SOURCE_INVERT', 7, @CDProfileData.IniConfigData.LowFuelLevelInvertFlag, 1.0);
  SetIniData(IniFile, ProfileName, 'ERROR_OUTPUT_ACTIVE_LEVEL', 0, @CDProfileData.IniConfigData.ErrorOutputActiveLevel, 1.0);
  SetIniData(IniFile, ProfileName, 'MASTER_FREQ_ERROR_MODE', 0, @CDProfileData.IniConfigData.MasterFreqErrorMode, 1.0);
  SetIniData(IniFile, ProfileName, 'RV_OUTPUT_ERROR_MODE', 0, @CDProfileData.IniConfigData.RVOutputErrorMode, 1.0);
  SetIniData(IniFile, ProfileName, 'RV_OUTPUT_MODE', 0, @CDProfileData.IniConfigData.RVOutputMode, 1.0);
  SetIniData(IniFile, ProfileName, 'RV_OUTPUT_PULLUP', 0, @CDProfileData.IniConfigData.RVPullupMode, 1.0);
  SetIniData(IniFile, ProfileName, 'RV_OUTPUT_ERROR_OUT1', 1, @CDProfileData.IniConfigData.ErrorRVOutput1, 0.0);
  SetIniData(IniFile, ProfileName, 'RV_OUTPUT_ERROR_OUT2', 1, @CDProfileData.IniConfigData.ErrorRVOutput2, 0.0);
  SetIniData(IniFile, ProfileName, 'RV_OUTPUT_ERROR_TOGGLE_TIME', 0, @CDProfileData.IniConfigData.ErrorRvOutputToggleTime, 10.0);
  SetIniData(IniFile, ProfileName, 'MASTER_FREQ_MODE', 0, @CDProfileData.IniConfigData.MasterFreqMode, 1.0);
  SetIniData(IniFile, ProfileName, 'MASTER_FREQ_ERROR_OUT1', 0, @CDProfileData.IniConfigData.ErrorMasterFreq1, 100.0);
  SetIniData(IniFile, ProfileName, 'MASTER_FREQ_ERROR_OUT2', 0, @CDProfileData.IniConfigData.ErrorMasterFreq2, 100.0);
  SetIniData(IniFile, ProfileName, 'MASTER_FREQ_ERROR_TOGGLE_TIME', 0, @CDProfileData.IniConfigData.ErrorMasterToggleTime, 10.0);
  SetIniData(IniFile, ProfileName, 'RS485_MODE', 0, @CDProfileData.IniConfigData.RS485_MessageMode, 1.0);
  SetIniData(IniFile, ProfileName, 'RS485_TIME', 0, @CDProfileData.IniConfigData.RS485_MessageTime, 10.0);
  SetIniData(IniFile, ProfileName, 'RV_SETPOINT_TABLE', 2, @CDProfileData.IniConfigData.SetpointTable, 0.0);
  SetIniData(IniFile, ProfileName, 'MASTER_FREQUENCY_TABLE', 3, @CDProfileData.IniConfigData.MasterFreqTable, 0.0);
  SetIniData(IniFile, ProfileName, 'PITCH_CONTROL_AXIS', 0, @CDProfileData.IniConfigData.PitchAxis, 1.0);
  SetIniData(IniFile, ProfileName, 'PITCH_AXIS_OFFSET', 0, @CDProfileData.IniConfigData.PitchAxisOffset, 10000.0);
  SetIniData(IniFile, ProfileName, 'PITCH_G_ANGLE_CALIBRATION', 4, @CDProfileData.IniConfigData.PitchAxisCalibration, 0.0);
  SetIniData(IniFile, ProfileName, 'PITCH_ANGLE_TABLE', 5, @CDProfileData.IniConfigData.PitchAngleTable, 100.0);
  SetIniData(IniFile, ProfileName, 'PITCH_INDEX_TABLE', 5, @CDProfileData.IniConfigData.PitchIndexTable, 100.0);
  SetIniData(IniFile, ProfileName, 'PITCH_K_FACTOR', 6, @CDProfileData.IniConfigData.PitchKFactorTable, 1000.0);

  IniFile.Free;
  bLoading := bLoadingSave;
end;

//-------------------------------------------------------------------------------------

procedure stCDProfileData.UpdatePitchTable;
var
  i, j: integer;
begin
  for i := 0 to 15 do
    begin
    PitchAngleLabels^[i].Visible := GuiConfigData.PitchAngleTable.TableLength > i;
    PitchAngleLabels^[i].Caption := format('%1.2f', [GuiConfigData.PitchAngleTable.TablePoint[i].Point1 / 100.0]);
    PitchIndexLabels^[i].Visible := GuiConfigData.PitchIndexTable.TableLength > i;
    PitchIndexLabels^[i].Caption := format('%1.2f', [GuiConfigData.PitchIndexTable.TablePoint[i].Point1 / 100.0]);
    for j := 0 to 15 do
      begin
      PitchKFactorTable^[i,j].Visible := (GuiConfigData.PitchAngleTable.TableLength > i) and (GuiConfigData.PitchIndexTable.TableLength > j);
      PitchKFactorTable^[i,j].Text := format('%1.3f', [GuiConfigData.PitchKFactorTable[i, j] / 1000.0]);
      end;
    end;
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

function stCDProfileData.GetPitchCalibration : string;
var
  i: integer;
begin
  Result := '';
  for i := 0 to GuiConfigData.PitchAxisCalibration.TableLength - 1 do
    begin
    if i <> 0 then Result := Result + ',';
    Result := Result + format('[%1.4f,%1.2f]', [GuiConfigData.PitchAxisCalibration.TablePoint[i].Point1 / 10000.0, GuiConfigData.PitchAxisCalibration.TablePoint[i].Point2 / 100.0]);
    end;
end;

//-------------------------------------------------------------------------------------

procedure stCDProfileData.SetPitchCalibration (NewData: String);
var
  i: integer;
  ParseResults: stParseResults;
begin
  ParseRecords(NewData, ParseResults);
  GuiConfigData.PitchAxisCalibration.TableLength := ParseResults.ParseLength;
  for i := 0 to 15 do
    begin
    GuiConfigData.PitchAxisCalibration.TablePoint[i].Point1 := 0;
    GuiConfigData.PitchAxisCalibration.TablePoint[i].Point2 := 0;
    if ParseResults.ParseLength > i then
      begin
      GuiConfigData.PitchAxisCalibration.TablePoint[i].Point1 := round(ParseResults.ParsePoint[i].Point1 * 10000.0);
      GuiConfigData.PitchAxisCalibration.TablePoint[i].Point2 := round(ParseResults.ParsePoint[i].Point2 * 100.0);
      end;
    end;
end;

//-------------------------------------------------------------------------------------

function stCDProfileData.GetPitchAngles : string;
var
  i: integer;
begin
  Result := '';
  for i := 0 to GuiConfigData.PitchAngleTable.TableLength - 1 do
    begin
    if i <> 0 then Result := Result + ',';
    Result := Result + format('%1.2f', [GuiConfigData.PitchAngleTable.TablePoint[i].Point1 / 100.0]);
    end;
end;

//-------------------------------------------------------------------------------------

procedure stCDProfileData.SetPitchAngles (NewData: String);
var
  i: integer;
begin
  ParseFields(NewData, FieldResults);
  GuiConfigData.PitchAngleTable.TableLength := FieldResults.FieldCount;
  for i := 0 to 15 do
    begin
    GuiConfigData.PitchAngleTable.TablePoint[i].Point1 := 0;
    if FieldResults.FieldCount > i then
      GuiConfigData.PitchAngleTable.TablePoint[i].Point1 := round(FieldResults.FieldValue[i] * 100.0);
    end;
end;

//-------------------------------------------------------------------------------------

function stCDProfileData.GetPitchIndexes : string;
var
  i: integer;
begin
  Result := '';
  for i := 0 to GuiConfigData.PitchIndexTable.TableLength - 1 do
    begin
    if i <> 0 then Result := Result + ',';
    Result := Result + format('%1.2f', [GuiConfigData.PitchIndexTable.TablePoint[i].Point1 / 100.0]);
    end;
end;

//-------------------------------------------------------------------------------------

procedure stCDProfileData.SetPitchIndexes (NewData: String);
var
  i: integer;
begin
  ParseFields(NewData, FieldResults);
  GuiConfigData.PitchIndexTable.TableLength := FieldResults.FieldCount;
  for i := 0 to 15 do
    begin
    GuiConfigData.PitchIndexTable.TablePoint[i].Point1 := 0;
    if FieldResults.FieldCount > i then
      GuiConfigData.PitchIndexTable.TablePoint[i].Point1 := round(FieldResults.FieldValue[i] * 100.0);
    end;
end;

//-------------------------------------------------------------------------------------

procedure stCDProfileData.SetPitchTable;
var
  StrIndex, i, j: integer;
  TmpFloat: single;
begin
  for i := 0 to 15 do
    begin
    for j := 0 to 15 do
      begin
      GuiConfigData.PitchKFactorTable[i, j] := 1000;
      if (GuiConfigData.PitchAngleTable.TableLength > i) and (GuiConfigData.PitchIndexTable.TableLength > j) then
        begin
        StrIndex := 1;
        MyStrToFloat(PitchKFactorTable^[i,j].Text, StrIndex, TmpFloat);
        GuiConfigData.PitchKFactorTable[i, j] := round(TmpFloat * 1000);
        end;
      end;
    end;
end;

//-------------------------------------------------------------------------------------

procedure stCDProfileData.SetPitchAngleLabel (AngleOffset: integer; AngleLabel: TLabel);
begin
  AngleLabel.Visible := GuiConfigData.PitchAngleTable.TableLength > AngleOffset;
  AngleLabel.Caption := format('%1.2f', [GuiConfigData.PitchAngleTable.TablePoint[AngleOffset].Point1 / 100.0]);
end;

//-------------------------------------------------------------------------------------

procedure stCDProfileData.SetPitchIndexLabel (IndexOffset: integer; IndexLabel: TLabel);
begin
  IndexLabel.Visible := GuiConfigData.PitchIndexTable.TableLength > IndexOffset;
  IndexLabel.Caption := format('%1.2f', [GuiConfigData.PitchIndexTable.TablePoint[IndexOffset].Point1 / 100.0]);
end;

//-------------------------------------------------------------------------------------

procedure stCDProfileData.SetPitchKFactorEdit (AngleOffset, IndexOffset: Integer; KFactorEdit: TEdit);
begin
  KFactorEdit.Visible := (GuiConfigData.PitchAngleTable.TableLength > AngleOffset) and (GuiConfigData.PitchIndexTable.TableLength > IndexOffset);
  KFactorEdit.Text := format('%1.3f', [GuiConfigData.PitchKFactorTable[AngleOffset, IndexOffset] / 1000.0]);
end;

procedure stCDProfileData.ClearData;
begin
  ZeroMemory(@GuiConfigData, sizeof(GuiConfigData));
  GuiConfigData.RVOutputMode := 1;
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
end;

procedure stCDProfileData.UpdateCD_41;
begin
  move(GuiConfigData.ProfileName, CD_41_Upload.ProfileName, Sizeof(CD_41_Upload.ProfileName));
  CD_41_Upload.DIG0 := GuiConfigData.DIG0;
  CD_41_Upload.DIG1 := GuiConfigData.DIG1;
  CD_41_Upload.DIG2 := GuiConfigData.DIG2;
  CD_41_Upload.DIG3 := GuiConfigData.DIG3;
  CD_41_Upload.DIG4 := GuiConfigData.DIG4;
  CD_41_Upload.DIG5 := GuiConfigData.DIG5;
  CD_41_Upload.KmaFilterGain := GuiConfigData.KmaFilterGain;
  CD_41_Upload.MasterIndexFilteringGain := GuiConfigData.MasterIndexFilteringGain;
  CD_41_Upload.AccelVoltageXAdcFilteringGain := GuiConfigData.AccelVoltageXAdcFilteringGain;
  CD_41_Upload.AccelVoltageYAdcFilteringGain := GuiConfigData.AccelVoltageYAdcFilteringGain;
  CD_41_Upload.AccelVoltageZAdcFilteringGain := GuiConfigData.AccelVoltageZAdcFilteringGain;
  CD_41_Upload.ReverseFlags := 0;
  if GuiConfigData.KMAReverseFlag then CD_41_Upload.ReverseFlags := CD_41_Upload.ReverseFlags or 1;
  if GuiConfigData.Slave1ReverseFlag then CD_41_Upload.ReverseFlags := CD_41_Upload.ReverseFlags or 2;
  if GuiConfigData.Slave2ReverseFlag then CD_41_Upload.ReverseFlags := CD_41_Upload.ReverseFlags or 4;
  if GuiConfigData.Slave3ReverseFlag then CD_41_Upload.ReverseFlags := CD_41_Upload.ReverseFlags or 8;
  if GuiConfigData.Slave4ReverseFlag then CD_41_Upload.ReverseFlags := CD_41_Upload.ReverseFlags or $10;
  if GuiConfigData.Slave5ReverseFlag then CD_41_Upload.ReverseFlags := CD_41_Upload.ReverseFlags or $20;
  if GuiConfigData.Slave6ReverseFlag then CD_41_Upload.ReverseFlags := CD_41_Upload.ReverseFlags or $40;
  if GuiConfigData.Slave7ReverseFlag then CD_41_Upload.ReverseFlags := CD_41_Upload.ReverseFlags or $80;
  if GuiConfigData.Slave8ReverseFlag then CD_41_Upload.ReverseFlags := CD_41_Upload.ReverseFlags or $100;
  if GuiConfigData.Slave9ReverseFlag then CD_41_Upload.ReverseFlags := CD_41_Upload.ReverseFlags or $200;
  if GuiConfigData.Slave10ReverseFlag then CD_41_Upload.ReverseFlags := CD_41_Upload.ReverseFlags or $400;
  CD_41_Upload.LowFuelLevelAlarmMode := (GuiConfigData.LowFuelLevelOutputLevel and 1);
  if GuiConfigData.LowFuelLevelInvertFlag then
     CD_41_Upload.LowFuelLevelAlarmMode := CD_41_Upload.LowFuelLevelAlarmMode or 2;
  CD_41_Upload.LowFuelLevelTripPoint := GuiConfigData.LowFuelLevelTripPoint;
  CD_41_Upload.ErrorCondition := (GuiConfigData.ErrorOutputActiveLevel and 1) or
                                 ((GuiConfigData.RVOutputErrorMode and 3) shl 3) or
                                 ((GuiConfigData.MasterFreqErrorMode and 3) shl 1);
  CD_41_Upload.RVOutputMode := (GuiConfigData.RVOutputMode and 3) or (( GuiConfigData.RVPullupMode and 1) shl 2);
  CD_41_Upload.ErrorRVOutput1 := GuiConfigData.ErrorRVOutput1;
  CD_41_Upload.ErrorRVOutput2 := GuiConfigData.ErrorRVOutput2;
  CD_41_Upload.ErrorRvOutputToggleTime := GuiConfigData.ErrorRvOutputToggleTime;
  CD_41_Upload.MasterFreqMode := (GuiConfigData.MasterFreqMode and 1);
  CD_41_Upload.ErrorMasterFreq1 := GuiConfigData.ErrorMasterFreq1;
  CD_41_Upload.ErrorMasterFreq2 := GuiConfigData.ErrorMasterFreq2;
  CD_41_Upload.ErrorMasterToggleTime := GuiConfigData.ErrorMasterToggleTime;
  CD_41_Upload.RS485_MessageMode := GuiConfigData.RS485_MessageMode;
  CD_41_Upload.RS485_MessageTime := GuiConfigData.RS485_MessageTime;
  move(GuiConfigData.SetpointTable, CD_41_Upload.SetpointTable, sizeof(CD_41_Upload.SetpointTable));
  move(GuiConfigData.MasterFreqTable, CD_41_Upload.MasterFreqTable, sizeof(CD_41_Upload.MasterFreqTable));
  CD_41_Upload.CRC16 := CalcCrcArray(@CD_41_Upload, 2044);
end;

procedure stCDProfileData.UpdateCD_42;
begin
  move(GuiCDFactoryData.TimeStr, CD_42_Upload.TimeStr, Sizeof(CD_42_Upload.TimeStr));
  move(GuiCDFactoryData.DateStr, CD_42_Upload.DateStr, Sizeof(CD_42_Upload.DateStr));
  move(GuiCDFactoryData.NameTesterStr, CD_42_Upload.NameTesterStr, Sizeof(CD_42_Upload.NameTesterStr));
  move(GuiCDFactoryData.ModelNumberStr, CD_42_Upload.ModelNumberStr, Sizeof(CD_42_Upload.ModelNumberStr));
  move(GuiCDFactoryData.SerialNumberStr, CD_42_Upload.SerialNumberStr, Sizeof(CD_42_Upload.SerialNumberStr));
  CD_42_Upload.Calibration33 := GuiCDFactoryData.Calibration33;
  CD_42_Upload.PowerRail5KFactor := GuiCDFactoryData.PowerRail5KFactor;
  CD_42_Upload.KmaKFactor := GuiCDFactoryData.KmaKFactor;
  CD_42_Upload.PowerInputKFactor := GuiCDFactoryData.PowerInputKFactor;
  CD_42_Upload.RVOutputVoltageKFactor := GuiCDFactoryData.RVOutputVoltageKFactor;
  CD_42_Upload.RVOutputCurrentKFactor := GuiCDFactoryData.RVOutputCurrentKFactor;
  CD_42_Upload.PwmOutputKFactor := GuiCDFactoryData.PwmOutputKFactor;
  CD_42_Upload.Hardware := GuiCDFactoryData.Hardware;
  CD_42_Upload.CRC16 := CalcCrcArray(@CD_42_Upload, 2044);
end;

procedure stCDProfileData.UpdateCD_43;
begin
  CD_43_Upload.ControlAxis := GuiConfigData.PitchAxis;
  CD_43_Upload.ControlAxisOffset := GuiConfigData.PitchAxisOffset;
  move(GuiConfigData.PitchAngleTable, CD_43_Upload.ControlAngleTable, sizeof(CD_43_Upload.ControlAngleTable));
  move(GuiConfigData.PitchIndexTable, CD_43_Upload.ControlIndexTable, sizeof(CD_43_Upload.ControlIndexTable));
  move(GuiConfigData.PitchAxisCalibration, CD_43_Upload.ControlAxisCalibration, sizeof(CD_43_Upload.ControlAxisCalibration));
  move(GuiConfigData.PitchKFactorTable, CD_43_Upload.ControlKTable, sizeof(CD_43_Upload.ControlKTable));
  CD_43_Upload.CRC16 := CalcCrcArray(@CD_43_Upload, 2044);
end;

//-------------------------------------------------------------------------------------


end.
