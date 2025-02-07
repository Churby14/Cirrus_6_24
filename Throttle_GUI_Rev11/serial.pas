unit serial;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, global, windows;

type
  TSerialThread = class(TThread)
  private
  protected
    procedure Execute; override;
  end;

type
  TSerialMessage = record
    bReady: boolean;
    Cmd: byte;
    CmdLen: word;
    CmdData: array [0..10000] of byte;
  end;

var
  SerialThread: TSerialThread;

procedure ProcessSerialTimeout;

implementation

uses
  crc16, comm, unit1;

var
  NewMessage: TSerialMessage;
  SysVarTimer: integer;
  NoResponseCounter: integer;

var
  OldEEPROMMode: byte;
var
  EventTmpBuffer: array [0..32767] of byte;
  DirectTmpBuffer: array [0..32767] of byte;

//*********************************************************************************************************************************
//******************************************************************************************************************
//
// a Valid message has been received
//
procedure ProcessMessage;
var
  TmpIndex, CDCommand: byte;
  SerialResponse, PageNumber, i: integer;
  FirmwareVersion: LongInt;
  CDLength: integer;
label
  ExitHere;
begin

 Serial_Timeout := 0;
 NoResponseCounter := 0;
 bLockCommPort := TRUE;
 //
 // If RS485 Direct is selected, then adjust message to make it look like it came from
 // the Programmer.
 //
  if SetpointData.CommConfig = COMM_CD_RS485_DIRECT then
    begin
    move(NewMessage.CmdData[0], &DirectTmpBuffer, NewMessage.CmdLen);
    NewMessage.CmdData[0] := NewMessage.Cmd;
    move(&DirectTmpBuffer, NewMessage.CmdData[1], NewMessage.CmdLen);
    NewMessage.CmdLen := NewMessage.CmdLen + 1;
    NewMessage.Cmd := $BC;
    end;

  if (NewMessage.Cmd = $BC) then
    begin
    CDCommTimer := 1;
    CDCommand := NewMessage.CmdData[0];
    CDLength := NewMessage.CmdLen - 1;

    if (CDCommand = $EE) then
      begin
      FirmwarePageNumber := NewMessage.CmdData[1];
      TestMessage(format('Firmware Page = %d', [FirmwarePageNumber]),0);
      bCDSendProgramingData := true;
      end;

    if (CDCommand = $70) then
      begin
      move(NewMessage.CmdData[1], &CD_70, SizeOf(CD_70));
      bCD_70_Update := true;
      end;

    if (CDCommand = $73) then
      begin
      move(NewMessage.CmdData[1], &CD_73, CDLength);
      bCD_73_Update := true;
      end;

    if (CDCommand = $99) then
      begin
      move(NewMessage.CmdData[1], &CD_99, SizeOf(CD_99));
      bCD_99_Update := true;
      end;

    if (CDCommand = $11) then
      begin
      CDVerifyResults := NewMessage.CmdData[1];
      bUploadDone := true;
      end;

    if (CDCommand = $12) then
      begin
      bUploadDone := true;
      end;

    if (CDCommand = $13) then
      begin
      bUploadDone := true;
      end;

    if (CDCommand = $14) then
      begin
      bCD_14_Update := true;
      end;

    if (CDCommand = $71) then
      begin
      move(NewMessage.CmdData[1], &CD_71, CDLength);
      bCD_71_Update := true;
      end;

    if (CDCommand = $48) then
      begin
      move(NewMessage.CmdData[1], &CD_48, CDLength);
      bCD_48_Update := true;
      end;

    if (CDCommand = $41) then
      begin
      TmpIndex := NewMessage.CmdData[1];
      if TmpIndex < 4 then
        begin
        move(NewMessage.CmdData[2], &CD_41_Download.Partition[TmpIndex] , 512);
        if TmpIndex = 3 then
          begin
          bCD_41_Update := true;
          end
        else
            begin
            CDCommTimer := 0;
            end;
        end;
      end;

    if (CDCommand = $42) then
      begin
      TmpIndex := NewMessage.CmdData[1];
      if TmpIndex < 4 then
        begin
        move(NewMessage.CmdData[2], &CD_42_Download.Partition[TmpIndex] , 512);
        if TmpIndex = 3 then
          begin
          bCD_42_Update := true;
          end;
        end
      else
          begin
          CDCommTimer := 0;
          end;

      end;

    if (CDCommand = $43) then
      begin
      TmpIndex := NewMessage.CmdData[1];
      if TmpIndex < 4 then
        begin
        move(NewMessage.CmdData[2], &CD_43_Download.Partition[TmpIndex], 512);
        if TmpIndex = 3 then
          begin
          bCD_43_Update := true;
          end
        else
          begin
          CDCommTimer := 0;
          end;
        end;

      end;

    if (CDCommand = $44) then
      begin
      move(&NewMessage.CmdData[1], &CD_44_Index, 2);
      if CD_44_Index = CDFlashIndex then
        begin
        CDFlashIndex := CDFlashIndex + 1;
        Form1.ProgressBar1.Position:= CDFlashIndex;
        move(NewMessage.CmdData[3], &EventTmpBuffer[CD_44_Index * 256], 256);
        if CD_44_Index = 127 then
          begin
          move(&EventTmpBuffer[0], &CD_44, 32768);
          bCD_44_Update := true;
          end;
        end;
      end;
    end;
