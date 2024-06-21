unit filter;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, global;

procedure FilterReading(NewValue: single; var CurrentValue: single; FilterFactor: single);
procedure  FilterCounts (Ch: integer);

implementation

uses Unit1;

var
  AdcFilterTimer: array [0..15] of integer;
  AdcUpdateCounter: array [0..15] of integer;
  OldAdcCounts: array [0..15] of LongInt;

//******************************************************************************************************************

procedure FilterReading(NewValue: single; var CurrentValue: single; FilterFactor: single);
var
  Filter: single;
begin
  if FilterFactor < 0 then
    begin
    CurrentValue := NewValue;
    exit;
    end;
  if abs(CurrentValue) <> 0 then
    begin
    Filter := abs(NewValue - CurrentValue) / abs(CurrentValue) * FilterFactor;
    Filter := Filter * Filter;
    if Filter > 1.0 then
      Filter := 1.0;
    end
  else
    Filter := 1.0;
  CurrentValue := (CurrentValue * (1.0 - Filter)) + (NewValue * Filter);
end;

//******************************************************************************************************
//******************************************************************************************************

procedure  FilterCounts (Ch: integer);
begin
  if abs(TestData.AdcCounts[Ch] - AdcFilteredCounts[Ch]) > 200 then
    begin
    AdcFilteredCounts[Ch] := TestData.AdcCounts[Ch];
    TestMessage(format('Filter Reset Channel %u', [Ch]));
    end;
  AdcFilteredCounts[Ch] := (AdcFilteredCounts[Ch] * 0.997) + (TestData.AdcCounts[Ch] * 0.003);
end;

end.

