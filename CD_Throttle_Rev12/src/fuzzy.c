// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Filename:      fuzzy.c
// Author:        Charlie Urban
// Revision:      2021-3-29
//
// Description:   This module contains functions that:
//            - Performs fuzzy logic calculations to assign as quality
//            - score to sensors A and sensor B
//
//
// ------------------------------------------------------------------------
//
// ------------------------------------------------------------------------
//
// Revision Log:
//
//  0.3     2021-03-29    fuzzy.c creation
//
// ------------------------------------------------------------------------
//
// Design Details and Explanations:
//
// ------------------------------------------------------------------------
//
//          Determine which sensor to use
//
// ------------------------------------------------------------------------
//
// Implementation Notes:
//
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//                              Include Files
// ------------------------------------------------------------------------
#include "global.h"

// ------------------------------------------------------------------------
//                               Definitions
// ------------------------------------------------------------------------
#define MAX_INPUTS        3
#define MAX_OUTPUTS       1 // Always 1 to avoid confusion

// ------------------------------------------------------------------------
//                               Local Variables
// ------------------------------------------------------------------------

//
// Pointers to the implementations of member functions
//
static _FUZZY_MF fuzzy1_gMF[2] =
{
  Fuzzy_Trimf,
  Fuzzy_Trapmf
};
//
// Arrays of inputs and outputs
//
static int32_t fuzzy_inputs[MAX_INPUTS];  // This array length should be the max number of inputs
static int32_t fuzzy_outputs[MAX_OUTPUTS]; // This array length should be the max number of outputs

// ------------------------------------------------------------------------
//
// Qual_1 - Controls the fuzzy logic test 1 inputs, runs the fuzzy logic test 1
//          only once and then sets Throttle.Quality1 to the fuzzy logic test 1
//          output
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void Qual_1 (void) {

  fuzzy_outputs[0] = 0;

  while (SysVars.Quality1 == 0) {
    //
    // Input 0: 33Vref
    //
    fuzzy_inputs[0] = SysVars.PowerRail33VDC;
    //
    // Input 1: 5Vref
    //
    fuzzy_inputs[1] = SysVars.PowerRail5VDC;
    //
    // Input 2: Noise
    //
    fuzzy_inputs[2] = SysVars.Noise;
    //
    // Run fuzzy logic test 1 and store result
    //


    // MYABE ADD CODE THAT CONVERTS INPUTS TO MAX AND MIN ALLOWABLE VALUES

    Fuzzy_Evaluate(Fuzzy1Configuration);
    SysVars.Quality1 = fuzzy_outputs[0] + 1; // + 1 ensures this value is greater than 0

  }
}

// ------------------------------------------------------------------------
//
// Qual_2 - Controls the fuzzy logic test 1 inputs, runs the fuzzy logic test 1
//          only once and then sets Throttle.Quality1 to the fuzzy logic test 1
//          output
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void Qual_2 (void) {

  fuzzy_outputs[0] = 0;

  while (SysVars.Quality2 == 0) {

    // Read Input: Kma1Change
    fuzzy_inputs[0] = SysVars.TemperatureCelsius;
    // Read Input: Kma2Change
    fuzzy_inputs[1] = 5000;//ForeignSensor.CanChange;
    // Read Input: 33Vref
    fuzzy_inputs[2] = SysVars.PowerRail33VDC;


    Fuzzy_Evaluate(Fuzzy2Configuration);
    SysVars.Quality2 = fuzzy_outputs[0] + 1; // ensures this vlaue is greater than 0;
  }
}

// ------------------------------------------------------------------------
//
// Min - returns the lesser of two values
//
// Params:    a - value being compared to b
//            b - value being compared to a
//
// Returns:   The lesser of a and b
//
// Requirement(s):
//
int32_t Min(int32_t a, int32_t b) {
  if (a > b) {
    return b;
  }
  else {
    return a;
  }
}

