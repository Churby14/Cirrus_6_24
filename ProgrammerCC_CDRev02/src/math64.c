//
// ******************************************************************************
//
//  Description:       Provides 64-bit match support
//
//  Release History:
//
//     2018-10-12  RLW     Initial Release
//
// ******************************************************************************

#include "global.h"
#include "math.h"

// ******************************************************************************
// ******************************************************************************
//
// This routine provides rounding
//
//    Result = (Dividend + (Divisor / 2)) / Divisor
//
int64_t DivideWithRounding64(int64_t Dividend, int32_t Divisor, enumMultDivideResult MultDivideResult) {

  int8_t bNeg;
  int64_t Result;

  if ( Divisor == 0 ) {
    if ( MultDivideResult == RESULT_16 ) {
      return SHRT_MAX;
    }
    if ( MultDivideResult == RESULT_32 ) {
      return LONG_MAX;
    }
    return LONG_LONG_MAX;
  }
  bNeg = FALSE;
  if ( Dividend < 0 ) {
    Dividend = -Dividend;
    bNeg = TRUE;
  }
  if ( Divisor < 0 ) {
    Divisor = -Divisor;
    bNeg = !bNeg;
  }
  Result = (Dividend + (Divisor / 2)) / Divisor;
  if ( (MultDivideResult == RESULT_16) && (Result > SHRT_MAX) ) {
    Result = SHRT_MAX;
  }
  if ( (MultDivideResult == RESULT_32) && (Result > LONG_MAX) ) {
    Result = LONG_MAX;
  }
  if ( bNeg ) {
    Result = -Result;
  }
  return Result;
}

// ******************************************************************************
// ******************************************************************************
//
// This routine provides rounding
//
//    Result = (Dividend + (Divisor / 2)) / Divisor
//
int64_t MultDivideWithRounding64(int32_t Mult1, int32_t Mult2, int32_t Divisor, enumMultDivideResult MultDivideResult) {

  uint8_t bNeg;
  int64_t Result;
  int64_t Dividend;

  if ( Divisor == 0 ) {
    if ( MultDivideResult == RESULT_16 ) {
      return SHRT_MAX;
    }
    if ( MultDivideResult == RESULT_32 ) {
      return LONG_MAX;
    }
    return LONG_LONG_MAX;
  }
  bNeg = FALSE;
  Dividend = (int64_t) Mult1 * Mult2;
  if ( Dividend < 0 ) {
    Dividend = -Dividend;
    bNeg = TRUE;
  }
  if ( Divisor < 0 ) {
    Divisor = -Divisor;
    bNeg = !bNeg;
  }
  Result = (Dividend + (Divisor / 2)) / Divisor;
  if ( (MultDivideResult == RESULT_16) && (Result > SHRT_MAX) ) {
    Result = SHRT_MAX;
  }
  if ( (MultDivideResult == RESULT_32) && (Result > LONG_MAX) ) {
    Result = LONG_MAX;
  }
  if ( bNeg ) {
    Result = -Result;
  }
  return Result;
}
