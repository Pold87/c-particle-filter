/*
 * conv_func.c
 *
 * Code generation for function 'conv_func'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "conv_func.h"
#include "conv_func_data.h"

/* Function Definitions */
real_T conv_func(const emlrtStack *sp, real_T x, real_T y)
{
  (void)sp;
  covrtLogFcn(&emlrtCoverageInstance, 0U, 0);
  covrtLogBasicBlock(&emlrtCoverageInstance, 0U, 0);

  /* UNTITLED Summary of this function goes here */
  /*    Detailed explanation goes here */
  return x + y;
}

/* End of code generation (conv_func.c) */