// ------------------------------------------------------------------------
//
// Max - returns the greater of two values
//
// Params:    a - value being compared to b
//            b - value being compared to a
//
// Returns:   The greater of a and b
//
// Requirement(s):
//
int32_t Max(int32_t a, int32_t b) {
  if (a > b) {
    return a;
  }
  else {
    return b;
  }
}

// ------------------------------------------------------------------------
//
// Fuzzy_Trimf - Outputs the result of a triangular member function
//
// Params:    x - sensor's value
//            p - array of 3 points outlining the triangle
/*
 *              p[1]
 *               /\
 *            t1/--\t2
 *             /|   \
 *       p[0] /_|____\ p[2]
 *              x
*/
// Returns:   Fuzzy logic score from a trapezoid membershift function
//
// Requirement(s):
//
int32_t Fuzzy_Trimf(int32_t x, int32_t *p)
{
  int32_t a = p[0], b = p[1], c = p[2]; // assign points of triangle to a, b and c
  int32_t t1 = (10000 * (x - a)) / (b - a); //outputs how high on the left-hand side x is
  int32_t t2 = (10000 * (c - x)) / (c - b); // outputs how high on the right-hand side x is
  if ((a == b) && (b == c)) {
    return (int32_t) (x == a);
  }
  if (a == b) {
    return (int32_t) (t2*(b <= x)*(x <= c));
  }
  if (b == c) {
    return (int32_t) (t1*(a <= x)*(x <= b));
  }
  t1 = Min(t1, t2);
  return (int32_t) Max(t1, 0);
}

// ------------------------------------------------------------------------
//
// Fuzzy_Trampmf - Outputs the result of a trapezoidal member function
//
// Params:    x - sensor's value
//            p - array of 4 points outlining the trapezoid
/*
 *        p[1]  ______________ p[2]
 *             /      |       \
 *            /       |        \
 *      p[0] /________|_________\ p[3]
 *                    x
*/
// Returns:   Fuzzy logic score from a trapezoid membership function
//
// Requirement(s):
//
int32_t Fuzzy_Trapmf(int32_t x, int32_t* p) {
  int32_t a = p[0], b = p[1], c = p[2], d = p[3]; // assign points of trapezoi to a, b, c and d
  int32_t t1 = ((x <= c) ? 10000 : ((d < x) ? 0 : ((c != d) ? ((10000*(d - x)) / (d - c)) : 0)));
  int32_t t2 = ((b <= x) ? 10000 : ((x < a) ? 0 : ((a != b) ? ((10000*(x - a)) / (b - a)) : 0)));
  return (int32_t) Min(t1, t2);
}

// ------------------------------------------------------------------------
//
// Fuzzy_Array_Operation - Applies a comparator (Min, Max, etc.) to an array and returns
//                         the result of the comparator
//
// Params:    array - array of values to compare using pfnOp
//            size - size of array
//            pfnOp - function to apply
//
// Returns:   The value in the array that meets the criteria of the comparator (i.e.,
//            minimum of array for Min comparator)
//
// Requirement(s):
//
int32_t Fuzzy_Array_Operation(int32_t *array, int8_t size, _FUZZY_ARR_OP pfnOp) {

  int8_t i; // counter in for loop
  int32_t ret = 0; // value returned by function

  if (size == 0) {
    return ret;
  }
  if (size == 1) {
    return array[0];
  }

  ret = array[0];
  for (i = 1; i < size; i++) {
    ret = (*pfnOp)(ret, array[i]);
  }
  return ret;
}

