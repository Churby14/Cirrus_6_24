// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: analog.c
// Author  : Rick Wright
// Revision: C
//
// Description:     This module contains functions that:
//                      - Filters A/D inputs
//                      - Convert Filtered A/D counts into Voltage
//                      - Convert Voltage into Engineering Units
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Revision Log:
//
//  A           CRU   12/21/2021   Baseline Release
//  B           CRU   06/13/2022   Changes per PR #19 in PACT
//  C           CRU   06/30/2022   Changes per PR #22 in PACT
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Design Details and Explanations:
// ------------------------------------------------------------------------
//
//  The digital filter equation is a discrete Infinite Impulse Response (IIR) Low-Pass type Filter:
//
//      X = (X * (1 - G)) + (Y * G)                     Eq. 1
//
//    Where:
//      X  - Filtered Output
//      G - Gain, 0 to 1
//      Y = Input Value to be Filtered
//
//   NOTE: In a real system, G would always be greater than 0.   And with G==1, no filtering is done
//
//  The above equation works very well when using floating point. But, as the STM32F072 doesn't include a
//  Floating Point Co-processor, Floating Point is not allowed in this project.  Only integers are allowed
//
//  As integers are only allowed, having the Gain be from 0 to 1 would result in only 0 and 1 being allowed.
//   And as stated above, Gain would never be set to 0, this leaves only 1, which, as stated above, would
//  provide no filtering.  So, the first change to the above equation is changing the Gain from 0 to 1, to 0 to 16384
//
//      X = (((X * (16384 - G)) + (Y * G)) / 16384          Eq. 2
//
//  Now, a Gain of 8192 in Eq. 2 would result in the same filtering as a gain of 0.5 from Eq. 1
//
//  But, there is still some work that needs to be done.  As all values are integers, the division will always round down.
//  preventing the filtered output from every reaching the input value.    Example:  Given X starts at 0, G=8000, and Y=100
//
//      T1:  X = (( 0 * (16384-8000)) + (100 * 8000)) / 16384 = 48
//      T2:  X = ((48 * (16384-8000)) + (100 * 8000)) / 16384 = 73
//      T3:  X = ((73 * (16384-8000)) + (100 * 8000)) / 16384 = 86
//      T4:  X = ((86 * (16384-8000)) + (100 * 8000)) / 16384 = 92
//      T5:  X = ((92 * (16384-8000)) + (100 * 8000)) / 16384 = 95
//      T6:  X = ((95 * (16384-8000)) + (100 * 8000)) / 16384 = 97
//      T7:  X = ((97 * (16384-8000)) + (100 * 8000)) / 16384 = 98
//      T8:  X = ((98 * (16384-8000)) + (100 * 8000)) / 16384 = 98
//      T9:  X = ((98 * (16384-8000)) + (100 * 8000)) / 16384 = 98
//
// As can be seen, while the input value is 100, the output only goes to 98
//
//  To solve this problem, the input value is multiplied by 100:
//
//      X = (((X * (16384 - G)) + ((Y * 100) * G)) / 16384
//
//  This results in X (filter output) having a fixed-point value of xxx.xx and now the equation will return 9998, or a
//  value of 99.98.  Closer to 100 (input value), but we can still make some improvements.  By multiplying
//  the input value even more, the output value can be made to approach the input value even more.  But this time,
//  we don't want to make the filtered output value any bigger, so we will divide by the same amount:
//
//      X = (((X * (16384 - G)) + ((Y * 100 * 8192) * G)) / 16384
//      return X / 8192
//
//  This gets us to 9999 or 99.99, but still not 100.00.
//
//  One more thing.  Why was the value of 8192 and 16384 used?  8192  = 2^^13 and 16384 = 2^^14.  These values can be done with
//  shift operations.  Most modern compilers will replace the multiply/divide operations and use shift operations, which are
//  way faster.
//
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
//
// Implementation Notes:
//
// ------------------------------------------------------------------------
//                              Include Files
// ------------------------------------------------------------------------

#include "global.h"

// ------------------------------------------------------------------------
//                               Definitions
// ------------------------------------------------------------------------

#define KMA_OUTPUT_AT_0_DEG         4000                 // xx.xxx %
#define KMA_OUTPUT_AT_180_DEG       96000                // xx.xxx %

