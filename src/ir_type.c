#include "fir/ir_type.h"
#include <stdlib.h>
#include <string.h>

frIRType frIRType_i8 = {.unit = FR_TYPE_BUILTIN, .v.builtin.repr = "int8_t"};
frIRType frIRType_i16 = {.unit = FR_TYPE_BUILTIN, .v.builtin.repr = "int16_t"};
frIRType frIRType_i32 = {.unit = FR_TYPE_BUILTIN, .v.builtin.repr = "int32_t"};
frIRType frIRType_i64 = {.unit = FR_TYPE_BUILTIN, .v.builtin.repr = "int64_t"};
frIRType frIRType_u0 = {.unit = FR_TYPE_BUILTIN, .v.builtin.repr = "void"};
frIRType frIRType_u1 = {.unit = FR_TYPE_BUILTIN, .v.builtin.repr = "uint8_t"};
frIRType frIRType_u8 = {.unit = FR_TYPE_BUILTIN, .v.builtin.repr = "uint8_t"};
frIRType frIRType_u16 = {.unit = FR_TYPE_BUILTIN, .v.builtin.repr = "uint16_t"};
frIRType frIRType_u32 = {.unit = FR_TYPE_BUILTIN, .v.builtin.repr = "uint32_t"};
frIRType frIRType_u64 = {.unit = FR_TYPE_BUILTIN, .v.builtin.repr = "uint64_t"};
frIRType frIRType_f32 = {.unit = FR_TYPE_BUILTIN, .v.builtin.repr = "float"};
frIRType frIRType_f64 = {.unit = FR_TYPE_BUILTIN, .v.builtin.repr = "double"};
frIRType frIRType_str = {.unit = FR_TYPE_PTR, .v.ptr.pointee = &frIRType_u8};

void frIRType_print(frIRType *type, FILE *out, const char *ident) {
  if (!ident)
    ident = "";

  switch (type->unit) {
  case FR_TYPE_BUILTIN: {
    fprintf(out, "%s", type->v.builtin.repr);
    if (ident)
      fprintf(out, " %s", ident);
  } break;
  case FR_TYPE_PTR: {
    char *buf = malloc(strlen(ident) + 16);

    if (type->v.ptr.pointee->unit == FR_TYPE_ARR)
      sprintf(buf, "(*%s)", ident);
    else
      sprintf(buf, "*%s", ident);

    frIRType_print(type->v.ptr.pointee, out, buf);

    free(buf);
    break;
  }
  case FR_TYPE_ARR: {
    char *buf = malloc(strlen(ident) + 16);

    if (type->v.array.size == 0)
      sprintf(buf, "%s[]", ident);
    else
      sprintf(buf, "%s[%lu]", ident, type->v.array.size);

    frIRType_print(type->v.array.type, out, buf);

    free(buf);
    break;
  }
  case FR_TYPE_FUNCPTR: {
    char *buf = malloc(strlen(ident) + 16);

    sprintf(buf, "(*%s)()", ident);
    frIRType_print(type->v.funcptr.ret_type, out, buf);

    free(buf);
    break;
  }
  default:
    break;
  }
}
