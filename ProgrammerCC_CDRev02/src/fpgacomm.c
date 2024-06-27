//
// ******************************************************************************
//
//  Description:     FPGA Communications (CC Series Senders)
//
// Release History:
//
//  2019-07-23  RLW     Initial Release
//
// ******************************************************************************

#include "global.h"

static uint8_t     TestBuffer[10];
static uint16_t     EEPROM_Address;
//
// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
void ClearDUTData (void) {
    EEPROM_Address  = 0;
    SysVarsCC.DUT_Master_Period  = 0;
    SysVarsCC.DUT_EEPROM_Config  = 0;
    SysVarsCC.DUT_Slave_Period[0]  = 0;
    SysVarsCC.DUT_Slave_Period[1]  = 0;
    SysVarsCC.DUT_Slave_Period[2]  = 0;
    SysVarsCC.DUT_Slave_Period[3]  = 0;
    SysVarsCC.DUT_Setpoint  = 0;
    SysVarsCC.DUT_KMA_Period  = 0;
    SysVarsCC.DUT_AD7814_Counts  = 0;
    SysVarsCC.DUT_KMA_Counts  = 0;
    SysVarsCC.DUT_RV_Volts_Counts  = 0;
    SysVarsCC.DUT_RV_Amps_Counts  = 0;
    SysVarsCC.DUT_PWM_Counts  = 0;
    SysVarsCC.DUT_Master_Index  = 0;
    SysVarsCC.DUT_Feedback  = 0;
    SysVarsCC.DUT_EEPROM_Address  = 0;
    SysVarsCC.DUT_FW_Version  = 0;
    SysVarsCC.DUT_Status = 0;
    SysVarsCC.DUT_UpdateCounter = 0;
}

