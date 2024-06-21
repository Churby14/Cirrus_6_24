unit Unit4;

{$mode ObjFPC}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, StdCtrls, ExtCtrls,
  global, mystr2float, graph;

type

  { TForm4 }

  TForm4 = class(TForm)
    leSensorAOutput: TLabeledEdit;
    leSensorBOutput: TLabeledEdit;
    Procedure InitializeForm;


  private

  public

  end;

var
  Form4: TForm4;

implementation

{$R *.lfm}


procedure TForm4.InitializeForm;
begin

end;

end.

