#include "fir/ir.h"
#include "fir/arena.h"
#include "fir/ir_inst.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

frIR frIR_new(void) {
  frIR ir;
  FR_ZERO(&ir);
  ir.strings_arena = frArena_new(0x100);

  return ir;
}

void frIR_free(frIR *ir) {
  frIRFunc *func = ir->func;
  while (func) {
    frIRFunc *next = func->next;
    frIRFunc_free(func);
    free(func);
    func = next;
  }

  frArena_free(&ir->strings_arena);
}

char *frIR_strdup(frIR *ir, const char *s) {
  size_t len = strlen(s) + 1;
  char *news = frArena_alloc(&ir->strings_arena, len);
  memcpy(news, s, len);
  return news;
}

void frIR_print(frIR *ir, FILE *fd) {
  fputs("#include<stdint.h>\n", fd);

  frIRFunc *func = ir->func;
  while (func) {
    frIRFunc_header_print(func, fd);
    func = func->next;
  }

  func = ir->func;
  while (func) {
    frIRFunc_print(func, fd);
    func = func->next;
  }
}

void frIR_temp(frIR *ir, char s[FR_IDENT_SIZE]) {
  sprintf(s, "_%zu", ir->temp_index++);
}

frIRBlock *frIR_block(frIR *ir) {
  frIRBlock *block = malloc(sizeof(frIRBlock));
  *block = (frIRBlock){.inst = NULL, .func = ir->insert_point.func};
  frIR_temp(ir, block->name);

  frIRBlock_insert_after(block, ir->insert_point.block);
  return block;
}

frIRFunc *frIR_func(frIR *ir, frIRType *type, const char *name,
                    frIRType *args[FR_FUNC_MAX_ARGS]) {
  ir->temp_index = 0;

  frIRFunc *func = malloc(sizeof *func);
  *func = (frIRFunc){.name = frIR_strdup(ir, name),
                     .type = type,
                     .block = malloc(sizeof(frIRBlock)),
                     .next = ir->func};
  memcpy(func->args, args, sizeof *args * FR_FUNC_MAX_ARGS);
  *func->block =
      (frIRBlock){.inst = NULL, .next = NULL, .prev = NULL, .func = func};
  frIR_temp(ir, func->block->name);

  frIR_set_insert_func(ir, func);

  ir->func = func;
  ir->insert_point.func = func;
  return func;
}

frIRFunc *frIR_extfunc(frIR *ir, frIRType *type, const char *name,
                       frIRType *args[FR_FUNC_MAX_ARGS]) {
  ir->temp_index = 0;

  frIRFunc *func = malloc(sizeof *func);
  *func = (frIRFunc){.name = frIR_strdup(ir, name),
                     .type = type,
                     .block = NULL,
                     .next = ir->func,
                     .isextrn = 1};
  memcpy(func->args, args, sizeof *args * FR_FUNC_MAX_ARGS);

  frIR_set_insert_point(ir, NULL);

  ir->func = func;
  return func;
}

void frIR_insert(frIR *ir, frIRInst *inst) {
  if (ir->insert_point.inst) {
    ir->insert_point.inst->next = inst;
  } else {
    ir->insert_point.block->inst = inst;
  }
  ir->insert_point.inst = inst;
}

void frIR_set_insert_point(frIR *ir, frIRInst *inst) {
  ir->insert_point = (frInsertPoint){0};
  if (!inst) {
    return;
  }

  ir->insert_point.inst = inst;
  ir->insert_point.block = inst->block;
}

void frIR_set_insert_block(frIR *ir, frIRBlock *block) {
  ir->insert_point = (frInsertPoint){0};
  if (!block) {
    return;
  }

  ir->insert_point.block = block;
  ir->insert_point.func = block->func;

  frIRInst *inst = block->inst;
  while (inst) {
    if (inst->next == NULL)
      break;
    inst = inst->next;
  }

  ir->insert_point.inst = inst;
}

void frIR_set_insert_func(frIR *ir, frIRFunc *func) {
  ir->insert_point = (frInsertPoint){0};
  if (!func) {
    return;
  }

  frIRBlock *block = func->block;
  while (block) {
    if (block->next == NULL)
      break;
    block = block->next;
  }

  frIR_set_insert_block(ir, block);
}

frIRValue frIR_arg(frIR *ir, size_t index) {
  frIRFunc *func = ir->func;
  if (index >= FR_FUNC_MAX_ARGS) {
    return (frIRValue){.name = "_error_arg_exceeds_max_values",
                       .type = &frIRType_u0};
  }

  return (frIRValue){.name = FR_FUNC_ARG_NAMES[index],
                     .type = func->args[index]};
}

/* instructions */
frIRValue frIR_alloca(frIR *ir, frIRType *type) {
  assert(type != &frIRType_u0);
  frIRInst *inst = malloc(sizeof *inst);
  *inst =
      (frIRInst){.type = FR_ALLOCA, .v.alloca = {.type = type}, .next = NULL};
  frIR_temp(ir, inst->v.alloca.name);

  frIR_insert(ir, inst);
  return (frIRValue){.name = inst->v.alloca.name, .type = type};
}