#define VREFINT_CAL_ADDR            ((uint16_t*) ((uint32_t) 0x1FFFF7BA))

// ------------------------------------------------------------------------
//                  Local Conditional Compilation Switches
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//                          Local Variables
// ------------------------------------------------------------------------

static stFilterData     FilterRVOutputVoltage;
static stFilterData     FilterPowerRail5VDC;
static stFilterData     FilterKmaVoltage;
static stFilterData     FilterPowerInputVoltage;
static stFilterData     FilterTemperatureSensorVoltage;
static stFilterData     FilterRVOutputCurrentVoltage;
static stFilterData     FilterInternalReferenceCounts;
static stFilterData     FilterAccelX;
static stFilterData     FilterAccelY;
static stFilterData     FilterAccelZ;

// ------------------------------------------------------------------------
//
// DigitalFilter - Implements a Digital Filter.  For each signal being filtered, this routine needs to be called every
//                  5mS to provide the correct filtering response.
//
// Params:  *FilterData     Structure of type stFilterData.  Contains data needed to perform filtering function
//          NewData         Value to be filtered
//          Gain            Gain of filter, 1=Maximum Filtering   16384=No Filtering
//
// Returns: Filtered Value  (NOTE:  Return value is 100 times greater than input value)
//
static int32_t DigitalFilter(stFilterData *FilterData, int32_t NewData, int32_t Gain) {

    FilterData->RunningAverage = ((FilterData->RunningAverage * (16384 - Gain)) + (((int64_t)(NewData * 100) * 8192) * Gain)) / 16384;
    FilterData->FilterOutput = (int32_t)(FilterData->RunningAverage / 8192);
    return FilterData->FilterOutput;
}

// ------------------------------------------------------------------------
//
// ProcessAdcFilter:    This routine is called every 5ms (on average) from Main Loop.  This routine provides digital filtering
//                      of the KMA (Angular Sensor), Accelerometer, Temperature Sensor, Power supplies, and RV Output voltage
//                      and current.
//
// Params:      None
//
// Return:      Routine updates SysVars data structure
//
// Requirement:
//
void ProcessAdcFilter(void) {

    ExecutionStart(EXECUTION_PROCESS_ADC_FILTER);

    if ((bFactoryConfigurationValid != 0) && ((bFlashConfigurationValid != 0))) {
        SysVars.FilteredPowerInputCounts = DigitalFilter(&FilterPowerInputVoltage, SysVars.PowerInputCounts, FactoryConfiguration->PowerInputFilteringGain);
        SysVars.FilteredPowerRail5Counts = DigitalFilter(&FilterPowerRail5VDC, SysVars.PowerRail5Counts, FactoryConfiguration->PowerRail5FilteringGain);
        SysVars.FilteredIntRefCounts = DigitalFilter(&FilterInternalReferenceCounts, SysVars.IntRefCounts, FactoryConfiguration->InternalReferenceFilteringGain);
        SysVars.FilteredMasterIndex = DigitalFilter(&MasterIndexFiltering, SysVars.MasterIndex, FlashConfiguration->MasterIndexFilteringGain) / 100;
        //
        // Only run filtering if hardware is present and FactoryConfiguration data is valid
        //
        if (FactoryConfiguration->RVOutputPresent != 0) {
            SysVars.FilteredRVOutputVoltageCounts = DigitalFilter(&FilterRVOutputVoltage, SysVars.RVOutputVoltageCounts, FactoryConfiguration->RVOutputVoltageFilteringGain);
            SysVars.FilteredRVOutputCurrentCounts = DigitalFilter(&FilterRVOutputCurrentVoltage, SysVars.RVOutputCurrentCounts, FactoryConfiguration->RVOutputCurrentFilteringGain);
        }
        //
        // Only run filtering if hardware is present and FactoryConfiguration data is valid
        //
        if (FactoryConfiguration->TempSensorPresent != 0) {
            SysVars.FilteredTemperatureCounts = DigitalFilter(&FilterTemperatureSensorVoltage, SysVars.TemperatureCounts, FactoryConfiguration->TemperatureFilteringGain);
        }
        //
        // Only run filtering if hardware is present and FactoryConfiguration data is valid
        //
        if (FactoryConfiguration->KmaPresent != 0) {
            SysVars.FilteredKmaCounts = DigitalFilter(&FilterKmaVoltage, SysVars.KmaCounts, FlashConfiguration->KmaFilteringGain);
        }
        //
        // Only run filtering if hardware is present and FactoryConfiguration data is valid
        //
        if (FactoryConfiguration->AccelPresent != 0) {
            SysVars.FilteredAccelXCounts = DigitalFilter(&FilterAccelX, SysVars.AccelXCounts, FlashConfiguration->AccelXAdcFilteringGain);
            SysVars.FilteredAccelYCounts = DigitalFilter(&FilterAccelY, SysVars.AccelYCounts, FlashConfiguration->AccelYAdcFilteringGain);
            SysVars.FilteredAccelZCounts = DigitalFilter(&FilterAccelZ, SysVars.AccelZCounts, FlashConfiguration->AccelZAdcFilteringGain);
        }
    }
    ExecutionEnd(EXECUTION_PROCESS_ADC_FILTER);
}

