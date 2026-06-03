#ifndef FEATHER_IR_BLOCK_H
#define FEATHER_IR_BLOCK_H
#include "config.h"
#include "ir_inst.h"
#include <stdio.h>

FR_HEADER_BEGIN

struct frIRFunc;

typedef struct frIRBlock {
  frIRInst *inst;
  char name[FR_IDENT_SIZE];

  struct frIRBlock *next, *prev;
  struct frIRFunc *func;
} frIRBlock;

void frIRBlock_print(frIRBlock *block, FILE *out);
void frIRBlock_print_decls(frIRBlock *block, FILE *out);
void frIRBlock_unlink(frIRBlock *block);
void frIRBlock_insert_after(frIRBlock *block, frIRBlock *after);
void frIRBlock_set_after(frIRBlock *block, frIRBlock *after);
void frIRBlock_free(frIRBlock *block);

FR_HEADER_END

#endif
