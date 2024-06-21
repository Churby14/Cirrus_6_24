unit profile;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, ExtCtrls,
  Buttons, mystr2float, IniFiles;

type
  stProfilePair = record
    ProfileIndex: word;               // 18 to 527
    ProfileSetpoint: single;          // xx.xx if voltage, xxxx.x if Resistance
  end;

type
  TProfileData = class(TObject)
  private
    { private declarations }
  	fEEPROM_Config: word;                         // Bit0: Output Mode (1=Voltage, 0=Resistance)   Bit1: KMA Direction (0=Normal, 1=Inverted),
                                                  // Bit2: Low Fuel/Hold Logic (0=Disabled, 1=Enabled)    Bit3: Output Level (0=Normal, 1=Inverted)
                                                  // Bit4: Mode Select (0=Low Fuel, 1=Hold Logic)  Bits5-7: Reserved
  	fTrippoint: word;
  	fKmaSamples: word;                           // 10 to 8191
  	fHoldValue: word;
    fCalibrationScaling: single;                 // 1.0, 0.5, 0.25, or 0.1
    fProfileStr: string;
    fProfileScaling: single;
  	fProfile: array [0..509] of word;
    fProfilePairLength: integer;
    fProfilePair: array [0..99] of stProfilePair;
    fBM: TBitMap;
    fGraphImage: TImage;
    fBorderRect: Trect;
    procedure SetKmaSamples (Samples: word);
    procedure SetProfileStr (ProfileStr: string);
    function GetProfile (Index: integer) : word;
    procedure SetProfile (Index: integer; Value: word);
  public
    constructor Create;
    destructor Destroy; override;
    procedure ClearProfilePairs;
    procedure AddProfilePair (NewPair: stProfilePair);
    function GetProfilePair (Index: integer) : stProfilePair;
    function ParseProfileStr(ProfileStr: string): string;
    procedure ParseCompleteProfileStr (ProfileName: string);
    procedure Redraw;
    property ProfilePairLength: integer read fProfilePairLength;
    property EEPROM_Config: word read fEEPROM_Config write fEEPROM_Config;
    property Trippoint: word read fTrippoint write fTrippoint;
    property HoldValue: word read fHoldValue write fHoldValue;
    property KmaSamples: word read fKmaSamples write SetKmaSamples;
    property CalibrationScaling: single read fCalibrationScaling write fCalibrationScaling;
    property ProfileStr: string read fProfileStr write SetProfileStr;
    property Profile[Index: integer]: word read GetProfile write SetProfile;
    property GraphImage: TImage write fGraphImage;
    { public declarations }
    end;

implementation

uses
  global, parse;

constructor TProfileData.Create;
begin
end;

//********************************************************************************************

destructor TProfileData.Destroy;
begin
end;

//********************************************************************************************

procedure TProfileData.SetKmaSamples (Samples: word);
begin
  if Samples < 10 then Samples := 10;
  if Samples > 8191 then Samples := 8191;
  fKmaSamples := Samples;
end;

//********************************************************************************************

procedure TProfileData.SetProfileStr (ProfileStr: string);
begin
  fProfileStr := ProfileStr;
end;

//********************************************************************************************

function TProfileData.GetProfile (Index: integer) : word;
begin
  Result := fProfile[Index];
end;

//********************************************************************************************

procedure TProfileData.SetProfile (Index: integer; Value: word);
begin
  fProfile[Index] := Value;
end;

//********************************************************************************************

procedure TProfileData.ClearProfilePairs;
begin
  fProfilePairLength := 0;
end;

//********************************************************************************************

procedure TProfileData.AddProfilePair (NewPair: stProfilePair);
begin
  fProfilePair[fProfilePairLength] := NewPair;
  inc(fProfilePairLength);
end;

//********************************************************************************************

function TProfileData.GetProfilePair (Index: integer) : stProfilePair;
begin
  Result := fProfilePair[Index];
end;

//***********************************************************************************************************************