// ------------------------------------------------------------------------
//
// ProcessAnalogReadings:  This routine is called from main loop every 10ms (on average).  This routine converts the filtered analog
//                          readings into Engineering Units.
//
// Params:      None
//
// Return:      Results of conversions are stored in SysVars data structure
//
// Requirement:
//
void ProcessAnalogReadings(void) {

    int32_t FullScale;

    ExecutionStart(EXECUTION_PROCESS_ANALOG_READINGS);

    if ((bFactoryConfigurationValid != 0) && ((bFlashConfigurationValid != 0))) {

        //
        // Calculate 3.3V power rail based on STmicro factory calibration (stored at VREFINT_CAL_ADDR) in FLASH.
        // At the STmicro factory, they connect a precision 3.3VDC power supply to the chip and then do an A/D conversion
        //      on the internal Voltage Reference.  This value is then stored into VREFINT_CAL_ADDR in FLASH. - And can not be changed
        //  Now, in a user system, we also do an A/D conversion on Internal Voltage Reference.  The A/D value read in the user system should
        //      be a ratio of the value stored at VREFINT_CAL_ADDR, multiplied by 3.3.  This should result in the "actual" 3.3VDC power
        //      supply voltage.
        //
        SysVars.FactoryIntRefCounts = *VREFINT_CAL_ADDR;          // Read factory calibration value from FLASH
        if (SysVars.FilteredIntRefCounts != 0) {
            SysVars.PowerRail33VDC = (int32_t)MultDivideWithRounding64(330000, SysVars.FactoryIntRefCounts, SysVars.FilteredIntRefCounts, RESULT_32);           // x.xxx V
        }
        //
        // Calculate Power Input Voltage and +5VDC Power Supply voltage
        //
        //  FactoryConfiguration->Calibration33 = Measured value of 3.3 Power Supply when Sender was tested at CiES
        //
        //  The 100/342 (or 100/2046) ratio is used so the KFactor value is as close as possible to 10000 during factory testing at CiES
        //
        SysVars.PowerInputVoltage = (int32_t)MultDivideWithRounding64(FilterPowerInputVoltage.FilterOutput, FactoryConfiguration->Calibration33 * 100,
                                                                  FactoryConfiguration->PowerInputKFactor * 342, RESULT_32);
        SysVars.PowerRail5VDC = (int32_t)MultDivideWithRounding64(FilterPowerRail5VDC.FilterOutput, FactoryConfiguration->Calibration33 * 100, FactoryConfiguration->PowerRail5KFactor * 2046, RESULT_32);
        //
        // Run Temperature calculations if present and Factory Configuration data is valid
        //
        if (FactoryConfiguration->TempSensorPresent != 0) {
            //
            // Convert Temperature Sensor Filtered ADC counts into volts (Temperature Input Voltage) (x.xxxx V)
            //
            SysVars.TemperatureVoltage = (int32_t)MultDivideWithRounding64(FilterTemperatureSensorVoltage.FilterOutput, FactoryConfiguration->Calibration33, 40950, RESULT_32);
            //
            // Convert Temperature Input Voltage into  Deg C (xx.xx C)
            //
            SysVars.TemperatureCelsius = (int32_t)MultDivideWithRounding64(SysVars.TemperatureVoltage - 18663, 1000, -1169, RESULT_32);
            //
            // Convert Temperature (Deg C) into Frequency:
            //
            //  Freq = 200 + T
            //
            SysVars.TemperatureFrequency = 20000 + SysVars.TemperatureCelsius;
        }
        //
        // Run KMA calculations only if present
        //
        if (FactoryConfiguration->KmaPresent != 0) {
            //
            // Convert KMA Filtered ADC counts into KMA voltage (x.xxxx V)
            //
            // The output from the KMA is approx 0.20 to 4.8V - However, the A/D input range is 0V to 3.3V -  Therefore, to read the KMA output voltage,
            //  the signal goes through a voltage divider (22K/39K), which reduces the input from 0.2 to 4.8V to 0.128 to 3.069V.  To eliminate errors due to
            //  the tolerances of the voltage divider, a K-Factor is added.  The K-Factor is set during testing of the board.
            //
            // The 100/261 ratio is used so the KFactor is as close as possible to 10000 during factory testing at CiES
            //
            SysVars.KmaVoltage = (int32_t)MultDivideWithRounding64(FilterKmaVoltage.FilterOutput, FactoryConfiguration->Calibration33 * 100, FactoryConfiguration->KmaKFactor * 261, RESULT_32);
            //
            // Calculate KMA Percentage (xxx.xxx %).  Voltage output from KMA is a function of magnetic field and Vcc.
            //      Vcc is connected to +5VDC.
            //      Therefore the KMA Percentage is:
            //          Percentage = KMA Voltage / 5VDC Voltage
            //
            if (SysVars.PowerRail5VDC > 0) {
                SysVars.KmaPercentage = (int32_t)MultDivideWithRounding64(SysVars.KmaVoltage, 10000, SysVars.PowerRail5VDC, RESULT_32);
            }
            //
            // Calculate KMA Angle (xxx.xxx Deg) based on KMA Percentage.  4% = 0 Deg    96% = 180 Deg
            //
            SysVars.KmaAngle = (int32_t)MultDivideWithRounding64(SysVars.KmaPercentage - KMA_OUTPUT_AT_0_DEG, 180000, KMA_OUTPUT_AT_180_DEG - KMA_OUTPUT_AT_0_DEG, RESULT_32);
            //
            // Calculate KMA Index based on KMA Angle     0 Deg=MIN_INDEX,  180 Deg=MAX_INDEX
            //
            SysVars.KmaIndex = (int32_t)MultDivideWithRounding64(MAX_INDEX - MIN_INDEX, SysVars.KmaAngle, 180000, RESULT_32) + MIN_INDEX;             // 400-102300 xxxx.xx
            //
            // If reverse flag is set, then 0 to 180 Deg = MAX_INDEX to MIN_INDEX
            //
            if ((FlashConfiguration->ReverseIndexKma != 0)) {
                SysVars.KmaIndex = (MAX_INDEX + MIN_INDEX) - SysVars.KmaIndex;
            }
            //
            // Calculate KMA Frequency.
            //
            // KMA Index:
            //      400-102300 (xxxx.xx)            KMA Frequency =  1638400 Hz to 6406 Hz (xxxxx.xx)
            //
            if (SysVars.KmaIndex != 0) {
              SysVars.KmaFrequency = (int32_t)MultDivideWithRounding64(655360000, 1, SysVars.KmaIndex, RESULT_32);

            }
        }
        //
        // Run Accelerometer calculations if present and FactoryConfiguration data is valid
        //
        if (FactoryConfiguration->AccelPresent != 0) {
            //
            //  Convert Filtered Accelerometer Counts into g's based on accelerometer scaling (x.xxxx g)
            //  AccelerometerScaling:  0=2000, 1=4000, 2=8000
            //
            FullScale = 2000 * (int32_t)(1u << FlashConfiguration->AccelerometerScaling);
            SysVars.AccelGravityX = (int32_t)MultDivideWithRounding64(FilterAccelX.FilterOutput - 204800, FullScale, 20480, RESULT_32);
            SysVars.AccelGravityY = (int32_t)MultDivideWithRounding64(FilterAccelY.FilterOutput - 204800, FullScale, 20480, RESULT_32);
            SysVars.AccelGravityZ = (int32_t)MultDivideWithRounding64(FilterAccelZ.FilterOutput - 204800, FullScale, 20480, RESULT_32);
        }
        //
        // Run R/V Output calculations if present and FactoryConfiguration data is valid
        //
        if (FactoryConfiguration->RVOutputPresent != 0) {
            //
            //  Convert Filtered RV Output Voltage counts into voltage (xx.xxxx V)  To eliminate voltage divider tolerance errors, conversion includes a
            //  factory calibrated K-Factor.
            //
            //  NOTE:  Filtered Counts is multiplied by 1460 so KFactor is as close as possible to 10000
            //
            SysVars.RVOutputVoltage = (int32_t)MultDivideWithRounding64(FilterRVOutputVoltage.FilterOutput, 1460, FactoryConfiguration->RVOutputVoltageKFactor, RESULT_32);
            //
            // Convert Filtered RV Output Current (voltage across currrent sense resistor) into miliamps (xxx.xxx mA)
            //  Then add-in current used by 187K/91K feedback and 187K/39K voltage divider for A/D input  (278 || 226 = 124.7K)
            //  To eliminate voltage divider tolerance errors, conversion includes a K-Factor.
            //
            //  NOTE:  Filtered Counts is multiplied by 3114 so KFactor is as close as possible to 10000
            //
            SysVars.RVOutputCurrent = (int32_t)MultDivideWithRounding64(FilterRVOutputCurrentVoltage.FilterOutput, 3114, FactoryConfiguration->RVOutputCurrentKFactor, RESULT_32);
            SysVars.RVOutputCurrent += (int32_t)MultDivideWithRounding64(SysVars.RVOutputVoltage, 1, 1247, RESULT_32);

            if (SysVars.RVOutputCurrent != 0) {
                //
                // Calculate RV Output Ohms (R = V/I).  (xxxx.xx Ohms)
                //
                SysVars.RVOutputOhms = (int32_t)MultDivideWithRounding64(SysVars.RVOutputVoltage, 10000, SysVars.RVOutputCurrent, RESULT_32);
            }
            //
            // Calculate Power being dissipated by Current Sense Resistor
            //
            // P = (I*I)/R
            //
            //  Where: I = Current (xx.xxx mA)      R = Resistance of Current Sense Resistor (which is 1 Ohm)
            //          P = xxx.xxx mW
                //
            SysVars.CurrentSensePower = (int32_t)MultDivideWithRounding64(SysVars.RVOutputCurrent, SysVars.RVOutputCurrent, 1000000, RESULT_32);
            //
            // Calculate power being dissipated by MOSFET. (xx.xxx W)
            //
            // P = I * V
            //
            //  I = R/V Current
            //  V = R/V Output Voltage - Current Sense Resistor Voltage
            //
            //  Current Sense Resistor Voltage = R/V Output Current * Current Sense Resistance
            //
            //  Current Sense Resistance = 1 Ohm
            //
            //  Current = xxx.xxx mA
            //  Voltage = xx.xxxx V
            //  Power = xx.xxx W
            //  Current Sense Voltage = xx.xxxx V
            //
            //  V (x.xxx) = I (xx.xxx mA) * R (x.x ohms) / 10000 (Scaling Factor)
            //
            int32_t CurrentSenseVoltage = (int32_t)MultDivideWithRounding64(SysVars.RVOutputCurrent, 100, 10000, RESULT_32);
            SysVars.MosfetPower = (int32_t)MultDivideWithRounding64(SysVars.RVOutputCurrent, (SysVars.RVOutputVoltage - CurrentSenseVoltage), 10000000, RESULT_32);
            //
            // Check if Internal Current Source should be turned on or off
            //
            if (FlashConfiguration->RVOutputPullup != 0) {
                EXT_PULLUP_GPIO_Port->BSRR = EXT_PULLUP_Pin;
            } else {
                EXT_PULLUP_GPIO_Port->BRR = EXT_PULLUP_Pin;
            }
        }
    }
    ExecutionEnd(EXECUTION_PROCESS_ANALOG_READINGS);
}
