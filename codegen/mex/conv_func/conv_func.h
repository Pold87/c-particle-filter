/*
 * conv_func.h
 *
 * Code generation for function 'conv_func'
 *
 */

#ifndef CONV_FUNC_H
#define CONV_FUNC_H

/* Include files */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include "covrt.h"
#include "rtwtypes.h"
#include "conv_func_types.h"

/* Function Declarations */
extern real_T conv_func(const emlrtStack *sp, real_T x, real_T y);

#ifdef __WATCOMC__

#pragma aux conv_func value [8087];

#endif
#endif

/* End of code generation (conv_func.h) */
