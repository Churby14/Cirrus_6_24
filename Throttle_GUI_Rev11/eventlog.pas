unit eventlog;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils;

type
  TEventLogger = class(TObject)
  private
    LogFilename: string;
    LogBuffer: array [0..500000] of ANSIchar;
    LogOffset: dword;
    fLog: file;
  public
    procedure WriteStr(szString: string; WithDate: boolean);
    procedure WriteBin(var byBuff: array of byte; dwLen: dword);
    procedure Refresh;
    constructor Create(Filename: string);
    destructor Destroy; override;
  end;

var
  EventLogger: TEventLogger;
  DataLog: TEventLogger;

implementation

uses
  global;

//******************************************************************************************************
//******************************************************************************************************

constructor TEventLogger.Create(FileName: string);
begin
  LogFilename := FileName;
  LogOffset := 0;
end;

//******************************************************************************************************
//******************************************************************************************************

destructor TEventLogger.Destroy;
begin
  Refresh;
  inherited Destroy;
end;

//******************************************************************************************************
//******************************************************************************************************

procedure TEventLogger.Refresh;
var
  F1: TFileStream;
begin
  if LogOffset > 0 then
    begin
    if not FileExists(AppPath + LogFilename) then
      begin
      F1 := TFileStream.Create(AppPath + LogFilename, fmCreate);
      F1.Free;
      end;
    assignfile(flog, AppPath + LogFilename);
    reset(flog, 1);
    seek(flog, filesize(flog));
    blockwrite(flog, LogBuffer, LogOffset);
    closefile(flog);
    LogOffset := 0;
    end;
end;

//******************************************************************************************************
//******************************************************************************************************

procedure TEventLogger.WriteStr(szString: string; WithDate: boolean);
var
  i, j: dword;
  TmpStr: ansistring;
begin
  if szString = '' then
    exit;
  if WithDate then
    begin
    TmpStr := ansistring(FormatDateTime('yyyy/mm/dd hh:nn:ss - ', now));
    j := length(TmpStr);
    if (LogOffset + j) > (sizeof(LogBuffer) - 10) then
      Refresh;
    for i := 0 to j - 1 do
      LogBuffer[LogOffset + i] := TmpStr[i + 1];
    Inc(LogOffset, j);
    end;
  TmpStr := ansistring(szString);
  j := length(TmpStr);
  if (LogOffset + j) > (sizeof(LogBuffer) - 10) then
    Refresh;
  for i := 0 to j - 1 do
    LogBuffer[LogOffset + i] := TmpStr[i + 1];
  Inc(LogOffset, j);

  // If string doesn't end with a CR or LF, then add CRLF to log file

  if (LogBuffer[LogOffset - 1] <> chr($0D)) and (LogBuffer[LogOffset - 1] <> chr($0A)) then
    begin
    LogBuffer[LogOffset + 0] := chr($0D);
    LogBuffer[LogOffset + 1] := chr($0A);
    Inc(LogOffset, 2);
    end;
end;

//******************************************************************************************************
//******************************************************************************************************

procedure TEventLogger.WriteBin(var byBuff: array of byte; dwLen: dword);
begin
  if (LogOffset + dwLen) > (sizeof(LogBuffer) - 10) then
    Refresh;
  move(byBuff, &LogBuffer[LogOffset], dwLen);
  LogOffset := LogOffset + dwLen;
end;

end.

