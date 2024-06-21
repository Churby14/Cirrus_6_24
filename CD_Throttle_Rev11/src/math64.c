// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Filename: math64.c
// Author  : Rick Wright/Charlie Urban
// Revision: 3.1
//
//  Description:       Provides 64-bit match support
//
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Revision Log:
//
//  A           CRU   Baseline Release   12/21/2021
//  B           RLW 06/27/2022     Changes per PR #19 in PACT
//  C           CRU   06/30/2022   Changes per PR #22 in PACT
//  3.1         CRU   12/20/2022   Added unsigned 8-bit rounding
//
// ------------------------------------------------------------------------
// Design Details and Explanations:
// ------------------------------------------------------------------------

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
#define   MAX_U8   0xFF
#define   MAX_16   0x7FFF
#define   MAX_32   0x7FFFFFFF
#define   MAX_64   0x7FFFFFFFFFFFFFFFLL

// ------------------------------------------------------------------------
//                  Local Conditional Compilation Switches
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//                          Local Variables
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// MultDivideWithRounding64 - This routine provides rounding and up to 64 bit math to support
//                            routines that would overflow
//
// Params and Returns : Result = (Dividend + (Divisor / 2)) / Divisor
//
int64_t MultDivideWithRounding64(int32_t Mult1, int32_t Mult2, int32_t Divisor, enumMultDivideResult MultDivideResult) {

    uint8_t bNeg;
    int64_t Result;
    int64_t Dividend;

    if (Divisor == 0) {
        Error_Handler(FILE_MATH64,__LINE__);

    }
    bNeg = 0;
    Dividend = (int64_t)Mult1 * Mult2;
    if (Dividend < 0) {
        Dividend = -Dividend;
        bNeg = 1;
    }
    if (Divisor < 0) {
        Divisor = -Divisor;
        bNeg = !bNeg;
    }
    Result = (Dividend + (Divisor / 2)) / Divisor;

    if (MultDivideResult == RESULT_U8) {
      bNeg = 0; // Ensure bNeg is not set to flip the sign (unsigned)
      // If value is greater than max allowed, trigger an error as an overflow should not occur
      if (Result > MAX_U8) {
        Error_Handler(FILE_MATH64, __LINE__);
        Result = MAX_U8;
      }
      // Cannot be negative, if it is, flip the sign
      if (Result < 0) {
        Result = -Result;
      }
    }

    if ((MultDivideResult == RESULT_U8) && (Result >MAX_U8)) {
      Error_Handler(FILE_MATH64, __LINE__);
      bNeg = 0; // Ensure bNeg is not set to flip the sign
      Result = MAX_U8;
    }
    // Flip negative values to positive
    if ((MultDivideResult == RESULT_U8) && (Result < 0)) {
      bNeg = 0; // Ensure bNeg is not set to flip the sign

    }

    if ((MultDivideResult == RESULT_16) && (Result > MAX_16)) {
        Error_Handler(FILE_MATH64, __LINE__);
        Result = MAX_16;
    }
    if ((MultDivideResult == RESULT_32) && (Result > MAX_32)) {
        Error_Handler(FILE_MATH64, __LINE__);
    }
    if (bNeg != 0) {
        Result = -Result;
    }
    return Result;
}
