#include "fir/ir_func.h"
#include "fir/config.h"
#include <stdlib.h>

static void frIRFunc_print_prototype(frIRFunc *func, FILE *out) {
  frIRType_print(func->type, out, func->name);
  fputs(" (", out);

  frIRType **arg_type = func->args;
  size_t i = 0;
  while (*arg_type) {
    if (arg_type != func->args) {
      fputs(",", out);
    }
    frIRType_print(*arg_type, out, FR_FUNC_ARG_NAMES[i]);
    arg_type += 1;
    i++;
  }

  fputs(")", out);
}

void frIRFunc_header_print(frIRFunc *func, FILE *out) {
  frIRFunc_print_prototype(func, out);
  fputs(";\n", out);
}

void frIRFunc_print(frIRFunc *func, FILE *out) {
  if (func->isextrn)
    return;

  frIRFunc_print_prototype(func, out);
  fputs("{\n", out);

  frIRBlock *block = func->block;
  while (block) {
    frIRBlock_print_decls(block, out);
    block = block->next;
  }

  block = func->block;
  while (block) {
    frIRBlock_print(block, out);
    block = block->next;
  }

  fputs("}\n", out);
}

void frIRFunc_free(frIRFunc *func) {
  frIRBlock *block = func->block;
  while (block) {
    frIRBlock *next = block->next;
    frIRBlock_free(block);
    free(block);
    block = next;
  }
}
