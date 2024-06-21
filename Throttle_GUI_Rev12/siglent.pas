unit Siglent;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Unit1, telnetsshclient;

type
  TSiglentThread = class(TThread)
  private
  protected
    procedure Execute; override;
  end;

var
  SiglentValue: double;
  bNewSiglentValue: boolean;
  SiglentThread: TSiglentThread;
  bSiglentThreadRunning: boolean;
  comm: TTelnetSSHClient;

implementation

uses
    mystr2float;

procedure TSiglentThread.Execute;
var
  StrIndex, State: integer;
  bSetTrap: boolean;
  TmpStr, CommandStr: string;
  TmpFloat: single;
begin
   Sleep(1000);
   bSiglentThreadRunning := false;
   bSetTrap := false;
   State := 0;
   while not Terminated do
     begin
     Sleep(10);
        case (State) of
        0: begin
           if bSiglentThreadRunning then
              begin
                TestMessage('Connecting to Meter...', 3);
                comm:=TTelnetSSHClient.Create;
                comm.HostName:= '192.168.10.145';
                comm.TargetPort:='5024';
                comm.PrivateKeyFile := '';
                comm.UserName:='';
                comm.Password:='';
                comm.ProtocolType:=Telnet;       //Telnet or SSH
                TestMessage(comm.Connect, 3);
                if comm.Connected then
                   begin
                   TestMessage(comm.WelcomeMessage, 3);
                   State := 1;
                   end;
              end;
           end;
        1: begin
           CommandStr :='read?';
           TmpStr := comm.CommandResult(CommandStr);
           if length(TmpStr) > 5 then
              begin
              StrIndex := 1;
              MyStrToFloat(TmpStr, StrIndex, TmpFloat);
              SiglentValue := TmpFloat;
              bNewSiglentValue := true;
              end;
           if not bSiglentThreadRunning then
              begin
              comm.Disconnect;
              comm.Free;
              State := 0;
              end;
           end;
        end;
     end;
   TestMessage('Siglent Thread Terminated', 3);
   bSiglentThreadRunning := false;
end;


end.

