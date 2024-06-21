unit parse;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, ExtCtrls,
  StdCtrls, Buttons, StrUtils, global, mystr2float;


type
  stParsePoint = record
    Point1: single;
    Point2: single;
  end;

type
  stParseResults = record
    ParsePoint: array [0..100] of stParsePoint;
    ParseLength: LongInt;
  end;

type
  stRecordResults = record
    RecordCount: integer;
    RecordStr: array [0..100] of string;
  end;

type
  stFieldResults = record
    FieldCount: integer;
    FieldValue: array [0..100] of single;
  end;

var
  RecordResults: stRecordResults;
  FieldResults: stFieldResults;

function ParseRecords(RecordStr: string; var ParseResults: stParseResults): integer;
procedure ParseProfileRecord(RecordStr: string; var RecordResults: stRecordResults);
function ParseFields(FieldStr: string; var FieldResults: stFieldResults):integer;
function UpdateProfile (ProfileStr: string; ErrorLabel: Tlabel; var NewTable: stParseResults) : boolean;

implementation

//***********************************************************************************************************************

function GetChar (str: string; var strindex: integer) : char;
begin
  Result := ' ';
  if strindex > length(str) then exit;
  Result := str[strindex];
  strindex := strindex + 1;
end;

//***********************************************************************************************************************

function ParseFields(FieldStr: string; var FieldResults: stFieldResults):integer;
var
  CurrentStrIndex, Index, StrIndex: integer;
  Ch: char;
  TmpStr: string;
  TmpFloat: single;
begin
  Result := 0;
  FieldResults.FieldCount := 0;
  FieldStr := StringReplace(FieldStr, ' ', '', [rfReplaceAll]);         // Remove whitespace
  StrIndex := 1;
  while (True) do
    begin
    CurrentStrIndex := StrIndex;
    MyStrToFloat(FieldStr, StrIndex, TmpFloat);
    if CurrentStrIndex = StrIndex then
       begin
       Result := -5;
       break;
       end;
    FieldResults.FieldValue[FieldResults.FieldCount] := TmpFloat;
    FieldResults.FieldCount := FieldResults.FieldCount + 1;
    if StrIndex > length(FieldStr) then break;
    Ch := GetChar(FieldStr, StrIndex);
    if Ch <> ',' then
      begin
      Result := -3;                      // Missing Comma
      break;
      end;
    if FieldResults.FieldCount = 100 then
      begin
      Result := -4;                      // Too many pairs
      break;
      end;
    end;
end;

//***********************************************************************************************************************

procedure ParseProfileRecord(RecordStr: string; var RecordResults: stRecordResults);
var
  Index: integer;
  TmpStr: string;
begin
  RecordResults.RecordCount := 0;
  while (True) do
    begin
    Index := pos(']', RecordStr);
    if Index = 0 then
      exit;
    Index := PosEx(',', RecordStr, Index);
    if Index <> 0 then
      begin
      TmpStr := trim(midstr(RecordStr, 1, Index - 1));
      RecordStr := trim(midstr(RecordStr, Index + 1, 10000));
      end
    else
      begin
      TmpStr := RecordStr;
      RecordStr := '';
      end;
    TmpStr := midstr(TmpStr, 2, length(TmpStr) - 2);
    if TmpStr = '' then
      exit;
    RecordResults.RecordStr[RecordResults.RecordCount] := TmpStr;
    RecordResults.RecordCount := RecordResults.RecordCount + 1;
    if RecordStr = '' then
      break;
    end;
end;

//***********************************************************************************************************************

function ParseRecords(RecordStr: string; var ParseResults: stParseResults): integer;
var
  CurrentStrIndex, i, StrLen, StrIndex: integer;
  Ch: char;
  TmpFloat: single;
