/*
 * File: DischargeOverCurrent.c
 *
 * Code generated for Simulink model 'DischargeOverCurrent'.
 *
 * Model version                  : 1.103
 * Simulink Coder version         : 8.10 (R2016a) 10-Feb-2016
 * C/C++ source code generated on : Mon Apr 24 11:00:01 2017
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Freescale->HC(S)12
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "DischargeOverCurrent.h"
#include "DischargeOverCurrent_private.h"

/* Named constants for Chart: '<Root>/Logic' */
#define DischargeOve_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define DischargeOverCurrent_IN_Delay10 ((uint8_T)1U)
#define DischargeOverCurrent_IN_Delay11 ((uint8_T)2U)
#define DischargeOverCurrent_IN_Delay20 ((uint8_T)1U)
#define DischargeOverCurrent_IN_Delay21 ((uint8_T)2U)
#define DischargeOverCurrent_IN_Delay30 ((uint8_T)1U)
#define DischargeOverCurrent_IN_Fault1 ((uint8_T)3U)
#define DischargeOverCurrent_IN_Fault10 ((uint8_T)4U)
#define DischargeOverCurrent_IN_Fault2 ((uint8_T)3U)
#define DischargeOverCurrent_IN_Fault20 ((uint8_T)4U)
#define DischargeOverCurrent_IN_Fault3 ((uint8_T)2U)
#define DischargeOverCurrent_IN_Wait1  ((uint8_T)5U)
#define DischargeOverCurrent_IN_Wait2  ((uint8_T)5U)
#define DischargeOverCurrent_IN_Wait3  ((uint8_T)3U)

/* Block signals (auto storage) */
B_DischargeOverCurrent_T DischargeOverCurrent_B;

/* Block states (auto storage) */
DW_DischargeOverCurrent_T DischargeOverCurrent_DW;

/* Real-time model */
RT_MODEL_DischargeOverCurrent_T DischargeOverCurrent_M_;
RT_MODEL_DischargeOverCurrent_T *const DischargeOverCurrent_M =
  &DischargeOverCurrent_M_;

