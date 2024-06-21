unit Unit2;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, StdCtrls, ComCtrls, mystr2float, global;

type

  { TForm2 }

  TForm2 = class(TForm)
    cbManual5VDC: TCheckBox;
    cbManualAccelX: TCheckBox;
    cbManualAccelY: TCheckBox;
    cbManualAccelZ: TCheckBox;
    cbManualIntRef: TCheckBox;
    cbManualKma: TCheckBox;
    cbManualPowerInput: TCheckBox;
    cbManualRVOutputCurrent: TCheckBox;
    cbManualRVOutputVoltage: TCheckBox;
    cbManualSetpoint: TCheckBox;
    cbManualTemperature: TCheckBox;
    edtManual5VDC: TEdit;
    edtManualAccelX: TEdit;
    edtManualAccelY: TEdit;
    edtManualAccelZ: TEdit;
    edtManualIntRef: TEdit;
    edtManualKma: TEdit;
    edtManualPowerInput: TEdit;
    edtManualRVOutputCurrent: TEdit;
    edtManualRVOutputVoltage: TEdit;
    edtManualSetpoint: TEdit;
    edtManualTemperature: TEdit;
    tbSetpoint: TTrackBar;
    tbAccelY: TTrackBar;
    tbAccelZ: TTrackBar;
    tbIntRef: TTrackBar;
    tb5VDCPower: TTrackBar;
    tbRVCurrent: TTrackBar;
    tbAccelX: TTrackBar;
    tbTemperature: TTrackBar;
    tbPowerInput: TTrackBar;
    tbKma: TTrackBar;
    tbRVVoltage: TTrackBar;
    procedure edtManual5VDCChange(Sender: TObject);
    procedure edtManualAccelXChange(Sender: TObject);
    procedure edtManualAccelYChange(Sender: TObject);
    procedure edtManualAccelZChange(Sender: TObject);
    procedure edtManualIntRefChange(Sender: TObject);
    procedure edtManualKmaChange(Sender: TObject);
    procedure edtManualPowerInputChange(Sender: TObject);
    procedure edtManualRVOutputCurrentChange(Sender: TObject);
    procedure edtManualRVOutputVoltageChange(Sender: TObject);
    procedure edtManualSetpointChange(Sender: TObject);
    procedure edtManualTemperatureChange(Sender: TObject);
    procedure tb5VDCPowerChange(Sender: TObject);
    procedure tbAccelXChange(Sender: TObject);
    procedure tbAccelYChange(Sender: TObject);
    procedure tbAccelZChange(Sender: TObject);
    procedure tbIntRefChange(Sender: TObject);
    procedure tbKmaChange(Sender: TObject);
    procedure tbPowerInputChange(Sender: TObject);
    procedure tbRVCurrentChange(Sender: TObject);
    procedure tbRVVoltageChange(Sender: TObject);
    procedure tbSetpointChange(Sender: TObject);
    procedure tbTemperatureChange(Sender: TObject);
  private

  public

  end;

var
  Form2: TForm2;

implementation

{$R *.lfm}

{ TForm2 }

procedure TForm2.tbPowerInputChange(Sender: TObject);
begin
  edtManualPowerInput.Text := format('%d', [tbPowerInput.position]);
end;

procedure TForm2.tbRVCurrentChange(Sender: TObject);
begin
  edtManualRVOutputCurrent.Text := format('%d', [tbRVCurrent.position]);
end;

procedure TForm2.tbRVVoltageChange(Sender: TObject);
begin
  edtManualRVOutputVoltage.Text := format('%d', [tbRVVoltage.position]);
end;

procedure TForm2.tbSetpointChange(Sender: TObject);
begin
  if CD_70.BF4.RVOutputMode = 1 then
    edtManualSetpoint.Text := format('%1.3f', [tbSetpoint.position / 1000.0]);
  if CD_70.BF4.RVOutputMode = 2 then
    edtManualSetpoint.Text := format('%1.1f', [tbSetpoint.position / 10.0]);
end;

procedure TForm2.tbTemperatureChange(Sender: TObject);
begin
  edtManualTemperature.Text := format('%d', [tbTemperature.position]);
end;

procedure TForm2.edtManualPowerInputChange(Sender: TObject);
var
   StrIndex: integer;
   TmpFloat: single;
begin
  StrIndex := 1;
  TmpFloat := 0;
  MyStrToFloat(edtManualPowerInput.Text, StrIndex, TmpFloat);
  tbPowerInput.Position := round(TmpFloat);
end;

procedure TForm2.edtManualRVOutputCurrentChange(Sender: TObject);
var
   StrIndex: integer;
   TmpFloat: single;