void frIR_store(frIR *ir, frIRValue *dest, frIRValue *src) {
  frIRInst *inst = malloc(sizeof *inst);
  *inst =
      (frIRInst){.type = FR_STORE,
                 .v.store = {.name_dest = dest->name, .name_src = src->name},
                 .next = NULL};
  frIR_insert(ir, inst);
}

frIRValue frIR_load(frIR *ir, frIRValue *value, frIRType *type) {
  frIRInst *inst = malloc(sizeof *inst);
  *inst = (frIRInst){.type = FR_LOAD,
                     .v.load = {.type = type, .source = value->name},
                     .next = NULL};
  frIR_temp(ir, inst->v.load.name);
  frIR_insert(ir, inst);
  return (frIRValue){.name = inst->v.load.name, .type = type};
}

frIRValue frIR_constint(frIR *ir, frIRType *type, long number) {
  frIRInst *inst = malloc(sizeof *inst);
  *inst = (frIRInst){.type = FR_CONST_INT,
                     .v.const_int = {.type = type, .number = number},

                     .next = NULL};
  frIR_temp(ir, inst->v.const_int.name);
  frIR_insert(ir, inst);
  return (frIRValue){.name = inst->v.const_int.name, .type = type};
}

frIRValue frIR_constfloat(frIR *ir, frIRType *type, double number) {
  frIRInst *inst = malloc(sizeof *inst);
  *inst = (frIRInst){.type = FR_CONST_FLOAT,
                     .v.const_float = {.type = type, .number = number},
                     .next = NULL};
  frIR_temp(ir, inst->v.const_float.name);
  frIR_insert(ir, inst);
  return (frIRValue){.name = inst->v.const_float.name, .type = type};
}

frIRValue frIR_conststr(frIR *ir, const char *s) {
  frIRInst *inst = malloc(sizeof *inst);
  *inst = (frIRInst){.type = FR_CONST_STR,
                     .v.const_str.str = frIR_strdup(ir, s),
                     .next = NULL};
  frIR_temp(ir, inst->v.const_str.name);
  frIR_insert(ir, inst);
  return (frIRValue){.name = inst->v.const_str.name, .type = &frIRType_str};
}

void frIR_ret(frIR *ir, frIRValue *value) {
  frIRInst *inst = malloc(sizeof *inst);
  if (value)
    *inst = (frIRInst){
        .type = FR_RET, .v.ret = {.name = value->name}, .next = NULL};
  else
    *inst = (frIRInst){.type = FR_RET, .v.ret = {.name = NULL}, .next = NULL};
  frIR_insert(ir, inst);
}

static frIRValue frIR_binop(frIR *ir, frIRValue *lhs, frIRValue *rhs,
                            int type) {
  frIRInst *inst = malloc(sizeof *inst);
  *inst = (frIRInst){.type = FR_BINOP,
                     .v.binop = {.type = lhs->type,
                                 .name_lhs = lhs->name,
                                 .name_rhs = rhs->name,
                                 .op = type},
                     .next = NULL};
  frIR_temp(ir, inst->v.binop.name);
  frIR_insert(ir, inst);
  return (frIRValue){.name = inst->v.binop.name, .type = lhs->type};
}

frIRValue frIR_add(frIR *ir, frIRValue *lhs, frIRValue *rhs) {
  return frIR_binop(ir, lhs, rhs, FR_BINOP_ADD);
}

frIRValue frIR_sub(frIR *ir, frIRValue *lhs, frIRValue *rhs) {
  return frIR_binop(ir, lhs, rhs, FR_BINOP_SUB);
}

frIRValue frIR_mul(frIR *ir, frIRValue *lhs, frIRValue *rhs) {
  return frIR_binop(ir, lhs, rhs, FR_BINOP_MUL);
}

frIRValue frIR_div(frIR *ir, frIRValue *lhs, frIRValue *rhs) {
  return frIR_binop(ir, lhs, rhs, FR_BINOP_DIV);
}

frIRValue frIR_eq(frIR *ir, frIRValue *lhs, frIRValue *rhs) {
  return frIR_binop(ir, lhs, rhs, FR_BINOP_EQUAL);
}

frIRValue frIR_neq(frIR *ir, frIRValue *lhs, frIRValue *rhs) {
  return frIR_binop(ir, lhs, rhs, FR_BINOP_NEQUAL);
}

frIRValue frIR_lt(frIR *ir, frIRValue *lhs, frIRValue *rhs) {
  return frIR_binop(ir, lhs, rhs, FR_BINOP_LT);
}

frIRValue frIR_gt(frIR *ir, frIRValue *lhs, frIRValue *rhs) {
  return frIR_binop(ir, lhs, rhs, FR_BINOP_GT);
}

frIRValue frIR_lte(frIR *ir, frIRValue *lhs, frIRValue *rhs) {
  return frIR_binop(ir, lhs, rhs, FR_BINOP_LTE);
}

