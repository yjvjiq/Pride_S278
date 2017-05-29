/*
 * File: YoungMan_LT.c
 *
 * Code generated for Simulink model 'YoungMan_LT'.
 *
 * Model version                  : 1.1149
 * Simulink Coder version         : 8.10 (R2016a) 10-Feb-2016
 * C/C++ source code generated on : Fri May 12 15:07:17 2017
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Freescale->HC(S)12
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "YoungMan_LT.h"
#ifndef UCHAR_MAX
#include <limits.h>
#endif

#if ( UCHAR_MAX != (0xFFU) ) || ( SCHAR_MAX != (0x7F) )
#error Code was generated for compiler with different sized uchar/char. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( USHRT_MAX != (0xFFFFU) ) || ( SHRT_MAX != (0x7FFF) )
#error Code was generated for compiler with different sized ushort/short. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( UINT_MAX != (0xFFFFU) ) || ( INT_MAX != (0x7FFF) )
#error Code was generated for compiler with different sized uint/int. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( ULONG_MAX != (0xFFFFFFFFUL) ) || ( LONG_MAX != (0x7FFFFFFFL) )
#error Code was generated for compiler with different sized ulong/long. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

/* Exported block signals */
real32_T BiggestFeedbackCurt_Model;    /* '<S5>/Gain'
                                        * Output
                                        */
real32_T PowerOnSOCAjustFactor;        /* '<S4>/1-D Lookup Table'
                                        * Input
                                        */
real32_T m_askcurrent_Model;           /* '<S3>/Gain'
                                        * Output
                                        */
real32_T ChargeEndSOCAjustFactor;      /* '<S2>/2-D Lookup Table3'
                                        * Input
                                        */
real32_T BiggestDischargeCurt_Model;   /* '<S1>/Gain'
                                        * Output
                                        */

/* Exported data definition */

/* Definition for custom storage class: Default */
real32_T CAP_CONST_CAL = 240.0F;
real32_T DisChargeCurrentTable[132] = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
  0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
  0.0F, 0.0F, 0.0F, 0.05F, 0.15F, 0.15F, 0.15F, 0.3F, 0.3F, 0.3F, 0.3F, 0.3F,
  0.3F, 0.0F, 0.3F, 0.5F, 0.5F, 0.5F, 0.8F, 0.8F, 0.8F, 0.8F, 0.8F, 0.8F, 0.0F,
  0.5F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 0.0F, 0.5F, 1.0F,
  1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 0.0F, 0.5F, 1.2F, 1.2F, 1.2F,
  1.2F, 1.2F, 1.2F, 1.2F, 1.2F, 1.2F, 0.0F, 1.0F, 1.2F, 1.2F, 1.2F, 1.5F, 1.5F,
  1.5F, 1.5F, 1.5F, 1.5F, 0.0F, 1.0F, 1.2F, 1.2F, 1.2F, 1.5F, 1.5F, 1.5F, 1.5F,
  1.5F, 1.5F, 0.0F, 1.0F, 1.2F, 1.2F, 1.2F, 1.5F, 1.5F, 1.5F, 1.5F, 1.5F, 1.5F,
  0.0F, 0.6F, 0.72F, 0.72F, 0.72F, 0.9F, 0.9F, 0.9F, 0.9F, 0.9F, 0.9F, 0.0F,
  0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F } ;/* maximum discharge current, row: SOC, collumn: T  */

uint16_T Factor_ChargeEndSOCAjust[161] = { 30167U, 34745U, 38796U, 41771U,
  42205U, 46215U, 46583U, 53384U, 56493U, 56609U, 57819U, 59171U, 59220U, 60023U,
  60468U, 60681U, 60772U, 60997U, 61004U, 61019U, 61027U, 61147U, 61262U, 38047U,
  40190U, 44408U, 48524U, 49099U, 53806U, 54155U, 58837U, 60305U, 60361U, 60865U,
  61424U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U,
  61440U, 61440U, 40480U, 46169U, 51145U, 54050U, 54448U, 57615U, 57798U, 60139U,
  60873U, 60901U, 61152U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U,
  61440U, 61440U, 61440U, 61440U, 61440U, 42914U, 52148U, 57882U, 59577U, 59797U,
  61424U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U,
  61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 46884U, 55209U,
  59256U, 60477U, 60619U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U,
  61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U,
  61440U, 50857U, 58271U, 60631U, 61376U, 61440U, 61440U, 61440U, 61440U, 61440U,
  61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U,
  61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U,
  61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U,
  61440U, 61440U, 61440U, 61440U, 61440U, 61440U, 61440U } ;