//
// Process Firmware Information Message
//
if (NewMessage.Cmd = $B6) and (NewMessage.CmdLen = SizeOf(FirmwareInfo)) then
  begin
  move(NewMessage.CmdData[0], &FirmwareInfo, SizeOf(FirmwareInfo));
  bFirmwareUpdate := true;
  end;
//
// Process EEPROM Dump Message.  Message contains Calibration, Config, and Profile data
//
if (NewMessage.Cmd = $B7) and (NewMessage.CmdLen = 12) then
  begin
  move(NewMessage.CmdData[0], &CurrentEEPROMData, 12);
  if (CurrentEEPROMData.EEPROM_Config and $14) = $14 then
    begin
    CurrentEEPROMData.TripPoint := round((CurrentEEPROMData.Trippoint  - 50) * 1.125) + 4;
    CurrentEEPROMData.HoldValue :=  round((CurrentEEPROMData.HoldValue  - 50) * 1.125) + 4;
    end;
  bSerialEEPROMDataUpdate := true;
  end;
//
// Process EEPROM Dump Message.  Message contains Calibration, Config, and Profile data
//
if (NewMessage.Cmd = $B8) and (NewMessage.CmdLen = 265) then
  begin
  EEPROMConfigIndex := NewMessage.CmdData[0];
  if (EEPROMConfigIndex < 4) then
    move(NewMessage.CmdData[1], &CurrentEEPROMData.Partition[EEPROMConfigIndex], 264);
  EEPROMConfigIndex := EEPROMConfigIndex + 1;
  if EEPROMConfigIndex >= 4 then
    begin
    EEPROMConfigIndex := 0;
    //
    // When Hold Mode is enabled, the Trip Point and Hold Value is stored in EEPROM must be converted back into an Index value.
    //
    if (CurrentEEPROMData.EEPROM_Config and $14) = $14 then
      begin
      CurrentEEPROMData.TripPoint := round((CurrentEEPROMData.Trippoint  - 50) * 1.125) + 4;
      CurrentEEPROMData.HoldValue :=  round((CurrentEEPROMData.HoldValue  - 50) * 1.125) + 4;
      end;
    bSerialGetEEPROMDataUpdate := true;
    end
  else
      bSerialGetEEPROMConfigProfileData := true;
  end;

if (NewMessage.Cmd = $B5) and (NewMessage.CmdLen = SizeOf(SysVarsCC)) then
  begin
  move(NewMessage.CmdData[0], &SysVarsCC, SizeOf(SysVarsCC));
  bGetSysVars := true;
  bSysVarsUpdate := true;
  end;

if (NewMessage.Cmd = $EE) and (NewMessage.CmdLen = 1) then
  begin
  FirmwarePageNumber := NewMessage.CmdData[0];
  if FirmwarePageNumber = 99 then
      begin
      bReprogramming := FALSE;
      end
  else
      begin
      bSendFirmwareData := TRUE;
      end;
  end;