function GetChar (str: string; var strindex: integer) : char;
begin
  Result := ' ';
  if strindex > length(str) then exit;
  Result := str[strindex];
  strindex := strindex + 1;
end;

//***********************************************************************************************************************

function TProfileData.ParseProfileStr(ProfileStr: string): string;
var
  Status, YRange, XRange, XHalfRange, Point1, Point2, j, i, StrIndex: integer;
  CI: integer;
  Ch: char;
  MaxSetpoint, TmpFloat: single;
begin
 ProfileStr := StringReplace(ProfileStr, ' ', '', [rfReplaceAll]);         // Remove whitespace
 fProfileStr := ProfileStr;
 fProfilePairLength := 0;
 StrIndex := 1;
 MaxSetpoint := 0;
 Status := 0;
 while (True) do
   begin
   Ch := GetChar(ProfileStr, StrIndex);
   if Ch <> '[' then
     begin
     Status := -1;                      // Missing Left Bracket
     break;
     end;
   MyStrToFloat(ProfileStr, StrIndex, TmpFloat);
   fProfilePair[fProfilePairLength].ProfileIndex := round(TmpFloat);
   Ch := GetChar(ProfileStr, StrIndex);
   if Ch <> ',' then
     begin
     Status := -3;                      // Missing Comma
     break;
     end;
   MyStrToFloat(ProfileStr, StrIndex, TmpFloat);
   fProfilePair[fProfilePairLength].ProfileSetpoint := TmpFloat;
   if TmpFloat > MaxSetpoint then MaxSetpoint := TmpFloat;
   Ch := GetChar(ProfileStr, StrIndex);
   if Ch <> ']' then
     begin
     Status := -2;                      // Missing Right Bracket
     break;
     end;
   fProfilePairLength := fProfilePairLength + 1;
   if StrIndex > length(ProfileStr) then break;     // End of Profile String
   Ch := GetChar(ProfileStr, StrIndex);
   if Ch <> ',' then
     begin
     Status := -3;                      // Missing Comma
     break;
     end;
   if fProfilePairLength = 63 then
     begin
     Status := -4;                      // Too many pairs
     break;
     end;
   end;
  if (Status = 0) then
      begin
      if fProfilePairLength < 2 then Status := -15;
      if (Status = 0) and (fProfilePair[0].ProfileIndex <> 18) then Status := -10;
      if (Status = 0) and (fProfilePair[fProfilePairLength - 1].ProfileIndex <>  527) then Status := -11;
      if (Status = 0) then
        begin
        CI := -1;
        for i := 0 to fProfilePairLength - 1 do
          begin
          if (fProfilePair[i].ProfileIndex <= CI) then
            begin
            Status := -14;
            break;
            end;
          CI := fProfilePair[i].ProfileIndex;
          if fProfilePair[i].ProfileSetpoint < 0.0 then
            begin
            Status := -12;
            break;
            end;
          end;
        end;
      end;

  Result := '';
  if Status <> 0 then
      begin
      fProfilePairLength := 0;
      if Status = -1 then Result := 'Error: Missing Left Bracket';
      if Status = -2 then Result := 'Error: Missing Right Bracket';
      if Status = -3 then Result := 'Error: Missing Comma';
      if Status = -4 then Result := 'Error: Too many Pairs, Maximum is 63';
      if Status = -15 then Result := 'Error: Too few Pairs, Minimum is 2';
      if Status = -10 then Result := 'Error: Index of first pair MUST be 18';
      if Status = -11 then Result := 'Error: Index of last pair MUST be 527';
      if Status = -12 then Result := 'Error: Setpoint must be greater than 0.0';
      if Status = -14 then Result := 'Error: Each Index must be bigger than the previous Index';
      end
  else
    begin
    fProfileScaling := 10.0;
    fCalibrationScaling := 1.0;
    if (fEEPROM_Config and 1) = 1 then
      begin
      fProfileScaling := 100.0;
      if MaxSetpoint < 5.0 then fCalibrationScaling := 0.5;
      if MaxSetpoint < 2.5 then fCalibrationScaling := 0.25;
      if MaxSetpoint < 1.0 then fCalibrationScaling := 0.1;
      end;
    for i := 1 to fProfilePairLength - 1 do
      begin
      Point1 := round(fProfilePair[i].ProfileSetpoint * fProfileScaling / fCalibrationScaling);
      Point2 := round(fProfilePair[i - 1].ProfileSetpoint * fProfileScaling / fCalibrationScaling);
      XRange := fProfilePair[i].ProfileIndex - fProfilePair[i - 1].ProfileIndex;
      XHalfRange := XRange div 2;
      YRange := Point1 - Point2;
      if YRange < 0 then XHalfRange := -XHalfRange;
      for j := 0 to XRange do fProfile[j + fProfilePair[i - 1].ProfileIndex - 18] := (((YRange * j) + XHalfRange) div XRange) + Point2;
      end;
    end;
