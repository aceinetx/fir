#pragma once
#include "fir/ir.h"

namespace fir {
class IR {
public:
  IR();
  IR(const IR &) = delete;
  IR &operator=(IR &) = delete;
  IR(IR &&) noexcept;
  IR &operator=(IR &&) noexcept;
  ~IR();

  void print(FILE *out);

private:
  frIR ir;
};
} // namespace fir