real32_T Factor_PowerOnSOCAjust[6] = { 0.8159F, 0.8833F, 0.936F, 0.9692F,
  0.9886F, 1.0F } ;

real32_T FastChargeCurrentTable[168] = { 0.0F, 0.0F, 0.1F, 0.1F, 0.3F, 0.3F,
  0.5F, 0.5F, 1.0F, 1.0F, 1.0F, 0.3F, 0.18F, 0.0F, 0.0F, 0.0F, 0.1F, 0.1F, 0.3F,
  0.3F, 0.5F, 0.5F, 1.0F, 1.0F, 1.0F, 0.3F, 0.18F, 0.0F, 0.0F, 0.0F, 0.1F, 0.1F,
  0.3F, 0.3F, 0.5F, 0.5F, 1.0F, 1.0F, 1.0F, 0.3F, 0.18F, 0.0F, 0.0F, 0.0F, 0.1F,
  0.1F, 0.3F, 0.3F, 0.5F, 0.5F, 1.0F, 1.0F, 1.0F, 0.3F, 0.18F, 0.0F, 0.0F, 0.0F,
  0.1F, 0.1F, 0.3F, 0.3F, 0.5F, 0.5F, 1.0F, 1.0F, 1.0F, 0.3F, 0.18F, 0.0F, 0.0F,
  0.0F, 0.1F, 0.1F, 0.3F, 0.3F, 0.5F, 0.5F, 1.0F, 1.0F, 1.0F, 0.3F, 0.18F, 0.0F,
  0.0F, 0.0F, 0.1F, 0.1F, 0.3F, 0.3F, 0.5F, 0.5F, 1.0F, 1.0F, 1.0F, 0.3F, 0.18F,
  0.0F, 0.0F, 0.0F, 0.1F, 0.1F, 0.3F, 0.3F, 0.5F, 0.5F, 1.0F, 1.0F, 1.0F, 0.3F,
  0.18F, 0.0F, 0.0F, 0.0F, 0.1F, 0.1F, 0.3F, 0.3F, 0.5F, 0.5F, 1.0F, 1.0F, 1.0F,
  0.3F, 0.18F, 0.0F, 0.0F, 0.0F, 0.1F, 0.1F, 0.3F, 0.3F, 0.5F, 0.5F, 0.75F, 0.8F,
  0.8F, 0.3F, 0.18F, 0.0F, 0.0F, 0.0F, 0.1F, 0.1F, 0.3F, 0.3F, 0.5F, 0.5F, 0.75F,
  0.8F, 0.8F, 0.3F, 0.18F, 0.0F, 0.0F, 0.0F, 0.1F, 0.1F, 0.3F, 0.3F, 0.5F, 0.5F,
  0.75F, 0.8F, 0.8F, 0.3F, 0.18F, 0.0F } ;

real32_T FeedBackCurrentTable[108] = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
  0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
  0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 0.0F,
  0.0F, 0.0F, 0.0F, 1.5F, 1.5F, 1.5F, 1.5F, 1.5F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
  0.0F, 0.0F, 1.5F, 1.5F, 1.5F, 1.5F, 1.5F, 1.5F, 1.5F, 1.5F, 1.0F, 1.0F, 0.0F,
  0.0F, 1.5F, 1.5F, 1.5F, 1.5F, 1.5F, 1.5F, 1.5F, 1.5F, 1.0F, 1.0F, 0.0F, 0.0F,
  1.5F, 1.5F, 1.5F, 1.5F, 1.5F, 1.5F, 1.5F, 1.5F, 1.0F, 1.0F, 0.0F, 0.0F, 0.9F,
  0.9F, 0.9F, 0.9F, 0.9F, 0.9F, 0.9F, 0.9F, 0.6F, 0.6F, 0.0F, 0.0F, 0.0F, 0.0F,
  0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F } ;/* maximum discharge current, row: SOC, collumn: T  */