/* Model step function */
uint8_T DischargeOverCurrent_custom(real32_T cur, real32_T curM)
{
  /* specified return value */
  uint8_T F_lev;

  /* Chart: '<Root>/Logic' incorporates:
   *  Inport: '<Root>/cur'
   *  Inport: '<Root>/curM'
   */
  /* Gateway: Logic */
  if (DischargeOverCurrent_DW.temporalCounter_i1 < 15U) {
    DischargeOverCurrent_DW.temporalCounter_i1++;
  }

  if (DischargeOverCurrent_DW.temporalCounter_i2 < 15U) {
    DischargeOverCurrent_DW.temporalCounter_i2++;
  }

  if (DischargeOverCurrent_DW.temporalCounter_i3 < 15U) {
    DischargeOverCurrent_DW.temporalCounter_i3++;
  }

  /* During: Logic */
  if (DischargeOverCurrent_DW.is_active_c3_DischargeOverCurre == 0U) {
    /* Entry: Logic */
    DischargeOverCurrent_DW.is_active_c3_DischargeOverCurre = 1U;

    /* Entry Internal: Logic */
    /* Transition: '<S1>:104' */
    /* Entry Internal 'F': '<S1>:103' */
    /* Entry Internal 'F1': '<S1>:184' */
    /* Transition: '<S1>:182' */
    DischargeOverCurrent_DW.is_F1 = DischargeOverCurrent_IN_Wait1;

    /* Entry Internal 'F2': '<S1>:202' */
    /* Transition: '<S1>:210' */
    DischargeOverCurrent_DW.is_F2 = DischargeOverCurrent_IN_Wait2;

    /* Entry Internal 'F3': '<S1>:102' */
    /* Transition: '<S1>:108' */
    DischargeOverCurrent_DW.is_F3 = DischargeOverCurrent_IN_Wait3;
  } else {
    /* During 'F': '<S1>:103' */
    /* During 'F1': '<S1>:184' */
    switch (DischargeOverCurrent_DW.is_F1) {
     case DischargeOverCurrent_IN_Delay10:
      /* During 'Delay10': '<S1>:192' */
      if (!(cur > 1.1F * curM)) {
        /* Transition: '<S1>:185' */
        DischargeOverCurrent_DW.is_F1 = DischargeOverCurrent_IN_Wait1;
      } else {
        if (DischargeOverCurrent_DW.temporalCounter_i1 >= 10) {
          /* Transition: '<S1>:183' */
          DischargeOverCurrent_DW.is_F1 = DischargeOverCurrent_IN_Fault1;

          /* Entry 'Fault1': '<S1>:190' */
          DischargeOverCurrent_B.F_lev_i = 1U;
        }
      }
      break;

     case DischargeOverCurrent_IN_Delay11:
      /* During 'Delay11': '<S1>:188' */
      if (!(cur <= curM)) {
        /* Transition: '<S1>:191' */
        DischargeOverCurrent_DW.is_F1 = DischargeOverCurrent_IN_Fault1;

        /* Entry 'Fault1': '<S1>:190' */
        DischargeOverCurrent_B.F_lev_i = 1U;
      } else {
        if (DischargeOverCurrent_DW.temporalCounter_i1 >= 10) {
          /* Transition: '<S1>:181' */
          DischargeOverCurrent_DW.is_F1 = DischargeOverCurrent_IN_Fault10;

          /* Entry 'Fault10': '<S1>:187' */
          DischargeOverCurrent_B.F_lev_i = 0U;
        }
      }
      break;

     case DischargeOverCurrent_IN_Fault1:
      /* During 'Fault1': '<S1>:190' */
      if (cur <= curM) {
        /* Transition: '<S1>:189' */
        DischargeOverCurrent_DW.is_F1 = DischargeOverCurrent_IN_Delay11;
        DischargeOverCurrent_DW.temporalCounter_i1 = 0U;
      }
      break;

     case DischargeOverCurrent_IN_Fault10:
      /* During 'Fault10': '<S1>:187' */
      /* Transition: '<S1>:193' */
      DischargeOverCurrent_DW.is_F1 = DischargeOverCurrent_IN_Wait1;
      break;

     default:
      /* During 'Wait1': '<S1>:186' */
      if (cur > 1.1F * curM) {
        /* Transition: '<S1>:194' */
        DischargeOverCurrent_DW.is_F1 = DischargeOverCurrent_IN_Delay10;
        DischargeOverCurrent_DW.temporalCounter_i1 = 0U;
      }
      break;
    }

    /* During 'F2': '<S1>:202' */
    switch (DischargeOverCurrent_DW.is_F2) {
     case DischargeOverCurrent_IN_Delay20:
      /* During 'Delay20': '<S1>:207' */
      if (!(cur > 1.2F * curM)) {
        /* Transition: '<S1>:198' */
        DischargeOverCurrent_DW.is_F2 = DischargeOverCurrent_IN_Wait2;
      } else {
        if (DischargeOverCurrent_DW.temporalCounter_i2 >= 10) {
          /* Transition: '<S1>:209' */
          DischargeOverCurrent_DW.is_F2 = DischargeOverCurrent_IN_Fault2;

          /* Entry 'Fault2': '<S1>:200' */
          DischargeOverCurrent_B.F_lev_i = 2U;
        }
      }
      break;

     case DischargeOverCurrent_IN_Delay21:
      /* During 'Delay21': '<S1>:203' */
      if (!(cur <= curM)) {
        /* Transition: '<S1>:199' */
        DischargeOverCurrent_DW.is_F2 = DischargeOverCurrent_IN_Fault2;

        /* Entry 'Fault2': '<S1>:200' */
        DischargeOverCurrent_B.F_lev_i = 2U;
      } else {
        if (DischargeOverCurrent_DW.temporalCounter_i2 >= 10) {
          /* Transition: '<S1>:206' */
          DischargeOverCurrent_DW.is_F2 = DischargeOverCurrent_IN_Fault20;

          /* Entry 'Fault20': '<S1>:204' */
          DischargeOverCurrent_B.F_lev_i = 0U;
        }
      }
      break;

     case DischargeOverCurrent_IN_Fault2:
      /* During 'Fault2': '<S1>:200' */
      if (cur <= curM) {
        /* Transition: '<S1>:205' */
        DischargeOverCurrent_DW.is_F2 = DischargeOverCurrent_IN_Delay21;
        DischargeOverCurrent_DW.temporalCounter_i2 = 0U;
      }
      break;

     case DischargeOverCurrent_IN_Fault20:
      /* During 'Fault20': '<S1>:204' */
      /* Transition: '<S1>:208' */
      DischargeOverCurrent_DW.is_F2 = DischargeOverCurrent_IN_Wait2;
      break;

     default:
      /* During 'Wait2': '<S1>:201' */
      if (cur > 1.2F * curM) {
        /* Transition: '<S1>:211' */
        DischargeOverCurrent_DW.is_F2 = DischargeOverCurrent_IN_Delay20;
        DischargeOverCurrent_DW.temporalCounter_i2 = 0U;
      }
      break;
    }

    /* During 'F3': '<S1>:102' */
    switch (DischargeOverCurrent_DW.is_F3) {
     case DischargeOverCurrent_IN_Delay30:
      /* During 'Delay30': '<S1>:28' */
      if (!(cur > 1.5F * curM)) {
        /* Transition: '<S1>:24' */
        DischargeOverCurrent_DW.is_F3 = DischargeOverCurrent_IN_Wait3;
      } else {
        if (DischargeOverCurrent_DW.temporalCounter_i3 >= 10) {
          /* Transition: '<S1>:26' */
          DischargeOverCurrent_DW.is_F3 = DischargeOverCurrent_IN_Fault3;

          /* Entry 'Fault3': '<S1>:23' */
          DischargeOverCurrent_B.F_lev_i = 3U;
        }
      }
      break;

     case DischargeOverCurrent_IN_Fault3:
      /* During 'Fault3': '<S1>:23' */
      DischargeOverCurrent_B.F_lev_i = 3U;
      break;

     default:
      /* During 'Wait3': '<S1>:98' */
      if (cur > 1.5F * curM) {
        /* Transition: '<S1>:25' */
        DischargeOverCurrent_DW.is_F3 = DischargeOverCurrent_IN_Delay30;
        DischargeOverCurrent_DW.temporalCounter_i3 = 0U;
      }
      break;
    }
  }

  /* End of Chart: '<Root>/Logic' */

  /* Outport: '<Root>/F_lev' */
  F_lev = DischargeOverCurrent_B.F_lev_i;
  return F_lev;
}