end;

//***********************************************************************************************************************

procedure TProfileData.ParseCompleteProfileStr (ProfileName: string);
var
  i: integer;
  ProfileString: string;
  RecordResults: stRecordResults;
  FieldResults: stFieldResults;
  IniFile: TIniFile;
begin
 IniFile := TIniFile.Create(AppPath + 'PROFILES.INI');
 ProfileString := IniFile.ReadString('PROFILES', ProfileName, '-NONE-');
 if ProfileString = '-NONE-' then exit;

  ParseProfileRecord(ProfileString, RecordResults);
  ParseFields(RecordResults.RecordStr[0], FieldResults);

  fEEPROM_Config := 0;
  if (ProfileName[1] = 'V') then fEEPROM_Config := fEEPROM_Config or $01;
  if FieldResults.FieldValue[0] <> 0 then fEEPROM_Config := fEEPROM_Config or $02;
  if (round(FieldResults.FieldValue[1]) <> 0) then fEEPROM_Config := fEEPROM_Config or $04;
  if (round(FieldResults.FieldValue[1]) = 2) or (round(FieldResults.FieldValue[1]) = 4) then  fEEPROM_Config := fEEPROM_Config or $08;
  if (round(FieldResults.FieldValue[1]) = 3) or (round(FieldResults.FieldValue[1]) = 4) then  fEEPROM_Config := fEEPROM_Config or $10;
  fTrippoint := round(FieldResults.FieldValue[2]);
  fHoldValue := round(FieldResults.FieldValue[3]);
  fKmaSamples := round(FieldResults.FieldValue[4]);
  if fKmaSamples < 10 then fKmaSamples := 10;

  fProfileStr := '';
  for i := 1 to RecordResults.RecordCount - 1 do
    begin
    if i <> 1 then fProfileStr := fProfileStr + ', ';
    ParseFields(RecordResults.RecordStr[i], FieldResults);
    fProfileStr := fProfileStr + '[' + format('%u, %1.3f', [round(FieldResults.FieldValue[0]), FieldResults.FieldValue[1] ]) + ']';
    end;
  ParseProfileStr(fProfileStr);

end;

//******************************************************************************************************************

procedure TProfileData.Redraw;
var
  LastY, LastX, YSide, XSide, X, Y, i, Yoffset, Xoffset: integer;
  LastXValue, LastYValue, XRange, YRange: single;
  R: TRect;
  LowScaleY, HighScaleY: single;
  ProfilePair: stProfilePair;
  TmpStr: string;