real32_T SOC_Discharge[11] = { 0.0F, 0.1F, 0.2F, 0.3F, 0.4F, 0.5F, 0.6F, 0.7F,
  0.8F, 0.9F, 1.0F } ;

real32_T SOC_FastCharge[12] = { 0.0F, 0.1F, 0.2F, 0.3F, 0.4F, 0.5F, 0.6F, 0.7F,
  0.79F, 0.8F, 0.9F, 1.0F } ;

real32_T SOC_Feedback[12] = { 0.0F, 0.1F, 0.2F, 0.3F, 0.4F, 0.5F, 0.6F, 0.7F,
  0.8F, 0.9F, 0.95F, 1.0F } ;          /* row for max dicharge current table */

real32_T T_ChargeEndSOCAjust[7] = { 0.0F, 5.0F, 10.0F, 15.0F, 20.0F, 25.0F,
  35.0F } ;

real32_T T_Discharge[12] = { -40.0F, -30.0F, -20.0F, -10.0F, 0.0F, 10.0F, 20.0F,
  30.0F, 40.0F, 50.0F, 52.0F, 57.0F } ;/* collumn for max discharge current table */

real32_T T_Facdback[9] = { -10.0F, 0.0F, 10.0F, 20.0F, 30.0F, 40.0F, 50.0F,
  52.0F, 57.0F } ;

real32_T T_FastCharge[14] = { -10.0F, 0.0F, 2.0F, 5.0F, 7.0F, 10.0F, 12.0F,
  15.0F, 20.0F, 25.0F, 45.0F, 50.0F, 52.0F, 57.0F } ;

real32_T T_PowerOnSOCAjust[6] = { -25.0F, -15.0F, -5.0F, 5.0F, 15.0F, 25.0F } ;

real32_T V_CellLowest[23] = { 3.38F, 3.39F, 3.4F, 3.406F, 3.407F, 3.417F, 3.418F,
  3.44F, 3.458F, 3.459F, 3.47F, 3.49F, 3.491F, 3.52F, 3.55F, 3.57F, 3.58F,
  3.608F, 3.609F, 3.611F, 3.612F, 3.63F, 3.65F } ;

/* Constant parameters (auto storage) */
const ConstP_YoungMan_LT_T YoungMan_LT_ConstP = {
  /* Computed Parameter: linearizationSOF_maxIndex
   * Referenced by: '<S6>/linearizationSOF'
   */
  { 10U, 11U },

  /* Computed Parameter: uDLookupTable3_maxIndex
   * Referenced by: '<S2>/2-D Lookup Table3'
   */
  { 22U, 6U },

  /* Computed Parameter: uDLookupTable3_maxIndex_g
   * Referenced by: '<S3>/2-D Lookup Table3'
   */
  { 13U, 11U },

  /* Computed Parameter: linearizationSOF_maxIndex_f
   * Referenced by: '<S9>/linearizationSOF'
   */
  { 11U, 8U }
};

/* Real-time model */
RT_MODEL_YoungMan_LT_T YoungMan_LT_M_;
RT_MODEL_YoungMan_LT_T *const YoungMan_LT_M = &YoungMan_LT_M_;
real32_T look2_iflf_binlxpw(real32_T u0, real32_T u1, const real32_T bp0[],
  const real32_T bp1[], const real32_T table[], const uint32_T maxIndex[],
  uint32_T stride);
real32_T look2_ifs16bfflftu16n8_PYqzO3tA(real32_T u0, int16_T u1, const real32_T
  bp0[], const real32_T bp1[], const uint16_T table[], const uint32_T maxIndex[],
  uint32_T stride);
real32_T look2_iflf_linlxpw(real32_T u0, real32_T u1, const real32_T bp0[],
  const real32_T bp1[], const real32_T table[], const uint32_T maxIndex[],
  uint32_T stride);
real32_T look1_iflf_binlxpw(real32_T u0, const real32_T bp0[], const real32_T
  table[], uint32_T maxIndex);
