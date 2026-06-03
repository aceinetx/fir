#ifndef FEATHER_IR_INSERT_POINT_H
#define FEATHER_IR_INSERT_POINT_H
#include "ir_block.h"
#include "ir_func.h"
#include "ir_inst.h"

FR_HEADER_BEGIN

typedef struct frInsertPoint {
  frIRBlock *block;
  frIRInst *inst;
  frIRFunc *func;
} frInsertPoint;

FR_HEADER_END

#endif