frIRValue frIR_gte(frIR *ir, frIRValue *lhs, frIRValue *rhs) {
  return frIR_binop(ir, lhs, rhs, FR_BINOP_GTE);
}

frIRValue frIR_bitand(frIR *ir, frIRValue *lhs, frIRValue *rhs) {
  return frIR_binop(ir, lhs, rhs, FR_BINOP_AND);
}

frIRValue frIR_bitor(frIR *ir, frIRValue *lhs, frIRValue *rhs) {
  return frIR_binop(ir, lhs, rhs, FR_BINOP_OR);
}

frIRValue frIR_bitxor(frIR *ir, frIRValue *lhs, frIRValue *rhs) {
  return frIR_binop(ir, lhs, rhs, FR_BINOP_XOR);
}

frIRValue frIR_bitshl(frIR *ir, frIRValue *lhs, frIRValue *rhs) {
  return frIR_binop(ir, lhs, rhs, FR_BINOP_SHL);
}

frIRValue frIR_bitshr(frIR *ir, frIRValue *lhs, frIRValue *rhs) {
  return frIR_binop(ir, lhs, rhs, FR_BINOP_SHR);
}

frIRValue frIR_select(frIR *ir, frIRValue *test, frIRValue *true_value,
                      frIRValue *false_value) {
  if (true_value == false_value)
    return *true_value;

  frIRInst *inst = malloc(sizeof *inst);
  *inst = (frIRInst){.type = FR_SELECT,
                     .v.select =
                         {
                             .type = true_value->type,
                             .name_test = test->name,
                             .name_true = true_value->name,
                             .name_false = false_value->name,
                         },
                     .next = NULL};
  frIR_temp(ir, inst->v.select.name);
  frIR_insert(ir, inst);
  return (frIRValue){.name = inst->v.select.name, .type = true_value->type};
}

void frIR_br(frIR *ir, frIRBlock *block) {
  frIRInst *inst = malloc(sizeof *inst);
  *inst =
      (frIRInst){.type = FR_BR, .v.br.block_name = block->name, .next = NULL};
  frIR_insert(ir, inst);
}

void frIR_cond_br(frIR *ir, frIRValue *value, frIRBlock *true_block,
                  frIRBlock *false_block) {
  if (true_block == false_block) {
    frIR_br(ir, true_block);
    return;
  }

  frIRInst *inst = malloc(sizeof *inst);
  *inst = (frIRInst){.type = FR_COND_BR,
                     .v.cond_br = {.name_value = value->name,
                                   .true_block = true_block->name,
                                   .false_block = false_block->name},
                     .next = NULL};
  frIR_insert(ir, inst);
}

frIRValue frIR_cast(frIR *ir, frIRValue *value, frIRType *type) {
  if (type == value->type) {
    return *value;
  }

  frIRInst *inst = malloc(sizeof *inst);
  *inst = (frIRInst){.type = FR_CAST,
                     .v.cast = {.name_cast = value->name, .type = type},
                     .next = NULL};
  frIR_temp(ir, inst->v.cast.name);
  frIR_insert(ir, inst);
  return (frIRValue){.name = inst->v.cast.name, .type = type};
}

frIRValue frIR_call(frIR *ir, frIRFunc *func,
                    frIRValue *values[FR_FUNC_MAX_ARGS]) {
  frIRInst *inst = malloc(sizeof *inst);
  *inst = (frIRInst){
      .type = FR_CALL,
      .v.call = {.func = func->name, .args = {0}, .ret_type = func->type},
      .next = NULL};
  frIR_temp(ir, inst->v.call.name);
  for (int i = 0; i < FR_FUNC_MAX_ARGS; i++) {
    frIRValue *v = values[i];
    if (!v)
      break;
    inst->v.call.args[i] = v->name;
  }
  frIR_insert(ir, inst);
  return (frIRValue){.name = inst->v.call.name, .type = func->type};
}

void frIR_arrset(frIR *ir, frIRValue *array, frIRValue *index,
                 frIRValue *value) {
  assert(array->type->unit == FR_TYPE_PTR || array->type->unit == FR_TYPE_ARR);
  frIRInst *inst = malloc(sizeof *inst);
  *inst = (frIRInst){.type = FR_ARRSET,
                     .v.arrset = {.array = array->name,
                                  .index = index->name,
                                  .value = value->name},
                     .next = NULL};
  frIR_insert(ir, inst);
}

frIRValue frIR_arrget(frIR *ir, frIRValue *array, frIRValue *index) {
  assert(array->type->unit == FR_TYPE_PTR || array->type->unit == FR_TYPE_ARR);

  frIRType *type = array->type->unit == FR_TYPE_PTR ? array->type->v.ptr.pointee
                                                    : array->type->v.array.type;

  frIRInst *inst = malloc(sizeof *inst);
  *inst = (frIRInst){
      .type = FR_ARRGET,
      .v.arrget = {.array = array->name, .index = index->name, .type = type},
      .next = NULL};
  frIR_temp(ir, inst->v.arrget.name);
  frIR_insert(ir, inst);
  return (frIRValue){.name = inst->v.arrget.name, .type = type};
}