extern void YoungMa_BigDischargePowerAdjust(void);
extern void YoungMan_LT_ChargeEndSOCAjust(void);
extern void YoungMan_LT_FastChrgPowerAjust(void);
extern void YoungMan_LT_PowerOnSOCAjust(void);
extern void YoungM_PulseRechargePowerAdjust(void);
real32_T look2_iflf_binlxpw(real32_T u0, real32_T u1, const real32_T bp0[],
  const real32_T bp1[], const real32_T table[], const uint32_T maxIndex[],
  uint32_T stride)
{
  real32_T frac;
  uint32_T bpIndices[2];
  real32_T fractions[2];
  real32_T yL_1d;
  uint32_T iRght;
  uint32_T bpIdx;
  uint32_T iLeft;

  /* Lookup 2-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear'
     Extrapolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0UL]) {
    iLeft = 0UL;
    frac = (u0 - bp0[0UL]) / (bp0[1UL] - bp0[0UL]);
  } else if (u0 < bp0[maxIndex[0UL]]) {
    /* Binary Search */
    bpIdx = maxIndex[0UL] >> 1UL;
    iLeft = 0UL;
    iRght = maxIndex[0UL];
    while (iRght - iLeft > 1UL) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1UL;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1UL] - bp0[iLeft]);
  } else {
    iLeft = maxIndex[0UL] - 1UL;
    frac = (u0 - bp0[maxIndex[0UL] - 1UL]) / (bp0[maxIndex[0UL]] - bp0[maxIndex
      [0UL] - 1UL]);
  }

  fractions[0UL] = frac;
  bpIndices[0UL] = iLeft;

  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u1 <= bp1[0UL]) {
    iLeft = 0UL;
    frac = (u1 - bp1[0UL]) / (bp1[1UL] - bp1[0UL]);
  } else if (u1 < bp1[maxIndex[1UL]]) {
    /* Binary Search */
    bpIdx = maxIndex[1UL] >> 1UL;
    iLeft = 0UL;
    iRght = maxIndex[1UL];
    while (iRght - iLeft > 1UL) {
      if (u1 < bp1[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1UL;
    }

    frac = (u1 - bp1[iLeft]) / (bp1[iLeft + 1UL] - bp1[iLeft]);
  } else {
    iLeft = maxIndex[1UL] - 1UL;
    frac = (u1 - bp1[maxIndex[1UL] - 1UL]) / (bp1[maxIndex[1UL]] - bp1[maxIndex
      [1UL] - 1UL]);
  }

  /* Interpolation 2-D
     Interpolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  bpIdx = iLeft * stride + bpIndices[0UL];
  yL_1d = (table[bpIdx + 1UL] - table[bpIdx]) * fractions[0UL] + table[bpIdx];
  bpIdx += stride;
  return (((table[bpIdx + 1UL] - table[bpIdx]) * fractions[0UL] + table[bpIdx])
          - yL_1d) * frac + yL_1d;
}

real32_T look2_ifs16bfflftu16n8_PYqzO3tA(real32_T u0, int16_T u1, const real32_T
  bp0[], const real32_T bp1[], const uint16_T table[], const uint32_T maxIndex[],
  uint32_T stride)
{
  real32_T frac;
  uint32_T bpIndices[2];
  real32_T fractions[2];
  real32_T yL_1d;
  uint32_T bpIdx;

  /* Lookup 2-D
     Canonical function name: look2_ifs16bfflftu16n8DfIf_linlcspw
     Search method: 'linear'
     Use previous index: 'off'
     Interpolation method: 'Linear'
     Extrapolation method: 'Clip'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
     Rounding mode: 'simplest'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'linear'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0UL]) {
    bpIdx = 0UL;
    frac = 0.0F;
  } else if (u0 < bp0[maxIndex[0UL]]) {
    /* Linear Search */
    for (bpIdx = maxIndex[0UL] >> 1UL; u0 < bp0[bpIdx]; bpIdx--) {
    }

    while (u0 >= bp0[bpIdx + 1UL]) {
      bpIdx++;
    }

    frac = (u0 - bp0[bpIdx]) / (bp0[bpIdx + 1UL] - bp0[bpIdx]);
  } else {
    bpIdx = maxIndex[0UL] - 1UL;
    frac = 1.0F;
  }

  fractions[0UL] = frac;
  bpIndices[0UL] = bpIdx;

  /* Prelookup - Index and Fraction
     Index Search method: 'linear'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u1 < bp1[0UL]) {
    bpIdx = 0UL;
    frac = 0.0F;
  } else if (u1 < bp1[maxIndex[1UL]]) {
    /* Linear Search */
    for (bpIdx = maxIndex[1UL] >> 1UL; u1 < bp1[bpIdx]; bpIdx--) {
    }

    while (u1 >= bp1[bpIdx + 1UL]) {
      bpIdx++;
    }

    frac = ((real32_T)u1 - bp1[bpIdx]) / (bp1[bpIdx + 1UL] - bp1[bpIdx]);
  } else {
    bpIdx = maxIndex[1UL] - 1UL;
    frac = 1.0F;
  }

  /* Interpolation 2-D
     Interpolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Rounding mode: 'simplest'
     Overflow mode: 'portable wrapping'
   */
  bpIdx = bpIdx * stride + bpIndices[0UL];
  yL_1d = ((real32_T)table[bpIdx + 1UL] * 0.00390625F - (real32_T)table[bpIdx] *
           0.00390625F) * fractions[0UL] + (real32_T)table[bpIdx] * 0.00390625F;
  bpIdx += stride;
  return ((((real32_T)table[bpIdx + 1UL] * 0.00390625F - (real32_T)table[bpIdx] *
            0.00390625F) * fractions[0UL] + (real32_T)table[bpIdx] * 0.00390625F)
          - yL_1d) * frac + yL_1d;
}