if (NewMessage.Cmd = $70) then
  begin
    move(NewMessage.CmdData[0], &FirmwareVersion, 4);

    if (NewMessage.CmdLen = SizeOf(SysVars)) then
      begin
      move(NewMessage.CmdData[0], &SysVars, SizeOf(SysVars));
      end;

    if (SysVarsCC.EEPROM_Mode = EEPROM_MODE_NORMAL) and (OldEEPROMMode = EEPROM_MODE_READ_CONFIG) then
      begin
      bSerialGetEEPROMData := true;
      end;
    if (SysVarsCC.EEPROM_Mode = EEPROM_MODE_NORMAL) and (OldEEPROMMode = EEPROM_MODE_UPDATE_CONFIG) then
      begin
      TestMessage('EEPROM Config Update Done!', 0);
      end;
    if (SysVarsCC.EEPROM_Mode = EEPROM_MODE_NORMAL) and (OldEEPROMMode = EEPROM_MODE_READ_PROFILE) then
      begin
      FlashIndex := 0;
      NextFlashIndex := 0;
      bSerialGetEEPROMConfigProfileData := true;
      end;
    OldEEPROMMode := SysVarsCC.EEPROM_Mode;
    RVvoltage := SysVars.AdcVolts[10] / 1000.0;
    bGetSysVars := true;
    SysVarTimer := 0;
    bSysVarsUpdate := true;
    bUpdateTrend := true;
  end;

if (NewMessage.Cmd = $B2) and (NewMessage.CmdLen = 513) then
  begin
  FactoryIndex := NewMessage.CmdData[0];
  if (FactoryIndex < 4) then
    move(NewMessage.CmdData[1], &FactoryData.Partition[FactoryIndex] , 512);
  FactoryIndex := FactoryIndex + 1;
  if FactoryIndex >= 4 then
    begin
    FactoryIndex := 0;
    bFactoryUpdate := true;
    end
  else
      bSerialGetFactory := true;
  end;


ExitHere:
  bCommandPending := false;

end;

//*********************************************************************************************************************************
//******************************************************************************************************************

procedure ProcessSerialTimeout;

begin
if (Serial_Timeout > 10000) and (CommPortList.Count > 0) and (not bLockCommPort) then
  begin
  Serial_Timeout := 0;
  if PortListIndex >= CommPortList.Count then PortListIndex := 0;
  TestMessage(format('---> Comm Timeout on %s! <---', [CommPortList.Strings[PortListIndex]]), 0);
  SerialThread.Terminate;
  Sleep(100);
  SerialThread.Free;
  CommPort.Free;
  Sleep(500);
  PortListIndex := PortListIndex + 1;
  CommPort := TCommPort.Create;
  SerialThread := TSerialThread.Create(False);
  bResetSerial := False;
  end;

end;

//*********************************************************************************************************************************

var
  OldCommand: integer;
  GetCCTimer: integer;
  byBuff: array[0..3010] of byte;
  BufferLength: integer;

//*********************************************************************************************************************************

procedure ProcesSendThroughProgrammer;
var
  Trippoint, Holdvalue: word;
  TMP: integer;