begin
 Result := 0;
 StrLen := length(RecordStr);
 for i := 0 to 99 do
   begin
   ParseResults.ParsePoint[i].Point1 := 0.0;
   ParseResults.ParsePoint[i].Point2 := 0.0;
   end;
 ParseResults.ParseLength := 0;
 RecordStr := StringReplace(RecordStr, ' ', '', [rfReplaceAll]);         // Remove whitespace
 StrIndex := 1;
 while (True) do
   begin
   Ch := GetChar(RecordStr, StrIndex);
   if Ch <> '[' then
     begin
     Result := -1;                      // Missing Left Bracket
     break;
     end;
   CurrentStrIndex := StrIndex;
   MyStrToFloat(RecordStr, StrIndex, TmpFloat);
   if CurrentStrIndex = StrIndex then
     begin
     Result := -5;
     break;
     end;
   ParseResults.ParsePoint[ParseResults.ParseLength].Point1 := TmpFloat;
   Ch := GetChar(RecordStr, StrIndex);
   if Ch <> ',' then
     begin
     Result := -3;                      // Missing Comma
     break;
     end;
   CurrentStrIndex := StrIndex;
   MyStrToFloat(RecordStr, StrIndex, TmpFloat);
   if CurrentStrIndex = StrIndex then
     begin
     Result := -6;
     break;
     end;
   ParseResults.ParsePoint[ParseResults.ParseLength].Point2 := TmpFloat;
   Ch := GetChar(RecordStr, StrIndex);
   if Ch <> ']' then
     begin
     Result := -2;                      // Missing Right Bracket
     break;
     end;
   ParseResults.ParseLength := ParseResults.ParseLength + 1;
   if StrIndex > length(RecordStr) then break;
   Ch := GetChar(RecordStr, StrIndex);
   if Ch <> ',' then
     begin
     Result := -3;                      // Missing Comma
     break;
     end;
   if ParseResults.ParseLength = 63 then
     begin
     Result := -4;                      // Too many pairs
     break;
     end;
   end;
end;

//******************************************************************************************************
//
// Converts Profile String into Index/Setpoint pairs with error checking
//
function UpdateProfile (ProfileStr: string; ErrorLabel: Tlabel; var NewTable: stParseResults) : boolean;
var
  Status, i: integer;
  CI: single;

begin
  Result := false;
 NewTable.ParseLength := 0;
 Status := ParseRecords(ProfileStr, NewTable);
 if (Status = 0) then
     begin
     if NewTable.ParseLength < 2 then Status := -15;
     if (Status = 0) and (NewTable.ParsePoint[0].Point1 <> 18) then Status := -10;
     if (Status = 0) and (NewTable.ParsePoint[NewTable.ParseLength - 1].Point1 <>  527) then Status := -11;
     if (Status = 0) then
       begin
       CI := -1;
       for i := 0 to NewTable.ParseLength - 1 do
         begin
         if (NewTable.ParsePoint[i].Point1 <= CI) then
           begin
           Status := -14;
           break;
           end;
         CI := NewTable.ParsePoint[i].Point1;
         if NewTable.ParsePoint[i].Point2 < 0.0 then
           begin
           Status := -12;
           break;
           end;
         end;
       end;
     end;

 if Status <> 0 then
     begin
     if Status = -1 then ErrorLabel.Caption := 'Error: Missing Left Bracket';
     if Status = -2 then ErrorLabel.Caption := 'Error: Missing Right Bracket';
     if Status = -3 then ErrorLabel.Caption := 'Error: Missing Comma';
     if Status = -4 then ErrorLabel.Caption := 'Error: Too many Pairs, Maximum is 63';
     if Status = -15 then ErrorLabel.Caption := 'Error: Too few Pairs, Minimum is 2';
     if Status = -10 then ErrorLabel.Caption := 'Error: Index of first pair MUST be 18';
     if Status = -11 then ErrorLabel.Caption := 'Error: Index of last pair MUST be 527';
     if Status = -12 then ErrorLabel.Caption := 'Error: Setpoint must be greater than 0.0';
     if Status = -14 then ErrorLabel.Caption := 'Error: Each Index must be bigger than the previous Index';
     end
 else
   begin
   ErrorLabel.Caption := '';
   Result := true;
   end;

end;

end.

