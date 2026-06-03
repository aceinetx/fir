#ifndef FEATHER_IR_INST_H
#define FEATHER_IR_INST_H

#include "config.h"
#include "ir_type.h"
#include <stdint.h>
#include <stdio.h>

FR_HEADER_BEGIN

enum {
  FR_ALLOCA,
  FR_RET,
  FR_STORE,
  FR_LOAD,
  FR_CONST_INT,
  FR_CONST_FLOAT,
  FR_CONST_STR,
  FR_BINOP,
  FR_SELECT,
  FR_BR,
  FR_COND_BR,
  FR_CAST,
  FR_CALL,
  FR_ARRSET,
  FR_ARRGET,
};

enum {
  FR_BINOP_ADD,
  FR_BINOP_SUB,
  FR_BINOP_MUL,
  FR_BINOP_DIV,
  FR_BINOP_EQUAL,
  FR_BINOP_NEQUAL,
  FR_BINOP_LT,
  FR_BINOP_GT,
  FR_BINOP_LTE,
  FR_BINOP_GTE,
  FR_BINOP_AND,
  FR_BINOP_OR,
  FR_BINOP_XOR,
  FR_BINOP_SHL,
  FR_BINOP_SHR,
};

typedef struct frIRInst {
  union {
    struct {
      frIRType *type;
      char name[FR_IDENT_SIZE];
    } alloca;
    struct {
      const char *name;
    } ret;
    struct {
      const char *name_dest;
      const char *name_src;
    } store;
    struct {
      frIRType *type;
      char name[FR_IDENT_SIZE];
      const char *source;
    } load;
    struct {
      char name[FR_IDENT_SIZE];
      frIRType *type;
      long number;
    } const_int;
    struct {
      char name[FR_IDENT_SIZE];
      frIRType *type;
      double number;
    } const_float;
    struct {
      char name[FR_IDENT_SIZE];
      const char *str;
    } const_str;
    struct {
      frIRType *type;
      char name[FR_IDENT_SIZE];
      const char *name_lhs;
      const char *name_rhs;
      uint8_t op;
    } binop;
    struct {
      frIRType *type;
      char name[FR_IDENT_SIZE];
      const char *name_test;
      const char *name_true;
      const char *name_false;
    } select;
    struct {
      const char *block_name;
    } br;
    struct {
      const char *name_value;
      const char *true_block;
      const char *false_block;
    } cond_br;
    struct {
      char name[FR_IDENT_SIZE];
      const char *name_cast;
      frIRType *type;
    } cast;
    struct {
      const char *args[FR_FUNC_MAX_ARGS];
      frIRType *ret_type;
      char name[FR_IDENT_SIZE];
      const char *func;
    } call;
    struct {
      const char *array;
      const char *index;
      const char *value;
    } arrset;
    struct {
      frIRType *type;
      char name[FR_IDENT_SIZE];
      const char *array;
      const char *index;
    } arrget;
  } v;
  struct frIRBlock *block;
  struct frIRInst *next;
  uint8_t type;
} frIRInst;

void frIRInst_print(frIRInst *inst, FILE *out);

FR_HEADER_END

#endif