begin
 BufferLength := 0;
  byBuff[0] := $aa;
  byBuff[1] := $aa;
  byBuff[2] := $aa;
  byBuff[3] := $aa;
  byBuff[4] := $aa;
  byBuff[5] := $00;    // Sync

  if bSerialRelayMessage then
    begin
    bSerialRelayMessage := false;
    byBuff[6] := $BC;                                     // Command    Relay Command to CD Sender
    byBuff[7] := CDCommBufferLength and $FF;           // Length, LSB
    byBuff[8] := (CDCommBufferLength shr 8) and $FF;   // Length, MSB
    move(&CDCommBuffer,&byBuff[9], CDCommBufferLength);
    BufferLength := 8 + CDCommBufferLength;
    if CDCommBuffer[0] = $14 then TestMessage('Clear CD Event Log', 1);
    if CDCommBuffer[0] = $41 then TestMessage(format('Downloading CD Configuration Data, Part %u of 4', [CDCOmmBuffer[1] + 1]), 1);
    if CDCommBuffer[0] = $42 then TestMessage(format('Downloading CD Factory Data, Part %u of 4', [CDCOmmBuffer[1] + 1]), 1);
    if CDCommBuffer[0] = $43 then TestMessage(format('Downloading CD Fuzzy Configuration Data, Part %u of 4', [CDCOmmBuffer[1] + 1]), 1);
    if CDCommBuffer[0] = $44 then TestMessage(format('Downloading CD Event Log, Part %u of 128', [CDCOmmBuffer[1] + 1]), 1);
    if CDCommBuffer[0] = $48 then TestMessage('Downloading CD Error Status Inforation', 1);
    if (CDCommBuffer[0] = $70) and (OldCommand <> $70) then TestMessage('Downloading CD System Variables', 1);
    if (CDCommBuffer[0] = $73) then TestMessage('Downloading Foreign Sensor System Variables', 1);
    if CDCommBuffer[0] = $71 then TestMessage('Downloading CD Process Timing', 1);
    if CDCommBuffer[0] = $99 then TestMessage('Downloading CD Firmware Information', 1);
    if CDCommBuffer[0] = $CC then TestMessage('Uploading New Sender Firmware', 1);
    OldCommand := CDCommBuffer[0];
    end
  else if bSendFirmwareData then
    begin
    bSendFirmwareData := FALSE;
    byBuff[6] := $EE;    // Command     Issue Firmware Upload Command
    byBuff[7] := 2052 and $FF;    // Length, LSB
    byBuff[8] := (2052 shr 8) and $FF;    // Length, MSB
    move(&FirmwarePageNumber, &byBuff[9], 4);
    move(&FirmwareBinaryBuffer[FirmwarePageNumber * 2048], &byBuff[13], 2048);
    BufferLength := 2060;
    end
  else if bIssueReprogrammingCommand then
    begin
    bIssueReprogrammingCommand := FALSE;
    byBuff[6] := $BC;    // Command     Issue Reprogramming Command
    byBuff[7] := $04;    // Length, LSB
    byBuff[8] := $00;    // Length, MSB
    byBuff[9] := $12;     //
    byBuff[10] := $34;    //
    byBuff[11] := $56;    //
    byBuff[12] := $78;    //
    BufferLength := 12;
    bReprogramming := true;
    end
  else if bSerialGetVersion then
    begin
    bSerialGetVersion := false;
    byBuff[6] := $B6;    // Command     Get Version/CRC
    byBuff[7] := $00;    // Length, LSB
    byBuff[8] := $00;    // Length, MSB
    BufferLength := 8;
    TestMessage('Downloading Programming Unit Firmware Infor', 1);
    end
  else if bSerialGetEEPROMData then
    begin
    bSerialGetEEPROMData := false;
    byBuff[6] := $B7;    // Command     Get EEPROM Config Data
    byBuff[7] := $00;    // Length, LSB
    byBuff[8] := $00;    // Length, MSB
    BufferLength := 8;
    TestMessage('Downloading CC EEPROM Config Data', 1);
    end
  else if bSerialGetEEPROMConfigProfileData then
    begin
    bSerialGetEEPROMConfigProfileData := false;
    byBuff[6] := $B8;    // Command     Get EEPROM Config/Profile Data
    byBuff[7] := $01;    // Length, LSB
    byBuff[8] := $00;    // Length, MSB
    byBuff[9] := EEPROMConfigIndex;    // Data
    BufferLength := 9;
    TestMessage('Downloading CC EEPROM Config/Profile Data', 1);
    end
  else if bSerialWriteProfile then
    begin
    bSerialWriteProfile := false;
    byBuff[6] := $B9;                                  // Command     Write new Profile
    byBuff[7] := Sizeof(NewEEPROMData) and $FF;           // Length, LSB
    byBuff[8] := (Sizeof(NewEEPROMData) shr 8) and $FF;    // Length, MSB
    Trippoint := NewEEPROMData.Trippoint;
    HoldValue := NewEEPROMData.HoldValue;
    if (NewEEPROMData.EEPROM_Config and $14) = $14 then
      begin
      NewEEPROMData.Trippoint := round((NewEEPROMData.Trippoint - 4) / 1.125) + 50;
      NewEEPROMData.HoldValue := round((NewEEPROMData.HoldValue - 4) / 1.125) + 50;
      end;
    move(&NewEEPROMData, &byBuff[9], SizeOf(NewEEPROMData));
    NewEEPROMData.Trippoint := Trippoint;
    NewEEPROMData.HoldValue := HoldValue;
    BufferLength := 8 + Sizeof(NewEEPROMData);
    TestMessage('Uploading New CC Profile Data', 1);
    end
  else if bSerialVerifyMode then
    begin
    bSerialVerifyMode := false;
    byBuff[6] := $BA;                                     // Command    Serial_verify Mode
    byBuff[7] := Sizeof(SerialVerifyData) and $FF;           // Length, LSB
    byBuff[8] := (Sizeof(SerialVerifyData) shr 8) and $FF;    // Length, MSB
    move(&SerialVerifyData, &byBuff[9], SizeOf(SerialVerifyData));
    BufferLength := 8 + Sizeof(SerialVerifyData);
    TestMessage('Setting CC Verify Mode', 1);
    end
  else if bSerialSetFactory then
    begin
    bSerialSetFactory := false;
    byBuff[6] := $B4;    // Command     Set Factory Data
    byBuff[7] := SizeOf(FactoryData) and $FF;    // Length, LSB
    byBuff[8] := SizeOf(FactoryData) shr 8;    // Length, MSB
    move(&FactoryData, &byBuff[9], SizeOf(FactoryData));
    BufferLength := 8 + SizeOf(FactoryData);
    end
  else if bSerialGetFactory then
    begin
    bSerialGetFactory := false;
    byBuff[6] := $B2;    // Command     Get Factory Data
    byBuff[7] := $01;    // Length, LSB
    byBuff[8] := $00;    // Length, MSB
    byBuff[9] := FactoryIndex;    // Data
    BufferLength := 9;
    end
  else if bGetSysVars then
    begin
    bGetSysVars := false;
    inc(GetCCTimer);
    if not bGetCCData then GetCCTimer := 0;
    if GetCCTimer = 2 then
        begin
        GetCCTimer := 0;
        byBuff[6] := $B5;    // Command     Get Test Data
        byBuff[7] := 0;
        byBuff[8] := 0;
        BufferLength := 8;
        end
    else
        begin
        byBuff[6] := $70;    // Command     Get Test Data
        byBuff[7] := Sizeof(SetpointData) and $FF;    // Length, LSB
        byBuff[8] := (Sizeof(SetpointData) shr 8) and $FF;    // Length, MSB

        if (bRefreshConfig) then
          begin
           bRefreshConfig := FALSE;
           SetpointData.EnableBits := SetpointData.EnableBits or ENABLE_REFRESH_CONFIG;
           end
         else
           SetpointData.EnableBits := SetpointData.EnableBits and not ENABLE_REFRESH_CONFIG;

         if (bRefreshProfile) then
           begin
           bRefreshProfile := FALSE;
           SetpointData.EnableBits := SetpointData.EnableBits or ENABLE_REFRESH_PROFILE;
           end
         else
           SetpointData.EnableBits := SetpointData.EnableBits and not ENABLE_REFRESH_PROFILE;

         if (bUpdateConfig) then
           begin
           bUpdateConfig := FALSE;
           SetpointData.EnableBits := SetpointData.EnableBits or ENABLE_UPDATE_CONFIG;
           end
         else
           SetpointData.EnableBits := SetpointData.EnableBits and not ENABLE_UPDATE_CONFIG;
         move(&SetpointData, &byBuff[9], SizeOf(SetpointData));
         BufferLength := 8 + SizeOf(SetpointData);
        end;
    end
  else
    begin
    SysVarTimer := SysVarTimer + 1;
    if SysVarTimer > 500 then
      begin
      SysVarTimer := 0;
      bGetSysVars := true;
      end;
    BufferLength := 0;
    end;

