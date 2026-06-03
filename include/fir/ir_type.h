#ifndef FEATHER_IR_TYPE_H
#define FEATHER_IR_TYPE_H

#include "config.h"
#include <stdio.h>

FR_HEADER_BEGIN

enum { FR_TYPE_BUILTIN, FR_TYPE_PTR, FR_TYPE_ARR, FR_TYPE_FUNCPTR };

typedef struct frIRType {
  int unit;
  union {
    struct {
      const char *repr;
    } builtin;

    struct {
      struct frIRType *pointee;
    } ptr;

    struct {
      struct frIRType *type;
      size_t size;
    } array;

    struct {
      struct frIRType *ret_type;
      struct frIRType *arg_types[FR_FUNC_MAX_ARGS];
    } funcptr;
  } v;
} frIRType;

void frIRType_print(frIRType *type, FILE *out, const char *ident);

extern frIRType frIRType_i8;
extern frIRType frIRType_i16;
extern frIRType frIRType_i32;
extern frIRType frIRType_i64;
extern frIRType frIRType_u0;
extern frIRType frIRType_u1;
extern frIRType frIRType_u8;
extern frIRType frIRType_u16;
extern frIRType frIRType_u32;
extern frIRType frIRType_u64;
extern frIRType frIRType_f32;
extern frIRType frIRType_f64;
extern frIRType frIRType_str;

FR_HEADER_END

#endif
