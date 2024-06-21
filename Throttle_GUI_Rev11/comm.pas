unit Comm;

{$mode objfpc}{$H+}

interface

uses
Windows, Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, ExtCtrls,
Buttons, Registry;

type
  TCommPort = class(TObject)
  private
    fhPort: THANDLE;
  public
    constructor Create;
    destructor Destroy; override;
    function ReadBytes(var byBuff: array of byte): longword;
    procedure WriteBytes(var byBuff: array of byte; dwLen: longword);
  end;

var
  CommPortList : TStringList;
  CommPort: TCommPort;
  PortListIndex: integer;

procedure GenerateCommPortList;


implementation

uses
  global;

  //******************************************************************************************************************

procedure GenerateCommPortList;
var
  i: integer;
  reg:  TRegistry;
  TmpList: TStringList;
begin
  CommPortList := TStringList.Create;
  //
  // Get list of all existing COM ports from registry
  //
  reg := TRegistry.Create(KEY_READ or $0100);
  PortListIndex := 0;
  try
    TmpList := TSTringList.Create;
    reg.RootKey := HKEY_LOCAL_MACHINE;
    if reg.KeyExists('HARDWARE') and reg.OpenKey('HARDWARE', FALSE) then
      begin
      if reg.KeyExists('DEVICEMAP') and reg.OpenKey('DEVICEMAP', FALSE) then
        begin
        if reg.KeyExists('SERIALCOMM') and reg.OpenKey('SERIALCOMM', FALSE) then
          begin
          reg.GetValueNames(TmpList);
          for i := 0 to TmpList.Count - 1 do
            CommPortList.Add(reg.ReadString(TmpList[i]));
          end;
        end;
      end;
  finally
  end;
  reg.Free;
end;

//******************************************************************************************************************

constructor TCommPort.Create;
var
  sPort: string;
  config: string;
  dcb: TDCB;
  myTimeouts: TCOMMTIMEOUTS;
label
  TryAgain;
begin

  if CommPortList.Count = 0 then exit;
TryAgain:
if PortListIndex >= CommPortList.Count then PortListIndex := 0;
  sPort := format('\\.\%s', [CommPortList.Strings[PortListIndex]]);

  fhPort := CreateFile(PChar(sPort), GENERIC_READ or GENERIC_WRITE, 0, nil, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  if fhPort = INVALID_HANDLE_VALUE then
    begin
    PortListIndex := PortListIndex + 1;
    if PortListIndex >= CommPortList.Count then exit;
    goto TryAgain;
    end;
  SetupComm(fhPort, 8192, 8192);
  GetCommState(fhPort, dcb);
  config := 'baud=115200 parity=n data=8 stop=1' + #0;
  BuildCommDCB(PChar(config), dcb);
  SetCommState(fhPort, dcb);
  myTimeouts.ReadIntervalTimeout := MAXDWORD;
  myTimeouts.ReadTotalTimeoutMultiplier := 0;
  myTimeouts.ReadTotalTimeoutConstant := 0;
  myTimeouts.WriteTotalTimeoutMultiplier := 1;
  myTimeouts.WriteTotalTimeoutConstant := 100;
  SetCommTimeouts(fhPort, myTimeouts);
end;

//******************************************************************************************************************

destructor TCommPort.Destroy;
begin
  if (fhPort <> INVALID_HANDLE_VALUE) then
    CloseHandle(fhPort);
  inherited Destroy;
end;

//******************************************************************************************************************

function TCommPort.ReadBytes(var byBuff: array of byte): longword;
var
  dwBytesRead: longword;
begin
  ReadFile(fhPort, byBuff, 1000, dwBytesRead, nil);
  Result := dwBytesRead;
end;

//******************************************************************************************************************

procedure TCommPort.WriteBytes(var byBuff: array of byte; dwLen: longword);
var
  dwBytesWritten: longword;
begin
  WriteFile(fhPort, byBuff, dwLen, dwBytesWritten, nil);
end;

end.

