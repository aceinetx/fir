#ifndef FEATHER_IR_H
#define FEATHER_IR_H
#include "arena.h"
#include "config.h"
#include "ir_func.h"
#include "ir_insert_point.h"
#include "ir_inst.h"
#include "ir_value.h"
#include <stdio.h>

FR_HEADER_BEGIN

typedef struct frIR {
  frIRFunc *func;
  size_t temp_index;

  frInsertPoint insert_point;
} frIR;

frIR frIR_new(void);
void frIR_free(frIR *ir);

void frIR_print(frIR *ir, FILE *fd);
void frIR_temp(frIR *ir, char s[FR_IDENT_SIZE]);
frIRBlock *frIR_block(frIR *ir);
frIRFunc *frIR_func(frIR *ir, frIRType *type, const char *name,
                    frIRType *args[FR_FUNC_MAX_ARGS]);
frIRFunc *frIR_extfunc(frIR *ir, frIRType *type, const char *name,
                       frIRType *args[FR_FUNC_MAX_ARGS]);
void frIR_insert(frIR *ir, frIRInst *inst);
void frIR_set_insert_point(frIR *ir, frIRInst *inst);
void frIR_set_insert_block(frIR *ir, frIRBlock *block);
void frIR_set_insert_func(frIR *ir, frIRFunc *func);
frIRValue frIR_arg(frIR *ir, size_t index);

/* instructions */
frIRValue frIR_alloca(frIR *ir, frIRType *type);
void frIR_store(frIR *ir, frIRValue *dest, frIRValue *src);
frIRValue frIR_load(frIR *ir, frIRValue *value, frIRType *type);
frIRValue frIR_constint(frIR *ir, frIRType *type, long number);
frIRValue frIR_constfloat(frIR *ir, frIRType *type, double number);
frIRValue frIR_conststr(frIR *ir, const char *s);
void frIR_ret(frIR *ir, frIRValue *value);
frIRValue frIR_add(frIR *ir, frIRValue *lhs, frIRValue *rhs);
frIRValue frIR_sub(frIR *ir, frIRValue *lhs, frIRValue *rhs);
frIRValue frIR_mul(frIR *ir, frIRValue *lhs, frIRValue *rhs);
frIRValue frIR_div(frIR *ir, frIRValue *lhs, frIRValue *rhs);
frIRValue frIR_eq(frIR *ir, frIRValue *lhs, frIRValue *rhs);
frIRValue frIR_neq(frIR *ir, frIRValue *lhs, frIRValue *rhs);
frIRValue frIR_lt(frIR *ir, frIRValue *lhs, frIRValue *rhs);
frIRValue frIR_gt(frIR *ir, frIRValue *lhs, frIRValue *rhs);
frIRValue frIR_lte(frIR *ir, frIRValue *lhs, frIRValue *rhs);
frIRValue frIR_gte(frIR *ir, frIRValue *lhs, frIRValue *rhs);
frIRValue frIR_bitand(frIR *ir, frIRValue *lhs, frIRValue *rhs);
frIRValue frIR_bitor(frIR *ir, frIRValue *lhs, frIRValue *rhs);
frIRValue frIR_bitxor(frIR *ir, frIRValue *lhs, frIRValue *rhs);
frIRValue frIR_bitshl(frIR *ir, frIRValue *lhs, frIRValue *rhs);
frIRValue frIR_bitshr(frIR *ir, frIRValue *lhs, frIRValue *rhs);
frIRValue frIR_select(frIR *ir, frIRValue *test, frIRValue *true_value,
                      frIRValue *false_value);
void frIR_br(frIR *ir, frIRBlock *block);
void frIR_cond_br(frIR *ir, frIRValue *value, frIRBlock *true_block,
                  frIRBlock *false_block);
frIRValue frIR_cast(frIR *ir, frIRValue *value, frIRType *type);
frIRValue frIR_call(frIR *ir, frIRFunc *func,
                    frIRValue *values[FR_FUNC_MAX_ARGS]);
void frIR_arrset(frIR *ir, frIRValue *array, frIRValue *index,
                 frIRValue *value);
frIRValue frIR_arrget(frIR *ir, frIRValue *array, frIRValue *index);

FR_HEADER_END

#endif