end;

//*********************************************************************************************************************************

procedure ProcessSendDirect;
begin
  BufferLength := 0;
  byBuff[0] := $aa;
  byBuff[1] := $aa;
  byBuff[2] := $aa;
  byBuff[3] := $aa;
  byBuff[4] := $aa;
  byBuff[5] := $00;    // Sync

  if bSerialRelayMessage then
    begin
    bSerialRelayMessage := false;
    byBuff[6] := CDCommBuffer[0];
    byBuff[7] := (CDCommBufferLength - 1) and $FF;           // Length, LSB
    byBuff[8] := ((CDCommBufferLength - 1) shr 8) and $FF;   // Length, MSB
    move(&CDCommBuffer[1],&byBuff[9], CDCommBufferLength - 1);
    BufferLength := 8 + CDCommBufferLength - 1;
    end;

end;

//*********************************************************************************************************************************

procedure ProcessReceiveData;

var
   BytesRead, i: integer;
   SerialByte: byte;
   byReadBuff: array[0..3010] of byte;
begin
 BytesRead := CommPort.ReadBytes(byReadBuff);
 for i := 0 to BytesRead - 1 do
   begin
   SerialByte := byReadBuff[i];
   case (SerialCase) of
     0:
       begin
       if (SerialByte = $AA) then
         begin
         SyncCount := SyncCount + 1;
         end
       else if (SerialByte = $00) and (SyncCount > 3) then
        begin
        Inc(SerialCase);
        end
       else
        begin
        SyncCount := 0;
        end;
       end;
     1:
       begin
       NewMessage.Cmd := SerialByte;
       Crc16Result := CalcCrc16(SerialByte, 0);
       Inc(SerialCase);
       end;
     2:
       begin
       CmdLen := SerialByte;
       Crc16Result := CalcCrc16(SerialByte, Crc16Result);
       Inc(SerialCase);
       end;
     3:
       begin
       CmdLen := CmdLen + (SerialByte shl 8);
       Crc16Result := CalcCrc16(SerialByte, Crc16Result);
       NewMessage.CmdLen := CmdLen;
       SerialOffset := 0;
       Inc(SerialCase);
       if CmdLen = 0 then
         Inc(SerialCase);
       if CmdLen > 4000 then
         SerialCase := 0;
       end;
     4:
       begin
       NewMessage.CmdData[SerialOffset] := SerialByte;
       Inc(SerialOffset);
       Crc16Result := CalcCrc16(SerialByte, Crc16Result);
       Dec(CmdLen);
       if CmdLen = 0 then
         begin
         Inc(SerialCase);
         end;
       end;
     5:
       begin
       CmdCrc := SerialByte;
       Inc(SerialCase);
       end;
     6:
       begin
       CmdCrc := CmdCrc + (SerialByte shl 8);
       if CmdCrc = Crc16Result then
         begin
         queryPerformanceCounter(qProcessMessage);
         ProcessMessage();
         end
       else
           begin
           TestMessage('Bad CRC', 0);
           end;
       SerialCase := 0;
       end;
     end;
   end;