begin
  StrIndex := 1;
  TmpFloat := 0;
  MyStrToFloat(edtManualRVOutputCurrent.Text, StrIndex, TmpFloat);
  tbRVCurrent.Position := round(TmpFloat);
end;

procedure TForm2.edtManualRVOutputVoltageChange(Sender: TObject);
var
   StrIndex: integer;
   TmpFloat: single;
begin
  StrIndex := 1;
  TmpFloat := 0;
  MyStrToFloat(edtManualRVOutputVoltage.Text, StrIndex, TmpFloat);
  tbRVVoltage.Position := round(TmpFloat);
end;

procedure TForm2.edtManualSetpointChange(Sender: TObject);
var
   StrIndex: integer;
   TmpFloat: single;
begin
  StrIndex := 1;
  TmpFloat := 0;
  MyStrToFloat(edtManualSetpoint.Text, StrIndex, TmpFloat);
  if CD_70.BF4.RVOutputMode = 1 then
    tbSetpoint.Position := round(TmpFloat * 1000);
  if CD_70.BF4.RVOutputMode = 2 then
    tbSetpoint.Position := round(TmpFloat * 10);
end;

procedure TForm2.edtManualTemperatureChange(Sender: TObject);
var
   StrIndex: integer;
   TmpFloat: single;
begin
  StrIndex := 1;
  TmpFloat := 0;
  MyStrToFloat(edtManualTemperature.Text, StrIndex, TmpFloat);
  tbTemperature.Position := round(TmpFloat);
end;

procedure TForm2.tb5VDCPowerChange(Sender: TObject);
begin
  edtManual5VDC.Text := format('%d', [tb5VDCPower.position]);
end;

procedure TForm2.tbAccelXChange(Sender: TObject);
begin
  edtManualAccelX.Text := format('%d', [tbAccelX.position]);
end;

procedure TForm2.tbAccelYChange(Sender: TObject);
begin
  edtManualAccelY.Text := format('%d', [tbAccelY.position]);
end;

procedure TForm2.tbAccelZChange(Sender: TObject);
begin
  edtManualAccelZ.Text := format('%d', [tbAccelZ.position]);
end;

procedure TForm2.tbIntRefChange(Sender: TObject);
begin
  edtManualIntRef.Text := format('%d', [tbIntRef.position]);
end;

procedure TForm2.tbKmaChange(Sender: TObject);
begin
  edtManualKma.Text := format('%d', [tbKma.position]);
end;

procedure TForm2.edtManualKmaChange(Sender: TObject);
var
   StrIndex: integer;
   TmpFloat: single;
begin
  StrIndex := 1;
  TmpFloat := 0;
  MyStrToFloat(edtManualKma.Text, StrIndex, TmpFloat);
  tbKma.Position := round(TmpFloat);
end;

procedure TForm2.edtManual5VDCChange(Sender: TObject);
var
   StrIndex: integer;
   TmpFloat: single;
begin
  StrIndex := 1;
  TmpFloat := 0;
  MyStrToFloat(edtManual5VDC.Text, StrIndex, TmpFloat);
  tb5VDCPower.Position := round(TmpFloat);
end;

procedure TForm2.edtManualAccelXChange(Sender: TObject);
var
   StrIndex: integer;
   TmpFloat: single;
begin
  StrIndex := 1;
  TmpFloat := 0;
  MyStrToFloat(edtManualAccelX.Text, StrIndex, TmpFloat);
  tbAccelX.Position := round(TmpFloat);
end;

procedure TForm2.edtManualAccelYChange(Sender: TObject);
var
   StrIndex: integer;
   TmpFloat: single;
begin
  StrIndex := 1;
  TmpFloat := 0;
  MyStrToFloat(edtManualAccelY.Text, StrIndex, TmpFloat);
  tbAccelY.Position := round(TmpFloat);
end;

procedure TForm2.edtManualAccelZChange(Sender: TObject);
var
   StrIndex: integer;
   TmpFloat: single;
begin
  StrIndex := 1;
  TmpFloat := 0;
  MyStrToFloat(edtManualAccelZ.Text, StrIndex, TmpFloat);
  tbAccelZ.Position := round(TmpFloat);
end;

procedure TForm2.edtManualIntRefChange(Sender: TObject);
var
   StrIndex: integer;
   TmpFloat: single;
begin
  StrIndex := 1;
  TmpFloat := 0;
  MyStrToFloat(edtManualIntRef.Text, StrIndex, TmpFloat);
  tbIntRef.Position := round(TmpFloat);
end;

end.