real32_T look2_iflf_linlxpw(real32_T u0, real32_T u1, const real32_T bp0[],
  const real32_T bp1[], const real32_T table[], const uint32_T maxIndex[],
  uint32_T stride)
{
  real32_T frac;
  uint32_T bpIndices[2];
  real32_T fractions[2];
  real32_T yL_1d;
  uint32_T bpIdx;

  /* Lookup 2-D
     Search method: 'linear'
     Use previous index: 'off'
     Interpolation method: 'Linear'
     Extrapolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'linear'
     Extrapolation method: 'Linear'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0UL]) {
    bpIdx = 0UL;
    frac = (u0 - bp0[0UL]) / (bp0[1UL] - bp0[0UL]);
  } else if (u0 < bp0[maxIndex[0UL]]) {
    /* Linear Search */
    for (bpIdx = maxIndex[0UL] >> 1UL; u0 < bp0[bpIdx]; bpIdx--) {
    }

    while (u0 >= bp0[bpIdx + 1UL]) {
      bpIdx++;
    }

    frac = (u0 - bp0[bpIdx]) / (bp0[bpIdx + 1UL] - bp0[bpIdx]);
  } else {
    bpIdx = maxIndex[0UL] - 1UL;
    frac = (u0 - bp0[maxIndex[0UL] - 1UL]) / (bp0[maxIndex[0UL]] - bp0[maxIndex
      [0UL] - 1UL]);
  }

  fractions[0UL] = frac;
  bpIndices[0UL] = bpIdx;

  /* Prelookup - Index and Fraction
     Index Search method: 'linear'
     Extrapolation method: 'Linear'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u1 <= bp1[0UL]) {
    bpIdx = 0UL;
    frac = (u1 - bp1[0UL]) / (bp1[1UL] - bp1[0UL]);
  } else if (u1 < bp1[maxIndex[1UL]]) {
    /* Linear Search */
    for (bpIdx = maxIndex[1UL] >> 1UL; u1 < bp1[bpIdx]; bpIdx--) {
    }

    while (u1 >= bp1[bpIdx + 1UL]) {
      bpIdx++;
    }

    frac = (u1 - bp1[bpIdx]) / (bp1[bpIdx + 1UL] - bp1[bpIdx]);
  } else {
    bpIdx = maxIndex[1UL] - 1UL;
    frac = (u1 - bp1[maxIndex[1UL] - 1UL]) / (bp1[maxIndex[1UL]] - bp1[maxIndex
      [1UL] - 1UL]);
  }

  /* Interpolation 2-D
     Interpolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  bpIdx = bpIdx * stride + bpIndices[0UL];
  yL_1d = (table[bpIdx + 1UL] - table[bpIdx]) * fractions[0UL] + table[bpIdx];
  bpIdx += stride;
  return (((table[bpIdx + 1UL] - table[bpIdx]) * fractions[0UL] + table[bpIdx])
          - yL_1d) * frac + yL_1d;
}

real32_T look1_iflf_binlxpw(real32_T u0, const real32_T bp0[], const real32_T
  table[], uint32_T maxIndex)
{
  real32_T frac;
  uint32_T iRght;
  uint32_T iLeft;
  uint32_T bpIdx;

  /* Lookup 1-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear'
     Extrapolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0UL]) {
    iLeft = 0UL;
    frac = (u0 - bp0[0UL]) / (bp0[1UL] - bp0[0UL]);
  } else if (u0 < bp0[maxIndex]) {
    /* Binary Search */
    bpIdx = maxIndex >> 1UL;
    iLeft = 0UL;
    iRght = maxIndex;
    while (iRght - iLeft > 1UL) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1UL;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1UL] - bp0[iLeft]);
  } else {
    iLeft = maxIndex - 1UL;
    frac = (u0 - bp0[maxIndex - 1UL]) / (bp0[maxIndex] - bp0[maxIndex - 1UL]);
  }

  /* Interpolation 1-D
     Interpolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  return (table[iLeft + 1UL] - table[iLeft]) * frac + table[iLeft];
}

/* Output and update for atomic system: '<Root>/BigDischargePowerAdjust' */
void YoungMa_BigDischargePowerAdjust(void)
{
  real32_T rtb_Add;

  /* Switch: '<S6>/Switch' incorporates:
   *  Constant: '<S1>/Constant4'
   *  Constant: '<S6>/zero'
   *  Constant: '<S7>/Constant'
   *  Constant: '<S8>/Constant'
   *  DataTypeConversion: '<S1>/Data Type Conversion'
   *  Inport: '<Root>/Tavg'
   *  Logic: '<S6>/and'
   *  RelationalOperator: '<S7>/Compare'
   *  RelationalOperator: '<S8>/Compare'
   *  Sum: '<S1>/Add'
   */
  if (((real32_T)Tavg - 40.0F > -30.0F) && ((real32_T)Tavg - 40.0F < 57.0F)) {
    /* Lookup_n-D: '<S6>/linearizationSOF' incorporates:
     *  Inport: '<Root>/g_socValue'
     */
    rtb_Add = look2_iflf_binlxpw(Can_g_socValue, (real32_T)Tavg - 40.0F,
      (&(SOC_Discharge[0])), (&(T_Discharge[0])), (&(DisChargeCurrentTable[0])),
      YoungMan_LT_ConstP.linearizationSOF_maxIndex, 11UL);
  } else {
    rtb_Add = 0.0F;
  }

  /* End of Switch: '<S6>/Switch' */

  /* Gain: '<S1>/Gain' */
  BiggestDischargeCurt_Model = CAP_CONST_CAL * rtb_Add;
}