// ------------------------------------------------------------------------
//
// Fuzzy_MF_Out - Get the output area for that sensor's value based on rules applied
//
// Params:    fuzzyRuleSet - matrix of rules to be applied
//            x - sensor's value
//            o - Index for output (i.e., output0, output1, etc.)
//
// Returns:   Fuzzy logic score from a trapezoid membership function
//
// Requirement(s):
//
int32_t Fuzzy_MF_Out(int32_t** fuzzyRuleSet, int32_t x, int8_t o, stFuzzyConfiguration *FuzzyData) {

  int32_t mfOut; // fuzzy output of membership function
  int8_t r; // counter for rules
  int32_t TmpArr[4]; // Stores membership function data as a temporary array
  int32_t *mfoutptr[4]; // Pointer to TmpArr
  int8_t index; // holds which mf applies to the rule
  int8_t j;

  for (r = 0; r < FuzzyData->FuzzyRuleInfo.RuleCount; ++r) {
    index = FuzzyData->FuzzyOutputRules.OutputRule[r]; // gets the mf of the rule to apply
    if (index > 0) {
      index = index - 1;
      TmpArr[0] = FuzzyData->FuzzyOutputMFs.MF[index].Point0;
      TmpArr[1] = FuzzyData->FuzzyOutputMFs.MF[index].Point1;
      TmpArr[2] = FuzzyData->FuzzyOutputMFs.MF[index].Point2;
      TmpArr[3] = FuzzyData->FuzzyOutputMFs.MF[index].Point3;
      for (j = 0; j < 4; j++) {
        mfoutptr[j] = &TmpArr[j];
      }
      mfOut = (fuzzy1_gMF[FuzzyData->FuzzyOutputMFs.MF[index].Shape])(x, *mfoutptr);
    }
    else if (index < 0) {    // Indicates a NOT
      index = -index - 1;
      mfOut = FuzzyData->FuzzyOutputInfo.Max - (fuzzy1_gMF[FuzzyData->FuzzyOutputMFs.MF[index].Shape])(x, *mfoutptr);
    }
    else {
      mfOut = 0;
    }

    fuzzyRuleSet[0][r] = Min(mfOut, fuzzyRuleSet[1][r]);
  }

  return Fuzzy_Array_Operation(fuzzyRuleSet[0], FuzzyData->FuzzyRuleInfo.RuleCount, Max);
}

// ------------------------------------------------------------------------
//
// Fuzzy_Defuzz_Centroid - Convert output area to a truth value
//
// Params:    fuzzyRuleSet - matrix of rules to be applied
//            o - Index for output (i.e., output0, output1, etc.)
//
// Returns:   Truth value
//
// Requirement(s):
//
int32_t Fuzzy_Defuzz_Centroid(int32_t** fuzzyRuleSet, int8_t o, stFuzzyConfiguration *FuzzyData) {

    int32_t step = ((FuzzyData->FuzzyOutputInfo.Max - FuzzyData->FuzzyOutputInfo.Min)) / (FUZZY_RESOLUTION - 1); // step size
    int32_t area = 0; // area of membership function shape
    int32_t momentum = 0; // weight of the area
    int32_t dist, slice; // dist: how far the slice is from origin, slice: small area within membership function
    int8_t i; // counter for loop

    // calculate the area under the curve formed by the MF outputs
    for (i = 0; i < FUZZY_RESOLUTION; ++i) {
        dist = FuzzyData->FuzzyOutputInfo.Min + (step * i);
        slice = (step * Fuzzy_MF_Out(fuzzyRuleSet, dist, o, FuzzyData)) / 100;
        area += slice;
        momentum += slice*dist;
    }

    if (area == 0) {
      return (int32_t) ((FuzzyData->FuzzyOutputInfo.Max + FuzzyData->FuzzyOutputInfo.Min) / 2);
    }
    else {
      return (int32_t) ((momentum) / area);
    }
}

