#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "types.h"

#define FRACTIONAL_BITS 12
#define FLOAT_TO_FP(a)  ((FP)((a)*(1<<FRACTIONAL_BITS)))
#define MUL_FP(a, b)    ((FP)(((s64)(a)*(s64)(b))>>FRACTIONAL_BITS))
#define DIV_FP(a, b)    ((FP)(((s64)(a)<<FRACTIONAL_BITS)/(b)))
#define FP_TO_INT(a)    ((s32)(a)>>FRACTIONAL_BITS)
#define FP_TO_FLOAT(c)  ((c)/(float)(1<<FRACTIONAL_BITS))

void populateTables(void);