/* Output and update for atomic system: '<Root>/ChargeEndSOCAjust' */
void YoungMan_LT_ChargeEndSOCAjust(void)
{
  /* Lookup_n-D: '<S2>/2-D Lookup Table3' incorporates:
   *  Constant: '<S2>/Constant4'
   *  Inport: '<Root>/Tavg'
   *  Inport: '<Root>/g_lowestCellVoltage'
   *  Sum: '<S2>/Add'
   */
  ChargeEndSOCAjustFactor = look2_ifs16bfflftu16n8_PYqzO3tA(g_lowestCellVoltage,
    Tavg - 40, (&(V_CellLowest[0])), (&(T_ChargeEndSOCAjust[0])),
    (&(Factor_ChargeEndSOCAjust[0])), YoungMan_LT_ConstP.uDLookupTable3_maxIndex,
    23UL);
}

/* Output and update for atomic system: '<Root>/FastChrgPowerAjust' */
void YoungMan_LT_FastChrgPowerAjust(void)
{
  real32_T rtb_uDLookupTable3;

  /* Lookup_n-D: '<S3>/2-D Lookup Table3' incorporates:
   *  Constant: '<S3>/Constant4'
   *  DataTypeConversion: '<S3>/Data Type Conversion1'
   *  Inport: '<Root>/Tavg'
   *  Inport: '<Root>/g_socValue'
   *  Sum: '<S3>/Add'
   */
  rtb_uDLookupTable3 = look2_iflf_linlxpw((real32_T)Tavg - 40.0F, Can_g_socValue,
    (&(T_FastCharge[0])), (&(SOC_FastCharge[0])), (&(FastChargeCurrentTable[0])),
    YoungMan_LT_ConstP.uDLookupTable3_maxIndex_g, 14UL);

  /* Gain: '<S3>/Gain' */
  m_askcurrent_Model = CAP_CONST_CAL * rtb_uDLookupTable3;
}