// ------------------------------------------------------------------------
//
// Fuzzy1_Evaluate - Run fuzzy logic test 1 based on inputs and rules
//
// Params:    None
//
// Returns:   None
//
// Requirement(s):
//
void Fuzzy_Evaluate(stFuzzyConfiguration *FuzzyData) {
  //
  // input arrays for each input
  //
  int32_t fuzzy1Input0[3] = { 0, 0, 0 };
  int32_t fuzzy1Input1[3] = { 0, 0, 0 };
  int32_t fuzzy1Input2[3] = { 0, 0, 0 };
  int32_t* fuzzyInput[3] = { fuzzy1Input0, fuzzy1Input1, fuzzy1Input2, };
  int32_t RuleArr[3];
  int32_t TmpArr[4];
  int32_t *mfinptr[4];

  //
  // All rules being evaluated and applied
  //
  int32_t fuzzy1Rules[15] = { 0 };
  int32_t fuzzy1Fires[15] = { 0 };
  int32_t* fuzzy1RuleSet[2] = { fuzzy1Rules, fuzzy1Fires };
  int32_t sW = 0;
  int8_t i, j, r, o, n; // counters for loops
  int8_t index = 0; // values from rule being applied
  //
  // Transforming input to fuzzy Input
  //
  for (i = 0; i < FuzzyData->FuzzyMFCounts.NumberOfInputs; ++i) {
    for (j = 0; j < FuzzyData->FuzzyMFCounts.InputCount[i]; ++j) {

      TmpArr[0] = FuzzyData->FuzzyInputMFs.MF[i*3+j].Point0;
      TmpArr[1] = FuzzyData->FuzzyInputMFs.MF[i*3+j].Point1;
      TmpArr[2] = FuzzyData->FuzzyInputMFs.MF[i*3+j].Point2;
      TmpArr[3] = FuzzyData->FuzzyInputMFs.MF[i*3+j].Point3;
      for (n = 0; n < 4; n++) {
        mfinptr[n] = &TmpArr[n];
      }
      fuzzyInput[i][j] = (fuzzy1_gMF[FuzzyData->FuzzyInputMFs.MF[i*3+j].Shape])(fuzzy_inputs[i], *mfinptr);
    }
  }

  for (r = 0; r <  FuzzyData->FuzzyRuleInfo.RuleCount; ++r) {
    RuleArr[0] = FuzzyData->FuzzyInputRules.InputRules[r].Input0;
    RuleArr[1] = FuzzyData->FuzzyInputRules.InputRules[r].Input1;
    RuleArr[2] = FuzzyData->FuzzyInputRules.InputRules[r].Input2;
    if (FuzzyData->FuzzyRuleInfo.RuleType[r] == 1) {
      fuzzy1Fires[r] = 200000000;
      for (i = 0; i < FuzzyData->FuzzyMFCounts.NumberOfInputs; ++i) {
        index = RuleArr[i];
        if (index > 0) {
          fuzzy1Fires[r] = Min(fuzzy1Fires[r], fuzzyInput[i][index - 1]);
        }
        else if (index < 0) {
          fuzzy1Fires[r] = Min(fuzzy1Fires[r], 10000 - fuzzyInput[i][-index - 1]);
        }
        else {
          fuzzy1Fires[r] = Min(fuzzy1Fires[r], 10000);
        }
      }
    }
    else {
      fuzzy1Fires[r] = -200000000;
      for (i = 0; i < FuzzyData->FuzzyMFCounts.NumberOfInputs; ++i) {
        index = RuleArr[i];
        if (index > 0) {
          fuzzy1Fires[r] = Max(fuzzy1Fires[r], fuzzyInput[i][index - 1]);
        }
        else if (index < 0) {
          fuzzy1Fires[r] = Max(fuzzy1Fires[r], 10000 - fuzzyInput[i][-index - 1]);
        }
        else {
          fuzzy1Fires[r] = Max(fuzzy1Fires[r], 0);
        }
      }
    }

    fuzzy1Fires[r] = FuzzyData->FuzzyRuleInfo.RuleWeight[r] * fuzzy1Fires[r];
    sW += fuzzy1Fires[r];
  }

  if (sW == 0) {
    for (o = 0; o < MAX_OUTPUTS; ++o) {
      fuzzy_outputs[o] = ((FuzzyData->FuzzyOutputInfo.Max + FuzzyData->FuzzyOutputInfo.Min) / 2);
    }
  }
  else {
    for (o = 0; o < MAX_OUTPUTS; ++o) {
      fuzzy_outputs[o] = Fuzzy_Defuzz_Centroid(fuzzy1RuleSet, o, FuzzyData);
    }
  }
}
