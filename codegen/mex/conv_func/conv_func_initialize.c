/*
 * conv_func_initialize.c
 *
 * Code generation for function 'conv_func_initialize'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "conv_func.h"
#include "conv_func_initialize.h"
#include "_coder_conv_func_mex.h"
#include "conv_func_data.h"

/* Function Declarations */
static void conv_func_once(void);

/* Function Definitions */
static void conv_func_once(void)
{
  /* Allocate instance data */
  covrtAllocateInstanceData(&emlrtCoverageInstance);

  /* Initialize Coverage Information */
  covrtScriptInit(&emlrtCoverageInstance,
                  "/home/pold/Documents/Internship/particle_filter/conv_func.m",
                  0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0);

  /* Initialize Function Information */
  covrtFcnInit(&emlrtCoverageInstance, 0, 0, "conv_func", 10, -1, 145);

  /* Initialize Basic Block Information */
  covrtBasicBlockInit(&emlrtCoverageInstance, 0, 0, 130, -1, 139);

  /* Initialize If Information */
  /* Initialize MCDC Information */
  /* Initialize For Information */
  /* Initialize While Information */
  /* Initialize Switch Information */
  /* Start callback for coverage engine */
  covrtScriptStart(&emlrtCoverageInstance, 0U);
}

void conv_func_initialize(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  mexFunctionCreateRootTLS();
  emlrtBreakCheckR2012bFlagVar = emlrtGetBreakCheckFlagAddressR2012b();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  if (emlrtFirstTimeR2012b(emlrtRootTLSGlobal)) {
    conv_func_once();
  }
}

/* End of code generation (conv_func_initialize.c) */