//
// ********************************************************************************************************************************
// ********************************************************************************************************************************
//
// This routine is called every 1mS
//
void ProcessFPGACommunications (void) {

    static uint8_t      State, UpdateDelayCounter, WaitDelay, AddressDelay, ManualDelay, CommCounter;
    static uint16_t     *AddressPtr, AddressIndex, EndAddressIndex;
    static uint16_t     NewConfigData[6];
    uint16_t            Crc16, i;

    if (SysPars.CommConfig != COMM_CC) {
        ClearDUTData();
        return;
    }
    if (SysPars.DigIO_Configuration[0] != DIG_SERIAL_IN) {
        ClearDUTData();
        return;
    }
    if (SysPars.DigIO_Configuration[1] != DIG_SERIAL_OUT) {
        ClearDUTData();
        return;
    }
    if ((SysPars.EnableBits & ENABLE_POWER) == 0) {
        ClearDUTData();
        return;
    }
    SerialUsart2.ReceiveMode = 0;
    //
    // As Rx2BufferIn is updated in an interrupt routine, we need to read it one time.  Otherwise, it could be updated after the first if statement below, and
    //  before the next one.
    //
    uint8_t BufferCount = SerialUsart2.TxRxBufferCount;

    CommCounter++;
    if (BufferCount == 27) {
        CommCounter = 0;
        EEPROM_Address  = ((SerialUsart2.TxRxBuffer[24] & 0x3) << 9) + (SerialUsart2.TxRxBuffer[23] << 1) + ((SerialUsart2.TxRxBuffer[22] >> 7) & 0x01);
        SysVarsCC.DUT_Master_Period  = ((SerialUsart2.TxRxBuffer[2] & 0x1F) << 8) + SerialUsart2.TxRxBuffer[1];
        SysVarsCC.DUT_EEPROM_Config  = ((SerialUsart2.TxRxBuffer[4] & 0x1F) << 11) + (SerialUsart2.TxRxBuffer[3] << 3) + ((SerialUsart2.TxRxBuffer[2] >> 5) & 0x07);
        SysVarsCC.DUT_Slave_Period[0]  = ((SerialUsart2.TxRxBuffer[5] & 0x7F) << 3) + ((SerialUsart2.TxRxBuffer[4] >> 5) & 0x7);
        SysVarsCC.DUT_Slave_Period[1]  = ((SerialUsart2.TxRxBuffer[7] & 0x01) << 9) + (SerialUsart2.TxRxBuffer[6] << 1) + ((SerialUsart2.TxRxBuffer[5] >> 7) & 0x01);
        SysVarsCC.DUT_Slave_Period[2]  = ((SerialUsart2.TxRxBuffer[8] & 0x07) << 7) + ((SerialUsart2.TxRxBuffer[7] >> 1) & 0x7F);
        SysVarsCC.DUT_Slave_Period[3]  = ((SerialUsart2.TxRxBuffer[9] & 0x1F) << 5) + ((SerialUsart2.TxRxBuffer[8] >> 5) & 0x1F);
        SysVarsCC.DUT_Setpoint  = ((SerialUsart2.TxRxBuffer[11] & 0x07) << 11) + (SerialUsart2.TxRxBuffer[10] << 3) + ((SerialUsart2.TxRxBuffer[9] >> 5) & 0x07);
        SysVarsCC.DUT_KMA_Period  = ((SerialUsart2.TxRxBuffer[12] & 0x1F) << 5) + ((SerialUsart2.TxRxBuffer[11] >> 3) & 0x1F);
        SysVarsCC.DUT_AD7814_Counts  = ((SerialUsart2.TxRxBuffer[13] & 0x7F) << 3) + ((SerialUsart2.TxRxBuffer[12] >> 5) & 0x7);
        SysVarsCC.DUT_KMA_Counts  = ((SerialUsart2.TxRxBuffer[15] & 0x07) << 9) + (SerialUsart2.TxRxBuffer[14] << 1) + ((SerialUsart2.TxRxBuffer[13] >> 7) & 0x01);
        SysVarsCC.DUT_RV_Volts_Counts  = ((SerialUsart2.TxRxBuffer[16] & 0x7F) << 5) + ((SerialUsart2.TxRxBuffer[15] >> 3) & 0x1F);
        SysVarsCC.DUT_RV_Amps_Counts  = ((SerialUsart2.TxRxBuffer[18] & 0x07) << 9) + (SerialUsart2.TxRxBuffer[17] << 1) + ((SerialUsart2.TxRxBuffer[16] >> 7) & 0x01);
        SysVarsCC.DUT_PWM_Counts  = ((SerialUsart2.TxRxBuffer[19] & 0x7F) << 5) + ((SerialUsart2.TxRxBuffer[18] >> 3) & 0x1F);
        SysVarsCC.DUT_Master_Index  = ((SerialUsart2.TxRxBuffer[21] & 0x01) << 9) + (SerialUsart2.TxRxBuffer[20] << 1) + ((SerialUsart2.TxRxBuffer[19] >> 7) & 0x01);
        SysVarsCC.DUT_Feedback  = ((SerialUsart2.TxRxBuffer[22] & 0x7F) << 7) + ((SerialUsart2.TxRxBuffer[21] >> 1) & 0x7F);
        SysVarsCC.DUT_EEPROM_Address  = EEPROM_Address;
        SysVarsCC.DUT_FW_Version  = (SerialUsart2.TxRxBuffer[25] << 8) + SerialUsart2.TxRxBuffer[26];
        SysVarsCC.DUT_Status = ((SysVarsCC.DUT_EEPROM_Config & 0x1F) << 7) + (SerialUsart2.TxRxBuffer[0] & 0x3F);
        SysVarsCC.DUT_UpdateCounter++;

        if (SysVarsCC.DUT_EEPROM_Address == 20) {
            AddressDelay++;
            if (AddressDelay > 100) {
                AddressDelay = 0;
                if (SysVarsCC.DUT_Setpoint != SysPars.ManualSetpoint) {
                    SysVarsCC.EEPROM_Mode = EEPROM_MODE_UPDATE_MANUAL;
                    State = 11;
                }
            }
        } else {
            AddressDelay = 0;
        }
    } else if (CommCounter > 100) {
        ClearDUTData();
    }

    if ((BufferCount == 27) && (WaitDelay > 3)) WaitDelay = 3;
    if (WaitDelay != 0) WaitDelay--;
    if (WaitDelay != 0) return;

    switch (State) {
    case 0:
        TestBuffer[0] = 0x80;                           // Command
        TestBuffer[1] = AddressIndex & 0xFF;            // Address, LSB
        TestBuffer[2] = (AddressIndex >> 8) & 0xFF;     // Address, MSB
        TestBuffer[3] = 0x00;                           // Data, LSB
        TestBuffer[4] = 0x00;                           // Data, MSG

        ManualDelay++;
        if (((SysPars.EnableBits & ENABLE_MANUAL) != 0) && (ManualDelay > 100)) {
            ManualDelay = 0;
            TestBuffer[0] = 0x80 | (1 << 2) | 1;      // Command:  Master Enable, Serial_Verify=1, Set Serial_verify
            TestBuffer[1] = 10;                      // Address, LSB
            TestBuffer[2] = 0;                      // Address, MSB
            AddressIndex = 10;
        }
        //
        // Read
        //
        if (bConfigRefresh && (SerialUsart2.TxRxBufferCount == 27)) {
            bConfigRefresh = FALSE;
            AddressPtr = (uint16_t *)&EEPROMData;
            AddressIndex = 0;
            EndAddressIndex = 6;
            SysVarsCC.EEPROM_Mode = EEPROM_MODE_READ_CONFIG;
            State = 1;
        }
        if (bProfileRefresh && (SerialUsart2.TxRxBufferCount == 27)) {
            bProfileRefresh = FALSE;
            AddressPtr = (uint16_t *)&EEPROMData;
            AddressIndex = 0;
            EndAddressIndex = 528;
            SysVarsCC.EEPROM_Mode = EEPROM_MODE_READ_PROFILE;
            State = 1;
        }
        if (bConfigUpdateConfig && (SerialUsart2.TxRxBufferCount == 27)) {
            bConfigUpdateConfig = FALSE;
            for (i = 0 ; i < 6 ; i++) {
                NewConfigData[i] = SysPars.NewConfigData[i];
            }
            AddressPtr = (uint16_t *)&NewConfigData[0];
            AddressIndex = 0;
            SysVarsCC.EEPROM_Mode = EEPROM_MODE_UPDATE_CONFIG;
            State = 10;
        }
        if (bConfigUpdateProfile && (SerialUsart2.TxRxBufferCount == 27)) {
            bConfigUpdateProfile = FALSE;
            AddressPtr = (uint16_t *)&EEPROMData.OhmsKFactor;
            AddressIndex = 2;
            SysVarsCC.EEPROM_Mode = EEPROM_MODE_UPDATE_PROFILE;
            State = 10;
        }
        if (bSerialVerify && (SerialUsart2.TxRxBufferCount == 27)) {
            bSerialVerify = FALSE;
            AddressIndex = SerialVerifyData.VerifyAddress;
            TestBuffer[0] = 0x80 | (1 << 2) | SerialVerifyData.VerifyMode;      // Command:  Master Enable, Serial_Verify=1, Set Serial_verify
            TestBuffer[1] = SerialVerifyData.VerifyAddress & 0xFF;              // Address, LSB
            TestBuffer[2] = (SerialVerifyData.VerifyAddress >> 8) & 0xFF;       // Address, MSB
            TestBuffer[3] = 0x00;                                               // DAta, LSB
            TestBuffer[4] = 0x00;                                               // Data, MSG
        }
        break;
    case 1:
        TestBuffer[0] = 0x81 | (1 << 2);                // Command:  Master Enable, Serial_Verify=1, Set Serial_verify
        TestBuffer[1] = AddressIndex & 0xFF;            // Address, LSB
        TestBuffer[2] = (AddressIndex >> 8) & 0xFF;     // Address, MSB
        TestBuffer[3] = 0x00;                           // DAta, LSB
        TestBuffer[4] = 0x00;                           // Data, MSG
        EEPROM_Address = 0xFFFF;
        State = 2;
        break;
    case 2:
        TestBuffer[0] = 0x80;                           // Command
        TestBuffer[1] = AddressIndex & 0xFF;            // Address, LSB
        TestBuffer[2] = (AddressIndex >> 8) & 0xFF;     // Address, MSB
        TestBuffer[3] = 0x00;                           // DAta, LSB
        TestBuffer[4] = 0x00;                           // Data, MSG

        if (AddressIndex == (EEPROM_Address >> 1)) {
            if (++UpdateDelayCounter >= 3) {
                UpdateDelayCounter = 0;
                AddressPtr[AddressIndex++] = SysVarsCC.DUT_Setpoint;
                if (AddressIndex == EndAddressIndex) {
                    State = 3;
                }
            }
        }
        break;
    case 3:
        TestBuffer[0] = 0x80 | 0 | (1 << 2);            // Command:  Master Enable, Serial_Verify=0, Set Serial_verify
        TestBuffer[1] = 0;                              // Address, LSB
        TestBuffer[2] = 0;                              // Address, MSB
        TestBuffer[3] = 0x00;                           // DAta, LSB
        TestBuffer[4] = 0x00;                           // Data, MSG
        SysVarsCC.EEPROM_Mode = EEPROM_MODE_NORMAL;
        State = 0;
        break;

//--------------------------

    case 10:
        TestBuffer[0] = 0x80 | (2 << 2);                            // Command:  Master Enable,  Write EEPROM (Automatically sets Serial_verify)
        TestBuffer[1] = AddressIndex & 0xFF;                        // Address, LSB
        TestBuffer[2] = (AddressIndex >> 8) & 0xFF;                 // Address, MSB
        TestBuffer[3] = AddressPtr[AddressIndex] & 0xFF;            // Data, LSB
        TestBuffer[4] = (AddressPtr[AddressIndex] >> 8) & 0xFF;     // Data, MSG
        AddressIndex++;
        if ((AddressIndex == 6) && (SysVarsCC.EEPROM_Mode == EEPROM_MODE_UPDATE_CONFIG)) {
            State = 3;
        }
        if ((AddressIndex == 528) && (SysVarsCC.EEPROM_Mode == EEPROM_MODE_UPDATE_PROFILE)) {
            State = 3;
        }
        break;

    case 11:
        TestBuffer[0] = 0x80 | (2 << 2);                            // Command:  Master Enable,  Write EEPROM (Automatically sets Serial_verify)
        TestBuffer[1] = 10;                        // Address, LSB
        TestBuffer[2] = 0;                          // Address, MSB
        TestBuffer[3] = SysPars.ManualSetpoint & 0xFF;            // Data, LSB
        TestBuffer[4] = (SysPars.ManualSetpoint >> 8) & 0xFF;     // Data, MSG
        State = 3;
        break;

    }

    SerialUsart2.TxRxBufferCount = 0;
    WaitDelay = 10;
    Crc16  = CalcCrcArray ( &TestBuffer[0], 5 );
    TestBuffer[5] = Crc16 & 0xFF;
    TestBuffer[6] = (Crc16 >> 8) & 0xFF;

    if ((TestBuffer[0] & 0x05) == 4) {
        SerialUsart2.Transmitting = TRUE;
    }

    SerialUsart2.Transmitting = TRUE;
    HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&TestBuffer[0], 7);


}
