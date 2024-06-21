unit mystr2float;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, math;

procedure MyStrToFloat(str: string; var StrIndex: integer; var FloatValue: single);

implementation

//******************************************************************************************************************

procedure MyStrToFloat(str: string; var StrIndex: integer; var FloatValue: single);
var
   ExponValue, strlength: integer;
   Ch: char;
   bExponSign, bExpon, bSign, bDecimal: boolean;
   DecimalScaler: single;
begin
FloatValue := 0.0;
strlength := length(str);
bDecimal := False;
bExpon := false;
bExponSign := false;
ExponValue := 0;
bSign := False;
DecimalScaler := 0.1;

if StrIndex > strlength then exit;
Ch := str[StrIndex];
if (Ch = '-') then
  begin
  bSign := True;
  StrIndex := StrIndex + 1;
  end;
if (Ch = '+') then
  begin
  StrIndex := StrIndex + 1;
  end;

while (true) do
  begin
  if StrIndex > strlength then break;
  Ch := str[StrIndex];
  if bExpon then
    begin
    if (Ch >= '0') and (Ch <= '9') then
      begin
      ExponValue := (ExponValue * 10) + (Ord(Ch) - Ord('0'));
      StrIndex := StrIndex + 1;
      end;
    end
  else if Ch = '.' then
    begin
    StrIndex := StrIndex + 1;
    bDecimal := True;
    end
  else if (Ch >= '0') and (Ch <= '9') then
    begin
    StrIndex := StrIndex + 1;
    if bDecimal then
      begin
      FloatValue := FloatValue + ((Ord(Ch) - Ord('0')) * DecimalScaler);
      DecimalScaler := DecimalScaler / 10.0;
      end
    else
      begin
      FloatValue := (FloatValue * 10.0) + (Ord(Ch) - Ord('0'));
      end;
    end
  else if (Ch = 'E') then
    begin
    StrIndex := StrIndex + 1;
    Ch := str[StrIndex];
    if (Ch = '-') then
       begin
       bExponSign := True;
       StrIndex := StrIndex + 1;
       end
    else if (Ch = '+') then
      begin
      StrIndex := StrIndex + 1;
      end;
    bExpon := true;
    end
  else
    begin
    break;
    end;
  end;
  if bExpon then
    begin
    if bExponSign then ExponValue := -ExponValue;
    FloatValue := FloatValue * Power(10, ExponValue);
    end;
  if bSign then FloatValue := -FloatValue;
end;

end.

