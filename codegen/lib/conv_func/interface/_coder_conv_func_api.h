/*
 * File: _coder_conv_func_api.h
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 07-Feb-2016 23:23:36
 */

#ifndef _CODER_CONV_FUNC_API_H
#define _CODER_CONV_FUNC_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_conv_func_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern real_T conv_func(real_T x, real_T y);
extern void conv_func_api(const mxArray * const prhs[2], const mxArray *plhs[1]);
extern void conv_func_atexit(void);
extern void conv_func_initialize(void);
extern void conv_func_terminate(void);
extern void conv_func_xil_terminate(void);

#endif

/*
 * File trailer for _coder_conv_func_api.h
 *
 * [EOF]
 */
