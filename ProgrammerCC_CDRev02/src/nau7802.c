/*
 * nau7802.c
 *
 *  Created on: Oct 18, 2017
 *      Author: Rick
 */

#include "global.h"

static uint8_t		NAU7802Buffer[32];
static int32_t      MuxCh;

void InitializeNAU7802 (void) {

	NAU7802Buffer[0] = NAU7802_RR;                                              // Set RR Bit
	HAL_I2C_Mem_Write( &hi2c1, 0x55, 0x00, 1, &NAU7802Buffer[0], 1, 100 );
	NAU7802Buffer[0] = 0;														// RR Bit = 0
	HAL_I2C_Mem_Write( &hi2c1, 0x55, 0x00, 1, &NAU7802Buffer[0], 1, 100 );
	HAL_Delay( 2 );

	NAU7802Buffer[0] = NAU7802_PUD;                                             // Set PUD Bit
	HAL_I2C_Mem_Write( &hi2c1, 0x55, 0x00, 1, &NAU7802Buffer[0], 1, 100 );
	HAL_Delay( 2 );

	NAU7802Buffer[0] = NAU7802_PUD | NAU7802_PUA;                               // Set PUD and PUA bits
	HAL_I2C_Mem_Write( &hi2c1, 0x55, 0x00, 1, &NAU7802Buffer[0], 1, 100 );
	HAL_Delay( 2 );

	NAU7802Buffer[0] = (0 << 5) | (1 << 4) | (1 << 0);							// PGA Output Buffer Disabled (Bit 5), PGA Bypass Enable (Bit 4), Chopper Disabled (Bit 0)
	HAL_I2C_Mem_Write( &hi2c1, 0x55, 0x1B, 1, &NAU7802Buffer[0], 1, 100 );
	HAL_Delay( 2 );

	NAU7802Buffer[0] = NAU7802_CRS_40SPS;							            // Ch 1, 320SPS
	HAL_I2C_Mem_Write( &hi2c1, 0x55, 0x02, 1, &NAU7802Buffer[0], 1, 100 );

	NAU7802Buffer[0] = (3 << 4);                                                // REG)_CHPS = turned off, high (1) state
	HAL_I2C_Mem_Write( &hi2c1, 0x55, 0x15, 1, &NAU7802Buffer[0], 1, 100 );
}

//********************************************************************************************************************************
//********************************************************************************************************************************
//
void ProcessAdc (void) {

	static uint8_t	State, MuxDelay, OldPowerEnable, PowerCounter, OvercurrentDelay;
	uint8_t     PowerEnable;
	int32_t     RV_Current;


	switch (State) {
	case 0:
		MuxCh = (MuxCh + 1) & 0xF;
		if (MuxCh == 11) {                                      // Skip Channels 11 and 12
		    MuxCh = 13;
		}

		PowerEnable = (SysPars.EnableBits & 0x1) != 0;
		if (PowerEnable && !OldPowerEnable) {
		    PowerCounter = 20;
		}
		OldPowerEnable = PowerEnable;

		if (PowerCounter != 0) {
		    if (MuxCh < 13) MuxCh = 13;
		    if (MuxCh > 14) MuxCh = 13;
		    PowerCounter--;
		}

		HAL_GPIO_WritePin( MUX_A_GPIO_Port, MUX_A_Pin, (MuxCh >> 0) & 1 );
		HAL_GPIO_WritePin( MUX_B_GPIO_Port, MUX_B_Pin, (MuxCh >> 1) & 1 );
		HAL_GPIO_WritePin( MUX_C_GPIO_Port, MUX_C_Pin, (MuxCh >> 2) & 1 );
		HAL_GPIO_WritePin( MUX_D_GPIO_Port, MUX_D_Pin, (MuxCh >> 3) & 1 );
		MuxDelay = 6;
		State = 1;
		break;
	case 1:
		NAU7802Buffer[0] = NAU7802_PUD | NAU7802_PUA | NAU7802_CS;                  // Start Conversion
		HAL_I2C_Mem_Write( &hi2c1, 0x55, 0x00, 1, &NAU7802Buffer[0], 1, 100 );
		State = 2;
		break;
	case 2:
        HAL_I2C_Mem_Read( &hi2c1, 0x55, 0x00, 1, &NAU7802Buffer[0], 1, 100 );
		if ((NAU7802Buffer[0] & NAU7802_CR) != 0) {                                 // Wait for Conversion to finish
			HAL_I2C_Mem_Read( &hi2c1, 0x55, 0x12, 1, &NAU7802Buffer[0], 3, 100 );
			State = 1;
			if (--MuxDelay == 0) {
			    SysVars.AdcCounts[MuxCh] = ((int32_t)NAU7802Buffer[0] << 24) | ((int32_t)NAU7802Buffer[1] << 16) | ((int32_t)NAU7802Buffer[2] << 8);
			    SysVars.AdcCounts[MuxCh] /= (1 << 12);
			    SysVars.AdcVolts[MuxCh] = MultDivideWithRounding64(SysVars.AdcCounts[MuxCh] - FactoryConfiguration->AdcZero[MuxCh], FactoryConfiguration->AdcSpan[MuxCh], 10000000, RESULT_32);
                if (MuxCh == 14) {
                    SysVars.PS_Current = MultDivideWithRounding64(SysVars.AdcVolts[13] - SysVars.AdcVolts[14], 100000, 54169, RESULT_32);
                    SysVars.PS_Current -= MultDivideWithRounding64(SysVars.AdcVolts[14], 1, 110, RESULT_32);
                    if (SysVars.PS_Current > 5000) {
                        if (--OvercurrentDelay == 0) {
                            SysVars.OverCurrentFlag = 1;
                        }
                    } else {
                        OvercurrentDelay = 3;
                        SysVars.OverCurrentFlag = 0;
                    }
                }
                if (MuxCh == 15) {
                    RV_Current = MultDivideWithRounding64(SysVars.AdcVolts[15] - SysVars.AdcVolts[10], 1000000, 88727, RESULT_32);
                    RV_Current -= MultDivideWithRounding64(SysVars.AdcVolts[10], 10, 110, RESULT_32);
                    if ((RV_Current <= 0) || (SysVars.AdcVolts[10] <= 0)) {
                        SysVars.RV_Ohms = 9999999;
                    } else {
                        SysVars.RV_Ohms = MultDivideWithRounding64(SysVars.AdcVolts[10], 1000000, RV_Current, RESULT_32);
                    }
                }
				State = 0;
			}
		}
		break;
	}
}