begin
  fBM := TBitMap.Create;
  fBM.Width := fGraphImage.Width;
  fBM.Height := fGraphImage.Height;
  //
  // Clear Canvas
  //
  R.Top := 0;
  R.Left := 0;
  R.Right := fBM.Width;
  R.Bottom := fBM.Height;
  fBM.Canvas.Brush.Color := clBlack;
  fBM.Canvas.FillRect(R);
  fBM.Canvas.Pen.Color := clWhite;
  //
  // Final Position of Graphing Area
  //
  if (fEEPROM_Config and $04) = 0 then fBorderRect.Top := 20 else fBorderRect.Top := 40;
  fBorderRect.Left := 45;
  fBorderRect.Right := fBM.Width - 40;
  fBorderRect.Bottom := fBM.Height - 20;

  fBM.Canvas.MoveTo(fBorderRect.Left, fBorderRect.Top);
  fBM.Canvas.LineTo(fBorderRect.Right, fBorderRect.Top);
  fBM.Canvas.LineTo(fBorderRect.Right, fBorderRect.Bottom);
  fBM.Canvas.LineTo(fBorderRect.Left, fBorderRect.Bottom);
  fBM.Canvas.LineTo(fBorderRect.Left, fBorderRect.Top);

  HighScaleY := 0;
  for i := 0 to fProfilePairLength - 1 do
    begin
    ProfilePair := GetProfilePair(i);
    if ProfilePair.ProfileSetpoint > HighScaleY then HighScaleY := ProfilePair.ProfileSetpoint;
    end;
  HighScaleY := HighScaleY + 0.25;
  LowScaleY := -0.1;

  XRange := 530 - 15;
  X := fBorderRect.Right - fBorderRect.Left;
  YRange := HighScaleY - LowScaleY;
  Y := fBorderRect.Bottom - fBorderRect.Top;

  fBM.Canvas.Pen.Color := clDkGray;
  fBM.Canvas.Pen.Style := psDot;
  for i := 0 to fProfilePairLength - 1 do
    begin
    ProfilePair := GetProfilePair(i);
    Xoffset := fBorderRect.Left + round(((ProfilePair.ProfileIndex - 15) / XRange) * X);
    Yoffset := fBorderRect.Bottom - round(((ProfilePair.ProfileSetpoint - LowScaleY) / YRange) * Y);
    fBM.Canvas.MoveTo(fBorderRect.Left, Yoffset);
    fBM.Canvas.LineTo(fBorderRect.Right, Yoffset);
    fBM.Canvas.MoveTo(Xoffset, fBorderRect.Top);
    fBM.Canvas.LineTo(Xoffset, fBorderRect.Bottom);
    end;

  fBM.Canvas.Pen.Color := clAqua;
  fBM.Canvas.Pen.Width := 3;
  fBM.Canvas.Pen.Style := psSolid;
  if (fEEPROM_Config and $04) <> 0 then
    begin
    if (fEEPROM_Config and $10) = 0 then
      begin
      Xoffset := fBorderRect.Left + round(((fTrippoint - 15) / XRange) * X);
      fBM.Canvas.MoveTo(Xoffset, fBorderRect.Top - 5);
      fBM.Canvas.LineTo(Xoffset, fBorderRect.Bottom);
      fBM.Canvas.MoveTo(Xoffset, fBorderRect.Top - 30);
      fBM.Canvas.LineTo(Xoffset, fBorderRect.Top - 15);
      if (fEEPROM_Config and $08) <> 0 then
        begin
        fBM.Canvas.MoveTo(Xoffset, fBorderRect.Top - 30);
        fBM.Canvas.LineTo(fBorderRect.Left, fBorderRect.Top - 30);
        fBM.Canvas.MoveTo(Xoffset, fBorderRect.Top - 15);
        fBM.Canvas.LineTo(fBorderRect.Right, fBorderRect.Top - 15);
        end
      else
        begin
        fBM.Canvas.MoveTo(Xoffset, fBorderRect.Top - 15);
        fBM.Canvas.LineTo(fBorderRect.Left, fBorderRect.Top - 15);
        fBM.Canvas.MoveTo(Xoffset, fBorderRect.Top - 30);
        fBM.Canvas.LineTo(fBorderRect.Right, fBorderRect.Top - 30);
        end;
      end
    else
        begin
        Xoffset := fBorderRect.Left + round(((fTrippoint - 15) / XRange) * X);
        fBM.Canvas.MoveTo(Xoffset, fBorderRect.Top - 5);
        fBM.Canvas.LineTo(Xoffset, fBorderRect.Bottom);

        Yoffset := fBorderRect.Bottom - round((((fProfile[fHoldValue - 18] / fProfileScaling * fCalibrationScaling) - LowScaleY) / YRange) * Y);
        if (fEEPROM_Config and $08) <> 0 then
          begin
          fBM.Canvas.MoveTo(Xoffset, Yoffset);
          fBM.Canvas.LineTo(fBorderRect.Left, YOffset);
          end
        else
          begin
          fBM.Canvas.MoveTo(Xoffset, Yoffset);
          fBM.Canvas.LineTo(fBorderRect.Right, YOffset);
          end;
        end;
    end;
  fBM.Canvas.Pen.Width := 1;

  fBM.Canvas.Pen.Style := psSolid;
  fBM.Canvas.Pen.Color := clYellow;
  for i := 0 to fProfilePairLength - 1 do
    begin
    ProfilePair := GetProfilePair(i);
    Xoffset := fBorderRect.Left + round(((ProfilePair.ProfileIndex - 15) / XRange) * X);
    Yoffset := fBorderRect.Bottom - round(((ProfilePair.ProfileSetpoint - LowScaleY) / YRange) * Y);
    if i = 0 then
      begin
      fBM.Canvas.MoveTo(Xoffset, Yoffset);
      end
    else
      begin
      fBM.Canvas.LineTo(Xoffset, Yoffset);
      end;
    end;

  fBM.Canvas.Font.Color := clYellow;
  YSide := 0;
  XSide := 0;
  LastX := -9999;
  LastY := -9999;
  LastXValue := -9999.0;
  LastYValue := -9999.0;
  for i := 0 to fProfilePairLength - 1 do
    begin
    ProfilePair := GetProfilePair(i);
    Xoffset := fBorderRect.Left + round(((ProfilePair.ProfileIndex - 15) / XRange) * X);
    Yoffset := fBorderRect.Bottom - round(((ProfilePair.ProfileSetpoint - LowScaleY) / YRange) * Y);

    if LastYValue <> ProfilePair.ProfileSetpoint then
      begin
      LastYValue := ProfilePair.ProfileSetpoint;
      TmpStr := format('%5.2f', [ProfilePair.ProfileSetpoint]);
      if abs(LastY - Yoffset) < 10 then YSide := (Yside + 1) and 1;
      if (Yside = 1) and (abs(LastY - Yoffset) >= 10) then YSide := 0;
      LastY := Yoffset;
      if Yside = 0 then
        fBM.Canvas.TextOut(fBorderRect.Left - 3 - fBM.Canvas.TextWidth(TmpStr), Yoffset - 6, TmpStr)
      else
        fBM.Canvas.TextOut(fBorderRect.Right + 3, Yoffset - 6, TmpStr);
      end;

    if LastXValue <> ProfilePair.ProfileIndex then
      begin
      LastXValue := ProfilePair.ProfileIndex;
      TmpStr := format('%1.0f', [ProfilePair.ProfileIndex * 1.0]);
      if abs(LastX - Xoffset) < 10 then XSide := (Xside + 1) and 1;
      if (Xside = 1) and (abs(LastX - Xoffset) >= 10) then XSide := 0;
      LastX := Xoffset;
      if Xside = 0 then
        fBM.Canvas.TextOut(Xoffset - (fBM.Canvas.TextWidth(TmpStr) div 2), fBorderRect.Bottom + 3, TmpStr)
      else
        fBM.Canvas.TextOut(Xoffset - (fBM.Canvas.TextWidth(TmpStr) div 2), fBorderRect.Top - 16, TmpStr)
      end;
    end;

  fBM.Canvas.Pen.Width := 1;
  fGraphImage.Picture.Graphic := fBM;
  fBM.Free;
end;

end.