end;

//*********************************************************************************************************************************

procedure TSerialThread.Execute;
var
  i: integer;
  qTimeout, qnow: int64;
  BytesRead: integer;
  TMP: integer;
  byReadBuff: array[0..3010] of byte;
begin
   Sleep(1000);
   ThreadCounter := 10;
   bGetSysVars := true;
   queryPerformanceCounter(qnow);
   qlast := qnow;
   BufferLength := 0;
   while not Terminated do
     begin
     Sleep(1);

     {if bUploadCDFirmwareStarted or bReprogramming then
       begin
       // Do Nothing while firmware is being downloaded to CD Sender or Programming Unit
       end
     else }if bCommandPending then
       begin
       //
       // Command has been send, now wait for a response.
       //
       queryPerformanceCounter(qTimeout);
       if ((qTimeout - qSendCommand) / qFreq) > 0.5 then      // Error if no response for 0.5 seconds
         begin
         if byBuff[6] = $BC then
           TestMessage(format('No Response Cmd %X %X', [byBuff[6], byBuff[9]]), 0)
         else
           TestMessage(format('No Response Cmd %X', [byBuff[6]]), 0);
         bCommandPending := false;
         NoResponseCounter := NoResponseCounter + 1;
         if NoResponseCounter >= 3 then                     // Set Serial_Timeout if no response for 3 attempts
           begin
           NoResponseCounter := 0;
           Serial_Timeout := 20000;
           end;
         end;
       end
     else
       begin
       ProcessCDCommunications();
       if SetpointData.CommConfig = COMM_CD_RS485_DIRECT then
         begin
         ProcessSendDirect()
         end
       else
         begin
         ProcesSendThroughProgrammer();
         end;
       //
       // BufferLength will be set if there is something that needs to be sent
       //
       if BufferLength <> 0 then
         begin

         Crc16Result := 0;
         for i := 6 to BufferLength do Crc16Result := CalcCrc16(byBuff[i], Crc16Result);
         byBuff[BufferLength + 1] := Crc16Result and $FF;
         byBuff[BufferLength + 2] := (Crc16Result shr 8) and $FF;
         BytesRead := 1;             // Clear the Receive Buffer
         while BytesRead <> 0 do
           begin
           BytesRead := CommPort.ReadBytes(byReadBuff);
           end;

         queryPerformanceCounter(qSendCommand);
         CommPort.WriteBytes(byBuff, BufferLength + 3);
         bCommandPending := true;
         end;
       end;

     ProcessReceiveData();
     end;
   Sleep(1);
 end;

end.

