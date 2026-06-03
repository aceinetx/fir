#pragma once
#include "fir/ir.h"

namespace fir {
class IR {
public:
  IR();
  IR(const IR &) = delete;
  IR &operator=(IR &) = delete;
  IR(IR &&);
  IR &operator=(IR &&);
  ~IR();

private:
  frIR ir;
};
} // namespace fir
