#pragma once
#include "fir/ir_func.h"

namespace fir {
class Func {
public:
  Func(frIRFunc *func);

  operator frIRFunc *();

  void header_print(FILE *out);
  void print(FILE *out);

private:
  frIRFunc *func;
};
} // namespace fir
