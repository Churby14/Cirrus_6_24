// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Filename:      adc.c
// Author:        Rick Wright
// Revision:      C
//
// Description:   This module contains functions that:
//                      - Configure ADC
//                      - Executes ADC Self-Calibration
//                      - Read A/D inputs
//
// ------------------------------------------------------------------------
//
// Revision Log:
//
//  3.2    CRU     07/26/2022 Integration of released fuel sender software and throttle SW
//
// ------------------------------------------------------------------------
// Design Details and Explanations:
// ------------------------------------------------------------------------
//
// To make testing easier, each ADC value can be over written with a Manual (Maintence) Test
// value.
//
// The Manual Test values/mode comes from the HostData serial message, which can
// be received via DIGIO1 when configured in Serial Input/Output Mode and/or from
// the RS485 interface.
//
//  NOITE:  The Test Mode is automatically exited when the Test Message
//  hasn't been received for 3 seconds.
//
// ------------------------------------------------------------------------
// Implementation Notes:
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//                              Include Files
// ------------------------------------------------------------------------

#include "global.h"

// ------------------------------------------------------------------------
//                               Definitions
// ------------------------------------------------------------------------

#define ADC_CALIBRATION_DELAY       600             // x.xxx seconds
#define ADC_SAMPLING_DELAY          1000            // x.xxx seconds

// ------------------------------------------------------------------------
//                  Local Conditional Compilation Switches
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//                          Local Variables
// ------------------------------------------------------------------------


// ********************************************************************************************************************************
//
// Start_DMA - This routine is called once.  This routine starts the ADC in a DMA circulor mode
//
// Params:    ADC_HandleTypeDef     *hadc
//            uint32_t              *pData
//            uint32_t              Length
//
// Returns:   None
//
static void Start_DMA(ADC_HandleTypeDef *hadc, uint32_t *pData, uint32_t Length) {

    STM32_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0); // Used by ADC
    STM32_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    hadc->Instance->CR |= ADC_CR_ADEN;
    STM32_Delay(5);
    hadc->DMA_Handle->XferCpltCallback = NULL;
    hadc->Instance->ISR = ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR;
    hadc->Instance->CFGR1 |= ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG;
    hadc->DMA_Handle->Instance->CCR &= ~DMA_CCR_EN;
    hadc->DMA_Handle->DmaBaseAddress->IFCR = (DMA_FLAG_GL1 << hadc->DMA_Handle->ChannelIndex);
    hadc->DMA_Handle->Instance->CNDTR = Length;
    hadc->DMA_Handle->Instance->CPAR = (uint32_t)&hadc->Instance->DR;
    hadc->DMA_Handle->Instance->CMAR = (uint32_t)pData;
    hadc->DMA_Handle->Instance->CCR |= DMA_IT_TC;
    hadc->DMA_Handle->Instance->CCR &= ~(DMA_IT_HT | DMA_IT_TE);
    hadc->DMA_Handle->Instance->CCR |= DMA_CCR_EN;
    hadc->Instance->CR |= ADC_CR_ADSTART;
}
//
// ------------------------------------------------------------------------
//
// ProcessReadAdc - This routine is called from Main Loop 1000 times per second (on average) and performs the following functions:
//                    - Waits approx 0.6 seconds after power=up, then executes the ADC Self-Calibration
//                    - Waits approx 1.0 seconds after power-up, then starts ADC conversions in continuous cirular DMA mode
//
// Params:    None
//
// Returns:   ADC counts are stored in SysVars data structure
//
// Requirement(s):  SYS-039
//
void ProcessReadAdc(void) {

    static uint8_t    State;
    static uint16_t   AdcCalibrationDelay;
    static uint16_t   AdcCounts[5];

    ExecutionStart(EXECUTION_PROCESS_ADC);

    AdcCalibrationDelay++;
    switch (State) {
    case 0:
        //
        // Wait for delay before performing Self-Calibration
        //
        if (AdcCalibrationDelay > ADC_CALIBRATION_DELAY) {
            hadc.Instance->CFGR1 &= ~(ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG);
            hadc.Instance->CR |= ADC_CR_ADCAL;
            State = 1;
        }
        break;
    case 1:
        //
        // Wait for delay before starting ADC in Circular DMA Sampling Mode
        //
        if (AdcCalibrationDelay > ADC_SAMPLING_DELAY) {
          Start_DMA(&hadc, (uint32_t *)AdcCounts, 5);        //start the DMA collecting the data
          State = 2;
        }
        break;
    case 2:
        //
        // DMA stores ADC counts into AdcCounts array, this code then moves the data into the SysVar structure for use by other logic.
        //
        // For Testing, the actual ADC Counts can be replaced/overwritten by data received from a Host via serial
        //
        if (HostData.bManualTemperatureEnable != 0) {
          SysVars.TemperatureCounts = HostData.ManualTemperature;
        } else {
          SysVars.TemperatureCounts = AdcCounts[0];
        }
        if (HostData.bManualPowerInputEnable != 0) {
          SysVars.PowerInputCounts = HostData.ManualPowerInput;
        } else {
          SysVars.PowerInputCounts = AdcCounts[1];
        }
        if (HostData.bManualKmaEnable != 0) {
          SysVars.KmaCounts = HostData.ManualKma;
        } else {
          SysVars.KmaCounts = AdcCounts[2];
        }
        if (HostData.bManual5VDCEnable != 0) {
          SysVars.PowerRail5Counts = HostData.Manual5VDCInput;
        } else {
          SysVars.PowerRail5Counts = AdcCounts[3];
        }
        if (HostData.bManualIntRefEnable != 0) {
          SysVars.IntRefCounts = HostData.ManualIntRefEnable;
        } else {
          SysVars.IntRefCounts = AdcCounts[4];
        }
        break;
    default:
        Error_Handler(FILE_ADC, __LINE__);
        break;
    }
    ExecutionEnd(EXECUTION_PROCESS_ADC);
}


