unit graph;

{$mode objfpc}{$H+}

interface

uses
  Windows, Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, ExtCtrls, global,
  Buttons;

type
  TProfileGraph = class
  private
  public
    BM: TBitMap;
    GraphImage: TImage;
    HorzLowScale: single;
    HorzHighScale: single;
    GraphType: integer;
    VertLowScale: single;
    VertHighScale: single;
    BorderRect: Trect;
    GraphData: stDataArray;
    procedure Redraw;
    procedure AddPoint(X: single; Y: single);
  end;

implementation

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
  LastY, LastX, YSide, XSide, X, Y, i, Yoffset, Xoffset: integer;
  LastXValue, LastYValue, XRange, YRange: single;
  R: TRect;
  TmpStr: string;
begin
  BM := TBitMap.Create;
  BM.Width := GraphImage.Width;
  BM.Height := GraphImage.Height;
  //
  // Clear Canvas
  //
  R.Top := 0;
  R.Left := 0;
  R.Right := BM.Width;
  R.Bottom := BM.Height;
  BM.Canvas.Brush.Color := clBlack;
  BM.Canvas.FillRect(R);
  BM.Canvas.Pen.Color := clWhite;
  //
  // Final Position of Graphing Area
  //
  BorderRect.Top := 20;
  BorderRect.Left := 45;
  BorderRect.Right := BM.Width - 40;
  BorderRect.Bottom := BM.Height - 20;

  BM.Canvas.MoveTo(BorderRect.Left, BorderRect.Top);
  BM.Canvas.LineTo(BorderRect.Right, BorderRect.Top);
  BM.Canvas.LineTo(BorderRect.Right, BorderRect.Bottom);
  BM.Canvas.LineTo(BorderRect.Left, BorderRect.Bottom);
  BM.Canvas.LineTo(BorderRect.Left, BorderRect.Top);

  XRange := VertHighScale - VertLowScale;
  X := BorderRect.Right - BorderRect.Left;
  YRange := HorzHighScale - HorzLowScale;
  if (YRange = 0) then  YRange := 1;
  Y := BorderRect.Bottom - BorderRect.Top;

  if GraphType = 0 then
    begin
    BM.Canvas.Pen.Color := clDkGray;
    BM.Canvas.Pen.Style := psDot;
    for i := 0 to GraphData.NextIndex - 1 do
      begin
      Xoffset := BorderRect.Left + round(((GraphData.X[i] - VertLowScale) / XRange) * X);
      Yoffset := BorderRect.Bottom - round(((GraphData.Y[i] - HorzLowScale) / YRange) * Y);
      BM.Canvas.MoveTo(BorderRect.Left, Yoffset);
      BM.Canvas.LineTo(BorderRect.Right, Yoffset);
      BM.Canvas.MoveTo(Xoffset, BorderRect.Top);
      BM.Canvas.LineTo(Xoffset, BorderRect.Bottom);
      end;
    end;

  BM.Canvas.Pen.Style := psSolid;
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

  BM.Canvas.Font.Color := clYellow;
  YSide := 0;
  XSide := 0;
  LastX := -9999;
  LastY := -9999;
  LastXValue := -9999.0;
  LastYValue := -9999.0;
  if GraphType = 1 then
    begin
    TmpStr := format('%5.2f', [HorzHighScale]);
    BM.Canvas.TextOut(BorderRect.Left - 3 - BM.Canvas.TextWidth(TmpStr), BorderRect.Top - 6, TmpStr);
    TmpStr := format('%5.2f', [HorzLowScale]);
    BM.Canvas.TextOut(BorderRect.Left - 3 - BM.Canvas.TextWidth(TmpStr), BorderRect.Bottom - 6, TmpStr);
    TmpStr := format('%5.2f', [(HorzLowScale + HorzHighScale) / 2.0]);
    BM.Canvas.TextOut(BorderRect.Left - 3 - BM.Canvas.TextWidth(TmpStr), ((BorderRect.Top + BorderRect.Bottom) div 2) - 6, TmpStr);
    end;

  if GraphType = 0 then
    begin
    for i := 0 to GraphData.NextIndex - 1 do
      begin
      Xoffset := BorderRect.Left + round(((GraphData.X[i] - VertLowScale) / XRange) * X);
      Yoffset := BorderRect.Bottom - round(((GraphData.Y[i] - HorzLowScale) / YRange) * Y);

      if LastYValue <> GraphData.Y[i] then
        begin
        LastYValue := GraphData.Y[i];
        TmpStr := format('%5.2f', [GraphData.Y[i]]);
        if abs(LastY - Yoffset) < 10 then YSide := (Yside + 1) and 1;
        if (Yside = 1) and (abs(LastY - Yoffset) >= 10) then YSide := 0;
        LastY := Yoffset;
        if Yside = 0 then
          BM.Canvas.TextOut(BorderRect.Left - 3 - BM.Canvas.TextWidth(TmpStr), Yoffset - 6, TmpStr)
        else
          BM.Canvas.TextOut(BorderRect.Right + 3, Yoffset - 6, TmpStr);
        end;

      if LastXValue <> GraphData.X[i] then
        begin
        LastXValue := GraphData.X[i];
        TmpStr := format('%1.0f', [GraphData.X[i]]);
        if abs(LastX - Xoffset) < 10 then XSide := (Xside + 1) and 1;
        if (Xside = 1) and (abs(LastX - Xoffset) >= 10) then XSide := 0;
        LastX := Xoffset;
        if Xside = 0 then
          BM.Canvas.TextOut(Xoffset - (BM.Canvas.TextWidth(TmpStr) div 2), BorderRect.Bottom + 3, TmpStr)
        else
          BM.Canvas.TextOut(Xoffset - (BM.Canvas.TextWidth(TmpStr) div 2), BorderRect.Top - 16, TmpStr)
        end;
      end;
    end;

  BM.Canvas.Pen.Width := 1;
  GraphImage.Picture.Graphic := BM;
  BM.Free;
end;

end.

