unit Unit3;

{$mode ObjFPC}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, StdCtrls, ExtCtrls,
  global, mystr2float, graph, Unit4;

Function ConvertCANSetpoint (Percent: Single) : longint;
Function ConvertToFloat (FloatStr: string; ScaleFactor: single) : LongInt;

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
    //function GetSensorAOutput: int32;
  end;

type

  { TForm3 }

  TForm3 = class(TForm)
    Button2: TButton;
    btnAddPoint: TButton;
    Image1: TImage;
    lblState: TLabel;
    leSensorPositions: TLabeledEdit;
    lblThrottleAction: TLabel;
    lblUnits: TLabel;
    leSensorBIndex: TLabeledEdit;
    leThrottleOutput: TLabeledEdit;
    leSensorAIndex: TLabeledEdit;
    plSelectOutput: TPanel;
    plDataPoints: TPanel;
    rgThrottleOutputType: TRadioGroup;
    procedure btnAddPointClick(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure InitializeForm;
    procedure leSensorAIndexChange(Sender: TObject);
    procedure SensorFusion;
    procedure GetSensorSetpoint;
    procedure UpdateSensorBProfile;
    function GetSensorAOutput: int32;
    procedure UpdateOutput;
    procedure MergeCalTables;
  private

  public

  end;

var
  Form3: TForm3;
  OutputType: integer;
  TmpIndex: integer;
  TmpSensorACal: stTwoPointTable;
  TmpSensorBCal: stTwoPointTable;
  SensorAOutputTable: stTwoPointTable;
  SensorBOutputTable: stTwoPointTable;

  FinalAOutputTable: stTwoPointTable;
  FinalBOutputTable: stTwoPointTable;

  CalPoints: integer;
  ThrottleProfileGraph: TProfileGraph;
  bOutputCalDone: boolean;
  LastKmaIndexA: int32;
  LastKmaIndexB: int32;
  SensorFusionCal: stTwoPointTable;
  State: integer;
  NoiseCnt: integer;
  EndKmaIndexA: integer;
  EndKmaIndexB: integer;
  ErrorString: string;
  SensorASetpoint: int32;
  SensorBSetpoint: int32;
  KmaIndexA: int32;
  KmaIndexB: int32;
implementation

{$R *.lfm}

{ TForm3 }

Function ConvertToFloat (FloatStr: string; ScaleFactor: single) : LongInt;
var
  StrIndex: integer;
  TmpFloat: single;
begin
  StrIndex := 1;
  MyStrToFloat(FloatStr, StrIndex, TmpFloat);
  Result := round(TmpFloat * ScaleFactor);
end;

procedure TForm3.InitializeForm;
begin
  plSelectOutput.Visible := True;

  ThrottleProfileGraph := TProfileGraph.Create;
  ThrottleProfileGraph.GraphImage := Image1;
  ThrottleProfileGraph.VertHashMarks := 1;
  ThrottleProfileGraph.HorzHashMarks := 1;
  ThrottleProfileGraph.HorzTitle := 'Setpoint (%)';
  ThrottleProfileGraph.HorzLowScale := 0;
  ThrottleProfileGraph.HorzHighScale := 100.00;
  ThrottleProfileGraph.HorzPrecision := 1;
  ThrottleProfileGraph.VertTitle := 'Index';
  ThrottleProfileGraph.VertLowScale := 4;
  ThrottleProfileGraph.VertHighScale := 1023;
  ThrottleProfileGraph.VertPrecision := 1;
end;

// Looks at CD_70 and CD_73 data to set the Sensor A and Sensor B Setpoints
// this way we avoid if statements to determine which setpoitn is A and B
// throughout the code
procedure TForm3.GetSensorSetpoint;
begin
  // Local sensor is Sensor A
  if (CD_70.BF9.SensorLetter = 0) then
    begin
    KmaIndexA       := CD_70.ENG_KmaIndex;
    KmaIndexB       := CD_73.ENG_KmaIndex;
    end
  // Local sensor is Sensor B
  else
    begin
    KmaIndexA       := CD_73.ENG_KmaIndex;
    KmaIndexB       := CD_70.ENG_KmaIndex;
    end;
end;

procedure TForm3.leSensorAIndexChange(Sender: TObject);
begin
  GetSensorSetpoint;
  SensorFusion;
end;

procedure TForm3.UpdateOutput;
var
  Tmp: Single;
begin
  leThrottleOutput.Enabled:=True;
  Tmp := (100*(GetSensorAOutput - 32000))/(57600-32000);
  leThrottleOutput.Text := format('%1.2f', [Tmp]);
end;

// Uses TmpSensorACal and the current Sensor A Index to calculate the output
function TForm3.GetSensorAOutput: int32;
var
  i: integer;
  j: integer;
begin
  for i := 0 to TmpSensorACal.TableLength - 1 do
    begin
    // If the index is less than the lowest one used during the temporary profile then the
    // Reults should be the output at the lowest point
    if (KmaIndexA < TmpSensorACal.TablePoint[0].Point1) then
      begin
      Result := TmpSensorACal.TablePoint[0].Point2;
      end
    // If the index is greater than the highest one during the temporary profile then
    // the result should the the output at the highest point
    else if (KmaIndexA > TmpSensorACal.TablePoint[TmpSensorACal.TableLength - 1].Point1) then
      begin
        Result := TmpSensorACal.TablePoint[TmpSensorACal.TableLength - 1].Point2;
      end
    else if ((KmaIndexA > TmpSensorACal.TablePoint[i].Point1) and (KmaIndexA < TmpSensorACal.TablePoint[i+1].Point1)) then
      begin
      Result := Round((((KmaIndexA-TmpSensorACal.TablePoint[i].Point1) * (TmpSensorACal.TablePoint[i+1].Point2 - TmpSensorACal.TablePoint[i].Point2)) / (TmpSensorACal.TablePoint[i+1].Point1 - TmpSensorACal.TablePoint[i].Point1)) + TmpSensorACal.TablePoint[i].Point2);
      end;

    end;




end;

procedure TForm3.SensorFusion;
var
  i:integer;
  Tmp: int32;
begin
  // Match Sensor A index with Sensor B's index
    if (bOutputCalDone = True) then
      begin
      btnAddPoint.Visible := False;
      UpdateOutput;

      if (State = 0) then
        begin
        lblThrottleAction.Caption := 'Please Place Throttle back to Minimum Position';
        if ((abs(KmaIndexA - TmpSensorACal.TablePoint[0].Point1) < 1000) and (abs(KmaIndexB - TmpSensorBCal.TablePoint[0].Point1) < 1000)) then
          begin
          inc(State);
          end
        else
          begin
          lblState.Caption := format('Waiting for throttle index A to reach %u', [TmpSensorACal.TablePoint[0].Point1]);
          end;
        end;

      // State = confirms whether sensor indices have finished filtering
      // by checking if the change in indices is less than 1
      if (State = 1) then
        begin
        if (NoiseCnt >= 5) then
          begin
          inc(State);
          NoiseCnt := 0;
          end
        // Verify noise is less than 1
        else if ((abs(LastKmaIndexA - KmaIndexA) < 100) and (abs(LastKmaIndexB - KmaIndexB) < 100)) then
          begin
          inc(NoiseCnt);
          lblState.Caption := 'Waiting';
          end
        else
          begin
          lblState.Caption := 'Waiting';
          NoiseCnt := 0;
          LastKmaIndexA := KmaIndexA;
          LastKmaIndexB := KmaIndexB;
          end;
        end;

      if (State = 2) then
        begin
        lblState.Caption := 'Data Points Saved!';
        SensorFusionCal.TablePoint[TmpIndex].Point1:= KmaIndexA;
        SensorFusionCal.TablePoint[TmpIndex].Point2:= KmaIndexB;
        SensorFusionCal.TableLength := TmpIndex + 1;

        SensorAOutputTable.TablePoint[TmpIndex].Point1:=KmaIndexA;
        SensorBOutputTable.TablePoint[TmpIndex].Point1:=KmaIndexB;
        Tmp := GetSensorAOutput;
        SensorAOutputTable.TablePoint[TmpIndex].Point2:=Tmp;
        SensorBOutputTable.TablePoint[TmpIndex].Point2:=Tmp;

        SensorAOutputTable.TableLength:=TmpIndex;
        SensorBOutputTable.TableLength:=TmpIndex;

        inc(TmpIndex);
        inc(State);
        end;

      if (State = 3) then
        begin
        // Either max data points or throttle index is at the end
        if ((TmpIndex >= 32) or ((abs(SensorFusionCal.TablePoint[TmpIndex-1].Point1 - EndKmaIndexA) < 5000) and (abs(SensorFusionCal.TablePoint[TmpIndex-1].Point2 - EndKmaIndexB) < 5000)))  then
          begin
          State := 100; // ending state
          end
        else
          begin
          State := 4;
          end;
        end;

      // Have user move throttle until index has increased by at least 20
      if (State = 4) then
        begin
        if (((abs(KmaIndexA - SensorFusionCal.TablePoint[TmpIndex - 1].Point1) < 5000) or (abs(KmaIndexB - SensorFusionCal.TablePoint[TmpIndex - 1].Point2) < 5000))
              and((abs(KmaIndexA - SensorFusionCal.TablePoint[TmpIndex - 1].Point1) > 2000) or (abs(KmaIndexB - SensorFusionCal.TablePoint[TmpIndex - 1].Point2) > 2000))) then
          begin
          lblThrottleAction.Caption := 'Please Stop Moving Throttle';
          State := 5;
          end
        else if ((abs(KmaIndexA - SensorFusionCal.TablePoint[TmpIndex - 1].Point1) > 5000) or (abs(KmaIndexB - SensorFusionCal.TablePoint[TmpIndex - 1].Point2) > 5000)) then
          begin
          lblThrottleAction.Caption := 'Please Move Throttle Back';
          end
        else
          begin
          lblThrottleAction.Caption := 'Please Move Throttle Forward';
          end;
        end;

      // Verify user did not move throttle too far
      if (State = 5) then
        begin
        if ((abs(KmaIndexA - SensorFusionCal.TablePoint[TmpIndex - 1].Point1) > 5000) or (abs(KmaIndexB - SensorFusionCal.TablePoint[TmpIndex - 1].Point2) > 5000)) then
          begin
          State := 4;
          end
        else
          begin
          State := 1;
          end;
        end;

      if (State = 100) then
        begin
        lblThrottleAction.Caption := 'Calibration complete - verifying results';
        State := 110;
        end;

      // All indices must be in ascending order
      if (State = 110) then
        begin
          for i := 1 to SensorFusionCal.TableLength - 1 do
            begin
            if SensorFusionCal.TablePoint[i].Point1 < SensorFusionCal.TablePoint[i-1].Point1 then
              begin
              State := 1000;
              end
            else if SensorFusionCal.TablePoint[i].Point2 < SensorFusionCal.TablePoint[i-1].Point2 then
              begin
              State := 1000;
              end
            else
              begin
              State := 500;
              end;
            end;
        end;


      // Verification passed
      if (State = 500) then
        begin
        leSensorPositions.EditLabel.Caption := 'Results Valid - Please Enter a Profile Name';
        leSensorPositions.Text:= '';
        Button2.Caption:= 'Update Profile';
        SensorFusionCal.TableLength:= TmpIndex + 1;
        // Area to provide profile name
        rgThrottleOutputType.Visible:=False;
        plSelectOutput.Visible:=True;
        // Now need to merge TmpSensorXCal and the SensorXOutputTable
        MergeCalTables;

        end;


      // Error State
      if (State = 1000) then
        begin
        lblThrottleAction.Caption := ErrorString;
        btnAddPoint.Caption:= 'Restart';
        btnAddPoint.Visible:= True;
        end;
      end;
end;

//
// Routine creates the final calibrated values for sensors a and b
// By mergin TmpSensorXCal and SensorXOutputTable
procedure TForm3.MergeCalTables;
var
  i: integer;
  j: integer;
  n: integer;
  Tmp: integer;
begin

  // SENSOR A

  Tmp := TmpSensorACal.TableLength + SensorAOutputTable.TableLength;

  i := 0;
  j := 0;
  n := 0;
  while (n < Tmp) do
    begin
    if (TmpSensorACal.TablePoint[i].Point1 < SensorAOutputTable.TablePoint[j].Point1) then
      begin
      FinalAOutputTable.TablePoint[n].Point1:= TmpSensorACal.TablePoint[i].Point1;
      FinalAOutputTable.TablePoint[n].Point2:= TmpSensorACal.TablePoint[i].Point2;
      i := i + 1;
      end
    else if (TmpSensorACal.TablePoint[i].Point1 > SensorAOutputTable.TablePoint[j].Point1) then
      begin
      FinalAOutputTable.TablePoint[n].Point1:= SensorAOutputTable.TablePoint[j].Point1;
      FinalAOutputTable.TablePoint[n].Point2:= SensorAOutputTable.TablePoint[j].Point2;
      j := j + 1;
      end
    else
      begin
      FinalAOutputTable.TablePoint[n].Point1:= TmpSensorACal.TablePoint[i].Point1;
      FinalAOutputTable.TablePoint[n].Point2:= TmpSensorACal.TablePoint[i].Point2;
      i := i + 1;
      j := j + 1;
      end;

    n := i + j;

    end;

  FinalAOutputTable.TableLength:=Tmp;

  // SENSOR B

  Tmp := TmpSensorBCal.TableLength + SensorBOutputTable.TableLength;

  i := 0;
  j := 0;
  n := 0;
  while (n < Tmp) do
    begin
    if (TmpSensorBCal.TablePoint[i].Point1 < SensorBOutputTable.TablePoint[j].Point1) then
      begin
      FinalBOutputTable.TablePoint[n].Point1:= TmpSensorBCal.TablePoint[i].Point1;
      FinalBOutputTable.TablePoint[n].Point2:= TmpSensorBCal.TablePoint[i].Point2;
      i := i + 1;
      end
    else if (TmpSensorBCal.TablePoint[i].Point1 > SensorBOutputTable.TablePoint[j].Point1) then
      begin
      FinalBOutputTable.TablePoint[n].Point1:= SensorBOutputTable.TablePoint[j].Point1;
      FinalBOutputTable.TablePoint[n].Point2:= SensorBOutputTable.TablePoint[j].Point2;
      j := j + 1;
      end
    else
      begin
      FinalBOutputTable.TablePoint[n].Point1:= TmpSensorBCal.TablePoint[i].Point1;
      FinalBOutputTable.TablePoint[n].Point2:= TmpSensorBCal.TablePoint[i].Point2;
      i := i + 1;
      j := j + 1;
      end;

      n := i + j;

    end;

  FinalBOutputTable.TableLength:=Tmp;


end;

procedure TForm3.Button2Click(Sender: TObject);
var
  i:integer;
begin
  if Button2.Caption = 'Select' then
    begin
    OutputType := rgThrottleOutputType.ItemIndex;
    plSelectOutput.Visible := False;
    TmpIndex := 0;
    lblThrottleAction.Caption := 'Please Place Throttle at Minimum Position';
    if (OutputType = 0) then
      begin
      lblUnits.Caption:= 'V';
      ThrottleProfileGraph.VertHashMarks := 10;
      ThrottleProfileGraph.HorzHashMarks := 1;
      ThrottleProfileGraph.HorzTitle := 'Setpoint (V)';
      ThrottleProfileGraph.HorzLowScale := 0;
      ThrottleProfileGraph.HorzHighScale := 10;
      ThrottleProfileGraph.HorzPrecision := 1;
      ThrottleProfileGraph.VertTitle := 'Index';
      ThrottleProfileGraph.VertLowScale := 4;
      ThrottleProfileGraph.VertHighScale := 1023;
      ThrottleProfileGraph.VertPrecision := 1;
      end
    else if (OutputType = 1) then
      begin
      lblUnits.Caption:= 'Ohms';
      ThrottleProfileGraph.VertHashMarks := 10;
      ThrottleProfileGraph.HorzHashMarks := 100;
      ThrottleProfileGraph.VertTitle := 'Setpoint (Ohms)';
      ThrottleProfileGraph.HorzLowScale := 0;
      ThrottleProfileGraph.HorzHighScale := 1000;
      ThrottleProfileGraph.HorzPrecision := 1;
      ThrottleProfileGraph.VertTitle := 'Index';
      ThrottleProfileGraph.VertLowScale := 4;
      ThrottleProfileGraph.VertHighScale := 1023;
      ThrottleProfileGraph.VertPrecision := 1;
      end
    else if (OutputType = 2) then
      begin
      lblUnits.Caption:= 'Hz';
      ThrottleProfileGraph.VertHashMarks := 10;
      ThrottleProfileGraph.HorzHashMarks := 2000;
      ThrottleProfileGraph.HorzTitle := 'Setpoint (Hz)';
      ThrottleProfileGraph.HorzLowScale := 0;
      ThrottleProfileGraph.HorzHighScale := 18000;
      ThrottleProfileGraph.HorzPrecision := 1;
      ThrottleProfileGraph.VertTitle := 'Index';
      ThrottleProfileGraph.VertLowScale := 4;
      ThrottleProfileGraph.VertHighScale := 1023;
      ThrottleProfileGraph.VertPrecision := 1;
      end
    else
      begin
      lblUnits.Caption:= '%';
      end;

    CalPoints := round(ConvertToFloat(leSensorPositions.Text, 1.0));

    plDataPoints.Show;

    end;

  // Reset all global variables so another CAL can happen
  if Button2.Caption = 'Update Profile' then
    begin
    //State := 0;
    //bOutputCalDone := False;
    {SensorACalProfile.TableLength := TmpSensorACal.TableLength;
    for i := 0 to SensorACalProfile.TableLength - 1 do
      begin
      SensorACalProfile.TablePoint[i].Point1 := TmpSensorACal.TablePoint[i].Point1;
      SensorACalProfile.TablePoint[i].Point2 := TmpSensorACal.TablePoint[i].Point2;
      end;
    UpdateSensorBProfile(); }



    Form4.InitializeForm();
    Form4.Show;

    end;


end;

procedure TForm3.UpdateSensorBProfile;
var
  i: integer;
  j: integer;
  n: integer;
  Tmp: integer;
  SensorAIdxArray: array [0..32] of integer;
  TableUsedArray: array [0..32] of integer; // 0 = SensorFusionCal, 1 = TmpSensorCal
  TmpArray: stTwoPointTable;
begin

  // Sensor B's profile begins with TmpSensorBCal
  SensorBCalProfile.TableLength := TmpSensorBCal.TableLength + SensorFusionCal.TableLength;

  j := 0;
  n := 0;
  for i := 0 to SensorBCalProfile.TableLength - 1 do
    begin
    {
    if i = 0 then
      begin
      TmpArray.TablePoint[i].Point1 := TmpSensorBCal.TablePoint[i].Point1;
      TmpArray.TablePoint[i].Point2 := TmpSensorBCal.TablePoint[i].Point2;
      SensorAIdxArray[i] := n;
      TableUsedArray[i] := 1;
      n := n + 1;
      end

    else if i = SensorBCalProfile.TableLength - 1 then
      begin
      TmpArray.TablePoint[i].Point1 := TmpSensorBCal.TablePoint[i].Point1;
      TmpArray.TablePoint[i].Point2 := TmpSensorBCal.TablePoint[i].Point2;
      SensorAIdxArray[i] := n;
      TableUsedArray[i] := 1;
      end

    else
      begin
      // Select the lower index that is calibrated to
      if (TmpSensorBCal.TablePoint[j].Point1 < SensorFusionCal.TablePoint[n].Point2) then
        begin
        TmpArray.TablePoint[i].Point1 := TmpSensorBCal.TablePoint[j].Point1;
        SensorAIdxArray[i] := n;
        TableUsedArray[i] := 1;
        j := j + 1;
        end

      else if (TmpSensorBCal.TablePoint[j].Point1 > SensorFusionCal.TablePoint[n].Point2) then
        begin
        TmpArray.TablePoint[i].Point1 := SensorFusionCal.TablePoint[n].Point2;
        SensorAIdxArray[i] := n;
        TableUsedArray[i] := 0;
        n := n + 1;
        end
      // But, if the two are equal then store either one but increment both counters
      // and subtract for the table length
      else
        begin
        TmpArray.TablePoint[i].Point1 := SensorFusionCal.TablePoint[n].Point2;
        SensorAIdxArray[i] := n;
        TableUsedArray[i] := 0;
        n := n + 1;
        j := j + 1;
        SensorBCalProfile.TableLength := SensorBCalProfile.TableLength - 1;
        end;
      end;

    // Now that the point is set, need to calculate the corresponding output

    }



  end;

end;



procedure TForm3.btnAddPointClick(Sender: TObject);
begin
  // Match Sensor A index with throttle output
  if (bOutputCalDone = False) then
    begin
    // Load [Index, Output] into the temporary arrays for Sensor A and Sensor B
    // The temporary arrays only store the first calibration values. To finalize
    // the arrays, sensor matching will be done in another step
    if (TmpIndex <= CalPoints - 1) then
      begin
        TmpSensorACal.TablePoint[TmpIndex].Point1 := KmaIndexA;
        TmpSensorBCal.TablePoint[TmpIndex].Point1 := KmaIndexB;

      // Voltage scale factor = 1000
      if (OutputType = 0) then
        begin
        TmpSensorACal.TablePoint[TmpIndex].Point2 := ConvertToFloat(leThrottleOutput.Text, 1000.0);
        end
      // Resistive scale factor = 1000
      else if (OutputType = 1) then
        begin
        TmpSensorACal.TablePoint[TmpIndex].Point2 := ConvertToFloat(leThrottleOutput.Text, 1000.0);
        end
      // Frequency scale factor = 100
      else if (OutputType = 2) then
        begin
        TmpSensorACal.TablePoint[TmpIndex].Point2 := ConvertToFloat(leThrottleOutput.Text, 100.0);
        end
      // Can scale factor requires ConvertCANSetpoint function
      else
        begin
        TmpSensorACal.TablePoint[TmpIndex].Point2 := ConvertCANSetpoint(ConvertToFloat(leThrottleOutput.Text, 1.0));
        end;

      // Output of Sensor B is equal to that of Sensor A
      TmpSensorBCal.TablePoint[TmpIndex].Point2 := TmpSensorACal.TablePoint[TmpIndex].Point2;

      // Graph
      ThrottleProfileGraph.AddPoint(TmpSensorACal.TablePoint[TmpIndex].Point1/100, ConvertToFloat(leThrottleOutput.Text, 1.0));
      ThrottleProfileGraph.Redraw;

      end;

    inc(TmpIndex);
    // Set the Next Action label
    if (TmpIndex < CalPoints - 1) then
      begin
      lblThrottleAction.Caption := 'Please Place Throttle at Next Position';
      end
    else if (TmpIndex = CalPoints - 1) then
      begin
      lblThrottleAction.Caption := 'Please Place Throttle at Maximum Position';
      end
    else if (TmpIndex = CalPoints) then
      begin
      // Store last data point for later use
      EndKmaIndexA := TmpSensorACal.TablePoint[TmpIndex - 1].Point1;
      EndKmaIndexB := TmpSensorBCal.TablePoint[TmpIndex - 1].Point1;
      TmpSensorACal.TableLength:=TmpIndex;
      TmpSensorBCal.TableLength:=TmpIndex;
      lblThrottleAction.Caption := 'Please Place Throttle back to Minimum Position';
      btnAddPoint.Caption := 'Calibrate Sensor Indices';
      end
    else if (TmpIndex = CalPoints + 1) then
      begin
      leSensorBIndex.Enabled := True;
      leThrottleOutput.Enabled := False;
      bOutputCalDone := True;
      btnAddPoint.Caption := 'Add Data Point';
      TmpIndex := 0; // Reset counter
      State := 0;
      NoiseCnt := 0;
      LastKmaIndexA := KmaIndexA;
      LastKmaIndexB := KmaIndexB;
      end;
    end
  else
    begin
    // If State = 1000 then this button was clicked to restart
    if (State = 1000) then
      begin
      // Clear all routine variables
      State := 0;
      TmpIndex := 0;


      end;
    end;
end;

// Converts the CAN Setpoint from percentage to bits based on the CAN ICD.
// Standard is 1/256% per bit with a -125% offset
Function ConvertCANSetpoint (Percent: Single) : longint;
begin
  Result := round((Percent + 125) * 256);
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

end.

