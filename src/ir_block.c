#include "fir/ir_block.h"
#include "fir/ir_func.h"
#include <stdlib.h>

void frIRBlock_print(frIRBlock *block, FILE *out) {
  fprintf(out, "%s:{\n", block->name);

  frIRInst *inst = block->inst;
  while (inst) {
    frIRInst_print(inst, out);
    inst = inst->next;
  }
  fputs("}\n", out);
}

void frIRBlock_print_decls(frIRBlock *block, FILE *out) {
  frIRInst *inst = block->inst;
  while (inst) {
    if (inst->type == FR_ALLOCA) {
      const char *s = inst->v.alloca.name;
      frIRType alloca_type =
          (frIRType){.unit = FR_TYPE_ARR,
                     .v.array = {.type = inst->v.alloca.type, .size = 1}};
      if (inst->v.alloca.type->unit == FR_TYPE_ARR) {
        alloca_type = *inst->v.alloca.type;
      }
      frIRType_print(&alloca_type, out, s);
      fputs(";\n", out);
    } else if (inst->type == FR_CONST_INT) {
      frIRType_print(inst->v.const_int.type, out, inst->v.const_int.name);
      fputs(";\n", out);
    } else if (inst->type == FR_CONST_FLOAT) {
      frIRType_print(inst->v.const_float.type, out, inst->v.const_float.name);
      fputs(";\n", out);
    } else if (inst->type == FR_CONST_STR) {
      frIRType_print(&frIRType_str, out, inst->v.const_str.name);
      fputs(";\n", out);
    } else if (inst->type == FR_ARRINDEX) {
      frIRType_print(&inst->v.arrindex.type, out, inst->v.arrindex.name);
      fputs(";\n", out);
    } else if (inst->type == FR_CAST) {
      frIRType_print(inst->v.cast.type, out, inst->v.cast.name);
      fputs(";\n", out);
    } else if (inst->type == FR_LOAD) {
      frIRType_print(inst->v.load.type, out, inst->v.load.name);
      fputs(";\n", out);
    } else if (inst->type == FR_CALL) {
      frIRType_print(inst->v.call.ret_type, out, inst->v.call.name);
      fputs(";\n", out);
    } else if (inst->type == FR_BINOP) {
      frIRType_print(inst->v.binop.type, out, inst->v.binop.name);
      fputs(";\n", out);
    }
    inst = inst->next;
  }
}

void frIRBlock_unlink(frIRBlock *block) {
  frIRBlock **head = &block->func->block;
  if (block->prev)
    block->prev->next = block->next;
  else
    *head = block->next; // block was head

  if (block->next)
    block->next->prev = block->prev;
}

void frIRBlock_insert_after(frIRBlock *block, frIRBlock *after) {
  block->prev = after;
  block->next = after->next;

  if (after->next)
    after->next->prev = block;

  after->next = block;
}

void frIRBlock_set_after(frIRBlock *block, frIRBlock *after) {
  if (block == after)
    return;
  frIRBlock **head = &block->func->block;

  frIRBlock_unlink(block);

  if (after == NULL) {
    /* move to front */
    block->prev = NULL;
    block->next = *head;

    if (*head)
      (*head)->prev = block;

    *head = block;
  } else {
    frIRBlock_insert_after(block, after);
  }
}

void frIRBlock_free(frIRBlock *block) {
  frIRInst *inst = block->inst;
  while (inst) {
    frIRInst *next = inst->next;
    free(inst);
    inst = next;
  }
}
