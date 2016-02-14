/*
 * conv_func_terminate.c
 *
 * Code generation for function 'conv_func_terminate'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "conv_func.h"
#include "conv_func_terminate.h"
#include "_coder_conv_func_mex.h"
#include "conv_func_data.h"

/* Function Definitions */
void conv_func_atexit(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);

  /* Free instance data */
  covrtFreeInstanceData(&emlrtCoverageInstance);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

void conv_func_terminate(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/* End of code generation (conv_func_terminate.c) */