/* Output and update for atomic system: '<Root>/PowerOnSOCAjust' */
void YoungMan_LT_PowerOnSOCAjust(void)
{
  /* Lookup_n-D: '<S4>/1-D Lookup Table' incorporates:
   *  Constant: '<S4>/Constant4'
   *  Inport: '<Root>/g_lowestTemperature'
   *  Sum: '<S4>/Add'
   */
  PowerOnSOCAjustFactor = look1_iflf_binlxpw((real32_T)g_lowestTemperature -
    40.0F, (&(T_PowerOnSOCAjust[0])), (&(Factor_PowerOnSOCAjust[0])), 5UL);
}

/* Output and update for atomic system: '<Root>/PulseRechargePowerAdjust' */
void YoungM_PulseRechargePowerAdjust(void)
{
  real32_T rtb_Add;

  /* Switch: '<S9>/Switch' incorporates:
   *  Constant: '<S10>/Constant'
   *  Constant: '<S11>/Constant'
   *  Constant: '<S5>/Constant4'
   *  Constant: '<S9>/zero'
   *  DataTypeConversion: '<S5>/Data Type Conversion'
   *  Inport: '<Root>/Tavg'
   *  Logic: '<S9>/and'
   *  RelationalOperator: '<S10>/Compare'
   *  RelationalOperator: '<S11>/Compare'
   *  Sum: '<S5>/Add'
   */
  if (((real32_T)Tavg - 40.0F > 0.0F) && ((real32_T)Tavg - 40.0F < 57.0F)) {
    /* Lookup_n-D: '<S9>/linearizationSOF' incorporates:
     *  Inport: '<Root>/g_socValue'
     */
    rtb_Add = look2_iflf_binlxpw(Can_g_socValue, (real32_T)Tavg - 40.0F,
      (&(SOC_Feedback[0])), (&(T_Facdback[0])), (&(FeedBackCurrentTable[0])),
      YoungMan_LT_ConstP.linearizationSOF_maxIndex_f, 12UL);
  } else {
    rtb_Add = 0.0F;
  }

  /* End of Switch: '<S9>/Switch' */

  /* Gain: '<S5>/Gain' */
  BiggestFeedbackCurt_Model = CAP_CONST_CAL * rtb_Add;
}

/* Model step function */
void YoungMan_LT_step(void)
{
  /* Outputs for Atomic SubSystem: '<Root>/BigDischargePowerAdjust' */
  YoungMa_BigDischargePowerAdjust();

  /* End of Outputs for SubSystem: '<Root>/BigDischargePowerAdjust' */

  /* Outputs for Atomic SubSystem: '<Root>/PulseRechargePowerAdjust' */
  YoungM_PulseRechargePowerAdjust();

  /* End of Outputs for SubSystem: '<Root>/PulseRechargePowerAdjust' */

  /* Outputs for Atomic SubSystem: '<Root>/FastChrgPowerAjust' */
  YoungMan_LT_FastChrgPowerAjust();

  /* End of Outputs for SubSystem: '<Root>/FastChrgPowerAjust' */

  /* Outputs for Atomic SubSystem: '<Root>/ChargeEndSOCAjust' */
  YoungMan_LT_ChargeEndSOCAjust();

  /* End of Outputs for SubSystem: '<Root>/ChargeEndSOCAjust' */

  /* Outputs for Atomic SubSystem: '<Root>/PowerOnSOCAjust' */
  YoungMan_LT_PowerOnSOCAjust();

  /* End of Outputs for SubSystem: '<Root>/PowerOnSOCAjust' */
}

/* Model initialize function */
void YoungMan_LT_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(YoungMan_LT_M, (NULL));

  /* block I/O */

  /* exported global signals */
  BiggestFeedbackCurt_Model = 0.0F;
  PowerOnSOCAjustFactor = 0.0F;
  m_askcurrent_Model = 0.0F;
  ChargeEndSOCAjustFactor = 0.0F;
  BiggestDischargeCurt_Model = 0.0F;
}

/* Model terminate function */
void YoungMan_LT_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
