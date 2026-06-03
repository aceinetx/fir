#ifndef FEATHER_IR_FUNC_H
#define FEATHER_IR_FUNC_H

#include "config.h"
#include "ir_block.h"
#include <stdio.h>

FR_HEADER_BEGIN

typedef struct frIRFunc {
  frIRType *args[FR_FUNC_MAX_ARGS];
  const char *name;
  frIRType *type;
  frIRBlock *block;

  struct frIRFunc *next;
  int isextrn;
} frIRFunc;

void frIRFunc_header_print(frIRFunc *func, FILE *out);
void frIRFunc_print(frIRFunc *func, FILE *out);
void frIRFunc_free(frIRFunc *func);

FR_HEADER_END

#endif
