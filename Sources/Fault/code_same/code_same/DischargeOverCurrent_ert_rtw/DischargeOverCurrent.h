/*
 * File: DischargeOverCurrent.h
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

#ifndef RTW_HEADER_DischargeOverCurrent_h_
#define RTW_HEADER_DischargeOverCurrent_h_
#include <stddef.h>
#include <string.h>
#ifndef DischargeOverCurrent_COMMON_INCLUDES_
# define DischargeOverCurrent_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* DischargeOverCurrent_COMMON_INCLUDES_ */

#include "DischargeOverCurrent_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block signals (auto storage) */
typedef struct {
  uint8_T F_lev_i;                     /* '<Root>/Logic' */
} B_DischargeOverCurrent_T;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  uint8_T is_active_c3_DischargeOverCurre;/* '<Root>/Logic' */
  uint8_T is_F1;                       /* '<Root>/Logic' */
  uint8_T is_F2;                       /* '<Root>/Logic' */
  uint8_T is_F3;                       /* '<Root>/Logic' */
  uint8_T temporalCounter_i1;          /* '<Root>/Logic' */
  uint8_T temporalCounter_i2;          /* '<Root>/Logic' */
  uint8_T temporalCounter_i3;          /* '<Root>/Logic' */
} DW_DischargeOverCurrent_T;

/* Real-time Model Data Structure */
struct tag_RTM_DischargeOverCurrent_T {
  const char_T * volatile errorStatus;
};

/* Block signals (auto storage) */
extern B_DischargeOverCurrent_T DischargeOverCurrent_B;

/* Block states (auto storage) */
extern DW_DischargeOverCurrent_T DischargeOverCurrent_DW;

/* Model entry point functions */
extern void DischargeOverCurrent_initialize(void);
extern void DischargeOverCurrent_terminate(void);

/* Customized model step function */
extern uint8_T DischargeOverCurrent_custom(real32_T cur, real32_T curM);

/* Real-time Model object */
extern RT_MODEL_DischargeOverCurrent_T *const DischargeOverCurrent_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'DischargeOverCurrent'
 * '<S1>'   : 'DischargeOverCurrent/Logic'
 */
#endif                                 /* RTW_HEADER_DischargeOverCurrent_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