/* Model initialize function */
void DischargeOverCurrent_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(DischargeOverCurrent_M, (NULL));

  /* block I/O */
  (void) memset(((void *) &DischargeOverCurrent_B), 0,
                sizeof(B_DischargeOverCurrent_T));

  /* states (dwork) */
  (void) memset((void *)&DischargeOverCurrent_DW, 0,
                sizeof(DW_DischargeOverCurrent_T));

  /* SystemInitialize for Chart: '<Root>/Logic' */
  DischargeOverCurrent_DW.is_F1 = DischargeOve_IN_NO_ACTIVE_CHILD;
  DischargeOverCurrent_DW.temporalCounter_i1 = 0U;
  DischargeOverCurrent_DW.is_F2 = DischargeOve_IN_NO_ACTIVE_CHILD;
  DischargeOverCurrent_DW.temporalCounter_i2 = 0U;
  DischargeOverCurrent_DW.is_F3 = DischargeOve_IN_NO_ACTIVE_CHILD;
  DischargeOverCurrent_DW.temporalCounter_i3 = 0U;
  DischargeOverCurrent_DW.is_active_c3_DischargeOverCurre = 0U;
  DischargeOverCurrent_B.F_lev_i = 0U;
}

/* Model terminate function */
void DischargeOverCurrent_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
