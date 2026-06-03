#ifndef FEATHER_IR_VALUE_H
#define FEATHER_IR_VALUE_H
#include "ir_type.h"

FR_HEADER_BEGIN

typedef struct frIRValue {
  const char *name;
  frIRType *type;
} frIRValue;

FR_HEADER_END

#endif
